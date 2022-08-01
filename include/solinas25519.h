#ifndef SOLINAS25519_H
#define SOLINAS25519_H

#include <stdint.h>
#include <stdio.h>
typedef unsigned char fiat_p25519_uint1;
typedef signed char fiat_p25519_int1;
#if defined(__GNUC__) || defined(__clang__)
#  define FIAT_P25519_FIAT_EXTENSION __extension__
#  define FIAT_P25519_FIAT_INLINE __inline__
#else
#  define FIAT_P25519_FIAT_EXTENSION
#  define FIAT_P25519_FIAT_INLINE
#endif

FIAT_P25519_FIAT_EXTENSION typedef signed __int128 fiat_p25519_int128;
FIAT_P25519_FIAT_EXTENSION typedef unsigned __int128 fiat_p25519_uint128;

#if (-1 & 3) != 3
#error "This code only works on a two's complement system"
#endif

#if !defined(FIAT_P25519_NO_ASM) && (defined(__GNUC__) || defined(__clang__))
static __inline__ uint64_t fiat_p25519_value_barrier_u64(uint64_t a) {
  __asm__("" : "+r"(a) : /* no inputs */);
  return a;
}
#else
#  define fiat_p25519_value_barrier_u64(x) (x)
#endif

void fiat_p25519_addcarryx_u64(uint64_t* out1, fiat_p25519_uint1* out2, fiat_p25519_uint1 arg1, uint64_t arg2, uint64_t arg3);

void fiat_p25519_mulx_u64(uint64_t* out1, uint64_t* out2, uint64_t arg1, uint64_t arg2);

void fiat_p25519_mul(uint64_t out1[4], const uint64_t arg1[4], const uint64_t arg2[4]);

#endif /* SOLINAS25519_H */
