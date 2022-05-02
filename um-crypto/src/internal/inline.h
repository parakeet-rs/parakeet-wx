#pragma once

#ifndef __umc_fi

#if __GNUC__ && !____clang__
#define __umc_fi __attribute__((always_inline)) inline
#elif _MSC_VER
#define __umc_fi __forceinline
#else
#define __umc_fi inline
#endif

#endif  // ifndef __umc_fi
