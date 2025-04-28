//
// Created by 79933 on 2025/3/27.
//

#include <chrono>
#include <deque>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>
#include <future>

#include "varmanager/varmanager.h"
#include "gtest/gtest.h"
//#include "PMLDevice.h"
#include "PMLVariable.h"

using namespace std;

TEST(varTest, test) {
    void *con;
    create_container(&con);
    string name = "var0";
    double get_var = 0;
    con_set_value_dbl(con, name.c_str(), 3.1415, nullptr, 0, 0);
    con_get_value_dbl(con, name.c_str(), &get_var);
    cout << get_var << endl;
    con_set_value_dbl(con, name.c_str(), 5, nullptr, 0, 0);
    con_get_value_dbl(con, name.c_str(), &get_var);
    cout << get_var << endl;
    free_container(con);
    auto now = chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto seconds = chrono::duration_cast<chrono::seconds>(duration);
    auto microseconds = chrono::duration_cast<chrono::microseconds>(duration - seconds);
    cout << seconds.count() << endl;
    cout << microseconds.count() << endl;
}

TEST(timeTransform, test){
    lvtime *time;
    auto timePoint = chrono::system_clock::now();
    auto timePtr = chrono::system_clock::time_point(chrono::seconds(time->time_s - 2082844800) + chrono::microseconds((uint64_t)((double)time->time_us/18446744073709.551615)));
}

chrono::system_clock::time_point time_transform(lvtime *time){
    if(time == nullptr)
        return chrono::system_clock::now();
    return chrono::system_clock::time_point(chrono::seconds(time->time_s - 2082844800) + chrono::microseconds((uint64_t)((double)time->time_us/18446744073709.551615)));
}

TEST(learn_chrono, test){
    //chrono::system_clock
    //获取当前时间点
    chrono::system_clock::time_point now = chrono::system_clock::now();

    //获取具体时间
    time_t time = chrono::system_clock::to_time_t(now);
    tm *pTime = localtime(&time);
    cout << pTime->tm_year + 1900 << endl;
    cout << pTime->tm_mon + 1 << endl;
    cout << pTime->tm_mday << endl;
    cout << pTime->tm_hour << endl;
    cout << pTime->tm_min << endl;
    cout << pTime->tm_sec << endl;
}


