#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace umc {

inline void str_free(char*& str) {
  assert(str && "umc::str_free: str is nullptr");
  free(str);
  str = nullptr;
}

inline void str_from_ptr(char*& result, const void* ptr, size_t len) {
  if (result) {
    str_free(result);
  }

  result = static_cast<char*>(calloc(len + 1, sizeof(char)));
  memcpy(result, ptr, len);
}

}  // namespace umc
