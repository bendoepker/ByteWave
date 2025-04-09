#ifndef BW_ASIO_IL_H
#define BW_ASIO_IL_H
#include <asiosys.h>
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

//WARN: THESE FUNCTIONS CAN ONLY BE CALLED AFTER _bw_asio_init()
//      THESE FUNCTIONS CAN ONLY BE CALLED BEFORE _bw_asio_exit()
BWError _bw_asio_start(void);
BWError _bw_asio_stop(void);

BWError _bw_asio_get_channels(long* num_input_channels, long* num_output_channels);
BWError _bw_asio_get_latencies(long* input_latency, long* output_latency);
BWError _bw_asio_get_buffer_size(long* min_size, long* max_size, long* preferred_size, long* granularity);

BWError _bw_asio_can_sample_rate(double sample_rate, uint8_t* output);
BWError _bw_asio_get_sample_rate(double* current_sample_rate);
BWError _bw_asio_set_sample_rate(double sample_rate);

BWError _bw_asio_get_clock_sources(ASIOClockSource* clocks, long* num_sources);
BWError _bw_asio_set_clock_source(long reference);

BWError _bw_asio_get_sample_position(ASIOSamples* s_pos, ASIOTimeStamp* t_stamp);

BWError _bw_asio_get_channel_info(ASIOChannelInfo* info);

BWError _bw_asio_create_buffers(ASIOBufferInfo* buffer_infos, long num_channels, long buffer_size, ASIOCallbacks* callbacks);
BWError _bw_asio_dispose_buffers(void);

BWError _bw_asio_control_panel(void);

BWError _bw_asio_future(long selector, void* params);
BWError _bw_asio_output_ready(void);

#ifdef __cplusplus
}
#endif
#endif //BW_ASIO_IL_H
