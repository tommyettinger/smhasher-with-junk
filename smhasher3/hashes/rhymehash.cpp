/*
 * rhymehash - rapidhash adapted for UTF-16 string hashing
 * Copyright (C) 2025 Nicolas De Carli
 * Copyright (C) 2025 Frank J. T. Wojcik
 * Copyright (C) 2026 Tommy Ettinger
 *
 * Based on 'wyhash', by Wang Yi <godspeed_china@yeah.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * You can contact the author at:
 *   - rapidhash source repository: https://github.com/Nicoshev/rapidhash
 */

/*
 *  Includes.
 */
#include "Platform.h"
#include "Hashlib.h"

#include "Mathmult.h"

/*
 *  Read functions.
 */
template <bool bswap>
static inline uint64_t rhyme_read64( const uint8_t * p ) {
    return GET_U64<bswap>(p, 0);
}

template <bool bswap>
static inline uint64_t rhyme_read32( const uint8_t * p ) {
    return GET_U32<bswap>(p, 0);
}

template <bool bswap>
static inline uint64_t rhyme_read16( const uint8_t * p ) {
    return GET_U16<bswap>(p, 0);
}

/*
 *  Multiply and xor mix function.
 *
 *  @param A  32-bit number.
 *  @param B  32-bit number.
 *
 *  Calculates 32-bit C = A * B.
 *  Returns right xorshift of C by 15.
 */
template <bool isProtected>
static inline uint32_t rhyme_mix(const uint32_t A, const uint32_t B) {
    uint32_t C = A * B;
    if (isProtected) {
        C ^= A ^ B;
    }
    return C ^ ROTL32(C, 11) ^ ROTL32(C, 20);
}

/*
 *  Default secret parameters.
 */
static const uint32_t rhyme_secret[8] = {
    UINT32_C(0xaa6c78a5), UINT32_C(0x962eacc9),
    UINT32_C(0xd433d4a3), UINT32_C(0x1de1aa47),
    UINT32_C(0x78bd642f), UINT32_C(0xa0b428db),
    UINT32_C(0x281c388c), UINT32_C(0xaaaaaaaa),
};

