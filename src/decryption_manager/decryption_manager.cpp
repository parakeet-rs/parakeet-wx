#include "decryption_manager.h"
#include "audio_detection_destination.h"

#include "parakeet-crypto/qmc2/footer_parser.h"
#include "parakeet-crypto/transformer/joox.h"
#include "parakeet-crypto/transformer/kgm.h"
#include "parakeet-crypto/transformer/kuwo.h"
#include "parakeet-crypto/transformer/ncm.h"

#include <parakeet-crypto/IStream.h>
#include <parakeet-crypto/ITransformer.h>
#include <parakeet-crypto/transformer/qmc.h>
#include <parakeet-crypto/transformer/xiami.h>
#include <parakeet-crypto/transformer/ximalaya.h>
#include <parakeet-crypto/xmly/scramble_key.h>

#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace parakeet_wx::decryption_manager
{

using namespace parakeet_crypto;

void DecryptionManager::SetConfig(const AppConfig &config)
{
    using namespace parakeet_crypto::transformer;

    auto x2m_scramble_key = xmly::CreateScrambleKey(config.ximalaya.x2m.init_value, config.ximalaya.x2m.step_value,
                                                    kXimalayaScrambleKeyLen);
    auto x3m_scramble_key = xmly::CreateScrambleKey(config.ximalaya.x3m.init_value, config.ximalaya.x3m.step_value,
                                                    kXimalayaScrambleKeyLen);

    transformers_ = std::vector<std::shared_ptr<ITransformer>>({
        CreateXimalayaDecryptionTransformer(x2m_scramble_key.data(), config.ximalaya.x2m.content_key.data(),
                                            config.ximalaya.x2m.content_key.size()),
        CreateXimalayaDecryptionTransformer(x3m_scramble_key.data(), config.ximalaya.x3m.content_key.data(),
                                            config.ximalaya.x3m.content_key.size()),
        CreateJooxDecryptionV4Transformer(config.joox),
        CreateQMC1StaticDecryptionTransformer(config.qmc.qmc1.key.data(), config.qmc.qmc1.key.size()),
        CreateQMC2DecryptionTransformer(qmc2::CreateQMC2FooterParser(
            config.qmc.qmc2.seed, config.qmc.qmc2.enc_v2_key1.data(), config.qmc.qmc2.enc_v2_key2.data())),
        CreateKGMDecryptionTransformer(config.kugou),            //
        CreateXiamiDecryptionTransformer(),                      //
        CreateKuwoDecryptionTransformer(config.kuwo.key.data()), //
        CreateNeteaseNCMDecryptionTransformer(config.netease.key.data()),
    });
}

std::optional<TransformerFindResult> DecryptionManager::FindDecryptionTransformer(std::ifstream &ifs)
{
    InputFileStream stream{ifs};
    for (auto &transformer : transformers_)
    {
        stream.Seek(0, SeekDirection::SEEK_FILE_BEGIN);
        AudioDetectionDestination audio_sink{};
        auto _transform_result = transformer->Transform(&audio_sink, &stream);
        // unused: _transform_result
        if (auto type = audio_sink.GetAudioType(); type != parakeet_audio::AudioType::kUnknownType)
        {
            stream.Seek(0, SeekDirection::SEEK_FILE_BEGIN);
            return std::make_optional<TransformerFindResult>({
                transformer,
                parakeet_audio::GetAudioTypeExtension(type),
            });
        }
    }
    stream.Seek(0, SeekDirection::SEEK_FILE_BEGIN);
    return {};
}

} // namespace parakeet_wx::decryption_manager
