#ifndef BW_PROCESS_H
#define BW_PROCESS_H

/*
*
*   ByteWave Processing function
*
*   These are the functions responsible for processing the audio,
*   routing the audio through the mixer tracks, and reporting the outputs
*   back to the Host APIs as well as any UI functions that will require it
*
*
*/

#include <stdint.h>
#include "mixer.h"

//The main processing function, every host api will call this function in their own thread
//and the audio will be routed through the mixer structures
void process_buffers(float* input_buffers, float* output_buffers,
                     uint32_t num_input_buffers, uint32_t num_output_buffers,
                     uint32_t buffer_size);

#endif //BW_PROCESS_H
