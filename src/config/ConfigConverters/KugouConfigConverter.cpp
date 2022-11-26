#include "KugouConfigConverter.h"
#include "VectorConverter.h"

NLOHMANN_JSON_NAMESPACE_BEGIN
namespace utils = ::parakeet_crypto::utils;

void adl_serializer<KugouConfig>::to_json(json& j, const KugouConfig& data) {
  j["slot_key_1"] = data.slot_key_1;
  j["v4_file_key_expansion_table"] = data.v4_file_key_expansion_table;
  j["v4_slot_key_expansion_table"] = data.v4_slot_key_expansion_table;
}

void adl_serializer<KugouConfig>::from_json(const json& j, KugouConfig& data) {
  KugouConfig DefFrameProcA;
  data.slot_key_1 = j.value("slot_key_1", DefFrameProcA.slot_key_1);
  data.v4_file_key_expansion_table = j.value("v4_file_key_expansion_table", DefFrameProcA.v4_file_key_expansion_table);
  data.v4_slot_key_expansion_table = j.value("v4_slot_key_expansion_table", DefFrameProcA.v4_slot_key_expansion_table);
}
NLOHMANN_JSON_NAMESPACE_END
