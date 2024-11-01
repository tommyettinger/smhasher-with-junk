/*
    Woothash takes (optimally) 64-bit inputs and produces a 64-bit hash as its result.
    It is an edited version of wyhash that uses at most 64-bit math instead of 128-bit.
    Original Author: Wang Yi <godspeed_china@yeah.net>
    Woothash Variant Author: Tommy Ettinger <tommy.ettinger@gmail.com>
*/
#ifndef woothash_version_3
#define woothash_version_3
#include <stdint.h>
#include <math.h>

#ifndef O1HASH_LITTLE_ENDIAN
#if defined(_WIN32) || defined(__LITTLE_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define O1HASH_LITTLE_ENDIAN 1
#elif defined(__BIG_ENDIAN__) || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define O1HASH_LITTLE_ENDIAN 0
#endif
#endif
#if (O1HASH_LITTLE_ENDIAN)
static inline uint64_t _wootr08(const uint8_t *p){ uint8_t  v; memcpy(&v, p, 1); return v; }
static inline uint64_t _wootr16(const uint8_t *p){ uint16_t v; memcpy(&v, p, 2); return v; }
static inline uint64_t _wootr32(const uint8_t *p){ uint32_t v; memcpy(&v, p, 4); return v; }
static inline uint64_t _wootr64(const uint8_t *p){ uint64_t v; memcpy(&v, p, 8); return v; }
static inline uint64_t __wootr64(const uint8_t *p){ return (_wootr32(p) << 32) | _wootr32(p + 4); }

#else
#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
static inline uint64_t _wootr08(const uint8_t *p){ uint8_t  v; memcpy(&v, p, 1); return v; }
static inline uint64_t _wootr16(const uint8_t *p){ uint16_t v; memcpy(&v, p, 2); return __builtin_bswap16(v); }
static inline uint64_t _wootr32(const uint8_t *p){ uint32_t v; memcpy(&v, p, 4); return __builtin_bswap32(v); }
static inline uint64_t _wootr64(const uint8_t *p){ uint64_t v; memcpy(&v, p, 8); return __builtin_bswap64(v); }
static inline uint64_t __wootr64(const uint8_t *p){ return _wootr32(p) | (_wootr32(p + 4) << 32); }

#elif defined(_MSC_VER)
static inline uint64_t _wootr08(const uint8_t *p){ uint8_t  v; memcpy(&v, p, 1); return v; }
static inline uint64_t _wootr16(const uint8_t *p){ uint16_t v; memcpy(&v, p, 2); return _byteswap_ushort(v); }
static inline uint64_t _wootr32(const uint8_t *p){ uint32_t v; memcpy(&v, p, 4); return _byteswap_ulong(v); }
static inline uint64_t _wootr64(const uint8_t *p){ uint64_t v; memcpy(&v, p, 8); return _byteswap_uint64(v); }
static inline uint64_t __wootr64(const uint8_t *p){ return _wootr32(p) | (_wootr32(p + 4) << 32); }
#endif
#endif

const uint64_t _wootp0 = 0xa0761d6478bd642full, _wootp1 = 0xe7037ed1a0b428dbull, _wootp2 = 0x8ebc6af09c88c6e3ull;
const uint64_t _wootp3 = 0x589965cc75374cc3ull, _wootp4 = 0x1d8e4e27c47d124full, _wootp5 = 0xeb44accab455d165ull;

// static inline uint64_t _wootmum(const uint64_t A, const uint64_t B) {
// 	uint64_t r = (A + ROTL64(B, 47)) * (B + ROTL64(A, 47));
// 	return r ^ r >> 28;
// }
static inline uint64_t _wootmum(const uint64_t A, const uint64_t B) {
	uint64_t r = (A ^ ROTL64(B, 39)) * (B ^ ROTL64(A, 39));
	return r ^ (r >> 32);
}

