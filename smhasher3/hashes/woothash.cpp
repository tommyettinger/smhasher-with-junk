/*
 * woothash
 *
 * author: Tommy Ettinger, 2024-07-05, github.com/tommyettinger
 * based mostly on Wyhash 1.0 by Wang Yi
 * license: The Unlicense
 */
#include "Platform.h"
#include "Hashlib.h"

/*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

		  0     1     2     3     4     5     6     7     8     9    10    11    12
		----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
		 4182  1314   598   320   147    80    39    23    11    16     9     1     1

		 13    14    15    16    17    18    19    20    21    22    23    24    25+
		----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
			5     2     1     2     2     1     0     1     2     0     0     0   126

----------------------------------------------------------------------------------------------
Summary for: woot
Overall result: FAIL            ( 160 / 188 passed)
Failures:
	Sparse              : [4/16, 3/48]
	TwoBytes            : [48]
	SeedBlockLen        : [9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 25, 26, 27, 28, 29, 30, 31]
	SeedBlockOffset     : [0, 1, 2, 3, 4, 5]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 502.333620 seconds

With seed issues eliminated, but others remaining or worse:
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

		  0     1     2     3     4     5     6     7     8     9    10    11    12
		----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
		 4467  1231   575   245   155    61    49    18    13     5     1     0     2

		 13    14    15    16    17    18    19    20    21    22    23    24    25+
		----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
			0     4     0     2     1     2     1     0     0     0     0     0    51

----------------------------------------------------------------------------------------------
Summary for: woot
Overall result: FAIL            ( 180 / 188 passed)
Failures:
	Sparse              : [3/48]
	Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE]]
	TwoBytes            : [20]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 480.338468 seconds

OK. Now it's down to Permutation tests failing.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

		  0     1     2     3     4     5     6     7     8     9    10    11    12
		----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
		 4409  1303   587   287   161    52    33    18     6     2     2     1     0

		 13    14    15    16    17    18    19    20    21    22    23    24    25+
		----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
			0     0     0     0     1     0     0     0     0     0     0     0    21

----------------------------------------------------------------------------------------------
Summary for: woot
Overall result: FAIL            ( 186 / 188 passed)
Failures:
	Permutation         : [4-bytes [3 high bits; LE], 4-bytes [3 high+low bits; LE]]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 589.268897 seconds

Now it fails just one test, Sparse with 48 bytes and 3 bits set.
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

		  0     1     2     3     4     5     6     7     8     9    10    11    12
		----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
		 4387  1233   650   287   165    86    41    13     8     7     3     2     0

		 13    14    15    16    17    18    19    20    21    22    23    24    25+
		----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
			0     0     0     0     0     0     0     0     0     0     0     0     1

----------------------------------------------------------------------------------------------
Summary for: woot
Overall result: FAIL            ( 187 / 188 passed)
Failures:
	Sparse              : [3/48]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 569.113429 seconds

After changing the whole tail section, the sparse problem is gone, but... well:
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

		  0     1     2     3     4     5     6     7     8     9    10    11    12
		----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
		 4331  1274   639   330   151    77    45    10    15     6     2     0     0

		 13    14    15    16    17    18    19    20    21    22    23    24    25+
		----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
			0     0     0     0     0     0     0     0     0     0     0     0     3

----------------------------------------------------------------------------------------------
Summary for: woot
Overall result: FAIL            ( 185 / 188 passed)
Failures:
	Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; BE]]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 462.914872 seconds
*/

const uint64_t _wootp0 = 0xa0761d6478bd642full, _wootp1 = 0xe7037ed1a0b428dbull, _wootp2 = 0x8ebc6af09c88c6e3ull;
const uint64_t _wootp3 = 0x589965cc75374cc3ull, _wootp4 = 0x1d8e4e27c47d124full, _wootp5 = 0xeb44accab455d165ull;

