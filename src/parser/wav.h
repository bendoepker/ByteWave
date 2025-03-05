#include <stdint.h>

typedef struct {
	unsigned char num_channels;
	unsigned char PCM;
	uint32_t sample_rate;
	uint32_t byte_rate;
	unsigned char block_align;
} wav_file_parameters;
