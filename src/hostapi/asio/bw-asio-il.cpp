#include "bw-asio-il.h"
#include <asiodrivers.h>
#include <bw-log.h>

//Globals
AsioDrivers* _bw_asio_drivers = 0;

//Wrap constructor / destructor
void _bw_asio_construct_drivers() { _bw_asio_drivers = new AsioDrivers(); }

void _bw_asio_destroy_drivers() { 
    delete _bw_asio_drivers; 
    _bw_asio_drivers = 0;
}

//Get Driver Names
//Returns the number of drivers (up to the total number of drivers)
//or -1010 if the class is not instantiated
long _bw_asio_get_drv_names(char** names, long max_drivers) {
    BW_LOG_ERROR("TEST");
    if(!_bw_asio_drivers) return -1010;
    return _bw_asio_drivers->getDriverNames(names, max_drivers);
}

//Retrieves the name of the current loaded driver
BWError _bw_asio_get_current_drv_name(char* name) {
    if(!_bw_asio_drivers) return BW_UNINITIALIZED;
    if(_bw_asio_drivers->getCurrentDriverName(name))
        return BW_OK;
    else return BW_FAILED;
}

//Retrieves the index of the current loaded driver
long _bw_asio_get_current_driver_index() {
    if(!_bw_asio_drivers) return BW_UNINITIALIZED;
    return _bw_asio_drivers->getCurrentDriverIndex();
}

BWError _bw_asio_load_driver(char* name) {
    if(!_bw_asio_drivers) return BW_UNINITIALIZED;
    if(_bw_asio_drivers->loadDriver(name))
        return BW_OK;
    else return BW_FAILED;
}

void _bw_asio_remove_current_driver() {
    if(!_bw_asio_drivers) return;
    _bw_asio_drivers->removeCurrentDriver();
    return;
}
