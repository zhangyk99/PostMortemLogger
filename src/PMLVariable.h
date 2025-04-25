//
// Created by 79933 on 2025/4/2.
//

#ifndef POSTMORTEMLOGGER_PMLVARIABLE_H
#define POSTMORTEMLOGGER_PMLVARIABLE_H

#include "PMLTypes.h"
#include "PMLOutput.h"
#include "varmanager/varmanager.h"

#include <utility>
#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <chrono>

using namespace std;

class Variable {
public:
    static chrono::system_clock::time_point TransformTime(lvtime *time) {
        if (time == nullptr) return std::chrono::system_clock::now();
        return std::chrono::system_clock::time_point(std::chrono::seconds(time->time_s - 2082844800) +
                                                     std::chrono::microseconds((uint64_t) ((double) time->time_us / 18446744073709.551615)));
    }

    static void callback(event_data_t eventData, void *p) {
        auto ptr = static_cast<Variable *>(p);
        for (size_t i = 0; i < ptr->varNameList.size(); i++) {
            if (ptr->varNameList[i] == eventData.name) {
                Data d;
                if (eventData.type == vartype_dbl)
                    d.dbl = eventData.value_dbl;
                if (eventData.type == vartype_str)
                    d.str = eventData.value_str;
                d.timePoint = TransformTime(&eventData.time);
                ptr->primaryData[i].push_back(d);
                if (ptr->primaryData[i].size() > 2) {
                    while (ptr->primaryData[i][1].timePoint + chrono::seconds(ptr->maxTime + 3) < ptr->primaryData[i].back().timePoint) {
                        ptr->primaryData[i].pop_front();
                    }
                }
            }
        }
    }

    static void callback_InShutdown(event_data_t eventData, void *p) {
        auto ptr = static_cast<Variable *>(p);
        if (eventData.value_dbl > 0) {
            ptr->stopTime = TransformTime(&eventData.time);
            ptr->stop = true;
        } else
            ptr->stop = false;
    }

    explicit Variable(void *con, const vector<string> &varNameList, const uint16_t preTime, const uint16_t postTime, const uint16_t frequency) :
            con(con), varNameList(varNameList), preTime(preTime), postTime(postTime), frequency(frequency) {
        for (auto &vName: varNameList) {
            vartype type;
            con_get_value_type(con, vName.c_str(), &type);
            if (type == vartype_dbl) {
                double dbl;
                con_get_value_dbl(con, vName.c_str(), &dbl);
                Data d{dbl};
                beginDataList.push_back(d);
            }
            if (type == vartype_str) {
                std::vector<char> str(256);
                con_get_value_str(con, vName.c_str(), str.data());
                string s(str.begin(), str.end());
                Data d{s};
                beginDataList.push_back(d);
            }
        }
        int32_t id = 0;
        maxTime = max(preTime, postTime);
        primaryData.resize(varNameList.size());
        for (const auto &i: varNameList) {
            con_add_callback(con, i.c_str(), 0, &id, callback, this);
            idList.push_back(id);
        }
        con_add_callback(con, "InShutdown", 0, &id_shutdown, callback_InShutdown, this);
    }

