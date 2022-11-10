#include "StringConvert.h"

#include <rapidjson/document.h>
#include <um-crypto/endian.h>

#include <type_traits>

namespace umd::utils::json {

// Note:
// All "std::string"s in this scope are UTF-8 encoded.

#define __UMD_PREPARE_WRITE_JSON_VALUE(value) \
  if (v.HasMember(key)) v[key] = value;       \
  else v.AddMember(rapidjson::StringRef(key), value, d.GetAllocator())

typedef rapidjson::Value V;
typedef rapidjson::Document D;

#pragma region  // Template signature
template <typename T>
inline void ReadValue(const V& v, const char* key, T& out, const T& def_value);

template <typename T>
inline void WriteValue(D& d, V& v, const char* key, const T& value);
#pragma endregion

#pragma region  // JSON <--> std::string
template <>
inline void ReadValue(const V& v, const char* key, std::string& out, const std::string& def_value) {
  if (v.HasMember(key) && v[key].IsString()) {
    out = v[key].GetString();
  } else {
    out = def_value;
  }
}

template <>
inline void WriteValue(D& d, V& v, const char* key, const std::string& value) {
  V result = V(reinterpret_cast<const char*>(value.c_str()), value.size(), d.GetAllocator());
  __UMD_PREPARE_WRITE_JSON_VALUE(result);
}
#pragma endregion

#pragma region  // JSON <--> Int
template <>
inline void ReadValue(const V& v, const char* key, int& out, const int& def_value) {
  if (v.HasMember(key) && v[key].IsInt()) {
    out = v[key].GetInt();
  } else {
    out = def_value;
  }
}

template <>
inline void WriteValue(D& d, V& v, const char* key, const int& value) {
  __UMD_PREPARE_WRITE_JSON_VALUE(V(value));
}

#pragma endregion

#pragma region  // JSON <--> uint8_t
template <>
inline void ReadValue(const V& v, const char* key, uint8_t& out, const uint8_t& def_value) {
  if (v.HasMember(key) && v[key].IsUint()) {
    out = v[key].GetUint();
  } else {
    out = def_value;
  }
}

template <>
inline void WriteValue(D& d, V& v, const char* key, const uint8_t& value) {
  __UMD_PREPARE_WRITE_JSON_VALUE(V(int(value)));
}
#pragma endregion

#pragma region  // JSON <--> std::vector<uint8_t>
template <>
inline void ReadValue(const V& v, const char* key, std::vector<uint8_t>& out, const std::vector<uint8_t>& def_value) {
  if (v.HasMember(key) && v[key].IsString()) {
    out = umc::utils::Base64Decode(std::string(v[key].GetString()));
  } else {
    out = def_value;
  }
}

template <>
inline void WriteValue(D& d, V& v, const char* key, const std::vector<uint8_t>& value) {
  std::string encoded = umc::utils::Base64Encode(value);
  V result = V(encoded.c_str(), encoded.length(), d.GetAllocator());
  __UMD_PREPARE_WRITE_JSON_VALUE(result);
}
#pragma endregion

namespace detail {

template <std::unsigned_integral A, class TContainer>
inline std::string ContainerToBase64(const TContainer& in) {
  std::vector<uint8_t> buf(in.size() * sizeof(A));
  auto p_out = buf.data();
  for (auto it = in.begin(); it < in.end(); it++) {
    umc::WriteLittleEndian<A>(p_out, *it);
    p_out += sizeof(A);
  }
  return umc::utils::Base64Encode(buf);
}

template <std::unsigned_integral A, class TContainer>
inline void Base64ToContainer(TContainer& out, const std::string& in) {
  std::vector<uint8_t> v_in = umc::utils::Base64Decode(in);
  if (v_in.size() < sizeof(A)) return;

  auto end_str = v_in.end() - sizeof(A);
  auto it_src = v_in.begin();  // char iter
  auto it_dst = out.begin();   // 'A' iter
  for (; it_src <= end_str && it_dst < out.end(); it_src += sizeof(A)) {
    *it_dst++ = umc::ReadLittleEndian<A>(&(*it_src));
  }
}

}  // namespace detail

#pragma region  // std::string <--> std::array<A, Size>
template <std::unsigned_integral A, size_t Size>
inline void ReadValue(const V& v, const char* key, std::array<A, Size>& out, const std::array<A, Size>& def_value) {
  if (v.HasMember(key) && v[key].IsString()) {
    std::fill(out.begin(), out.end(), 0);

    std::vector<A> buf(Size);
    detail::Base64ToContainer<A, std::vector<A>>(buf, std::string(v[key].GetString()));
    buf.resize(Size);

    std::copy(buf.begin(), buf.end(), out.begin());
  } else {
    out = def_value;
  }
};

template <std::unsigned_integral A, size_t Size>
inline void WriteValue(D& d, V& v, const char* key, const std::array<A, Size>& value) {
  WriteValue(d, v, key, detail::ContainerToBase64<A, std::array<A, Size>>(value));
};
#pragma endregion

#pragma region  // std::string <--> std::vector<A>
template <typename TContainer, std::integral A>
inline void ReadValue(const V& v, const char* key, TContainer& out, const TContainer& def_value) {
  if (v.HasMember(key) && v[key].IsString()) {
    std::fill(out.begin(), out.end(), 0);
    detail::Base64ToContainer(out, std::string(v[key].GetString()));
  } else {
    out = def_value;
  }
};

template <typename TContainer, std::integral A>
inline void WriteValue(D& d, V& v, const char* key, const TContainer& value) {
  WriteValue(d, v, key, detail::ContainerToBase64(value));
};
#pragma endregion

#undef __UMD_PREPARE_WRITE_JSON_VALUE

}  // namespace umd::utils::json
