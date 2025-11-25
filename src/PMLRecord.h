//
// Created by 79933 on 2025/6/19.
//

#ifndef POSTMORTEMLOGGER_PMLRECORD_H
#define POSTMORTEMLOGGER_PMLRECORD_H

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <charconv>

#include "PMLTypes.h"
#include "TDMS/TDMS_master.h"
#include "mdflib/MdfExport.h"

enum class RecordType : unsigned int{
    TDMS = 0,
    CSV = 1,
    CSVWithoutDes = 2,
    CSVWithoutDesUnit = 3,
    MDF3 = 4,
    MDF4 = 5
};

class Record {
protected:
    std::vector<std::string> chnDblList;
    std::vector<std::string> chnStrList;
    std::vector<Property> proList;
    uint16_t preTime;
    uint16_t postTime;
    uint16_t frequency;
    std::chrono::system_clock::time_point beginTime;
    std::chrono::system_clock::time_point stopTime;
    std::string filePath;
    bool isPre = true;

public:
    Record(const std::vector<std::string> &chnDblList,
           const std::vector<std::string> &chnStrList,
           const std::vector<Property> &proList,
           uint16_t preTime,
           uint16_t postTime,
           uint16_t frequency,
           std::chrono::system_clock::time_point beginTime,
           std::chrono::system_clock::time_point stopTime,
           std::string &filePath);

    virtual ~Record() = default;

    virtual void create_file() = 0;

    virtual int32_t set_data(std::vector<std::chrono::system_clock::time_point> timePoint, const std::vector<std::vector<double>> &dbl_data, const std::vector<std::vector<std::string>> &str_data) = 0;

    virtual void set_isPre(bool pro) { isPre = pro; }

    virtual void save() = 0;
};

Record::Record(const std::vector<std::string> &chnDblList, const std::vector<std::string> &chnStrList, const std::vector<Property> &proList,
               const uint16_t preTime, const uint16_t postTime, const uint16_t frequency,
               std::chrono::system_clock::time_point beginTime, std::chrono::system_clock::time_point stopTime, std::string &filePath)
        : chnDblList(chnDblList),
          chnStrList(chnStrList),
          proList(proList),
          preTime(preTime),
          postTime(postTime),
          frequency(frequency),
          beginTime(beginTime),
          stopTime(stopTime),
          filePath(filePath) {}

class TDMSRecord : public Record {
private:
    void *fileHandle = nullptr;
    void *groupHandlePre = nullptr;
    void *groupHandlePost = nullptr;
    void *channelTimeHandlePre = nullptr;
    void *channelTimeHandlePost = nullptr;
    std::vector<void *> channelHandlePreDbl;
    std::vector<void *> channelHandlePreStr;
    std::vector<void *> channelHandlePostDbl;
    std::vector<void *> channelHandlePostStr;
    std::vector<std::chrono::system_clock::time_point> vec_time;
    std::vector<std::vector<double>> vec_dbl;
    std::vector<std::vector<std::string>> vec_str;
    std::vector<int64_t> timePoints;
    uint64_t saveSize = 2000;

public:
    using Record::Record;

    //创建TDMS文件
    void create_file() override {
        vec_dbl.resize(chnDblList.size());
        vec_str.resize(chnStrList.size());
        for (auto cn: vec_dbl)
            cn.reserve(saveSize);
        for (auto cn: vec_str)
            cn.reserve(saveSize);
        filePath = filePath + ".tdms";
        fileHandle = nullptr;
        open_file(filePath.c_str(), "w", fileHandle);
//        std::cout << "create TDMS file" << std::endl;
        //创建属性
        for (const auto &pro: proList) {
            if (pro.type)
                create_file_property_double(fileHandle, pro.name.c_str(), pro.p_dbl);
            else
                create_file_property_string(fileHandle, pro.name.c_str(), pro.p_str.c_str());
        }
        //创建组（group）
        create_group(fileHandle, "Pre-shutdown", groupHandlePre);
        create_group(fileHandle, "Post-shutdown", groupHandlePost);
        //创建通道（channel)
        create_channel(groupHandlePre, "Time", channelTimeHandlePre);
        create_channel(groupHandlePost, "Time", channelTimeHandlePost);
        void *channelHandle = nullptr;
        for (const auto &cn: chnDblList) {
            create_channel(groupHandlePre, cn.c_str(), channelHandle);
            channelHandlePreDbl.push_back(channelHandle);
            create_channel(groupHandlePost, cn.c_str(), channelHandle);
            channelHandlePostDbl.push_back(channelHandle);
        }
        for (const auto &cn: chnStrList) {
            create_channel(groupHandlePre, cn.c_str(), channelHandle);
            channelHandlePreStr.push_back(channelHandle);
            create_channel(groupHandlePost, cn.c_str(), channelHandle);
            channelHandlePostStr.push_back(channelHandle);
        }
        timePoints.reserve(saveSize);
    }

    //写入数据
    int32_t set_data(std::vector<std::chrono::system_clock::time_point> timePoint, const std::vector<std::vector<double>> &dbl_data, const std::vector<std::vector<std::string>> &str_data) override {
        //将写入的数据由每个时刻记录各个通道的数据转置为各通道每个时刻的值，便于存储
        vec_dbl = transMatrix(dbl_data);
        vec_str = transMatrix(str_data);

        //将时间转成TDMS对应的存储格式
        timePoints.clear();
        for (const auto &time: timePoint)
            timePoints.push_back(time.time_since_epoch().count());

        return OPTION_OK;
    }

    //保存数据
    void save() override {
        if (timePoints.empty())
            return;
        if (isPre) {
            //测试每次pre存储数据量
//            cout << "pre save" << endl;
//            cout << timePoints.size() << endl;
            set_channel_data_time(channelTimeHandlePre, timePoints.size(), timePoints.data());
            for (uint32_t i = 0; i < vec_dbl.size(); i++)
                set_channel_data_double(channelHandlePreDbl[i], vec_dbl[i].size(), vec_dbl[i].data());
            std::vector<const char *> cstrs;
            cstrs.reserve(vec_str.size());
            for (uint32_t i = 0; i < vec_str.size(); i++) {
                for (auto &it: vec_str[i])
                    cstrs.push_back(it.c_str());
                set_channel_data_string(channelHandlePreStr[i], vec_str[i].size(), cstrs.data());
                cstrs.clear();
            }
        } else {
            //测试每次post存储数据量
//            cout << "post save" << endl;
//            cout << timePoints.size() << endl;
            set_channel_data_time(channelTimeHandlePost, timePoints.size(), timePoints.data());
            for (uint32_t i = 0; i < vec_dbl.size(); i++)
                set_channel_data_double(channelHandlePostDbl[i], vec_dbl[i].size(), vec_dbl[i].data());
            std::vector<const char *> cstrs;
            cstrs.reserve(vec_str.size());
            for (uint32_t i = 0; i < vec_str.size(); i++) {
                for (auto &it: vec_str[i])
                    cstrs.push_back(it.c_str());
                set_channel_data_string(channelHandlePostStr[i], vec_str[i].size(), cstrs.data());
                cstrs.clear();
            }
        }
        save_file(fileHandle);
        timePoints.clear();
        for (auto &cn: vec_dbl)
            cn.clear();
    }

