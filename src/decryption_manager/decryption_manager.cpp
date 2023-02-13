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

    transformers_ = std::vector<std::shared_ptr<ITransformer>>({
        // Transformer that has a header lookup
        CreateXiamiDecryptionTransformer(),
        CreateJooxDecryptionV4Transformer(config.joox),
        CreateNeteaseNCMDecryptionTransformer(config.netease.key.data()),
        CreateKuwoDecryptionTransformer(config.kuwo.key.data()),
        CreateKGMDecryptionTransformer(config.kugou),

        // Transformer that has a footer lookup
        CreateQMC2DecryptionTransformer(qmc2::CreateQMC2FooterParser(
            config.qmc.qmc2.seed, config.qmc.qmc2.enc_v2_key1.data(), config.qmc.qmc2.enc_v2_key2.data())),

        // Transformer that does not contain any identification...
        CreateQMC1StaticDecryptionTransformer(config.qmc.qmc1.key.data(), config.qmc.qmc1.key.size()),
    });

    auto add_ximalaya_variant = [&](const parakeet_wx::config::XimalayaVariantConfig &config) {
        if (auto scramble_key = xmly::CreateScrambleKey(config.init_value, config.step_value))
        {
            transformers_.push_back(CreateXimalayaDecryptionTransformer(*scramble_key, config.content_key));
        }
    };
    add_ximalaya_variant(config.ximalaya.x2m);
    add_ximalaya_variant(config.ximalaya.x3m);
}

std::optional<TransformerFindResult> DecryptionManager::FindDecryptionTransformer(std::ifstream &ifs)
{
    InputFileStream stream{ifs};
    for (auto &transformer : transformers_)
    {
        stream.Seek(0, SeekDirection::SEEK_FILE_BEGIN);
        AudioDetectionDestination audio_sink{};
        auto transform_result = transformer->Transform(&audio_sink, &stream);
#if !NDEBUG
        std::cerr << transformer->GetName() << ": "
                  << "transform_result(" << (int)transform_result << "), "
                  << "ifs.state(" << (int)ifs.rdstate() << ")" << std::endl;
#endif
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
