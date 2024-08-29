/*
 * tritiumhash
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
         4427  1253   599   302   157    71    35    16    11     2     4     3     3

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: tritium
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 535.139065 seconds
*/

 //------------------------------------------------------------
static const uint64_t A = UINT64_C(0x3C79AC492BA7B653);
static const uint64_t B = UINT64_C(0x1C69B3F74AC4AE35);
static const uint64_t C = UINT64_C(0xBEA225F9EB34556D);


// Moremur unary hash, by Pelle Evensen
// https://mostlymangling.blogspot.com/2019/12/stronger-better-morer-moremur-better.html
static inline uint64_t mix(uint64_t x) {
    constexpr uint32_t R0 = 27;
    constexpr uint32_t R1 = 33;
    constexpr uint32_t R2 = 27;
    x ^= x >> R0;
    x *= A;
    x ^= x >> R1;
    x *= B;
    x ^= x >> R2;
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

static inline uint64_t mix_stream_bulk(uint64_t h, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
    a ^= ROTL64(a, 39) ^ ROTL64(a, 14);
    b ^= ROTL64(b, 39) ^ ROTL64(b, 14);
    c ^= ROTL64(c, 39) ^ ROTL64(c, 14);
    d ^= ROTL64(d, 39) ^ ROTL64(d, 14);
    h += a * C;
    h *= C;
    h += b * C;
    h *= C;
    h += c * C;
    h *= C;
    h += d * C;
    h *= C;
    return h;
}

template <bool bswap>
static inline uint64_t tritiumhash(const uint8_t* buf, size_t len, uint64_t seed) {
    const uint8_t* const tail = buf + (len & ~7);
    uint64_t h = ((len ^ ROTL64(len, 3) ^ ROTL64(len, 47)) ^ (seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56)));
    
    while (len >= 64) {
        len -= 64;
        h = mix_stream_bulk(h, GET_U64<bswap>(buf, 0), GET_U64<bswap>(buf, 8),
            GET_U64<bswap>(buf, 16), GET_U64<bswap>(buf, 24));
        h = mix_stream_bulk(h, GET_U64<bswap>(buf, 32), GET_U64<bswap>(buf, 40),
            GET_U64<bswap>(buf, 48), GET_U64<bswap>(buf, 56));
        buf += 64;
    }

    while (len >= 8) {
        len -= 8;
        h = mix_stream(h, GET_U64<bswap>(buf, 0));
        buf += 8;
    }

    const uint8_t* const tail8 = buf;
    switch (len) {
    case 0: return mix(h);
    case 1: return mix(mix_stream(h, tail8[0]));
    case 2: return mix(mix_stream(h, GET_U16<bswap>(tail8, 0)));
    case 3: return mix(mix_stream(h, GET_U16<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[2]) << 16));
    case 4: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0)));
    case 5: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(tail8[4]) << 32));
    case 6: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32));
    case 7: return mix(mix_stream(h, GET_U32<bswap>(tail8, 0) | static_cast<uint64_t>(GET_U16<bswap>(tail8, 4)) << 32 | static_cast<uint64_t>(tail8[6]) << 48));
    default:;
    }

    return mix(h);
}

//------------------------------------------------------------
template <bool bswap>
static void tritium(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = tritiumhash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(tritiumhash,
    $.src_url = "https://github.com/tommyettinger/",
    $.src_status = HashFamilyInfo::SRC_ACTIVE
);

REGISTER_HASH(tritium,
    $.desc = "tritium hash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0x962071E7,// 0x36C11DC9,
    $.verification_BE = 0x483C6323,// 0x7092AFFA,
    $.hashfn_native = tritium<false>,
    $.hashfn_bswap = tritium<true>
);
