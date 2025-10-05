/*
 * adzehash - A fast, high-quality hash that avoids processor-specific code.
 * Copyright (C) 2025 Tommy Ettinger
 *
 * Structure based on 'mx3', by Jon Maiga, which is public domain under the Unlicense.
 * Uses C defines and macros from 'rapidhash', which is licensed under the
 * BSD 2-clause license. This does not use the rapidhash algorithm.
 *
 * This work is released to the public as free and unencumbered software under the
 * CC0 1.0 Universal license. See the file LICENSE in this directory.
 *
 * You can contact the author at:
 *   - GitHub: https://github.com/tommyettinger/
 */

/*
 *  Includes.
 */
#include <stdint.h>
#include <limits.h>
#include <string.h>

/*
 *  C++ macros.
 *
 *  ADZE_INLINE can be overridden to be stronger than a hint, i.e. by adding __attribute__((always_inline)).
 */
#ifdef __cplusplus
  #define ADZE_NOEXCEPT noexcept
  #ifndef ADZE_INLINE
    #define ADZE_INLINE inline
  #endif
#else
  #define ADZE_NOEXCEPT
  #ifndef ADZE_INLINE
    #define ADZE_INLINE static inline
  #endif
#endif

/*
 *  Endianness macros.
 */
#ifndef ADZE_LITTLE_ENDIAN
  #if defined(_WIN32) || defined(__LITTLE_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    #define ADZE_LITTLE_ENDIAN
  #elif defined(__BIG_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    #define ADZE_BIG_ENDIAN
  #else
    #warning "could not determine endianness! Falling back to little endian."
    #define ADZE_LITTLE_ENDIAN
  #endif
#endif

//    MX3 multiplier.
static const uint64_t ADZE_C = UINT64_C(0xBEA225F9EB34556D);

static const uint64_t ADZE_Q = UINT64_C (0x9E3779B97F4A7C55);
static const uint64_t ADZE_R = UINT64_C (0xC13FA9A902A63293);
static const uint64_t ADZE_S = UINT64_C (0xD1B54A32D192ED2D);
static const uint64_t ADZE_T = UINT64_C (0xDB4F0B9175AE2169);
static const uint64_t ADZE_U = UINT64_C (0xE19B01AA9D42C66D);
static const uint64_t ADZE_V = UINT64_C (0xE60E2B722B53AEF3);


/*
 *  From https://stackoverflow.com/a/776523 by Peter Cordes.
 *  @param n  the unsigned 64-bit number to bitwise-rotate left.
 *  @param c  the shift count for how many steps to rotate; should be unsigned.
 */
ADZE_INLINE uint64_t adze_rotl64 (uint64_t n, unsigned int c)
{
  const unsigned int mask = (CHAR_BIT*sizeof(n) - 1);  // assumes width is a power of 2.
  c &= mask;
  return (n<<c) | (n>>( (0u-c)&mask ));
}

/*
 *  Default seed. This is one of Moremur's multipliers, which is a mixing function by Pelle Evensen.
 *  It could be pretty much anything.
 */
#define ADZE_SEED UINT64_C(0x3C79AC492BA7B653)

/*
 *  Simple mixing function. Note that if the input is 0, this returns 0.
 *
 *  @param x  unsigned 64-bit number.
 */
ADZE_INLINE uint64_t adze_mix(uint64_t x) ADZE_NOEXCEPT {
  static const unsigned int R0 = 23u;
  static const unsigned int R1 = 43u;
  static const unsigned int R2 = 11u;
  static const unsigned int R3 = 50u;
  x = (x ^ adze_rotl64(x, R0) ^ adze_rotl64(x, R1));
  x *= ADZE_C;
  x = (x ^ adze_rotl64(x, R2) ^ adze_rotl64(x, R3));
  return x;
}

  /*
 *  Simple two-argument mixing function that combines both arguments. Note that if both inputs are 0, this returns 0.
 *
 *  @param a  unsigned 64-bit number; will be mixed with b.
 *  @param b  unsigned 64-bit number; will be mixed with a.
 */
ADZE_INLINE uint64_t adze_mix_multiple(uint64_t a, uint64_t b) ADZE_NOEXCEPT {
  static const unsigned int Q2 = 28u;
  static const unsigned int R2 = 29u;
    return
          (ROTL64(a, Q2) + b) * ADZE_Q
        + (ROTL64(b, R2) + a) * ADZE_R;
}

