# Good hashes using QOA

This just records results from hash-prospector that seem notable.
It isn't directly related to SMHasher, but could be useful for finalizers.

The timing data is likely quite wrong due to a simultaneous MT test running.

```
./prospector -q 28 -s -p xorr:16,qoa,xorr:15,qoa,xorr:15
// OR
./prospector -q 25 -s -p xorr:16,qoa,xorr:17,qoa,xorr:13

./prospector -Eep xorr:16,qoa:2f75,xorr:18,qoa:39f,xorr:13
bias      = 0.033100874701374738
speed     = 0.142 nsec / hash

./prospector -Eep xorr:16,qoa:97,xorr:18,qoa:2bd,xorr:13
bias      = 0.035942157628005095
speed     = 0.143 nsec / hash

./prospector -Eep xorr:16,qoa:49e3,xorr:17,qoa:179,xorr:13
bias      = 0.036397266308061241
speed     = 0.142 nsec / hash

/prospector -Eep xorr:16,qoa:168d,xorr:17,qoa:18d1,xorr:13
bias      = 0.038949476399362623
speed     = 0.141 nsec / hash

./prospector -Eep xorr:16,qoa:8697,xorr:15,qoa:409f,xorr:15
bias      = 0.039329847364410392
speed     = 0.144 nsec / hash

./prospector -Eep xorr:16,qoa:6295,xorr:15,qoa:a91,xorr:15
bias      = 0.039683978647342522
speed     = 0.142 nsec / hash

./prospector -Eep xorr:16,qoa:909b,xorr:17,qoa:2237,xorr:13
bias      = 0.039871968806044815
speed     = 0.127 nsec / hash

./prospector -Eep xorr:16,qoa:7b19,xorr:17,qoa:a9f,xorr:13
bias      = 0.039965905878694599
speed     = 0.129 nsec / hash

./prospector -Eep xorr:16,qoa:99c9,xorr:17,qoa:34e3,xorr:13
bias      = 0.040406847871365829
speed     = 0.144 nsec / hash

./prospector -Eep xorr:16,qoa:5269,xorr:15,qoa:268d,xorr:15
bias      = 0.041633076276120451
speed     = 0.141 nsec / hash

./prospector -Eep xorr:16,qoa:cb83,xorr:15,qoa:0961,xorr:15
bias      = 0.042702648198309656
speed     = 0.143 nsec / hash

./prospector -Eep xorr:16,qoa:a4a7,xorr:15,qoa:621d,xorr:15
bias      = 0.043294553925893466
speed     = 0.138 nsec / hash

./prospector -Eep xorr:16,qoa:52a9,xorr:15,qoa:572b,xorr:15
bias      = 0.044935739248354971
speed     = 0.138 nsec / hash

./prospector -Eep xorr:16,qoa:22eb,xorr:15,qoa:a029,xorr:15
bias      = 0.049379680461121019
speed     = 0.128 nsec / hash

./prospector -Eep xorr:16,qoa:795,xorr:15,qoa:8ac7,xorr:15
bias      = 0.049852164987430482
speed     = 0.128 nsec / hash

./prospector -Eep xorr:16,qoa:6391,xorr:15,qoa:3c87,xorr:15
bias      = 0.051092778337090708
speed     = 0.126 nsec / hash

```

