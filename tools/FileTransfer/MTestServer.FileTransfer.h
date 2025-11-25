#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

#ifdef _WIN32
#    ifdef DEVICE_EXPORT
#        define DEVICE_API __declspec(dllexport)
#    else
#        define DEVICE_API __declspec(dllimport)
#    endif
#else
#   define DEVICE_API __attribute__((visibility("default")))

#endif

#define FT_MODE_FTP_TRY_EXPLICIT    0
#define FT_MODE_FTP_EXPLICIT        1
#define FT_MODE_FTP_IMPLICIT        2
#define FT_MODE_FTP_PLAINTEXT       3
#define FT_MODE_SFTP                4

DEVICE_API int ft_add_server(const char *host_name, int type, const char *ip, int port, const char *user_name, const char *password, const char *default_remote_path);

DEVICE_API int ft_remove_server(const char *host_name);

DEVICE_API int ft_close_server(const char *host_name);

DEVICE_API int ft_clear_server();

/**
 * get all server name
 * @param host_list host list buffer, split by tab
 * @param size host_list size, if lack memory, size will be set to the required size
 * @return -1 is lack memory, 0 is success
 */
DEVICE_API int ft_get_server_list(char *host_list, int *size);

DEVICE_API int ft_get_server_count();

/**
 * download file from remote server
 * @param host_name host name in server list
 * @param local_file local file name
 * @param remote_file remote file name
 * @return 0 is success,-1 is not found server, -2 is connect error, -3 is login error, -4 is download error
 */
DEVICE_API int ft_download_file(const char *host_name, const char *local_file, const char *remote_file);

DEVICE_API int ft_upload_file(const char *host_name, const char *local_file, const char *remote_file);

DEVICE_API int ft_delete_file(const char *host_name, const char *file);

DEVICE_API int ft_rename_file(const char *host_name, const char *file, const char *new_file);

DEVICE_API int ft_mkdir(const char *host_name, const char *dir);

DEVICE_API int ft_chdir(const char *host_name, const char *dir);

DEVICE_API int ft_test_server_connection(const char *host_name);

DEVICE_API int ft_test_connection(int type, const char *ip, int port, const char *user_name, const char *password);

#ifdef __cplusplus
}
#endif