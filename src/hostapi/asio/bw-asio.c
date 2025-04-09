#include "bw-asio.h"
#include "bw-asio-il.h"
#include <bw-log.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

ASIODriverInfo* _bw_asio_driver_info = 0;
_asio_device* _bw_asio_devices = 0;
uint32_t _bw_asio_num_devices = 0;

//WARN: Not thread safe, uses malloc (and file io soon)
//TODO: Use saved default device (saved_device)
BWError BWAsio_Initialize() {
    BWError result = BW_OK;

    //Cpp class constructor
    _bw_asio_construct_drivers();

    result = BWAsio_QueryDevices(&_bw_asio_devices, & _bw_asio_num_devices);
    if(_bw_asio_num_devices == 0) return BW_FAILED; //No asio devices available

    long saved_device = 3;
    result = _bw_asio_load_driver(_bw_asio_devices[saved_device].name);
    if(result != BW_OK) return result;

    //Driver info struct initialization
    _bw_asio_driver_info = (ASIODriverInfo*)malloc(sizeof(ASIODriverInfo));
    if(_bw_asio_driver_info == 0) return BW_FAILED_MALLOC;
    memset(_bw_asio_driver_info, 0, sizeof(ASIODriverInfo));
    _bw_asio_driver_info->sysRef = GetCurrentProcess();
    _bw_asio_driver_info->asioVersion = 2;

    result = _bw_asio_init(_bw_asio_driver_info);
    if(result != BW_OK) return result;

    return result;
}

BWError BWAsio_Terminate() {
    BWError result = BW_OK;
    BW_PRINT("BW_Terminate");
    result = _bw_asio_exit(); //This will remove the current driver as well
    _bw_asio_destroy_drivers();
    return result;
}

//WARN: Close stream may need to be called before this to dispose old buffers
//TODO: Test above ^
BWError BWAsio_ChangeDevice(char device[32]) {
    //Reset driver info

    if(_bw_asio_driver_info != 0)
        memset(_bw_asio_driver_info, 0, sizeof(ASIODriverInfo));
    else {
        _bw_asio_driver_info = (ASIODriverInfo*)malloc(sizeof(ASIODriverInfo));
        if(_bw_asio_driver_info == 0) return BW_FAILED_MALLOC;
    }

    _bw_asio_driver_info->sysRef = GetCurrentProcess();
    _bw_asio_driver_info->asioVersion = 2;

    //NOTE: ASIO takes care of removing / unloading the old driver

    //Link new device driver
    BWError res = _bw_asio_load_driver(device);
    if(res) {
        BW_LOG_ERR("%d", res);
        return res;
    }
    res = _bw_asio_init(_bw_asio_driver_info);
    if(res) {
        BW_LOG_ERR("%d", res);
        return res;
    }

    return BW_OK;
}

//WARN: Not thread safe, uses malloc
BWError BWAsio_QueryDevices(_asio_device** devices, uint32_t* num_devices){
    BWError result = BW_OK;

    long max_drivers = 32; //Arbitrary number, unlikely to be more than 32 drivers on a pc
    char* driver_names[max_drivers];
    for(int i = 0; i < max_drivers; i++) {
        driver_names[i] = (char*)malloc(32); //Max 32 Bytes per name as per asio standard
        if(driver_names[i] == 0) return BW_FAILED_MALLOC;
    }
    long res = _bw_asio_get_drv_names(driver_names, max_drivers);
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
    (*devices) = (_asio_device*)malloc(sizeof(_asio_device) * res);
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
