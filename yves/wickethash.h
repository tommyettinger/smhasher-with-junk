/*
    Wickethash takes (optimally) 64-bit inputs and produces a 64-bit hash as its result.
    It is an edited version of wyhash that uses at most 64-bit math instead of 128-bit.
    Original Author: Wang Yi <godspeed_china@yeah.net>
    Wickethash Variant Author: Tommy Ettinger <tommy.ettinger@gmail.com>
*/

#ifndef wickethash_version_1
#define wickethash_version_1
#include <stdint.h>
#include <math.h>
const uint64_t _wicketp0 = 0xa0761d6478bd642full, _wicketp1 = 0xe7037ed1a0b428dbull, _wicketp2 = 0x8ebc6af09c88c6e3ull;
const uint64_t _wicketp3 = 0x589965cc75374cc3ull, _wicketp4 = 0x1d8e4e27c47d124full, _wicketp5 = 0xeb44accab455d165ull;

static inline uint64_t _wicketmum(const uint64_t A, const uint64_t B) {
	uint64_t r = (A ^ ROTL64(B, 39)) * (B ^ ROTL64(A, 39));
	return r ^ (r >> 32);
}

static inline uint64_t _wicketr08(const uint8_t *p){ uint8_t  v; memcpy(&v, p, 1); return v; }
static inline uint64_t _wicketr16(const uint8_t *p){ uint16_t v; memcpy(&v, p, 2); return v; }
static inline uint64_t _wicketr32(const uint8_t *p){ uint32_t v; memcpy(&v, p, 4); return v; }
static inline uint64_t _wicketr64(const uint8_t *p){ uint64_t v; memcpy(&v, p, 8); return v; }
//static inline uint64_t __wicketr64(const uint8_t *p){ return (_wicketr32(p) << 32) | _wicketr32(p + 4); }

