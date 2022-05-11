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
                             usize n) const {
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
    if (key.empty()) {
      error_ = true;
      return;
    }

    auto n = key.size();
    idx_offset = idx_offset % n;

#define QMC_GET_VALUE_AT_IDX(IDX) (GetCacheIndex(key, idx_offset, IDX, n))
    for (usize i = 0; i < kStaticCipherPageSize; i++) {
      cache_[i] = QMC_GET_VALUE_AT_IDX(i);
    }
    value_page_one_ = QMC_GET_VALUE_AT_IDX(kStaticCipherPageSize);
#undef QMC_GET_VALUE_AT_IDX
  }

  virtual const Str GetName() const override { return name_; };

 private:
  u8 value_page_one_;
  QMCv1Cache cache_;

  bool Write(const u8* in, usize len) override {
    if (error_) return false;

    usize out_size = buf_out_.size();
    buf_out_.resize(out_size + len);
    auto p_out = &buf_out_.at(out_size);

    for (usize i = 0; i < len; i++, offset_++) {
      if (offset_ == kStaticCipherPageSize) {
        p_out[i] = in[i] ^ value_page_one_;
      } else {
        p_out[i] = in[i] ^ cache_[offset_ % kStaticCipherPageSize];
      }
    }

    return true;
  }

  bool End() override { return !error_; }
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
