#include "bw_wasapi.h"

#include <mmdeviceapi.h>
#include <windows.h>

static int REF_TIMES_PER_SEC = 10000000;
static int REF_TIMES_PER_MSEC = 10000;

BWError BWWASAPIInitialize() {
    BWError result = BW_OK;
    HRESULT hres;


    return result;
}

BWError BWWASAPITerminate() {
    BWError result = BW_OK;
    //TODO: Implement SafeRelease of all resouces

    return result;
}

BWError BWWASAPIQueryDevices() {
    BWError result = BW_OK;
    //TODO: Move code from main function to here, add safe releases

    return result;
}
