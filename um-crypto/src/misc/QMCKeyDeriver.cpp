#include "um-crypto/misc/QMCKeyDeriver.h"
#include "um-crypto/utils/base64.h"

#include "tc_tea/tc_tea.h"

#include <algorithm>
#include <cmath>

namespace umc::misc::tencent {

namespace detail {
constexpr auto DecryptTencentTEA = tc_tea::cbc::Decrypt;

class QMCKeyDeriverImpl : public QMCKeyDeriver {
 private:
  u8 seed_;

 public:
  QMCKeyDeriverImpl(u8 seed) : seed_(seed) {}

  bool FromEKey(Vec<u8>& out, const Str ekey_b64) const override {
    Vec<u8> ekey = utils::Base64Decode(ekey_b64);
    return FromEKey(out, ekey);
  }

  bool FromEKey(Vec<u8>& out, const Vec<u8> ekey) const override {
    const auto ekey_len = ekey.size();

    if (ekey_len < 8) {
      out.resize(0);
      return false;
    }

    auto tea_key = DeriveTEAKey(ekey);
    out.resize(ekey_len);
    std::copy_n(ekey.begin(), 8u, out.begin());

    auto data_len = ekey_len - 8;
    auto p_key = tea_key.data();

    size_t out_len;
    if (!DecryptTencentTEA(&out[8], out_len, &ekey[8], data_len, p_key)) {
      out.resize(0);
      return false;
    };

    out.resize(8 + out_len);
    return true;
  }

 private:
  inline void MakeSimpleKey(Vec<u8>& out) const {
    double seed = static_cast<double>(seed_);
    for (auto& byte : out) {
      byte = static_cast<uint8_t>(fabs(tan(seed)) * 100.0);
      seed += 0.1;
    }
  }

  inline Vec<u8> DeriveTEAKey(const Vec<u8> ekey) const {
    Vec<u8> tea_key(16);
    Vec<u8> simple_key(8);
    MakeSimpleKey(simple_key);

    for (int i = 0; i < 16; i += 2) {
      tea_key[i + 0] = simple_key[i / 2];
      tea_key[i + 1] = ekey[i / 2];
    }

    return tea_key;
  }
};

}  // namespace detail

std::unique_ptr<QMCKeyDeriver> QMCKeyDeriver::Create(u8 seed) {
  return std::make_unique<detail::QMCKeyDeriverImpl>(seed);
}

}  // namespace umc::misc::tencent