static inline uint64_t woothash(const void* key, uint64_t len, int64_t seed){
	const uint8_t *p = (const uint8_t*)key;
	seed += _wootp1;
	seed ^= seed >> 23 ^ seed >> 47 ^ seed << 7 ^ seed << 51;
	uint64_t i, a = seed + _wootp4, b = seed + _wootp3, c = seed + _wootp2, d = seed + _wootp1;
	for (i = 0; i + 32 <= len; i += 32, p += 32)
	{
		a ^= (_wootr64(p     )) * _wootp1; a = ROTL64(a, 23); a *= _wootp3;
		b ^= (_wootr64(p +  8)) * _wootp2; b = ROTL64(b, 25); b *= _wootp4;
		c ^= (_wootr64(p + 16)) * _wootp3; c = ROTL64(c, 29); c *= _wootp5;
		d ^= (_wootr64(p + 24)) * _wootp4; d = ROTL64(d, 31); d *= _wootp1;
		seed += a + b + c + d;
	}
	seed += _wootp5;
	switch (len & 31) {
	case	1:	seed = _wootmum(seed, _wootr08(p) ^ _wootp1);	break;
	case	2:	seed = _wootmum(seed, _wootr16(p) ^ _wootp1);	break;
	case	3:	seed = _wootmum(seed, ((_wootr16(p) << 8) | _wootr08(p + 2)) ^ _wootp1);	break;
	case	4:	seed = _wootmum(seed, _wootr32(p) ^ _wootp1);	break;
	case	5:	seed = _wootmum(seed, ((_wootr32(p) << 8) | _wootr08(p + 4)) ^ _wootp1);	break;
	case	6:	seed = _wootmum(seed, ((_wootr32(p) << 16) | _wootr16(p + 4)) ^ _wootp1);	break;
	case	7:	seed = _wootmum(seed, ((_wootr32(p) << 24) | (_wootr16(p + 4) << 8) | _wootr08(p + 6)) ^ _wootp1);	break;
	case	8:	seed = _wootmum(seed, __wootr64(p) ^ _wootp1);	break;
	case	9:	seed = _wootmum(__wootr64(p) + seed, _wootr08(p + 8) ^ _wootp2);	break;
	case	10:	seed = _wootmum(__wootr64(p) + seed, _wootr16(p + 8) ^ _wootp2);	break;
	case	11:	seed = _wootmum(__wootr64(p) + seed, ((_wootr16(p + 8) << 8) | _wootr08(p + 8 + 2)) ^ _wootp2);	break;
	case	12:	seed = _wootmum(__wootr64(p) + seed, _wootr32(p + 8) ^ _wootp2);	break;
	case	13:	seed = _wootmum(__wootr64(p) + seed, ((_wootr32(p + 8) << 8) | _wootr08(p + 8 + 4)) ^ _wootp2);	break;
	case	14:	seed = _wootmum(__wootr64(p) + seed, ((_wootr32(p + 8) << 16) | _wootr16(p + 8 + 4)) ^ _wootp2);	break;
	case	15:	seed = _wootmum(__wootr64(p) + seed, ((_wootr32(p + 8) << 24) | (_wootr16(p + 8 + 4) << 8) | _wootr08(p + 8 + 6)) ^ _wootp2);	break;
	case	16:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) ^ _wootp2);	break;
	case	17:	seed = _wootmum(__wootr64(p) + seed, _wootr32(p + 8) + _wootp2) + _wootmum(_wootr32(p + 12) ^ seed, _wootr08(p + 16) ^ _wootp3);	break;
	case	18:	seed = _wootmum(__wootr64(p) + seed, _wootr32(p + 8) + _wootp2) + _wootmum(_wootr32(p + 12) ^ seed, _wootr16(p + 16) ^ _wootp3);	break;
	case	19:	seed = _wootmum(__wootr64(p) + seed, _wootr32(p + 8) + _wootp2) + _wootmum(_wootr32(p + 12) ^ seed, ((_wootr16(p + 16) << 8) | _wootr08(p + 16 + 2)) ^ _wootp3);	break;
	case	20:	seed = _wootmum(__wootr64(p) + seed, _wootr32(p + 8) + _wootp2) + _wootmum(_wootr32(p + 12) ^ seed, _wootr32(p + 16) ^ _wootp3);	break;
	case	21:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(_wootr16(p + 16) ^ seed, ((_wootr16(p + 18) << 8) | _wootr08(p + 16 + 4)) ^ _wootp3);	break;
	case	22:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(_wootr16(p + 16) ^ seed, (_wootr32(p + 18) << 16) ^ _wootp3);	break;
	case	23:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(_wootp4 + seed, ((_wootr32(p + 16) << 24) | (_wootr16(p + 16 + 4) << 8) | _wootr08(p + 16 + 6)) ^ _wootp3);	break;
	case	24:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(__wootr64(p + 16) + seed, seed ^ _wootp3);	break;
	case	25:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(__wootr64(p + 16) ^ seed, _wootr08(p + 24) ^ _wootp4);	break;
	case	26:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(__wootr64(p + 16) ^ seed, _wootr16(p + 24) ^ _wootp4);	break;
	case	27:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(__wootr64(p + 16) ^ seed, ((_wootr16(p + 24) << 8) | _wootr08(p + 24 + 2)) ^ _wootp4);	break;
	case	28:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(__wootr64(p + 16) ^ seed, _wootr32(p + 24) ^ _wootp4);	break;
	case	29:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(__wootr64(p + 16) ^ seed, ((_wootr32(p + 24) << 8) | _wootr08(p + 24 + 4)) ^ _wootp4);	break;
	case	30:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(__wootr64(p + 16) ^ seed, ((_wootr32(p + 24) << 16) | _wootr16(p + 24 + 4)) ^ _wootp4);	break;
	case	31:	seed = _wootmum(__wootr64(p) + seed, __wootr64(p + 8) + _wootp2) + _wootmum(__wootr64(p + 16) ^ seed, ((_wootr32(p + 24) << 24) | (_wootr16(p + 24 + 4) << 8) | _wootr08(p + 24 + 6)) ^ _wootp4);	break;
	}
	seed = (seed ^ seed >> 16) * (_wootp0 ^ (len + seed) << 4);
    return seed ^ seed >> 23 ^ seed >> 42;
	// return seed - (seed >> 31) + (seed << 33);

	// seed = (seed ^ seed << 23 ^ seed << 48 ^ seed >> 7 ^ seed >> 53 ^ len) * _wootp5;
    // seed = (seed ^ seed >> 31) * _wootp2;
	// return seed ^ seed >> 29;

    // seed ^= seed >> 27 ^ len ^ _wootp0;
    // seed *= 0x3C79AC492BA7B653ULL;
    // seed ^= seed >> 33;
    // seed *= 0x1C69B3F74AC4AE35ULL;
    // return seed ^ seed >> 27;
}
#endif

