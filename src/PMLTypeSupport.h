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

////        for(auto &str : varNameList)
////            cout << str << " ";
////        cout << endl;
////        cout << "preData size: ";
////        for(auto &chn : preData) {
////            if(chn.vct_dbl.empty())
////                cout << chn.vct_str.size() << ' ';
////            else
////                cout << chn.vct_dbl.size() << ' ';
////        }
////        cout << "postData size: ";
////        for(auto &chn : postData) {
////            if(chn.vct_dbl.empty())
////                cout << chn.vct_str.size() << ' ';
////            else
////                cout << chn.vct_dbl.size() << ' ';
////        }
//chrono::system_clock::time_point now = postTimestamp.front();
//time_t tp = chrono::system_clock::to_time_t(now);
//tm nowPoint;
//localtime_s(&nowPoint, &tp);
//void *fileHandle = nullptr;
//string name = "PML_" + to_string(nowPoint.tm_year + 1900) + two_digit(nowPoint.tm_mon + 1) + two_digit(nowPoint.tm_mday) + '_' +
//              two_digit(nowPoint.tm_hour) + two_digit(nowPoint.tm_min) + two_digit(nowPoint.tm_sec);
//cout << name << endl;
//string filePath = "D:/test/" + name + ".tdms";
//open_file(filePath.c_str(), "w", fileHandle);
//create_file_property_string(fileHandle, "name", name.c_str());
//create_file_property_string(fileHandle, "Description", warningDes.c_str());
//void *groupPreHandle = nullptr;
//void *groupPostHandle = nullptr;
//void *channelPreTimeHandle = nullptr;
//void *channelPostTimeHandle = nullptr;
//vector<void*> channelPreHandle(preData.size(), nullptr);
//vector<void*> channelPostHandle(postData.size(), nullptr);
//
//create_group(fileHandle, "Pre-shutdown", groupPreHandle);
//
//vector<const char *> cstrs;
//create_channel(groupPreHandle, "Time", channelPreTimeHandle);
//
//vector<timestamp> timestampList{};
//for (auto &ts: preTimestamp) {
//time_t time = chrono::system_clock::to_time_t(ts);
//tm timePoint = *localtime(&time);
//auto sinceEpoch = ts.time_since_epoch();
//auto micro = chrono::duration_cast<chrono::microseconds>(sinceEpoch) % chrono::seconds(1);
//timestamp t{timePoint.tm_year + 1900, timePoint.tm_mon + 1, timePoint.tm_mday, timePoint.tm_hour, timePoint.tm_min, timePoint.tm_sec, static_cast<int32_t>(micro.count())};
//timestampList.push_back(t);
//}
//set_channel_data_timestamp(channelPreTimeHandle, timestampList.size(), timestampList.data());
//
//for (size_t i = 0; i < varNameList.size(); i++) {
//create_channel(groupPreHandle, varNameList[i].c_str(), channelPreHandle[i]);
//
//create_channel_property_string(channelPreHandle[i], "Description", "");
//create_channel_property_string(channelPreHandle[i], "unit_string", "");
////            create_channel_property_int32(channelPreHandle[i], "NI_ArrayColumn", static_cast<int32_t>(i));
//
//if (preData[i].vec_dbl.empty()) {
//cstrs.clear();
//for (const auto &str: preData[i].vec_str)
//cstrs.push_back(str.c_str());
//set_channel_data_string(channelPreHandle[i], cstrs.size(), cstrs.data());
//} else {
//set_channel_data_double(channelPreHandle[i], preData[i].vec_dbl.size(), preData[i].vec_dbl.data());
//}
//}
//save_file(fileHandle);
//
//create_group(fileHandle, "Post-shutdown", groupPostHandle);
//
//create_channel(groupPostHandle, "Time", channelPostTimeHandle);
//timestampList.clear();
//for (auto &ts: postTimestamp) {
//time_t time = chrono::system_clock::to_time_t(ts);
//tm timePoint = *localtime(&time);
//auto sinceEpoch = ts.time_since_epoch();
//auto mirco = chrono::duration_cast<chrono::microseconds>(sinceEpoch) % chrono::seconds(1);
//timestamp t{timePoint.tm_year + 1900, timePoint.tm_mon + 1, timePoint.tm_mday, timePoint.tm_hour, timePoint.tm_min, timePoint.tm_sec, static_cast<int32_t>(mirco.count())};
//timestampList.push_back(t);
//}
//set_channel_data_timestamp(channelPostTimeHandle, timestampList.size(), timestampList.data());
//for (size_t i = 0; i < varNameList.size(); i++) {
//create_channel(groupPostHandle, varNameList[i].c_str(), channelPostHandle[i]);
//create_channel_property_string(channelPostHandle[i], "Description", "");
//create_channel_property_string(channelPostHandle[i], "unit_string", "");
////            create_channel_property_int32(channelPostHandle[i], "NI_ArrayColumn", static_cast<int32_t>(i));
//if (postData[i].vec_dbl.empty()) {
//cstrs.clear();
//for (const auto &str: postData[i].vec_str)
//cstrs.push_back(str.c_str());
//set_channel_data_string(channelPostHandle[i], cstrs.size(), cstrs.data());
//} else {
//set_channel_data_double(channelPostHandle[i], postData[i].vec_dbl.size(), postData[i].vec_dbl.data());
//}
//}
//save_file(fileHandle);
//close_file(fileHandle);
//cout << "finish write" << endl;

#endif //POSTMORTEMLOGGER_PMLTYPESUPPORT_H
