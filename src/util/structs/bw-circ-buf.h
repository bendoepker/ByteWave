#ifndef BW_CIRC_BUF_H
#define BW_CIRC_BUF_H

#include <bw-types.h>

typedef struct {
    float* array;
    size_t capacity;
    size_t write_ptr;
    size_t read_ptr;
} BWCircBufFloat;

//Allocate the internal buffers
BWError BWCBF_Create(BWCircBufFloat* circ_buf, size_t capacity);

//Deallocate the internal buffers
void BWCBF_Destroy(BWCircBufFloat* circ_buf);

//Append num items from buf to the circular buffer circ_buf
void BWCBF_Append(BWCircBufFloat* circ_buf, float* buf, size_t num);

//Copy the most recently appended
void BWCBF_GetMostRecent(BWCircBufFloat* circ_buf, float* buf, size_t num);


#endif //BW_CIRC_BUF_H
