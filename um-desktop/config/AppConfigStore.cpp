#include "AppConfigStore.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

#include <um-crypto/utils/base64.h>

#include "../types.h"
#include "../utils/AppDataPath.h"
#include "../utils/JSONExtension.h"

#include <fstream>

namespace json = umd::utils::json;

namespace umd::config {

AppConfigStore* AppConfigStore::instance_ = new AppConfigStore();

AppConfigStore::AppConfigStore() {
  manager_ = umc::decryption::DecryptionManager::Create();
  config_file_path_ = umd::utils::GetUserDataDirectory() / "config.json5";
}

constexpr int kModeReadConfigFromJSON = 1;
constexpr int kModeSaveConfigToJSON = 2;

template <int MODE>
inline void JSONManipulate(AppConfig& config, rapidjson::Document& doc) {
  using namespace rapidjson;
  if (!doc.IsObject()) {
    doc.SetObject();
  }

#define BEGIN_MANIP_NAMESPACED_VALUE(PARENT, NAMESPACE, SUBKEY)           \
  {                                                                       \
    if (!PARENT.HasMember(#SUBKEY)) {                                     \
      PARENT.AddMember(#SUBKEY, Value().SetObject(), doc.GetAllocator()); \
    }                                                                     \
    auto& childConf = config.NAMESPACE.SUBKEY;                            \
    auto& childJSON = PARENT[#SUBKEY];                                    \
    if (!childJSON.IsObject()) {                                          \
      childJSON.SetObject();                                              \
    }
#define END_MANIP_NAMESPACED_VALUE() }

#define MANIP_JSON_ITEM(KEY, DEFAULT_VAL)                         \
  if (MODE == kModeReadConfigFromJSON) {                          \
    json::ReadValue(childJSON, #KEY, childConf.KEY, DEFAULT_VAL); \
  } else {                                                        \
    json::WriteValue(doc, childJSON, #KEY, childConf.KEY);        \
  }

  // General config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, desktop, general) {
    MANIP_JSON_ITEM(thread_count, int(4));
    MANIP_JSON_ITEM(locale, Str("zh_CN"));
  }
  END_MANIP_NAMESPACED_VALUE()

  // Kugou config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, decryption, kugou) {
    using namespace umc::decryption::kugou;
    MANIP_JSON_ITEM(t1, KugouInternalTable{});
    MANIP_JSON_ITEM(t2, KugouInternalTable{});
    MANIP_JSON_ITEM(v2, KugouInternalTable{});
    MANIP_JSON_ITEM(vpr_key, KugouVPRKey{});
  }
  END_MANIP_NAMESPACED_VALUE()

  // Kuwo config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, decryption, kuwo) {
    using namespace umc::decryption::kuwo;
    MANIP_JSON_ITEM(key, KuwoKey{});
  }
  END_MANIP_NAMESPACED_VALUE()

  // Kugou config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, decryption, netease) {
    using namespace umc::decryption::netease;
    MANIP_JSON_ITEM(key, NCMContentKeyProtectionKey{});
  }
  END_MANIP_NAMESPACED_VALUE()

  // QQ Music (Tencent) config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, decryption, qmc) {
    using namespace umc::decryption::tencent;
    MANIP_JSON_ITEM(ekey_seed, u8(0));
    MANIP_JSON_ITEM(static_cipher_key, QMCv1Key{});
  }
  END_MANIP_NAMESPACED_VALUE()

  // Joox (Tencent) config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, decryption, joox) {
    using namespace umc::decryption::tencent;
    MANIP_JSON_ITEM(install_uuid, Str("ffffffffffffffffffffffffffffffff"));
    MANIP_JSON_ITEM(salt, JooxSalt{});
  }
  END_MANIP_NAMESPACED_VALUE()

  // Ximalaya config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, decryption, ximalaya) {
    using namespace umc::decryption::ximalaya;
    MANIP_JSON_ITEM(x2m_content_key, X2MContentKey{});
    MANIP_JSON_ITEM(x2m_scramble_table, ScrambleTable{});
    MANIP_JSON_ITEM(x3m_content_key, X3MContentKey{});
    MANIP_JSON_ITEM(x3m_scramble_table, ScrambleTable{});
  }
  END_MANIP_NAMESPACED_VALUE()

#undef BEGIN_MANIP_NAMESPACED_VALUE
#undef END_MANIP_NAMESPACED_VALUE
#undef MANIP_JSON_ITEM
}

bool AppConfigStore::LoadConfigFromDisk() {
  std::ifstream config_file(config_file_path_, std::fstream::binary);

  using namespace rapidjson;
  IStreamWrapper json_stream(config_file);
  Document d;

  if (config_file.is_open()) {
    d.ParseStream<kParseCommentsFlag | kParseTrailingCommasFlag>(json_stream);
  }

  config_ = {};
  JSONManipulate<kModeReadConfigFromJSON>(config_, d);
  manager_->SetConfig(config_.decryption);

  return true;
}

bool AppConfigStore::SaveConfigToDisk() {
  std::ofstream config_file(config_file_path_,
                            std::fstream::out | std::fstream::binary);

  using namespace rapidjson;
  Document d;
  JSONManipulate<kModeSaveConfigToJSON>(config_, d);

  OStreamWrapper json_stream(config_file);
  PrettyWriter<OStreamWrapper> writer(json_stream);
  writer.SetIndent(' ', 2);
  d.Accept(writer);

  return true;
}

}  // namespace umd::config
