# ssasa2024

Secure System Assignment Supplemental Assignment

## Rolling Notes (for documenting while learning)

- The algorithm involves a permutation-based design using the sponge construction, which consists of absorbing and squeezing phases.

### (core?) Data Structure

Tracks the state using a `5x5` array of 64-bit lanes (=1600 bits in total).

### Padding

TODO

### Permutation Function

function `f` implements the 5 steps: $\theta$, $\rho$, $\pi$, $\chi$ and $\iota$.

### Sponge Construction

- absorbing phase
- squeezing phase

### String everything together

The main programme should read the input, applying padding, do the Keccak permutation to produce the final hash.

### Tests

Citing an existing implementation on [sha3-256](https://github.com/euugenechou/sha3-256) for reference, for test comparisons

### Skeleton Code in progress (Development Note)

- `sha3.h` <- constants, data structure?
- `sha3.c` <- functions
- `test.c` <- IO, argv etc. to enable some basic printing the output capability
- `Makefile` <- build both mine and other's implementation, perform tests and compare the result