/**
 * @file mtestserver_common.h
 * @author Julles
 * @brief Define API for mtestserver common.
 * @version 1.0
 * @date 2025-04-15
 *
 * @copyright ITIMotor Copyright (c) 2025
 *
 */
#ifndef MTESTSERVER_COMMON_H
#define MTESTSERVER_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

#ifdef _WIN32
#    ifdef MTEST_EXPORT
#        define MTEST_API __declspec(dllexport)
#    else
#        define MTEST_API __declspec(dllimport)
#    endif
#else
#   define MTEST_API __attribute__((visibility("default")))

#endif

#define MAX_VERSION_LEN 64

//API ERROR CODE
#define DEVICE_NO_ERROR 0
#define DEVICE_HANDLE_ERROR (-1)
#define DEVICE_LACK_MEMORY (-2)

//Check null pointer and returns a handle error
#define CHECK_HANDLE(x) if(x == nullptr) return DEVICE_HANDLE_ERROR

//Channel List

/**
 * Clear all ChannelList
 */
MTEST_API void channel_list_clear();

/**
 * Add a list of channels with a specified name
 * @param name The name of the channel list cannot be repeated, and if it is repeated, the previous one will be replaced
 * @param channel_list Channel list, split by line breaks, one channel for every 7 fields,
 * The first field is the statistical type, the second field is the channel name, the third field is the variable name,
 * the fourth field is the variable type, the fifth field is the variable unit, the sixth field is the channel type, and the seventh field is the channel description
 * @return Whether to add success 0 - succeeded, 1 - already existed and replaced, other failed
 */
MTEST_API int channel_list_add(const char *name, const char *channel_list);

/**
 * Gets a list of channels with a specified name
 * @param name The name of the list of channels to get
 * @param channel_list Channel list, split by line breaks, one channel for every 7 fields,
 * The first field is the statistical type, the second field is the channel name, the third field is the variable name,
 * the fourth field is the variable type, the fifth field is the variable unit, the sixth field is the channel type, and the seventh field is the channel description
 * @param channel_list_size The size of the channel list, and when the bufferPtr size is insufficient, the size of the bufferPtr is returned
 * @return Whether to get success 0 - success, -1 - non-existent, -2 - missing memory, other failed
 */
MTEST_API int channel_list_get(const char *name, char *channel_list, int32_t *channel_list_size);

/**
 * Removes the list of channels with the specified name
 * @param name The name of the channel list
 * @return Removal Success 0 - Success, 1 - non-existent, Others Failed
 */
MTEST_API int channel_list_remove(const char *name);

/**
 * Rename the list of channels with the specified name
 * @param name old name
 * @param new_name new name
 * @return 0 - success, -1 - non-existent
 */
MTEST_API int channel_list_rename(const char *name, const char *new_name);


//MTest Config

#define CFG_BIN_PATH "bin_path"
#define CFG_CONFIG_PATH "config_path"
#define CFG_DATA_PATH "data_path"
#define CFG_LOG_PATH "log_path"

/**
 * Set the server global configuration
 * @param name config name
 * @param value config value
 * @return 0 - Success, -1 This configuration does not exist
 */
MTEST_API int mtest_config_set(const char *name, const char *value);

/**
 * Get the server global configuration
 * @param name config name
 * @param value config value
 * @param value_size config value bufferPtr size
 * @return 0 - Success, -1 This configuration does not exist, -2 missing memory, Others fail
 */
MTEST_API int mtest_config_get(const char *name, char *value, int32_t *value_size);

#ifdef __cplusplus
}
#endif

#endif //MTESTSERVER_COMMON_H
