#include "sha3.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const uint64_t keccak_round_constants[KECCAK_ROUND_COUNT] = {
    // Initialize with appropriate constants
    // These constants are essential in the Iota step to introduce asymmetry to
    // the permutation,
    // ensuring each round is different and thus contributing to the security of
    // the algorithm.
    0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
    0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
    0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
    0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
    0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
    0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
    0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
    0x8000000000008080, 0x0000000080000001, 0x8000000080008008};

void keccak_initialize(keccak_context_t* context) {
  memset(context, 0, sizeof(keccak_context_t));
}

uint64_t ROTL64(uint64_t x, uint64_t y) {
  return (x << y) | (x >> (64 - y));
}

void keccak_theta(uint64_t* state) {
  uint64_t C[5], D[5];
  int x, y;

  for (x = 0; x < 5; x++) {
    C[x] =
        state[x] ^ state[x + 5] ^ state[x + 10] ^ state[x + 15] ^ state[x + 20];
  }
  for (x = 0; x < 5; x++) {
    D[x] = C[(x + 4) % 5] ^ ROTL64(C[(x + 1) % 5], 1);
  }
  for (x = 0; x < 5; x++) {
    for (y = 0; y < 5; y++) {
      state[x + 5 * y] ^= D[x];
    }
  }
}

void keccak_rho(uint64_t* state) {
  static const int rho_offsets[25] = {0,  1, 62, 28, 27, 36, 44, 6,  55,
                                      20, 3, 10, 43, 25, 39, 41, 45, 15,
                                      21, 8, 18, 2,  61, 56, 14};
  int i;
  for (i = 0; i < 25; i++) {
    state[i] = ROTL64(state[i], rho_offsets[i]);
  }
}

void keccak_pi(uint64_t* state) {
  uint64_t temp[25];
  int x, y;
  memcpy(temp, state, sizeof(temp));
  for (x = 0; x < 5; x++) {
    for (y = 0; y < 5; y++) {
      state[y + 5 * ((2 * x + 3 * y) % 5)] = temp[x + 5 * y];
    }
  }
}

void keccak_chi(uint64_t* state) {
  uint64_t temp[5];
  int y, x;
  for (y = 0; y < 5; y++) {
    for (x = 0; x < 5; x++) {
      temp[x] = state[x + 5 * y];
    }
    for (x = 0; x < 5; x++) {
      state[x + 5 * y] ^= (~temp[(x + 1) % 5]) & temp[(x + 2) % 5];
    }
  }
}

void keccak_iota(uint64_t* state, int round) {
  state[0] ^= keccak_round_constants[round];
}

void keccak_permutation(uint64_t* state) {
  // Implement the permutation function f with the 5 steps (theta, rho, pi, chi,
  // iota)
  for (int round = 0; round < KECCAK_ROUND_COUNT; round++) {
    keccak_theta(state);
    keccak_rho(state);
    keccak_pi(state);
    keccak_chi(state);
    keccak_iota(state, round);
  }
}

// Keccak padding
void keccak_padding(keccak_context_t* context) {
  size_t rate_bytes = KECCAK_BITRATE / 8;
  context->dataQueue[context->dataQueueSize] =
      0x06;  // SHA3-256 (not Keccak256)
  memset(context->dataQueue + context->dataQueueSize + 1, 0,
         rate_bytes - context->dataQueueSize - 2);
  context->dataQueue[rate_bytes - 1] = 0x80;
  for (size_t i = 0; i < rate_bytes / 8; i++) {
    context->state[i] ^= ((uint64_t*)context->dataQueue)[i];
  }
  keccak_permutation(context->state);
  context->dataQueueSize = 0;
}

// Keccak absorb
void keccak_absorb(keccak_context_t* context, const uint8_t* data,
                   size_t length) {
  size_t rate_bytes = KECCAK_BITRATE / 8;
  size_t offset = 0;

  while (length > 0) {
    size_t blockSize = rate_bytes - context->dataQueueSize;
    if (blockSize > length) {
      blockSize = length;
    }

    memcpy(context->dataQueue + context->dataQueueSize, data + offset,
           blockSize);
    context->dataQueueSize += blockSize;
    offset += blockSize;
    length -= blockSize;

    if (context->dataQueueSize == rate_bytes) {
      for (size_t i = 0; i < rate_bytes / 8; i++) {
        context->state[i] ^= ((uint64_t*)context->dataQueue)[i];
      }
      keccak_permutation(context->state);
      context->dataQueueSize = 0;
    }
  }
}

// Keccak squeeze
void keccak_squeeze(keccak_context_t* context, uint8_t* hash,
                    size_t hashLength) {
  size_t rate_bytes = KECCAK_BITRATE / 8;
  size_t outputLength = 0;

  while (outputLength < hashLength) {
    if (context->dataQueueSize == rate_bytes) {
      keccak_permutation(context->state);
      context->dataQueueSize = 0;
    }

    size_t remainingBytes = hashLength - outputLength;
    size_t bytesToOutput =
        (remainingBytes < (rate_bytes - context->dataQueueSize))
            ? remainingBytes
            : (rate_bytes - context->dataQueueSize);

    memcpy(hash + outputLength, context->state + context->dataQueueSize,
           bytesToOutput);
    context->dataQueueSize += bytesToOutput;
    outputLength += bytesToOutput;
  }
}

// Keccak hash function
unsigned char* keccak_hash(unsigned char* data, size_t length) {
  keccak_context_t context;
  uint8_t* hash =
      (uint8_t*)malloc(32);  // SHA-3-256 produces a 256-bit (32-byte) hash
  memset(hash, 0, 32);       // if not zeroed, the result might contain garbage
  memset(&context, 0, sizeof(keccak_context_t));
  keccak_absorb(&context, data, length);
  keccak_padding(&context);
  keccak_squeeze(&context, hash, 32);

  return hash;
}
