#include "um-crypto/decryption/tencent/QMCv1Loader.h"
#include "um-crypto/endian.h"
#include "um-crypto/utils/StringHelper.h"

#include <cassert>

namespace umc::decryption::tencent {

// Private implementation

/**
 * @brief QMCv1 Encryption type.
 */
enum class QMCv1Type {
  /**
   * @brief
   * Used by WeYun, old QQ Music client (with extension e.g. `qmcflac`)
   * Old cipher with static keys.
   */
  kStaticCipher = 0,

  /**
   * @brief
   * Used by QQ Music client (with extension e.g. `mflac`).
   * Same cipher but with a different key for each file.
   * Key derivation parameter is different than {@link kStaticCipher}
   *
   * Do _not_ feed the file footer to this crypto.
   */
  kMapCipher,
};

namespace detail {

constexpr std::size_t kStaticCipherPageSize = 0x7fff;
typedef std::array<uint8_t, kStaticCipherPageSize> QMCv1Cache;

template <QMCv1Type Type>
class QMCv1LoaderImpl : public QMCv1Loader {
 private:
  inline std::size_t GetCacheIndex(const QMCv1Key& key, std::size_t idx_offset, std::size_t i, std::size_t n) const {
    std::size_t index = (i * i + idx_offset) % n;

    if constexpr (Type == QMCv1Type::kMapCipher) {
      uint8_t v = key[index];
      std::size_t shift = (index + 4) & 0b0111;
      return (v << shift) | (v >> shift);
    }

    return key[index];
  }

  std::string name_;
  std::size_t idx_offset_;

 public:
  QMCv1LoaderImpl(const QMCv1Key& key, std::size_t idx_offset) : idx_offset_(idx_offset) {
    const char* subtype = Type == QMCv1Type::kStaticCipher ? "static" : "map";
    name_ = utils::Format("QMCv1(%s)", subtype);

    if constexpr (Type == QMCv1Type::kStaticCipher) {
      SetKey(key);
    }
  }

  virtual const std::string GetName() const override { return name_; };

  inline void SetKey(const QMCv1Key& key) {
    if (key.empty()) {
      error_ = "key is empty.";
      return;
    }

    error_ = "";
    auto n = key.size();
    std::size_t idx_offset = idx_offset_ % n;

#define QMC_GET_VALUE_AT_IDX(IDX) (GetCacheIndex(key, idx_offset, IDX, n))
    for (std::size_t i = 0; i < kStaticCipherPageSize; i++) {
      cache_[i] = QMC_GET_VALUE_AT_IDX(i);
    }
    value_page_one_ = QMC_GET_VALUE_AT_IDX(kStaticCipherPageSize);
#undef QMC_GET_VALUE_AT_IDX
  }

  inline void SetFooterParser(std::shared_ptr<misc::tencent::QMCFooterParser> parser) {
    parser_ = parser;
  }

  virtual std::size_t InitWithFileFooter(const DetectionBuffer& buf) {
    if constexpr (Type == QMCv1Type::kStaticCipher) return 0;

    if (parser_) {
      auto parsed = parser_->Parse(buf.data(), buf.size());
      if (parsed && parsed->key.size() < 300) {
        // Error will be propagated within this method.
        SetKey(parsed->key);
        return parsed->eof_bytes_ignore;
      } else {
        error_ = "Not QMCv1";
        return 0;
      }
    }

    error_ = "QMC footer parser not set";
    return 0;
  }

 private:
  uint8_t value_page_one_;
  QMCv1Cache cache_;

  std::shared_ptr<misc::tencent::QMCFooterParser> parser_;

  bool Write(const uint8_t* in, std::size_t len) override {
    if (InErrorState()) return false;

    auto p_out = ExpandOutputBuffer(len);

    for (std::size_t i = 0; i < len; i++, offset_++) {
      if (offset_ == kStaticCipherPageSize) {
        p_out[i] = in[i] ^ value_page_one_;
      } else {
        p_out[i] = in[i] ^ cache_[offset_ % kStaticCipherPageSize];
      }
    }

    return true;
  }

  bool End() override {
    return !InErrorState();
  }
};

}  // namespace detail

// Public interface

std::unique_ptr<QMCv1Loader> QMCv1Loader::Create(const QMCv1Key& key) {
  return std::make_unique<detail::QMCv1LoaderImpl<QMCv1Type::kStaticCipher>>(key, 80923);
}

std::unique_ptr<QMCv1Loader> QMCv1Loader::Create(std::shared_ptr<misc::tencent::QMCFooterParser> parser) {
  auto cipher = std::make_unique<detail::QMCv1LoaderImpl<QMCv1Type::kMapCipher>>(QMCv1Key{}, 71214);
  cipher->SetFooterParser(parser);
  return cipher;
}

}  // namespace umc::decryption::tencent
