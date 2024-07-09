# ssasa2024

Secure System Assignment Supplemental Assignment

## Rolling Notes (for documenting while learning)

- The algorithm involves a permutation-based design using the sponge construction, which consists of absorbing and squeezing phases.

### Data Structure

Tracks the state using a `5x5` array of 64-bit lanes (=1600 bits in total).

### Padding

Padding Rule (Multi-rate Padding)

The padding rule for Keccak is designed to ensure that the padded message is a multiple of the bitrate. The specific padding used in Keccak is called "multi-rate padding" or "pad10*1". Here’s a step-by-step description of the padding process:

    Append a single bit 1 to the end of the message.
    Append zero or more bits 0 such that the total length of the message is congruent to −1 modulo the bitrate.
    Append a single bit 1 to the end of the message.

Example

Given a bitrate r = 1088, the message is padded so that its length is a multiple of r:

    For instance, if the message length is L bits:
        Append 1 bit: L + 1
        Append k zero bits such that (L + 1 + k) % r = r - 1
        Append 1 bit: L + 1 + k + 1 = multiple of r

### Permutation Function

function `f` implements the 5 steps: $\theta$, $\rho$, $\pi$, $\chi$ and $\iota$.

### Sponge Construction

- absorbing phase
- squeezing phase

### String everything together

The main programme should read the input, applying padding, do the Keccak permutation to produce the final hash.

### Tests

Citing an existing implementation on [sha3-256](https://github.com/euugenechou/sha3-256) for reference, for test comparisons
Note that the reference implementation contains full NIST test vectors, it should suffice to test against these vectors to assess the correctness of my implementation.

### Skeleton Code in progress (Development Note)

- `sha3.h` <- constants, data structure
- `sha3.c` <- functions
- `test.c` <- IO, argv etc. to enable some basic printing the output capability
- `Makefile` <- build both mine and other's implementation, perform tests and compare the result