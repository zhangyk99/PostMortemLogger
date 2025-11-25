//
// Created by 79933 on 2025/3/27.
//

#ifndef POSTMORTEMLOGGER_PMLDEVICE_H
#define POSTMORTEMLOGGER_PMLDEVICE_H

#include "PMLTypes.h"
#include "PMLTypeSupport.h"
#include "PMLVariable.h"

#include <memory>

class Device{
    std::string deviceName;
    std::shared_ptr<DeviceConfig> deviceConfig;
    void *con;
    int32_t errorCode = 0;
    std::shared_ptr<Variable> varPtr;
    std::vector<std::string> varDblList;
    std::vector<std::string> varStrList;
    std::vector<std::string> chnDblList;
    std::vector<std::string> chnStrList;
    std::unordered_map<std::string, std::string> unitMap;
    std::unordered_map<std::string, std::string> desMap;
    std::vector<Property> proList;

public:
    explicit Device(std::string name, const std::shared_ptr<DeviceConfig> &config, void *con) : deviceName(std::move(name)), deviceConfig(config), con(con) {
//        std::cout << "create PMLDevice" << std::endl;
//        msg_log_info("error code: " + std::to_string(errorCode));
        for(auto &chn : deviceConfig->channels) {
            if (chn->channelType == "Channel") {
                if(chn->varType == "DBL") {
                    varDblList.emplace_back(chn->varName);
                    if (chn->channelName.empty())
                        chnDblList.emplace_back(chn->varName);
                    else
                        chnDblList.emplace_back(chn->channelName);
                } else if (chn->varType == "STR") {
                    varStrList.emplace_back(chn->varName);
                    if (chn->channelName.empty())
                        chnStrList.emplace_back(chn->varName);
                    else
                        chnStrList.emplace_back(chn->channelName);
                }
                if(chn->channelName.empty()) {
                    unitMap[chn->varName] = chn->varUnit;
                    desMap[chn->varName] = chn->channelDes;
                }else {
                    unitMap[chn->channelName] = chn->varUnit;
                    desMap[chn->channelName] = chn->channelDes;
                }
            } else if (chn->channelType == "Property") {
                if(chn->channelType == "DBL") {
                    double pDouble;
                    con_get_value_dbl(con, chn->channelName.c_str(), &pDouble);
                    proList.emplace_back(chn->channelName, pDouble);
                }
                if(chn->channelType == "STR") {
                    char pString[64];
                    con_get_value_str(con, chn->varName.c_str(), pString);
                    proList.emplace_back(chn->channelName, pString);
                }
            } else {
//                auto vec_chn = parseChannelList(chn->channelName);
//                for (const auto &cn: vec_chn) {
//                    if (cn.channelType == "Channel") {
//                        if(chn->varType == "DBR") {
//                            varDblList.emplace_back(chn->varName);
//                            if (chn->channelName.empty())
//                                chnDblList.emplace_back(chn->varName);
//                            else
//                                chnDblList.emplace_back(chn->channelName);
//                        }
//                        else if (chn->varType == "STR") {
//                            varStrList.emplace_back(chn->varName);
//                            if (chn->channelName.empty())
//                                chnStrList.emplace_back(chn->varName);
//                            else
//                                chnStrList.emplace_back(chn->channelName);
//                        }
//                    } else {
//                        if(chn->channelType == "DBR") {
//                            double pDouble;
//                            con_get_value_dbl(con, chn->channelName.c_str(), &pDouble);
//                            proList.emplace_back(chn->channelName, pDouble);
//                        }
//                        if(chn->channelType == "STR") {
//                            char pString[64];
//                            con_get_value_str(con, chn->varName.c_str(), pString);
//                            proList.emplace_back(chn->channelName, pString);
//                        }
//                    }
//                }
            }
        }
        varPtr = std::make_shared<Variable>(varDblList, varStrList, chnDblList, chnStrList, con, RecordType::MDF4, deviceConfig->preStopTime, deviceConfig->postStopTime, deviceConfig->frequency);
    }

    explicit Device(const std::string &name, const std::string &config, void *con) : Device(name, parseConfig(config, errorCode), con) {}

    int32_t do_step() {
        return 0;
    }

    int32_t do_function(const std::string &name, const std::string &data, const std::string &rt) {
        return 0;
    }

    void get_all_variables(std::string &vars, std::string &des) {
        for (auto &chn: deviceConfig->channels)
            if (!chn->varName.empty())
                vars += chn->varName + ",";
        if (vars.back() == ',')vars.pop_back();
    }

    void get_written_variable(std::string &vars, std::string &des) {}

    int32_t get_error_code() const {
//        msg_log_info("get error code: " + std::to_string(errorCode));
        return errorCode;
    }

    int32_t get_timing_info(double *freq, int32_t *processor, int32_t *priority) {
        return 0;
    }

    void set_name(const std::string &name) { deviceName = name; }

    std::string get_name() const { return deviceName; }

    static std::string get_version() { return "V1.0.0"; }
};

#endif //POSTMORTEMLOGGER_PMLDEVICE_H
