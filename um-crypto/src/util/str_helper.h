#pragma once
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

inline void umc_str_from_ptr(char*& result, const void* ptr, size_t len) {
  if (result) {
    umc_str_free(result);
  }

  result = static_cast<char*>(calloc(len + 1, sizeof(char)));
  memcpy(result, ptr, len);
}

inline void umc_str_free(char*& str) {
  assert(str && "umc_str_free: str is nullptr");
  free(str);
  str = nullptr;
}
