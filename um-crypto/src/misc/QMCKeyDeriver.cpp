#include "um-crypto/misc/QMCKeyDeriver.h"
#include "um-crypto/utils/base64.h"

#include "tc_tea/tc_tea.h"

#include <algorithm>
#include <cmath>

namespace umc::misc::tencent {

namespace detail {
constexpr auto DecryptTencentTEA = tc_tea::cbc::Decrypt;
constexpr auto EncryptTencentTEA = tc_tea::cbc::Encrypt;

const static std::string enc_v2_prefix = std::string("QQMusic EncV2,Key:");

class QMCKeyDeriverImpl : public QMCKeyDeriver {
 private:
  u8 seed_;
  QMCEncV2Stage1Key enc_v2_stage1_key_;
  QMCEncV2Stage2Key enc_v2_stage2_key_;

 public:
  QMCKeyDeriverImpl(u8 seed,
                    QMCEncV2Stage1Key enc_v2_stage1_key,
                    QMCEncV2Stage2Key enc_v2_stage2_key)
      : seed_(seed),
        enc_v2_stage1_key_(enc_v2_stage1_key),
        enc_v2_stage2_key_(enc_v2_stage2_key) {}

  bool FromEKey(Vec<u8>& out, const Str ekey_b64) const override {
    Vec<u8> ekey = utils::Base64Decode(ekey_b64);
    return FromEKey(out, ekey);
  }

  bool FromEKey(Vec<u8>& out, const Vec<u8> input_ekey) const override {
    Vec<u8> ekey(input_ekey);
    if (memcmp(ekey.data(), enc_v2_prefix.data(), enc_v2_prefix.size()) == 0) {
      ekey.erase(ekey.begin(), ekey.begin() + enc_v2_prefix.size());
      if (!DecodeEncV2Key(ekey)) {
        out.resize(0);
        return false;
      }
    }

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

  bool ToEKey(Vec<u8>& out, const Vec<u8> key) const override {
    auto& ekey = out;
    ekey.resize(8 + tc_tea::cbc::GetEncryptedSize(key.size()));
    std::copy_n(key.begin(), 8, ekey.begin());

    auto tea_key = DeriveTEAKey(ekey);
    usize cipher_len;
    if (!EncryptTencentTEA(&ekey[8], cipher_len, &key[8], key.size() - 8,
                           tea_key.data())) {
      ekey.resize(0);
      return false;
    }
    ekey.resize(8 + cipher_len);
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

  inline bool DecodeEncV2Key(Vec<u8>& key) const {
    Vec<u8> decode_key_1(key.size());
    Vec<u8> decode_key_2(key.size());

    {
      umc::usize len = decode_key_1.size();
      if (!DecryptTencentTEA(decode_key_1.data(), len, key.data(), key.size(),
                             enc_v2_stage1_key_.data())) {
        return false;
      }
      decode_key_1.resize(len);
    }

    {
      umc::usize len = decode_key_2.size();
      if (!DecryptTencentTEA(decode_key_2.data(), len, decode_key_1.data(),
                             decode_key_1.size(), enc_v2_stage2_key_.data())) {
        return false;
      }
      decode_key_2.resize(len);
    }

    key = utils::Base64Decode(Str(decode_key_2.begin(), decode_key_2.end()));
    return true;
  }
};

}  // namespace detail

std::unique_ptr<QMCKeyDeriver> QMCKeyDeriver::Create(
    u8 seed,
    QMCEncV2Stage1Key enc_v2_stage1_key,
    QMCEncV2Stage2Key enc_v2_stage2_key) {
  return std::make_unique<detail::QMCKeyDeriverImpl>(seed, enc_v2_stage1_key,
                                                     enc_v2_stage2_key);
}

}  // namespace umc::misc::tencent
