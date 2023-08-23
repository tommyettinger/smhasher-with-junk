// metrohash128.cpp
//
// The MIT License (MIT)
//
// Copyright (c) 2015 J. Andrew Rogers
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "metrohash.h"

void metrohash128_1(const uint8_t * key, uint64_t len, uint64_t seed, uint8_t * out)
{
    static const uint64_t k0 = 0xC83A91E1;
    static const uint64_t k1 = 0x8648DBDB;
    static const uint64_t k2 = 0x7BDEC03B;
    static const uint64_t k3 = 0x2F5870A5;

    const uint8_t * ptr = reinterpret_cast<const uint8_t*>(key);
    const uint8_t * const end = ptr + len;
    
    uint64_t v[4];
    
    v[0] = ((static_cast<uint64_t>(seed) - k0) * k3) + len;
    v[1] = ((static_cast<uint64_t>(seed) + k1) * k2) + len;
    
    if (len >= 32)
    {        
        v[2] = ((static_cast<uint64_t>(seed) + k0) * k2) + len;
        v[3] = ((static_cast<uint64_t>(seed) - k1) * k3) + len;

        do
        {
            v[0] += read_u64(ptr) * k0; ptr += 8; v[0] = rotate_right(v[0],29) + v[2];
            v[1] += read_u64(ptr) * k1; ptr += 8; v[1] = rotate_right(v[1],29) + v[3];
            v[2] += read_u64(ptr) * k2; ptr += 8; v[2] = rotate_right(v[2],29) + v[0];
            v[3] += read_u64(ptr) * k3; ptr += 8; v[3] = rotate_right(v[3],29) + v[1];
        }
        while (ptr <= (end - 32));

        v[2] ^= rotate_right(((v[0] + v[3]) * k0) + v[1], 26) * k1;
        v[3] ^= rotate_right(((v[1] + v[2]) * k1) + v[0], 26) * k0;
        v[0] ^= rotate_right(((v[0] + v[2]) * k0) + v[3], 26) * k1;
        v[1] ^= rotate_right(((v[1] + v[3]) * k1) + v[2], 30) * k0;
    }
    
    if ((end - ptr) >= 16)
    {
        v[0] += read_u64(ptr) * k2; ptr += 8; v[0] = rotate_right(v[0],33) * k3;
        v[1] += read_u64(ptr) * k2; ptr += 8; v[1] = rotate_right(v[1],33) * k3;
        v[0] ^= rotate_right((v[0] * k2) + v[1], 17) * k1;
        v[1] ^= rotate_right((v[1] * k3) + v[0], 17) * k0;
    }
    
    if ((end - ptr) >= 8)
    {
        v[0] += read_u64(ptr) * k2; ptr += 8; v[0] = rotate_right(v[0],33) * k3;
        v[0] ^= rotate_right((v[0] * k2) + v[1], 20) * k1;
    }
    
    if ((end - ptr) >= 4)
    {
        v[1] += read_u32(ptr) * k2; ptr += 4; v[1] = rotate_right(v[1],33) * k3;
        v[1] ^= rotate_right((v[1] * k3) + v[0], 18) * k0;
    }
    
    if ((end - ptr) >= 2)
    {
        v[0] += read_u16(ptr) * k2; ptr += 2; v[0] = rotate_right(v[0],33) * k3;
        v[0] ^= rotate_right((v[0] * k2) + v[1], 24) * k1;
    }
    
    if ((end - ptr) >= 1)
    {
        v[1] += read_u8 (ptr) * k2; v[1] = rotate_right(v[1],33) * k3;
        v[1] ^= rotate_right((v[1] * k3) + v[0], 24) * k0;
    }
    
    v[0] += rotate_right((v[0] * k0) + v[1], 13);
    v[1] += rotate_right((v[1] * k1) + v[0], 37);
    v[0] += rotate_right((v[0] * k2) + v[1], 13);
    v[1] += rotate_right((v[1] * k3) + v[0], 37);

    memcpy(out, v, 16);
}


