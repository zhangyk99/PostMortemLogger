//
// Created by 79933 on 2025/4/1.
//

#ifndef POSTMORTEMLOGGER_PMLOUTPUT_H
#define POSTMORTEMLOGGER_PMLOUTPUT_H

#include <utility>
#include <vector>
#include <map>
#include <variant>

#include "TDMS/TDMS_master.h"
#include "PMLTypes.h"

class Output {
public:
    explicit Output(const vector<chrono::system_clock::time_point> &preTimestamp, const vector<chrono::system_clock::time_point> &postTimestamp, const vector<TargetData> &preData,
                    const vector<TargetData> &postData, const vector<string> &varNameList, string warningDes)
            : preTimestamp(preTimestamp), postTimestamp(postTimestamp), preData(preData), postData(postData), varNameList(varNameList), warningDes(std::move(warningDes)) {}

    static string two_digit(int num) {
        return (num < 10 ? "0" : "") + std::to_string(num);
    }

    void writeTDMS() {
//        for(auto &str : varNameList)
//            cout << str << " ";
//        cout << endl;
//        cout << "preData size: ";
//        for(auto &chn : preData) {
//            if(chn.vct_dbl.empty())
//                cout << chn.vct_str.size() << ' ';
//            else
//                cout << chn.vct_dbl.size() << ' ';
//        }
//        cout << "postData size: ";
//        for(auto &chn : postData) {
//            if(chn.vct_dbl.empty())
//                cout << chn.vct_str.size() << ' ';
//            else
//                cout << chn.vct_dbl.size() << ' ';
//        }
        chrono::system_clock::time_point now = postTimestamp.front();
        time_t tp = chrono::system_clock::to_time_t(now);
        tm nowPoint;
        localtime_s(&nowPoint, &tp);
        void *fileHandle = nullptr;
        string name = "PML_" + to_string(nowPoint.tm_year + 1900) + two_digit(nowPoint.tm_mon + 1) + two_digit(nowPoint.tm_mday) + '_' +
        two_digit(nowPoint.tm_hour) + two_digit(nowPoint.tm_min) + two_digit(nowPoint.tm_sec);
        cout << name << endl;
        string filePath = "D:/test/" + name + ".tdms";
        open_file(filePath.c_str(), "w", fileHandle);
        create_file_property_string(fileHandle, "name", name.c_str());
        create_file_property_string(fileHandle, "Description", warningDes.c_str());
        void *groupPreHandle = nullptr;

        create_group(fileHandle, "Pre-shutdown", groupPreHandle);

        void *channelPreHandle = nullptr;

        vector<const char *> cstrs;
        create_channel(groupPreHandle, "Time", channelPreHandle);

        vector<timestamp> timestampList{};
        for (auto &ts: preTimestamp) {
            time_t time = chrono::system_clock::to_time_t(ts);
            tm timePoint = *localtime(&time);
            auto sinceEpoch = ts.time_since_epoch();
            auto micro = chrono::duration_cast<chrono::microseconds>(sinceEpoch) % chrono::seconds(1);
            timestamp t{timePoint.tm_year + 1900, timePoint.tm_mon + 1, timePoint.tm_mday, timePoint.tm_hour, timePoint.tm_min, timePoint.tm_sec, static_cast<int32_t>(micro.count())};
            timestampList.push_back(t);
        }
        set_channel_data_timestamp(channelPreHandle, timestampList.size(), timestampList.data());

        for (size_t i = 0; i < varNameList.size(); i++) {
            create_channel(groupPreHandle, varNameList[i].c_str(), channelPreHandle);

            create_channel_property_string(channelPreHandle, "Description", "");
            create_channel_property_string(channelPreHandle, "unit_string", "");
//            create_channel_property_int32(channelPreHandle, "NI_ArrayColumn", static_cast<int32_t>(i));

            if (preData[i].vec_dbl.empty()) {
                cstrs.clear();
                for (const auto &str: preData[i].vec_str)
                    cstrs.push_back(str.c_str());
                set_channel_data_string(channelPreHandle, cstrs.size(), cstrs.data());
            } else {
                set_channel_data_double(channelPreHandle, preData[i].vec_dbl.size(), preData[i].vec_dbl.data());
            }
        }
        save_file(fileHandle);

        void *groupPostHandle = nullptr;
        create_group(fileHandle, "Post-shutdown", groupPostHandle);
        void *channelPostHandle = nullptr;
        create_channel(groupPostHandle, "Time", channelPostHandle);
        timestampList.clear();
        for (auto &ts: postTimestamp) {
            time_t time = chrono::system_clock::to_time_t(ts);
            tm timePoint = *localtime(&time);
            auto sinceEpoch = ts.time_since_epoch();
            auto mirco = chrono::duration_cast<chrono::microseconds>(sinceEpoch) % chrono::seconds(1);
            timestamp t{timePoint.tm_year + 1900, timePoint.tm_mon + 1, timePoint.tm_mday, timePoint.tm_hour, timePoint.tm_min, timePoint.tm_sec, static_cast<int32_t>(mirco.count())};
            timestampList.push_back(t);
        }
        set_channel_data_timestamp(channelPostHandle, timestampList.size(), timestampList.data());
        for (size_t i = 0; i < varNameList.size(); i++) {
            create_channel(groupPostHandle, varNameList[i].c_str(), channelPostHandle);
            create_channel_property_string(channelPostHandle, "Description", "");
            create_channel_property_string(channelPostHandle, "unit_string", "");
//            create_channel_property_int32(channelPostHandle, "NI_ArrayColumn", static_cast<int32_t>(i));
            if (postData[i].vec_dbl.empty()) {
                cstrs.clear();
                for (const auto &str: postData[i].vec_str)
                    cstrs.push_back(str.c_str());
                set_channel_data_string(channelPostHandle, cstrs.size(), cstrs.data());
            } else {
                set_channel_data_double(channelPostHandle, postData[i].vec_dbl.size(), postData[i].vec_dbl.data());
            }
        }
        save_file(fileHandle);
        close_file(fileHandle);
        cout << "finish write" << endl;
    }

    void writeCSV() {}

    void writeMDF() {}

private:
    vector<chrono::system_clock::time_point> preTimestamp;
    vector<chrono::system_clock::time_point> postTimestamp;
    vector<TargetData> preData;
    vector<TargetData> postData;
    vector<string> varNameList;
    string warningDes;
};


#endif //POSTMORTEMLOGGER_PMLOUTPUT_H
