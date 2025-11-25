//
// Created by 79933 on 2025/8/29.
//

#include <random>
#include <thread>

#include "PMLCircularMappedBuffer.h"
#include "gtest/gtest.h"

#ifdef _WIN32
#define FILE_DOUBLE_PATH "D:/test/mmap_double_input_test"
#define FILE_STRING_PATH "D:/test/mmap_string_input_test"
#else
#define FILE_DOUBLE_PATH "/home/disheng/WorkProject/test/mmap_double_input_test"
#define FILE_STRING_PATH "/home/disheng/WorkProject/test/mmap_string_input_test"
#endif


using namespace std;

/*
 * Buffer类用于缓存callback()函数捕获的数据，同时读取其中的目标数据
 * 所以在测试Buffer类时，主要针对缓存的特性进行测试
 *
 * 1、测试输入
 * 2、测试记录是否脱漏
 * 3、测试读取数据是否脱漏
 * 4、测试线程调用是否顺畅
 */

void GetError(int32_t &errorCode){
    EXPECT_NE(errorCode, 5001) << "filePath is error" << endl;
    EXPECT_NE(errorCode, 5002) << "file size set failed" << endl;
    EXPECT_NE(errorCode, 5003) << "file mmap is error" << endl;
    EXPECT_NE(errorCode, 5004) << "input data channel size is error" << endl;
    EXPECT_NE(errorCode, 5005) << "mmap window create error" << endl;
    EXPECT_EQ(errorCode, 0) << "another error" << endl;
}

void GenerateRandomDouble(vector<double> &vec_dbl){
    mt19937 rng(random_device{}());
    uniform_real_distribution<double> dist(-10000, 10000);
    generate(vec_dbl.begin(), vec_dbl.end(), [&](){ return dist(rng); });
}

TEST(random, test){
    vector<double> vec_dbl;
    vec_dbl.resize(10);
    GenerateRandomDouble(vec_dbl);
    for(const auto &it : vec_dbl)
        cout << it << ' ';
    cout << endl;
    GenerateRandomDouble(vec_dbl);
    for(const auto &it : vec_dbl)
        cout << it << ' ';
};

TEST(BufferDouble, createTest){
    uint16_t chnNum = 1000;
    uint16_t frequency = 1000;
    uint16_t recordTime = 10;
    string filePath = "D:/test/mmap_double_input_test";

    auto ptr = make_shared<CircularMappedBufferDBL>(chnNum, frequency, recordTime, filePath);
    auto errorCord = ptr->GetErrorCord();
    GetError(errorCord);
}

TEST(CPP, test){
#include <iostream>
    std::cout << __cplusplus << std::endl;
}

//测试读取时支持的最大窗口
TEST(BufferDouble, boundaryTest){
    uint16_t chnNum = 1000;
    uint16_t frequency = 1000;
    uint16_t recordTime = 10;
    string filePath = FILE_DOUBLE_PATH;

    auto ptr = make_shared<CircularMappedBufferDBL>(chnNum, frequency, recordTime, filePath);
    auto errorCord = ptr->GetErrorCord();
    GetError(errorCord);

    vector<vector<double>> vec_dbls;
    vector<double> vec_dbl;
    auto beginTime = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(chrono::system_clock::now()));
    chrono::system_clock::time_point timePoint;
    char toc_dbl[8] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    vec_dbl.resize(chnNum + 2);

    for(uint32_t i = 0; i < 1000; i++){
        GenerateRandomDouble(vec_dbl);
        memcpy(&vec_dbl[0], toc_dbl, sizeof(double));
        timePoint = beginTime + chrono::milliseconds(i);
        vec_dbl[1] = serialize_time_point(timePoint);
        vec_dbls.push_back(vec_dbl);
        ptr->WriteData(vec_dbl);
    }

    vector<vector<double>> vec_dbls_read;

    auto handle = ptr->GetReadFileOffset(beginTime, timePoint);
    ptr->ReadData(handle, vec_dbls_read, 600);
}