    //默认写入pre，写入post时调用，同时使用false作为参数
    void set_isPre(bool pre) override { isPre = pre; }

    ~TDMSRecord() override {
        close_file(fileHandle);
    }
};

class CSVRecord : public Record {
private:
    std::chrono::system_clock::time_point beginTime;
    bool isFirst = true;
#ifdef _WIN32
    HANDLE fileHandle;
    HANDLE mapHandle;
#else
    int fileHandle;
#endif
    char *buffer;
    uint64_t fileOffset = 0;
    uint32_t windowSize = 2 * 64 * 1024;
    uint32_t windowOffset = 0;
    uint32_t moveTime = 0;

public:
    using Record::Record;

    //移动映射文件窗口，win32系统无法一次映射2G以上文件
    void moveWindows() {
#ifdef _WIN32
        UnmapViewOfFile(buffer);
        windowOffset -= 64 * 1024;
        fileOffset += 64 * 1024;
        buffer = reinterpret_cast<char *>(MapViewOfFile(mapHandle, FILE_MAP_WRITE, DWORD(fileOffset >> 32), DWORD(fileOffset & 0xFFFFFFFF), windowSize));
        if (!buffer) {
            std::cerr << "MapViewOfFile failed (Move Windows), GLE=" << GetLastError() << "\n";
            return;
        }
        moveTime++;
#else
        if(msync(buffer, windowSize, MS_SYNC) == -1){
            return;
        }

        if(munmap(buffer, windowSize)){
            return;
        }
        windowOffset -= 64 * 1024;
        fileOffset += 64 * 1024;
        buffer = reinterpret_cast<char *>(mmap(nullptr, windowSize, PROT_WRITE, MAP_SHARED, fileHandle, fileOffset));
        if (!buffer) {
            //error cord
            std::cerr << "MapViewOfFile failed (Move Windows)"<< "\n";
            return;
        }
        moveTime++;
#endif
    }

    //创建文件
    void create_file() override {
        //创建文件句柄
#ifdef _WIN32
        fileHandle = CreateFileA(
                (filePath + ".csv").c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
        );
        if (fileHandle == INVALID_HANDLE_VALUE) {
            std::cerr << "CreateFile failed" << std::endl;
            return;
        }

        //设置文件大小
        LARGE_INTEGER size;
        size.QuadPart = (((preTime + postTime + 1) * frequency * (1 + chnDblList.size() + chnStrList.size())) / 1024 + 1) * 1024 * sizeof(char[64]);
        if (!SetFilePointerEx(fileHandle, size, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) {
            return;
        }

        //创建映射句柄
        mapHandle = CreateFileMapping(fileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
        if (!mapHandle) {
            std::cerr << "CreateFileMapping Failed" << std::endl;
            CloseHandle(fileHandle);
            return;
        }

        //打开映射窗口
        buffer = reinterpret_cast<char *>(MapViewOfFile(mapHandle, FILE_MAP_WRITE, 0, 0, windowSize));
#else
        fileHandle = open(filePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if(fileHandle == -1){
            //error code
            return;
        }

        //设置文件大小
        auto fileSize = (((preTime + postTime + 1) * frequency * (1 + chnDblList.size() + chnStrList.size())) / 1024 + 1) * 1024 * sizeof(char[64]);
        if (ftruncate(fileHandle, fileSize) == -1) {
            //errorCode
            return;
        }

        //打开映射窗口
        buffer = reinterpret_cast<char *>(mmap(nullptr, windowSize, PROT_WRITE, MAP_SHARED, fileHandle, fileOffset));
#endif
    }

    //写入并存储数据
    int32_t set_data(std::vector<std::chrono::system_clock::time_point> timePoint, const std::vector<std::vector<double>> &vec_dbl, const std::vector<std::vector<std::string>> &vec_str) override {
        if (isFirst) {
            beginTime = timePoint.front();
            isFirst = false;

            //创建属性
            memcpy(buffer + windowOffset, ("StartTime," + TransformStrTimeMS(beginTime)).c_str(), 28);
            windowOffset += 28;
            memcpy(buffer + windowOffset, ("\nStopTime," + TransformStrTimeMS(stopTime)).c_str(), 28);
            windowOffset += 28;
            memcpy(buffer + windowOffset, ("\nStopTimer," + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - beginTime).count() / 1000)).c_str(), 14);
            windowOffset += 14;
            memcpy(buffer + windowOffset, ("\nFrequency(Hz)," + std::to_string(frequency)).c_str(), 15 + std::to_string(frequency).size());
            windowOffset += 15 + std::to_string(frequency).size();
            for (const auto &it: proList) {
                if (it.type) {
                    memcpy(buffer + windowOffset, ('\n' + it.name + ',' + std::to_string(it.p_dbl)).c_str(), 2 + it.name.size() + std::to_string(it.p_dbl).size());
                } else {
                    memcpy(buffer + windowOffset, ('\n' + it.name + ',' + it.p_str).c_str(), 2 + it.name.size() + it.p_str.size());
                }
            }
            memcpy(buffer + windowOffset, "\n", 1);
            windowOffset += 1;

            //写入通道名
            memcpy(buffer + windowOffset, "\nTime", 5);
            windowOffset += 5;

            for (const auto &vn: chnDblList) {
                //moveWindows
                if (windowOffset + vn.size() + 3 > windowSize)
                    moveWindows();
                memcpy(buffer + windowOffset, ",", 1);
                memcpy(buffer + windowOffset + 1, ('"' + vn + '"').c_str(), vn.size() + 2);
                windowOffset += vn.size() + 3;
            }
            for (const auto &vn: chnStrList) {
                //moveWindows
                if (windowOffset + vn.size() + 3 > windowSize)
                    moveWindows();
                memcpy(buffer + windowOffset, ",", 1);
                memcpy(buffer + windowOffset + 1, ('"' + vn + '"').c_str(), vn.size() + 2);
                windowOffset += vn.size() + 3;
            }
        }

        for (uint32_t i = 0; i < timePoint.size(); i++) {
            std::array<char, 64> buf;
            {
                auto timeDouble = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(timePoint[i] - beginTime).count()) / 1000;
                auto [ptr, ec] = to_chars(buf.data(), buf.data() + buf.size(), timeDouble, std::chars_format::general, 15);
                if (ec == std::errc{}) {
                    auto len = ptr - buf.data();
                    //moveWindows
                    if (windowOffset + len + 1 > windowSize)
                        moveWindows();
                    memcpy(buffer + windowOffset, "\n", 1);
                    memcpy(buffer + windowOffset + 1, buf.data(), len);
                    windowOffset += len + 1;
                }
            }

            if (!vec_dbl.empty()) {
                for (auto &it: vec_dbl[i]) {
                    auto [ptr, ec] = to_chars(buf.data(), buf.data() + buf.size(), it, std::chars_format::general, 15);
                    if (ec == std::errc{}) {
                        auto len = ptr - buf.data();
                        //moveWindows
                        if (windowOffset + len + 1 > windowSize)
                            moveWindows();
                        memcpy(buffer + windowOffset, ",", 1);
                        memcpy(buffer + windowOffset + 1, buf.data(), len);
                        windowOffset += len + 1;
                    }
                }
            }

            if (!vec_str.empty()) {
                for (const auto &it: vec_str[i]) {
                    //moveWindows
                    if (windowOffset + it.size() + 3 > windowSize)
                        moveWindows();
                    memcpy(buffer + windowOffset, ",", 1);
                    memcpy(buffer + windowOffset + 1, ('"' + it + '"').c_str(), it.size() + 2);
                    windowOffset += it.size() + 3;
                }
            }
        }
        return 0;
    }