static inline uint64_t _wootmum(const uint64_t A, const uint64_t B) {
	uint64_t r = (A ^ ROTL64(B, 39)) * (B ^ ROTL64(A, 41));
	//return r ^ (r >> 32);
	return r ^ ROTL64(r, 23) ^ ROTL64(r, 47);

	// don't use this... it fails everything.
	//uint64_t a = A * _wootp1 + _wootp4;
	//uint64_t b = B * _wootp3 + _wootp2;
	//return (a ^ ROTL64(a, 23) ^ ROTL64(a, 40)) * _wootp1 - (b ^ ROTL64(b, 22) ^ ROTL64(b, 43)) * _wootp3 + _wootp5;
	// still fails...
	//uint64_t r = A * _wootp1 + B * _wootp3;
	//r ^= (r * r | 1ULL);
	//return (r ^ ROTL64(r, 22) ^ ROTL64(r, 43));
	// fails...
	//uint64_t a = A + _wootp4;
	//uint64_t b = _wootp2 - B;
	//a ^= (a * a | 1) ^ b ^ (b * b | 1);
	//return (a ^ ROTL64(a, 23) ^ ROTL64(a, 40));
}

template <bool bswap>
static inline uint64_t _wootr64 (const uint8_t* p) {
	return GET_U64<bswap>(p, 0);
}

template <bool bswap>
static inline uint64_t _wootr32 (const uint8_t* p) {
	return GET_U32<bswap>(p, 0);
}

template <bool bswap>
static inline uint64_t _wootr16 (const uint8_t* p) {
	return GET_U16<bswap>(p, 0);
}

template <bool bswap>
static inline uint64_t _wootr08 (const uint8_t* p) {
	return p[0];
}

template <bool bswap>
static inline uint64_t __wootr64 (const uint8_t* p) {
	return ((uint64_t)GET_U32<bswap>(p, 0) << 32) | GET_U32<bswap>(p, 4);
	//return GET_U64<bswap>(p, 0);
}

