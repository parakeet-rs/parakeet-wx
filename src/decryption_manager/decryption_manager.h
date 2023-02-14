#pragma once

#include "config/AppConfigTypes.h"
#include "parakeet-crypto/ITransformer.h"
#include <fstream>
#include <memory>
#include <optional>
#include <utility>

namespace parakeet_wx::decryption_manager
{

using config::AppConfig;
using parakeet_crypto::ITransformer;

struct TransformerFindResult
{
    std::shared_ptr<ITransformer> transformer;
    const char *ext;
};

class DecryptionManager
{
  private:
    std::vector<std::shared_ptr<ITransformer>> transformers_{};

  public:
    DecryptionManager() = default;
    void SetConfig(const AppConfig &config);
    std::optional<TransformerFindResult> FindDecryptionTransformer(std::ifstream &ifs);
};

} // namespace parakeet_wx::decryption_manager