    void save() override {}

    ~CSVRecord() override {
#ifdef _WIN32
        UnmapViewOfFile(buffer);
        CloseHandle(mapHandle);
        //将文件长度裁剪到与数据长度相同
        LARGE_INTEGER size;
        size.QuadPart = (64 * 1024) * moveTime + windowOffset;
        if (!SetFilePointerEx(fileHandle, size, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) {
            return;
        }
        //关闭文件句柄
        CloseHandle(fileHandle);
#else
        if(msync(buffer, windowSize, MS_SYNC) == -1){
            return;
        }

        if(munmap(buffer, windowSize) == -1){
            return;
        }

        if(ftruncate(fileHandle, (64 * 1024) * moveTime + windowOffset) == -1) {
            //error
            return;
        }
        close(fileHandle);
#endif
    }

};

class CSVWithoutDesRecord : public Record {
private:
    std::chrono::system_clock::time_point beginTime;
    bool isFirst = true;
#ifdef _WIN32
    HANDLE fileHandle;
    HANDLE mapHandle;
#else
    int fileHandle;
#endif
    char *buffer;
    uint64_t fileOffset = 0;
    uint32_t windowSize = 2 * 64 * 1024;
    uint32_t windowOffset = 0;
    uint32_t moveTime = 0;

public:
    using Record::Record;

    //移动映射文件窗口，win32系统无法一次映射2G以上文件
    void moveWindows() {
#ifdef _WIN32
        UnmapViewOfFile(buffer);
        windowOffset -= 64 * 1024;
        fileOffset += 64 * 1024;
        buffer = reinterpret_cast<char *>(MapViewOfFile(mapHandle, FILE_MAP_WRITE, DWORD(fileOffset >> 32), DWORD(fileOffset & 0xFFFFFFFF), windowSize));
        if (!buffer) {
            std::cerr << "MapViewOfFile failed (Move Windows), GLE=" << GetLastError() << "\n";
            return;
        }
        moveTime++;
#else
        if(msync(buffer, windowSize, MS_SYNC) == -1){
            return;
        }

        if(munmap(buffer, windowSize)){
            return;
        }
        windowOffset -= 64 * 1024;
        fileOffset += 64 * 1024;
        buffer = reinterpret_cast<char *>(mmap(nullptr, windowSize, PROT_WRITE, MAP_SHARED, fileHandle, fileOffset));
        if (!buffer) {
            //error cord
            std::cerr << "MapViewOfFile failed (Move Windows)"<< "\n";
            return;
        }
        moveTime++;
#endif
    }