class BufferDoubleTest : public ::testing::Test{
protected:
    //配置输入参数
    uint16_t chnNum = 1000;
    uint16_t frequency = 1000;
    uint16_t recordTime = 10;
    string filePath = FILE_DOUBLE_PATH;
    shared_ptr<CircularMappedBufferDBL> ptr;

    void SetUp() override{
        ptr = make_shared<CircularMappedBufferDBL>(chnNum, frequency, recordTime, filePath);
        auto errorCode = ptr->GetErrorCord();
        //判断返回值是否报错
        GetError(errorCode);
    }

    void TearDown() override{}
};

//测试输入不符合规范是否报错
TEST_F(BufferDoubleTest, inputTest){
    vector<double> vec_dbl_error;
    vec_dbl_error.reserve(chnNum);
    for(uint32_t i = 0; i < chnNum; i++)
        vec_dbl_error.push_back(0.123456);
    ptr->WriteData(vec_dbl_error);
    EXPECT_EQ(ptr->GetErrorCord(), 5004);

    ptr->ResetErrorCord();

    vector<double> vec_dbl_right;
    vec_dbl_right.reserve(chnNum + 2);
    for(uint32_t i = 0; i < chnNum + 2; i++)
        vec_dbl_right.push_back(0.123456);
    ptr->WriteData(vec_dbl_right);
    EXPECT_EQ(ptr->GetErrorCord(), 0);
}

TEST_F(BufferDoubleTest, valueTest){
    vector<vector<double>> vec_dbls;
    vector<double> vec_dbl;
    auto beginTime = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(chrono::system_clock::now()));
    chrono::system_clock::time_point timePoint;
    char toc_dbl[8] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    vec_dbl.resize(chnNum + 2);

    for(uint32_t i = 0; i < 101; i++){
        GenerateRandomDouble(vec_dbl);
        memcpy(&vec_dbl[0], toc_dbl, sizeof(double));
        timePoint = beginTime + chrono::milliseconds(i);
        vec_dbl[1] = serialize_time_point(timePoint);
        vec_dbls.push_back(vec_dbl);
        ptr->WriteData(vec_dbl);
    }

    vector<vector<double>> vec_dbls_read;

    auto handle = ptr->GetReadFileOffset(beginTime, timePoint);
    ptr->ReadData(handle, vec_dbls_read, 100);
    for(uint32_t i = 0; i < 100; i++)
        EXPECT_EQ(vec_dbls[i][i + 2], vec_dbls_read[i][i]);
}

TEST_F(BufferDoubleTest, timeLossTest){
    vector<vector<double>> vec_dbls;
    vector<double> vec_dbl;
    auto beginTime = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(chrono::system_clock::now()));
    chrono::system_clock::time_point timePoint;
    char toc_dbl[8] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    vec_dbl.resize(chnNum + 2);

    for(uint32_t i = 0; i < 101; i++){
        GenerateRandomDouble(vec_dbl);
        memcpy(&vec_dbl[0], toc_dbl, sizeof(double));
        timePoint = beginTime + chrono::milliseconds(i*2);
        vec_dbl[1] = serialize_time_point(timePoint);
        vec_dbls.push_back(vec_dbl);
        ptr->WriteData(vec_dbl);
    }

    vector<vector<double>> vec_dbls_read;

    auto handle = ptr->GetReadFileOffset(beginTime, timePoint);
    ptr->ReadData(handle, vec_dbls_read, 100);
    for(uint32_t i = 0; i < 50; i++)
        EXPECT_EQ(vec_dbls[i][i + 2], vec_dbls_read[2*i][i]);
    for(uint32_t i = 0; i < 10; i++)
        cout << vec_dbls_read[0][i] << ' ';
    cout << endl;
    for(uint32_t i = 0; i < 10; i++)
        cout << vec_dbls_read[1][i] << ' ';
    cout << endl;
    for(uint32_t i = 0; i < 10; i++)
        cout << vec_dbls_read[2][i] << ' ';
    cout << endl;
    for(uint32_t i = 0; i < 10; i++)
        cout << vec_dbls_read[3][i] << ' ';
    cout << endl;
    for(uint32_t i = 0; i < 10; i++)
        cout << vec_dbls_read[4][i] << ' ';
}

