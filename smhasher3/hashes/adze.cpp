/*
 * adzehash
 *
 * author: Tommy Ettinger, 2025-10-04, github.com/tommyettinger
 * structure based on mx3 by Jon Maiga, jonkagstrom.com, @jonkagstrom
 * license: CC0 license
 */
#include "Platform.h"
#include "Hashlib.h"

// adze
/*
Average        -    20.48 cycles/hash
Average       - 12.15 bytes/cycle - 39.61 GiB/sec @ 3.5 ghz
Average       - 12.13 bytes/cycle - 39.53 GiB/sec @ 3.5 ghz

----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4397  1268   615   281   163    80    36    22     8     8     3     2     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: adze
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 391.748336 seconds
*/

// adze5, slightly better bulk speed
/*
Average        -    20.46 cycles/hash
Average       - 12.42 bytes/cycle - 40.49 GiB/sec @ 3.5 ghz
Average       - 12.39 bytes/cycle - 40.40 GiB/sec @ 3.5 ghz

----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4399  1275   607   279   172    79    34    21     6     7     2     2     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: adze5
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 350.710457 seconds
*/

// adze6, even better bulk speed
/*
Average        -    20.54 cycles/hash
Average       - 12.62 bytes/cycle - 41.12 GiB/sec @ 3.5 ghz
Average       - 12.58 bytes/cycle - 41.02 GiB/sec @ 3.5 ghz

----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4390  1273   619   283   167    76    36    22     6     7     2     2     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: adze6
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 346.351209 seconds
*/

// adze7, small gains...
/*
Average        -    20.36 cycles/hash
Average       - 12.74 bytes/cycle - 41.54 GiB/sec @ 3.5 ghz
Average       - 12.68 bytes/cycle - 41.33 GiB/sec @ 3.5 ghz

----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4369  1283   617   284   173    81    35    22     6     7     3     3     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: adze7
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 346.568612 seconds
*/


// adze8, slower in bulk unless aligned just right.
/*
Average        -    20.45 cycles/hash
Average       - 10.98 bytes/cycle - 35.79 GiB/sec @ 3.5 ghz
Average       - 10.76 bytes/cycle - 35.08 GiB/sec @ 3.5 ghz

----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4370  1292   613   282   167    84    33    23     6     7     3     3     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: adze8
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 356.936920 seconds
*/

// adze7a, no faster at anything, slower at bulk...
/*
Average        -    20.53 cycles/hash
Average       -  8.27 bytes/cycle - 26.97 GiB/sec @ 3.5 ghz
Average       -  8.19 bytes/cycle - 26.71 GiB/sec @ 3.5 ghz

----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4382  1276   610   289   167    82    34    24     5     7     3     4     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: adze7a
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 371.547761 seconds
*/

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
/*
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
*/

//------------------------------------------------------------
// MX3 multiplier
static const uint64_t C = UINT64_C(0xBEA225F9EB34556D);

// Primes just greater than harmonious numbers
static const uint64_t Q = UINT64_C(0x9E3779B97F4A7C55);
static const uint64_t R = UINT64_C(0xC13FA9A902A63293);
static const uint64_t S = UINT64_C(0xD1B54A32D192ED2D);
static const uint64_t T = UINT64_C(0xDB4F0B9175AE2169);
static const uint64_t U = UINT64_C(0xE19B01AA9D42C66D);
static const uint64_t V = UINT64_C(0xE60E2B722B53AEF3);
static const uint64_t W = UINT64_C(0xE95E1DD17D35802B);
static const uint64_t X = UINT64_C(0xEBEDEED9D803C871);

static inline uint64_t mix(uint64_t x) {
    constexpr int R0 = 23;
    constexpr int R1 = 43;
    constexpr int R2 = 11;
    constexpr int R3 = 50;
    x = (x ^ ROTL64(x, R0) ^ ROTL64(x, R1));
    x *= C;
    x = (x ^ ROTL64(x, R2) ^ ROTL64(x, R3));
    return x;
}