// /*
//  *  rhymehash main function.
//  *
//  *  @param key     Buffer to be hashed.
//  *  @param len     @key length, in bytes.
//  *  @param seed    64-bit seed used to alter the hash result predictably.
//  *  @param secrets Array of 8 64-bit secrets used to alter hash result predictably.
//  *
//  *  Returns a 64-bit hash.
//  */
// template <bool bswap, bool isProtected, bool unrolled>
// static inline uint64_t rhymehash( const void * key, size_t len, uint64_t seed, const uint64_t * secrets ) {
//     const uint8_t * p = (const uint8_t *)key;
//     size_t          i = len;
//     uint64_t        a, b;
//
//     seed ^= rhyme_mix<isProtected>(seed ^ secrets[2], secrets[1]);
//
//     if (likely(len <= 16)) {
//         if (len >= 4) {
//             seed ^= len;
//             if (len >= 8) {
//                 const uint8_t * plast = p + len - 8;
//                 a = rhyme_read64<bswap>(p    );
//                 b = rhyme_read64<bswap>(plast);
//             } else {
//                 const uint8_t * plast = p + len - 4;
//                 a = rhyme_read32<bswap>(p    );
//                 b = rhyme_read32<bswap>(plast);
//             }
//         } else if (likely(len > 0)) {
//             a = (((uint64_t)p[0]) << 45) | p[len - 1];
//             b = p[len >> 1];
//         } else {
//             a = b = 0;
//         }
//     } else {
//         if (len > 112) {
//             uint64_t see1 = seed, see2 = seed;
//             uint64_t see3 = seed, see4 = seed;
//             uint64_t see5 = seed, see6 = seed;
//             if (unrolled) {
//                 while (i > 224) {
//                     seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p      ) ^ secrets[0],
//                             rhyme_read64<bswap>(p +   8) ^ seed);
//                     see1 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  16) ^ secrets[1],
//                             rhyme_read64<bswap>(p +  24) ^ see1);
//                     see2 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  32) ^ secrets[2],
//                             rhyme_read64<bswap>(p +  40) ^ see2);
//                     see3 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  48) ^ secrets[3],
//                             rhyme_read64<bswap>(p +  56) ^ see3);
//                     see4 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  64) ^ secrets[4],
//                             rhyme_read64<bswap>(p +  72) ^ see4);
//                     see5 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  80) ^ secrets[5],
//                             rhyme_read64<bswap>(p +  88) ^ see5);
//                     see6 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  96) ^ secrets[6],
//                             rhyme_read64<bswap>(p + 104) ^ see6);
//                     seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 112) ^ secrets[0],
//                             rhyme_read64<bswap>(p + 120) ^ seed);
//                     see1 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 128) ^ secrets[1],
//                             rhyme_read64<bswap>(p + 136) ^ see1);
//                     see2 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 144) ^ secrets[2],
//                             rhyme_read64<bswap>(p + 152) ^ see2);
//                     see3 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 160) ^ secrets[3],
//                             rhyme_read64<bswap>(p + 168) ^ see3);
//                     see4 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 176) ^ secrets[4],
//                             rhyme_read64<bswap>(p + 184) ^ see4);
//                     see5 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 192) ^ secrets[5],
//                             rhyme_read64<bswap>(p + 200) ^ see5);
//                     see6 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 208) ^ secrets[6],
//                             rhyme_read64<bswap>(p + 216) ^ see6);
//                     p   += 224; i -= 224;
//                 }
//                 if (i > 112) {
//                     seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p      ) ^ secrets[0],
//                             rhyme_read64<bswap>(p +   8) ^ seed);
//                     see1 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  16) ^ secrets[1],
//                             rhyme_read64<bswap>(p +  24) ^ see1);
//                     see2 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  32) ^ secrets[2],
//                             rhyme_read64<bswap>(p +  40) ^ see2);
//                     see3 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  48) ^ secrets[3],
//                             rhyme_read64<bswap>(p +  56) ^ see3);
//                     see4 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  64) ^ secrets[4],
//                             rhyme_read64<bswap>(p +  72) ^ see4);
//                     see5 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  80) ^ secrets[5],
//                             rhyme_read64<bswap>(p +  88) ^ see5);
//                     see6 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  96) ^ secrets[6],
//                             rhyme_read64<bswap>(p + 104) ^ see6);
//                     p   += 112; i -= 112;
//                 }
//             } else {
//                 do {
//                     seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p      ) ^ secrets[0],
//                             rhyme_read64<bswap>(p +   8) ^ seed);
//                     see1 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  16) ^ secrets[1],
//                             rhyme_read64<bswap>(p +  24) ^ see1);
//                     see2 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  32) ^ secrets[2],
//                             rhyme_read64<bswap>(p +  40) ^ see2);
//                     see3 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  48) ^ secrets[3],
//                             rhyme_read64<bswap>(p +  56) ^ see3);
//                     see4 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  64) ^ secrets[4],
//                             rhyme_read64<bswap>(p +  72) ^ see4);
//                     see5 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  80) ^ secrets[5],
//                             rhyme_read64<bswap>(p +  88) ^ see5);
//                     see6 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  96) ^ secrets[6],
//                             rhyme_read64<bswap>(p + 104) ^ see6);
//                     p   += 112; i -= 112;
//                 } while (i > 112);
//             }
//             seed ^= see1;
//             see2 ^= see3;
//             see4 ^= see5;
//             seed ^= see6;
//             see2 ^= see4;
//             seed ^= see2;
//         }
//         if (i > 16) {
//             seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p) ^ secrets[2],
//                     rhyme_read64<bswap>(p + 8) ^ seed);
//             if (i > 32) {
//                 seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 16) ^ secrets[2],
//                         rhyme_read64<bswap>(p + 24) ^ seed);
//                 if (i > 48) {
//                     seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 32) ^ secrets[1],
//                             rhyme_read64<bswap>(p + 40) ^ seed);
//                     if (i > 64) {
//                         seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 48) ^ secrets[1],
//                                 rhyme_read64<bswap>(p + 56) ^ seed);
//                         if (i > 80) {
//                             seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 64) ^ secrets[2],
//                                     rhyme_read64<bswap>(p + 72) ^ seed);
//                             if (i > 96) {
//                                 seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 80) ^ secrets[1],
//                                         rhyme_read64<bswap>(p + 88) ^ seed);
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//         a = rhyme_read64<bswap>(p + i - 16) ^ i;
//         b = rhyme_read64<bswap>(p + i -  8);
//     }
//     a ^= secrets[1];
//     b ^= seed;
//     rhyme_mum<isProtected>(&a, &b);
//     return rhyme_mix<isProtected>(a ^ secrets[7], b ^ secrets[1] ^ i);
// }
//
// /*
//  *  rhymehashMicro main function.
//  *
//  *  @param key     Buffer to be hashed.
//  *  @param len     @key length, in bytes.
//  *  @param seed    64-bit seed used to alter the hash result predictably.
//  *  @param secrets Array of 8 64-bit secrets used to alter hash result predictably.
//  *                 Note that secrets[5] and secrets[6] are unused here.
//  *
//  *  Returns a 64-bit hash.
//  */
// template <bool bswap, bool isProtected>
// static inline uint64_t rhymehashMicro( const void * key, size_t len, uint64_t seed, const uint64_t * secrets ) {
//     const uint8_t * p = (const uint8_t *)key;
//     size_t          i = len;
//     uint64_t        a, b;
//
//     seed ^= rhyme_mix<isProtected>(seed ^ secrets[2], secrets[1]);
//
//     if (likely(len <= 16)) {
//         if (len >= 4) {
//             seed ^= len;
//             if (len >= 8) {
//                 const uint8_t * plast = p + len - 8;
//                 a = rhyme_read64<bswap>(p    );
//                 b = rhyme_read64<bswap>(plast);
//             } else {
//                 const uint8_t * plast = p + len - 4;
//                 a = rhyme_read32<bswap>(p    );
//                 b = rhyme_read32<bswap>(plast);
//             }
//         } else if (likely(len > 0)) {
//             a = (((uint64_t)p[0]) << 45) | p[len - 1];
//             b = p[len >> 1];
//         } else {
//             a = b = 0;
//         }
//     } else {
//         if (len > 80) {
//             uint64_t see1 = seed, see2 = seed;
//             uint64_t see3 = seed, see4 = seed;
//             do {
//                 seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p      ) ^ secrets[0],
//                         rhyme_read64<bswap>(p +   8) ^ seed);
//                 see1 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  16) ^ secrets[1],
//                         rhyme_read64<bswap>(p +  24) ^ see1);
//                 see2 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  32) ^ secrets[2],
//                         rhyme_read64<bswap>(p +  40) ^ see2);
//                 see3 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  48) ^ secrets[3],
//                         rhyme_read64<bswap>(p +  56) ^ see3);
//                 see4 = rhyme_mix<isProtected>(rhyme_read64<bswap>(p +  64) ^ secrets[4],
//                         rhyme_read64<bswap>(p +  72) ^ see4);
//                 p   += 80; i -= 80;
//             } while (i > 80);
//             seed ^= see1;
//             see2 ^= see3;
//             seed ^= see4;
//             seed ^= see2;
//         }
//         if (i > 16) {
//             seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p) ^ secrets[2],
//                     rhyme_read64<bswap>(p + 8) ^ seed);
//             if (i > 32) {
//                 seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 16) ^ secrets[2],
//                         rhyme_read64<bswap>(p + 24) ^ seed);
//                 if (i > 48) {
//                     seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 32) ^ secrets[1],
//                             rhyme_read64<bswap>(p + 40) ^ seed);
//                     if (i > 64) {
//                         seed = rhyme_mix<isProtected>(rhyme_read64<bswap>(p + 48) ^ secrets[1],
//                                 rhyme_read64<bswap>(p + 56) ^ seed);
//                     }
//                 }
//             }
//         }
//         a = rhyme_read64<bswap>(p + i - 16) ^ i;
//         b = rhyme_read64<bswap>(p + i -  8);
//     }
//     a ^= secrets[1];
//     b ^= seed;
//     rhyme_mum<isProtected>(&a, &b);
//     return rhyme_mix<isProtected>(a ^ secrets[7], b ^ secrets[1] ^ i);
// }

