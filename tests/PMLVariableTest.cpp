//
// Created by 79933 on 2025/8/29.
//

#include <iostream>

#ifdef _WIN32

#include <windows.h>

#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#endif

#include <cstring>
#include <string>
#include <chrono>

#include "gtest/gtest.h"
#include "PMLDevice.h"

TEST(Demo, Basic) {
    EXPECT_EQ(1, 1);
}

TEST(device, test) {
    void *con;
    create_container(&con);
    con_set_value_dbl(con, "Var_0", 0, nullptr, nullptr, 0);
    con_set_value_dbl(con, "Var_1", 1, nullptr, nullptr, 1);
    con_set_value_dbl(con, "Var_2", 2, nullptr, nullptr, 2);
    con_set_value_dbl(con, "Var_3", 3, nullptr, nullptr, 3);

    con_set_value_str(con, "Var_4", "abc", nullptr, nullptr, 0);
    con_set_value_str(con, "Var_5", "def", nullptr, nullptr, 0);

    con_set_value_dbl(con, "InShutdown", 0, nullptr, nullptr, 0);

    std::string deviceConfig = {"{\n"
                                "  \"DeviceInfo\": {\n"
                                "    \"DeviceName\": \"PostMortemLogger\",\n"
                                "    \"DeviceID\": 0,\n"
                                "    \"DeviceType\": \"PostMortemLogger\",\n"
                                "    \"DeviceDes\": \"\",\n"
                                "    \"Version\": 0,\n"
                                "    \"Author\": \"\",\n"
                                "    \"CreateTime\": \"2025-03-26 10:54:08.353\",\n"
                                "    \"Reviser\": \"\",\n"
                                "    \"ModifyTime\": \"2025-11-24 14:40:56.642\"\n"
                                "  },\n"
                                "  \"DeviceConfig\": {\n"
                                "    \"PreStopTime\": 10.0,\n"
                                "    \"PostStopTime\": 10.0,\n"
                                "    \"Frequence\": 100.0,\n"
                                "    \"FileType\": 0,\n"
                                "    \"Channels\": [\n"
                                "      {\n"
                                "        \"StatisticsType\": null,\n"
                                "        \"ChannelName\": \"channel0\",\n"
                                "        \"VarName\": \"Var_0\",\n"
                                "        \"VarType\": \"DBL\",\n"
                                "        \"VarUnit\": null,\n"
                                "        \"ChannelType\": \"Channel\",\n"
                                "        \"ChannelDes\": null\n"
                                "      },\n"
                                "      {\n"
                                "        \"StatisticsType\": null,\n"
                                "        \"ChannelName\": \"channel1\",\n"
                                "        \"VarName\": \"Var_1\",\n"
                                "        \"VarType\": \"DBL\",\n"
                                "        \"VarUnit\": null,\n"
                                "        \"ChannelType\": \"Channel\",\n"
                                "        \"ChannelDes\": null\n"
                                "      },\n"
                                "      {\n"
                                "        \"StatisticsType\": null,\n"
                                "        \"ChannelName\": \"channel2\",\n"
                                "        \"VarName\": \"Var_2\",\n"
                                "        \"VarType\": \"DBL\",\n"
                                "        \"VarUnit\": null,\n"
                                "        \"ChannelType\": \"Channel\",\n"
                                "        \"ChannelDes\": null\n"
                                "      },\n"
                                "      {\n"
                                "        \"StatisticsType\": null,\n"
                                "        \"ChannelName\": \"channel3\",\n"
                                "        \"VarName\": \"Var_3\",\n"
                                "        \"VarType\": \"DBL\",\n"
                                "        \"VarUnit\": null,\n"
                                "        \"ChannelType\": \"Channel\",\n"
                                "        \"ChannelDes\": null\n"
                                "      },\n"
                                "      {\n"
                                "        \"StatisticsType\": null,\n"
                                "        \"ChannelName\": \"channel4\",\n"
                                "        \"VarName\": \"Var_4\",\n"
                                "        \"VarType\": \"STR\",\n"
                                "        \"VarUnit\": null,\n"
                                "        \"ChannelType\": \"Channel\",\n"
                                "        \"ChannelDes\": null\n"
                                "      },\n"
                                "      {\n"
                                "        \"StatisticsType\": null,\n"
                                "        \"ChannelName\": \"channel5\",\n"
                                "        \"VarName\": \"Var_5\",\n"
                                "        \"VarType\": \"STR\",\n"
                                "        \"VarUnit\": null,\n"
                                "        \"ChannelType\": \"Channel\",\n"
                                "        \"ChannelDes\": null\n"
                                "      }\n"
                                "    ],\n"
                                "    \"FileSites\": []\n"
                                "  }\n"
                                "}"};

    auto ptr = std::make_shared<Device>("Device", deviceConfig, con);

//    std::thread t0([&]{
//        double value;
//        for(int i = 0; i < 1000; i++){
//            value = sin(1 + i*0.01);
//            con_set_value_dbl(con, "Var_0", 0 + i, nullptr, nullptr, 0);
//            con_set_value_dbl(con, "Var_1", value, nullptr, nullptr, 0);
//            con_set_value_dbl(con, "Var_2", 2 + i, nullptr, nullptr, 0);
//            con_set_value_dbl(con, "Var_3", 3 + i, nullptr, nullptr, 0);
////            con_set_value_str(con, "Var_4", std::to_string(1 + i).c_str(), nullptr, nullptr, 0);
////            con_set_value_str(con, "Var_5", std::to_string(1 + i).c_str(), nullptr, nullptr, 0);
//            std::this_thread::sleep_for(std::chrono::milliseconds(10));
//        }
//    });
//    std::thread t1([&]{
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        con_set_value_dbl(con, "InShutdown", 1, nullptr, nullptr, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(11000));
        con_set_value_dbl(con, "InShutdown", 0, nullptr, nullptr, 0);
//        std::chrono::system_clock::time_point begin;
//        bool isStop = false;
//        for(int i = 0; i < 5000; i++){
//            double value;
//            con_get_value_dbl(con, "Var_1", &value);
//            if (!isStop && value <= 0) {
//                con_set_value_dbl(con, "InShutdown", 1, nullptr, nullptr, 0);
//                begin = std::chrono::system_clock::now();
//                isStop = true;
//            }
//            auto end = std::chrono::system_clock::now();
//            auto spendTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
//            if(isStop && (spendTime - 1'000 > 0)) {
//                con_set_value_dbl(con, "InShutdown", 0, nullptr, nullptr, 0);
//                isStop = false;
//            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(10));
//        }
//    });
//    t0.join();
//    t1.join();
}