/*
 *  A fast three-argument mixing function.
 *  If the arguments are all 0, this returns 0.
 *
 *  @param a  unsigned 64-bit number; will be mixed with b and c.
 *  @param b  unsigned 64-bit number; will be mixed with c and a.
 *  @param c  unsigned 64-bit number; will be mixed with a and b.
 */
ADZE_INLINE uint64_t
adze_mix_multiple (const uint64_t a, const uint64_t b, const uint64_t c) ADZE_NOEXCEPT
{
  static const unsigned int Q2 = 28u;
  static const unsigned int R2 = 29u;
  static const unsigned int S2 = 27u;
  return   (adze_rotl64 (a, Q2) + b) * ADZE_Q
         + (adze_rotl64 (b, R2) + c) * ADZE_R
         + (adze_rotl64 (c, S2) + a) * ADZE_S;
}

/*
 *  A fast four-argument mixing function.
 *  If the arguments are all 0, this returns 0.
 *
 *  @param a  unsigned 64-bit number; will be mixed with b and d.
 *  @param b  unsigned 64-bit number; will be mixed with c and a.
 *  @param c  unsigned 64-bit number; will be mixed with d and b.
 *  @param d  unsigned 64-bit number; will be mixed with a and c.
 */
ADZE_INLINE uint64_t
adze_mix_multiple (const uint64_t a, const uint64_t b, const uint64_t c,
                   const uint64_t d) ADZE_NOEXCEPT
{
  static const unsigned int Q2 = 28u;
  static const unsigned int R2 = 29u;
  static const unsigned int S2 = 27u;
  static const unsigned int T2 = 25u;
  return   (adze_rotl64 (a, Q2) + b) * ADZE_Q
         + (adze_rotl64 (b, R2) + c) * ADZE_R
         + (adze_rotl64 (c, S2) + d) * ADZE_S
         + (adze_rotl64 (d, T2) + a) * ADZE_T;
}

/*
 *  A fast five-argument mixing function.
 *  If the arguments are all 0, this returns 0.
 *
 *  @param a  unsigned 64-bit number; will be mixed with b and d.
 *  @param b  unsigned 64-bit number; will be mixed with c and a.
 *  @param c  unsigned 64-bit number; will be mixed with d and b.
 *  @param d  unsigned 64-bit number; will be mixed with e and c.
 *  @param e  unsigned 64-bit number; will be mixed with a and d.
 */
ADZE_INLINE uint64_t
adze_mix_multiple (const uint64_t a, const uint64_t b, const uint64_t c,
                   const uint64_t d, const uint64_t e) ADZE_NOEXCEPT
{
  static const unsigned int Q2 = 28u;
  static const unsigned int R2 = 29u;
  static const unsigned int S2 = 27u;
  static const unsigned int T2 = 25u;
  static const unsigned int U2 = 26u;
  return   (adze_rotl64 (a, Q2) + b) * ADZE_Q
         + (adze_rotl64 (b, R2) + c) * ADZE_R
         + (adze_rotl64 (c, S2) + d) * ADZE_S
         + (adze_rotl64 (d, T2) + e) * ADZE_T
         + (adze_rotl64 (e, U2) + a) * ADZE_U;
}

/*
 *  A fast six-argument mixing function.
 *  If the arguments are all 0, this returns 0.
 *
 *  @param a  unsigned 64-bit number; will be mixed with b and d.
 *  @param b  unsigned 64-bit number; will be mixed with c and a.
 *  @param c  unsigned 64-bit number; will be mixed with d and b.
 *  @param d  unsigned 64-bit number; will be mixed with e and c.
 *  @param e  unsigned 64-bit number; will be mixed with f and d.
 *  @param f  unsigned 64-bit number; will be mixed with a and e.
 */
