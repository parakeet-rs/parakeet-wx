#pragma once

#include "../DecryptionStream.h"

namespace umc::decryption::tencent {

typedef Vec<u8> QMCv1Key;

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

class QMCv1Loader : public DecryptionStream {
 public:
  virtual const Str GetName() const override { return "QMCv1(static/map)"; };

  static std::unique_ptr<QMCv1Loader> Create(const QMCv1Key& key,
                                             QMCv1Type type);
};

}  // namespace umc::decryption::tencent