TEST_F(BufferDoubleTest, threadTest){
    vector<vector<double>> vec_dbls;
    vector<double> vec_dbl;
    auto beginTime = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(chrono::system_clock::now()));
    chrono::system_clock::time_point timePoint;
    char toc_dbl[8] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    vec_dbl.resize(chnNum + 2);

    for(uint32_t i = 0; i < 100; i++){
        GenerateRandomDouble(vec_dbl);
        memcpy(&vec_dbl[0], toc_dbl, sizeof(double));
        timePoint = beginTime + chrono::milliseconds(i);
        vec_dbl[1] = serialize_time_point(timePoint);
        vec_dbls.push_back(vec_dbl);
        ptr->WriteData(vec_dbl);
    }
    vector<vector<double>> vec_dbls_read0;
    vector<vector<double>> vec_dbls_read1;

    thread t0([&]{
        auto handle = ptr->GetReadFileOffset(beginTime, timePoint);
        ptr->ReadData(handle, vec_dbls_read0, 100);

    });
    thread t1([&]{
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        auto handle = ptr->GetReadFileOffset(beginTime, timePoint);
        ptr->ReadData(handle, vec_dbls_read1, 100);
    });

    t0.join();
    t1.join();

    for(uint32_t i = 0; i < 100; i++)
        EXPECT_EQ(vec_dbls[i][i + 2], vec_dbls_read0[i][i]);

    for(uint32_t i = 0; i < 100; i++)
        EXPECT_EQ(vec_dbls[i][i + 2], vec_dbls_read1[i][i]);
}

using namespace std;

string random_string(size_t length,
                     string_view alphabet =
                     "0123456789"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                     "abcdefghijklmnopqrstuvwxyz") {
    if (alphabet.empty()) return "";

    // 线程安全且只初始化一次
    thread_local mt19937_64 rng{ random_device{}() };
    uniform_int_distribution<size_t> dist(0, alphabet.size() - 1);

    string s;
    s.resize(length);
    for (size_t i = 0; i < length; ++i) {
        s[i] = alphabet[dist(rng)];  // 无取模偏差
    }
    return s;
}

void GenerateRandomString(vector<string> &vec_str){
    generate(vec_str.begin(), vec_str.end(), [&] { return random_string(16); });
}

TEST(test, random) {
    vector<string> vec_str;
    vec_str.resize(10);
    GenerateRandomString(vec_str);
    for(const auto &it : vec_str)
        cout << it << ' ';
}

class BufferStringTest : public ::testing::Test{
protected:
    uint16_t chnNum = 1000;
    uint16_t frequency = 1000;
    uint16_t recordTime = 10;
    string filePath = FILE_STRING_PATH;
    shared_ptr<CircularMappedBufferSTR> ptr;

    void SetUp() override{
        ptr = make_shared<CircularMappedBufferSTR>(chnNum, frequency, recordTime, filePath);
        auto errorCode = ptr->GetErrorCode();
        GetError(errorCode);
    }

    void TearDown() override{}
};

TEST_F(BufferStringTest, inputTest){
    vector<string> vec_str_error;
    vec_str_error.reserve(chnNum);
    for(uint32_t i = 0; i < chnNum; i++)
        vec_str_error.push_back("123456");
    ptr->WriteData(vec_str_error);
    EXPECT_EQ(ptr->GetErrorCode(), 5004);

    ptr->ResetErrorCode();

    vector<string> vec_str_right;
    vec_str_right.reserve(chnNum + 1);
    for(uint32_t i = 0; i < chnNum + 1; i++)
        vec_str_right.push_back("123456");
    ptr->WriteData(vec_str_right);
    EXPECT_EQ(ptr->GetErrorCode(), 0);
}

