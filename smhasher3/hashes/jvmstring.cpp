/*
 * jvmstring hash
 * This hash follows the algorithm described in the documentation
 * for java.lang.String.hashCode() in any recent version of the JVM.
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

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
          298    81    47    30    16    10     7    10     3     4     0     0     4

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0  2265

----------------------------------------------------------------------------------------------
Summary for: jvmstring
Overall result: FAIL            ( 6 / 186 passed)
Failures:
    Sanity              : [Prepend zeroes]
    Avalanche           : [3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 20, 64, 128]
    BIC                 : [3, 8, 11, 15]
    Zeroes              : []
    Cyclic              : [4 cycles of 3 bytes, 4 cycles of 4 bytes, 4 cycles of 5 bytes, 4 cycles of 8 bytes, 8 cycles of 3 bytes, 8 cycles of 4 bytes, 8 cycles of 5 bytes, 8 cycles of 8 bytes, 12 cycles of 3 bytes, 12 cycles of 4 bytes, 12 cycles of 5 bytes, 12 cycles of 8 bytes, 16 cycles of 3 bytes, 16 cycles of 4 bytes, 16 cycles of 5 bytes, 16 cycles of 8 bytes]
    Sparse              : [6/2, 4/3, 4/4, 4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 10/2, 20/3, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [dictionary]
    TextNum             : [without commas, with commas]
    Text                : [FXXXXB, FBXXXX, XXXXFB, FooXXXXBar, FooBarXXXX, XXXXFooBar, FooooXXXXBaaar, FooooBaaarXXXX, XXXXFooooBaaar, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, XXXXFooooooBaaaaar, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, XXXXFooooooooBaaaaaaar, FooooooooooXXXXBaaaaaaaaar, FooooooooooBaaaaaaaaarXXXX, XXXXFooooooooooBaaaaaaaaar, Words alnum 1-4, Words alnum 5-8, Words alnum 1-16, Words alnum 1-32, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [8, 1024, 2048, 4096]
    PerlinNoise         : [2]
    Bitflip             : [3, 4, 8]
    SeedZeroes          : [1280, 8448]
    SeedSparse          : [2, 3, 6, 15, 18, 31, 52, 80, 200, 1025]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]
    Seed                : [2, 3, 6, 15, 18, 31, 52, 80, 200, 1025]
    SeedAvalanche       : [4, 8, 16, 24, 32, 64, 128]
    SeedBIC             : [3, 8, 11, 15]
    SeedBitflip         : [3, 4, 8]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 432.538959 seconds

// Using ROTL32(h, 25):
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
          843   193    99    57    23    11    16     4     4     5     2     3     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     1     0     0     0     0     1     2     0     0     2     1  1507

----------------------------------------------------------------------------------------------
Summary for: jvmstring
Overall result: FAIL            ( 64 / 186 passed)
Failures:
    Avalanche           : [3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 20, 64, 128]
    BIC                 : [3, 8, 11, 15]
    Sparse              : [6/2, 4/3, 4/4, 4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 10/2, 20/3, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [dictionary]
    TextNum             : [without commas, with commas]
    Text                : [FXXXXB, FBXXXX, XXXXFB, FooXXXXBar, FooBarXXXX, XXXXFooBar, FooooXXXXBaaar, FooooBaaarXXXX, XXXXFooooBaaar, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, FooooooooooBaaaaaaaaarXXXX, Words alnum 1-4, Words alnum 5-8, Words alnum 1-16, Words alnum 1-32, Long alnum last 1968-2128, Long alnum last 4016-4176, Long alnum last 8112-8272]
    TwoBytes            : [8, 1024, 2048, 4096]
    PerlinNoise         : [2]
    Bitflip             : [3, 4, 8]
    SeedZeroes          : [1280, 8448]
    SeedSparse          : [2, 3, 6, 15, 18]
    SeedBlockLen        : [8, 12, 16, 20, 24, 28]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]
    Seed                : [2, 3, 6, 15, 18]
    SeedAvalanche       : [4, 8]
    SeedBIC             : [3, 8, 11, 15]
    SeedBitflip         : [3, 4, 8]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 589.491525 seconds

// Using 0x9E3779B9u * ROTL(h, 7) and running through TheIronBorn's mixer:
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         1645   343   194    93    48    44    32    14    15     8    11     8    12

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            6     0     5     2     9     2     2     3     4     3     4     3   265

----------------------------------------------------------------------------------------------
Summary for: jvmstring
Overall result: FAIL            ( 153 / 186 passed)
Failures:
    BIC                 : [3, 8, 11, 15]
    Sparse              : [10/2, 20/3, 9/4, 5/9, 4/14, 4/16, 3/96]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    TextNum             : [with commas]
    TwoBytes            : [8]
    PerlinNoise         : [2]
    Bitflip             : [3, 4, 8]
    SeedBIC             : [3]
    SeedBitflip         : [3]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 566.933656 seconds

*/

//------------------------------------------------------------
static uint32_t jvmstring_impl( const uint8_t * data, size_t len, uint32_t h ) {
    for (size_t i = 0; i < len; ++i) {
        h = 0x9E3779B9u * ROTL32(h, 7) + data[i];
    }
    //return h ^ h >> 21 ^ h >> 11;
    
    //[16 21f0aaad 15 735a2d97 15]
    h ^= h >> 16;
    h *= 0x21F0AAADu;
    h ^= h >> 15;
    h *= 0x735A2D97u;
    h ^= h >> 15;
    return h;
}

//------------------------------------------------------------
template <bool bswap>
static void jvmstring( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint32_t h = jvmstring_impl((const uint8_t *)in, len, (uint32_t)(seed + len));

    PUT_U32<bswap>(h, (uint8_t *)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(jvmstring,
   $.src_url    = "https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp",
   $.src_status = HashFamilyInfo::SRC_FROZEN
 );

REGISTER_HASH(jvmstring,
   $.desc       = "jvmstring",
   $.hash_flags =
         FLAG_HASH_SMALL_SEED,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY     |
         FLAG_IMPL_LICENSE_MIT  |
         FLAG_IMPL_VERY_SLOW,
   $.bits = 32,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = jvmstring<false>,
   $.hashfn_bswap    = jvmstring<true>
 );
