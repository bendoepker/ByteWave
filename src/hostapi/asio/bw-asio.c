#include "bw-asio.h"
#include "bw-asio-il.h"
#include <bw-log.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <assert.h>
#include <math.h>

//Globals
ASIOCallbacks _asio_callbacks = {0};
ASIODriverInfo* _bw_asio_driver_info = 0;
uint32_t _bw_asio_num_devices = 0;
unsigned char _bw_asio_is_initialized = 0;

_asio_device _active_asio_device;

//Inner ASIO Functions
void bufferSwitch(long doubleBufferIndex, ASIOBool directProcess);
void sampleRateDidChange(ASIOSampleRate sRate);
long asioMessage(long selector, long value, void* message, double* opt);
ASIOTime* bufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess);

BWError BWAsio_Terminate() {
}

BWError BWAsio_Activate() {
    BWError result = BW_OK;

    //Cpp class constructor
    _bw_asio_construct_drivers();

    //SECTION: Load driver
    result = _bw_asio_load_driver(_active_asio_device.name);
    if(result != BW_OK) return result;

    //SECTION: Driver info struct initialization
    _bw_asio_driver_info = malloc(sizeof *_bw_asio_driver_info);
    if(_bw_asio_driver_info == 0) return BW_FAILED_MALLOC;
    memset(_bw_asio_driver_info, 0, sizeof(ASIODriverInfo));
    _bw_asio_driver_info->sysRef = GetCurrentProcess();
    _bw_asio_driver_info->asioVersion = 2;

    result = _bw_asio_init(_bw_asio_driver_info);
    if(result != BW_OK) return result;

    _bw_asio_is_initialized = 1;

    //SECTION: 

    return result;
}

BWError BWAsio_Deactivate() {
    BWError result = _bw_asio_exit();
    if(result != BW_OK) return result;
    _bw_asio_remove_current_driver();
    _bw_asio_destroy_drivers();
    _bw_asio_is_initialized = 0;
    return BW_OK;

}

//WARN: Not thread safe, uses malloc
BWError BWAsio_QueryDevices(_asio_device** devices, uint32_t* num_devices){

    _bw_asio_construct_drivers();

    long max_drivers = 32; //Arbitrary number, unlikely to be more than 32 drivers on a pc
    char* driver_names[max_drivers];
    for(int i = 0; i < max_drivers; i++) {
        driver_names[i] = malloc(32); //Max 32 Bytes per name as per asio standard
        if(driver_names[i] == 0) return BW_FAILED_MALLOC;
    }
    long num_drivers = _bw_asio_get_drv_names(driver_names, max_drivers);
    if(num_drivers < 0){
        if(num_drivers == -1010) return BW_UNINITIALIZED;
        return BW_FAILED;
    }

    //Translate the driver_names array into an array of asio_device
    *num_devices = num_drivers;
    (*devices) = malloc(sizeof *(*devices) * num_drivers);
    if((*devices) == 0) return BW_FAILED_MALLOC;
    for(int i = 0; i < num_drivers; i++) {
        memcpy((*devices)[i].name, driver_names[i], 32);
    }

    //Cleanup temp memory
    for(int i = 0; i < max_drivers; i++) {
        free(driver_names[i]);
        driver_names[i] = 0;
    }

    _bw_asio_destroy_drivers();

    return BW_OK;
}

void bufferSwitch(long doubleBufferIndex, ASIOBool directProcess) {
    //Backdoor into the actual audio callback (bufferSwitchTimeInfo)

    //Taken from the ASIO2 SDK, explanation in ASIOSDK/host/sample/hostsample.cpp
    ASIOTime timeInfo = {0};

	if(_bw_asio_get_sample_position(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == BW_OK)
		timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

    bufferSwitchTimeInfo(&timeInfo, doubleBufferIndex, directProcess);
}

void sampleRateDidChange(ASIOSampleRate sRate) {
    //TODO: This, likely just update a conversion algorithm and always use a single internal sample rate
    //      (likely 48000)
}

long asioMessage(long selector, long value, void* message, double* opt) {

    //Unused as of ASIO 2.3
    (void)message;
    (void)opt;

    switch(selector) {
        case kAsioSelectorSupported:
			if(value == kAsioResetRequest
			|| value == kAsioEngineVersion
			|| value == kAsioResyncRequest
			|| value == kAsioLatenciesChanged
			// the following three were added for ASIO 2.0, you don't necessarily have to support them
			|| value == kAsioSupportsTimeInfo
			|| value == kAsioSupportsTimeCode
			|| value == kAsioSupportsInputMonitor)
				return 1L;
        case kAsioResetRequest:
            //TODO: Handle reset request, currently we will just crash
            assert(1 && "ASIO Reset Request was sent");
            break;
        case kAsioResyncRequest:
            return 1L;
        case kAsioLatenciesChanged:
            //NOTE: Ignored for the time being, should trigger a latency tracking update if
            //      latency is being tracked
            return 1L;
        case kAsioEngineVersion:
            return 2L;
        case kAsioSupportsTimeInfo:
            return 1L;
        case kAsioSupportsTimeCode:
            return 0; //NOTE: May need to keep track of this info, needs more research
        case kAsioSupportsInputMonitor:
            return 0;
    }
    return 0;
}

ASIOTime* bufferSwitchTimeInfo(ASIOTime* params, long doubleBufferIndex, ASIOBool directProcess) {
    
}