void change_con(void *con){
    for(size_t i = 1; i <= 100000; i++){
        for(size_t j = 0; j < 500; j++) {
            con_set_value_dbl(con, ("var_dbl" + to_string(j)).c_str(), 3.14 + 0.01 * i, nullptr, nullptr, 0);
            con_set_value_str(con, ("var_str" + to_string(j)).c_str(), to_string(123 + i).c_str(), nullptr, nullptr, 0);
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

void shutdown(void *con){
    this_thread::sleep_for(chrono::seconds(10));
    con_set_value_dbl(con, "InShutdown", 1, nullptr, nullptr, 0);
    this_thread::sleep_for(chrono::seconds(2));
    con_set_value_dbl(con, "InShutdown", 0, nullptr, nullptr, 0);
    this_thread::sleep_for(chrono::seconds(1));
    con_set_value_dbl(con, "InShutdown", 1, nullptr, nullptr, 0);
}

void startRecorde(shared_ptr<Variable> ptr){
    this_thread::sleep_for(chrono::seconds(10));
    ptr->RecordeData();
    this_thread::sleep_for(chrono::seconds(3));
    ptr->RecordeData();
}

TEST(PMLTest, test){
    void *con = nullptr;
    auto begin = chrono::system_clock::now();
    vector<string> varNameList;
    for(size_t i = 0; i < 500; i++){
        varNameList.emplace_back(("var_dbl" + to_string(i)).c_str());
    }
    for(size_t i = 0; i < 500; i++){
        varNameList.emplace_back(("var_str" + to_string(i)).c_str());
    }
    create_container(&con);
    for(size_t i = 0; i < 500; i++){
        con_set_value_dbl(con, ("var_dbl" + to_string(i)).c_str(), 3.14, nullptr, nullptr, 0);
        con_set_value_str(con, ("var_str" + to_string(i)).c_str(), "123", nullptr, nullptr, 0);
    }
    con_set_value_dbl(con, "InShutdown", 0, nullptr, nullptr, 0);
    auto ptr = make_shared<Variable>(con, varNameList, 10, 10, 1000);
    thread t0(change_con, con);
    thread t1(shutdown, con);
    thread t2(startRecorde, ptr);
    t0.join();
    t1.join();
    t2.join();
    auto end = chrono::system_clock::now();
    auto period = chrono::duration_cast<chrono::milliseconds>(end - begin);
    cout << period.count();
}

TEST(setTimestamp, test){
    uint16_t preTime = 5;
    uint16_t postTime = 5;
    uint16_t frequency = 200;
    chrono::system_clock::time_point stopTime = chrono::system_clock::now();
    vector<chrono::system_clock::time_point> preTimestamp;
    vector<chrono::system_clock::time_point> postTimestamp;
    for(size_t i = 0; i < frequency*postTime; i++){
        preTimestamp.push_back(stopTime - chrono::seconds(preTime) + chrono::microseconds((1000000/frequency)*i));
    }
    for(size_t i = 0; i < frequency*preTime; i++){
        postTimestamp.push_back(stopTime + chrono::microseconds((1000000/frequency)*i));
    }
    chrono::system_clock::time_point end = chrono::system_clock::now();
    auto timePeriod = chrono::duration_cast<chrono::microseconds>(end - stopTime);
    cout << preTimestamp.size() << endl;
    cout << postTimestamp.size() << endl;
    cout << timePeriod.count() << endl;
}

TEST(TDMS, test){
    vector<string> varNameList{"var_dbl", "var_string"};
    vector<chrono::system_clock::time_point> preTimestamp;
    vector<chrono::system_clock::time_point> postTimestamp;
    vector<TargetData> preData(varNameList.size());
    vector<TargetData> postData(varNameList.size());
    chrono::system_clock::time_point stopTime = chrono::system_clock::now();
    uint16_t preTime = 5;
    uint16_t postTime = 5;
    uint16_t frequency = 1000;
    for(size_t i = 0; i < frequency*preTime; i++)
        preTimestamp.push_back(stopTime - chrono::seconds(preTime) + chrono::microseconds(1000000/frequency*i));
    for(size_t i = 0; i < frequency*postTime; i++)
        postTimestamp.push_back(stopTime + chrono::microseconds(1000000/frequency*i));
    for(size_t i = 0; i < preTimestamp.size(); i++){
        preData[0].vec_dbl.push_back(3.14 + 0.01*i);
        preData[1].vec_str.push_back(to_string(123 + i));
        postData[0].vec_dbl.push_back(3.14 + 0.01*i);
        postData[1].vec_str.push_back(to_string(123 + i));
    }
    string str = "PML TDMS Test";
    Output tdms(preTimestamp, postTimestamp, preData, postData, varNameList, str);
    tdms.writeTDMS();
}

TEST(time, test){
    chrono::system_clock::time_point now = chrono::system_clock::now();
    time_t now_point = chrono::system_clock::to_time_t(now);

    tm localtime;
    localtime_s(&localtime, &now_point);
    cout << "year: " << localtime.tm_year + 1900 << endl;
    cout << "month: " << localtime.tm_mon + 1 << endl;
    cout << "day: " << localtime.tm_mday << endl;
    cout << localtime.tm_hour << ":" << localtime.tm_min << ":" << localtime.tm_sec;
}

TEST(DataDefault, test){
    Data d{};
    cout << d.dbl << endl;
    cout << d.str << endl;
    time_t time = chrono::system_clock::to_time_t(d.timePoint);
    tm local;
    localtime_s(&local, &time);
    cout << local.tm_year << '-' << local.tm_mon << '-' << local.tm_mday << ' ' << local.tm_hour << ':' << local.tm_min << ':' << local.tm_sec << endl;
    cout << d.timePoint.time_since_epoch().count();
}

TEST(DataDBL, test){
    double dbl = 3.14;
    Data d(dbl);
    cout << d.dbl << endl;
    cout << d.str << endl;
    time_t time = chrono::system_clock::to_time_t(d.timePoint);
    tm local;
    localtime_s(&local, &time);
    cout << local.tm_year + 1900 << '-' << local.tm_mon + 1 << '-' << local.tm_mday << ' ' << local.tm_hour << ':' << local.tm_min << ':' << local.tm_sec << endl;
}

TEST(thread, test) {
    chrono::system_clock::time_point begin = chrono::system_clock::now();
    for(size_t i = 0; i < 200; i++)
        this_thread::sleep_for(chrono::milliseconds(1));
    chrono::system_clock::time_point end = chrono::system_clock::now();
    auto period = chrono::duration_cast<chrono::microseconds>(end - begin);
    cout << period.count();
}

TEST(vec_Data_size, test){
    deque<Data> data;
    cout << data.max_size() << endl;
    vector<deque<Data>> deque;
    cout << deque.max_size() << endl;
}

TEST(outputTDMS, test){
    vector<string> varNameList{};
    for(size_t i = 0; i < 10; i++){
        varNameList.emplace_back("var_dbl" + to_string(i));
        varNameList.emplace_back("var_str" + to_string(i));
    }
    vector<chrono::system_clock::time_point> preTimestamp;
    vector<chrono::system_clock::time_point> postTimestamp;
    vector<TargetData> preData(varNameList.size());
    vector<TargetData> postData(varNameList.size());
    chrono::system_clock::time_point stopTime = chrono::system_clock::now();
    uint16_t preTime = 100;
    uint16_t postTime = 100;
    uint16_t frequency = 1000;
    for(size_t i = 0; i < frequency*preTime; i++)
        preTimestamp.push_back(stopTime - chrono::seconds(preTime) + chrono::microseconds(1000000/frequency*i));
    for(size_t i = 0; i < frequency*postTime; i++)
        postTimestamp.push_back(stopTime + chrono::microseconds(1000000/frequency*i));
    for(size_t i = 0; i < preTimestamp.size(); i++){
        for(size_t j = 0; j < 10; j++) {
            preData[j].vec_dbl.push_back(3.14 + 0.01*i);
            postData[j].vec_str.push_back(to_string(123 + i));
        }
    }
    string str = "PML TDMS Test";
    chrono::system_clock::time_point begin = chrono::system_clock::now();
    Output tdms(preTimestamp, postTimestamp, preData, postData, varNameList, str);
    tdms.writeTDMS();
    chrono::system_clock::time_point end = chrono::system_clock::now();
    cout << "spent time: " << chrono::duration_cast<chrono::microseconds>(end - begin).count();
}

TEST(createTimestamp, test){
    vector<chrono::system_clock::time_point> preTimestamp{};
    vector<chrono::system_clock::time_point> postTimestamp{};
    chrono::system_clock::time_point stop = chrono::system_clock::now();
    for(size_t i = 0; i < 100000; i++)
        preTimestamp.push_back(stop - chrono::seconds(10) + chrono::milliseconds(i));
    for(size_t i = 0; i < 100000; i++)
        postTimestamp.push_back(stop + chrono::milliseconds(i));
    chrono::system_clock::time_point end = chrono::system_clock::now();
    cout << chrono::duration_cast<chrono::microseconds>(end - stop).count() << endl;
    cout << preTimestamp.size() << endl;
    cout << postTimestamp.size() << endl;
}