static inline uint64_t wickethash(const void* key, uint64_t len, uint64_t seed) {
	const uint8_t *p = (const uint8_t*)key;
	seed += _wicketp1;
	seed ^= seed >> 23 ^ seed >> 48 ^ seed << 7 ^ seed << 53;
//	uint64_t i, a = seed ^ _wicketp4, b = ROTL64(seed, 17) ^ _wicketp3, c = ROTL64(seed, 31) ^ _wicketp2, d = ROTL64(seed, 47) ^ _wicketp1;
	uint64_t i, a = seed + _wicketp4, b = seed + _wicketp3, c = seed + _wicketp2, d = seed + _wicketp1;
	for (i = 0; i + 32 <= len; i += 32, p += 32)
	{
		//a += (_wicketr64(p     )) * _wicketp1;// a += ROTL64(a, 23);// a *= _wicketp3;
		//b += (_wicketr64(p +  8)) * _wicketp2;// b += ROTL64(b, 25);// b *= _wicketp4;
		//c += (_wicketr64(p + 16)) * _wicketp3;// c += ROTL64(c, 29);// c *= _wicketp5;
		//d += (_wicketr64(p + 24)) * _wicketp4;// d += ROTL64(d, 31);// d *= _wicketp1;
		//uint64_t t = a + _wicketp0;
		a += _wicketr64(p     ); a += ROTL64(a, 29); a *= _wicketp1;
		b += _wicketr64(p +  8); b += ROTL64(b, 37); b *= _wicketp2;
		c += _wicketr64(p + 16); c += ROTL64(c, 41); c *= _wicketp3;
		d += _wicketr64(p + 24); d += ROTL64(d, 21); d *= _wicketp4;
		seed += a^b^c^d;
	}
	seed += _wicketp5;
	switch (len & 31) {
	case	1:	seed = _wicketmum(seed, _wicketr08(p) ^ _wicketp1);	break;
	case	2:	seed = _wicketmum(seed, _wicketr16(p) ^ _wicketp1);	break;
	case	3:	seed = _wicketmum(seed, ((_wicketr16(p) << 8) | _wicketr08(p + 2)) ^ _wicketp1);	break;
	case	4:	seed = _wicketmum(seed, _wicketr32(p) ^ _wicketp1);	break;
	case	5:	seed = _wicketmum(seed, ((_wicketr32(p) << 8) | _wicketr08(p + 4)) ^ _wicketp1);	break;
	case	6:	seed = _wicketmum(seed, ((_wicketr32(p) << 16) | _wicketr16(p + 4)) ^ _wicketp1);	break;
	case	7:	seed = _wicketmum(seed, ((_wicketr32(p) << 24) | (_wicketr16(p + 4) << 8) | _wicketr08(p + 6)) ^ _wicketp1);	break;
	case	8:	seed = _wicketmum(seed, _wicketr64(p) ^ _wicketp1);	break;
	case	9:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr08(p + 8) ^ _wicketp2);	break;
	case	10:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr16(p + 8) ^ _wicketp2);	break;
	case	11:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr16(p + 8) << 8) | _wicketr08(p + 8 + 2)) ^ _wicketp2);	break;
	case	12:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) ^ _wicketp2);	break;
	case	13:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr32(p + 8) << 8) | _wicketr08(p + 8 + 4)) ^ _wicketp2);	break;
	case	14:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr32(p + 8) << 16) | _wicketr16(p + 8 + 4)) ^ _wicketp2);	break;
	case	15:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr32(p + 8) << 24) | (_wicketr16(p + 8 + 4) << 8) | _wicketr08(p + 8 + 6)) ^ _wicketp2);	break;
	case	16:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2);	break;
	case	17:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, _wicketr08(p + 16) ^ _wicketp3);	break;
	case	18:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, _wicketr16(p + 16) ^ _wicketp3);	break;
	case	19:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, ((_wicketr16(p + 16) << 8) | _wicketr08(p + 16 + 2)) ^ _wicketp3);	break;
	case	20:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, _wicketr32(p + 16) ^ _wicketp3);	break;
	case	21:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr16(p + 16) ^ seed, ((_wicketr16(p + 18) << 8) | _wicketr08(p + 16 + 4)) ^ _wicketp3);	break;
	case	22:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr16(p + 16) ^ seed, (_wicketr32(p + 18) << 16) ^ _wicketp3);	break;
	case	23:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketp4 + seed, ((_wicketr32(p + 16) << 24) | (_wicketr16(p + 16 + 4) << 8) | _wicketr08(p + 16 + 6)) ^ _wicketp3);	break;
	case	24:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) + seed, seed ^ _wicketp3);	break;
	case	25:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, _wicketr08(p + 24) ^ _wicketp4);	break;
	case	26:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, _wicketr16(p + 24) ^ _wicketp4);	break;
	case	27:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr16(p + 24) << 8) | _wicketr08(p + 24 + 2)) ^ _wicketp4);	break;
	case	28:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, _wicketr32(p + 24) ^ _wicketp4);	break;
	case	29:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr32(p + 24) << 8) | _wicketr08(p + 24 + 4)) ^ _wicketp4);	break;
	case	30:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr32(p + 24) << 16) | _wicketr16(p + 24 + 4)) ^ _wicketp4);	break;
	case	31:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr32(p + 24) << 24) | (_wicketr16(p + 24 + 4) << 8) | _wicketr08(p + 24 + 6)) ^ _wicketp4);	break;
	}
	seed = (seed ^ seed << 16) * (len ^ _wicketp0 ^ seed >> 32);
	return seed - (seed >> 31) + (seed << 33);
}