/*
 *  rhymehashNano main function.
 *
 *  @param key     Buffer to be hashed.
 *  @param len     @key length, in bytes.
 *  @param seed    64-bit seed used to alter the hash result predictably.
 *  @param secrets Array of 8 32-bit secrets used to alter hash result predictably.
 *                 Note that secrets[3] through secrets[6] are unused here.
 *
 *  Returns a 32-bit hash.
 */
template <bool bswap, bool isProtected>
static inline uint32_t rhymehashNano( const void * key, size_t len, uint64_t seed, const uint32_t * secrets ) {
    const uint8_t * p = (const uint8_t *)key;
    size_t          i = len;
    uint32_t        a, b;

    seed ^= ((seed << 32 | seed >> 32) ^ 0x5555555555555555UL) * ((seed << 16 | seed >> 48) ^ 0xAAAAAAAAAAAAAAAAUL);
    uint32_t s = (uint32_t)(seed ^ seed >> 20 ^ seed >> 42);
    if (likely(len <= 4)) {
        if (len >= 2) {
            s ^= len;
            const uint8_t * plast = p + len - 2;
            a = rhyme_read16<bswap>(p    );
            b = rhyme_read16<bswap>(plast) ^ secrets[0];
        } else if (len == 1) {
            a = p[0];
            b = secrets[1];
        } else {
            a = b = 0;
        }
    } else {
        if (len > 12) {
            uint32_t see1 = s, see2 = s;
            do {
                s    = rhyme_mix<isProtected>(rhyme_read16<bswap>(p      ) ^ secrets[0],
                        rhyme_read16<bswap>(p +   2) ^ s)   + ROTL32(s   , 13);
                see1 = rhyme_mix<isProtected>(rhyme_read16<bswap>(p +  4) ^ secrets[1],
                        rhyme_read16<bswap>(p +  6) ^ see1) + ROTL32(see1, 9);
                see2 = rhyme_mix<isProtected>(rhyme_read16<bswap>(p +  8) ^ secrets[2],
                        rhyme_read16<bswap>(p +  10) ^ see2) + ROTL32(see2, 5);
                p   += 12; i -= 12;
            } while (i > 12);
            s ^= see1;
            s ^= see2;
        }
        if (i > 4) {
            s = rhyme_mix<isProtected>(rhyme_read16<bswap>(p) ^ secrets[2],
                    rhyme_read16<bswap>(p + 2) ^ s);
            if (i > 8) {
                s = rhyme_mix<isProtected>(rhyme_read16<bswap>(p + 4) ^ secrets[2],
                        rhyme_read16<bswap>(p + 6) ^ s);
            }
        }
        a = rhyme_read16<bswap>(p + i - 4) ^ i;
        b = rhyme_read16<bswap>(p + i - 2);
    }
    a ^= secrets[1];
    b ^= s;
    a = rhyme_mix<isProtected>(a ^ secrets[7], b ^ secrets[1]);
    a ^= a >> 16;
    a *= 0x21f0aaad;
    a ^= a >> 15;
    a *= 0xd35a2d97;
    a ^= a >> 15;
    return a;
}

