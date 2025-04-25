//
// Created by 79933 on 2025/3/27.
//

#ifndef POSTMORTEMLOGGER_PMLTYPES_H
#define POSTMORTEMLOGGER_PMLTYPES_H

#include "varmanager/varmanager.h"

#include <cstdint>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <memory>
#include <chrono>

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

struct Data{
    double dbl;
    string str;
    chrono::system_clock::time_point timePoint;

    Data() : dbl(0), timePoint(std::chrono::system_clock::time_point{}) {}

    explicit Data(const double d) : dbl(d), timePoint(chrono::system_clock::now()) {}

    explicit Data(string &s) : dbl(0), str(s), timePoint(chrono::system_clock::now()) {}

    bool operator==(const Data& other) const {
        return dbl == other.dbl &&
               str == other.str &&
               timePoint == other.timePoint;
    }

    bool operator!=(const Data& other) const {
        return !(*this == other);
    }

    [[nodiscard]] bool empty() const {
        return dbl == 0 &&
               str.empty() &&
               timePoint.time_since_epoch().count() == 0;
    }
};

struct ChannelData{
    bool type;
    deque<Data> dataQue{};
};

struct TargetData{
    vector<double> vec_dbl;
    vector<string> vec_str;
};

#endif //POSTMORTEMLOGGER_PMLTYPES_H