static inline uint32_t wicket32(const void* key, uint64_t len, uint64_t seed) {
	const uint8_t *p = (const uint8_t*)key;
//	uint64_t i, a = seed ^ _wicketp4, b = ROTL64(seed, 17) ^ _wicketp3, c = ROTL64(seed, 31) ^ _wicketp2, d = ROTL64(seed, 47) ^ _wicketp1;
	uint64_t i, a = seed + _wicketp4, b = seed + _wicketp3, c = seed + _wicketp2, d = seed + _wicketp1;
	for (i = 0; i + 32 <= len; i += 32, p += 32)
	{
		a ^= (_wicketr64(p     )) * _wicketp1; a = ROTL64(a, 23); a *= _wicketp3;
		b ^= (_wicketr64(p +  8)) * _wicketp2; b = ROTL64(b, 25); b *= _wicketp4;
		c ^= (_wicketr64(p + 16)) * _wicketp3; c = ROTL64(c, 29); c *= _wicketp5;
		d ^= (_wicketr64(p + 24)) * _wicketp4; d = ROTL64(d, 31); d *= _wicketp1;
		seed += a + b + c + d;
	}
	seed += _wicketp5;
	switch (len & 31) {
	case	1:	seed = _wicketmum(seed, _wicketr08(p) ^ _wicketp1);	break;
	case	2:	seed = _wicketmum(seed, _wicketr16(p) ^ _wicketp1);	break;
	case	3:	seed = _wicketmum(seed, ((_wicketr16(p) << 8) | _wicketr08(p + 2)) ^ _wicketp1);	break;
	case	4:	seed = _wicketmum(seed, _wicketr32(p) ^ _wicketp1);	break;
	case	5:	seed = _wicketmum(seed, ((_wicketr32(p) << 8) | _wicketr08(p + 4)) ^ _wicketp1);	break;
	case	6:	seed = _wicketmum(seed, ((_wicketr32(p) << 16) | _wicketr16(p + 4)) ^ _wicketp1);	break;
	case	7:	seed = _wicketmum(seed, ((_wicketr32(p) << 24) | (_wicketr16(p + 4) << 8) | _wicketr08(p + 6)) ^ _wicketp1);	break;
	case	8:	seed = _wicketmum(seed, _wicketr64(p) ^ _wicketp1);	break;
	case	9:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr08(p + 8) ^ _wicketp2);	break;
	case	10:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr16(p + 8) ^ _wicketp2);	break;
	case	11:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr16(p + 8) << 8) | _wicketr08(p + 8 + 2)) ^ _wicketp2);	break;
	case	12:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) ^ _wicketp2);	break;
	case	13:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr32(p + 8) << 8) | _wicketr08(p + 8 + 4)) ^ _wicketp2);	break;
	case	14:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr32(p + 8) << 16) | _wicketr16(p + 8 + 4)) ^ _wicketp2);	break;
	case	15:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr32(p + 8) << 24) | (_wicketr16(p + 8 + 4) << 8) | _wicketr08(p + 8 + 6)) ^ _wicketp2);	break;
	case	16:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2);	break;
	case	17:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, _wicketr08(p + 16) ^ _wicketp3);	break;
	case	18:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, _wicketr16(p + 16) ^ _wicketp3);	break;
	case	19:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, ((_wicketr16(p + 16) << 8) | _wicketr08(p + 16 + 2)) ^ _wicketp3);	break;
	case	20:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, _wicketr32(p + 16) ^ _wicketp3);	break;
	case	21:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr16(p + 16) ^ seed, ((_wicketr16(p + 18) << 8) | _wicketr08(p + 16 + 4)) ^ _wicketp3);	break;
	case	22:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr16(p + 16) ^ seed, (_wicketr32(p + 18) << 16) ^ _wicketp3);	break;
	case	23:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketp4 + seed, ((_wicketr32(p + 16) << 24) | (_wicketr16(p + 16 + 4) << 8) | _wicketr08(p + 16 + 6)) ^ _wicketp3);	break;
	case	24:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) + seed, seed ^ _wicketp3);	break;
	case	25:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, _wicketr08(p + 24) ^ _wicketp4);	break;
	case	26:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, _wicketr16(p + 24) ^ _wicketp4);	break;
	case	27:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr16(p + 24) << 8) | _wicketr08(p + 24 + 2)) ^ _wicketp4);	break;
	case	28:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, _wicketr32(p + 24) ^ _wicketp4);	break;
	case	29:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr32(p + 24) << 8) | _wicketr08(p + 24 + 4)) ^ _wicketp4);	break;
	case	30:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr32(p + 24) << 16) | _wicketr16(p + 24 + 4)) ^ _wicketp4);	break;
	case	31:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr32(p + 24) << 24) | (_wicketr16(p + 24 + 4) << 8) | _wicketr08(p + 24 + 6)) ^ _wicketp4);	break;
	}
	seed = (seed ^ seed << 16) * (len ^ _wicketp0 ^ seed >> 32);
	return (uint32_t)(seed - (seed >> 32));
}
#endif

