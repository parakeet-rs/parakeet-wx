#pragma once
#include <boost/algorithm/hex.hpp>

#include <algorithm>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

namespace umd::config {

template <typename T, typename R>
inline bool map_contains(std::unordered_map<T, R> m, const T& key) {
  return m.find(key) == m.end();
}

inline std::string BytesToHex(const std::vector<uint8_t>& v) {
  const char* hex = "0123456789abcdef";

  if (v.size() == 0) {
    return "";
  }

  std::string s;
  s.resize(v.size() * 3);

  char* p = s.data();
  for (auto byte : v) {
    *p++ = hex[(byte & 0xF0) >> 4];
    *p++ = hex[byte & 0x0F];
    *p++ = ' ';
  }

  // Remove last whitespace
  s.pop_back();

  return s;
}

inline std::vector<uint8_t> HexToBytes(const std::string& hex) {
  std::string s(hex);
  s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

  std::vector<uint8_t> bytes;
  boost::algorithm::unhex(s, std::back_inserter(bytes));
  return bytes;
}

template <typename K>
class ConfigBase {
 public:
  inline const std::string& GetStringItem(K k) { return strs_.at(k); }
  inline int GetIntItem(K k) { return ints_.at(k); }
  inline const std::vector<uint8_t>& GetBytesItem(K k) { return bytes_.at(k); }

  inline void SetStringFromRef(K k, std::string& v) { strs_[k] = v; }
  inline void SetBytesFromRef(K k, std::vector<uint8_t>& v) { bytes_[k] = v; }
  inline void SetIntItem(K k, int v) { ints_[k] = v; }

  inline void SetStringItem(K k, std::string v) { strs_[k] = v; }
  inline void SetBytesItem(K k, std::vector<uint8_t> v) { bytes_[k] = v; }

  inline std::string GetItemAsString(K k) {
    if (map_contains(strs_, k)) {
      return GetStringItem(k);
    }

    if (map_contains(ints_, k)) {
      return std::to_string(GetIntItem(k));
    }

    if (map_contains(bytes_, k)) {
      return BytesToHex(GetBytesItem(k));
    }
    return "";
  }

  inline void SetItemAsString(K k, const std::string& v) {
    if (map_contains(strs_, k)) {
      strs_[k] = v;
      return;
    }

    if (map_contains(ints_, k)) {
      ints_[k] = atoi(v.c_str());
      return;
    }

    if (map_contains(bytes_, k)) {
      bytes_[k] = HexToBytes(v);
      return;
    }
  }

 protected:
  std::unordered_map<K, int> ints_;
  std::unordered_map<K, std::string> strs_;
  std::unordered_map<K, std::vector<uint8_t>> bytes_;
};

}  // namespace umd::config
