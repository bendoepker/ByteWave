#include "wav.h"
#include <stdint.h>

typedef struct wav_file_parameters {
	unsigned char num_channels;
	unsigned char PCM;
	uint32_t sample_rate;
	uint32_t byte_rate;
	unsigned char block_align;
};