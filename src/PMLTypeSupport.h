//
// Created by 79933 on 2025/3/27.
//

#ifndef POSTMORTEMLOGGER_PMLTYPESUPPORT_H
#define POSTMORTEMLOGGER_PMLTYPESUPPORT_H

//#include <memory>
//#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <rapidjson/error/en.h>

#define get_json_string(x) x.IsNull() ? "" : x.GetString()

std::shared_ptr<DeviceConfig> parseConfig(const std::string &config) {
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
    for (rapidjson::SizeType i = 0; i < Channels.Size(); i++) {
        auto chn = std::make_shared<Channel>();
        chn->statisticsType = get_json_string(Channels[i]["StatisticsType"]);
        chn->channelName = get_json_string(Channels[i]["ChannelName"]);
        chn->varName = get_json_string(Channels[i]["VarName"]);
        chn->varType = get_json_string(Channels[i]["VarType"]);
        chn->varUnit = get_json_string(Channels[i]["VarUnit"]);
        chn->channelType = get_json_string(Channels[i]["ChannelType"]);
        chn->channelDes = get_json_string(Channels[i]["ChannelDes"]);
        cfg->channels.push_back(chn);
    }
    return cfg;
}

std::shared_ptr<DeviceConfig> parseConfig(const std::string &config, int &errorCode) {
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

//std::vector<Channel> parseChannelList(const std::string &channelListName) {
//    int32_t size = 0;
//    if (channel_list_get(channelListName.c_str(), nullptr, &size) != -1)
//        return {};
//    std::string chn_str{};
//    chn_str.resize(size);
//    if (channel_list_get(channelListName.c_str(), chn_str.data(), &size) != 0)
//        return {};
//    std::vector<Channel> vec_chn{};
//    std::stringstream ss(chn_str);
//    std::string item;
//    std::vector<std::string> result;
//    while (std::getline(ss, item, ',')) {
//        result.push_back(item);
//    }
//    if (result.size() % 7 != 0)
//        return {};
//    Channel chn;
//    for (size_t i = 0; i < result.size() / 7; i++) {
//        chn.statisticsType = result[i];
//        chn.channelName =    result[i + 1];
//        chn.varName =        result[i + 2];
//        chn.varType =        result[i + 3];
//        chn.varUnit =        result[i + 4];
//        chn.channelType =    result[i + 5];
//        chn.channelDes =     result[i + 6];
//        vec_chn.push_back(chn);
//    }
//    return vec_chn;
//}

//void VarRecord() {
//    chrono::system_clock::time_point begin = chrono::system_clock::now();
//    queue<event_data_t> que{};
//    this_thread::sleep_for(chrono::microseconds(1000000));
//    {
//        lock_guard<mutex> lock(mtx_data);
//        primaryData.swap(que);
//    }
//    auto begin_time = TransformTime(&que.front().time);
//    auto end_time = TransformTime(&que.back().time);
//    auto time_pos = begin_time + chrono::microseconds(1000);
//    size_t index = 1;
//    vector<timestamp> ts;
//    auto ts_size = chrono::duration_cast<chrono::milliseconds>(end_time - begin_time).count();
//    ts.reserve(ts_size);
//    for(size_t i = 0; i < ts_size; i++){
//        time_t tp = std::chrono::system_clock::to_time_t(begin_time + chrono::microseconds(1000*i));
//        tm t;
//        localtime_s(&t, &tp);
//        ts.push_back({t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, static_cast<int32_t>((chrono::duration_cast<chrono::microseconds>((begin_time + chrono::microseconds(1000*i)).time_since_epoch()) % chrono::seconds(1)).count())});
//    }
//    for (auto &chn: table) {
//        if (chn.type) {
//            chn.dbl.clear();
//            chn.dbl.resize(ts_size);
//        }
//        else {
//            chn.str.clear();
//            chn.str.resize(ts_size);
//        }
//    }
//    vector<uint32_t> chnNum(ts_size, 1);
//    uint32_t count = 1;
//    for(size_t i = 0; i < table.size(); i++){
//        table[i].dbl[0] = beginData[i];
//    }
//    while (!que.empty()) {
//        if (TransformTime(&que.front().time) > time_pos) {
////                for (size_t i = 0; i < table.size(); i++) {
////                    if (table[i].type) {
////                        if (chnNum[i] == index) {
////                            table[i].dbl[index] = table[i].dbl[index - 1];
////                            chnNum[i]++;
////                        }
////                    } else {
////                        if (chnNum[i] == index) {
////                            table[i].str[index] = table[i].str[index - 1];
////                            chnNum[i]++;
////                        }
////                    }
////                }
//            index++;
//            if(index == ts_size)
//                index--;
//            time_pos = time_pos + chrono::microseconds(1000);
//        } else {
//            if (table[var_index[que.front().name]].type) {
//                chnNum[var_index[que.front().name]]++;
//                table[var_index[que.front().name]].dbl[index] = que.front().value_dbl;
//            } else {
//                chnNum[var_index[que.front().name]]++;
//                table[var_index[que.front().name]].str[index] = que.front().value_str;
//            }
//            que.pop();
//        }
////            cout << index << " " << count << endl;
//        count++;
//    }
//    string filePath = "D:/test/" + TransformStrTimeS(begin_time) + ".tdms";
//    void *fileHandle = nullptr;
//    open_file(filePath.c_str(), "w", fileHandle);
//    void *groupHandle = nullptr;
//    create_group(fileHandle, "data", groupHandle);
//    void *channelHandle = nullptr;
//    create_channel(groupHandle, "Time", channelHandle);
//    set_channel_data_timestamp(channelHandle, index, ts.data());
//    vector<const char *> cstrs;
//    for (size_t i = 0; i < table.size(); i++) {
////                if(chnList[i].empty()){
////                    create_channel(groupHandle, chnList[i].c_str(), channelHandle);
////                } else{
//        create_channel(groupHandle, varList[i].c_str(), channelHandle);
////                }
//        if (table[i].type) {
//            set_channel_data_double(channelHandle, index, table[i].dbl.data());
//        } else {
//            for (size_t j = 0; j < table[i].str.size(); j++)
//                cstrs[j] = table[i].str[j].c_str();
//            set_channel_data_string(channelHandle, cstrs.size(), cstrs.data());
//        }
//    }
//    save_file(fileHandle);
//    close_file(fileHandle);
//    chrono::system_clock::time_point end = chrono::system_clock::now();
//    cout << "time spend:" << chrono::duration_cast<chrono::microseconds>(end - begin).count() << endl;
//}

#endif //POSTMORTEMLOGGER_PMLTYPESUPPORT_H
