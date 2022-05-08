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

#define BEGIN_MANIP_NAMESPACED_VALUE(PARENT, NAMESPACE)                      \
  {                                                                          \
    if (!PARENT.HasMember(#NAMESPACE)) {                                     \
      PARENT.AddMember(#NAMESPACE, Value().SetObject(), doc.GetAllocator()); \
    }                                                                        \
    auto& childConf = config.NAMESPACE;                                      \
    auto& childJSON = PARENT[#NAMESPACE];                                    \
    if (!childJSON.IsObject()) {                                             \
      childJSON.SetObject();                                                 \
    }
#define END_MANIP_NAMESPACED_VALUE() }

#define MANIP_JSON_ITEM(KEY, DEFAULT_VAL)                         \
  if (MODE == kModeReadConfigFromJSON) {                          \
    json::ReadValue(childJSON, #KEY, childConf.KEY, DEFAULT_VAL); \
  } else {                                                        \
    json::WriteValue(doc, childJSON, #KEY, childConf.KEY);        \
  }

  // General config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, general) {
    MANIP_JSON_ITEM(thread_count, (int)4);
  }
  END_MANIP_NAMESPACED_VALUE()

  // Kugou config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, kugou) {
    using namespace umc::kugou;
    MANIP_JSON_ITEM(t1, KugouTable{});
    MANIP_JSON_ITEM(t2, KugouTable{});
    MANIP_JSON_ITEM(v2, KugouTable{});
    MANIP_JSON_ITEM(vpr_key, KugouFileKey{});
  }
  END_MANIP_NAMESPACED_VALUE()

  // QQ Music (Tencent) config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, tencent) {
    using namespace umc::tencent;
    MANIP_JSON_ITEM(ekey_seed, 0);
    MANIP_JSON_ITEM(static_key, umc::tencent::StaticCipherKey{});
  }
  END_MANIP_NAMESPACED_VALUE()

  // Ximalaya config
  BEGIN_MANIP_NAMESPACED_VALUE(doc, xmly) {
    using namespace umc::ximalaya;
    MANIP_JSON_ITEM(x2m_content_key, XimalayaX2MContentKey{});
    MANIP_JSON_ITEM(x3m_content_key, XimalayaHeaderContentKey{});
    MANIP_JSON_ITEM(x3m_scramble_indexes, XimalayaHeaderScrambleTable{});
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
