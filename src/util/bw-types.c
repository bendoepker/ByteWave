#include <bw-types.h>
#include <bw-log.h>

const char* _bw_get_error_message(BWError err) {
    switch(err) {
        case BW_OK:
            return "No Error";
        case BW_FAILED:
            return "Failed, unspecified";
        case BW_UNINITIALIZED:
            return "Host API / Device was not initialized";
        case BW_WINDOWS_ERROR:
            return "Error with Windows";
        case BW_FAILED_MALLOC:
            return "Failed to execute malloc(), system out of memory";
        case BW_BAD_PARAMS:
            return "Incorrect parameters were passed to the function";
        case BW_HARDWARE_MALFUNCTION:
            return "External hardware device malfunction";
        case BW_FILE_WRITE_ERROR:
            return "Error writing to a file";
        case BW_FILE_READ_ERROR:
            return "Error reading a file";
        case BW_FILE_OPEN_ERROR:
            return "Error opening a file";
        case BW_VERSION_INCOMPATIBLE:
            return "Incompatible version found";
        default:
            return "Error message unspecified";
    }
}

void BWError_Handle(BWError err) {
    if(err != BW_OK) {
        BW_LOG_ERR("%s", _bw_get_error_message(err));
    }
}
