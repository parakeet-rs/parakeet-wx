#pragma once

#include "kugou/KugouFileLoader.h"
#include "kuwo/KuwoFileLoader.h"
#include "tencent/JooxFileLoader.h"
#include "tencent/QMCv1Loader.h"
#include "tencent/QMCv2Loader.h"
#include "xiami/XiamiFileLoader.h"
#include "ximalaya/XimalayaFileLoader.h"

#include "um-crypto/utils/AudioTypes.h"

namespace umc::decryption {

namespace config {

struct KugouConfig {
  kugou::KugouInternalTable t1;
  kugou::KugouInternalTable t2;
  kugou::KugouInternalTable v2;
  kugou::KugouVPRKey vpr_key;
};

struct KuwoConfig {
  kuwo::KuwoKey key;
};

struct JooxConfig {
  Str install_uuid;
  tencent::JooxSalt salt;
};

struct QMCConfig {
  u8 ekey_seed;
  tencent::QMCv1Key static_cipher_key;
};

struct XimalayaConfig {
  ximalaya::X2MContentKey x2m_content_key;
  ximalaya::ScrambleTable x2m_scramble_table;
  ximalaya::X3MContentKey x3m_content_key;
  ximalaya::ScrambleTable x3m_scramble_table;
};

struct DecryptionConfig {
  KugouConfig kugou;
  KuwoConfig kuwo;
  JooxConfig joox;
  QMCConfig qmc;
  XimalayaConfig ximalaya;
};

}  // namespace config

struct DetectionResult {
  /**
   * @brief Number of bytes to preserve, i.e. data that should be discarded.
   * This is usually the padding that was used by the detection.
   */
  usize footer_discard_len;
  utils::AudioType audio_type;
  Str audio_ext;
  std::unique_ptr<DecryptionStream> decryptor;
};

class DecryptionManager {
 public:
  virtual const config::DecryptionConfig& GetConfig() const = 0;
  virtual void SetConfig(config::DecryptionConfig& config) = 0;

  /**
   * @brief Get a list of detected decryptor.
   * Header will be supplied to the decryptor,
   *   feed decryptor with rest of the file.
   *
   * @param header File header
   * @param footer File footer
   * @return Vec<std::unique_ptr<DetectionResult>>
   */
  virtual Vec<std::unique_ptr<DetectionResult>> DetectDecryptors(
      const DetectionBuffer& header,
      const DetectionBuffer& footer) = 0;

  /**
   * @brief Get the first working decryptor.
   * Header will be supplied to the decryptor,
   *   feed decryptor with rest of the file.
   *
   * @param header
   * @param footer
   * @return std::unique_ptr<DetectionResult>
   */
  std::unique_ptr<DetectionResult> DetectDecryptor(
      const DetectionBuffer& header,
      const DetectionBuffer& footer) {
    auto result = DetectDecryptors(header, footer);
    if (result.size() > 0) {
      return std::move(result[0]);
    }
    return nullptr;
  }

  static std::unique_ptr<DecryptionManager> Create();
};

}  // namespace umc::decryption
