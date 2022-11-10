#include "um-crypto/decryption/DecryptionManager.h"
#include "um-crypto/utils/DetectAudioType.h"

#include <sstream>

namespace umc::decryption {

namespace detail {

class DecryptionManagerImpl : public DecryptionManager {
 private:
  config::DecryptionConfig config_;

 public:
  DecryptionManagerImpl() {}
  const config::DecryptionConfig& GetConfig() const override { return config_; }
  void SetConfig(config::DecryptionConfig& config) override { config_ = config; }

  std::vector<std::unique_ptr<DetectionResult>> DetectDecryptors(const DetectionBuffer& header,
                                                                 const DetectionBuffer& footer,
                                                                 bool remove_unknown_format) {
    std::stringstream ss;
    ss.write(reinterpret_cast<const char*>(header.data()), header.size());

    // add some padding
    ss.write(std::string(header.size(), 0).c_str(), header.size());

    ss.write(reinterpret_cast<const char*>(footer.data()), footer.size());
    return DetectDecryptors(ss, remove_unknown_format);
  };

  std::vector<std::unique_ptr<DetectionResult>> DetectDecryptors(std::istream& stream,
                                                                 bool remove_unknown_format = true) override {
    using utils::AudioType;

    std::vector<std::unique_ptr<DetectionResult>> result;

    std::vector<u8> header(kDetectionBufferLen);  // initial header size.
    DetectionBuffer footer;
    stream.seekg(0, std::ios::beg);
    stream.read(reinterpret_cast<char*>(header.data()), kDetectionBufferLen);
    if (stream.gcount() < kDetectionBufferLen) {
      // buffer too small
      return result;
    }
    stream.seekg(0, std::ios::end);
    std::size_t file_len = stream.tellg();
    stream.seekg(file_len - kDetectionBufferLen, std::ios::beg);
    stream.read(reinterpret_cast<char*>(footer.data()), kDetectionBufferLen);
    if (stream.gcount() < kDetectionBufferLen) {
      // buffer too small
      return result;
    }
    std::size_t bytes_left = file_len - kDetectionBufferLen;
    stream.seekg(kDetectionBufferLen, std::ios::beg);

    for (auto& decryptor : GetDecryptorsFromConfig()) {
      auto name = decryptor->GetName();
      auto footer_len = decryptor->InitWithFileFooter(footer);
      if (decryptor->InErrorState()) continue;

      // We want to decrypt at least `kDetectionBufferLen` bytes of data.
      bool bad = false;
      auto p_in = header.data();
      while (bytes_left > 0 && decryptor->GetOutputSize() < kDetectionBufferLen) {
        std::size_t bytes_left_in_buffer = header.data() + header.size() - p_in;

        // Should we feed more data?
        if (bytes_left_in_buffer == 0) {
          std::size_t bytes_to_read = std::min(kDetectionBufferLen, bytes_left);
          std::size_t pos = header.size();
          header.resize(pos + bytes_to_read);

          p_in = &header[pos];
          stream.read(reinterpret_cast<char*>(p_in), bytes_to_read);
          bytes_left_in_buffer = stream.gcount();
          if (bytes_left_in_buffer < bytes_to_read) {
            // io error?
            bad = true;
            break;
          }

          bytes_left -= bytes_to_read;
        }

        std::size_t bytes_written = std::min(kDetectionBufferLen, bytes_left_in_buffer);
        if (!decryptor->Write(p_in, bytes_written) || decryptor->InErrorState()) {
          // decryption error?
          bad = true;
          break;
        }

        bytes_left_in_buffer -= bytes_written;
        p_in += bytes_written;
      }

      if (bad) continue;

      std::size_t decrypted_size = decryptor->GetOutputSize();
      std::vector<u8> decrypted_peek(decrypted_size);
      decryptor->Peek(decrypted_peek.data(), decrypted_size);

      auto audio_type = utils::DetectAudioType(decrypted_peek);
      if (remove_unknown_format && audio_type == AudioType::kUnknownType) {
        continue;
      }

      auto item = std::make_unique<DetectionResult>();
      item->decryptor = std::move(decryptor);
      item->header_discard_len = p_in - header.data();
      item->footer_discard_len = footer_len;
      item->audio_type = audio_type;
      item->audio_ext = utils::GetAudioTypeExtension(audio_type);
      result.push_back(std::move(item));
    }

    std::sort(result.begin(), result.end(),
              [](std::unique_ptr<DetectionResult>& left, std::unique_ptr<DetectionResult>& right) -> bool {
                // Prefer audio_type with higher rank;
                //   lossless > lossy > unknown (bin)
                return left->audio_type > right->audio_type;
              });

    return result;
  };

 private:
  inline std::vector<std::unique_ptr<DecryptionStream>> GetDecryptorsFromConfig() {
    const auto& c = config_;
    std::vector<std::unique_ptr<DecryptionStream>> result;

    // Add kugou ciphers
    kugou::KugouSlotKeys kgm_slot_keys;
    kgm_slot_keys[1] = c.kugou.slot_key_1;
    result.push_back(kugou::KugouFileLoader::Create(kgm_slot_keys, c.kugou.v4_slot_key_expansion_table,
                                                    c.kugou.v4_file_key_expansion_table));

    // Add kuwo ciphers
    result.push_back(kuwo::KuwoFileLoader::Create(c.kuwo.key));

    // Add Netease ciphers
    result.push_back(netease::NCMFileLoader::Create(c.netease.key));

    // Add Joox
    result.push_back(tencent::JooxFileLoader::Create(c.joox.install_uuid, c.joox.salt));

    // Add QMCv1 (static)
    result.push_back(tencent::QMCv1Loader::Create(c.qmc.static_cipher_key));

    // Add QMCv2 (map)
    auto qmc_footer_parser = std::shared_ptr(umc::misc::tencent::QMCFooterParser::Create(
        umc::misc::tencent::QMCKeyDeriver::Create(c.qmc.ekey_seed, c.qmc.enc_v2_stage1_key, c.qmc.enc_v2_stage2_key)));
    result.push_back(tencent::QMCv1Loader::Create(qmc_footer_parser));

    // Add QMCv2 (RC4)
    result.push_back(tencent::QMCv2Loader::Create(qmc_footer_parser));

    // Add Xiami
    result.push_back(xiami::XiamiFileLoader::Create());

    // Add Ximalaya
    result.push_back(ximalaya::XimalayaFileLoader::Create(c.ximalaya.x2m_content_key, c.ximalaya.x2m_scramble_table));

    result.push_back(ximalaya::XimalayaFileLoader::Create(c.ximalaya.x3m_content_key, c.ximalaya.x3m_scramble_table));

    return result;
  }
};

}  // namespace detail

std::unique_ptr<DecryptionManager> DecryptionManager::Create() {
  return std::make_unique<detail::DecryptionManagerImpl>();
}

}  // namespace umc::decryption