#include <vector>
#include <string>
#include <memory>

class B{
    std::vector<std::string> strs;
public:
    explicit B(std::vector<std::string> &strs) : strs(strs){}

    void display(){
        for(const auto & it : strs)
            std::cout << it << ' ';
    }
};

class A{
    std::vector<std::string> strs;
public:
    explicit A(){
        strs.push_back("hello");
        strs.push_back("world");
        auto ptr = std::make_shared<B>(strs);
        ptr->display();
    }
};

TEST(variable, test){
    A a;
//    void *con;
//    create_container(&con);
//    con_set_value_str(con, "Var_0", std::to_string(0).c_str(), nullptr, nullptr, 0);
//    con_set_value_str(con, "Var_0", std::to_string(1).c_str(), nullptr, nullptr, 0);
//    con_set_value_str(con, "Var_0", std::to_string(2).c_str(), nullptr, nullptr, 0);
//    con_set_value_str(con, "Var_0", std::to_string(3).c_str(), nullptr, nullptr, 0);
}

TEST(fileRead, test){
    char str[64];
    std::vector<std::string> vec_str;
    const char* data = "Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.Hello, world! This is a test string.";

    for(int i = 0; i < 1000; i++) {
        memcpy(str, data + i, 10);
        vec_str.push_back(str);
    }
}