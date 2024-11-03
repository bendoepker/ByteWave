#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

class CircularBufferFloat32 {
public:
    CircularBufferFloat32(){
        m_num_blocks = 2;
        m_block_size = 512;
    }
    CircularBufferFloat32(int num_samples, int num_blocks = 2);

    void write(int num_samples, const float* data_in);
    void read(int num_samples, float* data_out);
    void write_block(const float* data_in);
    void read_block(float* data_out);
private:
    float* m_data_buffer;
    int m_buffer_size;
    int m_block_size;
    int m_num_blocks;
    int m_write_ptr = 0;
    int m_read_ptr = 0;
};

class CircularBufferFloat64 {
public:
    CircularBufferFloat64(){
        m_num_blocks = 2;
        m_block_size = 512;
        m_data_buffer = new double[m_num_blocks * m_block_size];
    }
    CircularBufferFloat64(int num_samples, int num_blocks = 2);

    void write(int num_samples, const double* data_in);
    void read(int num_samples, double* data_out);
    void write_block(const double* data_in);
    void read_block(double* data_in);
private:
    double* m_data_buffer;
    int m_buffer_size;
    int m_block_size;
    int m_num_blocks;
    int m_write_ptr = 0;
    int m_read_ptr = 0;
};

#endif