template <bool bswap>
static inline uint64_t woothash(const void* key, uint64_t len, uint64_t seed) {
	const uint8_t* p = (const uint8_t*)key;
	//seed += _wootp1;
	//seed ^= seed >> 23 ^ seed >> 48 ^ seed << 7 ^ seed << 53;
	seed = seed ^ ROTL64(seed, 29) ^ ROTL64(seed, 47) ^ len;
	//uint64_t i, a = seed + _wootp4, b = ROTL64(seed, 17) + _wootp3, c = ROTL64(seed, 31) + _wootp2, d = ROTL64(seed, 47) + _wootp1;
	uint64_t i, a = ROTL64(seed, 53) ^ _wootp4, b = ROTL64(seed, 13) ^ _wootp3, c = ROTL64(seed, 43) ^ _wootp2, d = ROTL64(seed, 23) ^ _wootp1;
	//uint64_t i, a = seed + _wootp4, b = a ^ _wootp3, c = b - _wootp2, d = c ^ _wootp1;
	//uint64_t i, a = seed + _wootp4, b = seed + _wootp3, c = seed + _wootp2, d = seed + _wootp1;
	for (i = 0; i + 32 <= len; i += 32, p += 32)
	{
		a += (_wootr64<bswap>(p     )) * _wootp1; a = ROTL64(a, 27); a *= _wootp3;
		b += (_wootr64<bswap>(p +  8)) * _wootp2; b = ROTL64(b, 28); b *= _wootp4;
		c += (_wootr64<bswap>(p + 16)) * _wootp3; c = ROTL64(c, 30); c *= _wootp5;
		d += (_wootr64<bswap>(p + 24)) * _wootp4; d = ROTL64(d, 31); d *= _wootp1;
	}
	//seed ^= a ^ b ^ c ^ d;
	//seed += _wootp5;
	//switch (len & 31) {
	//case	0:	seed += _wootmum(seed, _wootp4 + seed); break;
	//case	1:	seed += _wootmum(seed, _wootr08<bswap>(p) ^ _wootp1);	break;
	//case	2:	seed += _wootmum(seed, _wootr16<bswap>(p) ^ _wootp1);	break;
	//case	3:	seed += _wootmum(seed, ((_wootr16<bswap>(p) << 8) | _wootr08<bswap>(p + 2)) ^ _wootp1);	break;
	//case	4:	seed += _wootmum(seed, _wootr32<bswap>(p) ^ _wootp1);	break;
	//case	5:	seed += _wootmum(seed, ((_wootr32<bswap>(p) << 8) | _wootr08<bswap>(p + 4)) ^ _wootp1);	break;
	//case	6:	seed += _wootmum(seed, ((_wootr32<bswap>(p) << 16) | _wootr16<bswap>(p + 4)) ^ _wootp1);	break;
	//case	7:	seed += _wootmum(seed, ((_wootr32<bswap>(p) << 24) | (_wootr16<bswap>(p + 4) << 8) | _wootr08<bswap>(p + 6)) ^ _wootp1);	break;
	//case	8:	seed += _wootmum(seed, __wootr64<bswap>(p) ^ _wootp1);	break;
	//case	9:	seed += _wootmum(__wootr64<bswap>(p) - seed, _wootr08<bswap>(p + 8) ^ _wootp2);	break;
	//case	10:	seed += _wootmum(__wootr64<bswap>(p) - seed, _wootr16<bswap>(p + 8) ^ _wootp2);	break;
	//case	11:	seed += _wootmum(__wootr64<bswap>(p) - seed, ((_wootr16<bswap>(p + 8) << 8) | _wootr08<bswap>(p + 8 + 2)) ^ _wootp2);	break;
	//case	12:	seed += _wootmum(__wootr64<bswap>(p) - seed, _wootr32<bswap>(p + 8) ^ _wootp2);	break;
	//case	13:	seed += _wootmum(__wootr64<bswap>(p) - seed, ((_wootr32<bswap>(p + 8) << 8) | _wootr08<bswap>(p + 8 + 4)) ^ _wootp2);	break;
	//case	14:	seed += _wootmum(__wootr64<bswap>(p) - seed, ((_wootr32<bswap>(p + 8) << 16) | _wootr16<bswap>(p + 8 + 4)) ^ _wootp2);	break;
	//case	15:	seed += _wootmum(__wootr64<bswap>(p) - seed, ((_wootr32<bswap>(p + 8) << 24) | (_wootr16<bswap>(p + 8 + 4) << 8) | _wootr08<bswap>(p + 8 + 6)) ^ _wootp2);	break;
	//case	16:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) ^ _wootp2);	break;
	//case	17:	seed += _wootmum(__wootr64<bswap>(p) - seed, _wootr32<bswap>(p + 8) + _wootp2) + _wootmum(_wootr32<bswap>(p + 12) ^ seed, _wootr08<bswap>(p + 16) ^ _wootp3);	break;
	//case	18:	seed += _wootmum(__wootr64<bswap>(p) - seed, _wootr32<bswap>(p + 8) + _wootp2) + _wootmum(_wootr32<bswap>(p + 12) ^ seed, _wootr16<bswap>(p + 16) ^ _wootp3);	break;
	//case	19:	seed += _wootmum(__wootr64<bswap>(p) - seed, _wootr32<bswap>(p + 8) + _wootp2) + _wootmum(_wootr32<bswap>(p + 12) ^ seed, ((_wootr16<bswap>(p + 16) << 8) | _wootr08<bswap>(p + 16 + 2)) ^ _wootp3);	break;
	//case	20:	seed += _wootmum(__wootr64<bswap>(p) - seed, _wootr32<bswap>(p + 8) + _wootp2) + _wootmum(_wootr32<bswap>(p + 12) ^ seed, _wootr32<bswap>(p + 16) ^ _wootp3);	break;
	//case	21:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(_wootr16<bswap>(p + 16) ^ seed, ((_wootr16<bswap>(p + 18) << 8) | _wootr08<bswap>(p + 16 + 4)) ^ _wootp3);	break;
	//case	22:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(_wootr16<bswap>(p + 16) ^ seed, (_wootr32<bswap>(p + 18) << 16) ^ _wootp3);	break;
	//case	23:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(_wootp4 + seed, ((_wootr32<bswap>(p + 16) << 24) | (_wootr16<bswap>(p + 16 + 4) << 8) | _wootr08<bswap>(p + 16 + 6)) ^ _wootp3);	break;
	//case	24:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(__wootr64<bswap>(p + 16) + seed, seed ^ _wootp3);	break;
	//case	25:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(__wootr64<bswap>(p + 16) ^ seed, _wootr08<bswap>(p + 24) ^ _wootp4);	break;
	//case	26:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(__wootr64<bswap>(p + 16) ^ seed, _wootr16<bswap>(p + 24) ^ _wootp4);	break;
	//case	27:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(__wootr64<bswap>(p + 16) ^ seed, ((_wootr16<bswap>(p + 24) << 8) | _wootr08<bswap>(p + 24 + 2)) ^ _wootp4);	break;
	//case	28:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(__wootr64<bswap>(p + 16) ^ seed, _wootr32<bswap>(p + 24) ^ _wootp4);	break;
	//case	29:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(__wootr64<bswap>(p + 16) ^ seed, ((_wootr32<bswap>(p + 24) << 8) | _wootr08<bswap>(p + 24 + 4)) ^ _wootp4);	break;
	//case	30:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(__wootr64<bswap>(p + 16) ^ seed, ((_wootr32<bswap>(p + 24) << 16) | _wootr16<bswap>(p + 24 + 4)) ^ _wootp4);	break;
	//case	31:	seed += _wootmum(__wootr64<bswap>(p) - seed, __wootr64<bswap>(p + 8) + _wootp2) + _wootmum(__wootr64<bswap>(p + 16) ^ seed, ((_wootr32<bswap>(p + 24) << 24) | (_wootr16<bswap>(p + 24 + 4) << 8) | _wootr08<bswap>(p + 24 + 6)) ^ _wootp4);	break;
	//}

	switch (len & 31) {
	case	1:	a += _wootr08<bswap>(p);	break;
	case	2:	a += _wootr16<bswap>(p);	break;
	case	3:	a += ((_wootr16<bswap>(p) << 8) | _wootr08<bswap>(p + 2));	break;
	case	4:	a += _wootr32<bswap>(p);	break;
	case	5:	a += ((_wootr32<bswap>(p) << 8) | _wootr08<bswap>(p + 4));	break;
	case	6:	a += ((_wootr32<bswap>(p) << 16) | _wootr16<bswap>(p + 4));	break;
	case	7:	a += ((_wootr32<bswap>(p) << 24) | (_wootr16<bswap>(p + 4) << 8) | _wootr08<bswap>(p + 6));	break;
	case	8:	a += __wootr64<bswap>(p);	break;
	case	9:	a += __wootr64<bswap>(p); b += _wootr08<bswap>(p + 8);	break;
	case	10:	a += __wootr64<bswap>(p); b += _wootr16<bswap>(p + 8);	break;
	case	11:	a += __wootr64<bswap>(p); b += ((_wootr16<bswap>(p + 8) << 8) | _wootr08<bswap>(p + 8 + 2));	break;
	case	12:	a += __wootr64<bswap>(p); b += _wootr32<bswap>(p + 8);	break;
	case	13:	a += __wootr64<bswap>(p); b += ((_wootr32<bswap>(p + 8) << 8) | _wootr08<bswap>(p + 8 + 4));	break;
	case	14:	a += __wootr64<bswap>(p); b += ((_wootr32<bswap>(p + 8) << 16) | _wootr16<bswap>(p + 8 + 4));	break;
	case	15:	a += __wootr64<bswap>(p); b += ((_wootr32<bswap>(p + 8) << 24) | (_wootr16<bswap>(p + 8 + 4) << 8) | _wootr08<bswap>(p + 8 + 4 + 2));	break;
	case	16:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8);	break;
	case	17:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += _wootr08<bswap>(p + 8 + 8);	break;
	case	18:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += _wootr16<bswap>(p + 8 + 8);	break;
	case	19:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += ((_wootr16<bswap>(p + 8 + 8) << 8) | _wootr08<bswap>(p + 8 + 8 + 2));	break;
	case	20:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += _wootr32<bswap>(p + 8 + 8);	break;
	case	21:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += _wootr32<bswap>(p + 8 + 8); d += _wootr08<bswap>(p + 8 + 8 + 4);	break;
	case	22:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += _wootr32<bswap>(p + 8 + 8); d += _wootr16<bswap>(p + 8 + 8 + 4);	break;
	case	23:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += ((_wootr32<bswap>(p + 8 + 8) << 24) | (_wootr16<bswap>(p + 8 + 8 + 4) << 8) | _wootr08<bswap>(p + 8 + 8 + 4 + 2));	break;
	case	24:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += __wootr64<bswap>(p + 8 + 8);	break;
	case	25:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += __wootr64<bswap>(p + 8 + 8); d += _wootr08<bswap>(p + 8 + 8 + 8);	break;
	case	26:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += __wootr64<bswap>(p + 8 + 8); d += _wootr16<bswap>(p + 8 + 8 + 8);	break;
	case	27:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += __wootr64<bswap>(p + 8 + 8); d += ((_wootr16<bswap>(p + 8 + 8 + 8) << 8) | _wootr08<bswap>(p + 8 + 8 + 8 + 2));	break;
	case	28:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += __wootr64<bswap>(p + 8 + 8); d += _wootr32<bswap>(p + 8 + 8 + 8);	break;
	case	29:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += __wootr64<bswap>(p + 8 + 8); d += ((_wootr32<bswap>(p + 8 + 8 + 8) << 8) | _wootr08<bswap>(p + 8 + 8 + 8 + 4));	break;
	case	30:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += __wootr64<bswap>(p + 8 + 8); d += ((_wootr32<bswap>(p + 8 + 8 + 8) << 16) | _wootr16<bswap>(p + 8 + 8 + 8 + 4));	break;
	case	31:	a += __wootr64<bswap>(p); b += __wootr64<bswap>(p + 8); c += __wootr64<bswap>(p + 8 + 8); d += ((_wootr32<bswap>(p + 8 + 8 + 8) << 24) | (_wootr16<bswap>(p + 8 + 8 + 8 + 4) << 8) | _wootr08<bswap>(p + 8 + 8 + 8 + 4 + 2));	break;
	}

	a *= _wootp1; a = ROTL64(a, 27); a *= _wootp3;
	b *= _wootp2; b = ROTL64(b, 28); b *= _wootp4;
	c *= _wootp3; c = ROTL64(c, 30); c *= _wootp5;
	d *= _wootp4; d = ROTL64(d, 31); d *= _wootp1;

	
	seed ^= a ^ b ^ c ^ d;

