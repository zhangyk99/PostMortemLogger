//
// Created by 79933 on 2025/3/12.
//

#ifndef TDMS_MASTER_TDMS_MASTER_H
#define TDMS_MASTER_TDMS_MASTER_H

#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

#include "TDMS_errorcode.h"
#include "cstdint"

#ifdef _WIN32
#    ifdef TDMS_EXPORT
#        define TDMS_API __declspec(dllexport)
#    else
#        define TDMS_API __declspec(dllimport)
#    endif
#else
#     define TDMS_API __attribute__ ((visibility("default")))

#endif
struct timestamp{
    int32_t year;
    int32_t mon;
    int32_t day;
    int32_t hour;
    int32_t minter;
    int32_t second;
    int32_t microsecond;

    bool operator==(const timestamp &other) const {
        return (year == other.year) && (mon == other.mon) && (day == other.day) && (hour == other.hour) && (minter == other.minter) && (second == other.second) && (microsecond == other.microsecond);
    }
};

//file option
TDMS_API int32_t open_file(const char *filePath, const char *mode, void *&fileHandle);

TDMS_API int32_t close_file(void *&fileHandle);

TDMS_API int32_t save_file(void *fileHandle);

TDMS_API int32_t delete_file(const char *filePath);

//file property
TDMS_API int32_t get_num_file_property(void *fileHandle);

TDMS_API int32_t get_file_property(void *fileHandle, const char *propertyNames[]);

TDMS_API int32_t get_file_property_datatype(void *fileHandle, const char *propertyName);

TDMS_API int32_t get_file_property_int8(void *fileHandle, const char *name, int8_t &data);

TDMS_API int32_t get_file_property_int16(void *fileHandle, const char *name, int16_t &data);

TDMS_API int32_t get_file_property_int32(void *fileHandle, const char *name, int32_t &data);

TDMS_API int32_t get_file_property_int64(void *fileHandle, const char *name, int64_t &data);

TDMS_API int32_t get_file_property_uint8(void *fileHandle, const char *name, uint8_t &data);

TDMS_API int32_t get_file_property_uint16(void *fileHandle, const char *name, uint16_t &data);

TDMS_API int32_t get_file_property_uint32(void *fileHandle, const char *name, uint32_t &data);

TDMS_API int32_t get_file_property_uint64(void *fileHandle, const char *name, uint64_t &data);

TDMS_API int32_t get_file_property_float(void *fileHandle, const char *name, float &data);

TDMS_API int32_t get_file_property_double(void *fileHandle, const char *name, double &data);

TDMS_API int32_t get_file_property_string(void *fileHandle, const char *name, char *data, size_t size);

TDMS_API int32_t get_file_property_bool(void *fileHandle, const char *name, bool &data);

TDMS_API int32_t get_file_property_timestamp(void *fileHandle, const char *name, timestamp &data);

TDMS_API int32_t get_file_property_time(void *fileHandle, const char *name, int64_t &data);

TDMS_API int32_t create_file_property_int8(void *fileHandle, const char *name, int8_t data);

TDMS_API int32_t create_file_property_int16(void *fileHandle, const char *name, int16_t data);

TDMS_API int32_t create_file_property_int32(void *fileHandle, const char *name, int32_t data);

TDMS_API int32_t create_file_property_int64(void *fileHandle, const char *name, int64_t data);

TDMS_API int32_t create_file_property_uint8(void *fileHandle, const char *name, uint8_t data);

TDMS_API int32_t create_file_property_uint16(void *fileHandle, const char *name, uint16_t data);

TDMS_API int32_t create_file_property_uint32(void *fileHandle, const char *name, uint32_t data);

TDMS_API int32_t create_file_property_uint64(void *fileHandle, const char *name, uint64_t data);

TDMS_API int32_t create_file_property_float(void *fileHandle, const char *name, float data);

TDMS_API int32_t create_file_property_double(void *fileHandle, const char *name, double data);

TDMS_API int32_t create_file_property_string(void *fileHandle, const char *name, const char *data);

TDMS_API int32_t create_file_property_bool(void *fileHandle, const char *name, bool data);

TDMS_API int32_t create_file_property_timestamp(void *fileHandle, const char *name, timestamp data);

TDMS_API int32_t create_file_property_time(void *fileHandle, const char *name, int64_t data);

TDMS_API int32_t delete_file_property(void *fileHandle, const char* name);

//group option
TDMS_API int32_t create_group(void *fileHandle, const char *name, void *&groupHandle);

TDMS_API int32_t delete_group(void *groupHandle);

TDMS_API int32_t get_num_group(void *fileHandle);

TDMS_API int32_t get_group(void *fileHandle, void *groupHandles[]);

TDMS_API int32_t get_group_name(void *groupHandle, char *name);

//group property
TDMS_API int32_t get_num_group_property(void *groupHandle);

