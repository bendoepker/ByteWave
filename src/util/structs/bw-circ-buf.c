#include <bw-circ-buf.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

BWError BWCBF_Create(BWCircBufFloat* circ_buf, size_t capacity) {
    memset(circ_buf, 0, sizeof(*circ_buf));
    circ_buf->capacity = capacity;
    circ_buf->array = calloc(capacity, sizeof(float));
    if(circ_buf->array == 0)
        return BW_FAILED_MALLOC;

    return BW_OK;
}

void BWCBF_Destroy(BWCircBufFloat* circ_buf) {
    free(circ_buf->array);
    circ_buf->array = 0;
}

void BWCBF_Append(BWCircBufFloat* circ_buf, float* buf, size_t num) {
    size_t num_after_wp = circ_buf->capacity - circ_buf->write_ptr;
    size_t buf_ptr = 0;
    if(num <= num_after_wp) {
        memcpy(circ_buf->array + circ_buf->write_ptr, buf, num * sizeof(float));
        circ_buf->write_ptr += num;
        return;
    } else {
        memcpy(circ_buf->array + circ_buf->write_ptr, buf, num_after_wp * sizeof(float));
        circ_buf->write_ptr = 0;
        num -= num_after_wp;
        buf_ptr += num_after_wp;
    }
    while(num > circ_buf->capacity) {
        memcpy(circ_buf->array, buf + buf_ptr, circ_buf->capacity * sizeof(float));
        buf_ptr += circ_buf->capacity;
        num -= circ_buf->capacity;
    }
    memcpy(circ_buf->array, buf + buf_ptr, num * sizeof(float));
    circ_buf->write_ptr = num;
}

void BWCBF_GetMostRecent(BWCircBufFloat* circ_buf, float* buf, size_t num) {
    assert(num < circ_buf->capacity);
    assert(num > 0);

    //Retrieve the 'num' elements added to the buffer from least recent to most recent
    if(circ_buf->write_ptr >= num) {
        //All of the elements are before the write pointer (write ptr is the first element after last write)
        memcpy(buf, circ_buf->array + (circ_buf->write_ptr - num), num * sizeof(float));
    } else {
        //Read (num - write ptr) elements from the back of the list, then read (write ptr) elements from the front
        size_t num_wp = num - circ_buf->write_ptr;
        memcpy(buf, circ_buf->array + (circ_buf->capacity - (num_wp)), (num - circ_buf->write_ptr) * sizeof(float));
        memcpy(buf + num_wp, circ_buf->array, circ_buf->write_ptr * sizeof(float));
    }
}