//	seed = (seed ^ len) * (_wootp0 ^ seed << 16);
//	return seed ^ ROTL64(seed, 31) ^ ROTL64(seed, 19);
	seed ^= ROTL64(seed, 23) ^ ROTL64(seed, 43);
	seed = seed * _wootp5 + len;
	seed ^= ROTL64(seed, 11) ^ ROTL64(seed, 50);
	return seed;
	//seed = (seed ^ seed << 16) * (len ^ _wootp0 ^ seed >> 32);
	//return seed - (seed >> 31) + (seed << 33);

}

//------------------------------------------------------------
template <bool bswap>
static void woot(const void* in, const size_t len, const seed_t seed, void* out) {
    uint64_t h = woothash<bswap>((const uint8_t*)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t*)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(woothash,
    $.src_url = "https://github.com/tommyettinger/",
    $.src_status = HashFamilyInfo::SRC_ACTIVE
);

REGISTER_HASH(woot,
    $.desc = "woothash 64-bit",
    $.hash_flags =
    0,
    $.impl_flags =
    FLAG_IMPL_MULTIPLY_64_64 |
    FLAG_IMPL_ROTATE |
    FLAG_IMPL_LICENSE_PUBLIC_DOMAIN,
    $.bits = 64,
    $.verification_LE = 0,
    $.verification_BE = 0,
    $.hashfn_native = woot<false>,
    $.hashfn_bswap = woot<true>
);
