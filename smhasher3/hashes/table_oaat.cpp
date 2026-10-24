/*
 * One-byte-at-a-time hash based on tabulation hashing.
 * Copyright (C) 2026       Tommy Ettinger
 * Copyright (C) 2021-2022  Frank J. T. Wojcik
 * Copyright (c) 2016       aappleby
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

constexpr uint64_t TABLE[256] = {
    0x39D194BE568BB843UL, 0xF7E5890C924DDD21UL, 0xB492F903DF242A79UL, 0x46A4DD1C370F236DUL,
    0xE1AC36111AE95F8BUL, 0xA53D64547EAF8063UL, 0xF422050ADDD18FBBUL, 0x2C28C7FA62EA63C9UL,
    0x84E6E587AF10574DUL, 0xE60C54C3B18EA5EBUL, 0x9EF49B69C6820771UL, 0x2A34B7A2821BF71BUL,
    0x6C1221F1B32ED72BUL, 0x73EC048879556A7BUL, 0x4BCDEBAF067404C5UL, 0x173B22F3B58660D9UL,
    0xFDF46E2121227669UL, 0x83A9A7D23771CD09UL, 0xEA05977B81876AC9UL, 0x0C556C5026DFE1EDUL,
    0x73305C357AB644E3UL, 0x3E319E92149D47E5UL, 0x8F82D198D755F02BUL, 0xD27E225BF251047DUL,
    0x17E64B6F0AEC026BUL, 0xF0A34499BD360AF3UL, 0x885BDD46878B85E5UL, 0xA3B65D98FC824C4DUL,
    0xD44D3E3E9DC05417UL, 0x2681A5DD4BF88EC9UL, 0x57348E41B32B0FCBUL, 0x8D96EA637DD96201UL,
    0xB243FA683A4C86F3UL, 0x99B24530BFEA80F9UL, 0xBF9CBE0C42961535UL, 0x224399EB84AF89B7UL,
    0x8C944547787CF539UL, 0x47311E96A55E0677UL, 0xCCD649C3AD9E1199UL, 0x47B065729E9E02AFUL,
    0xB3C1E7EC4A812A2FUL, 0xC649167DB0AC3CCBUL, 0x4F6EAF890DC11875UL, 0xF9A41119E7BCD2A1UL,
    0xC1A82BFF157C5131UL, 0x3C2151225B1EBFCDUL, 0xC8E9B7AE0648A8F3UL, 0x714269B2B5E0E9F1UL,
    0xC73EC114228EED6BUL, 0x7B04676B619EC551UL, 0x21A39B25A29C77D9UL, 0x17375D0B1FB1AC41UL,
    0xDA130CFEB4E74681UL, 0xD93C2A6101FE65B5UL, 0x72D596C70E791079UL, 0x65E9D6F815063B13UL,
    0x564FC3ABAE013A4DUL, 0xFE88295965C2BCC3UL, 0xD5A07264BF89E171UL, 0x6489F3C2B15CC1EBUL,
    0xB814C84699FB3A57UL, 0xFD668C47A2C66C51UL, 0x9E9D1673408DBA0FUL, 0xB29F49AC42B19E0FUL,
    0x38C7DB842D84A9F5UL, 0x0D2D8AA4BE39F227UL, 0x73440C7AE72B6C53UL, 0x11FAD5D17640859FUL,
    0x6B116243CA73876FUL, 0x99A2F007B4874D5FUL, 0x8D13B718A3C64BE9UL, 0x5A35761BB4E8C607UL,
    0xE4B8C00AE565EFC5UL, 0xA969020FADD199CFUL, 0xA3BAD680F0E2CD93UL, 0xCF5586BD7843818DUL,
    0x44B4213E75E79C4BUL, 0x58F9835CDC5D3035UL, 0x53F1A09D632F3C15UL, 0x57C1259227EE45B9UL,
    0x618270776E9C8ED5UL, 0xAB9862D8CF43C297UL, 0x44E1A8A172E49BF7UL, 0xFE1906937DE55405UL,
    0x6532581F4A72C2EFUL, 0xFB8C4B894069E9B5UL, 0xCBA00EBD1B59638DUL, 0x379CC534E61B25C5UL,
    0x6F06AF0CA5D9413DUL, 0x723967113DA3B385UL, 0xC83B8F3086D156AFUL, 0xF328C38B5235F09DUL,
    0x6B1982F42F161DE9UL, 0x5067AA7C807CA7B5UL, 0x319BB1F298A91E59UL, 0xFD0F576532B090C3UL,
    0xC32FB4A9107F05CDUL, 0x149D666DB58A1FC1UL, 0xF1F8280ADEA9D60BUL, 0x9C1AE2D6802EE37BUL,
    0x5209DF16C5FCB4C1UL, 0x422FFD1ADB2E2681UL, 0x1AC2DB4D739C12C7UL, 0x4CFE131BA3A843D5UL,
    0xD72ABCC046982CF7UL, 0xB455B0E056EEC7C1UL, 0x5ED510E8F3120D5FUL, 0x66D6702634E90D9FUL,
    0x3FF626330D352585UL, 0xC1E71ACCDB0A5587UL, 0x343FA2D975B9C481UL, 0x69F9C42A71495C79UL,
    0x4E295FD6BA498187UL, 0xA3C6D983CA2B7CC1UL, 0x75C296AC3D2F0647UL, 0x384560DBBCEA81D7UL,
    0xE8C8E3EC6C4D46C5UL, 0x405DEB9BC68B132DUL, 0xA2D2D9BF9C7201B1UL, 0xD3F2D428D9984E71UL,
    0x3ECF5A540F115D85UL, 0x9C5F17DC40DD2C91UL, 0xB6D28FEA063A4731UL, 0xCE71088B9ED71273UL,
    0x0D9E54446CB3DB71UL, 0x3D199D4FD504531DUL, 0x5DDFB1804A2D8735UL, 0x28D43CDCF225233FUL,
    0x90F4CCD2134DF8E9UL, 0x13348AF61FDD308BUL, 0x0D5A71D0528F7B8DUL, 0xA352582F9D27CF09UL,
    0x1BC653C0BE5582BDUL, 0xA2FACE8EB8014775UL, 0x5A6EB3C24A78C5C3UL, 0xC26CC70F9B61B855UL,
    0x356EE4AC1FA8E0C5UL, 0xC0A3F5788CC532DDUL, 0xE4D39B7180977139UL, 0xC449933A3D50E66FUL,
    0x1BDA47B6CCC805B3UL, 0x8D87D309F16125BDUL, 0x29B921BBE885A1EBUL, 0xED183D70A34437E9UL,
    0xED0D9A1015BC4FD5UL, 0x3A7C4CE33893235DUL, 0xF206E36D521983EDUL, 0x61E09F8DA96C3B49UL,
    0x693EE62A27048BEDUL, 0x59B470228A63AFD7UL, 0xE8C9DDBC00B8ADC5UL, 0x21E48ECD28B5333FUL,
    0xE165A4E83A4B3F85UL, 0x6E471E429B2F92B1UL, 0x7E3A435C5889CB2BUL, 0xE1E775A3CA9186A1UL,
    0x377C439B6045B655UL, 0xF0961DCAF770A621UL, 0xEE57253EE44101DDUL, 0x878E8051777B68E5UL,
    0x48585A6DE459789FUL, 0xB34590EE9CF802EBUL, 0xA7E45B211A319E3DUL, 0x3281BD3D4319EACDUL,
    0x8872F08AF3C635D5UL, 0x59D1E3105F52EC59UL, 0x6158CA1E8BECA739UL, 0x9B7A70122B738F85UL,
    0x0AC0B53CE136EDB3UL, 0x9E51775ACFD0C043UL, 0xBC46F43471692FA1UL, 0xD91AE0E267B4A5C3UL,
    0x9490FDC386CF6923UL, 0xCB26B888CDE93867UL, 0x05A8D1E6D86D4FB1UL, 0x343E90AECD95918FUL,
    0x14EAB67FB858D091UL, 0xE9B7B371A81140F3UL, 0x084A771EC92E9E97UL, 0x761B61260D5F59A3UL,
    0x47B5D88C5108F65FUL, 0x54318BCDA91F5387UL, 0x09B1375660B77167UL, 0xDF473A12B8242CD7UL,
    0x09337D54E583D50FUL, 0x3481EEA9D1FB051DUL, 0x7A51A1E79827B22BUL, 0x570F5ADE83923E41UL,
    0xAC2B1CD5C2B21FE1UL, 0xFB098A76318942F7UL, 0x024B8E5A9FD74399UL, 0x370B1E63A35036F5UL,
    0x2BE63C4F6C6CB111UL, 0x78D44441FB6C672DUL, 0xD9614A25C2B9E0EFUL, 0xCA43D0F912971C5FUL,
    0x86C18BFACE290AEBUL, 0xEF73BCB382512909UL, 0x40DEEAC8E618569FUL, 0xA17710FB453AF413UL,
    0x8DA0FEC6130695BBUL, 0x7763438C9E34F125UL, 0xD98390952F4F87C9UL, 0x031BDCAAF4BE1949UL,
    0x23B21FB85AB57981UL, 0xC53B9E283EEA60C5UL, 0xF223160D76436B97UL, 0xA176566707F3205BUL,
    0xC9507D0573D49FC1UL, 0x40647EBA1AFAC533UL, 0x8FC7564257663163UL, 0x045EF753363E2923UL,
    0xA7C1CE583A452577UL, 0xF46BC62E72291273UL, 0x16371194E5B39717UL, 0x4434DC3BEA3D5607UL,
    0x0BDE66F72A4D2C41UL, 0x4DC4A8336F57BC21UL, 0x4706913A75A9DF31UL, 0x7944BFA966D4E007UL,
    0x5823A6D1E7316C37UL, 0xCDD2AFB1249E0723UL, 0xC85682AE075F3CD9UL, 0xE99089687F84FA95UL,
    0xFC718D0BF8A9A2A1UL, 0x15341C5D148D9FF9UL, 0x2D6D659D3F0308E9UL, 0x31D2E0BD4FDD4251UL,
    0x88F2CC14648FAAF7UL, 0x3E8FA10334B7EA15UL, 0x72A7C0C62EA53DE1UL, 0x1B7221B74F2DD219UL,
    0x35EA347A3B849C39UL, 0x31026D62CDE7273DUL, 0xAF35638B81C53339UL, 0xBFE9677AA2030615UL,
    0x36E1875CD9C166C9UL, 0x534F41D1CDCF882DUL, 0xF949E16231B8A1BBUL, 0x24824EE07F2BCB6BUL,
    0x71B1246453E2D3BDUL, 0x790EB60D451A3EC7UL, 0xD0370F64CF559643UL, 0xA1133D66B86BBC83UL,
    0xA1403989A3E9EAFBUL, 0xCA3B0A7C170B3717UL, 0x0E0CDC9553BD69E1UL, 0x1095EDC7FB10B0D5UL,
    0xD8FF4C9190798A95UL, 0xE42C647A231B4F57UL, 0x4370F2A1D3775385UL, 0x1D35C7894C1B5D1DUL,
    0x203A5AB347CA0DF7UL, 0xF1AA5E444F0256CFUL, 0x3CD81A590EB2CC5FUL, 0x46F2E379B41D6219UL
};

//------------------------------------------------------------
static uint64_t TableOAAT_impl( const uint8_t * data, size_t len, uint64_t seed ) {

    /*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         2201   607   309   165    84    62    30    11    14    10     6     9     3

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            6     3     3     5     6     4     4     6     4     4     2     2  3329

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: FAIL            ( 64 / 186 passed)
Failures:
    Avalanche           : [3, 4, 5, 6, 7, 8, 9, 10, 12, 16, 20, 64, 128]
    BIC                 : [3, 8, 11, 15]
    Cyclic              : [16 cycles of 4 bytes]
    Sparse              : [4/3, 4/5, 3/9, 3/10, 3/12, 3/14, 20/3, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [Long alnum last 1968-2128, Long alnum last 4016-4176, Long alnum last 8112-8272]
    TwoBytes            : [8]
    PerlinNoise         : [2]
    Bitflip             : [3, 4, 8]
    SeedZeroes          : [1280, 8448]
    SeedSparse          : [2, 3, 6, 15, 18, 31, 52, 80, 200]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]
    Seed                : [2, 3, 6, 15, 18, 31, 52, 80, 200]
    SeedAvalanche       : [4, 8, 16, 24, 32, 64, 128]
    SeedBIC             : [3, 8, 11, 15]
    SeedBitflip         : [3, 4, 8]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 333.465743 seconds
     */
    // uint64_t h = seed ^ ROTL64(seed, 25) ^ ROTL64(seed, 50) ^ TABLE[len & 255];
    //
    // for (size_t i = 0; i < len; i++) {
    //     h = TABLE[data[i]] - ROTL64(h, 5);
    // }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    // return h;


    // Passes all tests???
    /*
    ----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4477  1252   578   293   153    73    26    13     8    11     2     0     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 628.196754 seconds
    */
    // uint64_t h = seed ^ (TABLE[len & 255] + TABLE[len >> 8 & 255] + TABLE[len >> 16 & 255] + TABLE[len >> 24 & 255]);
    // h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    //
    // for (size_t i = 0; i < len; i++) {
    //     h = ROTL64(h, 11) * 0xBEA225F9EB34556DUL + TABLE[data[i]];
    // }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    // h *= 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    // return h;


    // Fails some tests, but is much faster...
    /*
    ----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         3389  1086   544   288   188   107    73    58    44    31    20    23     9

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
           12    34    10     9     3    13     6     9     3     5    10     3   912

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: FAIL            ( 159 / 186 passed)
Failures:
    Cyclic              : [16 cycles of 4 bytes]
    PerlinNoise         : [2]
    SeedSparse          : [2, 3, 6, 15, 18, 31]
    Seed                : [2, 3, 6, 15, 18, 31, 52]
    SeedAvalanche       : [4, 8, 16, 24, 32]
    SeedBIC             : [3, 8, 11, 15]
    SeedBitflip         : [3, 4, 8]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 336.754661 seconds
     */
    // uint64_t h = seed ^ (TABLE[len & 255] + TABLE[len >> 8 & 255] + TABLE[len >> 16 & 255] + TABLE[len >> 24 & 255]);
    // h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    //
    // for (size_t i = 0; i < len; i++) {
    //     h = TABLE[data[i]] - ROTL64(h, 11);
    // }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    // h *= 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    // return h;

    // Passes everything again!
    /*
    ----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4433  1308   565   289   154    72    25    25    12     6     0     0     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 341.576793 seconds
    */
    // uint64_t h = seed ^ (TABLE[len & 255] + TABLE[len >> 8 & 255] + TABLE[len >> 16 & 255] + TABLE[len >> 24 & 255]);
    // h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    // h *= 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    //
    // for (size_t i = 0; i < len; i++) {
    //     h = TABLE[(data[i] + i) & 255] - ROTL64(h, 11);
    // }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    // h *= 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    // return h;

    // Passes everything, similar but not quite as good...
    /*
    ----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4424  1322   593   258   132    81    35    17     6     8     2     4     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     1     2     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 340.474856 seconds
     */
    // uint64_t h = seed;
    // h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    // h = h * 0xBEA225F9EB34556DUL + len;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    //
    // for (size_t i = 0; i < len; i++) {
    //     h = TABLE[(data[i] + i) & 255] - ROTL64(h, 11);
    // }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    // h *= 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    // return h;

    // Just summing changing TABLE entries isn't enough.
    /*
----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         3068   917   435   221   116    50    34    12    10     4     4     5     6

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     2     1     1     0     1     2     2     0  1998

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: FAIL            ( 77 / 186 passed)
Failures:
    Cyclic              : [4 cycles of 3 bytes, 4 cycles of 4 bytes, 8 cycles of 3 bytes, 8 cycles of 4 bytes, 12 cycles of 3 bytes, 12 cycles of 4 bytes, 16 cycles of 3 bytes, 16 cycles of 4 bytes, 16 cycles of 5 bytes]
    Sparse              : [6/2, 4/3, 4/4, 4/5, 3/6, 3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 10/2, 20/3, 9/4, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]
    Text                : [dictionary, numbers without commas, numbers with commas, FXXXXB, FBXXXX, XXXXFB, FooXXXXBar, FooBarXXXX, XXXXFooBar, FooooXXXXBaaar, FooooBaaarXXXX, XXXXFooooBaaar, FooooooXXXXBaaaaar, FooooooBaaaaarXXXX, XXXXFooooooBaaaaar, FooooooooXXXXBaaaaaaar, FooooooooBaaaaaaarXXXX, XXXXFooooooooBaaaaaaar, FooooooooooXXXXBaaaaaaaaar, FooooooooooBaaaaaaaaarXXXX, XXXXFooooooooooBaaaaaaaaar, Words alnum 1-4, Words alnum 5-8, Words alnum 1-16, Words alnum 1-32, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [8, 1024, 2048, 4096]
    PerlinNoise         : [2]
    Bitflip             : [3, 4]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]
    SeedBlockOffset     : [0, 1, 2, 3, 4, 5]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 278.178300 seconds
    */
    // uint64_t h = seed + 0x2127599BF4325C37UL;
    // h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    // h = h * 0xBEA225F9EB34556DUL + len;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    //
    // for (size_t i = 0; i < len; i++) {
    //     h += TABLE[(data[i] + i) & 255];
    // }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    // h *= 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    // return h;

    // Because i is used in the main loop, we don't need len at the start.
    /*
    ----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4396  1306   603   295   149    67    31    21     9     7     3     0     2

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 350.574168 seconds
    */
    // uint64_t h = seed;
    // h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    // h = h * 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    //
    // for (size_t i = 0; i < len; i++) {
    //     h = TABLE[(data[i] + i) & 255] - ROTL64(h, 11);
    // }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    // h *= 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    // return h;

    // Replacing the table with a multiply doesn't work.
    /*
    ----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4117  1098   551   291   140    75    33    18     8    14     2     8     1

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     1     0     1     2     0     1     0     1     0     0     0   525

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: FAIL            ( 131 / 186 passed)
Failures:
    Sparse              : [3/7, 3/8, 3/9, 3/10, 3/12, 3/14, 5/9, 4/14, 4/16, 3/32, 3/48, 3/64, 3/96, 2/128, 2/256, 2/512, 2/1024, 2/1280]
    Text                : [dictionary, numbers without commas, numbers with commas, Long alnum first 1968-2128, Long alnum last 1968-2128, Long alnum first 4016-4176, Long alnum last 4016-4176, Long alnum first 8112-8272, Long alnum last 8112-8272]
    TwoBytes            : [8, 1024, 2048, 4096]
    SeedBlockLen        : [8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 359.015566 seconds
    */
    // uint64_t h = seed;
    // h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    // h = h * 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    //
    // for (size_t i = 0; i < len; i++) {
    //     h = (data[i] + i) * 0x2127599BF4325C37UL - ROTL64(h, 11);
    // }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    // h *= 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    // return h;

    // Fails different tests... no table here.
    /*
    ----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4355  1326   598   261   114    76    42    18    16    11     0     3     5

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            2     3     4     5     0     1     1     0     0     1     0     2    45

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: FAIL            ( 171 / 186 passed)
Failures:
    Zeroes              : []
    Permutation         : [4-bytes [3 low bits; LE], 4-bytes [3 low bits; BE], 4-bytes [3 high bits; LE], 4-bytes [3 high bits; BE], 4-bytes [3 high+low bits; LE], 4-bytes [3 high+low bits; BE], 4-bytes [0, low bit; LE], 4-bytes [0, low bit; BE], 4-bytes [0, high bit; LE], 4-bytes [0, high bit; BE], 8-bytes [0, low bit; LE], 8-bytes [0, low bit; BE], 8-bytes [0, high bit; LE], 8-bytes [0, high bit; BE]]

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 646.282891 seconds
    */
    // uint64_t h = seed;
    // h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    // h = h * 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    //
    // for (size_t i = 0; i < len; i++) {
    //     h = (data[i] - ROTL64(h, 11)) * (0x2127599BF4325C37UL + i * i + i);
    // }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    // h *= 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    // return h;

    // This one passes without a table! Not very fast though.
    /*
    ----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4397  1247   624   301   145    82    44    20    16     6     7     0     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 644.856766 seconds
    */
    // uint64_t h = seed;
    // h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    // h = h * 0xBEA225F9EB34556DUL + len;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    //
    // for (size_t i = 0; i < len; i++) {
    //     h = (data[i] - ROTL64(h, 11)) * 0x2127599BF4325C37UL;
    // }
    // h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    // h *= 0xBEA225F9EB34556DUL;
    // h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    // return h;

    // Doesn't need the rotation if it multiplies.
    /*
    ----------------------------------------------------------------------------------------------
-log2(p-value) summary:

          0     1     2     3     4     5     6     7     8     9    10    11    12
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
         4416  1263   641   271   142    82    40    17     8     7     1     1     0

         13    14    15    16    17    18    19    20    21    22    23    24    25+
        ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
            0     0     0     0     0     0     0     0     0     0     0     0     0

----------------------------------------------------------------------------------------------
Summary for: TableOAAT
Overall result: pass            ( 186 / 186 passed)

----------------------------------------------------------------------------------------------
Verification value is 0x00000001 - Testing took 537.280629 seconds
     */
    uint64_t h = seed;
    h ^= ROTL64(h, 25) ^ ROTL64(h, 50);
    h = h * 0xBEA225F9EB34556DUL + len;
    h ^= ROTL64(h, 21) ^ ROTL64(h, 44);

    for (size_t i = 0; i < len; i++) {
        h = (data[i] - h) * 0x2127599BF4325C37UL;
    }
    h ^= ROTL64(h, 23) ^ ROTL64(h, 47);
    h *= 0xBEA225F9EB34556DUL;
    h ^= ROTL64(h, 21) ^ ROTL64(h, 44);
    return h;

}

//------------------------------------------------------------
template <bool bswap>
static void TableOAAT( const void * in, const size_t len, const seed_t seed, void * out ) {
    uint64_t h = TableOAAT_impl((const uint8_t *)in, len, (uint64_t)seed);

    PUT_U64<bswap>(h, (uint8_t *)out, 0);
}

//------------------------------------------------------------
REGISTER_FAMILY(TableOAAT,
   $.src_url    = "https://github.com/aappleby/smhasher/blob/master/src/Hashes.cpp",
   $.src_status = HashFamilyInfo::SRC_ACTIVE
 );

REGISTER_HASH(TableOAAT,
   $.desc       = "OAAT hash based on tabulation hashing",
   $.hash_flags = 0,
   $.impl_flags =
         FLAG_IMPL_SANITY_FAILS  |
         FLAG_IMPL_ROTATE      |
         FLAG_IMPL_LICENSE_MIT   |
         FLAG_IMPL_VERY_SLOW,
   $.bits = 64,
   $.verification_LE = 0,
   $.verification_BE = 0,
   $.hashfn_native   = TableOAAT<false>,
   $.hashfn_bswap    = TableOAAT<true>
 );
