#include "bw-jack.h"
#include <jack/jack.h>
#include <bw-log.h>

BWError BWJack_Activate(BWHostApi_AudioDevice* audio_device) {
}

BWError BWJack_Deactivate() {
    return BW_FAILED;
}

BWError BWJack_QueryDevices(_jack_device** devices, uint32_t* num_devices) {
    return 0;
}
