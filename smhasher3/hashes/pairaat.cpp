/*
 * Small One-At-A-Time functions
 * Copyright (C) 2021-2022  Frank J. T. Wojcik
 * Copyright (c) 2016       Sokolov Yura aka funny_falcon <funny.falcon@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#include "Platform.h"
#include "Hashlib.h"

// work in progress!

static const uint64_t A = UINT64_C(0x3C79AC492BA7B653);
static const uint64_t B = UINT64_C(0x1C69B3F74AC4AE35);
static const uint64_t C = UINT64_C(0xBEA225F9EB34556D);
static const uint64_t PHI = UINT64_C(0x9E3779B97F4A7C15);
// Moremur unary hash, by Pelle Evensen
// https://mostlymangling.blogspot.com/2019/12/stronger-better-morer-moremur-better.html
static uint64_t moremur(uint64_t x) {
    constexpr int R0 = 27;
    constexpr int R1 = 33;
    constexpr int R2 = 27;
    x ^= x >> R0;
    x *= A;
    x ^= x >> R1;
    x *= B;
    x ^= x >> R2;
    return x;
}


/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         1853   452   227   133    58    18    14     8     6     3     1     0     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: PairAAT
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 184.633249 seconds
 */

template <bool bswap>
static uint32_t PairAAT_impl( const uint8_t * str, size_t len, uint32_t seed ) {
    const uint8_t * const end = str + len - 1;
    uint32_t h1 = seed + 0x3b5db7f9u;
    uint32_t h2 = seed ^ ROTL32(seed, 10) ^ ROTL32(seed, 23);

    for (; str < end; str+= 2) {
        h1 += GET_U16<bswap>(str, 0);
        h1 += h1 << 3; // h1 *= 9
        h2 += h1;
        h2  = ROTL32(h2, 7);
        h2 += h2 << 2; // h2 *= 5
    }

    if ((len & 1) == 1) {
        h1 += end[0] + 0x9e3779b9u;
        h1 += h1 << 3; // h1 *= 9
        h2 += h1;
        h2  = ROTL32(h2, 7);
        h2 += h2 << 2; // h2 *= 5
    }

    h1 ^= h2 ^ h2 >> 13;

    h1 += ROTL32(h2, 14);
    h2 ^= h1; h2 += ROTL32(h1, 26);
    h1 ^= h2; h1 += ROTL32(h2, 5);
    h2 ^= h1; h2 += ROTL32(h1, 24);

    return h2;
}

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         2421   591   269   117    71    33    13     8     0     1     3     0     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: PairAAT-B
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 244.413047 seconds
*/

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4424  1266   598   300   144    78    45    21     5     4     2     2     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: PairAAT64
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 386.091023 seconds
*/
template <bool bswap>
static uint64_t PairAAT_impl_B( const uint8_t * str, size_t len, uint64_t seed ) {
    const uint8_t * const end = str + len - 1;
    uint64_t h1 = seed + 0x3C79AC492BA7B653UL;
    uint64_t h2 = seed ^ ROTL64(seed, 17) ^ ROTL64(seed, 47);

    for (; str < end; str+= 2) {
        h1 += GET_U16<bswap>(str, 0);
        h1 += h1 << 3;
        h2 += h1;
        h2  = ROTL64(h2, 7);
        h2 += h2 << 2;
    }

    if ((len & 1) == 1) {
        h1 += end[0] + 0x9E3779B97F4A7C15UL;
        h1 += h1 << 3;
        h2 += h1;
        h2  = ROTL64(h2, 7);
        h2 += h2 << 2;
    }

    h1 ^= h2 ^ h2 >> 27;

    h1 += ROTL64(h2, 25);
    h2 ^= h1; h2 += ROTL64(h1, 53);
    h1 ^= h2; h1 += ROTL64(h2, 11);
    h2 ^= h1; h2 += ROTL64(h1, 46);
    h1 ^= h2; h1 += ROTL64(h2, 37);
    h2 ^= h1; h2 += ROTL64(h1, 19);
    return h2;
}