//-----------------------------------------------------------------------------
// template <bool bswap, bool isProtected, bool unrolled>
// static void RhymeHash64( const void * in, const size_t len, const seed_t seed, void * out ) {
//     if (isLE()) {
//         PUT_U64<bswap>(rhymehash<false, isProtected, unrolled>(in, len,
//                 (uint64_t)seed, rhyme_secret), (uint8_t *)out, 0);
//     } else {
//         PUT_U64<bswap>(rhymehash<true, isProtected, unrolled>(in, len,
//                 (uint64_t)seed, rhyme_secret), (uint8_t *)out, 0);
//     }
// }
//
// template <bool bswap, bool isProtected>
// static void RhymeHashMicro64( const void * in, const size_t len, const seed_t seed, void * out ) {
//     if (isLE()) {
//         PUT_U64<bswap>(rhymehashMicro<false, isProtected>(in, len, (uint64_t)seed, rhyme_secret), (uint8_t *)out, 0);
//     } else {
//         PUT_U64<bswap>(rhymehashMicro<true, isProtected>(in, len, (uint64_t)seed, rhyme_secret), (uint8_t *)out, 0);
//     }
// }

template <bool bswap, bool isProtected>
static void RhymeHashNano32( const void * in, const size_t len, const seed_t seed, void * out ) {
    if (isLE()) {
        PUT_U32<bswap>(rhymehashNano<false, isProtected>(in, len, (uint64_t)seed, rhyme_secret), (uint8_t *)out, 0);
    } else {
        PUT_U32<bswap>(rhymehashNano<true, isProtected>(in, len, (uint64_t)seed, rhyme_secret), (uint8_t *)out, 0);
    }
}