    //创建文件
    void create_file() override {
#ifdef _WIN32
        //创建文件句柄
        fileHandle = CreateFileA(
                (filePath + ".csv").c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
        );
        if (fileHandle == INVALID_HANDLE_VALUE) {
            std::cerr << "CreateFile failed" << std::endl;
            return;
        }

        //设置文件大小
        LARGE_INTEGER size;
        size.QuadPart = (((preTime + postTime + 1) * frequency * (1 + chnDblList.size() + chnStrList.size())) / 1024 + 1) * 1024 * sizeof(char[64]);
        if (!SetFilePointerEx(fileHandle, size, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) {
            return;
        }

        //创建映射句柄
        mapHandle = CreateFileMapping(fileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
        if (!mapHandle) {
            std::cerr << "CreateFileMapping Failed" << std::endl;
            CloseHandle(fileHandle);
            return;
        }

        //打开映射窗口
        buffer = reinterpret_cast<char *>(MapViewOfFile(mapHandle, FILE_MAP_WRITE, 0, 0, windowSize));
#else
        fileHandle = open((filePath + ".csv").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if(fileHandle == -1){
            //error code
            return;
        }

        //设置文件大小
        auto fileSize = (((preTime + postTime + 1) * frequency * (1 + chnDblList.size() + chnStrList.size())) / 1024 + 1) * 1024 * sizeof(char[64]);
        if (ftruncate(fileHandle, fileSize) == -1) {
            //errorCode
            return;
        }
#endif
    }

    //写入并存储数据
    int32_t set_data(std::vector<std::chrono::system_clock::time_point> timePoint, const std::vector<std::vector<double>> &vec_dbl, const std::vector<std::vector<std::string>> &vec_str) override {
        if (isFirst) {
            beginTime = timePoint.front();
            isFirst = false;

            //创建属性
            memcpy(buffer + windowOffset, ("StartTime," + TransformStrTimeMS(beginTime)).c_str(), 28);
            windowOffset += 28;
            memcpy(buffer + windowOffset, ("\nStopTime," + TransformStrTimeMS(beginTime)).c_str(), 27);
            windowOffset += 28;
            memcpy(buffer + windowOffset, ("\nStopTimer," + std::to_string(1000)).c_str(), 14);
            windowOffset += 14;
            memcpy(buffer + windowOffset, ("\nFrequency(Hz)," + std::to_string(frequency)).c_str(), 15 + std::to_string(frequency).size());
            windowOffset += 15 + std::to_string(frequency).size();
            for (const auto &it: proList) {
                if (it.type) {
                    memcpy(buffer + windowOffset, ('\n' + it.name + ',' + std::to_string(it.p_dbl)).c_str(), 2 + it.name.size() + std::to_string(it.p_dbl).size());
                } else {
                    memcpy(buffer + windowOffset, ('\n' + it.name + ',' + it.p_str).c_str(), 2 + it.name.size() + it.p_str.size());
                }
            }
            memcpy(buffer + windowOffset, "\n", 1);
            windowOffset += 1;

            //写入通道名
            memcpy(buffer + windowOffset, "\nTime", 5);
            windowOffset += 5;

            for (const auto &vn: chnDblList) {
                //moveWindows
                if (windowOffset + vn.size() + 3 > windowSize)
                    moveWindows();
                memcpy(buffer + windowOffset, ",", 1);
                memcpy(buffer + windowOffset + 1, ('"' + vn + '"').c_str(), vn.size() + 2);
                windowOffset += vn.size() + 3;
            }
            for (const auto &vn: chnStrList) {
                //moveWindows
                if (windowOffset + vn.size() + 3 > windowSize)
                    moveWindows();
                memcpy(buffer + windowOffset, ",", 1);
                memcpy(buffer + windowOffset + 1, ('"' + vn + '"').c_str(), vn.size() + 2);
                windowOffset += vn.size() + 3;
            }
        }

        for (uint32_t i = 0; i < timePoint.size(); i++) {
            std::array<char, 64> buf;
            {
//                cout << TransformStrTimeMS(timePoint[i]) << endl;
                auto timeDouble = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(timePoint[i] - beginTime).count()) / 1000;
                auto [ptr, ec] = to_chars(buf.data(), buf.data() + buf.size(), timeDouble, std::chars_format::general, 15);
                if (ec == std::errc{}) {
                    auto len = ptr - buf.data();
                    //moveWindows
                    if (windowOffset + len + 1 > windowSize)
                        moveWindows();
                    memcpy(buffer + windowOffset, "\n", 1);
                    memcpy(buffer + windowOffset + 1, buf.data(), len);
                    windowOffset += len + 1;
                }
            }

            if (!vec_dbl.empty()) {
                for (auto &it: vec_dbl[i]) {
                    auto [ptr, ec] = to_chars(buf.data(), buf.data() + buf.size(), it, std::chars_format::general, 15);
                    if (ec == std::errc{}) {
                        auto len = ptr - buf.data();
                        //moveWindows
                        if (windowOffset + len + 1 > windowSize)
                            moveWindows();
                        memcpy(buffer + windowOffset, ",", 1);
                        memcpy(buffer + windowOffset + 1, buf.data(), len);
                        windowOffset += len + 1;
                    }
                }
            }

            if (!vec_str.empty()) {
                for (const auto &it: vec_str[i]) {
                    //moveWindows
                    if (windowOffset + it.size() + 3 > windowSize)
                        moveWindows();
                    memcpy(buffer + windowOffset, ",", 1);
                    memcpy(buffer + windowOffset + 1, ('"' + it + '"').c_str(), it.size() + 2);
                    windowOffset += it.size() + 3;
                }
            }
        }
        return 0;
    }

    void save() override {}

    ~CSVWithoutDesRecord() override {
#ifdef _WIN32
        UnmapViewOfFile(buffer);
        CloseHandle(mapHandle);
        //将文件长度裁剪到与数据长度相同
        LARGE_INTEGER size;
        size.QuadPart = (64 * 1024) * moveTime + windowOffset;
        if (!SetFilePointerEx(fileHandle, size, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) {
            return;
        }
        //关闭文件句柄
        CloseHandle(fileHandle);
#else
        if(msync(buffer, windowSize, MS_SYNC) == -1){
            return;
        }

        if(munmap(buffer, windowSize) == -1){
            return;
        }

        if(ftruncate(fileHandle, (64 * 1024) * moveTime + windowOffset) == -1) {
            //error
            return;
        }
        close(fileHandle);
#endif
    }
};

class CSVWithoutDesUnitRecord : public Record {
private:
    std::chrono::system_clock::time_point beginTime;
    bool isFirst = true;
#ifdef _WIN32
    HANDLE fileHandle;
    HANDLE mapHandle;
#else
    int fileHandle;
#endif
    char *buffer;
    uint64_t fileSize = 0;
    uint64_t fileOffset = 0;
    uint32_t windowSize = 2 * 64 * 1024;
    uint32_t windowOffset = 0;
    uint32_t moveTime = 0;

public:
    using Record::Record;

    //移动映射文件窗口，win32系统无法一次映射2G以上文件
    void moveWindows() {
#ifdef _WIN32
        UnmapViewOfFile(buffer);
        windowOffset -= 64 * 1024;
        fileOffset += 64 * 1024;
        buffer = reinterpret_cast<char *>(MapViewOfFile(mapHandle, FILE_MAP_WRITE, DWORD(fileOffset >> 32), DWORD(fileOffset & 0xFFFFFFFF), windowSize));
        if (!buffer) {
            std::cerr << "MapViewOfFile failed (Move Windows), GLE=" << GetLastError() << "\n";
            return;
        }
        moveTime++;
#else
        if(msync(buffer, windowSize, MS_SYNC) == -1){
            return;
        }

        if(munmap(buffer, windowSize)){
            return;
        }
        windowOffset -= 64 * 1024;
        fileOffset += 64 * 1024;
        buffer = reinterpret_cast<char *>(mmap(nullptr, windowSize, PROT_WRITE, MAP_SHARED, fileHandle, fileOffset));
        if (!buffer) {
            //error cord
            std::cerr << "MapViewOfFile failed (Move Windows)"<< "\n";
            return;
        }
        moveTime++;
#endif
    }

    //创建文件
    void create_file() override {
#ifdef _WIN32
        //创建文件句柄
        fileHandle = CreateFileA(
                (filePath + ".csv").c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL
        );
        if (fileHandle == INVALID_HANDLE_VALUE) {
            std::cerr << "CreateFile failed" << std::endl;
            return;
        }

        //设置文件大小
        LARGE_INTEGER size;
        size.QuadPart = (((preTime + postTime + 1) * frequency * (1 + chnDblList.size() + chnStrList.size())) / 1024 + 1) * 1024 * sizeof(char[64]);
        if (!SetFilePointerEx(fileHandle, size, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) {
            return;
        }

        //创建映射句柄
        mapHandle = CreateFileMapping(fileHandle, NULL, PAGE_READWRITE, 0, 0, NULL);
        if (!mapHandle) {
            std::cerr << "CreateFileMapping Failed" << std::endl;
            CloseHandle(fileHandle);
            return;
        }

        //打开映射窗口
        buffer = reinterpret_cast<char *>(MapViewOfFile(mapHandle, FILE_MAP_WRITE, 0, 0, windowSize));

#else
        fileHandle = open(filePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if(fileHandle == -1){
            //error code
            return;
        }

        //设置文件大小
        auto fileSize = (((preTime + postTime + 1) * frequency * (1 + chnDblList.size() + chnStrList.size())) / 1024 + 1) * 1024 * sizeof(char[64]);
        if (ftruncate(fileHandle, fileSize) == -1) {
            //errorCode
            return;
        }
#endif
    }

    //写入并存储数据
    int32_t set_data(std::vector<std::chrono::system_clock::time_point> timePoint, const std::vector<std::vector<double>> &vec_dbl, const std::vector<std::vector<std::string>> &vec_str) override {
        if (isFirst) {
            beginTime = timePoint.front();
            isFirst = false;

            //创建属性
            memcpy(buffer + windowOffset, ("StartTime," + TransformStrTimeMS(beginTime)).c_str(), 28);
            windowOffset += 28;
            memcpy(buffer + windowOffset, ("\nStopTime," + TransformStrTimeMS(beginTime)).c_str(), 27);
            windowOffset += 28;
            memcpy(buffer + windowOffset, ("\nStopTimer," + std::to_string(1000)).c_str(), 14);
            windowOffset += 14;
            memcpy(buffer + windowOffset, ("\nFrequency(Hz)," + std::to_string(frequency)).c_str(), 15 + std::to_string(frequency).size());
            windowOffset += 15 + std::to_string(frequency).size();
            for (const auto &it: proList) {
                if (it.type)
                    memcpy(buffer + windowOffset, ('\n' + it.name + ',' + std::to_string(it.p_dbl)).c_str(), 2 + it.name.size() + std::to_string(it.p_dbl).size());
                else
                    memcpy(buffer + windowOffset, ('\n' + it.name + ',' + it.p_str).c_str(), 2 + it.name.size() + it.p_str.size());
            }
            memcpy(buffer + windowOffset, "\n", 1);
            windowOffset += 1;

            //写入通道名
            memcpy(buffer + windowOffset, "\nTime", 5);
            windowOffset += 5;

            for (const auto &vn: chnDblList) {
                //moveWindows
                if (windowOffset + vn.size() + 3 > windowSize)
                    moveWindows();
                memcpy(buffer + windowOffset, ",", 1);
                memcpy(buffer + windowOffset + 1, ('"' + vn + '"').c_str(), vn.size() + 2);
                windowOffset += vn.size() + 3;
            }
            for (const auto &vn: chnStrList) {
                //moveWindows
                if (windowOffset + vn.size() + 3 > windowSize)
                    moveWindows();
                memcpy(buffer + windowOffset, ",", 1);
                memcpy(buffer + windowOffset + 1, ('"' + vn + '"').c_str(), vn.size() + 2);
                windowOffset += vn.size() + 3;
            }
        }

        for (uint32_t i = 0; i < timePoint.size(); i++) {
            std::array<char, 64> buf;
            {
//                cout << TransformStrTimeMS(timePoint[i]) << endl;
                auto timeDouble = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(timePoint[i] - beginTime).count()) / 1000;
                auto [ptr, ec] = to_chars(buf.data(), buf.data() + buf.size(), timeDouble, std::chars_format::general, 15);
                if (ec == std::errc{}) {
                    auto len = ptr - buf.data();
                    //moveWindows
                    if (windowOffset + len + 1 > windowSize)
                        moveWindows();
                    memcpy(buffer + windowOffset, "\n", 1);
                    memcpy(buffer + windowOffset + 1, buf.data(), len);
                    windowOffset += len + 1;
                }
            }

            if (!vec_dbl.empty()) {
                for (auto &it: vec_dbl[i]) {
                    auto [ptr, ec] = to_chars(buf.data(), buf.data() + buf.size(), it, std::chars_format::general, 15);
                    if (ec == std::errc{}) {
                        auto len = ptr - buf.data();
                        //moveWindows
                        if (windowOffset + len + 1 > windowSize)
                            moveWindows();
                        memcpy(buffer + windowOffset, ",", 1);
                        memcpy(buffer + windowOffset + 1, buf.data(), len);
                        windowOffset += len + 1;
                    }
                }
            }

            if (!vec_str.empty()) {
                for (const auto &it: vec_str[i]) {
                    //moveWindows
                    if (windowOffset + it.size() + 3 > windowSize)
                        moveWindows();
                    memcpy(buffer + windowOffset, ",", 1);
                    memcpy(buffer + windowOffset + 1, ('"' + it + '"').c_str(), it.size() + 2);
                    windowOffset += it.size() + 3;
                }
            }
        }
        return 0;
    }

    void save() override {}

    ~CSVWithoutDesUnitRecord() override {
#ifdef _WIN32
        UnmapViewOfFile(buffer);
        CloseHandle(mapHandle);
        //将文件长度裁剪到与数据长度相同
        LARGE_INTEGER size;
        size.QuadPart = (64 * 1024) * moveTime + windowOffset;
        if (!SetFilePointerEx(fileHandle, size, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) {
            return;
        }
        //关闭文件句柄
        CloseHandle(fileHandle);
#else
        if(msync(buffer, windowSize, MS_SYNC) == -1){
            return;
        }

        if(munmap(buffer, windowSize) == -1){
            return;
        }

        if(ftruncate(fileHandle, (64 * 1024) * moveTime + windowOffset) == -1) {
            //error
            return;
        }
        close(fileHandle);
#endif
    }
};

using namespace mdf;
using namespace MdfLibrary;
using namespace MdfLibrary::ExportFunctions;

class MDF3Record : public Record {
private:
    mdf::MdfWriter *writer;
    IChannelGroup *cgp;
    IChannelGroup *cgc;
    std::vector<IChannel *> propertyList;
    std::vector<IChannel *> channelList;
    bool isBegin = true;

public:
    using Record::Record;

    //创建文件
    void create_file() override {
        filePath = filePath + ".mdf";
        writer = MdfWriterInit(MdfWriterType::Mdf3Basic, filePath.c_str());

        auto *dgp = MdfWriterCreateDataGroup(writer);
        cgp = MdfDataGroupCreateChannelGroup(dgp);
        MdfChannelGroupSetName(cgp, "PropertyData");
        propertyList.resize(proList.size() + 4);

        propertyList[0] = MdfChannelGroupCreateChannel(cgp);
        MdfChannelSetName(propertyList[0], "StartTime");
        MdfChannelSetDescription(propertyList[0], "StartTime");
        MdfChannelSetType(propertyList[0], ChannelType::FixedLength);
        MdfChannelSetDataType(propertyList[0], ChannelDataType::StringAscii);
        MdfChannelSetDataBytes(propertyList[0], 32);

        propertyList[1] = MdfChannelGroupCreateChannel(cgp);
        MdfChannelSetName(propertyList[1], "StopTime");
        MdfChannelSetDescription(propertyList[1], "StopTime");
        MdfChannelSetType(propertyList[1], ChannelType::FixedLength);
        MdfChannelSetDataType(propertyList[1], ChannelDataType::StringAscii);
        MdfChannelSetDataBytes(propertyList[1], 32);

        propertyList[2] = MdfChannelGroupCreateChannel(cgp);
        MdfChannelSetName(propertyList[2], "StopTimer");
        MdfChannelSetDescription(propertyList[2], "StopTimer");
        MdfChannelSetType(propertyList[2], ChannelType::FixedLength);
        MdfChannelSetDataType(propertyList[2], ChannelDataType::FloatLe);
        MdfChannelSetDataBytes(propertyList[2], sizeof(float));

        propertyList[3] = MdfChannelGroupCreateChannel(cgp);
        MdfChannelSetName(propertyList[3], "Frequency(Hz)");
        MdfChannelSetDescription(propertyList[3], "Frequency(Hz)");
        MdfChannelSetType(propertyList[3], ChannelType::FixedLength);
        MdfChannelSetDataType(propertyList[3], ChannelDataType::FloatLe);
        MdfChannelSetDataBytes(propertyList[3], sizeof(float));

        for (uint32_t i = 0; i < proList.size(); i++) {
            if (proList[i].type) {
                propertyList[i + 4] = MdfChannelGroupCreateChannel(cgp);
                MdfChannelSetName(propertyList[i + 4], proList[i].name.c_str());
                MdfChannelSetDescription(propertyList[i + 4], proList[i].name.c_str());
                MdfChannelSetType(propertyList[i + 4], ChannelType::FixedLength);
                MdfChannelSetDataType(propertyList[i + 4], ChannelDataType::FloatLe);
                MdfChannelSetDataBytes(propertyList[i + 4], sizeof(float));
            } else {
                propertyList[i + 4] = MdfChannelGroupCreateChannel(cgp);
                MdfChannelSetName(propertyList[i + 4], proList[i].name.c_str());
                MdfChannelSetDescription(propertyList[i + 4], proList[i].name.c_str());
                MdfChannelSetType(propertyList[i + 4], ChannelType::FixedLength);
                MdfChannelSetDataType(propertyList[i + 4], ChannelDataType::StringAscii);
                MdfChannelSetDataBytes(propertyList[i + 4], 20);
            }
        }

        auto *dgc = MdfWriterCreateDataGroup(writer);
        cgc = MdfDataGroupCreateChannelGroup(dgc);
        MdfChannelGroupSetName(cgc, "PropertyData");
        channelList.resize(chnDblList.size() + chnStrList.size() + 2);

        //创建时间通道
        channelList[0] = MdfChannelGroupCreateChannel(cgc);
        MdfChannelSetName(channelList[0], "Time");
        MdfChannelSetDescription(channelList[0], "Time channel");
        MdfChannelSetType(channelList[0], ChannelType::Master);
        MdfChannelSetDataType(channelList[0], ChannelDataType::FloatLe);
        MdfChannelSetDataBytes(channelList[0], 4);
        MdfChannelSetUnit(channelList[0], "s");

        channelList[1] = MdfChannelGroupCreateChannel(cgc);
        MdfChannelSetName(channelList[1], "DataTime");
        MdfChannelSetDescription(channelList[1], "Time channel");
        MdfChannelSetType(channelList[1], ChannelType::FixedLength);
        MdfChannelSetDataType(channelList[1], ChannelDataType::StringAscii);
        MdfChannelSetDataBytes(channelList[1], 20);
        //创建double通道
        for (uint32_t i = 0; i < chnDblList.size(); i++) {
            channelList[i + 2] = MdfChannelGroupCreateChannel(cgc);
            MdfChannelSetName(channelList[i + 2], chnDblList[i].c_str());
            MdfChannelSetDescription(channelList[i + 2], "float");
            MdfChannelSetType(channelList[i + 2], ChannelType::FixedLength);
            MdfChannelSetDataType(channelList[i + 2], ChannelDataType::FloatLe);
            MdfChannelSetDataBytes(channelList[i + 2], sizeof(float));
        }
        //创建string通道
        for (uint32_t i = 0; i < chnStrList.size(); i++) {
            channelList[i + chnDblList.size() + 2] = MdfChannelGroupCreateChannel(cgc);
            MdfChannelSetName(channelList[i + chnDblList.size() + 2], chnStrList[i].c_str());
            MdfChannelSetDescription(channelList[i + chnDblList.size() + 2], "string");
            MdfChannelSetType(channelList[i + chnDblList.size() + 2], ChannelType::FixedLength);
            MdfChannelSetDataType(channelList[i + chnDblList.size() + 2], ChannelDataType::StringAscii);
            MdfChannelSetDataBytes(channelList[i + chnDblList.size() + 2], 64);
        }

        MdfWriterInitMeasurement(writer);
        MdfWriterStartMeasurement(writer, 100000000);
        MdfChannelSetChannelValueAsString(propertyList[0], TransformStrTimeMS(beginTime).c_str());
        MdfChannelSetChannelValueAsString(propertyList[1], TransformStrTimeMS(stopTime).c_str());
        MdfChannelSetChannelValueAsFloat(propertyList[2], static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - beginTime).count()) / 1000);
        MdfChannelSetChannelValueAsFloat(propertyList[3], frequency);
        for (uint32_t i = 0; i < proList.size(); i++) {
            if (proList[i].type) {
                MdfChannelSetChannelValueAsFloat(propertyList[i + 4], proList[i].p_dbl);
            } else {
                MdfChannelSetChannelValueAsString(propertyList[i + 4], proList[i].p_str.c_str());
            }
        }
        MdfWriterSaveSample(writer, cgp, 10000000);
        MdfWriterStopMeasurement(writer, 110000000);
        MdfWriterFinalizeMeasurement(writer);
    }

    //写入并存储数据
    int32_t set_data(std::vector<std::chrono::system_clock::time_point> timePoint, const std::vector<std::vector<double>> &vec_dbl, const std::vector<std::vector<std::string>> &vec_str) override {
        if (isBegin) {
            beginTime = timePoint.front();
            isBegin = false;
            MdfWriterInitMeasurement(writer);
            MdfWriterStartMeasurement(writer, TimeStampToNs(beginTime));
        }
        //记录数据
        for (uint32_t i = 0; i < timePoint.size(); i++) {
            MdfChannelSetChannelValueAsFloat(channelList[0], static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(timePoint[i] - beginTime).count()));
            MdfChannelSetChannelValueAsString(channelList[1], TransformStrTimeMS(timePoint[i]).c_str());
            for (uint32_t j = 0; j < chnDblList.size(); j++)
                MdfChannelSetChannelValueAsFloat(channelList[j + 2], vec_dbl[i][j]);
            for (uint32_t j = 0; j < chnStrList.size(); j++)
                MdfChannelSetChannelValueAsString(channelList[j + chnDblList.size() + 2], vec_str[i][j].c_str());
            auto ns70 = TimeStampToNs(timePoint[i]);
            MdfWriterSaveSample(writer, cgc, ns70);
        }
        return 0;
    }

    void save() override {}

    ~MDF3Record() override {
        MdfWriterStopMeasurement(writer, TimeStampToNs(beginTime));
        MdfWriterFinalizeMeasurement(writer);
    }
};

#include <iostream>

class MDF4Record : public Record {
private:
    //
//    std::chrono::system_clock::time_point beginTime;
//    std::chrono::system_clock::time_point endTime;
    MdfWriter *writer;
    IChannelGroup *cgp;
    IChannelGroup *cgc;
    std::vector<IChannel *> proHandles;
    std::vector<IChannel *> chnHandles;
    bool isBegin = true;

public:
    using Record::Record;

    void create_file() override {
        writer = MdfWriterInit(MdfWriterType::Mdf4Basic, (filePath + ".mf4").c_str());

        auto *pro_group = MdfWriterCreateDataGroup(writer);
        cgp = MdfDataGroupCreateChannelGroup(pro_group);
        MdfChannelGroupSetName(cgp, "PropertyData");

        proHandles.resize(proList.size() + 4);

        proHandles[0] = MdfChannelGroupCreateChannel(cgp);
        MdfChannelSetName(proHandles[0], "StartTime");
        MdfChannelSetType(proHandles[0], ChannelType::FixedLength);
        MdfChannelSetSync(proHandles[0], ChannelSyncType::None);
        MdfChannelSetDataType(proHandles[0], ChannelDataType::StringAscii);
        MdfChannelSetDataBytes(proHandles[0], 32);

        proHandles[1] = MdfChannelGroupCreateChannel(cgp);
        MdfChannelSetName(proHandles[1], "StopTime");
        MdfChannelSetType(proHandles[1], ChannelType::FixedLength);
        MdfChannelSetSync(proHandles[1], ChannelSyncType::None);
        MdfChannelSetDataType(proHandles[1], ChannelDataType::StringAscii);
        MdfChannelSetDataBytes(proHandles[1], 32);

        proHandles[2] = MdfChannelGroupCreateChannel(cgp);
        MdfChannelSetName(proHandles[2], "StopTimer");
        MdfChannelSetType(proHandles[2], ChannelType::FixedLength);
        MdfChannelSetSync(proHandles[2], ChannelSyncType::None);
        MdfChannelSetDataType(proHandles[2], ChannelDataType::FloatLe);
        MdfChannelSetDataBytes(proHandles[2], 8);

        proHandles[3] = MdfChannelGroupCreateChannel(cgp);
        MdfChannelSetName(proHandles[3], "Frequency(Hz)");
        MdfChannelSetType(proHandles[3], ChannelType::FixedLength);
        MdfChannelSetSync(proHandles[3], ChannelSyncType::None);
        MdfChannelSetDataType(proHandles[3], ChannelDataType::FloatLe);
        MdfChannelSetDataBytes(proHandles[3], 8);

        for (uint32_t i = 0; i < proList.size(); i++) {
            if (proList[i].type) {
                proHandles[i + 4] = MdfChannelGroupCreateChannel(cgp);
                MdfChannelSetName(proHandles[i + 4], proList[i].name.c_str());
                MdfChannelSetType(proHandles[i + 4], ChannelType::FixedLength);
                MdfChannelSetSync(proHandles[i + 4], ChannelSyncType::None);
                MdfChannelSetDataType(proHandles[i + 4], ChannelDataType::FloatLe);
                MdfChannelSetDataBytes(proHandles[i + 4], 8);
            } else {
                proHandles[i + 4] = MdfChannelGroupCreateChannel(cgp);
                MdfChannelSetName(proHandles[i + 4], proList[i].name.c_str());
                MdfChannelSetType(proHandles[i + 4], ChannelType::FixedLength);
                MdfChannelSetSync(proHandles[i + 4], ChannelSyncType::None);
                MdfChannelSetDataType(proHandles[i + 4], ChannelDataType::StringAscii);
                MdfChannelSetDataBytes(proHandles[i + 4], 64);
            }
        }

        MdfWriterInitMeasurement(writer);
        MdfWriterStartMeasurement(writer, 1000000000);

        MdfChannelSetChannelValueAsString(proHandles[0], TransformStrTimeMS(beginTime).c_str());
        MdfChannelSetChannelValueAsString(proHandles[1], TransformStrTimeMS(stopTime).c_str());
        MdfChannelSetChannelValueAsFloat(proHandles[2], std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - beginTime).count() / 1000);
        MdfChannelSetChannelValueAsFloat(proHandles[3], frequency);
        for (uint32_t i = 0; i < proList.size(); i++) {
            if (proList[i].type) {
                MdfChannelSetChannelValueAsFloat(proHandles[i + 4], proList[i].p_dbl);
            } else {
                MdfChannelSetChannelValueAsString(proHandles[i + 4], proList[i].p_str.c_str());
            }
        }
        MdfWriterSaveSample(writer, cgp, 100000000);
        MdfWriterStopMeasurement(writer, 1100000000);
        MdfWriterFinalizeMeasurement(writer);

        auto *data_group = MdfWriterCreateDataGroup(writer);;
        cgc = MdfDataGroupCreateChannelGroup(data_group);
        MdfChannelGroupSetName(cgc, "ChannelData");

        chnHandles.resize(chnDblList.size() + chnStrList.size() + 2);
        //创建Time时间通道
        chnHandles[0] = MdfChannelGroupCreateChannel(cgc);
        MdfChannelSetName(chnHandles[0], "Time");
        MdfChannelSetType(chnHandles[0], ChannelType::Master);
        MdfChannelSetSync(chnHandles[0], ChannelSyncType::Time);
        MdfChannelSetDataType(chnHandles[0], ChannelDataType::FloatLe);
        MdfChannelSetDataBytes(chnHandles[0], 8);
        MdfChannelSetUnit(chnHandles[0], "s");
        //创建DataTime时间通道
        chnHandles[1] = MdfChannelGroupCreateChannel(cgc);
        MdfChannelSetName(chnHandles[1], "DataTime");
        MdfChannelSetType(chnHandles[1], ChannelType::FixedLength);
        MdfChannelSetSync(chnHandles[1], ChannelSyncType::None);
        MdfChannelSetDataType(chnHandles[1], ChannelDataType::StringAscii);
        MdfChannelSetDataBytes(chnHandles[1], 32);

        //创建double通道
        for (uint32_t i = 0; i < chnDblList.size(); i++) {
            chnHandles[i + 2] = MdfChannelGroupCreateChannel(cgc);
            MdfChannelSetName(chnHandles[i + 2], chnDblList[i].c_str());
            MdfChannelSetType(chnHandles[i + 2], ChannelType::FixedLength);
            MdfChannelSetSync(chnHandles[i + 2], ChannelSyncType::None);
            MdfChannelSetDataType(chnHandles[i + 2], ChannelDataType::FloatLe);
            MdfChannelSetDataBytes(chnHandles[i + 2], 8);
        }

        //创建string通道
        for (uint32_t i = 0; i < chnStrList.size(); i++) {
            chnHandles[i + chnDblList.size() + 2] = MdfChannelGroupCreateChannel(cgc);
            MdfChannelSetName(chnHandles[i + chnDblList.size() + 2], chnStrList[i].c_str());
            MdfChannelSetType(chnHandles[i + chnDblList.size() + 2], ChannelType::FixedLength);
            MdfChannelSetSync(chnHandles[i + chnDblList.size() + 2], ChannelSyncType::None);
            MdfChannelSetDataType(chnHandles[i + chnDblList.size() + 2], ChannelDataType::StringAscii);
            MdfChannelSetDataBytes(chnHandles[i + chnDblList.size() + 2], 64);
        }
    }

    //写入并存储数据
    int32_t set_data(std::vector<std::chrono::system_clock::time_point> timePoint, const std::vector<std::vector<double>> &vec_dbl, const std::vector<std::vector<std::string>> &vec_str) override {
        //开始记录
        if (isBegin) {
            MdfWriterInitMeasurement(writer);
            beginTime = timePoint.front();
            MdfWriterStartMeasurement(writer, TimeStampToNs(beginTime));
            isBegin = false;
        }
        //记录输入数据
        for (uint32_t i = 0; i < timePoint.size(); i++) {
            MdfChannelSetChannelValueAsFloat(chnHandles[0], static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(timePoint[i] - beginTime).count()) / 1000);
            MdfChannelSetChannelValueAsString(chnHandles[1], TransformStrTimeMS(timePoint[i]).c_str());
            for (uint32_t j = 0; j < chnDblList.size(); j++)
                MdfChannelSetChannelValueAsFloat(chnHandles[j + 2], vec_dbl[i][j]);
            for (uint32_t j = 0; j < chnStrList.size(); j++)
                MdfChannelSetChannelValueAsString(chnHandles[j + chnDblList.size() + 2], vec_str[i][j].c_str());
            MdfWriterSaveSample(writer, cgc, TimeStampToNs(timePoint[i]));
            stopTime = timePoint[i];
        }
        return 0;
    }

