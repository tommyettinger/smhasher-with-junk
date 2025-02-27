#pragma once

// murk, an edit on mx3
// author: Jon Maiga, 2020-08-03, jonkagstrom.com, @jonkagstrom
// license: CC0 license
//
// v1.0.0, 2020-08-13: https://github.com/jonmaiga/mx3/releases/tag/v1.0.0
// v2.0.0, 2020-10-22: https://github.com/jonmaiga/mx3/releases/tag/v2.0.0
// v3.0.0, 2022-04-19: https://github.com/jonmaiga/mx3/releases/tag/v3.0.0

// Passes SMHasher, Yves fork, but is mysteriously slower than mx3 v3.0.0 .
// It only removes 4 multiplications and changes a shift from 39 to 37...

#include <stddef.h>
#include <stdint.h>

namespace murk {

static const uint64_t C = 0xbea225f9eb34556d;

inline uint64_t mix(uint64_t x) {
	x ^= x >> 32;
	x *= C;
	x ^= x >> 29;
	x *= C;
	x ^= x >> 32;
	x *= C;
	x ^= x >> 29;
	return x;
}

class random {
public:
	explicit random(uint64_t seed) : _counter(mix(seed + C)) {}
	uint64_t operator()() { return mix(_counter++); }
private:
	uint64_t _counter;
};

inline uint64_t mix_stream(uint64_t h, uint64_t x) {
	x *= C;
	x ^= x >> 37;
	h += x;
	h *= C;
	return h;
}

inline uint64_t mix_stream(uint64_t h, uint64_t a, uint64_t b, uint64_t c, uint64_t d) {
	a *= C;
	b *= C;
	c *= C;
	d *= C;
	a ^= a >> 37;
	b ^= b >> 37;
	c ^= c >> 37;
	d ^= d >> 37;
	h += a;
	h *= C;
	h += b;
	h *= C;
	h += c;
	h *= C;
	h += d;
	h *= C;
	return h;
}

inline uint64_t murk_hash(const uint8_t* buf, uint64_t len, uint64_t seed) {
	const uint64_t* buf64 = reinterpret_cast<const uint64_t*>(buf);
	uint64_t h = mix_stream(seed, len + 1);
	while (len >= 64) {
		len -= 64;
		h = mix_stream(h, buf64[0], buf64[1], buf64[2], buf64[3]);
		h = mix_stream(h, buf64[4], buf64[5], buf64[6], buf64[7]);
		buf64 += 8;
	}
	
	while (len >= 8) {
		len -= 8;
		h = mix_stream(h, buf64[0]);
		buf64 += 1;
	}

	const uint8_t* const tail8 = reinterpret_cast<const uint8_t*>(buf64);
	const uint16_t* const tail16 = reinterpret_cast<const uint16_t*>(buf64);
	const uint32_t* const tail32 = reinterpret_cast<const uint32_t*>(buf64);
	switch (len) {
		case 0: return mix(h);
		case 1: return mix(mix_stream(h, tail8[0]));
		case 2: return mix(mix_stream(h, tail16[0]));
		case 3: return mix(mix_stream(h, tail16[0] | static_cast<uint64_t>(tail8[2]) << 16));
		case 4: return mix(mix_stream(h, tail32[0]));
		case 5: return mix(mix_stream(h, tail32[0] | static_cast<uint64_t>(tail8[4]) << 32));
		case 6: return mix(mix_stream(h, tail32[0] | static_cast<uint64_t>(tail16[2]) << 32));
		case 7: return mix(mix_stream(h, tail32[0] | static_cast<uint64_t>(tail16[2]) << 32 | static_cast<uint64_t>(tail8[6]) << 48));
		default: ;
	}
	return mix(h);
}

}