template <bool bswap>
static uint64_t PairAAT64_impl( const uint8_t * str, size_t len, uint64_t seed ) {
    const uint8_t * const end = str + len - 1;
    uint64_t h1 = seed + 0xD1B92B09B92266DDUL;
    uint64_t h2 = seed ^ ROTL64(seed, 22) ^ ROTL64(seed, 47);

    for (; str < end; str+= 2) {
        // h1 += GET_U16<bswap>(str, 0) + 0xBEA225F9EB34556DUL;//h1 -= h1 << 31;//h1 *= 0xBEA225F9EB34556DUL;
        // h2 += h1;
        // h2 ^= ROTL64(h2, 13) ^ ROTL64(h2, 43);
        // h2 -= h2 << 31;
        // h2  = ROTL64(h2, 13);
        h2 += (h1 += GET_U16<bswap>(str, 0) + 0xBEA225F9EB34556DUL);
        h2  = ROTL64(h2, 13) * 0x3C79AC492BA7B653UL;
        // h2 *= 0xF9B25D65UL;
        // h2 *= 0x3C79AC492BA7B653UL;
    }

    if ((len & 1) == 1) {
        // h1 += end[0] + 0x9E3779B97F4A7C55UL;//h1 -= h1 << 31;//h1 *= 0xBEA225F9EB34556DUL;
        // h2 += h1;
        // h2 ^= ROTL64(h2, 13) ^ ROTL64(h2, 43);
        // h2 -= h2 << 31;
        // h2  = ROTL64(h2, 13);
        // h2 *= 0xF9B25D65UL;
        // h2 *= 0x3C79AC492BA7B653UL;
        h2 += (h1 += end[0] + 0x9E3779B97F4A7C55UL);
        h2  = ROTL64(h2, 13) * 0x3C79AC492BA7B653UL;
    }

    h1 ^= h2 ^ h2 >> 27;

    h1 += ROTL64(h2, 25);
    h2 ^= h1; h2 += ROTL64(h1, 53);
    h1 ^= h2; h1 += ROTL64(h2, 11);
    h2 ^= h1; h2 += ROTL64(h1, 46);

    return h2;
}

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4308  1303   654   289   178    88    32    18    11     4     0     2     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: PairAAT64-B
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 385.763824 seconds
 */
template <bool bswap>
static uint64_t PairAAT64_impl_B( const uint8_t * str, size_t len, uint64_t seed ) {
    constexpr int M0 = 27;
    constexpr int M1 = 33;
    constexpr int M2 = 27;
    constexpr int R0 = 22;
    constexpr int R1 = 47;
    constexpr int R2 = 13;

    const uint8_t * const end = str + len - 1;
    uint64_t x = seed ^ ROTL64(seed, R0) ^ ROTL64(seed, R1) ^ PHI;

    for (; str < end; str+= 2) {
        // h1 += GET_U16<bswap>(str, 0) + 0xBEA225F9EB34556DUL;//h1 -= h1 << 31;//h1 *= 0xBEA225F9EB34556DUL;
        // h2 += h1;
        // h2 ^= ROTL64(h2, 13) ^ ROTL64(h2, 43);
        // h2 -= h2 << 31;
        // h2  = ROTL64(h2, 13);
        x += GET_U16<bswap>(str, 0) + C;
        x  = ROTL64(x, R2) * A;
        // h2 *= 0xF9B25D65UL;
        // h2 *= 0x3C79AC492BA7B653UL;
    }

    if ((len & 1) == 1) {
        // h1 += end[0] + 0x9E3779B97F4A7C55UL;//h1 -= h1 << 31;//h1 *= 0xBEA225F9EB34556DUL;
        // h2 += h1;
        // h2 ^= ROTL64(h2, 13) ^ ROTL64(h2, 43);
        // h2 -= h2 << 31;
        // h2  = ROTL64(h2, 13);
        // h2 *= 0xF9B25D65UL;
        // h2 *= 0x3C79AC492BA7B653UL;
        x += end[0] + PHI;
        x  = ROTL64(x, R2) * A;
    }
    x ^= x >> M0;
    x *= A;
    x ^= x >> M1;
    x *= B;
    x ^= x >> M2;
    return x;
}
/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4401  1269   616   284   148    87    43    18    12     8     0     1     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: PairAAT64-C
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 296.780160 seconds
 */

// using only the low 32 bits of output:
/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         2365   579   264   147    78    43    22    11     9     5     2     0     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     1     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: PairAAT-C
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 247.482652 seconds
 */
template <bool bswap>
static uint64_t PairAAT64_impl_C( const uint8_t * str, size_t len, uint64_t seed ) {
    const uint8_t * const end = str + len - 1;
    uint64_t h1 = seed + 0xD1B92B09B92266DDUL;
    uint64_t h2 = seed ^ ROTL64(seed, 22) ^ ROTL64(seed, 47) ^ PHI;

    for (; str < end; str+= 2) {
        h1 += GET_U16<bswap>(str, 0);
        h1 += h1 << 8;
        h2 += h1;
        h2  = ROTL64(h2, 7);
        h2 += h2 << 2;
    }

    if ((len & 1) == 1) {
        h1 += end[0] + 0x9E3779B97F4A7C15UL;
        h1 += h1 << 8;
        h2 += h1;
        h2  = ROTL64(h2, 7);
        h2 += h2 << 2;
    }

    h1 ^= h2 ^ h2 >> 27;

    h1 += ROTL64(h2, 25);
    h2 ^= h1; h2 += ROTL64(h1, 53);
    h1 ^= h2; h1 += ROTL64(h2, 11);
    h2 ^= h1; h2 += ROTL64(h1, 46);
    h1 ^= h2; h1 += ROTL64(h2, 37);
    h2 ^= h1; h2 += ROTL64(h1, 19);
    return h2;
}

