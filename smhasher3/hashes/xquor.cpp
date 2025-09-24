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
         4364  1305   601   306   133    93    42    13    12     6     3     2     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     2     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: xquor
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 387.843551 seconds
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
    //
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


template <bool bswap>
static inline uint64_t xquorhash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int R1 = 37;
    constexpr int R2 = 25;

    const uint8_t* const tail = buf + (len & ~7);
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
        s = (s ^ s >> R1) + mix_stream_bulk(
            GET_U64<bswap>(buf, 32),
            GET_U64<bswap>(buf, 40),
            GET_U64<bswap>(buf, 48),
            GET_U64<bswap>(buf, 56));
        buf += 64;
    }

    while (len >= 8) {
        len -= 8;
        s = mix_stream(s, GET_U64<bswap>(buf, 0));
        buf += 8;
    }

    const uint8_t* const tail8 = buf;
    switch (len) {
    case 1: s = (mix_stream(s, tail8[0])); break;
    case 2: s = (mix_stream(s, GET_U16<bswap>(tail8, 0))); break;
    case 3: s = (mix_stream(s, GET_U16<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[2]) << 16)); break;
    case 4: s = (mix_stream(s, GET_U32<bswap>(tail8, 0))); break;
    case 5: s = (mix_stream(s, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[4]) << 32)); break;
    case 6: s = (mix_stream(s, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32)); break;
    case 7: s = (mix_stream(s, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32 | static_cast<uint64_t>(tail8[6]) << 48)); break;
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
    $.verification_LE = 0x73331738,
    $.verification_BE = 0xE8FBCAD7,
    $.hashfn_native = xquor<false>,
    $.hashfn_bswap = xquor<true>
);
