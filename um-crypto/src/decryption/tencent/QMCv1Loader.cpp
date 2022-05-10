#include "um-crypto/decryption/tencent/QMCv1Loader.h"
#include "um-crypto/endian.h"
#include "um-crypto/utils/StringHelper.h"

#include <cassert>

namespace umc::decryption::tencent {

// Private implementation

namespace detail {

constexpr usize kStaticCipherPageSize = 0x7fff;
typedef Arr<u8, kStaticCipherPageSize> QMCv1Cache;

template <bool RotateValueWithIndex>
class QMCv1LoaderImpl : public QMCv1Loader {
 private:
  inline usize GetCacheIndex(const QMCv1Key& key,
                             usize idx_offset,
                             usize i,
                             usize n) {
    usize index = (i * i + idx_offset) % n;

    if constexpr (RotateValueWithIndex) {
      u8 v = key[index];
      usize shift = (index + 4) & 0b0111;
      return (v << shift) | (v >> shift);
    }

    return key[index];
  }

  Str name_;

 public:
  QMCv1LoaderImpl(const QMCv1Key& key,
                  usize idx_offset,
                  const char* subtype_name) {
    name_ = utils::Format("QMCv1(%s)", subtype_name);

#define QMC_GET_VALUE_AT_IDX(IDX) (GetCacheIndex(key, idx_offset, IDX, n))
    auto n = key.size();
    idx_offset = idx_offset % n;

    for (usize i = 0; i < kStaticCipherPageSize; i++) {
      cache_[i] = QMC_GET_VALUE_AT_IDX(i);
    }
    value_page_one_ = QMC_GET_VALUE_AT_IDX(kStaticCipherPageSize);
    value_page_other_ = QMC_GET_VALUE_AT_IDX(0);
#undef QMC_GET_VALUE_AT_IDX
  }

  virtual const Str GetName() const override { return name_; };

 private:
  u8 value_page_one_;
  u8 value_page_other_;
  QMCv1Cache cache_;

  bool Write(const u8* in, usize len) override {
    while (ReadBlock(in, len, kStaticCipherPageSize)) {
      DecryptBlock();
    }

    assert(len == 0);
    return true;
  }

  bool End() override {
    DecryptBlock();
    return true;
  }

  bool DecryptBlock() {
    usize len = buf_in_.size();

    u8* p_in = buf_in_.data();

    if (len > 0) {
      // Offset starts at (offset_ - len)
      *p_in ^= ((offset_ - len) == kStaticCipherPageSize) ? value_page_one_
                                                          : value_page_other_;
      for (usize i = 1; i < len; i++) {
        p_in[i] ^= cache_[i];
      }
    }

    buf_out_.insert(buf_out_.end(), buf_in_.begin(), buf_in_.end());
    buf_in_.resize(0);

    return true;
  }
};

}  // namespace detail

// Public interface

std::unique_ptr<QMCv1Loader> QMCv1Loader::Create(const QMCv1Key& key,
                                                 QMCv1Type type) {
  if (type == QMCv1Type::kStaticCipher) {
    return std::make_unique<detail::QMCv1LoaderImpl<false>>(key, 80923,
                                                            "static");
  } else if (type == QMCv1Type::kMapCipher) {
    return std::make_unique<detail::QMCv1LoaderImpl<true>>(key, 71214, "map");
  } else {
    throw std::invalid_argument("Unknown type for QMCv1");
  }
}

}  // namespace umc::decryption::tencent
