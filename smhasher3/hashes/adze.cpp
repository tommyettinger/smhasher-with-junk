/*
 * adzehash
 *
 * author: Tommy Ettinger, 2025-10-04, github.com/tommyettinger
 * structure based on mx3 by Jon Maiga, jonkagstrom.com, @jonkagstrom
 * license: CC0 license
 */
#include "Platform.h"
#include "Hashlib.h"

// adze7b is much more faithful to adze7; it only changes a xorshift to a rotation.
// FASTEST SO FAR (in bulk)!
/*
Average        -    20.37 cycles/hash
Average       - 12.75 bytes/cycle - 41.56 GiB/sec @ 3.5 ghz
Average       - 12.71 bytes/cycle - 41.43 GiB/sec @ 3.5 ghz

----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4370  1282   613   285   176    83    32    23     5     8     3     3     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: adze7b
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 345.398971 seconds
*/

// Something changed when I updated to VS 2026, and now adze7b is somewhat slower.
// adze7c is faster, about as fast as ax in bulk and as fast as adze7b for small keys.
// When compiled with clang++, this performs better than with MSVC.
// This wasn't compiling with Clang for a while because TestEndianness.c.in was present
// on Windows with CMake 4.2.3, but TestEndianess.c.in (with one 'n') was not.
/*
Small key speed test - [1, 31]-byte keys
Average        -    18.33 cycles/hash
Bulk speed test - 262144-byte keys
Average       - 11.11 bytes/cycle - 36.22 GiB/sec @ 3.5 ghz
Bulk speed test - [262017, 262144]-byte keys
Average       - 11.01 bytes/cycle - 35.89 GiB/sec @ 3.5 ghz
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4384  1297   589   315   155    67    33    27     8     4     4     0     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: adze7c
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 353.981997 seconds

// OK, now adze7b seems faster than it was... This uses const a lot more, and compiles with MinGW.
Small key speed test - [1, 31]-byte keys
Average        -    17.43 cycles/hash
Bulk speed test - 262144-byte keys
Average       - 12.74 bytes/cycle - 41.53 GiB/sec @ 3.5 ghz
Bulk speed test - [262017, 262144]-byte keys
Average       - 12.73 bytes/cycle - 41.49 GiB/sec @ 3.5 ghz

// And adze7c seems slower than adze7b now. Huh. Also, MinGW with more const.
Small key speed test - [1, 31]-byte keys
Average        -    17.49 cycles/hash
Bulk speed test - 262144-byte keys
Average       - 12.14 bytes/cycle - 39.56 GiB/sec @ 3.5 ghz
Bulk speed test - [262017, 262144]-byte keys
Average       - 12.13 bytes/cycle - 39.53 GiB/sec @ 3.5 ghz

// Tried to remove one xor-rotate-xor-rotate operation from near the end of adze7b, making adze7d.
// It did not go well at all.
----------------------------------------------------------------------------------------------
Summary for: adze7d
Overall result: FAIL            ( 42 / 188 passed)
Failures:
    Avalanche           : [3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 20, 64, 128]
    BIC                 : [3, 8, 11, 15]
    Sparse              : [6/2, 4/3, 4/4, 4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 10/2, 20/3, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [numbers without commas, numbers with commas, FXXXXB, FBXXXX, XXXXFB, FooXXXXBar, FooBarXXXX, XXXXFooBar, FooooXXXXBaaar, FooooBaaarXXXX, XXXXFooooBaaar, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, XXXXFooooooBaaaaar, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, XXXXFooooooooBaaaaaaar, FooooooooooXXXXBaaaaaaaaar, FooooooooooBaaaaaaaaarXXXX, XXXXFooooooooooBaaaaaaaaar, Long alnum last 1968-2128, Long alnum last 4016-4176, Long alnum last 8112-8272]
    TwoBytes            : [20, 32, 48, 1024, 2048, 4096]
    PerlinNoise         : [2]
    Bitflip             : [3, 4, 8]
    SeedZeroes          : [1280, 8448]
    SeedSparse          : [2, 3, 6, 15, 18, 31, 52, 80, 200]
    SeedBlockLen        : [12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [3]
    Seed                : [2, 3, 6, 15, 18, 31, 52, 80, 200, 1025]
    SeedAvalanche       : [4, 8, 16, 24, 32, 64, 128]
    SeedBIC             : [3, 8, 11, 15]
    SeedBitflip         : [3, 4, 8]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 345.798031 seconds

// Trying to reduce the multiple mixes by using += with one fewer arg in adze7d.
// It actually slows it down for small keys, and it fails a lot of tests.

*********FAIL*********

----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         3174   936   485   228   162    93    59    53    33    18    25    18    18

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
           17    34    14    11     8    12     5     8     5     3     6    10  1448

----------------------------------------------------------------------------------------------
Summary for: adze7d
Overall result: FAIL            ( 139 / 188 passed)
Failures:
    Sparse              : [3/48, 2/128, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [Long alnum last 1968-2128, Long alnum last 4016-4176, Long alnum last 8112-8272]
    TwoBytes            : [48, 1024]
    PerlinNoise         : [2]
    SeedZeroes          : [1280, 8448]
    SeedSparse          : [2, 3, 6, 15, 18, 31]
    Seed                : [2, 3, 6, 15, 18, 31]
    SeedAvalanche       : [4, 8, 16, 24]
    SeedBIC             : [3, 8, 11, 15]
    SeedBitflip         : [3, 4, 8]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 340.245441 seconds

// Bringing it closer back to adze7b, but with a slightly lighter 32-byte loop, has just one failure.
// This version is very slightly faster in bulk, but very slightly slower for small keys.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4379  1290   605   314   154    66    34    24     8     4     4     0     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     1

----------------------------------------------------------------------------------------------
Summary for: adze7d
Overall result: FAIL            ( 187 / 188 passed)
Failures:
    Sparse              : [3/96]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 339.993276 seconds
*/