    void save() override {}

    ~MDF4Record() override {
        MdfWriterStopMeasurement(writer, TimeStampToNs(stopTime));
        MdfWriterFinalizeMeasurement(writer);
    }
};

// 工厂函数
std::unique_ptr<Record> createRecord(
        RecordType type,
        uint16_t preTime,
        uint16_t postTime,
        uint16_t frequency,
        std::chrono::system_clock::time_point beginTime,
        std::chrono::system_clock::time_point stopTime,
        std::string &filePath,
        const std::vector<std::string> &dbl_names = {},
        const std::vector<std::string> &str_names = {},
        const std::vector<Property> proList = {}
) {
    switch (type) {
        case RecordType::TDMS:
            return std::make_unique<TDMSRecord>(dbl_names, str_names, proList, preTime, postTime, frequency, beginTime, stopTime, filePath);
        case RecordType::CSV:
            return std::make_unique<CSVRecord>(dbl_names, str_names, proList, preTime, postTime, frequency, beginTime, stopTime, filePath);
        case RecordType::CSVWithoutDes:
            return std::make_unique<CSVWithoutDesRecord>(dbl_names, str_names, proList, preTime, postTime, frequency, beginTime, stopTime, filePath);
        case RecordType::CSVWithoutDesUnit:
            return std::make_unique<CSVWithoutDesUnitRecord>(dbl_names, str_names, proList, preTime, postTime, frequency, beginTime, stopTime, filePath);
        case RecordType::MDF3:
            return std::make_unique<MDF3Record>(dbl_names, str_names, proList, preTime, postTime, frequency, beginTime, stopTime, filePath);
        case RecordType::MDF4:
            return std::make_unique<MDF4Record>(dbl_names, str_names, proList, preTime, postTime, frequency, beginTime, stopTime, filePath);
        default:
            return nullptr;
    }
}

#endif //POSTMORTEMLOGGER_PMLRECORD_H
