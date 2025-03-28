//
// Created by 79933 on 2025/3/27.
//

#ifndef POSTMORTEMLOGGER_PMLDEVICE_H
#define POSTMORTEMLOGGER_PMLDEVICE_H

#include <cstdint>
#include <string>
#include <utility>

#include "PMLTypes.h"
#include "PMLTypeSupport.h"

using namespace std;

class PMLDevice {
public:
    explicit PMLDevice(string name, const shared_ptr<DeviceConfig> &config, void *con) : deviceName(std::move(name)), deviceConfig(config), con(con) {

    }

    explicit PMLDevice(const string &name, const string &config, void *con) : PMLDevice(name, parseConfig(config, errorcode), con) {}

    int32_t do_step() {
        return 0;
    }

    int32_t do_function(const string &name, const string &data, const string &rt) {
        return 0;
    }

    void GetAllVariables(std::string &vars, std::string &des) {
        int i = 1, j = 1;
        for (auto &chn: deviceConfig->channels) {
            if (!chn->varName.empty()) {
                vars += chn->varName + ",";
            }
        }
        if (vars.back() == ',')vars.pop_back();
    }

    void GetWrittenVariables(std::string &vars, std::string &des) {}

    int32_t get_error_code() const { return errorcode; }

    int32_t get_timing_info(double *freq, int32_t *processor, int32_t *priority) {
        return 0;
    }

    int32_t set_name(const string &name) { deviceName = name; }

    string get_name() const { return deviceName; }

    static std::string get_version() { return "V1.0.0"; }

private:
    string deviceName;
    shared_ptr<DeviceConfig> deviceConfig;
    void *con;
    int32_t errorcode;
};

#endif //POSTMORTEMLOGGER_PMLDEVICE_H