static inline uint64_t mix_multiple(uint64_t a, uint64_t b) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    return
          (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + a) * R;
}

static inline uint64_t mix_multiple(uint64_t a, uint64_t b, uint64_t c) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    constexpr int S2 = 27;
    return 
          (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + a) * S;
}

static inline uint64_t mix_multiple(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
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

static inline uint64_t mix_multiple(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e) {
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

static inline uint64_t mix_multiple(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f) {
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

static inline uint64_t mix_multiple(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g) {
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

static inline uint64_t mix_multiple(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g, uint64_t h) {
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


static inline uint64_t mix_bulk(uint64_t h, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
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

static inline uint64_t mix_bulk(uint64_t h, uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f, uint64_t g) {
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
static inline uint64_t adzehash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int S = 25;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56));

    while (len >= 64) {
        len -= 64;
        s = s * C +
            mix_multiple(
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24));
        s = (s ^ s >> S) +
            mix_multiple(
                GET_U64<bswap>(buf, 32),
                GET_U64<bswap>(buf, 40),
                GET_U64<bswap>(buf, 48),
                GET_U64<bswap>(buf, 56));
        buf += 64;
    }

    while (len > 30) {
        len -= 8;
        s = mix_multiple(s, GET_U64<bswap>(buf, 0));
        buf += 8;
    }

    switch (len) {
    case 1:  s = mix_multiple(s, buf[0]); break;
    case 2:  s = mix_multiple(s, GET_U16<bswap>(buf, 0)); break;
    case 3:  s = mix_multiple(s, GET_U16<bswap>(buf, 0), buf[2]); break;
    case 4:  s = mix_multiple(s, GET_U32<bswap>(buf, 0)); break;
    case 5:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), buf[4]); break;
    case 6:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
    case 7:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
    case 8:  s = mix_multiple(s, GET_U64<bswap>(buf, 0)); break;
    case 9:  s = mix_multiple(s, GET_U64<bswap>(buf, 0), buf[8]); break;
    case 10: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
    case 11: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
    case 12: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
    case 13: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
    case 14: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
    case 15: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
    case 16: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
    case 17: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
    case 18: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
    case 19: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
    case 20: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
    case 21: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
    case 22: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
    case 23: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
    case 24: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
    case 25: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
    case 26: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
    case 27: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
    case 28: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
    case 29: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
    case 30: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
    }

    return mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void adze(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adzehash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}


template <bool bswap>
static inline uint64_t adze5hash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int S = 25;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56));

    while (len >= 80) {
        len -= 80;
        s = s * C +
            mix_multiple(
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24),
                GET_U64<bswap>(buf, 32));
        s = (s ^ s >> S) +
            mix_multiple(
                GET_U64<bswap>(buf, 40),
                GET_U64<bswap>(buf, 48),
                GET_U64<bswap>(buf, 56),
                GET_U64<bswap>(buf, 64),
                GET_U64<bswap>(buf, 72));
        buf += 80;
    }

    while (len > 30) {
        len -= 8;
        s = mix_multiple(s, GET_U64<bswap>(buf, 0));
        buf += 8;
    }

    switch (len) {
    case 1:  s = mix_multiple(s, buf[0]); break;
    case 2:  s = mix_multiple(s, GET_U16<bswap>(buf, 0)); break;
    case 3:  s = mix_multiple(s, GET_U16<bswap>(buf, 0), buf[2]); break;
    case 4:  s = mix_multiple(s, GET_U32<bswap>(buf, 0)); break;
    case 5:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), buf[4]); break;
    case 6:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
    case 7:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
    case 8:  s = mix_multiple(s, GET_U64<bswap>(buf, 0)); break;
    case 9:  s = mix_multiple(s, GET_U64<bswap>(buf, 0), buf[8]); break;
    case 10: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
    case 11: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
    case 12: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
    case 13: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
    case 14: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
    case 15: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
    case 16: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
    case 17: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
    case 18: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
    case 19: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
    case 20: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
    case 21: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
    case 22: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
    case 23: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
    case 24: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
    case 25: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
    case 26: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
    case 27: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
    case 28: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
    case 29: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
    case 30: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
    }

    return mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void adze5(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adze5hash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

template <bool bswap>
static inline uint64_t adze6hash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int S = 25;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56));

    while (len >= 96) {
        len -= 96;
        s = s * C +
            mix_multiple(
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24),
                GET_U64<bswap>(buf, 32),
                GET_U64<bswap>(buf, 40));
        s = (s ^ s >> S) +
            mix_multiple(
                GET_U64<bswap>(buf, 48),
                GET_U64<bswap>(buf, 56),
                GET_U64<bswap>(buf, 64),
                GET_U64<bswap>(buf, 72),
                GET_U64<bswap>(buf, 80),
                GET_U64<bswap>(buf, 88));
        buf += 96;
    }

    while (len > 30) {
        len -= 8;
        s = mix_multiple(s, GET_U64<bswap>(buf, 0));
        buf += 8;
    }

    switch (len) {
    case 1:  s = mix_multiple(s, buf[0]); break;
    case 2:  s = mix_multiple(s, GET_U16<bswap>(buf, 0)); break;
    case 3:  s = mix_multiple(s, GET_U16<bswap>(buf, 0), buf[2]); break;
    case 4:  s = mix_multiple(s, GET_U32<bswap>(buf, 0)); break;
    case 5:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), buf[4]); break;
    case 6:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
    case 7:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
    case 8:  s = mix_multiple(s, GET_U64<bswap>(buf, 0)); break;
    case 9:  s = mix_multiple(s, GET_U64<bswap>(buf, 0), buf[8]); break;
    case 10: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
    case 11: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
    case 12: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
    case 13: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
    case 14: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
    case 15: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
    case 16: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
    case 17: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
    case 18: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
    case 19: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
    case 20: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
    case 21: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
    case 22: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
    case 23: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
    case 24: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
    case 25: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
    case 26: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
    case 27: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
    case 28: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
    case 29: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
    case 30: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
    }

    return mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void adze6(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adze6hash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

template <bool bswap>
static inline uint64_t adze7hash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int S = 25;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56));

    while (len >= 112) {
        len -= 112;
        s = s * C +
            mix_multiple(
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24),
                GET_U64<bswap>(buf, 32),
                GET_U64<bswap>(buf, 40),
                GET_U64<bswap>(buf, 48));
        s = (s ^ s >> S) +
            mix_multiple(
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
        s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        buf += 32;
    }

    switch (len) {
    case 1:  s = mix_multiple(s, buf[0]); break;
    case 2:  s = mix_multiple(s, GET_U16<bswap>(buf, 0)); break;
    case 3:  s = mix_multiple(s, GET_U16<bswap>(buf, 0), buf[2]); break;
    case 4:  s = mix_multiple(s, GET_U32<bswap>(buf, 0)); break;
    case 5:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), buf[4]); break;
    case 6:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
    case 7:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
    case 8:  s = mix_multiple(s, GET_U64<bswap>(buf, 0)); break;
    case 9:  s = mix_multiple(s, GET_U64<bswap>(buf, 0), buf[8]); break;
    case 10: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
    case 11: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
    case 12: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
    case 13: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
    case 14: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
    case 15: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
    case 16: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
    case 17: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
    case 18: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
    case 19: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
    case 20: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
    case 21: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
    case 22: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
    case 23: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
    case 24: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
    case 25: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
    case 26: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
    case 27: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
    case 28: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
    case 29: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
    case 30: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
    case 31: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28), buf[30]); break;
    }

    return mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void adze7(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adze7hash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

template <bool bswap>
static inline uint64_t adze8hash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int S = 25;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56));

    while (len >= 128) {
        len -= 128;
        s = s * C +
            mix_multiple(
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24),
                GET_U64<bswap>(buf, 32),
                GET_U64<bswap>(buf, 40),
                GET_U64<bswap>(buf, 48),
                GET_U64<bswap>(buf, 56));
        s = (s ^ s >> S) +
            mix_multiple(
                GET_U64<bswap>(buf, 64),
                GET_U64<bswap>(buf, 72),
                GET_U64<bswap>(buf, 80),
                GET_U64<bswap>(buf, 88),
                GET_U64<bswap>(buf, 96),
                GET_U64<bswap>(buf, 104),
                GET_U64<bswap>(buf, 112),
                GET_U64<bswap>(buf, 120));
        buf += 128;
    }

    while (len >= 32) {
        len -= 32;
        s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        buf += 32;
    }

    switch (len) {
    case 1:  s = mix_multiple(s, buf[0]); break;
    case 2:  s = mix_multiple(s, GET_U16<bswap>(buf, 0)); break;
    case 3:  s = mix_multiple(s, GET_U16<bswap>(buf, 0), buf[2]); break;
    case 4:  s = mix_multiple(s, GET_U32<bswap>(buf, 0)); break;
    case 5:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), buf[4]); break;
    case 6:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
    case 7:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
    case 8:  s = mix_multiple(s, GET_U64<bswap>(buf, 0)); break;
    case 9:  s = mix_multiple(s, GET_U64<bswap>(buf, 0), buf[8]); break;
    case 10: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
    case 11: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
    case 12: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
    case 13: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
    case 14: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
    case 15: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
    case 16: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
    case 17: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
    case 18: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
    case 19: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
    case 20: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
    case 21: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
    case 22: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
    case 23: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
    case 24: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
    case 25: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
    case 26: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
    case 27: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
    case 28: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
    case 29: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
    case 30: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
    case 31: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28), buf[30]); break;
    }

    return mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void adze8(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adze8hash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}