TEST_F(BufferStringTest, valueTest){
    vector<vector<string>> vec_strs;
    vector<string> vec_str;
    auto beginTime = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(chrono::system_clock::now()));
    chrono::system_clock::time_point timePoint;
    char toc_str[8] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    vec_str.resize(chnNum + 1);

    for(uint32_t i = 0; i < 101; i++){
        GenerateRandomString(vec_str);
        timePoint = beginTime + chrono::milliseconds(i);
        vec_str[0] = TransformTimeToStr(timePoint);
        vec_strs.push_back(vec_str);
        ptr->WriteData(vec_str);
    }

    vector<vector<string>> vec_strs_read;

    auto handle = ptr->GetReadFileOffset(beginTime, timePoint);
    ptr->ReadData(handle, vec_strs_read, 100);
    for(uint32_t i = 0; i < 100; i++)
        EXPECT_EQ(string_view(vec_strs[i][i + 1]).substr(0, 16), string_view(vec_strs_read[i][i]).substr(0,16));
}

TEST_F(BufferStringTest, timeLossTest){
    vector<vector<string>> vec_strs;
    vector<string> vec_str;
    auto beginTime = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(chrono::system_clock::now()));
    chrono::system_clock::time_point timePoint;
    char toc_str[8] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    vec_str.resize(chnNum + 1);

    for(uint32_t i = 0; i < 101; i++){
        GenerateRandomString(vec_str);
        timePoint = beginTime + chrono::milliseconds(2*i);
        vec_str[0] = TransformTimeToStr(timePoint);
        vec_strs.push_back(vec_str);
        ptr->WriteData(vec_str);
    }

    vector<vector<string>> vec_strs_read;

    auto handle = ptr->GetReadFileOffset(beginTime, timePoint);
    ptr->ReadData(handle, vec_strs_read, 100);
    for(uint32_t i = 0; i < 50; i++)
        EXPECT_EQ(string_view(vec_strs[i][i + 1]).substr(0, 16), string_view(vec_strs_read[2*i][i]).substr(0,16));
//    for(uint32_t i = 0; i < 10; i++)
//        cout << string_view(vec_strs_read[0][i]).substr(0,16) << ' ';
//    cout << endl;
//    for(uint32_t i = 0; i < 10; i++)
//        cout << string_view(vec_strs_read[1][i]).substr(0,16) << ' ';
//    cout << endl;
//    for(uint32_t i = 0; i < 10; i++)
//        cout << string_view(vec_strs_read[2][i]).substr(0,16) << ' ';
//    cout << endl;
//    for(uint32_t i = 0; i < 10; i++)
//        cout << string_view(vec_strs_read[3][i]).substr(0,16) << ' ';
//    cout << endl;
//    for(uint32_t i = 0; i < 10; i++)
//        cout << string_view(vec_strs_read[4][i]).substr(0,16) << ' ';
}

TEST_F(BufferStringTest, threadTest){
    vector<vector<string>> vec_strs;
    vector<string> vec_str;
    auto beginTime = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(chrono::system_clock::now()));
    chrono::system_clock::time_point timePoint;
    char toc_str[8] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    vec_str.resize(chnNum + 1);

    for(uint32_t i = 0; i < 101; i++){
        GenerateRandomString(vec_str);
        timePoint = beginTime + chrono::milliseconds(i);
        vec_str[0] = TransformTimeToStr(timePoint);
        vec_strs.push_back(vec_str);
        ptr->WriteData(vec_str);
    }

    thread t0([&] {
        vector<vector<string>> vec_strs_read;
        auto handle = ptr->GetReadFileOffset(beginTime, timePoint);
        ptr->ReadData(handle, vec_strs_read, 100);
        for(uint32_t i = 0; i < 100; i++)
            EXPECT_EQ(string_view(vec_strs[i][i + 1]).substr(0, 16), string_view(vec_strs_read[i][i]).substr(0,16));
    });

    thread t1([&] {
        this_thread::sleep_for(chrono::milliseconds(100));
        vector<vector<string>> vec_strs_read;
        auto handle = ptr->GetReadFileOffset(beginTime, timePoint);
        ptr->ReadData(handle, vec_strs_read, 100);
        for(uint32_t i = 0; i < 100; i++)
            EXPECT_EQ(string_view(vec_strs[i][i + 1]).substr(0, 16), string_view(vec_strs_read[i][i]).substr(0,16));
    });

    t0.join();
    t1.join();
}