//------------------------------------------------------------
// MX3 multiplier
static constexpr uint64_t C = UINT64_C(0xBEA225F9EB34556D);

// Primes just greater than harmonious numbers
static constexpr uint64_t Q = UINT64_C(0x9E3779B97F4A7C55);
static constexpr uint64_t R = UINT64_C(0xC13FA9A902A63293);
static constexpr uint64_t S = UINT64_C(0xD1B54A32D192ED2D);
static constexpr uint64_t T = UINT64_C(0xDB4F0B9175AE2169);
static constexpr uint64_t U = UINT64_C(0xE19B01AA9D42C66D);
static constexpr uint64_t V = UINT64_C(0xE60E2B722B53AEF3);
static constexpr uint64_t W = UINT64_C(0xE95E1DD17D35802B);
static constexpr uint64_t X = UINT64_C(0xEBEDEED9D803C871);

static inline uint64_t adze_mix(uint64_t x) {
    constexpr int R0 = 23;
    constexpr int R1 = 43;
    constexpr int R2 = 11;
    constexpr int R3 = 50;
    x = (x ^ ROTL64(x, R0) ^ ROTL64(x, R1));
    x *= C;
    x = (x ^ ROTL64(x, R2) ^ ROTL64(x, R3));
    return x;
}

static inline uint64_t adze_mix(const uint64_t a, const uint64_t b) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    return
          (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + a) * R;
}

static inline uint64_t adze_mix(const uint64_t a, const uint64_t b, const uint64_t c) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    constexpr int S2 = 27;
    return
          (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + a) * S;
}

static inline uint64_t adze_mix(const uint64_t a, const uint64_t b, const uint64_t c, const uint64_t d) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    constexpr int S2 = 27;
    constexpr int T2 = 25;
    return
          (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + d) * S
        + (ROTL64(d, T2) + a) * T;
}

static inline uint64_t adze_mix(const uint64_t a, const uint64_t b, const uint64_t c, const uint64_t d, const uint64_t e) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    constexpr int S2 = 27;
    constexpr int T2 = 25;
    constexpr int U2 = 26;
    return
          (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + d) * S
        + (ROTL64(d, T2) + e) * T
        + (ROTL64(e, U2) + a) * U;
}

static inline uint64_t adze_mix(const uint64_t a, const uint64_t b, const uint64_t c, const uint64_t d, const uint64_t e, const uint64_t f) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    constexpr int S2 = 27;
    constexpr int T2 = 25;
    constexpr int U2 = 26;
    constexpr int V2 = 30;
    return
          (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + d) * S
        + (ROTL64(d, T2) + e) * T
        + (ROTL64(e, U2) + f) * U
        + (ROTL64(f, V2) + a) * V;
}

static inline uint64_t adze_mix(const uint64_t a, const uint64_t b, const uint64_t c, const uint64_t d, const uint64_t e, const uint64_t f, const uint64_t g) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    constexpr int S2 = 27;
    constexpr int T2 = 25;
    constexpr int U2 = 26;
    constexpr int V2 = 30;
    constexpr int W2 = 23;
    return
          (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + d) * S
        + (ROTL64(d, T2) + e) * T
        + (ROTL64(e, U2) + f) * U
        + (ROTL64(f, V2) + g) * V
        + (ROTL64(g, W2) + a) * W;
}