ADZE_INLINE uint64_t
adze_mix_multiple (const uint64_t a, const uint64_t b, const uint64_t c,
                   const uint64_t d, const uint64_t e, const uint64_t f) ADZE_NOEXCEPT
{
  static const unsigned int Q2 = 28u;
  static const unsigned int R2 = 29u;
  static const unsigned int S2 = 27u;
  static const unsigned int T2 = 25u;
  static const unsigned int U2 = 26u;
  static const unsigned int V2 = 30u;
  return   (adze_rotl64 (a, Q2) + b) * ADZE_Q
         + (adze_rotl64 (b, R2) + c) * ADZE_R
         + (adze_rotl64 (c, S2) + d) * ADZE_S
         + (adze_rotl64 (d, T2) + e) * ADZE_T
         + (adze_rotl64 (e, U2) + f) * ADZE_U
         + (adze_rotl64 (f, V2) + a) * ADZE_V;
}

/*
 *  Read functions.
 */
#ifdef ADZE_LITTLE_ENDIAN
ADZE_INLINE uint64_t adze_read64(const uint8_t * p, const uint32_t i) ADZE_NOEXCEPT { uint64_t v; memcpy(&v, &p[i], 8); return v;}
ADZE_INLINE uint64_t adze_read32(const uint8_t * p, const uint32_t i) ADZE_NOEXCEPT { uint32_t v; memcpy(&v, &p[i], 4); return v;}
ADZE_INLINE uint64_t adze_read16(const uint8_t * p, const uint32_t i) ADZE_NOEXCEPT { uint16_t v; memcpy(&v, &p[i], 2); return v;}
#elif defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
ADZE_INLINE uint64_t adze_read64(const uint8_t *p, const uint32_t i) ADZE_NOEXCEPT { uint64_t v; memcpy(&v, &p[i], sizeof(uint64_t)); return __builtin_bswap64(v);}
ADZE_INLINE uint64_t adze_read32(const uint8_t *p, const uint32_t i) ADZE_NOEXCEPT { uint32_t v; memcpy(&v, &p[i], sizeof(uint32_t)); return __builtin_bswap32(v);}
ADZE_INLINE uint64_t adze_read16(const uint8_t *p, const uint32_t i) ADZE_NOEXCEPT { uint16_t v; memcpy(&v, &p[i], sizeof(uint16_t)); return __builtin_bswap16(v);}
#elif defined(_MSC_VER)
ADZE_INLINE uint64_t adze_read64(const uint8_t *p, const uint32_t i) ADZE_NOEXCEPT { uint64_t v; memcpy(&v, &p[i], sizeof(uint64_t)); return _byteswap_uint64(v);}
ADZE_INLINE uint64_t adze_read32(const uint8_t *p, const uint32_t i) ADZE_NOEXCEPT { uint32_t v; memcpy(&v, &p[i], sizeof(uint32_t)); return _byteswap_ulong(v);}
ADZE_INLINE uint64_t adze_read16(const uint8_t *p, const uint32_t i) ADZE_NOEXCEPT { uint16_t v; memcpy(&v, &p[i], sizeof(uint16_t)); return _byteswap_ushort(v);}
#else
ADZE_INLINE uint64_t adze_read64(const uint8_t *p) ADZE_NOEXCEPT {
  uint64_t v; memcpy(&v, p, 8);
  return (((v >> 56) & 0xff)| ((v >> 40) & 0xff00)| ((v >> 24) & 0xff0000)| ((v >>  8) & 0xff000000)| ((v <<  8) & 0xff00000000)| ((v << 24) & 0xff0000000000)| ((v << 40) & 0xff000000000000)| ((v << 56) & 0xff00000000000000));
}
ADZE_INLINE uint64_t adze_read32(const uint8_t *p) ADZE_NOEXCEPT {
  uint32_t v; memcpy(&v, p, 4);
  return (((v >> 24) & 0xff)| ((v >>  8) & 0xff00)| ((v <<  8) & 0xff0000)| ((v << 24) & 0xff000000));
}
ADZE_INLINE uint64_t adze_read16(const uint8_t *p) ADZE_NOEXCEPT {
  uint16_t v; memcpy(&v, p, 2);
  return (((v >>  8) & 0xff)| ((v <<  8) & 0xff00));
}
#endif

/*
 *  adzehash main function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *
 *  Returns a 64-bit hash.
 */
