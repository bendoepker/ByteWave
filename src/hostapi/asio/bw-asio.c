#include "bw-asio.h"
#include "bw-asio-il.h"
#include <stdlib.h>
#include <string.h>

BWError BWAsioInitialize() {
    BWError result = BW_OK;

    bw_asio_construct_drivers();
    return result;
}

BWError BWAsioTerminate() {
    BWError result = BW_OK;

    bw_asio_destroy_drivers();
    return result;
}

//WARN: Not thread safe, uses malloc
BWError BWAsioQueryDevices(asio_device** devices, uint32_t* num_devices){
    BWError result = BW_OK;

    long max_drivers = 32; //Arbitrary number, unlikely to be more than 32 drivers on a pc
    char* driver_names[max_drivers];
    for(int i = 0; i < max_drivers; i++) {
        driver_names[i] = (char*)malloc(32); //Max 32 Bytes per name as per asio standard
        if(driver_names[i] == 0) return BW_FAILED_MALLOC;
    }
    long res = bw_asio_get_drv_names(driver_names, max_drivers);
    if(res < 0){
        if(res == -1010) return BW_UNINITIALIZED;
        return BW_FAILED;
    }
    *num_devices = res;
    if(res == 0) {
        (*devices) = 0;
        return BW_OK;
    }

    //Translate the driver_names array into an array of asio_device
    (*devices) = (asio_device*)malloc(sizeof(asio_device) * res);
    for(int i = 0; i < res; i++) {
        (*devices)[i].device_index = i;
        memcpy((*devices)[i].name, driver_names[i], 32);
    }

    for(int i = 0; i < max_drivers; i++) {
        free(driver_names[i]);
        driver_names[i] = 0;
    }
    return result;
}