void metrohash128_2(const uint8_t * key, uint64_t len, uint64_t seed, uint8_t * out)
{
    static const uint64_t k0 = 0xD6D018F5;
    static const uint64_t k1 = 0xA2AA033B;
    static const uint64_t k2 = 0x62992FC1;
    static const uint64_t k3 = 0x30BC5B29; 

    const uint8_t * ptr = reinterpret_cast<const uint8_t*>(key);
    const uint8_t * const end = ptr + len;
    
    uint64_t v[4];
    
    v[0] = ((static_cast<uint64_t>(seed) - k0) * k3) + len;
    v[1] = ((static_cast<uint64_t>(seed) + k1) * k2) + len;
    
    if (len >= 32)
    {        
        v[2] = ((static_cast<uint64_t>(seed) + k0) * k2) + len;
        v[3] = ((static_cast<uint64_t>(seed) - k1) * k3) + len;

        do
        {
            v[0] += read_u64(ptr) * k0; ptr += 8; v[0] = rotate_right(v[0],29) + v[2];
            v[1] += read_u64(ptr) * k1; ptr += 8; v[1] = rotate_right(v[1],29) + v[3];
            v[2] += read_u64(ptr) * k2; ptr += 8; v[2] = rotate_right(v[2],29) + v[0];
            v[3] += read_u64(ptr) * k3; ptr += 8; v[3] = rotate_right(v[3],29) + v[1];
        }
        while (ptr <= (end - 32));

        v[2] ^= rotate_right(((v[0] + v[3]) * k0) + v[1], 33) * k1;
        v[3] ^= rotate_right(((v[1] + v[2]) * k1) + v[0], 33) * k0;
        v[0] ^= rotate_right(((v[0] + v[2]) * k0) + v[3], 33) * k1;
        v[1] ^= rotate_right(((v[1] + v[3]) * k1) + v[2], 33) * k0;
    }
    
    if ((end - ptr) >= 16)
    {
        v[0] += read_u64(ptr) * k2; ptr += 8; v[0] = rotate_right(v[0],29) * k3;
        v[1] += read_u64(ptr) * k2; ptr += 8; v[1] = rotate_right(v[1],29) * k3;
        v[0] ^= rotate_right((v[0] * k2) + v[1], 29) * k1;
        v[1] ^= rotate_right((v[1] * k3) + v[0], 29) * k0;
    }
    
    if ((end - ptr) >= 8)
    {
        v[0] += read_u64(ptr) * k2; ptr += 8; v[0] = rotate_right(v[0],29) * k3;
        v[0] ^= rotate_right((v[0] * k2) + v[1], 29) * k1;
    }
    
    if ((end - ptr) >= 4)
    {
        v[1] += read_u32(ptr) * k2; ptr += 4; v[1] = rotate_right(v[1],29) * k3;
        v[1] ^= rotate_right((v[1] * k3) + v[0], 25) * k0;
    }
    
    if ((end - ptr) >= 2)
    {
        v[0] += read_u16(ptr) * k2; ptr += 2; v[0] = rotate_right(v[0],29) * k3;
        v[0] ^= rotate_right((v[0] * k2) + v[1], 30) * k1;
    }
    
    if ((end - ptr) >= 1)
    {
          v[1] += read_u8 (ptr) * k2; v[1] = rotate_right(v[1],29) * k3;
          v[1] ^= rotate_right((v[1] * k3) + v[0], 18) * k0;
    }
    
    v[0] += rotate_right((v[0] * k0) + v[1], 33);
    v[1] += rotate_right((v[1] * k1) + v[0], 33);
    v[0] += rotate_right((v[0] * k2) + v[1], 33);
    v[1] += rotate_right((v[1] * k3) + v[0], 33);

    memcpy(out, v, 16);
}


