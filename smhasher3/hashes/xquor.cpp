/*
 * xquorhash
 *
 * author: Tommy Ettinger, 2024-07-05, github.com/tommyettinger
 * based partly on mx3 by Jon Maiga, jonkagstrom.com, @jonkagstrom
 * uses moremur by Pelle Evensen, https://mostlymangling.blogspot.com/2019/12/stronger-better-morer-moremur-better.html
 * license: CC0 license
 */
#include "Platform.h"
#include "Hashlib.h"

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4372  1318   580   302   143    79    46    13    18     5     3     2     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     2     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: xquor
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 366.494846 seconds
*/

//------------------------------------------------------------
// MX3 multiplier
static const uint64_t C = UINT64_C(0xBEA225F9EB34556D);
static const uint64_t O = UINT64_C(7);

static const uint64_t Q = UINT64_C(0x9E3779B97F4A7C55);
static const uint64_t R = UINT64_C(0xC13FA9A902A63293);
static const uint64_t S = UINT64_C(0xD1B54A32D192ED2D);
static const uint64_t T = UINT64_C(0xDB4F0B9175AE2169);
static const uint64_t U = UINT64_C(0xE19B01AA9D42C66D);
static const uint64_t V = UINT64_C(0xE60E2B722B53AEF3);
static const uint64_t W = UINT64_C(0xE95E1DD17D35802B);
static const uint64_t X = UINT64_C(0xEBEDEED9D803C871);

static inline uint64_t mix(uint64_t x) {
    //constexpr int R = 27;
    //x ^= x * x | O;
    //x = ROTR64(x, R);
    //x ^= x * x | O;
    //x ^= x >> R;
    //return x;
    constexpr int R0 = 23;
    constexpr int R1 = 43;
    constexpr int R2 = 11;
    constexpr int R3 = 50;
    x = (x ^ ROTL64(x, R0) ^ ROTL64(x, R1));
    x *= C;
    x = (x ^ ROTL64(x, R2) ^ ROTL64(x, R3));
    return x;
}

static inline uint64_t mix_stream(uint64_t h, uint64_t x) {
    constexpr uint32_t R1 = 39;
    x *= C;
    x ^= (x >> R1);
    h += x * C;
    h *= C;
    return h;
    //constexpr int R = 27;
    //x ^= x * x | O;
    //h += ROTR64(x, R);
    //h ^= h * h | O;
    //return h ^ h >> R;
}

static inline uint64_t mix_stream_bulk(uint64_t a, uint64_t b, uint64_t c) {
    constexpr int Q2 = 28;
    constexpr int R2 = 29;
    constexpr int S2 = 27;
    return 
          (ROTL64(a, Q2) + b) * Q
        + (ROTL64(b, R2) + c) * R
        + (ROTL64(c, S2) + a) * S;
}

static inline uint64_t mix_stream_bulk(uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
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

static inline uint64_t mix_stream_bulk(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e) {
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

static inline uint64_t mix_stream_bulk(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e, uint64_t f) {
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
        + (ROTL64(e, U2) + f) * U;
        + (ROTL64(f, V2) + a) * V;
}


template <bool bswap>
static inline uint64_t xquorhash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int R1 = 37;
    constexpr int R2 = 25;
    constexpr int R3 = 50;

    // This strengthens the hash against tests that mainly use the seed.
    uint64_t s = ((len ^ ROTL64(len, 3) ^ ROTL64(len, 47)) ^ (seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56)));
    
    while (len >= 64) {
        len -= 64;
        s = s * C +
            mix_stream_bulk(
                GET_U64<bswap>(buf, 0),
                GET_U64<bswap>(buf, 8),
                GET_U64<bswap>(buf, 16),
                GET_U64<bswap>(buf, 24));
        s = (s ^ s >> R2) + 
            mix_stream_bulk(
                GET_U64<bswap>(buf, 32),
                GET_U64<bswap>(buf, 40),
                GET_U64<bswap>(buf, 48),
                GET_U64<bswap>(buf, 56));
        buf += 64;
    }

    while (len > 30) {
        len -= 8;
        s = mix_stream(s, GET_U64<bswap>(buf, 0));
        buf += 8;
    }

    switch (len) {
    case 1:  s = mix_stream(s, buf[0]); break;
    case 2:  s = mix_stream(s, GET_U16<bswap>(buf, 0)); break;
    case 3:  s = mix_stream_bulk(s, GET_U16<bswap>(buf, 0), buf[2]); break;
    case 4:  s = mix_stream(s, GET_U32<bswap>(buf, 0)); break;
    case 5:  s = mix_stream_bulk(s, GET_U32<bswap>(buf, 0), buf[4]); break;
    case 6:  s = mix_stream_bulk(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4)); break;
    case 7:  s = mix_stream_bulk(s, GET_U32<bswap>(buf, 0), GET_U16<bswap>(buf, 4), buf[6]); break;
    case 8:  s = mix_stream(s, GET_U64<bswap>(buf, 0)); break;
    case 9:  s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), buf[8]); break;
    case 10: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8)); break;
    case 11: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U16<bswap>(buf, 8), buf[10]); break;
    case 12: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8)); break;
    case 13: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), buf[12]); break;
    case 14: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12)); break;
    case 15: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U32<bswap>(buf, 8), GET_U16<bswap>(buf, 12), buf[14]); break;
    case 16: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8)); break;
    case 17: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), buf[16]); break;
    case 18: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16)); break;
    case 19: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U16<bswap>(buf, 16), buf[18]); break;
    case 20: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16)); break;
    case 21: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), buf[20]); break;
    case 22: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20)); break;
    case 23: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U32<bswap>(buf, 16), GET_U16<bswap>(buf, 20), buf[22]); break;
    case 24: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16)); break;
    case 25: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), buf[24]); break;
    case 26: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24)); break;
    case 27: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U16<bswap>(buf, 24), buf[26]); break;
    case 28: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24)); break;
    case 29: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), buf[28]); break;
    case 30: s = mix_stream_bulk(s, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8), GET_U64<bswap>(buf, 16), GET_U32<bswap>(buf, 24), GET_U16<bswap>(buf, 28)); break;
    }

    return mix(s);
}

//------------------------------------------------------------
template <bool bswap>
static void xquor(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = xquorhash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(xquorhash,
    $.src_url = "https://github.com/tommyettinger/",
    $.src_status = HashFamilyInfo::SRC_ACTIVE
);

REGISTER_HASH(xquor,
    $.desc = "xquor hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0x139CC0CF,
    $.verification_BE = 0x6B0A9FD6,
    $.hashfn_native = xquor<false>,
    $.hashfn_bswap = xquor<true>
);
