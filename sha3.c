#include "sha3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const uint64_t keccak_round_constants[KECCAK_ROUND_COUNT] = {
    // Initialize with appropriate constants
    // These constants are essential in the Iota step to introduce asymmetry to the permutation,
    // ensuring each round is different and thus contributing to the security of the algorithm.

    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
    0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
    0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
    0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
    0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL};

void keccak_initialize(keccak_context_t* context) {
  memset(context, 0, sizeof(keccak_context_t));
}

void keccak_absorb(keccak_context_t* context, const uint8_t* data,
                   size_t length) {
  // Implement the absorb phase
}

void keccak_squeeze(keccak_context_t* context, uint8_t* hash,
                    size_t hashLength) {
  // Implement the squeeze phase
}

void keccak_permutation(uint64_t* state) {
  // Implement the permutation function f with the 5 steps (theta, rho, pi, chi, iota)
  for (int round = 0; round < KECCAK_ROUND_COUNT; round++) {
    keccak_theta(state);
    keccak_rho(state);
    keccak_pi(state);
    keccak_chi(state);
    keccak_iota(state, round);
  }
}

void keccak_theta(uint64_t* state) {
  uint64_t C[5], D[5];
  for (int x = 0; x < 5; x++) {
    C[x] =
        state[x] ^ state[x + 5] ^ state[x + 10] ^ state[x + 15] ^ state[x + 20];
  }
  for (int x = 0; x < 5; x++) {
    D[x] = C[(x + 4) % 5] ^ ROTL64(C[(x + 1) % 5], 1);
  }
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 5; y++) {
      state[x + 5 * y] ^= D[x];
    }
  }
}

void keccak_rho(uint64_t* state) {
  static const int rho_offsets[25] = {0,  1, 62, 28, 27, 36, 44, 6,  55,
                                      20, 3, 10, 43, 25, 39, 41, 45, 15,
                                      21, 8, 18, 2,  61, 56, 14};
  for (int i = 0; i < 25; i++) {
    state[i] = ROTL64(state[i], rho_offsets[i]);
  }
}

void keccak_pi(uint64_t* state) {
  uint64_t temp[25];
  memcpy(temp, state, sizeof(temp));
  for (int x = 0; x < 5; x++) {
    for (int y = 0; y < 5; y++) {
      state[y + 5 * ((2 * x + 3 * y) % 5)] = temp[x + 5 * y];
    }
  }
}

void keccak_chi(uint64_t* state) {
  uint64_t temp[5];
  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 5; x++) {
      temp[x] = state[x + 5 * y];
    }
    for (int x = 0; x < 5; x++) {
      state[x + 5 * y] ^= (~temp[(x + 1) % 5]) & temp[(x + 2) % 5];
    }
  }
}

void keccak_iota(uint64_t* state, int round) {
  state[0] ^= keccak_round_constants[round];
}

uint64_t ROTL64(uint64_t x, uint64_t y) {
  return (x << y) | (x >> (64 - y));
}

void keccak_padding(keccak_context_t* context) {
  // Implement the padding function
  // Calculate the position where the padding should start
  size_t padding_start = context->dataQueueSize;

  // Add the first padding bit with 1
  context->dataQueue[padding_start++] = 0x01;

  // Fill the rest with zeroes
  while (padding_start < (KECCAK_BITRATE / 8)) {
    context->dataQueue[padding_start++] = 0x00;
  }

  // Replace the last bit with 1
  context->dataQueue[(KECCAK_BITRATE / 8) - 1] |= 0x80;

  // Update the size of the data queue to be exactly the bitrate
  context->dataQueueSize = KECCAK_BITRATE / 8;
}

unsigned char* keccak_hash(unsigned char* data, size_t length) {
  keccak_context_t context;
  keccak_initialize(&context);

  keccak_absorb(&context, data, length);
  keccak_padding(&context);

  unsigned char* hash = (unsigned char*)malloc(SHA3_256_DIGEST_SIZE);
  if (hash == NULL) {
    return NULL;  // Handle memory allocation failure
  }

  keccak_squeeze(&context, hash, SHA3_256_DIGEST_SIZE);
  return hash;
}
