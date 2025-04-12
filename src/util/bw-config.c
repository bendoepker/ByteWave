#include <bw-config.h>
#include <bw-log.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

BWError _get_current_directory(char** directory_name);

BWError BWConfig_Write(char* file_name, BWConfigData* data) {
    uint32_t version = 1; //This is version one of the config file
    FILE* file = fopen(file_name, "w");
    if(file == 0) return BW_FILE_OPEN_ERROR;

    //SECTION: File ID
    size_t num_written = fwrite("BWCD", 1, 4, file);
    if(num_written != 4) return BW_FILE_WRITE_ERROR;

    //SECTION: Version Tag
    num_written = fwrite(&version, 4, 1, file);
    if(num_written != 1) return BW_FILE_WRITE_ERROR;

    //SECTION: Host API Tag
    num_written = fwrite(&data->host_api, 1, 4, file);
    if(num_written != 4) return BW_FILE_WRITE_ERROR;

    //SECTION: Device Name Length
    num_written = fwrite(&data->device_name_length, 4, 1, file);
    if(num_written != 1) return BW_FILE_WRITE_ERROR;

    //SECTION: Device Name
    num_written = fwrite(data->device_name, 1, data->device_name_length, file);
    if(num_written != data->device_name_length) return BW_FILE_WRITE_ERROR;

    //SECTION: Window Width
    num_written = fwrite(&data->window_width, 4, 1, file);
    if(num_written != 1) return BW_FILE_WRITE_ERROR;

    //SECTION: Window Height
    num_written = fwrite(&data->window_height, 4, 1, file);
    if(num_written != 1) return BW_FILE_WRITE_ERROR;

    fclose(file);
    return BW_OK;
}

BWError BWConfig_Read(char* file_name, BWConfigData* data) {
    FILE* file = fopen(file_name, "r");
    uint32_t version; //Should be 1
    if(file == 0) return BW_FILE_OPEN_ERROR;

    //SECTION: Verify File ID
    char file_id[4];
    size_t num_read = fread(file_id, 1, 4, file);
    if(num_read != 4) return BW_FILE_READ_ERROR;

    //SECTION: Verify Version Tag
    num_read = fread(&version, 4, 1, file);
    if(num_read != 1) return BW_FILE_READ_ERROR;
    if(version != 1) return BW_VERSION_INCOMPATIBLE;

    //SECTION: Host API
    num_read = fread(data->host_api, 1, 4, file);
    if(num_read != 4) return BW_FILE_READ_ERROR;

    //SECTION: Device Name Length
    num_read = fread(&data->device_name_length, 4, 1, file);
    if(num_read != 1) return BW_FILE_READ_ERROR;

    //SECTION: Device Name
    data->device_name = (char*)malloc(data->device_name_length);
    num_read = fread(data->device_name, 1, data->device_name_length, file);
    if(num_read != data->device_name_length) return BW_FILE_READ_ERROR;

    //SECTION: Window Width
    num_read = fread(&data->window_width, 4, 1, file);
    if(num_read != 1) return BW_FILE_READ_ERROR;

    //SECTION: Window Height
    num_read = fread(&data->window_height, 4, 1, file);
    if(num_read != 1) return BW_FILE_READ_ERROR;

    fclose(file);

    return BW_OK;
}

#ifdef _WIN32
BWError _get_current_directory(char** directory_name) {
    int name_length = GetCurrentDirectory(0, 0);
    if(name_length == 0) return BW_FAILED; //Could not get the directory name
    *directory_name = (char*)malloc(name_length);
    if(directory_name == 0) return BW_FAILED_MALLOC;

    if(!GetCurrentDirectory(name_length, *directory_name)) return BW_FAILED;
    return BW_OK;
}
#elif defined __linux__

BWError _get_current_directory(char** directory_name) {
    //TODO: THIS
}
#endif //Windows
