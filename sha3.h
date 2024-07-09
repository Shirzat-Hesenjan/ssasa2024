#include <stdlib.h>
#include <stdint.h>

#define SHA3_256_DIGEST_SIZE 32
#define KECCAK_STATE_SIZE 25
#define KECCAK_BITRATE 1088
#define KECCAK_CAPACITY (1600 - KECCAK_BITRATE)
#define KECCAK_ROUND_COUNT 24

typedef struct {
  uint64_t state[KECCAK_STATE_SIZE];
  uint8_t dataQueue[KECCAK_BITRATE / 8];
  size_t dataQueueSize;
} keccak_context_t;

unsigned char* keccak_hash(unsigned char* data, size_t length);
