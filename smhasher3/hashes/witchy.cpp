/*
 * witchyhash
 *
 * original author: Jon Maiga, 2020-08-03, jonkagstrom.com, @jonkagstrom
 * additional author: Tommy Ettinger, 2024-07-05, github.com/tommyettinger
 * license: CC0 license
 */
#include "Platform.h"
#include "Hashlib.h"

/*
// With R1=9, R2=11, R3=37:
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4388  1312   578   309   146    83    30    21     5     8     1     1     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: witchy
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 551.431841 seconds

// With R1=39, R2=29, R3=37 (same as Ax):
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4378  1309   622   276   147    86    23    20     5     6     3     2     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     0     0     2     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: witchy
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 767.254373 seconds

// Same as above, but using only h += (a * Q + c); instead of a rotation in there.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4375  1307   608   288   151    81    30    22     5     5     3     2     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     0     0     2     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: witchy
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 569.601139 seconds

// Just adding up 4 products with h!
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4372  1324   609   280   149    82    26    19     5     6     3     2     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     0     0     2     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: witchy
Overall result: pass            ( 188 / 188 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 670.944335 seconds
*/

// Moremur multipliers
static const uint64_t A = UINT64_C(0x3C79AC492BA7B653);
static const uint64_t B = UINT64_C(0x1C69B3F74AC4AE35);
// MX3 multiplier
static const uint64_t C = UINT64_C(0xBEA225F9EB34556D);
// Random 64-bit probable primes, as given by Java's BigInteger class.
static const uint64_t Q = UINT64_C(0xD1B92B09B92266DD);
static const uint64_t R = UINT64_C(0x9995988B72E0D285);
static const uint64_t S = UINT64_C(0x8FADF5E286E31587);
static const uint64_t T = UINT64_C(0xFCF8B405D3D0783B);

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

static inline uint64_t mix_stream(uint64_t h, uint64_t x) {
    constexpr uint32_t R1 = 39;

    x *= C;
    x ^= (x >> R1);
    h += x * C;
    h *= C;
    return h;
}

static inline uint64_t mix_stream_bulk(uint64_t h, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
    //constexpr int R2 = 29;
    //h += (ROTL64(a, R2) - c) * Q;
    //h += (ROTL64(b, R2) - d) * R;
    //h += (ROTL64(c, R2) - b) * S;
    //h += (ROTL64(d, R2) - a) * T;
    //return h;

    // the above runs at the same speed as the following, due to instruction pipelining.
    return h + a * Q + b * R + c * S + d * T;
    //h += (a * Q);// + c
    //h += (b * R);// + d
    //h += (c * S);// + b
    //h += (d * T);// + a
}

template <bool bswap>
static inline uint64_t witchyhash(const uint8_t* buf, size_t len, uint64_t seed) {
    constexpr int R3 = 37;

    const uint8_t* const tail = buf + (len & ~7);
    uint64_t h = len ^ seed ^ ROTL64(seed, 29) ^ ROTL64(seed, 47);
    //uint64_t h = ((len ^ ROTL64(len, 3) ^ ROTL64(len, 47)) + (seed ^ ROTL64(seed, 23) ^ ROTL64(seed, 56)));

    while (len >= 16) {
        len -= 16;
        h = mix_stream_bulk(h * C, GET_U16<bswap>(buf, 0), GET_U16<bswap>(buf, 2),
            GET_U16<bswap>(buf, 4), GET_U16<bswap>(buf, 6));
        h = mix_stream_bulk(ROTL64(h, R3), GET_U16<bswap>(buf, 8), GET_U16<bswap>(buf, 10),
            GET_U16<bswap>(buf, 12), GET_U16<bswap>(buf, 14));
        buf += 16;
    }

    while (len >= 2) {
        len -= 2;
        h = mix_stream(h, GET_U16<bswap>(buf, 0));
        buf += 2;
    }
    
    const uint8_t* const tail8 = buf;
    if (len) h = mix_stream(h, tail8[0]);

    return mix(h);
}

//------------------------------------------------------------
template <bool bswap>
static void witchy(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = witchyhash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(witchyhash,
    $.src_url = "https://github.com/tommyettinger/",
    $.src_status = HashFamilyInfo::SRC_ACTIVE
);

REGISTER_HASH(witchy,
    $.desc = "witchy",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = witchy<false>,
    $.hashfn_bswap = witchy<true>
);
