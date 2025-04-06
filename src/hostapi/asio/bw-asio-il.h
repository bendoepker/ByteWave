#ifndef BW_ASIO_IL_H
#define BW_ASIO_IL_H
#ifdef __cplusplus
extern "C" {
#endif

#include <bw-types.h>

//Function decls
void bw_asio_construct_drivers();
void bw_asio_destroy_drivers();

//Finds the drivers on the system
long bw_asio_get_drv_names(char** names, long max_drivers);

//
BWError bw_asio_get_current_drv_name(char* name);
long bw_asio_get_current_driver_index();

//Load and unload the driver from memory
BWError bw_asio_load_driver(char* name);
void bw_asio_remove_current_driver();

#ifdef __cplusplus
}
#endif
#endif //BW_ASIO_IL_H
