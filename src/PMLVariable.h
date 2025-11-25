//
// Created by 79933 on 2025/4/2.
//

#ifndef POSTMORTEMLOGGER_PMLVARIABLE_H
#define POSTMORTEMLOGGER_PMLVARIABLE_H

#include <thread>
#include <unordered_map>

#include "PMLTypes.h"
#include "PMLCircularMappedBuffer.h"
#include "PMLRecord.h"
#include "varmanager/varmanager.h"
#include "serverlogger/logger.h"

class Variable {
private:
    void *con;
    std::vector<std::string> varDblList;
    std::vector<std::string> varStrList;
    std::vector<std::string> chnDblList;
    std::vector<std::string> chnStrList;
    std::vector<Property> proList;
    uint16_t preTime;
    uint16_t postTime;
    uint16_t frequency;
    RecordType fileType;
    int32_t id_double;
    int32_t id_string;
    int32_t id_InShutdown;
    std::shared_ptr<CircularMappedBufferDBL> dblBufferPtr;
    std::shared_ptr<CircularMappedBufferSTR> strBufferPtr;
    std::vector<double> dbl_data;
    std::vector<std::string> str_data;
    std::unordered_map<std::string, uint32_t> id_dbl;
    std::unordered_map<std::string, uint32_t> id_str;
    std::chrono::system_clock::time_point beginTime;
    std::chrono::system_clock::time_point stopTime;
    std::chrono::system_clock::time_point posTime_dbl;
    std::chrono::system_clock::time_point posTime_str;
    char toc[8]{'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    bool isShutdown = false;

public:
    //double数据回调函数，用于记录变化的double数据
    static void callback_double(event_data_t data, void *p) {
        auto ptr = static_cast<Variable *>(p);
        ptr->dbl_data[ptr->id_dbl[data.name]] = data.value_dbl;
        if (TransformTime(&data.time) >= ptr->posTime_dbl + std::chrono::microseconds(1'000'000 / ptr->frequency)) {
            ptr->posTime_dbl = std::chrono::system_clock::time_point(std::chrono::floor<std::chrono::microseconds>(TransformTime(&data.time)));
            ptr->dbl_data[1] = serialize_time_point(ptr->posTime_dbl);
            ptr->dblBufferPtr->WriteData(ptr->dbl_data);
        }
    }

    //string数据回调函数，用于记录变化的string数据
    static void callback_string(event_data_t data, void *p) {
        auto ptr = static_cast<Variable *>(p);
        ptr->str_data[ptr->id_str[data.name]] = data.value_str;
        if (TransformTime(&data.time) >= ptr->posTime_str + std::chrono::microseconds(1'000'000 / ptr->frequency)) {
            ptr->posTime_str = std::chrono::system_clock::time_point(std::chrono::floor<std::chrono::microseconds>(TransformTime(&data.time)));
            ptr->str_data[0] = TransformTimeToStr(ptr->posTime_str);
//            ptr->strBufferPtr->WriteData(ptr->str_data);
        }
    }

    //用于监测InShutdown的变化，记录停机情况
    static void callback_InShutdown(event_data_t data, void *p) {
        msg_log_info("InShutdown: " + std::to_string(data.value_dbl));
        auto ptr = static_cast<Variable *>(p);
        if (data.value_dbl > 0 && !ptr->isShutdown) {
            ptr->stopTime = TransformTime(&data.time);
            std::thread(&Variable::VarRecord, ptr).detach();
            ptr->isShutdown = true;
        } else if (data.value_dbl == 0) {
            ptr->isShutdown = false;
        }
    }

    explicit Variable(std::vector<std::string> &varDblList, std::vector<std::string> &varStrList, std::vector<std::string> &chnDblList, std::vector<std::string> &chnStrList,
                      void *con, RecordType fileType, uint16_t preTime, uint16_t postTime, uint16_t frequency) :
                      varDblList(varDblList), varStrList(varStrList), chnDblList(chnDblList), chnStrList(chnStrList),
                      con(con), fileType(fileType), preTime(preTime), postTime(postTime), frequency(frequency){
        std::cout << "Variable begin set config" << std::endl;
        //没有输入通道
        if (varDblList.empty() && varStrList.empty())
            return;
            //只输入string通道
        else if (varDblList.empty()) {
            std::cout << "string data" << std::endl;
            str_data.resize(varStrList.size() + 1);
            std::string str_names;
            str_names.reserve(varStrList.size());

            for (uint32_t i = 0; i < varStrList.size(); i++) {
                str_names += varStrList[i] + ',';
                id_str[varStrList[i]] = i + 1;
                char str[64]{};
                con_get_value_str(con, varStrList[i].c_str(), str);
                str_data[i + 1] = str;
            }
            str_names.pop_back();
#ifdef _WIN32
            strBufferPtr = std::make_shared<CircularMappedBufferSTR>(varStrList.size(), frequency, max(preTime, postTime), "D:/test/sliding_mmap_string");
#else
            strBufferPtr = std::make_shared<CircularMappedBufferSTR>(varStrList.size(), frequency, std::max(preTime, postTime), "/mnt/d/test/sliding_mmap_string");
#endif
            beginTime = std::chrono::system_clock::now();
            posTime_dbl = beginTime;
            str_data[0] = TransformTimeToStr(beginTime);
            strBufferPtr->WriteData(str_data);

            con_add_callback(con, str_names.c_str(), 0, &id_double, callback_string, this);
            con_add_callback(con, "InShutdown", 0, &id_InShutdown, callback_InShutdown, this);
            //只输入double通道
        } else if (varStrList.empty()) {
            std::cout << "double data" << std::endl;
            dbl_data.resize(varDblList.size() + 2);
            memcpy(&dbl_data[0], toc, sizeof(double));
            std::string dbl_names;
            dbl_names.reserve(varDblList.size());

            for (uint32_t i = 0; i < varDblList.size(); i++) {
                dbl_names += varDblList[i] + ',';
                id_dbl[varDblList[i]] = i + 2;
                con_get_value_dbl(con, varDblList[i].c_str(), &dbl_data[i + 2]);
            }
            dbl_names.pop_back();
#ifdef _WIN32
            dblBufferPtr = std::make_shared<CircularMappedBufferDBL>(varDblList.size(), frequency, max(preTime, postTime), "D:/test/sliding_mmap_double");
#else
            dblBufferPtr = std::make_shared<CircularMappedBufferDBL>(varDblList.size(), frequency, std::max(preTime, postTime), "/mnt/d/test/sliding_mmap_double");
            std::cout << "finish create buffer" << std::endl;
#endif
            beginTime = std::chrono::system_clock::now();
            posTime_str = beginTime;
            dbl_data[1] = serialize_time_point(beginTime);
            dblBufferPtr->WriteData(dbl_data);

            con_add_callback(con, dbl_names.c_str(), 0, &id_double, callback_double, this);
            con_add_callback(con, "InShutdown", 0, &id_InShutdown, callback_InShutdown, this);
            msg_log_info("finish created");
            //同时输入两种通道
        } else {
            std::cout << "both double and string" << std::endl;
            dbl_data.resize(varDblList.size() + 2);
            memcpy(&dbl_data[0], toc, sizeof(double));
            str_data.resize(varStrList.size() + 1);
            std::string dbl_names;
            std::string str_names;
            //如何预估string总长度
            dbl_names.reserve(varDblList.size());
            str_names.reserve(varStrList.size());

            for (uint32_t i = 0; i < varDblList.size(); i++) {
                dbl_names += varDblList[i] + ',';
                id_dbl[varDblList[i]] = i + 2;
                con_get_value_dbl(con, varDblList[i].c_str(), &dbl_data[i + 2]);
            }
            dbl_names.pop_back();
            for (uint32_t i = 0; i < varStrList.size(); i++) {
                str_names += varStrList[i] + ',';
                id_str[varStrList[i]] = i + 1;
                char str[64]{};
                con_get_value_str(con, varStrList[i].c_str(), str);
                str_data[i + 1] = str;
            }
            str_names.pop_back();

#ifdef _WIN32
            dblBufferPtr = std::make_shared<CircularMappedBufferDBL>(varDblList.size(), frequency, max(preTime, postTime), "D:/test/sliding_mmap_double");
            strBufferPtr = std::make_shared<CircularMappedBufferSTR>(varStrList.size(), frequency, max(preTime, postTime), "D:/test/sliding_mmap_string");
#else
            dblBufferPtr = std::make_shared<CircularMappedBufferDBL>(varDblList.size(), frequency, std::max(preTime, postTime), "/mnt/d/test/sliding_mmap_double");
            strBufferPtr = std::make_shared<CircularMappedBufferSTR>(varStrList.size(), frequency, std::max(preTime, postTime), "/mnt/d/test/sliding_mmap_string");
#endif
            beginTime = std::chrono::system_clock::now();
            posTime_dbl = beginTime;
            posTime_str = beginTime;
            dbl_data[1] = serialize_time_point(beginTime);
            dblBufferPtr->WriteData(dbl_data);
            str_data[0] = TransformTimeToStr(beginTime);
            strBufferPtr->WriteData(str_data);

            con_add_callback(con, dbl_names.c_str(), 0, &id_double, callback_double, this);
            con_add_callback(con, str_names.c_str(), 0, &id_string, callback_string, this);
            con_add_callback(con, "InShutdown", 0, &id_InShutdown, callback_InShutdown, this);
        }
    }

    //用于记录停机前后数据
    void VarRecord() {
        std::chrono::system_clock::time_point begin = std::chrono::system_clock::now();
        stopTime = std::chrono::floor<std::chrono::milliseconds>(stopTime);
        std::chrono::system_clock::time_point startTime;
        if (beginTime < stopTime - std::chrono::seconds(preTime))
            startTime = stopTime - std::chrono::seconds(preTime);
        else
            startTime = beginTime;
        std::cout << "startTime: " << TransformStrTimeMS(startTime) << std::endl;
        std::cout << "beginTime: " << TransformStrTimeMS(beginTime) << std::endl;
        std::cout << "stopTime: " << TransformStrTimeMS(stopTime) << std::endl;
        std::shared_ptr<Record> recordPtr;
#ifdef _WIN32
        std::string filePath = "D:/test/";
#else
        std::string filePath = "/mnt/d/test/";
#endif
        filePath += TransformStrTimeMS(stopTime);
        recordPtr = createRecord(this->fileType, preTime, postTime, frequency, beginTime, stopTime, filePath, chnDblList, chnStrList, proList);
        recordPtr->create_file();

        //使用线程同时运行，加快查找Offset的速度
        int32_t dblHandle = 0;
        int32_t strHandle = 0;
        std::thread t0([&] {
            if (dblBufferPtr) {
                dblHandle = dblBufferPtr->GetReadFileOffset(startTime, stopTime);
            }
        });
        std::thread t1([&] {
            if (strBufferPtr)
                strHandle = strBufferPtr->GetReadFileOffset(startTime, stopTime);
            std::cout << strHandle << std::endl;
        });
        t0.join();
        t1.join();
        std::cout << "finish get offset" << std::endl;

        //准备缓存接收数据
        std::vector<std::chrono::system_clock::time_point> vec_time;
        std::vector<std::vector<double>> vec_dbl;
        std::vector<std::vector<std::string>> vec_str;
        vec_time.reserve(frequency);
        vec_dbl.reserve(frequency);
        vec_str.reserve(frequency);

        //处理pre数据
        //计算记录总时长
        int32_t preCount = std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count();

        if (preCount > preTime * 1000) {
            for (uint32_t i = 0; i < preTime; i++) {
                //清空缓存区
                vec_time.clear();
                vec_dbl.clear();
                vec_str.clear();

                //读取缓存数据
                if (dblBufferPtr)
                    dblBufferPtr->ReadData(dblHandle, vec_dbl, frequency);
                if (strBufferPtr)
                    strBufferPtr->ReadData(strHandle, vec_str, frequency);

                for (uint32_t j = 0; j < frequency; j++)
                    vec_time.push_back(stopTime - std::chrono::seconds(preTime - i) + std::chrono::microseconds(j * 1'000'000 / frequency));

                //写入输出文件
                recordPtr->set_data(vec_time, vec_dbl, vec_str);
                recordPtr->save();
            }
        } else {
            uint16_t cycleTimes = preCount / 1000;
            if (preCount % 1000)
                cycleTimes++;
            for (uint32_t i = 0; i < cycleTimes; i++) {
                //清空缓存区
                vec_time.clear();
                vec_dbl.clear();
                vec_str.clear();

                if (preCount - 1000 > 0) {
                    preCount -= 1000;
                    //记录时间数据
                    for (uint32_t j = 0; j < frequency; j++)
                        vec_time.push_back(startTime + std::chrono::seconds(i) + std::chrono::microseconds(j * 1'000'000 / frequency));
                    //读取缓存数据
                    if (dblBufferPtr)
                        dblBufferPtr->ReadData(dblHandle, vec_dbl, frequency);
                    if (strBufferPtr)
                        strBufferPtr->ReadData(strHandle, vec_str, frequency);
                } else {
                    //记录时间数据
                    uint16_t timeSize = static_cast<uint16_t>(preCount * static_cast<double>(frequency) / 1000);
                    for (uint32_t j = 0; j < timeSize; j++)
                        vec_time.push_back(startTime + std::chrono::seconds(i) + std::chrono::microseconds(j * 1'000'000 / frequency));
                    //读取缓存数据
                    if (dblBufferPtr)
                        dblBufferPtr->ReadData(dblHandle, vec_dbl, timeSize);
                    if (strBufferPtr)
                        strBufferPtr->ReadData(strHandle, vec_str, timeSize);
                }

                //写入输出文件
                recordPtr->set_data(vec_time, vec_dbl, vec_str);
                recordPtr->save();
            }
        }

        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        auto spendTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

        std::cout << "finish pre" << std::endl;
        //等待数据记录完成
        std::this_thread::sleep_for(std::chrono::milliseconds(postTime * 1000 - spendTime));

        //设置post输入（仅对于TDMS）
        recordPtr->set_isPre(false);

        //处理post数据
        for (uint32_t i = 0; i < postTime; i++) {
            //清空缓存区
            vec_time.clear();
            vec_dbl.clear();
            vec_str.clear();

            //读取缓存数据
            if (dblBufferPtr)
                dblBufferPtr->ReadData(dblHandle, vec_dbl, frequency);
            if (strBufferPtr)
                strBufferPtr->ReadData(dblHandle, vec_str, frequency);

            uint32_t dataSize = 0;
            if (dblBufferPtr && strBufferPtr) {
#ifdef _WIN32
                dataSize = min(vec_dbl.size(), vec_str.size());
#else
                dataSize = std::min(vec_dbl.size(), vec_str.size());
#endif
                vec_dbl.resize(dataSize);
                vec_str.resize(dataSize);
            } else if (dblBufferPtr) {
                dataSize = vec_dbl.size();
            } else if (strBufferPtr) {
                dataSize = vec_str.size();
            }

            //记录时间数据
            for (uint32_t j = 0; j < dataSize; j++)
                vec_time.push_back(stopTime + std::chrono::seconds(i) + std::chrono::microseconds(j * 1'000'000 / frequency));

            //写入输出文件
            recordPtr->set_data(vec_time, vec_dbl, vec_str);
            recordPtr->save();
        }

        std::cout << "finish var record" << std::endl;
    }

    ~Variable() {
        //释放句柄
        close_event(id_double);
        close_event(id_string);
        close_event(id_InShutdown);
    }
};

#endif //POSTMORTEMLOGGER_PMLVARIABLE_H