template <bool bswap>
static inline uint64_t adze7ahash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int S = 25;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56));

    while (len >= 112) {
        len -= 112;
        s = mix_multiple(s,
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24),
                GET_U64<bswap>(buf, 32),
                GET_U64<bswap>(buf, 40),
                GET_U64<bswap>(buf, 48));
        s = mix_multiple(s,
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
        s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        buf += 32;
    }

    switch (len) {
    case 1:  s = mix_multiple(s, buf[0]); break;
    case 2:  s = mix_multiple(s, GET_U16<bswap>(buf, 0)); break;
    case 3:  s = mix_multiple(s, GET_U16<bswap>(buf, 0), buf[2]); break;
    case 4:  s = mix_multiple(s, GET_U32<bswap>(buf, 0)); break;
    case 5:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), buf[4]); break;
    case 6:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
    case 7:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
    case 8:  s = mix_multiple(s, GET_U64<bswap>(buf, 0)); break;
    case 9:  s = mix_multiple(s, GET_U64<bswap>(buf, 0), buf[8]); break;
    case 10: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
    case 11: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
    case 12: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
    case 13: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
    case 14: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
    case 15: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
    case 16: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
    case 17: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
    case 18: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
    case 19: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
    case 20: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
    case 21: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
    case 22: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
    case 23: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
    case 24: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
    case 25: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
    case 26: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
    case 27: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
    case 28: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
    case 29: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
    case 30: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
    case 31: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28), buf[30]); break;
    }

    return mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void adze7a(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adze7ahash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

template <bool bswap>
static inline uint64_t adze7bhash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int S1 = 23;
    constexpr int S2 = 56;
    constexpr int R1 = 39;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, S1) ^ ROTL64(seed, S2));

    while (len >= 112) {
        len -= 112;
        s = s * C +
            mix_multiple(
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24),
                GET_U64<bswap>(buf, 32),
                GET_U64<bswap>(buf, 40),
                GET_U64<bswap>(buf, 48));
        s = ROTL64(s, R1) +
            mix_multiple(
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
        s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        buf += 32;
    }

    switch (len) {
    case 1:  s = mix_multiple(s, buf[0]); break;
    case 2:  s = mix_multiple(s, GET_U16<bswap>(buf, 0)); break;
    case 3:  s = mix_multiple(s, GET_U16<bswap>(buf, 0), buf[2]); break;
    case 4:  s = mix_multiple(s, GET_U32<bswap>(buf, 0)); break;
    case 5:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), buf[4]); break;
    case 6:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
    case 7:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
    case 8:  s = mix_multiple(s, GET_U64<bswap>(buf, 0)); break;
    case 9:  s = mix_multiple(s, GET_U64<bswap>(buf, 0), buf[8]); break;
    case 10: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
    case 11: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
    case 12: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
    case 13: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
    case 14: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
    case 15: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
    case 16: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
    case 17: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
    case 18: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
    case 19: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
    case 20: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
    case 21: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
    case 22: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
    case 23: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
    case 24: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
    case 25: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
    case 26: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
    case 27: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
    case 28: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
    case 29: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
    case 30: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
    case 31: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28), buf[30]); break;
    }

    return mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void adze7b(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adze7bhash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}


