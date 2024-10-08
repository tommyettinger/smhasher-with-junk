/* sha512.c - SHA reference implementation using C            */
/*   Written and placed in public domain by Jeffrey Walton    */

/* xlc -DTEST_MAIN sha512.c -o sha512.exe           */
/* gcc -DTEST_MAIN -std=c99 sha512.c -o sha512.exe  */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

static const uint64_t K512[] =
{
    0x428a2f98d728ae22, 0x7137449123ef65cd,
    0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
    0x3956c25bf348b538, 0x59f111f1b605d019,
    0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
    0xd807aa98a3030242, 0x12835b0145706fbe,
    0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
    0x72be5d74f27b896f, 0x80deb1fe3b1696b1,
    0x9bdc06a725c71235, 0xc19bf174cf692694,
    0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
    0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483,
    0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
    0x983e5152ee66dfab, 0xa831c66d2db43210,
    0xb00327c898fb213f, 0xbf597fc7beef0ee4,
    0xc6e00bf33da88fc2, 0xd5a79147930aa725,
    0x06ca6351e003826f, 0x142929670a0e6e70,
    0x27b70a8546d22ffc, 0x2e1b21385c26c926,
    0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
    0x650a73548baf63de, 0x766a0abb3c77b2a8,
    0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001,
    0xc24b8b70d0f89791, 0xc76c51a30654be30,
    0xd192e819d6ef5218, 0xd69906245565a910,
    0xf40e35855771202a, 0x106aa07032bbd1b8,
    0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
    0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
    0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
    0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
    0x748f82ee5defb2fc, 0x78a5636f43172f60,
    0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9,
    0xbef9a3f7b2c67915, 0xc67178f2e372532b,
    0xca273eceea26619c, 0xd186b8c721c0c207,
    0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
    0x06f067aa72176fba, 0x0a637dc5a2c898a6,
    0x113f9804bef90dae, 0x1b710b35131c471b,
    0x28db77f523047d84, 0x32caab7b40c72493,
    0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
    0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
    0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

#define ROTATE(x,y)  (((x)>>(y)) | ((x)<<(64-(y))))
#define Sigma0(x)    (ROTATE((x),28) ^ ROTATE((x),34) ^ ROTATE((x),39))
#define Sigma1(x)    (ROTATE((x),14) ^ ROTATE((x),18) ^ ROTATE((x),41))
#define sigma0(x)    (ROTATE((x), 1) ^ ROTATE((x), 8) ^ ((x)>> 7))
#define sigma1(x)    (ROTATE((x),19) ^ ROTATE((x),61) ^ ((x)>> 6))

#define Ch(x,y,z)    (((x) & (y)) ^ ((~(x)) & (z)))
#define Maj(x,y,z)   (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

/* Avoid undefined behavior                    */
/* https://stackoverflow.com/q/29538935/608639 */
uint64_t B2U64(uint8_t val, uint8_t sh)
{
    return ((uint64_t)val) << sh;
}

/* Process multiple blocks. The caller is responsible for setting the initial */
/*  state, and the caller is responsible for padding the final block.        */
void sha512_process(uint64_t state[8], const uint8_t data[], uint64_t length)
{
    uint64_t a, b, c, d, e, f, g, h, s0, s1, T1, T2;
    uint64_t X[16];

    size_t blocks = length / 128;
    while (blocks--)
    {
        a = state[0];
        b = state[1];
        c = state[2];
        d = state[3];
        e = state[4];
        f = state[5];
        g = state[6];
        h = state[7];

        unsigned int i;
        for (i = 0; i < 16; i++)
        {
            X[i] = B2U64(data[0], 56) | B2U64(data[1], 48) | B2U64(data[2], 40) | B2U64(data[3], 32) |
                    B2U64(data[4], 24) | B2U64(data[5], 16) | B2U64(data[6], 8) | B2U64(data[7], 0);
            data += 8;

            T1 = h;
            T1 += Sigma1(e);
            T1 += Ch(e, f, g);
            T1 += K512[i];
            T1 += X[i];

            T2 = Sigma0(a);
            T2 += Maj(a, b, c);

            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        for (i = 16; i < 80; i++)
        {
            s0 = X[(i + 1) & 0x0f];
            s0 = sigma0(s0);
            s1 = X[(i + 14) & 0x0f];
            s1 = sigma1(s1);

            T1 = X[i & 0xf] += s0 + s1 + X[(i + 9) & 0xf];
            T1 += h + Sigma1(e) + Ch(e, f, g) + K512[i];
            T2 = Sigma0(a) + Maj(a, b, c);

            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
        state[5] += f;
        state[6] += g;
        state[7] += h;
    }
}

#if defined(TEST_MAIN)

#include <stdio.h>
#include <string.h>
int main(int argc, char* argv[])
{
    /* empty message with padding */
    uint8_t message[128];
    memset(message, 0x00, sizeof(message));
    message[0] = 0x80;

    /* initial state */
    uint64_t state[8] = {
        0x6a09e667f3bcc908, 0xbb67ae8584caa73b,
        0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1,
        0x510e527fade682d1, 0x9b05688c2b3e6c1f,
        0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
    };

    sha512_process(state, message, sizeof(message));

    const uint8_t b1 = (uint8_t)(state[0] >> 56);
    const uint8_t b2 = (uint8_t)(state[0] >> 48);
    const uint8_t b3 = (uint8_t)(state[0] >> 40);
    const uint8_t b4 = (uint8_t)(state[0] >> 32);
    const uint8_t b5 = (uint8_t)(state[0] >> 24);
    const uint8_t b6 = (uint8_t)(state[0] >> 16);
    const uint8_t b7 = (uint8_t)(state[0] >>  8);
    const uint8_t b8 = (uint8_t)(state[0] >>  0);

    /* cf83e1357eefb8bd... */
    printf("SHA512 hash of empty message: ");
    printf("%02X%02X%02X%02X%02X%02X%02X%02X...\n",
        b1, b2, b3, b4, b5, b6, b7, b8);

    int success = ((b1 == 0xCF) && (b2 == 0x83) && (b3 == 0xE1) && (b4 == 0x35) &&
                    (b5 == 0x7E) && (b6 == 0xEF) && (b7 == 0xB8) && (b8 == 0xBD));

    if (success)
        printf("Success!\n");
    else
        printf("Failure!\n");

    return (success != 0 ? 0 : 1);
}

#endif
