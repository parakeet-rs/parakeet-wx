#pragma once
#include "um-crypto/types.h"

#include <algorithm>

namespace umc::utils {

template <typename... Args>
std::string Format(const char* fmt, Args... args) {
  auto text_len = std::snprintf(nullptr, 0, fmt, args...);
  if (text_len < 0) return "";

  // String contains the extra '\x00' at the end.
  std::string formatted(text_len, 0);
  std::snprintf(formatted.data(), text_len + 1, fmt, args...);
  return formatted;
}

inline bool IsWhitespaceOrNull(char c) {
  return c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == '\x00' || c == '\n';
}

inline bool IsHexChar(const char c) {
  return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

inline u8 HexLookup(const char c) {
  if (c >= '0' && c <= '9') return static_cast<u8>(c - '0');
  if (c >= 'A' && c <= 'F') return static_cast<u8>(c - 'A' + 10);
  if (c >= 'a' && c <= 'f') return static_cast<u8>(c - 'a' + 10);

  // we know this is wrong, but hey, we'll accept it.
  return 0;
}

inline std::string UnescapeCharSequence(const std::string& s) {
  std::string result;
  result.reserve(s.size());
  bool open_escape = false;
  for (const auto c : s) {
    if (open_escape) {
      switch (c) {
        case 'n':
          result.push_back('\n');
          break;
        case 'r':
          result.push_back('\r');
          break;
        case 't':
          result.push_back('\t');
          break;
        case 'f':
          result.push_back('\f');
          break;
        case 'v':
          result.push_back('\v');
          break;

        default:
          result.push_back(c);
          break;
      }
      open_escape = false;
    } else if (c == '\\') {
      open_escape = true;
    } else {
      result.push_back(c);
    }
  }

  return result;
}

inline std::string RemoveWhitespace(const std::string& s) {
  std::string result;
  result.reserve(s.size());
  for (const auto c : s) {
    if (!IsWhitespaceOrNull(c)) {
      result.push_back(c);
    }
  }
  return result;
}

inline std::vector<std::string> ParseCSVLine(const u8* str, std::size_t len) {
  std::vector<std::string> result;

  const u8* str_begin = str;
  while (len) {
    if (*str == ',') {
      result.push_back(std::string(str_begin, str));
      str_begin = str + 1;
    }
    str++;
    len--;
  }

  if (str_begin != str) {
    result.push_back(std::string(str_begin, str));
  }

  return result;
}

}  // namespace umc::utils