template <bool bswap>
static inline uint64_t adze7chash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int S1 = 23;
    constexpr int S2 = 56;
    constexpr int R1 = 39;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = (len ^ seed ^ ROTL64(seed, S1) ^ ROTL64(seed, S2));

    while (len >= 64) {
        len -= 64;
        s = mix_bulk(s * C,
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24));
        s = mix_bulk(ROTL64(s, R1),
            GET_U64<bswap>(buf, 32),
            GET_U64<bswap>(buf, 40),
            GET_U64<bswap>(buf, 48),
            GET_U64<bswap>(buf, 56));
        buf += 64;
    }

    while (len >= 32) {
        len -= 32;
        s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        buf += 32;
    }

    switch (len) {
    case 1:  s = mix_multiple(s, buf[0]); break;
    case 2:  s = mix_multiple(s, GET_U16<bswap>(buf, 0)); break;
    case 3:  s = mix_multiple(s, GET_U16<bswap>(buf, 0), buf[2]); break;
    case 4:  s = mix_multiple(s, GET_U32<bswap>(buf, 0)); break;
    case 5:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), buf[4]); break;
    case 6:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
    case 7:  s = mix_multiple(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
    case 8:  s = mix_multiple(s, GET_U64<bswap>(buf, 0)); break;
    case 9:  s = mix_multiple(s, GET_U64<bswap>(buf, 0), buf[8]); break;
    case 10: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
    case 11: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
    case 12: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
    case 13: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
    case 14: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
    case 15: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
    case 16: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
    case 17: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
    case 18: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
    case 19: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
    case 20: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
    case 21: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
    case 22: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
    case 23: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
    case 24: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
    case 25: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
    case 26: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
    case 27: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
    case 28: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
    case 29: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
    case 30: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
    case 31: s = mix_multiple(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28), buf[30]); break;
    }

    return mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void adze7c(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = adze7chash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(adzehash,
    $.src_url = "https://github.com/tommyettinger/",
    $.src_status = HashFamilyInfo::SRC_ACTIVE
);

REGISTER_HASH(adze,
    $.desc = "adze hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0x603832DE,
    $.verification_BE = 0x01183EA4,
    $.hashfn_native = adze<false>,
    $.hashfn_bswap = adze<true>
);

REGISTER_HASH(adze5,
    $.desc = "adze5 hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = adze5<false>,
    $.hashfn_bswap = adze5<true>
);

REGISTER_HASH(adze6,
    $.desc = "adze6 hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = adze6<false>,
    $.hashfn_bswap = adze6<true>
);

REGISTER_HASH(adze7,
    $.desc = "adze7 hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = adze7<false>,
    $.hashfn_bswap = adze7<true>
);

REGISTER_HASH(adze8,
    $.desc = "adze8 hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = adze8<false>,
    $.hashfn_bswap = adze8<true>
);

REGISTER_HASH(adze7a,
    $.desc = "adze7a hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = adze7a<false>,
    $.hashfn_bswap = adze7a<true>
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
