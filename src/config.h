#ifndef HUO_CONFIG_H
#define HUO_CONFIG_H 1

#if defined(__GNUC__)
#define __UINT128__ (unsigned __int128)
#define MUL_OVERFLOW(a, b, res) __builtin_mul_overflow((a), (b), (res))
#endif

#endif