static inline uint64_t adze_mix(const uint64_t a, const uint64_t b, const uint64_t c, const uint64_t d, const uint64_t e, const uint64_t f, const uint64_t g, const uint64_t h) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    constexpr int S2 = 27;
    constexpr int T2 = 25;
    constexpr int U2 = 26;
    constexpr int V2 = 30;
    constexpr int W2 = 23;
    constexpr int X2 = 31;
    return
          (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + d) * S
        + (ROTL64(d, T2) + e) * T
        + (ROTL64(e, U2) + f) * U
        + (ROTL64(f, V2) + g) * V
        + (ROTL64(g, W2) + h) * W
        + (ROTL64(h, X2) + a) * X;
}


static inline uint64_t adze_mix_bulk(const uint64_t h, const uint64_t a, const uint64_t b, const uint64_t c, const uint64_t d) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    constexpr int S2 = 27;
    constexpr int T2 = 25;
    return h
        + (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + d) * S
        + (ROTL64(d, T2) + a) * T;
}

static inline uint64_t adze_mix_bulk(const uint64_t h, const uint64_t a, const uint64_t b, const uint64_t c, const uint64_t d, const uint64_t e, const uint64_t f, const uint64_t g) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    constexpr int S2 = 27;
    constexpr int T2 = 25;
    constexpr int U2 = 26;
    constexpr int V2 = 30;
    constexpr int W2 = 23;
    return h
        + (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + d) * S
        + (ROTL64(d, T2) + e) * T
        + (ROTL64(e, U2) + f) * U
        + (ROTL64(f, V2) + g) * V
        + (ROTL64(g, W2) + a) * W;
}



template <bool bswap>
static uint64_t adze7bhash(const uint8_t* buf, size_t len, const uint64_t seed) {
    constexpr int S1 = 23;
    constexpr int S2 = 56;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, S1) ^ ROTL64(seed, S2));

    while (len >= 112) {
        constexpr int R1 = 39;
        len -= 112;
        s = s * C +
            adze_mix(
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24),
                GET_U64<bswap>(buf, 32),
                GET_U64<bswap>(buf, 40),
                GET_U64<bswap>(buf, 48));
        s = ROTL64(s, R1) +
            adze_mix(
                GET_U64<bswap>(buf, 56),
                GET_U64<bswap>(buf, 64),
                GET_U64<bswap>(buf, 72),
                GET_U64<bswap>(buf, 80),
                GET_U64<bswap>(buf, 88),
                GET_U64<bswap>(buf, 96),
                GET_U64<bswap>(buf, 104));
        buf += 112;
    }

    while (len >= 32) {
        len -= 32;
        s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        buf += 32;
    }

    switch (len) {
        case 1:  s = adze_mix(s, buf[0]); break;
        case 2:  s = adze_mix(s, GET_U16<bswap>(buf, 0)); break;
        case 3:  s = adze_mix(s, GET_U16<bswap>(buf, 0), buf[2]); break;
        case 4:  s = adze_mix(s, GET_U32<bswap>(buf, 0)); break;
        case 5:  s = adze_mix(s, GET_U32<bswap>(buf, 0), buf[4]); break;
        case 6:  s = adze_mix(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
        case 7:  s = adze_mix(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
        case 8:  s = adze_mix(s, GET_U64<bswap>(buf, 0)); break;
        case 9:  s = adze_mix(s, GET_U64<bswap>(buf, 0), buf[8]); break;
        case 10: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
        case 11: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
        case 12: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
        case 13: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
        case 14: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
        case 15: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
        case 16: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
        case 17: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
        case 18: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
        case 19: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
        case 20: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
        case 21: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
        case 22: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
        case 23: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
        case 24: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
        case 25: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
        case 26: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
        case 27: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
        case 28: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
        case 29: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
        case 30: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
        case 31: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28), buf[30]); break;
        default:;
    }

    return adze_mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void adze7b(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adze7bhash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}