//-----------------------------------------------------------------------------
REGISTER_FAMILY(rhymehash,
   $.src_url    = "https://github.com/Nicoshev/rhymehash",
   $.src_status = HashFamilyInfo::SRC_ACTIVE
 );

// REGISTER_HASH(rhymehash,
//    $.desc       = "rhymehash v0, 64-bit",
//    $.sort_order = 0,
//    $.hash_flags =
//      0,
//    $.impl_flags =
//          FLAG_IMPL_MULTIPLY  |
//          FLAG_IMPL_LICENSE_MIT,
//    $.bits = 64,
//    $.verification_LE = 0x1FDC65EE,
//    $.verification_BE = 0xB2DB16B5,
//    $.hashfn_native   = RhymeHash64<false, false, true>,
//    $.hashfn_bswap    = RhymeHash64<true, false, true>
// );
//
// REGISTER_HASH(rhymehash__protected,
//    $.desc       = "rhymehash v0, 64-bit protected version",
//    $.sort_order = 10,
//    $.hash_flags =
//      0,
//    $.impl_flags =
//          FLAG_IMPL_MULTIPLY  |
//          FLAG_IMPL_LICENSE_MIT,
//    $.bits = 64,
//    $.verification_LE = 0x72C9270A,
//    $.verification_BE = 0x9A145308,
//    $.hashfn_native   = RhymeHash64<false, true, false>,
//    $.hashfn_bswap    = RhymeHash64<true, true, false>
// );
//
// REGISTER_HASH(rhymehash_micro,
//    $.desc       = "rhymehashMicro v0, 64-bit",
//    $.sort_order = 20,
//    $.hash_flags =
//      0,
//    $.impl_flags =
//          FLAG_IMPL_MULTIPLY  |
//          FLAG_IMPL_LICENSE_MIT,
//    $.bits = 64,
//    $.verification_LE = 0x6F183D61,
//    $.verification_BE = 0xFAAE4D8F,
//    $.hashfn_native   = RhymeHashMicro64<false, false>,
//    $.hashfn_bswap    = RhymeHashMicro64<true, false>
// );
//
// REGISTER_HASH(rhymehash_micro__protected,
//    $.desc       = "rhymehashMicro v0, 64-bit protected version",
//    $.sort_order = 30,
//    $.hash_flags =
//      0,
//    $.impl_flags =
//          FLAG_IMPL_MULTIPLY  |
//          FLAG_IMPL_LICENSE_MIT,
//    $.bits = 64,
//    $.verification_LE = 0xC7F9987C,
//    $.verification_BE = 0xDC04682C,
//    $.hashfn_native   = RhymeHashMicro64<false, true>,
//    $.hashfn_bswap    = RhymeHashMicro64<true, true>
// );

REGISTER_HASH(rhymehash_nano,
   $.desc       = "rhymehashNano v0, 32-bit",
   $.sort_order = 40,
   $.hash_flags =
     0,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY |
         FLAG_IMPL_ROTATE |
         FLAG_IMPL_LICENSE_MIT,
   $.bits = 32,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = RhymeHashNano32<false, false>,
   $.hashfn_bswap    = RhymeHashNano32<true, false>
);

REGISTER_HASH(rhymehash_nano__protected,
   $.desc       = "rhymehashNano v0, 32-bit protected version",
   $.sort_order = 50,
   $.hash_flags =
     0,
   $.impl_flags =
         FLAG_IMPL_MULTIPLY  |
         FLAG_IMPL_ROTATE |
         FLAG_IMPL_LICENSE_MIT,
   $.bits = 32,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = RhymeHashNano32<false, true>,
   $.hashfn_bswap    = RhymeHashNano32<true, true>
);
