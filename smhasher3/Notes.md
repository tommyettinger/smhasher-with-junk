# Good hashes using QOA

This just records results from hash-prospector that seem notable.
It isn't directly related to SMHasher, but could be useful for finalizers.

The timing data is likely quite wrong due to a simultaneous MT test running.

```
./prospector -q 28 -s -p xorr:16,qoa,xorr:15,qoa,xorr:15

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

