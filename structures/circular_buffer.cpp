#include "circular_buffer.h"

// CIRCULAR BUFFER FLOAT 32

CircularBufferFloat32::CircularBufferFloat32(int num_samples, int num_blocks) {
    m_block_size = num_samples;
    m_num_blocks = num_blocks;
    m_buffer_size = num_blocks * num_samples;
    m_data_buffer = new float[m_buffer_size];
}

void CircularBufferFloat32::write(int num_samples, const float* data_in){
    for(int i = 0; i < num_samples; i++) {
        m_data_buffer[m_write_ptr] = data_in[i];
        m_write_ptr++;
        if(m_write_ptr >= m_buffer_size) m_write_ptr = 0;
    }
}

void CircularBufferFloat32::read(int num_samples, float* data_out){
    for(int i = 0; i < num_samples; i++){
        data_out[i] = m_data_buffer[m_read_ptr];
        m_read_ptr++;
        if(m_read_ptr >= m_buffer_size) m_read_ptr = 0;
    }
}

void CircularBufferFloat32::write_block(const float* data_in){
    for(int i = 0; i < m_block_size; i++) {
        m_data_buffer[m_write_ptr] = data_in[i];
        m_write_ptr++;
        if(m_write_ptr >= m_buffer_size) m_write_ptr = 0;
    }
}

void CircularBufferFloat32::read_block(float* data_out){
    for(int i = 0; i < m_block_size; i++){
        data_out[i] = m_data_buffer[m_read_ptr];
        m_read_ptr++;
        if(m_read_ptr >= m_buffer_size) m_read_ptr = 0;
    }
}

// CIRCULAR BUFFER FLOAT 32

CircularBufferFloat64::CircularBufferFloat64(int num_samples, int num_blocks){
    m_block_size = num_samples;
    m_num_blocks = num_blocks;
    m_buffer_size = num_blocks * num_samples;
    m_data_buffer = new double[m_buffer_size];
}

void CircularBufferFloat64::write(int num_samples, const double* data_in){
    for(int i = 0; i < num_samples; i++) {
        m_data_buffer[m_write_ptr] = data_in[i];
        m_write_ptr++;
        if(m_write_ptr >= m_buffer_size) m_write_ptr = 0;
    }
}

void CircularBufferFloat64::read(int num_samples, double* data_out){
    for(int i = 0; i < num_samples; i++){
        data_out[i] = m_data_buffer[m_read_ptr];
        m_read_ptr++;
        if(m_read_ptr >= m_buffer_size) m_read_ptr = 0;
    }
}

void CircularBufferFloat64::write_block(const double* data_in){
    for(int i = 0; i < m_block_size; i++) {
        m_data_buffer[m_write_ptr] = data_in[i];
        m_write_ptr++;
        if(m_write_ptr >= m_buffer_size) m_write_ptr = 0;
    }
}

void CircularBufferFloat64::read_block(double* data_out){
    for(int i = 0; i < m_block_size; i++){
        data_out[i] = m_data_buffer[m_read_ptr];
        m_read_ptr++;
        if(m_read_ptr >= m_buffer_size) m_read_ptr = 0;
    }
}