//#ifndef wickethash_version_1
//#define wickethash_version_1
//#include <stdint.h>
//#include <math.h>
//const uint64_t _wicketp0 = 0xa0761d6478bd642full, _wicketp1 = 0xe7037ed1a0b428dbull, _wicketp2 = 0x8ebc6af09c88c6e3ull;
//const uint64_t _wicketp3 = 0x589965cc75374cc3ull, _wicketp4 = 0x1d8e4e27c47d124full, _wicketp5 = 0xeb44accab455d165ull;
//
//static inline uint64_t _wicketmum(const uint64_t A, const uint64_t B) {
//	uint64_t r = (A ^ ROTL64(B, 39)) * (B ^ ROTL64(A, 39));
//	return r ^ (r >> 32);
//}
//
//static inline uint64_t _wicketr08(const uint8_t *p){ uint8_t  v; memcpy(&v, p, 1); return v; }
//static inline uint64_t _wicketr16(const uint8_t *p){ uint16_t v; memcpy(&v, p, 2); return v; }
//static inline uint64_t _wicketr32(const uint8_t *p){ uint32_t v; memcpy(&v, p, 4); return v; }
//static inline uint64_t _wicketr64(const uint8_t *p){ uint64_t v; memcpy(&v, p, 8); return v; }
//static inline uint64_t _wicketr64(const uint8_t *p){ return (_wicketr32(p) << 32) | _wicketr32(p + 4); }
//
//static inline uint64_t wickethash(const void* key, uint64_t len, uint64_t seed) {
//	const uint8_t *p = (const uint8_t*)key;
//	seed += _wicketp1;
//	seed ^= seed >> 23 ^ seed >> 48 ^ seed << 7 ^ seed << 53;
//	uint64_t i, a = seed ^ _wicketp4, b = ROTL64(seed, 17) ^ _wicketp3, c = ROTL64(seed, 31) ^ _wicketp2, d = ROTL64(seed, 47) ^ _wicketp1;
//	for (i = 0; i + 32 <= len; i += 32, p += 32)
//	{
//		a = (_wicketr64(p     ) ^ a) * _wicketp1; a = ROTL64(a, 23); a *= _wicketp3;
//		b = (_wicketr64(p +  8) ^ b) * _wicketp2; b = ROTL64(b, 25); b *= _wicketp4;
//		c = (_wicketr64(p + 16) ^ c) * _wicketp3; c = ROTL64(c, 29); c *= _wicketp5;
//		d = (_wicketr64(p + 24) ^ d) * _wicketp4; d = ROTL64(d, 31); d *= _wicketp1;
//		seed += a + b + c + d;
//	}
//	seed += _wicketp5;
//	switch (len & 31) {
//	case	1:	seed = _wicketmum(seed, _wicketr08(p) ^ _wicketp1);	break;
//	case	2:	seed = _wicketmum(seed, _wicketr16(p) ^ _wicketp1);	break;
//	case	3:	seed = _wicketmum(seed, ((_wicketr16(p) << 8) | _wicketr08(p + 2)) ^ _wicketp1);	break;
//	case	4:	seed = _wicketmum(seed, _wicketr32(p) ^ _wicketp1);	break;
//	case	5:	seed = _wicketmum(seed, ((_wicketr32(p) << 8) | _wicketr08(p + 4)) ^ _wicketp1);	break;
//	case	6:	seed = _wicketmum(seed, ((_wicketr32(p) << 16) | _wicketr16(p + 4)) ^ _wicketp1);	break;
//	case	7:	seed = _wicketmum(seed, ((_wicketr32(p) << 24) | (_wicketr16(p + 4) << 8) | _wicketr08(p + 6)) ^ _wicketp1);	break;
//	case	8:	seed = _wicketmum(seed, _wicketr64(p) ^ _wicketp1);	break;
//	case	9:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr08(p + 8) ^ _wicketp2);	break;
//	case	10:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr16(p + 8) ^ _wicketp2);	break;
//	case	11:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr16(p + 8) << 8) | _wicketr08(p + 8 + 2)) ^ _wicketp2);	break;
//	case	12:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) ^ _wicketp2);	break;
//	case	13:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr32(p + 8) << 8) | _wicketr08(p + 8 + 4)) ^ _wicketp2);	break;
//	case	14:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr32(p + 8) << 16) | _wicketr16(p + 8 + 4)) ^ _wicketp2);	break;
//	case	15:	seed = _wicketmum(_wicketr64(p) + seed, ((_wicketr32(p + 8) << 24) | (_wicketr16(p + 8 + 4) << 8) | _wicketr08(p + 8 + 6)) ^ _wicketp2);	break;
//	case	16:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2);	break;
//	case	17:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, _wicketr08(p + 16) ^ _wicketp3);	break;
//	case	18:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, _wicketr16(p + 16) ^ _wicketp3);	break;
//	case	19:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, ((_wicketr16(p + 16) << 8) | _wicketr08(p + 16 + 2)) ^ _wicketp3);	break;
//	case	20:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr32(p + 8) + _wicketp2) + _wicketmum(_wicketr32(p + 12) ^ seed, _wicketr32(p + 16) ^ _wicketp3);	break;
//	case	21:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr16(p + 16) ^ seed, ((_wicketr16(p + 18) << 8) | _wicketr08(p + 16 + 4)) ^ _wicketp3);	break;
//	case	22:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr16(p + 16) ^ seed, (_wicketr32(p + 18) << 16) ^ _wicketp3);	break;
//	case	23:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketp4 + seed, ((_wicketr32(p + 16) << 24) | (_wicketr16(p + 16 + 4) << 8) | _wicketr08(p + 16 + 6)) ^ _wicketp3);	break;
//	case	24:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) + seed, seed ^ _wicketp3);	break;
//	case	25:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, _wicketr08(p + 24) ^ _wicketp4);	break;
//	case	26:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, _wicketr16(p + 24) ^ _wicketp4);	break;
//	case	27:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr16(p + 24) << 8) | _wicketr08(p + 24 + 2)) ^ _wicketp4);	break;
//	case	28:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, _wicketr32(p + 24) ^ _wicketp4);	break;
//	case	29:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr32(p + 24) << 8) | _wicketr08(p + 24 + 4)) ^ _wicketp4);	break;
//	case	30:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr32(p + 24) << 16) | _wicketr16(p + 24 + 4)) ^ _wicketp4);	break;
//	case	31:	seed = _wicketmum(_wicketr64(p) + seed, _wicketr64(p + 8) + _wicketp2) + _wicketmum(_wicketr64(p + 16) ^ seed, ((_wicketr32(p + 24) << 24) | (_wicketr16(p + 24 + 4) << 8) | _wicketr08(p + 24 + 6)) ^ _wicketp4);	break;
//	}
//	seed = (seed ^ seed << 16) * (len ^ _wicketp0 ^ seed >> 32);
//	return seed - (seed >> 31) + (seed << 33);
//}
//