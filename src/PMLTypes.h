//
// Created by 79933 on 2025/3/27.
//

#ifndef POSTMORTEMLOGGER_PMLTYPES_H
#define POSTMORTEMLOGGER_PMLTYPES_H

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
using namespace std;

struct Channel{
    string statisticsType;
    string channelName;
    string varName;
    string varType;
    string varUnit;
    string channelType;
    string channelDes;
};

struct DeviceConfig{
    double preStopTime;
    double postStopTime;
    double frequency;
    uint16_t fileType;
    vector<shared_ptr<Channel>> channels;
};

#endif //POSTMORTEMLOGGER_PMLTYPES_H
