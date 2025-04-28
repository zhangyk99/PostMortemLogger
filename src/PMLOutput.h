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
        void *fileHandle = nullptr;
        time_t stopTime = chrono::system_clock::to_time_t(postTimestamp.front());
        tm t;
        localtime_s(&t, &stopTime);
        string fileName = "PML_" + to_string(t.tm_year + 1900) + two_digit(t.tm_mon + 1) + two_digit(t.tm_mday) + '_' + two_digit(t.tm_hour) + two_digit(t.tm_min) + two_digit(t.tm_sec);
        open_file((fileName + ".tdms").c_str(), "w", fileHandle);
        create_file_property_string(fileHandle, "name", fileName.c_str());
        create_file_property_string(fileHandle, "Description", warningDes.c_str());

        void *groupPreHandle = nullptr;
        void *groupPostHandle = nullptr;
        create_group(fileHandle, "Pre_shutdown", groupPreHandle);
        create_group(fileHandle, "Post_shutdown", groupPostHandle);

        void *channelPreTimeHandle = nullptr;
        void *channelPostTimeHandle = nullptr;
        vector<void *> channelPreHandles(varNameList.size(), nullptr);
        vector<void *> channelPostHandles(varNameList.size(), nullptr);
        vector<timestamp> pre_timestamp{};
        vector<timestamp> post_timestamp{};
        for (auto &ts: preTimestamp) {
            time_t tp = chrono::system_clock::to_time_t(ts);
            tm pre_t;
            localtime_s(&pre_t, &tp);
            pre_timestamp.push_back({pre_t.tm_year + 1900, pre_t.tm_mon + 1, pre_t.tm_mday, pre_t.tm_hour, pre_t.tm_min, pre_t.tm_sec,
                                     static_cast<int32_t>((chrono::duration_cast<chrono::microseconds>(ts.time_since_epoch()) % chrono::seconds(1)).count())});
        }
        for (auto &ts: postTimestamp) {
            time_t tp = chrono::system_clock::to_time_t(ts);
            tm post_t;
            localtime_s(&post_t, &tp);
            post_timestamp.push_back({post_t.tm_year + 1900, post_t.tm_mon + 1, post_t.tm_mday, post_t.tm_hour, post_t.tm_min, post_t.tm_sec,
                                      static_cast<int32_t>((chrono::duration_cast<chrono::microseconds>(ts.time_since_epoch()) % chrono::seconds(1)).count())});
        }

        if (varNameList.size() <= 1000) {
            if (pre_timestamp.size() <= 10000) {
                create_channel(groupPreHandle, "Time", channelPreTimeHandle);
                set_channel_data_timestamp(channelPreTimeHandle, pre_timestamp.size(), pre_timestamp.data());
                for (size_t i = 0; i < varNameList.size(); i++) {
                    create_channel(groupPreHandle, varNameList[i].c_str(), channelPreHandles[i]);
                    if (preData[i].vec_str.empty()) {
                        set_channel_data_double(channelPreHandles[i], preData[i].vec_dbl.size(), preData[i].vec_dbl.data());
                    } else {
                        set_channel_data_string(channelPreHandles[i], postData[i].vec_str.size(), preData[i].vec_str.data());
                    }
                }
                save_file(fileHandle);
            } else {
                create_channel(groupPreHandle, "Time", channelPreTimeHandle);
                for (size_t i = 0; i < varNameList.size(); i++)
                    create_channel(groupPreHandle, varNameList[i].c_str(), channelPreHandles[i]);
                size_t data_size = pre_timestamp.size();
                size_t j = 0;
                while (data_size) {
                    if (data_size >= 10000) {
                        set_channel_data_timestamp(channelPreTimeHandle, 10000, &pre_timestamp[10000 * j]);
                        for (size_t i = 0; i < varNameList.size(); i++) {
                            if (preData[i].vec_str.empty()) {
                                set_channel_data_double(channelPreHandles[i], 10000, &preData[i].vec_dbl[10000 * j]);
                            } else {
                                set_channel_data_string(channelPreHandles[i], 10000, &preData[i].vec_str[10000 * j]);
                            }
                        }
                        j++;
                        data_size -= 10000;
                        save_file(fileHandle);
                    } else {
                        set_channel_data_timestamp(channelPostTimeHandle, data_size, &pre_timestamp[10000 * j]);
                        for (size_t i = 0; i < varNameList.size(); i++) {
                            if (preData[i].vec_str.empty()) {
                                set_channel_data_double(channelPreHandles[i], data_size, &preData[i].vec_dbl[10000 * j]);
                            } else {
                                set_channel_data_string(channelPreHandles[i], data_size, &preData[i].vec_str[10000 * j]);
                            }
                        }
                        data_size = 0;
                        save_file(fileHandle);
                    }
                }
            }
            if (post_timestamp.size() <= 10000) {
                create_channel(groupPostHandle, "Time", channelPostTimeHandle);
                set_channel_data_timestamp(channelPostTimeHandle, post_timestamp.size(), post_timestamp.data());
                for (size_t i = 0; i < varNameList.size(); i++) {
                    create_channel(groupPostHandle, varNameList[i].c_str(), channelPostHandles[i]);
                    if (postData[i].vec_str.empty()) {
                        set_channel_data_double(channelPostHandles[i], postData[i].vec_dbl.size(), postData[i].vec_dbl.data());
                    } else {
                        set_channel_data_string(channelPostHandles[i], postData[i].vec_str.size(), postData[i].vec_str.data());
                    }
                }
                save_file(fileHandle);
            } else {
                create_channel(groupPostHandle, "Time", channelPostTimeHandle);
                for (size_t i = 0; i < varNameList.size(); i++)
                    create_channel(groupPostHandle, varNameList[i].c_str(), channelPostHandles[i]);
                size_t data_size = post_timestamp.size();
                size_t j = 0;
                while (data_size) {
                    if (data_size >= 10000) {
                        for (size_t i = 0; i < varNameList.size(); i++) {
                            set_channel_data_timestamp(channelPostTimeHandle, 10000, &post_timestamp[10000 * j]);
                            if (postData[i].vec_dbl.empty()) {
                                set_channel_data_double(channelPostHandles[i], 10000, &postData[i].vec_dbl[10000 * j]);
                            } else {
                                set_channel_data_string(channelPostHandles[i], 10000, &postData[i].vec_str[10000 * j]);
                            }
                        }
                        data_size -= 10000;
                        j++;
                        save_file(fileHandle);
                    } else {
                        for (size_t i = 0; i < varNameList.size(); i++) {
                            set_channel_data_timestamp(channelPostTimeHandle, data_size, &post_timestamp[10000 * j]);
                            if (postData[i].vec_str.empty()) {
                                set_channel_data_double(channelPostHandles[i], data_size, &postData[i].vec_dbl[10000 * j]);
                            } else {
                                set_channel_data_string(channelPostHandles[i], data_size, &postData[i].vec_str[10000 * j]);
                            }
                        }
                        data_size = 0;
                        save_file(fileHandle);
                    }
                }
            }
        } else {
            create_channel(groupPreHandle, "Time", channelPreTimeHandle);
            size_t name_size = varNameList.size();
            size_t j = 0;
            if (pre_timestamp.size() <= 10000) {
                set_channel_data_timestamp(channelPreTimeHandle, pre_timestamp.size(), pre_timestamp.data());
                while (name_size) {
                    if (name_size <= 1000) {
                        for (size_t i = 0; i < 1000; i++) {
                            create_channel(groupPreHandle, varNameList[i + j * 1000].c_str(), channelPreHandles[i + j * 1000]);
                            if (preData[i + j * 1000].vec_str.empty()) {
                                set_channel_data_double(channelPreHandles[i + j * 1000], preData[i + j * 1000].vec_dbl.size(), preData[i + j * 1000].vec_dbl.data());
                            } else {
                                set_channel_data_string(channelPreHandles[i + j * 1000], preData[i + j * 1000].vec_str.size(), preData[i + j * 1000].vec_str.data());
                            }
                        }
                        name_size -= 1000;
                        j++;
                        save_file(fileHandle);
                    } else {
                        for (size_t i = 0; i < name_size; i++) {
                            create_channel(groupPostHandle, varNameList[i + j * 1000].c_str(), channelPreHandles[i + j * 1000]);
                            if (preData[i + j * 1000].vec_str.empty()) {
                                set_channel_data_double(channelPreHandles[i + j * 1000], preData[i + j * 1000].vec_dbl.size(), preData[i + j * 1000].vec_dbl.data());
                            } else {
                                set_channel_data_string(channelPreHandles[i + j * 1000], preData[i + j * 1000].vec_str.size(), preData[i + j * 1000].vec_str.data());
                            }
                        }
                        name_size = 0;
                        save_file(fileHandle);
                    }
                }
            } else {

            }

            create_channel(groupPostHandle, "Time", channelPostTimeHandle);
            name_size = varNameList.size();
            j = 0;
            if (post_timestamp.size() <= 10000) {
                set_channel_data_timestamp(channelPostTimeHandle, post_timestamp.size(), post_timestamp.data());
                while (name_size) {
                    if (name_size <= 1000) {
                        for (size_t i = 0; i < 1000; i++) {
                            create_channel(groupPostHandle, varNameList[i + j * 1000].c_str(), channelPostHandles[i + j * 1000]);
                            if (postData[i + j * 1000].vec_str.empty()) {
                                set_channel_data_double(channelPostHandles[i + j * 1000], postData[i + j * 1000].vec_dbl.size(), postData[i + j * 1000].vec_dbl.data());
                            } else {
                                set_channel_data_string(channelPostHandles[i + j * 1000], postData[i + j * 1000].vec_str.size(), postData[i + j * 1000].vec_str.data());
                            }
                        }
                        name_size -= 1000;
                        j++;
                        save_file(fileHandle);
                    } else {
                        for(size_t i = 0; i < name_size; i++){
                            create_channel(groupPostHandle)
                        }
                    }
                }
            } else {

            }
        }
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
