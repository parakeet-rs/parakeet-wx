#pragma once

#if __GNUC__ && !____clang__
#define __umc_fi __attribute__((always_inline))
#elif _MSC_VER
#define __umc_fi __forceinline
#else
#define __umc_fi inline
#endif