template <bool bswap>
static inline uint64_t adze7chash(const uint8_t* buf, size_t len, const uint64_t seed) {
    constexpr int S1 = 23;
    constexpr int S2 = 56;
    constexpr int R1 = 39;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, S1) ^ ROTL64(seed, S2));

    while (len >= 64) {
        len -= 64;
        s = adze_mix_bulk(s * C,
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24));
        s = adze_mix_bulk(ROTL64(s, R1),
            GET_U64<bswap>(buf, 32),
            GET_U64<bswap>(buf, 40),
            GET_U64<bswap>(buf, 48),
            GET_U64<bswap>(buf, 56));
        buf += 64;
    }

    while (len >= 32) {
        len -= 32;
        s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        buf += 32;
    }

    switch (len) {
    case 1:  s = adze_mix(s, buf[0]); break;
    case 2:  s = adze_mix(s, GET_U16<bswap>(buf, 0)); break;
    case 3:  s = adze_mix(s, GET_U16<bswap>(buf, 0), buf[2]); break;
    case 4:  s = adze_mix(s, GET_U32<bswap>(buf, 0)); break;
    case 5:  s = adze_mix(s, GET_U32<bswap>(buf, 0), buf[4]); break;
    case 6:  s = adze_mix(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
    case 7:  s = adze_mix(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
    case 8:  s = adze_mix(s, GET_U64<bswap>(buf, 0)); break;
    case 9:  s = adze_mix(s, GET_U64<bswap>(buf, 0), buf[8]); break;
    case 10: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
    case 11: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
    case 12: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
    case 13: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
    case 14: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
    case 15: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
    case 16: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
    case 17: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
    case 18: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
    case 19: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
    case 20: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
    case 21: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
    case 22: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
    case 23: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
    case 24: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
    case 25: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
    case 26: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
    case 27: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
    case 28: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
    case 29: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
    case 30: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
    case 31: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28), buf[30]); break;
    }

    return adze_mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void adze7c(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adze7chash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}


template <bool bswap>
static uint64_t adze7dhash(const uint8_t* buf, size_t len, const uint64_t seed) {
    constexpr int S1 = 23;
    constexpr int S2 = 56;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, S1) ^ ROTL64(seed, S2));

    while (len >= 112) {
        constexpr int R1 = 39;
        len -= 112;
        s = s * C +
            adze_mix(
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24),
                GET_U64<bswap>(buf, 32),
                GET_U64<bswap>(buf, 40),
                GET_U64<bswap>(buf, 48));
        s = ROTL64(s, R1) +
            adze_mix(
                GET_U64<bswap>(buf, 56),
                GET_U64<bswap>(buf, 64),
                GET_U64<bswap>(buf, 72),
                GET_U64<bswap>(buf, 80),
                GET_U64<bswap>(buf, 88),
                GET_U64<bswap>(buf, 96),
                GET_U64<bswap>(buf, 104));
        buf += 112;
    }

    while (len >= 32) {
        len -= 32;
        s = s * U + adze_mix(GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        buf += 32;
    }

    switch (len) {
        case 1:  s = adze_mix(s, buf[0]); break;
        case 2:  s = adze_mix(s, GET_U16<bswap>(buf, 0)); break;
        case 3:  s = adze_mix(s, GET_U16<bswap>(buf, 0), buf[2]); break;
        case 4:  s = adze_mix(s, GET_U32<bswap>(buf, 0)); break;
        case 5:  s = adze_mix(s, GET_U32<bswap>(buf, 0), buf[4]); break;
        case 6:  s = adze_mix(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
        case 7:  s = adze_mix(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
        case 8:  s = adze_mix(s, GET_U64<bswap>(buf, 0)); break;
        case 9:  s = adze_mix(s, GET_U64<bswap>(buf, 0), buf[8]); break;
        case 10: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
        case 11: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
        case 12: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
        case 13: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
        case 14: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
        case 15: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
        case 16: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
        case 17: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
        case 18: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
        case 19: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
        case 20: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
        case 21: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
        case 22: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
        case 23: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
        case 24: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
        case 25: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
        case 26: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
        case 27: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
        case 28: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
        case 29: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
        case 30: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
        case 31: s = adze_mix(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28), buf[30]); break;
        default:;
    }
    
    s = adze_mix(s);
    return s;
}

//------------------------------------------------------------
template <bool bswap>
static void adze7d(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adze7dhash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(adzehash,
    $.src_url = "https://github.com/tommyettinger/",
    $.src_status = HashFamilyInfo::SRC_ACTIVE
);

REGISTER_HASH(adze7b,
    $.desc = "adze7b hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0x40309497,
    $.verification_BE = 0x15530E19,
    $.hashfn_native = adze7b<false>,
    $.hashfn_bswap = adze7b<true>
);

REGISTER_HASH(adze7c,
    $.desc = "adze7c hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0x886836D5,
    $.verification_BE = 0xCC3E8F54,
    $.hashfn_native = adze7c<false>,
    $.hashfn_bswap = adze7c<true>
);

REGISTER_HASH(adze7d,
    $.desc = "adze7d hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = adze7d<false>,
    $.hashfn_bswap = adze7d<true>
);