const uint64_t _specklep0 = 0xa0761d6478bd642full, _specklep1 = 0xe7037ed1a0b428dbull, _specklep2 = 0x8ebc6af09c88c6e3ull;
const uint64_t _specklep3 = 0x589965cc75374cc3ull, _specklep4 = 0x1d8e4e27c47d124full, _specklep5 = 0xeb44accab455d165ull;

static inline uint64_t _specklemum(const uint64_t A, const uint64_t B) {
	uint64_t r = (A ^ rotate_left(B, 39)) * (B ^ rotate_left(A, 39));
	return r ^ (r >> 32);
}

static inline uint64_t _speckler08(const uint8_t *p){ uint8_t  v; memcpy(&v, p, 1); return v; }
static inline uint64_t _speckler16(const uint8_t *p){ uint16_t v; memcpy(&v, p, 2); return v; }
static inline uint64_t _speckler32(const uint8_t *p){ uint32_t v; memcpy(&v, p, 4); return v; }
static inline uint64_t _speckler64(const uint8_t *p){ uint64_t v; memcpy(&v, p, 8); return v; }
static inline uint64_t __speckler64(const uint8_t *p){ return (_speckler32(p) << 32) | _speckler32(p + 4); }


void speckleHash(const uint8_t * key, uint64_t len, uint64_t seed, uint8_t * out) {
    const uint8_t * p = key;
    uint64_t v[2];
    seed += _specklep1;
    seed ^= seed >> 29 ^ seed >> 43 ^ seed << 7 ^ seed << 53;
    v[0] = seed;
    seed += _specklep2;
    seed ^= seed >> 29 ^ seed >> 43 ^ seed << 7 ^ seed << 53;
    v[1] = seed;
    seed += _specklep3;
    seed ^= seed >> 29 ^ seed >> 43 ^ seed << 7 ^ seed << 53;

	uint64_t i, a = seed + _specklep4, b = seed + _specklep3, c = seed + _specklep2, d = seed + _specklep1;
	for (i = 0; i + 32 <= len; i += 32, p += 32)
	{
		a ^= (_speckler64(p     )) + _specklep1; a = rotate_left(a, 23) * _specklep3;
		b ^= (_speckler64(p +  8)) + _specklep2; b = rotate_left(b, 25) * _specklep4;
		c ^= (_speckler64(p + 16)) + _specklep3; c = rotate_left(c, 29) * _specklep5;
		d ^= (_speckler64(p + 24)) + _specklep4; d = rotate_left(d, 31) * _specklep1;
		seed += a + b + c + d;
	}
	seed += _specklep5;
	switch (len & 31) {
	case	1:	seed = _specklemum(seed, _speckler08(p) ^ _specklep1);	break;
	case	2:	seed = _specklemum(seed, _speckler16(p) ^ _specklep1);	break;
	case	3:	seed = _specklemum(seed, ((_speckler16(p) << 8) | _speckler08(p + 2)) ^ _specklep1);	break;
	case	4:	seed = _specklemum(seed, _speckler32(p) ^ _specklep1);	break;
	case	5:	seed = _specklemum(seed, ((_speckler32(p) << 8) | _speckler08(p + 4)) ^ _specklep1);	break;
	case	6:	seed = _specklemum(seed, ((_speckler32(p) << 16) | _speckler16(p + 4)) ^ _specklep1);	break;
	case	7:	seed = _specklemum(seed, ((_speckler32(p) << 24) | (_speckler16(p + 4) << 8) | _speckler08(p + 6)) ^ _specklep1);	break;
	case	8:	seed = _specklemum(seed, __speckler64(p) ^ _specklep1);	break;
	case	9:	seed = _specklemum(__speckler64(p) + seed, _speckler08(p + 8) ^ _specklep2);	break;
	case	10:	seed = _specklemum(__speckler64(p) + seed, _speckler16(p + 8) ^ _specklep2);	break;
	case	11:	seed = _specklemum(__speckler64(p) + seed, ((_speckler16(p + 8) << 8) | _speckler08(p + 8 + 2)) ^ _specklep2);	break;
	case	12:	seed = _specklemum(__speckler64(p) + seed, _speckler32(p + 8) ^ _specklep2);	break;
	case	13:	seed = _specklemum(__speckler64(p) + seed, ((_speckler32(p + 8) << 8) | _speckler08(p + 8 + 4)) ^ _specklep2);	break;
	case	14:	seed = _specklemum(__speckler64(p) + seed, ((_speckler32(p + 8) << 16) | _speckler16(p + 8 + 4)) ^ _specklep2);	break;
	case	15:	seed = _specklemum(__speckler64(p) + seed, ((_speckler32(p + 8) << 24) | (_speckler16(p + 8 + 4) << 8) | _speckler08(p + 8 + 6)) ^ _specklep2);	break;
	case	16:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2);	break;
	case	17:	seed = _specklemum(__speckler64(p) + seed, _speckler32(p + 8) + _specklep2) + _specklemum(_speckler32(p + 12) ^ seed, _speckler08(p + 16) ^ _specklep3);	break;
	case	18:	seed = _specklemum(__speckler64(p) + seed, _speckler32(p + 8) + _specklep2) + _specklemum(_speckler32(p + 12) ^ seed, _speckler16(p + 16) ^ _specklep3);	break;
	case	19:	seed = _specklemum(__speckler64(p) + seed, _speckler32(p + 8) + _specklep2) + _specklemum(_speckler32(p + 12) ^ seed, ((_speckler16(p + 16) << 8) | _speckler08(p + 16 + 2)) ^ _specklep3);	break;
	case	20:	seed = _specklemum(__speckler64(p) + seed, _speckler32(p + 8) + _specklep2) + _specklemum(_speckler32(p + 12) ^ seed, _speckler32(p + 16) ^ _specklep3);	break;
	case	21:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(_speckler16(p + 16) ^ seed, ((_speckler16(p + 18) << 8) | _speckler08(p + 16 + 4)) ^ _specklep3);	break;
	case	22:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(_speckler16(p + 16) ^ seed, (_speckler32(p + 18) << 16) ^ _specklep3);	break;
	case	23:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(_specklep4 + seed, ((_speckler32(p + 16) << 24) | (_speckler16(p + 16 + 4) << 8) | _speckler08(p + 16 + 6)) ^ _specklep3);	break;
	case	24:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(__speckler64(p + 16) + seed, seed ^ _specklep3);	break;
	case	25:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(__speckler64(p + 16) ^ seed, _speckler08(p + 24) ^ _specklep4);	break;
	case	26:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(__speckler64(p + 16) ^ seed, _speckler16(p + 24) ^ _specklep4);	break;
	case	27:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(__speckler64(p + 16) ^ seed, ((_speckler16(p + 24) << 8) | _speckler08(p + 24 + 2)) ^ _specklep4);	break;
	case	28:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(__speckler64(p + 16) ^ seed, _speckler32(p + 24) ^ _specklep4);	break;
	case	29:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(__speckler64(p + 16) ^ seed, ((_speckler32(p + 24) << 8) | _speckler08(p + 24 + 4)) ^ _specklep4);	break;
	case	30:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(__speckler64(p + 16) ^ seed, ((_speckler32(p + 24) << 16) | _speckler16(p + 24 + 4)) ^ _specklep4);	break;
	case	31:	seed = _specklemum(__speckler64(p) + seed, __speckler64(p + 8) + _specklep2) + _specklemum(__speckler64(p + 16) ^ seed, ((_speckler32(p + 24) << 24) | (_speckler16(p + 24 + 4) << 8) | _speckler08(p + 24 + 6)) ^ _specklep4);	break;
	}
    v[0] += (seed ^ rotate_left(seed, 31) ^ rotate_left(seed, 17)) * _specklep0 ^ len;
    v[1] += (seed ^ rotate_left(seed, 21) ^ rotate_left(seed, 53)) * (_specklep5 ^ len << 1);
    memcpy(out, v, 16);
}
