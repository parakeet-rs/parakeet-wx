#include <rapidjson/document.h>
#include "../types.h"

namespace umd::utils::json {
typedef rapidjson::Value V;
typedef rapidjson::Document D;

template <typename T>
void ReadValue(const V& v, const char* key, T& out, const T& def_value);

template <>
void ReadValue(const V& v,
               const char* key,
               U8Str& out,
               const U8Str& def_value) {
  if (v.HasMember(key) && v[key].IsString()) {
    out = umc::U8StrFromStr(v[key].GetString());
  } else {
    out = def_value;
  }
}

template <>
void ReadValue(const V& v, const char* key, int& out, const int& def_value) {
  if (v.HasMember(key) && v[key].IsInt()) {
    out = v[key].GetInt();
  } else {
    out = def_value;
  }
}

template <>
void ReadValue(const V& v,
               const char* key,
               Vec<u8>& out,
               const Vec<u8>& def_value) {
  if (v.HasMember(key) && v[key].IsString()) {
    out = umc::utils::Base64Decode(Str(v[key].GetString()));
  } else {
    out = def_value;
  }
}

template <typename T>
void WriteValue(D& d, V& v, const char* key, const T& value);

#define __UMD_PREPARE_WRITE_JSON_VALUE(value)                        \
  if (v.HasMember(key)) {                                            \
    v[key] = value;                                                  \
  } else {                                                           \
    v.AddMember(rapidjson::StringRef(key), value, d.GetAllocator()); \
  }

template <>
void WriteValue(D& d, V& v, const char* key, const U8Str& value) {
  V result = V(reinterpret_cast<const char*>(value.c_str()), value.size(),
               d.GetAllocator());
  __UMD_PREPARE_WRITE_JSON_VALUE(result);
}

template <>
void WriteValue(D& d, V& v, const char* key, const Vec<u8>& value) {
  Str encoded = umc::utils::Base64Encode(value);
  V result = V(encoded.c_str(), encoded.length(), d.GetAllocator());
  __UMD_PREPARE_WRITE_JSON_VALUE(result);
}

template <>
void WriteValue(D& d, V& v, const char* key, const int& value) {
  __UMD_PREPARE_WRITE_JSON_VALUE(V(value));
}

#undef __UMD_PREPARE_WRITE_JSON_VALUE

}  // namespace umd::utils::json
