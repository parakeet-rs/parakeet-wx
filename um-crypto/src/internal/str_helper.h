#pragma once
#include "um-crypto/common.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace umc {

inline void RemoveWhitespace(Str& s) {
  s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
}

inline Vec<Str> ParseCSVLine(const u8* str, usize len) {
  Vec<Str> result;

  const u8* str_begin = str;
  while (len) {
    if (*str == ',') {
      result.push_back(Str(str_begin, str));
      str_begin = str + 1;
    }
    str++;
    len--;
  }

  if (str_begin != str) {
    result.push_back(Str(str_begin, str));
  }

  return result;
}

}  // namespace umc
