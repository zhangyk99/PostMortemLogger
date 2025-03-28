//
// Created by 79933 on 2025/3/27.
//

#ifndef POSTMORTEMLOGGER_PMLTYPESUPPORT_H
#define POSTMORTEMLOGGER_PMLTYPESUPPORT_H

#include <memory>
#include <vector>
#include "PMLTypeSupport.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <rapidjson/error/en.h>

#define get_json_string(x) x.IsNull() ? "" : x.GetString()

shared_ptr<DeviceConfig> parseConfig(const std::string &config) {
    rapidjson::Document doc1;
    doc1.Parse<rapidjson::kParseNanAndInfFlag>(config.c_str());
    if (doc1.HasParseError())
        return nullptr;
    const rapidjson::Value &doc = doc1["DeviceConfig"];
    auto cfg = std::make_shared<DeviceConfig>();
    cfg->preStopTime = doc["PreStopTime"].GetDouble();
    cfg->postStopTime = doc["PostStopTime"].GetDouble();
    cfg->frequency = doc["Frequence"].GetDouble();
    cfg->fileType = doc["FileType"].GetInt();
    const rapidjson::Value &Channels = doc["Channels"];
    for(rapidjson::SizeType i = 0; i < Channels.Size(); i++) {
        shared_ptr<Channel> chn;
        chn->statisticsType = get_json_string(doc["StatisticsType"]);
        chn->channelName = get_json_string(doc["ChannelName"]);
        chn->varName = get_json_string(doc["VarName"]);
        chn->varType = get_json_string(doc["VarType"]);
        chn->varUnit = get_json_string(doc["VarUnit"]);
        chn->channelType = get_json_string(doc["ChannelType"]);
        chn->channelDes = get_json_string(doc["ChannelDes"]);
        cfg->channels.push_back(chn);
    }
    return cfg;
}

shared_ptr<DeviceConfig> parseConfig(const std::string &config, int &errorCode) {
    try {
        auto cfg = parseConfig(config);
        if (cfg) return cfg;
        errorCode = 5000;
        return std::make_shared<DeviceConfig>();
    } catch (...) {
        errorCode = 5000;
        return std::make_shared<DeviceConfig>();
    }
}

#endif //POSTMORTEMLOGGER_PMLTYPESUPPORT_H
