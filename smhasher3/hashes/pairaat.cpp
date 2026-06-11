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
    /*
     * now h1 passes all collision checks,
     * so it is suitable for hash-tables with prime numbers.
     */
    h1 += ROTL32(h2, 14);
    h2 ^= h1; h2 += ROTL32(h1, 26);
    h1 ^= h2; h1 += ROTL32(h2, 5);
    h2 ^= h1; h2 += ROTL32(h1, 24);

    return h2;
}

//------------------------------------------------------------
template <bool bswap>
static void PairAAT( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint32_t h = PairAAT_impl<bswap>((const uint8_t *)in, len, (uint32_t)seed);

    PUT_U32<bswap>(h, (uint8_t *)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(pairaat,
   $.src_url    = "https://github.com/rurban/smhasher/commit/3931fd6f723f4fb2afab6ef9a628912220e90ce7",
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