TDMS_API int32_t get_group_property(void *groupHandle, const char *propertyNames[]);

TDMS_API int32_t get_group_property_datatype(void *groupHandle, const char *name);

TDMS_API int32_t get_group_property_int8(void *groupHandle, const char *name, int8_t &data);

TDMS_API int32_t get_group_property_int16(void *groupHandle, const char *name, int16_t &data);

TDMS_API int32_t get_group_property_int32(void *groupHandle, const char *name, int32_t &data);

TDMS_API int32_t get_group_property_int64(void *groupHandle, const char *name, int64_t &data);

TDMS_API int32_t get_group_property_uint8(void *groupHandle, const char *name, uint8_t &data);

TDMS_API int32_t get_group_property_uint16(void *groupHandle, const char *name, uint16_t &data);

TDMS_API int32_t get_group_property_uint32(void *groupHandle, const char *name, uint32_t &data);

TDMS_API int32_t get_group_property_uint64(void *groupHandle, const char *name, uint64_t &data);

TDMS_API int32_t get_group_property_float(void *groupHandle, const char *name, float &data);

TDMS_API int32_t get_group_property_double(void *groupHandle, const char *name, double &data);

TDMS_API int32_t get_group_property_string(void *groupHandle, const char *name, char *data, size_t size);

TDMS_API int32_t get_group_property_bool(void *groupHandle, const char *name, bool &data);

TDMS_API int32_t get_group_property_timestamp(void *groupHandle, const char *name, timestamp &data);

TDMS_API int32_t get_group_property_time(void *groupHandle, const char *name, int64_t &data);

TDMS_API int32_t create_group_property_int8(void *groupHandle, const char *name, int8_t data);

TDMS_API int32_t create_group_property_int16(void *groupHandle, const char *name, int16_t data);

TDMS_API int32_t create_group_property_int32(void *groupHandle, const char *name, int32_t data);

TDMS_API int32_t create_group_property_int64(void *groupHandle, const char *name, int64_t data);

TDMS_API int32_t create_group_property_uint8(void *groupHandle, const char *name, uint8_t data);

TDMS_API int32_t create_group_property_uint16(void *groupHandle, const char *name, uint16_t data);

TDMS_API int32_t create_group_property_uint32(void *groupHandle, const char *name, uint32_t data);

TDMS_API int32_t create_group_property_uint64(void *groupHandle, const char *name, uint64_t data);

TDMS_API int32_t create_group_property_float(void *groupHandle, const char *name, float data);

TDMS_API int32_t create_group_property_double(void *groupHandle, const char *name, double data);

TDMS_API int32_t create_group_property_string(void *groupHandle, const char *name, const char *data);

TDMS_API int32_t create_group_property_bool(void *groupHandle, const char *name, bool data);

TDMS_API int32_t create_group_property_timestamp(void *groupHandle, const char *name, timestamp data);

TDMS_API int32_t create_group_property_time(void *groupHandle, const char *name, int64_t data);

TDMS_API int32_t delete_group_property(void *groupHandle, const char *name);

//channel option
TDMS_API int32_t create_channel(void *groupHandle, const char *name, void*&channelHandle);

TDMS_API int32_t delete_channel(void *channelHandle);

TDMS_API int32_t get_num_channel(void *groupHandle);

TDMS_API int32_t get_channel(void *groupHandle, void *channelHandles[]);

TDMS_API int32_t get_channel_name(void *channelHandle, char *name);

//
TDMS_API int32_t get_num_channel_property(void *channelHandle);

TDMS_API int32_t get_channel_property(void *channelHandle, const char *propertyNames[]);

TDMS_API int32_t get_channel_property_datatype(void *channelHandle, const char *name);

TDMS_API int32_t get_channel_property_int8(void *channelHandle, const char *name, int8_t &data);

TDMS_API int32_t get_channel_property_int16(void *channelHandle, const char *name, int16_t &data);

TDMS_API int32_t get_channel_property_int32(void *channelHandle, const char *name, int32_t &data);

TDMS_API int32_t get_channel_property_int64(void *channelHandle, const char *name, int64_t &data);

TDMS_API int32_t get_channel_property_uint8(void *channelHandle, const char *name, uint8_t &data);

TDMS_API int32_t get_channel_property_uint16(void *channelHandle, const char *name, uint16_t &data);

TDMS_API int32_t get_channel_property_uint32(void *channelHandle, const char *name, uint32_t &data);

TDMS_API int32_t get_channel_property_uint64(void *channelHandle, const char *name, uint64_t &data);

TDMS_API int32_t get_channel_property_float(void *channelHandle, const char *name, float &data);

TDMS_API int32_t get_channel_property_double(void *channelHandle, const char *name, double &data);

TDMS_API int32_t get_channel_property_string(void *channelHandle, const char *name, char *data, size_t size);

TDMS_API int32_t get_channel_property_bool(void *channelHandle, const char *name, bool &data);

