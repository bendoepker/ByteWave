#ifndef BW_ASIO_IL_H
#define BW_ASIO_IL_H
#include <asio.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <bw-types.h>

//Asio driver functions
void _bw_asio_construct_drivers();
void _bw_asio_destroy_drivers();
long _bw_asio_get_drv_names(char** names, long max_drivers);
BWError _bw_asio_get_current_drv_name(char* name);
long _bw_asio_get_current_driver_index();
BWError _bw_asio_load_driver(char* name);
void _bw_asio_remove_current_driver();

//Asio stream functions
BWError _bw_asio_init(ASIODriverInfo* info);
BWError _bw_asio_exit(void);
BWError _bw_asio_start(void);
BWError _bw_asio_stop(void);
BWError _bw_asio_get_channels(long* input_latency, long* output_latency);


#ifdef __cplusplus
}
#endif
#endif //BW_ASIO_IL_H