ADZE_INLINE uint64_t adze_hash_internal(const void *key, size_t len, uint64_t seed) ADZE_NOEXCEPT {
  static const unsigned int S = 25u;
  const uint8_t *buf=(const uint8_t *)key; 
  uint64_t h = (len ^ adze_rotl64 (len, 3u) ^ adze_rotl64 (len, 47u))
               ^ seed;

  while (len >= 64) {
    len -= 64;
    h = (h * ADZE_C) + adze_mix_multiple (adze_read64 (buf,  0), adze_read64 (buf,  8), adze_read64(buf, 16), adze_read64(buf, 24));
    h = (h ^ h >> S) + adze_mix_multiple (adze_read64 (buf, 32), adze_read64 (buf, 40), adze_read64(buf, 48), adze_read64(buf, 56));
    buf += 64;
  }

  while (len > 30) {
    len -= 8;
    h = adze_mix_multiple(h, adze_read64(buf, 0));
    buf += 8;
  }
    
    switch (len) {
    case 1:  h = adze_mix_multiple(h, buf[0]); break;
    case 2:  h = adze_mix_multiple(h, adze_read16(buf, 0)); break;
    case 3:  h = adze_mix_multiple(h, adze_read16(buf, 0), buf[2]); break;
    case 4:  h = adze_mix_multiple(h, adze_read32(buf, 0)); break;
    case 5:  h = adze_mix_multiple(h, adze_read32(buf, 0), buf[4]); break;
    case 6:  h = adze_mix_multiple(h, adze_read32(buf, 0), adze_read16(buf, 4)); break;
    case 7:  h = adze_mix_multiple(h, adze_read32(buf, 0), adze_read16(buf, 4), buf[6]); break;
    case 8:  h = adze_mix_multiple(h, adze_read64(buf, 0)); break;
    case 9:  h = adze_mix_multiple(h, adze_read64(buf, 0), buf[8]); break;
    case 10: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read16(buf, 8)); break;
    case 11: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read16(buf, 8), buf[10]); break;
    case 12: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read32(buf, 8)); break;
    case 13: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read32(buf, 8), buf[12]); break;
    case 14: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read32(buf, 8), adze_read16(buf, 12)); break;
    case 15: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read32(buf, 8), adze_read16(buf, 12), buf[14]); break;
    case 16: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8)); break;
    case 17: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), buf[16]); break;
    case 18: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read16(buf, 16)); break;
    case 19: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read16(buf, 16), buf[18]); break;
    case 20: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read32(buf, 16)); break;
    case 21: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read32(buf, 16), buf[20]); break;
    case 22: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read32(buf, 16), adze_read16(buf, 20)); break;
    case 23: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read32(buf, 16), adze_read16(buf, 20), buf[22]); break;
    case 24: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read64(buf, 16)); break;
    case 25: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read64(buf, 16), buf[24]); break;
    case 26: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read64(buf, 16), adze_read16(buf, 24)); break;
    case 27: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read64(buf, 16), adze_read16(buf, 24), buf[26]); break;
    case 28: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read64(buf, 16), adze_read32(buf, 24)); break;
    case 29: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read64(buf, 16), adze_read32(buf, 24), buf[28]); break;
    case 30: h = adze_mix_multiple(h, adze_read64(buf, 0), adze_read64(buf, 8), adze_read64(buf, 16), adze_read32(buf, 24), adze_read16(buf, 28)); break;
    }

  return adze_mix(h);
}

/*
 *  adzehash seeded hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably; will be mixed a little.
 *
 *  Calls adze_hash_internal using provided parameters, but does some mixing to seed.
 *  This is, in fact, nearly identical to adze_hash_internal, except that this
 *  function mixes seed with a XOR-Rotate-XOR-Rotate step. If you are confident that your seed is not going
 *  to be used in conjunction with other extremely similar seeds (such as seed, seed+1, seed+2, etc.), then
 *  you can call adze_hash_internal directly with your unmixed seed to save a few cycles.
 *
 *  Returns a 64-bit hash.
 */
ADZE_INLINE uint64_t adze_hash_seeded(const void *key, size_t len, uint64_t seed) ADZE_NOEXCEPT {
  return adze_hash_internal (key, len, (seed ^ adze_rotl64 (seed, 23u) ^ adze_rotl64 (seed, 56u)));
}

/*
 *  adzehash default hash function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *
 *  Calls adze_hash_internal using provided parameters and the default seed, without mixing.
 *
 *  Returns a 64-bit hash.
 */
ADZE_INLINE uint64_t adze_hash(const void *key, size_t len) ADZE_NOEXCEPT {
  return adze_hash_internal(key, len, ADZE_SEED);
}
