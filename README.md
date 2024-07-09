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

Theta:

    This step involves XORing each bit of the state with a parity bit calculated from the state itself.

Rho:

    This step rotates the bits within the state by a specific offset.

Pi:

    This step permutes the bits within the state, effectively rearranging them.

Chi:

    This step performs a non-linear transformation on the state.

Iota:

    This step introduces a round-dependent constant into the state to ensure asymmetry.

### Sponge Construction

- absorbing phase
- squeezing phase

#### Absorbing phase

Determine the Rate in Bytes:

    The bitrate r is typically defined in bits. Since we need to process data in bytes, we convert it to bytes by dividing by 8.
    For KECCAK_BITRATE = 1088, the rate in bytes is 1088 / 8 = 136 bytes.

Process Full Blocks of Data:

    The function processes the input data in chunks of rate_bytes (136 bytes).
    For each full block of rate_bytes, XOR the input data with the corresponding part of the state.
    After XOR-ing, apply the Keccak permutation to mix the state.

Handle the Remaining Data:

    If the input data length is not a multiple of rate_bytes, there will be leftover data less than rate_bytes.
    Copy the remaining data into the dataQueue to be handled later during padding.

#### Squeeze phase

Initialize Variables:

    rate_bytes is calculated as KECCAK_BITRATE / 8.
    outputLength keeps track of how many bytes have been copied to the hash buffer.

Squeeze Process:

    The function continues to squeeze data from the state until the required hashLength is reached.
    If dataQueueSize equals rate_bytes, the state needs to be permuted to produce more output.
    Copy the minimum of the remaining bytes needed and rate_bytes from the state to the hash buffer.
    Update outputLength and dataQueueSize accordingly.


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
- `run_test.py` <- automate few extra tests (random generated, NIST test vectors)

It works! see [this build](https://github.com/Shirzat-Hesenjan/ssasa2024/actions/runs/9865522573/job/27242615718)