    int32_t RecordeData() const {
        thread([this]() {
            cout << "thread begin" << endl;

            vector<deque<Data>> prePrimaryData;
            vector<deque<Data>> postPrimaryData;
            vector<TargetData> preData(primaryData.size());
            vector<TargetData> postData(primaryData.size());
            vector<chrono::system_clock::time_point> preTimestamp;
            vector<chrono::system_clock::time_point> postTimestamp;
            if (stop) {
                lock_guard<mutex> lock(mtx);
                prePrimaryData.assign(primaryData.begin(), primaryData.end());
            }
            for (size_t i = 0; i < frequency * postTime; i++) {
                preTimestamp.push_back(stopTime - chrono::seconds(preTime) + chrono::microseconds((1000000 / frequency) * i));
            }
            for (size_t i = 0; i < frequency * preTime; i++) {
                postTimestamp.push_back(stopTime + chrono::microseconds((1000000 / frequency) * i));
            }
            {
                this_thread::sleep_for(chrono::seconds(postTime));
                lock_guard<mutex> lock(mtx);
                postPrimaryData.assign(primaryData.begin(), primaryData.end());
            }
            chrono::system_clock::time_point begin = chrono::system_clock::now();
            for (size_t i = 0; i < prePrimaryData.size(); i++) {
                prePrimaryData[i].push_front(beginDataList[i]);
                for (auto &t: preTimestamp) {
                    while (prePrimaryData[i].front().timePoint <= t) {
                        if(prePrimaryData[i].size() > 1) {
                            if (prePrimaryData[i][1].timePoint > t)
                                break;
                        } else
                            break;
                        prePrimaryData[i].pop_front();
                        if(prePrimaryData[i].empty())
                            break;
                    }
//                    if (d.empty()) {
//                        if (postPrimaryData[i].front().str.empty()) {
//                            preData[i].vec_dbl.push_back(postPrimaryData[i].front().dbl);
//                        } else {
//                            preData[i].vec_str.push_back(postPrimaryData[i].front().str);
//                        }
//                        continue;
//                    }
                    if (prePrimaryData[i].front().str.empty())
                        preData[i].vec_dbl.push_back(prePrimaryData[i].front().dbl);
                    else
                        preData[i].vec_str.push_back(prePrimaryData[i].front().str);
                }
            }
            for (size_t i = 0; i < postPrimaryData.size(); i++) {
                postPrimaryData[i].push_front(beginDataList[i]);
                for (auto &t: postTimestamp) {
                    while (postPrimaryData[i].front().timePoint <= t) {
                        if(postPrimaryData[i].size() > 1) {
                            if (postPrimaryData[i][1].timePoint > t)
                                break;
                        } else
                            break;
                        postPrimaryData[i].pop_front();
                        if(postPrimaryData[i].empty())
                            break;
                    }
//                    if (d.empty()) {
//                        if (postPrimaryData[i].front().str.empty()) {
//                            postData[i].vec_dbl.push_back(postPrimaryData[i].front().dbl);
//                        } else {
//                            postData[i].vec_str.push_back(postPrimaryData[i].front().str);
//                        }
//                        continue;
//                    }
                    if (postPrimaryData[i].front().str.empty())
                        postData[i].vec_dbl.push_back(postPrimaryData[i].front().dbl);
                    else
                        postData[i].vec_str.push_back(postPrimaryData[i].front().str);
                }
            }
            cout << "preData: " << endl;
            for (auto &chn: preData)
                cout << chn.vec_dbl.size() << ' ' << chn.vec_str.size() << endl;
            cout << "postData: " << endl;
            for (auto &chn: postData)
                cout << chn.vec_dbl.size() << ' ' << chn.vec_str.size() << endl;
            string warning = "PML TDMS Test";
            chrono::system_clock::time_point process = chrono::system_clock::now();
            auto ptr = make_shared<Output>(preTimestamp, postTimestamp, preData, postData, varNameList, warning);
            ptr->writeTDMS();
            chrono::system_clock::time_point write = chrono::system_clock::now();
            cout << "process: " << chrono::duration_cast<chrono::microseconds>(process - begin).count() << endl;
            cout << "write: " << chrono::duration_cast<chrono::microseconds>(write - process).count() << endl;
        }).detach();
        return 0;
    }

private:
    void *con{};
    vector<string> varNameList{};
    uint16_t preTime;
    uint16_t postTime;
    uint16_t maxTime;
    uint16_t frequency;
    vector<int32_t> idList{};
    int32_t id_shutdown;
    chrono::system_clock::time_point stopTime;
    bool stop = false;
    vector<Data> beginDataList;
    vector<deque<Data>> primaryData;
    mutable mutex mtx;
};

#endif //POSTMORTEMLOGGER_PMLVARIABLE_H