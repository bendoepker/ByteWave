#include <iostream>
#include "circular_buffer.h"

int main() {

    std::cout << "Testing CBF32" << std::endl;
    CircularBufferFloat32* test32 = new CircularBufferFloat32(10, 3);
    std::cout << "Write Block & Read Block" << std::endl;
    float data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    test32->write_block(data);
    float* data2 = new float[10];
    test32->read_block(data2);
    for(int i = 0; i < 10; i++){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Write & Read" << std::endl;
    float data5[3] = {10, 11, 12};
    test32->write(3, data5);
    float* data6 = new float[10];
    test32->read(10, data6);
    for(int i = 0; i < 10; i++) {
        std::cout << data6[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;


    std::cout << "Testing CBF64" << std::endl;
    CircularBufferFloat64* test64 = new CircularBufferFloat64(10, 3);
    std::cout << "Write Block & Read Block" << std::endl;
    double data3[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    test64->write_block(data3);
    double* data4 = new double[10];
    test64->read_block(data4);
    for(int i = 0; i < 10; i++){
        std::cout << data4[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Write & Read" << std::endl;
    double data7[3] = {10, 11, 12};
    test64->write(3, data7);
    double* data8= new double[10];
    test64->read(10, data8);
    for(int i = 0; i < 10; i++) {
        std::cout << data8[i] << " ";
    }
    std::cout << std::endl;
}