//------------------------------------------------------------
template <bool bswap>
static void PairAAT( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint32_t h = PairAAT_impl<bswap>((const uint8_t *)in, len, (uint32_t)seed);

    PUT_U32<bswap>(h, (uint8_t *)out, 0);
}
template <bool bswap>
static void PairAAT_B( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint32_t h = PairAAT_impl_B<bswap>((const uint8_t *)in, len, (uint64_t)seed);

    PUT_U32<bswap>(h, (uint8_t *)out, 0);
}
template <bool bswap>
static void PairAAT_C( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint32_t h = PairAAT64_impl_C<bswap>((const uint8_t *)in, len, (uint64_t)seed);

    PUT_U32<bswap>(h, (uint8_t *)out, 0);
}
template <bool bswap>
static void PairAAT64( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint64_t h = PairAAT64_impl<bswap>((const uint8_t *)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t *)out, 0);
}
template <bool bswap>
static void PairAAT64_B( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint64_t h = PairAAT64_impl_B<bswap>((const uint8_t *)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t *)out, 0);
}
template <bool bswap>
static void PairAAT64_C( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint64_t h = PairAAT64_impl_C<bswap>((const uint8_t *)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t *)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(pairaat,
   $.src_url    = "https://github.com/tommyettinger/smhasher-with-junk/blob/0996fd9d2d1aed3de42633f4f7553d4f9a3a4e03/smhasher3/hashes/pairaat.cpp",
   $.src_status = HashFamilyInfo::SRC_ACTIVE
 );

REGISTER_HASH(PairAAT,
   $.desc       = "PairAAT (Small non-multiplicative 16-bit-AAT, mostly by funny-falcon)",
   $.hash_flags =
         FLAG_HASH_SMALL_SEED,
   $.impl_flags =
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_VERY_SLOW,
   $.bits = 32,
   $.verification_LE = 0xD3BF4AF7,
   $.verification_BE = 0xF884A602,
   $.hashfn_native   = PairAAT<false>,
   $.hashfn_bswap    = PairAAT<true>
 );

REGISTER_HASH(PairAAT_B,
   $.desc       = "PairAAT B (Small non-multiplicative 16-bit-AAT, mostly by funny-falcon)",
   $.hash_flags = 0,
   $.impl_flags =
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_VERY_SLOW,
   $.bits = 32,
   $.verification_LE = 0xF6422FB0,
   $.verification_BE = 0x240FA851,
   $.hashfn_native   = PairAAT_B<false>,
   $.hashfn_bswap    = PairAAT_B<true>
 );

REGISTER_HASH(PairAAT_C,
   $.desc       = "PairAAT C (Small non-multiplicative 16-bit-AAT, mostly by funny-falcon)",
   $.hash_flags = 0,
   $.impl_flags =
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_VERY_SLOW,
   $.bits = 32,
   $.verification_LE = 0x7304BB5F,
   $.verification_BE = 0x5329173D,
   $.hashfn_native   = PairAAT_C<false>,
   $.hashfn_bswap    = PairAAT_C<true>
 );

REGISTER_HASH(PairAAT64,
   $.desc       = "PairAAT64 (Small 16-bit-AAT, mostly by funny-falcon)",
   $.hash_flags = 0,
   $.impl_flags =
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_MULTIPLY_64_64 |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_VERY_SLOW,
   $.bits = 64,
   $.verification_LE = 0x267B0FEB,
   $.verification_BE = 0xF631506D,
   $.hashfn_native   = PairAAT64<false>,
   $.hashfn_bswap    = PairAAT64<true>
 );

REGISTER_HASH(PairAAT64_B,
   $.desc       = "PairAAT64 B (Small 16-bit-AAT, mostly by funny-falcon)",
   $.hash_flags = 0,
   $.impl_flags =
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_MULTIPLY_64_64 |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_VERY_SLOW,
   $.bits = 64,
   $.verification_LE = 0x9E76660B,
   $.verification_BE = 0x666360A1,
   $.hashfn_native   = PairAAT64_B<false>,
   $.hashfn_bswap    = PairAAT64_B<true>
 );

REGISTER_HASH(PairAAT64_C,
   $.desc       = "PairAAT64 C (Small non-multiplicative 16-bit-AAT, mostly by funny-falcon)",
   $.hash_flags = 0,
   $.impl_flags =
         FLAG_IMPL_ROTATE       |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_VERY_SLOW,
   $.bits = 64,
   $.verification_LE = 0x05C53179,
   $.verification_BE = 0x991681F9,
   $.hashfn_native   = PairAAT64_C<false>,
   $.hashfn_bswap    = PairAAT64_C<true>
 );
