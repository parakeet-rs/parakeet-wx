#include "um-crypto/decryption/DecryptionManager.h"
#include "um-crypto/utils/DetectAudioType.h"

namespace umc::decryption {

namespace detail {

class DecryptionManagerImpl : public DecryptionManager {
 private:
  config::DecryptionConfig config_;

 public:
  DecryptionManagerImpl() {}
  const config::DecryptionConfig& GetConfig() const override { return config_; }
  void SetConfig(config::DecryptionConfig& config) override {
    config_ = config;
  }

  Vec<std::unique_ptr<DetectionResult>> DetectDecryptors(
      const DetectionBuffer& header,
      const DetectionBuffer& footer,
      bool remove_unknown_format) {
    using utils::AudioType;

    Vec<std::unique_ptr<DetectionResult>> result;

    for (auto& decryptor : GetDecryptorsFromConfig()) {
      auto footer_len = decryptor->InitWithFileFooter(footer);
      if (decryptor->InErrorState()) continue;
      if (!decryptor->Write(header.data(), header.size())) continue;
      if (decryptor->InErrorState()) continue;

      usize decrypted_size = decryptor->GetOutputSize();
      Vec<u8> decrypted_peek(decrypted_size);
      decryptor->Peek(decrypted_peek.data(), decrypted_size);

      auto audio_type = utils::DetectAudioType(decrypted_peek);
      if (remove_unknown_format && audio_type == AudioType::kUnknownType) {
        continue;
      }

      auto item = std::make_unique<DetectionResult>();
      item->decryptor = std::move(decryptor);
      item->footer_discard_len = footer_len;
      item->audio_type = audio_type;
      item->audio_ext = utils::GetAudioTypeExtension(audio_type);
      result.push_back(std::move(item));
    }

    std::sort(result.begin(), result.end(),
              [](std::unique_ptr<DetectionResult>& left,
                 std::unique_ptr<DetectionResult>& right) -> bool {
                // Prefer audio_type with higher rank;
                //   lossless > lossy > unknown (bin)
                return left->audio_type > right->audio_type;
              });

    return result;
  };

 private:
  inline Vec<std::unique_ptr<DecryptionStream>> GetDecryptorsFromConfig() {
    const auto& c = config_;
    Vec<std::unique_ptr<DecryptionStream>> result;

    // Add kugou ciphers
    result.push_back(kugou::KugouFileLoader::Create(c.kugou.t1, c.kugou.t2,
                                                    c.kugou.v2, true));
    result.push_back(kugou::KugouFileLoader::Create(
        c.kugou.t1, c.kugou.t2, c.kugou.v2, c.kugou.vpr_key, true));

    // Add kuwo ciphers
    result.push_back(kuwo::KuwoFileLoader::Create(c.kuwo.key));

    // Add Netease ciphers
    result.push_back(netease::NCMFileLoader::Create(c.netease.key));

    // Add Joox
    result.push_back(
        tencent::JooxFileLoader::Create(c.joox.install_uuid, c.joox.salt));

    // Add QMCv1 (static)
    result.push_back(tencent::QMCv1Loader::Create(c.qmc.static_cipher_key));

    // Add QMCv2 (map)
    auto qmc_footer_parser =
        std::shared_ptr(umc::misc::tencent::QMCFooterParser::Create(
            umc::misc::tencent::QMCKeyDeriver::Create(c.qmc.ekey_seed)));
    result.push_back(tencent::QMCv1Loader::Create(qmc_footer_parser));
    result.push_back(tencent::QMCv2Loader::Create(qmc_footer_parser));

    // TODO: Add QMCv2 (RC4)

    // Add Xiami
    result.push_back(xiami::XiamiFileLoader::Create());

    // Add Ximalaya
    result.push_back(ximalaya::XimalayaFileLoader::Create(
        c.ximalaya.x2m_content_key, c.ximalaya.x2m_scramble_table));

    result.push_back(ximalaya::XimalayaFileLoader::Create(
        c.ximalaya.x3m_content_key, c.ximalaya.x3m_scramble_table));

    return result;
  }
};

}  // namespace detail

std::unique_ptr<DecryptionManager> DecryptionManager::Create() {
  return std::make_unique<detail::DecryptionManagerImpl>();
}

}  // namespace umc::decryption
