#define DEVICE_EXPORT

#include "mtestserver_device.h"
#include "src/PMLDevice.h"
#include <string>

#define SERVER_DEVICE Device
#define CAST_DEVICE(x) static_cast<SERVER_DEVICE *>(x)

bool str_copy(const std::string &str, char *desk_str, int32_t max_len) {
    desk_str[str.copy(desk_str, max_len - 1)] = '\0';
    return max_len > static_cast<int32_t>(str.size());
}

int32_t str_lack_copy(const std::string &str, char *desk_str, int32_t *max_len) {
    if (str_copy(str, desk_str, *max_len)) return DEVICE_NO_ERROR;
    *max_len = (int32_t) str.size() + 1;
    return DEVICE_LACK_MEMORY;
}

int32_t new_device(const char *name, const char *lic, const char *config, void *con, void *func, void **device) {
//    msg_log_info("new device");
    *device = new SERVER_DEVICE(name, config, con);
    return static_cast<int32_t>(CAST_DEVICE(*device)->get_error_code());
}

int32_t do_step(void *device) {
//    msg_log_info("do step");
    CHECK_HANDLE(device);
    CAST_DEVICE(device)->do_step();
    return DEVICE_NO_ERROR;
}

int32_t do_function(void *device, const char *name, const char *data, int32_t timeout, char *result, int32_t *max_len) {
//    msg_log_info("do function");
    std::string rt;
    CAST_DEVICE(device)->do_function(name, data, rt);
    return rt.empty() ? DEVICE_NO_ERROR : str_lack_copy(rt, result, max_len);
}

int32_t get_timing_loop_info(void *device, double *freq, int32_t *processor, int32_t *priority) {
//    msg_log_info("get timing loop info");
    CHECK_HANDLE(device);
    CAST_DEVICE(device)->get_timing_info(freq, processor, priority);
    return DEVICE_NO_ERROR;
}

int32_t get_all_variables(void *device, char *name, char *type, char *description, int32_t *max_len) {
//    msg_log_info("get alla variables");
    CHECK_HANDLE(device);
    CHECK_HANDLE(name);
    CHECK_HANDLE(type);
    CHECK_HANDLE(description);
    CHECK_HANDLE(max_len);
    std::string vars, des;
    CAST_DEVICE(device)->get_all_variables(vars, des);
    if (*max_len > 0 && str_copy(vars, name, *max_len) && str_copy(des, description, *max_len)) {
        type[0] = '\0';
        return DEVICE_NO_ERROR;
    }
    *max_len = (int32_t) (vars.size() > des.size() ? vars.size() : des.size()) + 1;
    return DEVICE_LACK_MEMORY;
}

int32_t get_write_variables(void *device, char *name, char *type, char *description, int32_t *max_len) {
//    msg_log_info("get write variables");
    CHECK_HANDLE(device);
    CHECK_HANDLE(name);
    CHECK_HANDLE(type);
    CHECK_HANDLE(description);
    CHECK_HANDLE(max_len);
    std::string vars, des;
    CAST_DEVICE(device)->get_written_variable(vars, des);
    if (*max_len > 0 && str_copy(vars, name, *max_len) && str_copy(des, description, *max_len)) {
        type[0] = '\0';
        return DEVICE_NO_ERROR;
    }
    *max_len = (int32_t) (vars.size() > des.size() ? vars.size() : des.size()) + 1;
    return DEVICE_LACK_MEMORY;
}

int32_t set_device_name(void *device, const char *name) {
//    msg_log_info("set device name");
    CHECK_HANDLE(device);
    CAST_DEVICE(device)->set_name(name);
    return DEVICE_NO_ERROR;
}

int32_t get_device_name(void *device, char *name, int32_t *max_len) {
//    msg_log_info("get device name");
    CHECK_HANDLE(device);
    return str_lack_copy(CAST_DEVICE(device)->get_name(), name, max_len);
}

int32_t get_device_version(char *version) {
//    msg_log_info("get device version");
    str_copy(SERVER_DEVICE::get_version(), version, MAX_VERSION_LEN);
    return DEVICE_NO_ERROR;
}

int32_t close_device(void *device) {
//    msg_log_info("close device");
    CHECK_HANDLE(device);
    delete CAST_DEVICE(device);
    return DEVICE_NO_ERROR;
}