TDMS_API int32_t get_channel_property_timestamp(void *channelHandle, const char *name, timestamp &data);

TDMS_API int32_t get_channel_property_time(void *channelHandle, const char *name, int64_t &data);

TDMS_API int32_t create_channel_property_int8(void *channelHandle, const char *name, int8_t data);

TDMS_API int32_t create_channel_property_int16(void *channelHandle, const char *name, int16_t data);

TDMS_API int32_t create_channel_property_int32(void *channelHandle, const char *name, int32_t data);

TDMS_API int32_t create_channel_property_int64(void *channelHandle, const char *name, int64_t data);

TDMS_API int32_t create_channel_property_uint8(void *channelHandle, const char *name, uint8_t data);

TDMS_API int32_t create_channel_property_uint16(void *channelHandle, const char *name, uint16_t data);

TDMS_API int32_t create_channel_property_uint32(void *channelHandle, const char *name, uint32_t data);

TDMS_API int32_t create_channel_property_uint64(void *channelHandle, const char *name, uint64_t data);

TDMS_API int32_t create_channel_property_float(void *channelHandle, const char *name, float data);

TDMS_API int32_t create_channel_property_double(void *channelHandle, const char *name, double data);

TDMS_API int32_t create_channel_property_string(void *channelHandle, const char *name, const char *data);

TDMS_API int32_t create_channel_property_bool(void *channelHandle, const char *name, bool data);

TDMS_API int32_t create_channel_property_timestamp(void *channelHandle, const char *name, timestamp data);

TDMS_API int32_t create_channel_property_time(void *channelHandle, const char *name, int64_t data);

TDMS_API int32_t delete_channel_property(void *channelHandle, const char *name);

//channel data
TDMS_API int32_t get_channel_datatype(void *channelHandle);

TDMS_API int32_t get_num_channel_data(void *channelHandle, uint64_t &dataNum);

TDMS_API int32_t get_channel_data_int8(void *channelHandle, int8_t data[]);

TDMS_API int32_t get_channel_data_int16(void *channelHandle, int16_t data[]);

TDMS_API int32_t get_channel_data_int32(void *channelHandle, int32_t data[]);

TDMS_API int32_t get_channel_data_int64(void *channelHandle, int64_t data[]);

TDMS_API int32_t get_channel_data_uint8(void *channelHandle, uint8_t data[]);

TDMS_API int32_t get_channel_data_uint16(void *channelHandle, uint16_t data[]);

TDMS_API int32_t get_channel_data_uint32(void *channelHandle, uint32_t data[]);

TDMS_API int32_t get_channel_data_uint64(void *channelHandle, uint64_t data[]);

TDMS_API int32_t get_channel_data_float(void *channelHandle, float data[]);

TDMS_API int32_t get_channel_data_double(void *channelHandle, double data[]);

TDMS_API int32_t get_channel_data_string(void *channelHandle, const char *data[]);

TDMS_API int32_t get_channel_data_bool(void *channelHandle, bool data[]);

TDMS_API int32_t get_channel_data_timestamp(void *channelHandle, timestamp data[]);

TDMS_API int32_t set_channel_data_int8(void *channelHandle, uint64_t dataNum, int8_t data[]);

TDMS_API int32_t set_channel_data_int16(void *channelHandle, uint64_t dataNum, int16_t data[]);

TDMS_API int32_t set_channel_data_int32(void *channelHandle, uint64_t dataNum, int32_t data[]);

TDMS_API int32_t set_channel_data_int64(void *channelHandle, uint64_t dataNum, int64_t data[]);

TDMS_API int32_t set_channel_data_uint8(void *channelHandle, uint64_t dataNum, uint8_t data[]);

TDMS_API int32_t set_channel_data_uint16(void *channelHandle, uint64_t dataNum, uint16_t data[]);

TDMS_API int32_t set_channel_data_uint32(void *channelHandle, uint64_t dataNum, uint32_t data[]);

TDMS_API int32_t set_channel_data_uint64(void *channelHandle, uint64_t dataNum, uint64_t data[]);

TDMS_API int32_t set_channel_data_float(void *channelHandle, uint64_t dataNum, float data[]);

TDMS_API int32_t set_channel_data_double(void *channelHandle, uint64_t dataNum, double data[]);

TDMS_API int32_t set_channel_data_string(void *channelHandle, uint64_t dataNum, const char *data[]);

TDMS_API int32_t set_channel_data_bool(void *channelHandle, uint64_t dataNum, bool data[]);

TDMS_API int32_t set_channel_data_timestamp(void *channelHandle, uint64_t dataNum, timestamp data[]);

TDMS_API int32_t set_channel_data_time(void *channelHandle, uint64_t dataNum, int64_t data[]);

TDMS_API const char *get_error_description(int32_t errorCode);

#ifdef __cplusplus
};
#endif

#endif //TDMS_MASTER_TDMS_MASTER_H