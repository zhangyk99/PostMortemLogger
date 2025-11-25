//
// Created by 79933 on 2025/3/27.
//

#include <windows.h>
#include <thread>
#include <chrono>
#include <deque>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>
#include <future>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <string>

#include "varmanager/varmanager.h"
#include "gtest/gtest.h"
//#include "PMLDevice.h"
#include "PMLVariable.h"

using namespace std;

#include <queue>
#include <utility>

//TEST(MDF3, test) {
//    using namespace mdf;
//    auto writer = MdfFactory::CreateMdfWriter(MdfWriterType::Mdf3Basic);
//
//    writer->Init("D:/test/MDF3_test.mdf");
//
//    auto *header = writer->Header();
//    ASSERT_TRUE(header != nullptr);
//    header->Author("Ingemar Hedvall");
//    header->Department("Home Alone");
//    header->Description("Testing Sample");
//    header->Project("Mdf3WriteHD");
//    header->StartTime(TimeStampToNs());
//    header->Subject("PXY");
//
//    auto *data_group = writer->CreateDataGroup();
//    auto *group = writer->CreateChannelGroup(data_group);
//    group->Name("PML_test");
//
//    std::vector<IChannel *> chnList;
//    chnList.reserve(1'000);
//
//    for (size_t i = 0; i < 1000; i++) {
//        auto *chn = group->CreateChannel();
//        chn->Name("var_dbl" + to_string(i));
//        chn->Type(ChannelType::FixedLength);
//        chn->DataType(ChannelDataType::FloatLe);
//        chn->DataBytes(4);
//        chnList.push_back(chn);
//    }
//
//    writer->InitMeasurement();
//    auto tick_time = TimeStampToNs();
//    writer->StartMeasurement(TimeStampToNs());
//    for (size_t sample = 0; sample < 100; ++sample) {
//        auto cn_list = group->Channels();
//        double value = 0.01 * static_cast<double>(sample);
//        for (auto *channel: cn_list) {
//            channel->SetChannelValue(value);
//        }
//        writer->SaveSample(*group, TimeStampToNs());
//        tick_time += 1'000'000;
//    }
//    writer->StopMeasurement(tick_time);
//    writer->FinalizeMeasurement();
//}

//TEST(MDF4, test) {
//    using namespace mdf;
//    auto writer = MdfFactory::CreateMdfWriter(MdfWriterType::Mdf4Basic);
//    writer->Init("D:/test/MDF4_test.mf4");
//
//    auto *header = writer->Header();
//    auto *history = header->CreateFileHistory();
//    history->Description("Test data types");
//    history->ToolName("MdfWrite");
//    history->ToolVendor("ACME Road Runner Company");
//    history->ToolVersion("1.0");
//    history->UserName("Ingemar Hedvall");
//
//    auto *data_group = header->CreateDataGroup();
//
//    auto *group1 = data_group->CreateChannelGroup();
//    group1->Name("FloatMaster");
//
//    auto *master1 = group1->CreateChannel();
//    master1->Name("Time");
//    master1->Type(ChannelType::Master);
//    master1->Sync(ChannelSyncType::Time);
//    master1->DataType(ChannelDataType::FloatLe);
//    master1->DataBytes(4);
//
//    auto *ch1 = group1->CreateChannel();
//    ch1->Name("Intel32");
//    ch1->Type(ChannelType::FixedLength);
//    ch1->Sync(ChannelSyncType::None);
//    ch1->DataType(ChannelDataType::FloatLe);
//    ch1->DataBytes(4);
//
//    auto *group2 = data_group->CreateChannelGroup();
//    group1->Name("DoubleMaster");
//
//    auto *master2 = group2->CreateChannel();
//    master2->Name("Time");
//    master2->Type(ChannelType::Master);
//    master2->Sync(ChannelSyncType::Time);
//    master2->DataType(ChannelDataType::FloatBe);
//    master2->DataBytes(8);
//
//    auto *ch2 = group2->CreateChannel();
//    ch2->Name("Intel32");
//    ch2->Type(ChannelType::FixedLength);
//    ch2->Sync(ChannelSyncType::None);
//    ch2->DataType(ChannelDataType::FloatLe);
//    ch2->DataBytes(4);
//
//    auto *group3 = data_group->CreateChannelGroup();
//    group3->Name("Signed32Master");
//
//    auto *master3 = group3->CreateChannel();
//    master3->Name("Time");
//    master3->Unit("s");
//    master3->Type(ChannelType::Master);
//    master3->Sync(ChannelSyncType::Time);
//    master3->DataType(ChannelDataType::SignedIntegerLe);
//    master3->DataBytes(4);
//
//    auto cc3 = master3->CreateChannelConversion();
//    cc3->Name("NsToS");
//    cc3->Description("Nanoseconds to seconds conversion");
//    cc3->Unit("s");
//    cc3->Type(ConversionType::Linear);
//    cc3->Parameter(0, 0.0);
//    cc3->Parameter(1, 1E-9);
//
//    auto *ch3 = group3->CreateChannel();
//    ch3->Name("Intel32");
//    ch3->Type(ChannelType::FixedLength);
//    ch3->Sync(ChannelSyncType::None);
//    ch3->DataType(ChannelDataType::FloatLe);
//    ch3->DataBytes(4);
//
//    auto *group4 = data_group->CreateChannelGroup();
//    group4->Name("Signed64Master");
//
//    auto *master4 = group4->CreateChannel();
//    master4->Name("Time");
//    master4->Unit("s");
//    master4->Type(ChannelType::Master);
//    master4->Sync(ChannelSyncType::Time);
//    master4->DataType(ChannelDataType::SignedIntegerLe);
//    master4->DataBytes(8);
//
//    auto cc4 = master4->CreateChannelConversion();
//    cc4->Name("NsToS");
//    cc4->Description("Nanoseconds to seconds conversion");
//    cc4->Unit("s");
//    cc4->Type(ConversionType::Linear);
//    cc4->Parameter(0, 0.0);
//    cc4->Parameter(1, 1E-9);
//
//    auto *ch4 = group4->CreateChannel();
//    ch4->Name("Intel32");
//    ch4->Type(ChannelType::FixedLength);
//    ch4->Sync(ChannelSyncType::None);
//    ch4->DataType(ChannelDataType::FloatLe);
//    ch4->DataBytes(4);
//
//    writer->PreTrigTime(2.0); // Store one seconds samples before start
//    writer->InitMeasurement(); // Start the internal cache
//    auto tick_time = TimeStampToNs();
//
//    // Fill cache with 1 second data
//    uint64_t sample = 0;
//    for (; sample < 1000; ++sample) {
//        const auto value = static_cast<uint32_t>(sample);
//        ch1->SetChannelValue(value);
//        ch2->SetChannelValue(value);
//        ch3->SetChannelValue(value);
//        ch4->SetChannelValue(value);
//        writer->SaveSample(*group1, tick_time);
//        writer->SaveSample(*group2, tick_time);
//        writer->SaveSample(*group3, tick_time);
//        writer->SaveSample(*group4, tick_time);
//        tick_time += 2'000'000; // Stepping 1ms
//    }
//
//    writer->StartMeasurement(tick_time);
//
//    // Fill with 1 seconds samples after start.
//    for (; sample < 2000; ++sample) {
//        const auto value = static_cast<uint32_t>(sample);
//        ch1->SetChannelValue(value);
//        ch2->SetChannelValue(value);
//        ch3->SetChannelValue(value);
//        ch4->SetChannelValue(value);
//        writer->SaveSample(*group1, tick_time);
//        writer->SaveSample(*group2, tick_time);
//        writer->SaveSample(*group3, tick_time);
//        writer->SaveSample(*group4, tick_time);
//        tick_time += 2'000'000;
//    }
//    writer->StopMeasurement(tick_time);
//    writer->FinalizeMeasurement();
//}

TEST(TDMS_test, test) {
    string file_path{"D:/test/TDMS_test.tdms"};
    void *fileHandle;
    open_file(file_path.c_str(), "w", fileHandle);
    void *groupHandle;
    create_group(fileHandle, "group", groupHandle);

    vector<double> vec_data;
    vec_data.reserve(10000);
    for (size_t i = 0; i < 10000; i++)
        vec_data.push_back(0.123456 + i);
    void *channelHandle;
    for (size_t i = 0; i < 5000; i++) {
        create_channel(groupHandle, ("channel_" + to_string(i)).c_str(), channelHandle);
        set_channel_data_double(channelHandle, vec_data.size(), vec_data.data());
    }
    save_file(fileHandle);
    close_file(fileHandle);
}

class BufferDBL {
private:
    uint32_t chnNum;
    uint16_t frequency;
    uint16_t recordTime;
    HANDLE fileHandle;
    HANDLE mapHandle;
    uint64_t fileSize = 0;
    uint64_t fileOffset = 0;
    uint64_t fileCount = 0;
    uint32_t windowSize = 64 * 1024;
    uint32_t windowOffset = 0;
    void *windowsMap = nullptr;

public:
    BufferDBL(string &filePath, uint32_t chnNum, uint16_t frequency, uint16_t recordTime) :
            chnNum(chnNum), frequency(frequency), recordTime(recordTime) {
        fileHandle = CreateFileA(
                filePath.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                nullptr,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
        );
        if (fileHandle == INVALID_HANDLE_VALUE) {
            cerr << "CreateFile failed: " << GetLastError() << endl;
            return;
        }

        uint32_t preSize = windowSize / (chnNum * sizeof(double));
        fileSize = static_cast<uint64_t>(frequency * recordTime / preSize * 1.2 + 1) * windowSize;

        LARGE_INTEGER size;
        size.QuadPart = fileSize;
        if (!SetFilePointerEx(fileHandle, size, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) {
            cout << "File size set failed!" << endl;
        }

        mapHandle = CreateFileMappingA(
                fileHandle,
                nullptr,
                PAGE_READWRITE,
                DWORD(fileSize >> 32),
                DWORD(fileSize & 0xFFFFFFFF),
                nullptr
        );

        if (!mapHandle) {
            cerr << "CreateFileMapping failed: " << GetLastError() << endl;
            CloseHandle(fileHandle);
            return;
        }
    }

    void WriteData(vector<double> &vec_dbl) {
        if (vec_dbl.size() != chnNum + 2) {
            cout << "size error";
            return;
        }
        if (!windowsMap) {
            windowsMap = MapViewOfFile(
                    mapHandle,
                    FILE_MAP_ALL_ACCESS,
                    DWORD(fileOffset >> 32),
                    DWORD(fileOffset & 0xFFFFFFFF),
                    windowSize
            );
            if (!windowsMap) {
                cerr << "Initial map fail: " << GetLastError() << endl;
                return;
            }
        }

        if (windowSize - windowOffset <= chnNum * sizeof(double)) {
            auto *buffer = reinterpret_cast<double *>(windowsMap);
            memcpy(buffer + windowOffset / sizeof(double), vec_dbl.data(), windowSize - windowOffset);
            fileOffset += windowSize;
            if (fileOffset >= fileSize) {
                fileOffset = 0;
                fileCount++;
            }
            UnmapViewOfFile(windowsMap);
            windowsMap = MapViewOfFile(
                    mapHandle,
                    FILE_MAP_ALL_ACCESS,
                    DWORD(fileOffset >> 32),
                    DWORD(fileOffset & 0xFFFFFFFF),
                    windowSize
            );
            if (!windowsMap) {
                cerr << "Initial map fail: " << GetLastError() << endl;
                return;
            }
            buffer = reinterpret_cast<double *>(windowsMap);

            memcpy(buffer, vec_dbl.data() + (windowSize - windowOffset) / sizeof(double), vec_dbl.size() * sizeof(double) - windowSize + windowOffset);
            windowOffset = vec_dbl.size() * sizeof(double) - windowSize + windowOffset;
        } else {
            auto *buffer = reinterpret_cast<double *>(windowsMap);
            memcpy(buffer + windowOffset / sizeof(double), vec_dbl.data(), vec_dbl.size() * sizeof(double));
            windowOffset += (chnNum + 2) * sizeof(double);
        }
    }

    ~BufferDBL() {
        if (!windowsMap)
            UnmapViewOfFile(windowsMap);
        CloseHandle(mapHandle);
        CloseHandle(fileHandle);
    }

    HANDLE GetMapHandle() const { return mapHandle; }

    uint64_t GetFileSize() const { return fileSize; }

    uint64_t GetFileOffset() const { return fileOffset; }

    uint64_t GetFileCount() const { return fileCount; }

    uint32_t GetWindowSize() const { return windowSize; }
};

TEST(mdflibrary, test){
    std::cout << "C example" << std::endl;
    {
        std::cout << "Write" << std::endl;
        auto* Writer = MdfWriterInit(MdfWriterType::Mdf4Basic, "D:/test/test_c.mf4");
        std::cout << "Writer: " << Writer << std::endl;
        auto* Header = MdfWriterGetHeader(Writer);
        std::cout << "Header: " << Header << std::endl;
        MdfHeaderSetAuthor(Header, "Caller");
        MdfHeaderSetDepartment(Header, "Home Alone");
        MdfHeaderSetDescription(Header, "Testing i");
        MdfHeaderSetProject(Header, "Mdf3WriteHD");
        MdfHeaderSetStartTime(Header, 1000);
        MdfHeaderSetSubject(Header, "PXY");
        auto* History = MdfHeaderCreateFileHistory(Header);
        MdfFileHistorySetTime(History, 1000000);
        MdfFileHistorySetDescription(History, "Initial stuff");
        MdfFileHistorySetToolName(History, "Unit Test");
        MdfFileHistorySetToolVendor(History, "ACME");
        MdfFileHistorySetToolVersion(History, "2.3");
        MdfFileHistorySetUserName(History, "Ducky");

        {  // DataGroup 1
            auto* dg = MdfWriterCreateDataGroup(Writer);
            std::cout << "DataGroup: " << dg << std::endl;
            auto* cg1 = MdfDataGroupCreateChannelGroup(dg);
            std::cout << "ChannelGroup1: " << cg1 << std::endl;
            MdfChannelGroupSetName(cg1, "Test 1");
            MdfChannelGroupSetDescription(cg1, "Test channel group");
            auto* cg2 = MdfDataGroupCreateChannelGroup(dg);
            std::cout << "ChannelGroup2: " << cg2 << std::endl;
            MdfChannelGroupSetName(cg2, "Test 2");
            MdfChannelGroupSetDescription(cg2, "Test channel group");

            auto* si = MdfChannelGroupCreateSourceInformation(cg1);
            std::cout << "SourceInformation: " << si << std::endl;
            MdfSourceInformationSetName(si, "SI-Name");
            MdfSourceInformationSetPath(si, "SI-Path");
            MdfSourceInformationSetDescription(si, "SI-Desc");
            MdfSourceInformationSetType(si, SourceType::Bus);
            MdfSourceInformationSetBus(si, BusType::Can);

            mdf::IChannel* pChannel1[3];
            {
                pChannel1[0] = MdfChannelGroupCreateChannel(cg1);
                std::cout << "Channel: " << pChannel1[0] << std::endl;
                MdfChannelSetName(pChannel1[0], "Time");
                MdfChannelSetDescription(pChannel1[0], "Time channel");
                MdfChannelSetType(pChannel1[0], ChannelType::Master);
                MdfChannelSetSync(pChannel1[0], ChannelSyncType::Time);
                MdfChannelSetDataType(pChannel1[0], ChannelDataType::FloatLe);
                MdfChannelSetDataBytes(pChannel1[0], 4);
                MdfChannelSetUnit(pChannel1[0], "s");
                MdfChannelSetRange(pChannel1[0], 0, 100);
            }

            {
                pChannel1[1] = MdfChannelGroupCreateChannel(cg1);
                MdfChannelSetName(pChannel1[1], "SignedLe");
                MdfChannelSetDescription(pChannel1[1], "int32_t");
                MdfChannelSetType(pChannel1[1], ChannelType::FixedLength);
                MdfChannelSetDataType(pChannel1[1], ChannelDataType::SignedIntegerLe);
                MdfChannelSetDataBytes(pChannel1[1], sizeof(int32_t));
            }
            {
                pChannel1[2] = MdfChannelGroupCreateChannel(cg1);
                MdfChannelSetName(pChannel1[2], "SignedBe");
                MdfChannelSetDescription(pChannel1[2], "int8_t");
                MdfChannelSetType(pChannel1[2], ChannelType::FixedLength);
                MdfChannelSetDataType(pChannel1[2], ChannelDataType::SignedIntegerLe);
                MdfChannelSetDataBytes(pChannel1[2], sizeof(int8_t));
            }

            mdf::IChannel* pChannel2[3];
            {
                pChannel2[0] = MdfChannelGroupCreateChannel(cg2);
                std::cout << "Channel: " << pChannel2[0] << std::endl;
                MdfChannelSetName(pChannel2[0], "Time");
                MdfChannelSetDescription(pChannel2[0], "Time channel");
                MdfChannelSetType(pChannel2[0], ChannelType::Master);
                MdfChannelSetSync(pChannel2[0], ChannelSyncType::Time);
                MdfChannelSetDataType(pChannel2[0], ChannelDataType::FloatLe);
                MdfChannelSetDataBytes(pChannel2[0], 4);
                MdfChannelSetUnit(pChannel2[0], "s");
                MdfChannelSetRange(pChannel2[0], 0, 100);
            }

            {
                pChannel2[1] = MdfChannelGroupCreateChannel(cg2);
                MdfChannelSetName(pChannel2[1], "SignedLe");
                MdfChannelSetDescription(pChannel2[1], "int32_t");
                MdfChannelSetType(pChannel2[1], ChannelType::FixedLength);
                MdfChannelSetDataType(pChannel2[1], ChannelDataType::SignedIntegerLe);
                MdfChannelSetDataBytes(pChannel2[1], sizeof(int32_t));
            }
            {
                pChannel2[2] = MdfChannelGroupCreateChannel(cg2);
                MdfChannelSetName(pChannel2[2], "FloatBe");
                MdfChannelSetDescription(pChannel2[2], "double");
                MdfChannelSetType(pChannel2[2], ChannelType::FixedLength);
                MdfChannelSetDataType(pChannel2[2], ChannelDataType::FloatBe);
                MdfChannelSetDataBytes(pChannel2[2], sizeof(double));
            }

            MdfWriterInitMeasurement(Writer);
            MdfWriterStartMeasurement(Writer, 100000000);
            std::cout << "Start measure" << std::endl;
            for (size_t i = 0; i < 50; i++) {
                MdfChannelSetChannelValueAsFloat(pChannel1[1], i * 2);
                MdfChannelSetChannelValueAsFloat(pChannel1[2], i * 3);
                MdfChannelSetChannelValueAsFloat(pChannel2[1], i * 4);
                MdfChannelSetChannelValueAsFloat(pChannel2[2], i * 5);
                MdfWriterSaveSample(Writer, cg1, 100000000 + i * 10000);
                MdfWriterSaveSample(Writer, cg2, 100000000 + i * 10000);
            }
            std::cout << "Stop measure" << std::endl;
            MdfWriterStopMeasurement(Writer, 1100000000);
            MdfWriterFinalizeMeasurement(Writer);
        }
        {  // DataGroup 2
            auto* dg = MdfHeaderCreateDataGroup(Header);
            auto* cg = MdfDataGroupCreateChannelGroup(dg);
            std::cout << "ChannelGroup: " << cg << std::endl;
            MdfChannelGroupSetName(cg, "Test");
            MdfChannelGroupSetDescription(cg, "Test channel group");

            mdf::IChannel* pChannel[3];
            {
                pChannel[0] = MdfChannelGroupCreateChannel(cg);
                std::cout << "Channel: " << pChannel[0] << std::endl;
                MdfChannelSetName(pChannel[0], "Time");
                MdfChannelSetDescription(pChannel[0], "Time channel");
                MdfChannelSetType(pChannel[0], ChannelType::Master);
                MdfChannelSetSync(pChannel[0], ChannelSyncType::Time);
                MdfChannelSetDataType(pChannel[0], ChannelDataType::FloatLe);
                MdfChannelSetDataBytes(pChannel[0], 4);
                MdfChannelSetUnit(pChannel[0], "s");
                MdfChannelSetRange(pChannel[0], 0, 100);
            }

            {
                pChannel[1] = MdfChannelGroupCreateChannel(cg);
                MdfChannelSetName(pChannel[1], "SignedLe");
                MdfChannelSetDescription(pChannel[1], "int32_t");
                MdfChannelSetType(pChannel[1], ChannelType::FixedLength);
                MdfChannelSetDataType(pChannel[1], ChannelDataType::SignedIntegerLe);
                MdfChannelSetDataBytes(pChannel[1], sizeof(int32_t));
            }

            {
                pChannel[2] = MdfChannelGroupCreateChannel(cg);
                MdfChannelSetName(pChannel[2], "String");
                MdfChannelSetDescription(pChannel[2], "string");
                MdfChannelSetType(pChannel[2], ChannelType::FixedLength);
                MdfChannelSetDataType(pChannel[2], ChannelDataType::StringAscii);
                MdfChannelSetDataBytes(pChannel[2], 4);
            }

            MdfWriterInitMeasurement(Writer);
            MdfWriterStartMeasurement(Writer, 100000000);
            std::cout << "Start measure" << std::endl;
            for (size_t i = 0; i < 90; i++) {
                MdfChannelSetChannelValueAsFloat(pChannel[1], i * 2);
                MdfChannelSetChannelValueAsString(pChannel[2],
                                                  std::to_string(i * 3).c_str());
                MdfWriterSaveSample(Writer, cg, 100000000 + i * 1000);
            }
            std::cout << "Stop measure" << std::endl;
            MdfWriterStopMeasurement(Writer, 1100000000);
            MdfWriterFinalizeMeasurement(Writer);
        }
    }
}

class WriteData {

};

//class ProcessTest {
//private:
//    shared_ptr<BufferDBL> bufferPtr;
//    uint16_t chnNum;
//    uint16_t frequency;
//    uint16_t preTime;
//    uint16_t postTime;
//    chrono::system_clock::time_point stopTime;
//
//
//public:
//    int32_t data = 0;
//    int32_t num = 0;
//
//    ProcessTest(shared_ptr<BufferDBL> &buffer, uint16_t chnNum, uint16_t frequency, uint16_t preTime, uint16_t postTime) :
//            bufferPtr(buffer), chnNum(chnNum), frequency(frequency), preTime(preTime), postTime(postTime) {
//
//    }
//
//    void VarRecord(){
//        auto beginTime= stopTime - chrono::seconds(preTime);
//        auto mapHandle = bufferPtr->GetMapHandle();
//        auto windowSize = bufferPtr->GetWindowSize();
//        auto fileOffset = bufferPtr->GetFileOffset() - windowSize;
//        auto fileSize = bufferPtr->GetFileSize();
//        auto fileCount = bufferPtr->GetFileCount();
//
//        auto mapWindows = MapViewOfFile(
//                mapHandle,
//                FILE_MAP_ALL_ACCESS,
//                DWORD(fileOffset >> 32),
//                DWORD(fileOffset & 0xFFFFFFFF),
//                windowSize
//                );
//
//        auto *buffer = reinterpret_cast<double*>(mapWindows);
//        for(size_t i = 0; i < windowSize/sizeof(double); i++){
//            if(isnan(buffer[i])){
////                if(chrono::system_clock::time_point(buffer[i + 1]) == beginTime)
//            }
//        }
//    }
//};

class VarTest {
private:
    void *con;
    uint16_t frequency = 1000;
    uint16_t preTime;
    uint16_t postTime;
    vector<string> varNames;
    vector<double> beginData;
    unordered_map<string, uint32_t> varId;
    vector<vector<double>> vec_data;
    chrono::system_clock::time_point posTime;
    condition_variable cv;
    int32_t id = 0;
    mutable mutex mtx; //用于锁定primaryData;
    shared_ptr<BufferDBL> bufferPtr;

public:
    static void callback(event_data_t data, void *p) {
        auto ptr = static_cast<VarTest *>(p);
        ptr->beginData[ptr->varId[data.name]] = data.value_dbl;
        if (TransformTime(&data.time) >= ptr->posTime + chrono::microseconds(1'000'000 / ptr->frequency)) {
            ptr->posTime = ptr->posTime + chrono::microseconds(1'000'000 / ptr->frequency);
            ptr->beginData[1] = serialize_time_point(ptr->posTime);
            ptr->bufferPtr->WriteData(ptr->beginData);
            cout << TransformStrTimeMS(ptr->posTime) << endl;
        }

//        DataPointDBL dp;
//        dp.id = ptr->varId[data.name];
//        dp.timestamp = TransformTime(&data.time);
//        dp.dbl = data.value_dbl;
//        {
//            lock_guard<mutex> lock(ptr->mtx);
//            ptr->primaryData.push(dp);
//        }
//        ptr->cv.notify_one();
    }

    static void callback_InShutdown(event_data_t data, void *p) {
        auto ptr = static_cast<VarTest *>(p);
        if (data.value_dbl == 0)
            ptr->Record();
    }

    VarTest(void *con, vector<string> &varNames) : con(con), varNames(varNames) {
        string str_names;
        double dbl;
        beginData.reserve(varNames.size() + 2);
        string filePath = "D:/test/sliding_mmap.dat";
        bufferPtr = make_shared<BufferDBL>(filePath, 5000, 1000, 10);
        char str_toc[8] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
        double toc;
        memcpy(&toc, str_toc, sizeof(double));
        beginData.push_back(toc);
        beginData.push_back(chrono::duration_cast<chrono::duration<double>>(posTime.time_since_epoch()).count());
        for (size_t i = 0; i < varNames.size(); i++) {
            str_names += varNames[i] + ',';
            con_get_value_dbl(con, varNames[i].c_str(), &dbl);
            beginData.push_back(dbl);
            varId[varNames[i]] = i + 2;
        }
        str_names.pop_back();
        posTime = chrono::system_clock::time_point(floor<chrono::milliseconds>(chrono::system_clock::now()));

        cout << "beginTime: " << TransformStrTimeMS(posTime) << endl;
        con_add_callback(con, str_names.c_str(), 0, &id, callback, this);
    }

    void Record() {
        auto begin = chrono::system_clock::now();
        auto stopTime = chrono::system_clock::time_point(floor<chrono::milliseconds>(begin));
        chrono::system_clock::time_point pos_time;
        bool running = true;
        auto fileSize = bufferPtr->GetFileSize();
        void *windowsMap = nullptr;
        string mmapPath = "D:/test/sliding_mmap.dat";
        string filePath = "D:/test/csv_test.csv";
        ofstream outFile(filePath);
        HANDLE fileHandle = CreateFileA(
                mmapPath.c_str(),
                GENERIC_READ,
                0,
                nullptr,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
        );

        HANDLE mapHandle = CreateFileMappingA(
                fileHandle,
                nullptr,
                PAGE_READWRITE,
                DWORD(fileSize >> 32),
                DWORD(fileSize & 0xFFFFFFFF),
                nullptr
        );

        //pre
        auto beginTime = stopTime - chrono::seconds(preTime);
        thread([&]() {
            auto fileOffset = bufferPtr->GetFileOffset();
            auto windowSize = bufferPtr->GetWindowSize();
            uint32_t windowOffset = 0;
            auto distance = preTime * frequency * (varNames.size() + 2) * sizeof(double);
            if (fileOffset < distance && !bufferPtr->GetFileCount()) {
                fileOffset = 0;
            } else {
                fileOffset = (fileOffset - distance) / windowSize * windowSize;
            }
            //get
            while (running) {
                if (windowSize == 0)
                    windowsMap = MapViewOfFile(
                            mapHandle,
                            FILE_MAP_ALL_ACCESS,
                            DWORD(fileOffset >> 32),
                            DWORD(fileOffset & 0xFFFFFFFF),
                            windowSize
                    );
                auto *buffer = reinterpret_cast<double *>(windowsMap);
                for (size_t i = 0; i < windowSize; i++) {
                    if (isnan(buffer[i])) {
                        if (deserialize_time_point(buffer[i + 1]) >= beginTime && deserialize_time_point(buffer[i + 1]) <= stopTime) {
                            windowOffset = i;
                            running = false;
                            break;
                        }
                    }
                }
                fileOffset += windowSize;
            }

            //post
        }).detach();
    }

    ~VarTest() {

    }
};

TEST(interpolation, test) {
    void *con;
    create_container(&con);
    vector<string> str_name;
    for (size_t i = 0; i < 5000; i++) {
        str_name.push_back("var_" + to_string(i));
        con_set_value_dbl(con, ("var_" + to_string(i)).c_str(), 0.123456, nullptr, nullptr, 0);
    }
    auto ptr = make_unique<VarTest>(con, str_name);
    thread([&] {
        timeBeginPeriod(1);
        for (size_t i = 0; i < 1000; i++) {
            for (size_t j = 0; j < 5000; j++)
                con_set_value_dbl(con, str_name[j].c_str(), i + 1.123456, nullptr, nullptr, 0);
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        timeEndPeriod(1);
    }).join();

}

TEST(mmapwrite, test) {
    string filePath = "D:/test/sliding_mmap.dat";
    HANDLE fileHandle = CreateFileA(
            filePath.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
    );
    if (fileHandle == INVALID_HANDLE_VALUE) {
        cerr << "CreateFile failed: " << GetLastError() << endl;
        return;
    }

    uint64_t fileSize = 64 * 1024 * 1024;
    uint32_t windowSize = 64 * 1024;

    LARGE_INTEGER size;
    size.QuadPart = fileSize;

    if (!SetFilePointerEx(fileHandle, size, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) {
        cout << "File size set failed!" << endl;
    }

    HANDLE mapHandle = CreateFileMappingA(
            fileHandle,
            nullptr,
            PAGE_READONLY,
            DWORD(fileSize >> 32),
            DWORD(fileSize & 0xFFFFFFFF),
            nullptr
    );

    if (!mapHandle) {
        cerr << "CreateFileMapping failed: " << GetLastError() << endl;
        CloseHandle(fileHandle);
        return;
    }

    uint64_t fileOffset = 0;
    for (size_t i = 0; i < 1024; i++) {
        vector<double> vec_data(5000, i);
        fileOffset = 1024 * 64 * i;
        void *windowsMap = MapViewOfFile(
                mapHandle,
                FILE_MAP_ALL_ACCESS,
                DWORD(fileOffset >> 32),
                DWORD(fileOffset & 0xFFFFFFFF),
                windowSize
        );
        if (!windowsMap) {
            cerr << "Map fail " << fileOffset << ": " << GetLastError() << endl;
            return;
        }

        auto *buffer = reinterpret_cast<DataPointDBL *>(windowsMap);
        memcpy(buffer, vec_data.data(), vec_data.size() * sizeof(double));

        UnmapViewOfFile(windowsMap);
    }
}

TEST(array, test) {
    vector<double> vec_dbl(100, 0);
    vector<double> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    memcpy(vec_dbl.data(), data.data(), 3 * sizeof(double));
    memcpy(vec_dbl.data() + 3, data.data() + 3, 7 * sizeof(double));
    for (auto &it: vec_dbl)
        cout << it << ' ';
}

bool double_bit_equal(double a, double b) { //校验toc
    uint64_t ua, ub;
    std::memcpy(&ua, &a, sizeof(double));
    std::memcpy(&ub, &b, sizeof(double));
    return ua == ub;
}

double timePointToDouble(std::chrono::system_clock::time_point tp) {
    using namespace std::chrono;
    auto duration_since_epoch = tp.time_since_epoch();
    return duration_cast<duration<double>>(duration_since_epoch).count();
}

TEST(timestampToDouble, test) {
    auto now = chrono::system_clock::now();
    auto timestamp = timePointToDouble(now);
    std::cout << "Timestamp: " << timestamp << " seconds since epoch\n";
}

TEST(TDMSWrite, test) {
    string filePath = "D:/test/TDMSTest.tdms";
    void *fileHandle = nullptr;
    open_file(filePath.c_str(), "w", fileHandle);
    void *groupHandle = nullptr;
    create_group(fileHandle, "group", groupHandle);
    vector<void *> channelHandles(10, nullptr);
    vector<double> vec_dbl{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (size_t i = 0; i < 10; i++)
        create_channel(groupHandle, ("channel" + to_string(i)).c_str(), channelHandles[i]);
    for (size_t i = 0; i < 10; i++) {
        set_channel_data_double(channelHandles[0], 1, &vec_dbl[i]);
        set_channel_data_double(channelHandles[1], 1, &vec_dbl[i]);
        set_channel_data_double(channelHandles[2], 1, &vec_dbl[i]);
        set_channel_data_double(channelHandles[3], 1, &vec_dbl[i]);
        set_channel_data_double(channelHandles[4], 1, &vec_dbl[i]);
        set_channel_data_double(channelHandles[5], 1, &vec_dbl[i]);
        set_channel_data_double(channelHandles[6], 1, &vec_dbl[i]);
        set_channel_data_double(channelHandles[7], 1, &vec_dbl[i]);
        set_channel_data_double(channelHandles[8], 1, &vec_dbl[i]);
        set_channel_data_double(channelHandles[9], 1, &vec_dbl[i]);
    }
    save_file(fileHandle);
    close_file(fileHandle);
}

void writeVectorToCSV(const std::vector<double> &data, const std::string &filename) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // 将vector中的数据转换为逗号分隔的字符串
    std::ostringstream oss;
    for (size_t i = 0; i < data.size(); ++i) {
        if (i != 0) {
            oss << ",";
        }
        oss << data[i];
    }

    // 写入文件
    outFile << oss.str();

    outFile.close();
    std::cout << "Data successfully written to " << filename << std::endl;
}


//TEST(mmapFileRead, test) {
//    auto begin = chrono::system_clock::now();
//    auto beginTime = chrono::system_clock::time_point(chrono::microseconds(1753076727122000));
//    auto endTime = chrono::system_clock::time_point(chrono::microseconds(1753076728122000));
//    auto posTime = beginTime;
//    string filePath = "D:/test/sliding_mmap.dat";
//    auto fileHandle = CreateFileA(
//            filePath.c_str(),
//            GENERIC_READ,
//            0,
//            nullptr,
//            OPEN_EXISTING,
//            FILE_ATTRIBUTE_NORMAL,
//            nullptr
//    );
//
//    uint64_t fileSize = 528'000'000;
//    uint64_t fileOffset = 0;
//    uint32_t windowSize = 64 * 1024;
//
//    LARGE_INTEGER size;
//    size.QuadPart = fileSize;
//
//    string csvFilePath = "D:/test/csvFile.csv";
//    std::ofstream outFile(csvFilePath);
//
//    if (!outFile.is_open()) {
//        std::cerr << "Error opening file: " << csvFilePath << std::endl;
//        return;
//    }
//
//    auto mapHandle = CreateFileMappingA(
//            fileHandle,
//            nullptr,
//            PAGE_READONLY,
//            DWORD(fileSize >> 32),
//            DWORD(fileSize & 0xFFFFFFFF),
//            nullptr
//    );
//
//    fileOffset = windowSize * 0;
//    auto windowsMap = MapViewOfFile(
//            mapHandle,
//            FILE_MAP_READ,
//            DWORD(fileOffset >> 32),
//            DWORD(fileOffset & 0xFFFFFFFF),
//            windowSize
//    );
//
//    //after obtain beginTime and beginData
//    auto *buffer = reinterpret_cast<double *>(windowsMap);
//    vector<double> bufferData(5000, 0);
//    bool isAim = false;
//    bool running = true;
//    size_t count = 0;
//    while (running) {
//        for (size_t i = 0; i < windowSize / sizeof(double); i++) {
//            std::ostringstream oss;
//            if (isnan(buffer[i])) {
//                i++;
//                count = 0;
//                auto timestamp = deserialize_time_point(buffer[i]);
//                if (timestamp > endTime) {
//                    running = false;
//                    break;
//                } else if (timestamp < beginTime) {
//                    i += 5000;
//                } else {
//                    isAim = true;
//                    if (posTime == timestamp) {
//                        oss << '\n';
//                        oss << TransformStrTimeMS(timestamp);
//                    } else {
//                        oss << '\n';
//                        oss << TransformStrTimeMS(posTime);
//                        for (size_t j = 0; j < 5000; j++) {
//                            oss << ',' << bufferData[j];
//                        }
//                    }
//                    posTime = posTime + chrono::microseconds(1'000);
//                }
//            } else {
//                if (isAim) {
//                    bufferData[count] = buffer[i];
//                    count++;
//                    oss << ',' << buffer[i];
//                }
//            }
//            outFile << oss.str();
//        }
//
//        UnmapViewOfFile(windowsMap);
//
//        fileOffset += windowSize;
//        windowsMap = MapViewOfFile(
//                mapHandle,
//                FILE_MAP_READ,
//                DWORD(fileOffset >> 32),
//                DWORD(fileOffset & 0xFFFFFFFF),
//                windowSize
//        );
//        buffer = reinterpret_cast<double *>(windowsMap);
//    }
//
//    outFile.close();
//    auto end = chrono::system_clock::now();
//    CloseHandle(mapHandle);
//    CloseHandle(fileHandle);
//    cout << chrono::duration_cast<microseconds>(end - begin).count();
//}

TEST(double_timestamp, test) {
    auto tp = chrono::system_clock::now();
    time_t ts = chrono::system_clock::to_time_t(tp);
    tm tim;
    localtime_s(&tim, &ts);
    cout << 1900 + tim.tm_year << ' ' << 1 + tim.tm_mon << ' ' << tim.tm_mday << ' ' << tim.tm_hour << ' ' << tim.tm_min << ' ' << tim.tm_sec << '.'
         << (chrono::duration_cast<chrono::microseconds>(tp.time_since_epoch()) % chrono::seconds(1)).count() << endl;

    auto d = serialize_time_point(tp);
//    cout << d << endl;
    auto t = deserialize_time_point(d);
    time_t ts_ = chrono::system_clock::to_time_t(t);
    tm tm_;
    localtime_s(&tm_, &ts_);
    cout << 1900 + tm_.tm_year << ' ' << 1 + tm_.tm_mon << ' ' << tm_.tm_mday << ' ' << tm_.tm_hour << ' ' << tm_.tm_min << ' ' << tm_.tm_sec << '.'
         << (chrono::duration_cast<chrono::microseconds>(tp.time_since_epoch()) % chrono::seconds(1)).count() << endl;
}

TEST(TDMS, test) {
    vector<double> data{0.123456, 1.123456, 2.123456, 3.123456, 4.123456, 5.123456, 6.123456, 7.123456, 8.123456, 9.123456};
    string filePath = "D:/test/tdms_row.tdms";
    void *fileHandle;
    open_file(filePath.c_str(), "w", fileHandle);
    void *groupHandle = nullptr;
    create_group(fileHandle, "group", groupHandle);
    void *channel0 = nullptr;
    void *channel1 = nullptr;
    void *channel2 = nullptr;
    void *channel3 = nullptr;
    void *channel4 = nullptr;
    create_channel(groupHandle, "channel0", channel0);
    create_channel(groupHandle, "channel1", channel1);
    create_channel(groupHandle, "channel2", channel2);
    create_channel(groupHandle, "channel3", channel3);
    create_channel(groupHandle, "channel4", channel4);
    set_channel_data_double(channel0, 10, data.data());
    set_channel_data_double(channel1, 10, data.data());
    set_channel_data_double(channel2, 10, data.data());
    set_channel_data_double(channel3, 10, data.data());
    set_channel_data_double(channel4, 10, data.data());

    save_file(fileHandle);
    close_file(fileHandle);
}

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

TEST(timestamp, test) {
    std::string timestamp = "2025 07 21 13:45:27.122000";

    // 分割时间和微秒
    std::string datetime_part = timestamp.substr(0, 19); // "2025 07 15 17:23:39"
    std::string microsecond_part = timestamp.substr(20); // "417000"

    // 解析 datetime 为 std::tm
    std::tm tm = {};
    std::istringstream ss(datetime_part);
    ss >> std::get_time(&tm, "%Y %m %d %H:%M:%S");
    if (ss.fail()) {
        std::cerr << "Failed to parse time." << std::endl;
        return;
    }

    // 转换为 time_t
    std::time_t tt = std::mktime(&tm);
    if (tt == -1) {
        std::cerr << "Failed to convert to time_t." << std::endl;
        return;
    }

    // 获取系统时钟起点
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(tt);

    // 加上微秒部分
    int micros = std::stoi(microsecond_part);
    tp += std::chrono::microseconds(micros);

    // 输出时间点的 duration（可选）
    auto since_epoch = tp.time_since_epoch();
    std::cout << "Time point in microseconds since epoch: "
              << std::chrono::duration_cast<std::chrono::microseconds>(since_epoch).count()
              << " μs\n";
    auto timeStamp = chrono::system_clock::time_point(chrono::microseconds(1752571419417000));
}

#include "PMLVariable.h"

TEST(PMLCircularMappedBuffer, test) {
    void *con = nullptr;
    create_container(&con);
    vector<string> varList;
    varList.reserve(50);
    for (size_t i = 0; i < 50; i++) {
        varList.push_back("var_" + to_string(i));
        con_set_value_dbl(con, ("var_" + to_string(i)).c_str(), 0.123456, nullptr, nullptr, 0);
    }
//    auto ptr = make_shared<Variable>(con, varList, 5, 1000, 2, 2);
    thread t0([&] {
        timeBeginPeriod(1);
        auto begin = chrono::system_clock::now();
        for (size_t i = 0; i < 200000; i++) {
            for (size_t j = 0; j < 50; j++)
                con_set_value_dbl(con, varList[j].c_str(), i + 1.123456, nullptr, nullptr, 0);
            this_thread::sleep_for(chrono::milliseconds(1));
        }
        auto end = chrono::system_clock::now();
        cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
        timeEndPeriod(1);
    });
    thread t1{[&] {
        this_thread::sleep_for(chrono::seconds(4));
//        ptr->Recode();
    }};
    t0.join();
    t1.join();
}

TEST(double_to_timestamp, test) {
    cout << TransformStrTimeMS(deserialize_time_point(8.66139015447255e-309)) << endl;
}

TEST(TDMS_time_test, test) {
    void *fileHandle = nullptr;
    void *groupHandle = nullptr;
    void *channelTimeHandle = nullptr;
    void *channelHandle = nullptr;

    auto time = chrono::system_clock::now();
    vector<double> dbl_data{0.123456, 1.123456, 2.123456, 3.123456, 4.123456, 5.123456, 6.123456, 7.123456, 8.123456, 9.123456};
    vector<int64_t> time_data{
            time.time_since_epoch().count(),
            (time + chrono::milliseconds(1)).time_since_epoch().count(),
            (time + chrono::milliseconds(2)).time_since_epoch().count(),
            (time + chrono::milliseconds(3)).time_since_epoch().count(),
            (time + chrono::milliseconds(4)).time_since_epoch().count(),
            (time + chrono::milliseconds(5)).time_since_epoch().count(),
            (time + chrono::milliseconds(6)).time_since_epoch().count(),
            (time + chrono::milliseconds(7)).time_since_epoch().count(),
            (time + chrono::milliseconds(8)).time_since_epoch().count(),
            (time + chrono::milliseconds(9)).time_since_epoch().count()
    };
//    vector<timestamp> time_data{
//            {2025, 7, 29, 10, 51, 30, 123456},
//            {2025, 7, 29, 10, 51, 30, 123456},
//            {2025, 7, 29, 10, 51, 30, 123456},
//            {2025, 7, 29, 10, 51, 30, 123456},
//            {2025, 7, 29, 10, 51, 30, 123456},
//            {2025, 7, 29, 10, 51, 30, 123456},
//            {2025, 7, 29, 10, 51, 30, 123456},
//            {2025, 7, 29, 10, 51, 30, 123456},
//            {2025, 7, 29, 10, 51, 30, 123456},
//            {2025, 7, 29, 10, 51, 30, 123456}
//    };

    open_file("D:/test/Time_test.tdms", "w", fileHandle);
    create_group(fileHandle, "group", groupHandle);
    create_channel(groupHandle, "Time", channelTimeHandle);
    create_channel(groupHandle, "channel", channelHandle);

    set_channel_data_time(channelTimeHandle, time_data.size(), time_data.data());
//    set_channel_data_timestamp(channelTimeHandle, time_data.size(), time_data.data());
    set_channel_data_double(channelHandle, dbl_data.size(), dbl_data.data());
    save_file(fileHandle);

    set_channel_data_time(channelTimeHandle, time_data.size(), time_data.data());
//    set_channel_data_timestamp(channelTimeHandle, time_data.size(), time_data.data());
    set_channel_data_double(channelHandle, dbl_data.size(), dbl_data.data());
    save_file(fileHandle);

    close_file(fileHandle);
}

//#include "mdf/isourceinformation.h"
//#include "mdf/ichannelgroup.h"
//#include "mdf/idatagroup.h"
//#include "mdf/ievent.h"
//#include "mdf/ifilehistory.h"
//
//#include "mdf/mdffactory.h"
//#include "mdf/mdfreader.h"
//#include "mdf/mdfwriter.h"
//#include "mdf/mdflogstream.h"
//
//using namespace std::this_thread;
//using namespace std::chrono_literals;
//using namespace std::filesystem;
//using namespace mdf;
//
//TEST(MDF_test, test) {
//    auto writer = MdfFactory::CreateMdfWriter(MdfWriterType::Mdf3Basic);
//    writer->Init("D:/test/MDF3_test.mdf");
//
//    auto *header = writer->Header();
//    ASSERT_TRUE(header != nullptr);
//    header->Author("Ingemar Hedvall");
//    header->Department("Home Alone");
//    header->Description("Testing Value Types");
//    header->Project("Mdf3WriteHD");
//    header->StartTime(TimeStampToNs());
//    header->Subject("PXY");
//
//    auto *dg3 = writer->CreateDataGroup();
//    auto *cg3 = mdf::MdfWriter::CreateChannelGroup(dg3);
//    {
//        auto *master = mdf::MdfWriter::CreateChannel(cg3);
//        master->Name("Time");
//        master->Description("Time channel");
//        master->Type(ChannelType::Master);
//        master->DataType(ChannelDataType::FloatLe);
//        master->DataBytes(4);
//        master->Unit("s");
//    }
//    {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("UnsignedLe");
//        channel->Description("uint32_t");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::UnsignedIntegerLe);
//        channel->DataBytes(sizeof(uint32_t));
//    }
//    {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("UnsignedBe");
//        channel->Description("uint16_t");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::UnsignedIntegerBe);
//        channel->DataBytes(sizeof(uint16_t));
//    }
//    {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("SignedLe");
//        channel->Description("int32_t");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::SignedIntegerLe);
//        channel->DataBytes(sizeof(int32_t));
//    }
//    {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("SignedBe");
//        channel->Description("int8_t");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::SignedIntegerLe);
//        channel->DataBytes(sizeof(int8_t));
//    }
//    {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("FloatLe");
//        channel->Description("float");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::FloatLe);
//        channel->DataBytes(sizeof(float));
//    }
//    {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("FloatBe");
//        channel->Description("double");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::FloatBe);
//        channel->DataBytes(sizeof(double));
//    }
//    {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("String");
//        channel->Description("string");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::StringAscii);
//        channel->DataBytes(10);
//    }
//    {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("Array");
//        channel->Description("vector");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::ByteArray);
//        channel->DataBytes(5);
//    }
//    {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("Date");
//        channel->Description("CANopen Date");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::CanOpenDate);
//        // channel->DataBytes(7);
//    }
//    {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("Time");
//        channel->Description("CANopen Time");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::CanOpenTime);
//        // channel->DataBytes(7);
//    }
//
//    writer->InitMeasurement();
//    writer->StartMeasurement(TimeStampToNs());
//    for (size_t sample = 0; sample < 100; ++sample) {
//        auto cn_list = cg3->Channels();
//        cn_list[0]->SetChannelValue(0.01 * static_cast<double>(sample));
//        cn_list[1]->SetChannelValue(sample);
//        cn_list[2]->SetChannelValue(sample);
//        cn_list[3]->SetChannelValue(-static_cast<int>(sample));
//        cn_list[4]->SetChannelValue(-static_cast<int>(sample));
//        cn_list[5]->SetChannelValue(11.1 * static_cast<double>(sample));
//        cn_list[6]->SetChannelValue(11.1 * static_cast<double>(sample));
//
//        cn_list[7]->SetChannelValue(std::to_string(sample));
//        std::vector<uint8_t> temp(5, 0);
//        temp[0] = 'T';
//        cn_list[8]->SetChannelValue(temp);
//
//        auto ns70 = TimeStampToNs();
//        cn_list[9]->SetChannelValue(ns70);
//        cn_list[10]->SetChannelValue(ns70);
//
//        writer->SaveSample(*cg3, ns70);
//        sleep_for(10ms);
//    }
//    writer->StopMeasurement(TimeStampToNs());
//    writer->FinalizeMeasurement();
//}
//
//TEST(MDF4_test, test) {
//    auto writer = MdfFactory::CreateMdfWriter(MdfWriterType::Mdf4Basic);
//    writer->Init("D:/test/MDF4_test.mdf");
//
//    auto *header = writer->Header();
//    auto *history = header->CreateFileHistory();
//    history->Description("Test data types");
//    history->ToolName("MdfWrite");
//    history->ToolVendor("ACME Road Runner Company");
//    history->ToolVersion("1.0");
//    history->UserName("Ingemar Hedvall");
//
//    auto *data_group = header->CreateDataGroup();
//    auto *group1 = data_group->CreateChannelGroup();
//    group1->Name("Float");
//
//
//    auto *ch1 = group1->CreateChannel();
//    ch1->Name("Intel32");
//    ch1->Type(ChannelType::FixedLength);
//    ch1->Sync(ChannelSyncType::None);
//    ch1->DataType(ChannelDataType::FloatLe);
//    ch1->Flags(CnFlag::InvalidValid);
//    ch1->DataBytes(4);
//
//    auto *ch2 = group1->CreateChannel();
//    ch2->Name("Intel64");
//    ch2->Type(ChannelType::FixedLength);
//    ch2->Sync(ChannelSyncType::None);
//    ch2->DataType(ChannelDataType::FloatLe);
//    ch2->Flags(CnFlag::InvalidValid);
//    ch2->DataBytes(8);
//
//    auto *ch3 = group1->CreateChannel();
//    ch3->Name("Motorola32");
//    ch3->Type(ChannelType::FixedLength);
//    ch3->Sync(ChannelSyncType::None);
//    ch3->DataType(ChannelDataType::FloatBe);
//    ch3->Flags(CnFlag::InvalidValid);
//    ch3->DataBytes(4);
//
//    auto *ch4 = group1->CreateChannel();
//    ch4->Name("Motorola64");
//    ch4->Type(ChannelType::FixedLength);
//    ch4->Sync(ChannelSyncType::None);
//    ch4->DataType(ChannelDataType::FloatBe);
//    ch4->Flags(CnFlag::InvalidValid);
//    ch4->DataBytes(8);
//
//    writer->PreTrigTime(0);
//    writer->InitMeasurement();
//
//    auto tick_time = TimeStampToNs();
//    writer->StartMeasurement(tick_time);
//
//    for (size_t sample = 0; sample < 100; ++sample) {
//        double value = static_cast<double>(sample) + 0.23;
//        bool valid = (sample % 2) == 0; // Even samples are valid
//        ch1->SetChannelValue(value, valid);
//        ch2->SetChannelValue(value, valid);
//        ch3->SetChannelValue(value, valid);
//        ch4->SetChannelValue(value, valid);
//
//        writer->SaveSample(*group1, tick_time);
//        tick_time += 1'000'000'000;
//    }
//    writer->StopMeasurement(tick_time);
//    writer->FinalizeMeasurement();
//}
//
//TEST(test_sizeof, test) {
//    vector<string> vec_str;
//    vec_str.push_back("Hello World Hello World Hello World Hello World Hello World 123");
//    vec_str.push_back("nihao");
//    char str[64]{};
//    char buffer[128]{};
//    memcpy(buffer, vec_str[0].c_str(), vec_str[0].size());
//    memcpy(buffer + 64, vec_str[1].c_str(), vec_str[1].size());
//    for (size_t i = 0; i < 128; i++) {
//        if (i == 64)
//            cout << endl;
//        cout << buffer[i] << ' ';
//    }
//    cout << endl;
//    string s = buffer;
//    cout << s.size() << endl;
//    cout << s << endl;
//    string s0 = buffer + 64;
//    cout << s0.size() << endl;
//    cout << s0;
//}
//
//TEST(con_get_str, test) {
//    void *con = nullptr;
//    create_container(&con);
//    string str_set = "hello world";
//    char str_get[16]{};
//    con_set_value_str(con, "var_0", str_set.data(), nullptr, nullptr, 0);
//    con_get_value_str(con, "var_0", str_get);
//    string str = str_get;
//    cout << str;
//}
//
//class varStrTest {
//private:
//    int32_t id;
//    shared_ptr<CircularMappedBufferSTR> bufferPtr;
//    vector<string> str_data;
//    unordered_map<string, uint32_t> varId;
//    char toc[17] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
//    chrono::system_clock::time_point posTime;
//    uint32_t count = 0;
//
//public:
//    static void callback(event_data_t data, void *p) {
//        auto ptr = static_cast<varStrTest *>(p);
//        ptr->str_data[ptr->varId[data.name]] = data.value_str;
//        if (TransformTime(&data.time) >= ptr->posTime + chrono::microseconds(1'000)) {
//            ptr->posTime = ptr->posTime + chrono::microseconds(1'000);
//            int64_t tick = chrono::duration_cast<chrono::milliseconds>(ptr->posTime.time_since_epoch()).count();
//            memcpy(&ptr->toc[8], &tick, sizeof(tick));
//            ptr->str_data[0] = ptr->toc;
//            cout << ptr->str_data[0] << endl;
//            ptr->bufferPtr->WriteData(ptr->str_data);
////            cout << "record times: " << ++ptr->count << endl;
////            for(const auto &it : ptr->str_data)
////                cout << it << endl;
////            cout << endl;
//        }
//    }
//
//    varStrTest(void *con, vector<string> &varList) {
//        str_data.reserve(varList.size() + 1);
//        string str_names;
//        for (const auto &var: varList)
//            str_names += var + ',';
//        toc[16] = '\0';
//        auto beginTime = serialize_time_point(chrono::system_clock::now());
//        memcpy(&toc[8], &beginTime, sizeof(double));
//        str_names.pop_back();
//        str_data.emplace_back(toc);
//        for (uint32_t i = 0; i < varList.size(); i++) {
//            char str_get[64]{};
//            con_get_value_str(con, varList[i].c_str(), str_get);
//            str_data.emplace_back(str_get);
//            varId[varList[i]] = i + 1;
//        }
//        bufferPtr = make_shared<CircularMappedBufferSTR>(varList.size(), 1000, 10, "D:/test/");
//        posTime = chrono::system_clock::now();
//        con_add_callback(con, str_names.c_str(), 0, &id, callback, this);
//    }
//
//    uint64_t GetOffset(){
//        chrono::system_clock::time_point timePoint = chrono::system_clock::now() - chrono::milliseconds(100);
////        bufferPtr->GetReadFileOffset(timePoint);
////        return bufferPtr->Get_ReadFileOffset();
//    }
//};
//
//
//TEST(CircularMappedBufferSTR, test) {
//    void *con = nullptr;
//    create_container(&con);
////    con_set_value_str(con, "var", ("Hello World " + to_string(0)).c_str(), nullptr, nullptr, 0);
////    char str_get[64]{};
////    con_get_value_str(con, "var", str_get);
////    string str = str_get;
////    cout << str;
//
//    vector<string> varList;
//    varList.reserve(100);
//    for (uint32_t i = 0; i < 1000; i++) {
//        con_set_value_str(con, ("var" + to_string(i)).c_str(), ("Hello World " + to_string(0)).c_str(), nullptr, nullptr, 0);
//        varList.push_back("var" + to_string(i));
//    }
//
//    auto ptr = make_shared<varStrTest>(con, varList);
//    thread([&] {
//        timeBeginPeriod(1);
//
//        for (uint32_t i = 0; i < 1000; i++) {
//            for (uint32_t j = 0; j < 1000; j++)
//                con_set_value_str(con, varList[j].c_str(), ("Hello World " + to_string(i + 1)).c_str(), nullptr, nullptr, 0);
//            this_thread::sleep_for(chrono::milliseconds(1));
//        }
//        timeEndPeriod(1);
//    }).join();
////
////    cout << ptr->GetOffset();
//}
//
//class varDblTest {
//private:
//    int32_t id = 0;
//    chrono::system_clock::time_point posTime;
//    chrono::system_clock::time_point stopTime;
//    shared_ptr<CircularMappedBufferDBL> bufferPtr;
//    shared_ptr<Record> recordPtr;
//    vector<double> dbl_data;
//    unordered_map<string, uint32_t> varId;
//    vector<string> varList;
//    bool stop = false;
//
//public:
//    static void callback(event_data_t data, void *p) {
//        auto ptr = static_cast<varDblTest *>(p);
//        ptr->dbl_data[ptr->varId[data.name]] = data.value_dbl;
//        if (TransformTime(&data.time) >= ptr->posTime + chrono::microseconds(1'000)) {
//            ptr->posTime = ptr->posTime + chrono::microseconds(1'000);
//            ptr->dbl_data[1] = serialize_time_point(ptr->posTime);
//            ptr->bufferPtr->WriteData(ptr->dbl_data);
//        }
//    }
//
//    static void callback_InShutdown(event_data_t data, void *p){
//        auto ptr = static_cast<varDblTest *>(p);
//        if(data.value_dbl > 0 && !ptr->stop){
//            ptr->stopTime = TransformTime(&data.time);
//            ptr->Record();
//            ptr->stop = true;
//        } else if(data.value_dbl == 0) {
//            ptr->stop = false;
//        }
//    }
//
//    varDblTest(void *con, vector<string> &varList) : varList(varList){
//        dbl_data.reserve(varList.size() + 2);
//        double dbl_toc;
//        char toc[8]{'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
//        memcpy(&dbl_toc, toc, sizeof(double));
//        dbl_data.push_back(dbl_toc);
//        dbl_data.push_back(chrono::duration_cast<chrono::duration<double>>(chrono::system_clock::now().time_since_epoch()).count());
//
//        string str_names;
//        double dbl;
//        for (uint32_t i = 0; i < varList.size(); i++) {
//            con_get_value_dbl(con, varList[i].c_str(), &dbl);
//            dbl_data.push_back(dbl);
//            varId[varList[i]] = i + 2;
//            str_names += varList[i] + ',';
//        }
//        str_names.pop_back();
//
//        bufferPtr = make_shared<CircularMappedBufferDBL>(varList.size(), 100, 10, "D:/test/sliding_mmap_double.dat");
//        posTime = chrono::system_clock::now();
//        con_add_callback(con, str_names.c_str(), 0, &id, callback, this);
//        con_add_callback(con, "InShutdown", 0, &id, callback_InShutdown, this);
//    }
//
//    void Record(){
//        vector<string> str_names;
//        uint16_t preTime = 3;
//        uint16_t postTime = 3;
//        uint16_t frequency = 1000;
//        vector<vector<double>> dbls;
//        vector<vector<string>> strs;
//        vector<chrono::system_clock::time_point> times;
//        vector<double> vec_dbl;
//        vector<string> vec_str;
//
//        for(uint32_t i = 0; i < 10; i++)
//            str_names.push_back("var_str_" + to_string(i));
//
//        for(uint32_t i = 0; i < 10; i++)
//            vec_str.push_back(to_string(123 + i));
//
//        for(uint32_t i = 0; i < 600; i++)
//            times.push_back(posTime + chrono::microseconds(1'000));
//
//        for(uint32_t i = 0; i < 600; i++){
//            vector<string> str;
//            for(int32_t j = 0; j < 100; j++)
//                str.push_back(to_string(i + 100));
//            strs.push_back(str);
//        }
//
//        cout << TransformStrTimeMS(stopTime) << endl;
////        bufferPtr->GetReadFileOffset(stopTime);
//        bufferPtr->ReadData(dbls, 600);
////        recordPtr = createRecord(0, varList, str_names, preTime, postTime, frequency);
//        recordPtr->create_file();
//        recordPtr->set_data(times, dbls, strs);
//        recordPtr->save();
//    }
//};
//
//TEST(CircularMappedBufferDBL, test) {
//    void *con = nullptr;
//    create_container(&con);
//    vector<string> varList;
//    varList.reserve(100);
//
//    for (uint32_t i = 0; i < 100; i++) {
//        con_set_value_dbl(con, ("var" + to_string(i)).c_str(), 0.123456, nullptr, nullptr, 0);
//        varList.push_back("var" + to_string(i));
//    }
//    con_set_value_dbl(con, "InShutdown", 0, nullptr, nullptr, 0);
//
//    auto ptr = make_shared<varDblTest>(con, varList);
//
//    thread t0([&] {
//        timeBeginPeriod(1);
//        for (uint32_t i = 0; i < 10000; i++) {
//            for (uint32_t j = 0; j < 100; j++)
//                con_set_value_dbl(con, ("var" + to_string(j)).c_str(), 1.123456 + i, nullptr, nullptr, 0);
//            this_thread::sleep_for(chrono::milliseconds(1));
//        }
//        timeEndPeriod(1);
//    });
//
//    thread t1 ([&] {
//        this_thread::sleep_for(chrono::seconds(2));
//        con_set_value_dbl(con, "InShutdown", 1, nullptr, nullptr, 0);
//    });
//
//    t0.join();
//    t1.join();
//
////    auto vec_dbl = ptr->GetFileOffset();
////    cout << vec_dbl.size() << endl;
////    cout << vec_dbl.front().size() << endl;
//}
//
//class varRecordTest{
//private:
//    void *con;
//    vector<string> dblList;
//    vector<string> strList;
//    uint16_t preTime;
//    uint16_t postTime;
//    uint16_t frequency;
//    int32_t id_double;
//    int32_t id_string;
//    int32_t id_InShutdown;
//    shared_ptr<CircularMappedBufferDBL> dblBufferPtr;
//    shared_ptr<CircularMappedBufferSTR> strBufferPtr;
//    vector<double> dbl_data;
//    vector<string> str_data;
//    unordered_map<string, uint32_t> id_dbl;
//    unordered_map<string, uint32_t> id_str;
//    chrono::system_clock::time_point stopTime;
//    chrono::system_clock::time_point posTime_dbl;
//    chrono::system_clock::time_point posTime_str;
//    char toc_dbl[8] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
//    char toc_str[17] = {'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
//    bool isShutdown = false;
//    shared_ptr<Record> recordPtr;
//
//public:
//    int data = 0;
//    int32_t d = 0;
//
//    //对于posTime一定要限制精度
//    static void callback_double(event_data_t data, void *p){
//        auto ptr = static_cast<varRecordTest*>(p);
//        ptr->dbl_data[ptr->id_dbl[data.name]] = data.value_dbl;
//        if(TransformTime(&data.time) >= ptr->posTime_dbl + chrono::microseconds(1'000)){
//            ptr->posTime_dbl = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(TransformTime(&data.time)));
//            ptr->dbl_data[1] = serialize_time_point(ptr->posTime_dbl);
//            auto time = deserialize_time_point(ptr->dbl_data[1]);
////            cout << TransformStrTimeMS(time) << endl;
//            ptr->dblBufferPtr->WriteData(ptr->dbl_data);
//        }
//    }
//
//    static void callback_string(event_data_t data, void *p){
//        auto ptr = static_cast<varRecordTest*>(p);
//        ptr->str_data[ptr->id_str[data.name]] = data.value_str;
//        if(TransformTime(&data.time) >= ptr->posTime_str + chrono::microseconds(1'000)){
////            string time;
//            ptr->posTime_str = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(TransformTime(&data.time)));
////            int64_t timePoint = duration_cast<chrono::microseconds>(ptr->posTime_str.time_since_epoch()).count();
////            cout << TransformStrTimeMS(ptr->posTime_str) << endl;
////            memcpy(&time, &timePoint, sizeof(timePoint));
////            auto time = chrono::system_clock::time_point(chrono::microseconds(timePoint));
////            cout << TransformStrTimeMS(time) << endl;
////            string toc{'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
////            toc += time;
//            ptr->str_data[0] = TransformTimeToStr(ptr->posTime_str);
//            ptr->strBufferPtr->WriteData(ptr->str_data);
//        }
//    }
//
//    static void callback_InShutdown(event_data_t data, void *p){
//        auto ptr = static_cast<varRecordTest*>(p);
//        if (data.value_dbl > 0 && !ptr->isShutdown) {
//            ptr->stopTime = TransformTime(&data.time);
//            ptr->Record();
//            ptr->isShutdown = true;
//        } else if(data.value_dbl == 0) {
//            ptr->isShutdown = false;
//        }
//    }
//
//    varRecordTest(void *con, vector<string> &dblList, vector<string> &strList, uint16_t preTime, uint16_t postTime, uint16_t frequency) :
//    dblList(dblList), strList(strList), preTime(preTime), postTime(postTime), frequency(frequency){
//        dbl_data.resize(dblList.size() + 2);
//        memcpy(&dbl_data[0], toc_dbl, sizeof(double));
//        str_data.resize(strList.size() + 1);
//        string dbl_names;
//        string str_names;
//        dbl_names.reserve(dblList.size());
//        str_names.reserve(strList.size());
//
//        for(uint32_t i = 0; i < dblList.size(); i++) {
//            dbl_names += dblList[i] + ',';
//            id_dbl[dblList[i]] = i + 2;
//            con_get_value_dbl(con, dblList[i].c_str(), &dbl_data[i + 2]);
//        }
//        dbl_names.pop_back();
//        for(uint32_t i = 0; i < strList.size(); i++) {
//            str_names += strList[i] + ',';
//            id_str[strList[i]] = i + 1;
//            char str[64]{};
//            con_get_value_str(con, strList[i].c_str(), str);
//            str_data[i + 1] = str;
//        }
//        str_names.pop_back();
//        string filePath = "D:/test/PML_mmap";
//        dblBufferPtr = make_shared<CircularMappedBufferDBL>(dblList.size(), frequency, max(preTime, postTime), "D:/test/sliding_mmap_double.dat");
//        strBufferPtr = make_shared<CircularMappedBufferSTR>(strList.size(), frequency, max(preTime, postTime), "D:/test/sliding_mmap_string.dat");
//        recordPtr = createRecord(0, 10, 10, 1000, filePath, dblList, strList);
//        recordPtr->create_file();
//
//        con_add_callback(con, dbl_names.c_str(), 0, &id_double, callback_double, this);
//        con_add_callback(con, str_names.c_str(), 0, &id_string, callback_string, this);
//        con_add_callback(con, "InShutdown", 0, &id_InShutdown, callback_InShutdown, this);
//    }
//
//    void Record(){
//        auto beginTime = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(stopTime - chrono::seconds(preTime)));
//        auto begin = chrono::system_clock::now();
//        cout << "beginTime: " << TransformStrTimeMS(beginTime) << endl;
//        cout << "stopTime: " << TransformStrTimeMS(stopTime) << endl;
//        thread t0([&]{
//            dblBufferPtr->GetReadFileOffset(beginTime, stopTime);
//        });
//        thread t1([&]{
//            strBufferPtr->GetReadFileOffset(beginTime, stopTime);
//        });
//        t0.join();
//        t1.join();
//
//        vector<chrono::system_clock::time_point> vec_time;
//        vector<vector<double>> vec_dbl;
//        vector<vector<string>> vec_str;
//        vec_time.reserve(1000);
//        vec_dbl.reserve(1000);
//        vec_str.reserve(1000);
//
//        for(uint32_t i = 0; i < preTime; i++){
//            vec_time.clear();
//            for (uint64_t j = 0; j < 1000; j++) {
//                vec_time.push_back(stopTime - chrono::seconds(preTime - i) + chrono::microseconds(j * 1'000'000 / frequency));
//                cout << TransformStrTimeMS(stopTime - chrono::seconds(preTime - i) + chrono::microseconds(j * 1'000'000 / frequency)) << endl;
//            }
//            vec_dbl.clear();
//            vec_str.clear();
//            dblBufferPtr->ReadData(vec_dbl, 1000);
//            cout << vec_dbl.size() << endl;
//            strBufferPtr->ReadData(vec_str, 1000);
//            cout << vec_str.size() << endl;
//
//            recordPtr->set_data(vec_time, vec_dbl, vec_str);
//            recordPtr->save();
//        }
//
//        this_thread::sleep_for(chrono::seconds(postTime));
//
//        recordPtr->set_isPre(false);
//
//        for(uint32_t i = 0; i < postTime; i++){
//            vec_time.clear();
////            for(uint64_t j = 0; j < 1000; j++)
////                vec_time.push_back(stopTime);
//
//            vec_dbl.clear();
//            vec_str.clear();
//            dblBufferPtr->ReadData(vec_dbl, 1000);
//            cout << vec_dbl.size() << endl;
//            strBufferPtr->ReadData(vec_str, 1000);
//            cout << vec_str.size() << endl;
//
//            recordPtr->set_data(vec_time, vec_dbl, vec_str);
//            recordPtr->save();
//        }
//
//        auto end = chrono::system_clock::now();
//        cout << chrono::duration_cast<chrono::microseconds>(end - begin).count() << endl;
//    }
//
//    ~varRecordTest(){
//        close_event(id_double);
//        close_event(id_string);
//        close_event(id_InShutdown);
//    }
//};
//
//TEST(RecordData, test){
//    vector<string> dbl_names;
//    vector<string> str_names;
//    uint16_t preTime = 3;
//    uint16_t postTime = 3;
//    uint16_t frequency = 1000;
//
//    dbl_names.reserve(100);
//    str_names.reserve(100);
//
//    void *con;
//    create_container(&con);
//    for(uint32_t i = 0; i < 100; i++){
//        dbl_names.push_back("var_dbl_" + to_string(i));
//        con_set_value_dbl(con, ("var_dbl_" + to_string(i)).c_str(), 0.123456, nullptr, nullptr, 0);
//        str_names.push_back("var_str_" + to_string(i));
//        con_set_value_str(con, ("var_str_" + to_string(i)).c_str(), to_string(123).c_str(), nullptr, nullptr, 0);
//    }
//    con_set_value_dbl(con, "InShutdown", 0, nullptr, nullptr, 0);
//
////    auto ptr = make_shared<varRecordTest>(con, dbl_names, str_names, frequency, preTime, postTime);
//    auto ptr = make_shared<Variable>(con, 5, frequency, preTime, postTime, dbl_names, str_names, dbl_names, str_names);
//
//    cout << "finish set" << endl;
//    thread t0([&]{
//        for(uint32_t i = 0; i < 10000; i++){
//            timeBeginPeriod(1);
//            for(uint32_t j = 0; j < 100; j++){
//                con_set_value_dbl(con, ("var_dbl_" + to_string(j)).c_str(), i + 1.123456, nullptr, nullptr, 0);
//                con_set_value_str(con, ("var_str_" + to_string(j)).c_str(), to_string(i + 124).c_str(), nullptr, nullptr, 0);
//            }
//            this_thread::sleep_for(chrono::milliseconds(1));
//            timeEndPeriod(1);
//        }
//    });
//    thread t1([&]{
//        this_thread::sleep_for(chrono::seconds(5));
//        con_set_value_dbl(con, "InShutdown", 1, nullptr, nullptr, 0);
//    });
//
//    t0.join();
//    t1.join();
//}
//
//TEST(recordTest, test){
//    vector<string> dbl_names;
//    vector<string> str_names;
//    uint16_t preTime = 10;
//    uint16_t postTime = 10;
//    uint16_t frequency = 1000;
//
//    dbl_names.reserve(100);
//    str_names.reserve(100);
//    for(uint32_t i = 0; i < 100; i++){
//        dbl_names.push_back("var_dbl_" + to_string(i));
//        str_names.push_back("var_str_" + to_string(i));
//    }
//    string filePath = "D:/test/mmap";
//    auto recordPtr = createRecord(1, preTime, postTime, frequency, filePath, {}, str_names);
//    recordPtr->create_file();
//
//    vector<chrono::system_clock::time_point> vec_time;
//    vector<vector<double>> vec_dbl;
//    vector<vector<string>> vec_str;
//
//    auto posTime = chrono::system_clock::time_point(chrono::floor<chrono::milliseconds>(chrono::system_clock::now()));
//    for(uint32_t i = 0; i < 1000; i++)
//        vec_time.push_back(posTime + chrono::milliseconds(i));
//
//    for(uint32_t i = 0; i < 1000; i++){
//        vector<double> dbl;
//        for(uint32_t j = 0; j < 100; j++)
//            dbl.push_back(i + 0.123456);
//        vec_dbl.push_back(dbl);
//    }
//
//    for(uint32_t i = 0; i < 1000; i++){
//        vector<string> str;
//        for(uint32_t j = 0; j < 100; j++)
//            str.push_back(to_string(i + 123));
//        vec_str.push_back(str);
//    }
//
//    recordPtr->set_data(vec_time, {}, vec_str);
//    recordPtr->save();
//
//    recordPtr->set_isPre(false);
//    recordPtr->set_data(vec_time, {}, vec_str);
//    recordPtr->save();
//}

TEST(timeoutput, time){
    unsigned char time[8]{0x10, 0xB8, 0xE9, 0xBA, 0x27, 0x3D, 0x06, 0x00};
    double timeInt{};
    memcpy(&timeInt, &time, sizeof(time));
    auto timePoint = deserialize_time_point(timeInt);
    cout << TransformStrTimeMS(timePoint);
}

TEST(mmap_string_read, test) {
    uint64_t fileSize = 0;

    HANDLE fileHandle = CreateFileA(
            "D:/test/sliding_mmap_string.dat",
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
    );

    if (fileHandle == INVALID_HANDLE_VALUE) {
        cout << "CreateFileA failed with error: " << GetLastError() << endl;
        return;
    }

    HANDLE mapHandle = CreateFileMappingA(
            fileHandle,
            nullptr,
            PAGE_READWRITE,
            DWORD(fileSize >> 32),
            DWORD(fileSize & 0xFFFFFFFF),
            nullptr
    );
    if (!mapHandle) {
        std::cout << "CreateFileMappingA failed. Error code: " << GetLastError() << std::endl;
        return;
    }

    void *windowsMap = MapViewOfFile(
            mapHandle,
            FILE_MAP_READ,
            0,
            0,
            65536
    );

    if (!windowsMap)
        return;

    char *buffer = reinterpret_cast<char*>(windowsMap);
    string str_toc{'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    for(uint32_t i = 0; i < 65536 / 64; i++){
        string str(buffer, buffer + 8);
        string str1(buffer + 8, buffer + 16);
        if(str == str_toc){
            double timePoint;
            memcpy(&timePoint, buffer + 8, sizeof(double));
            auto time = deserialize_time_point(timePoint);
            cout << TransformStrTimeMS(time) << endl;
        } else {
            string str_data(buffer);
            cout << str_data << endl;
        }
        buffer += 64;
    }

    UnmapViewOfFile(windowsMap);
    CloseHandle(mapHandle);
    CloseHandle(fileHandle);
}

TEST(TDMS_dll, test){
    void *fileHandle;
    open_file("D:/test/TDMS_dll_test.tdms", "w", fileHandle);
    void *groupHandle;
    create_group(fileHandle, "group", groupHandle);
    void *channelHandle;
    create_channel(groupHandle, "channel", channelHandle);
    vector<int64_t> timePoints;
    chrono::system_clock::time_point timePoint = chrono::system_clock::now();
    for(uint32_t i = 0; i < 1000; i++)
        timePoints.push_back((timePoint + chrono::microseconds(i * 1'000)).time_since_epoch().count());
    set_channel_data_time(channelHandle, 1000, timePoints.data());
    save_file(fileHandle);
    timePoints.clear();

    timePoint = chrono::system_clock::now();
    for(uint32_t i = 0; i < 1000; i++)
        timePoints.push_back((timePoint + chrono::microseconds(i * 1'000)).time_since_epoch().count());
    set_channel_data_time(channelHandle, 1000, timePoints.data());
    save_file(fileHandle);
    timePoints.clear();
    close_file(fileHandle);

//    void *fileHandle;
//    open_file("D:/test/TDMS_dll_test.tdms", "w", fileHandle);
//    void *groupHandle;
//    create_group(fileHandle, "group", groupHandle);
//    void *channelHandle;
//    create_channel(groupHandle, "channel", channelHandle);
//    vector<timestamp> timePoints{
//            {2025, 8, 25, 15, 45, 30, 123456},
//            {2025, 8, 25, 15, 46, 30, 123456},
//            {2025, 8, 25, 15, 47, 30, 123456},
//            {2025, 8, 25, 15, 48, 30, 123456},
//            {2025, 8, 25, 15, 49, 30, 123456},
//            {2025, 8, 25, 15, 50, 30, 123456},
//            {2025, 8, 25, 15, 51, 30, 123456},
//            {2025, 8, 25, 15, 52, 30, 123456},
//            {2025, 8, 25, 15, 53, 30, 123456},
//            {2025, 8, 25, 15, 54, 30, 123456},
//    };
//    set_channel_data_timestamp(channelHandle, 10, timePoints.data());
//    save_file(fileHandle);
//
//    set_channel_data_timestamp(channelHandle, 10, timePoints.data());
//    save_file(fileHandle);
//    close_file(fileHandle);
}

vector<vector<int>> transpose(const vector<vector<int>>& matrix){
    if(matrix.empty()) return {};

    uint32_t rows = matrix.size();
    uint32_t cols = matrix[0].size();

    vector<vector<int>> result(cols, vector<int>(rows));

    for(uint32_t i = 0; i < rows; i++){
        for(uint32_t j = 0; j < cols; j++)
            result[j][i] = matrix[i][j];
    }

    return result;
}



TEST(matrix, test){
    vector<vector<int>> matrix;
    for(uint32_t i = 0; i < 10; i++) {
        vector<int> vec_int;
        for (uint32_t j = 0; j < 20; j++)
            vec_int.push_back(i*20 + j);
        matrix.push_back(vec_int);
    }

    auto result = transMatrix(matrix);

    for(const auto &row : result){
        for(const auto &it : row)
            cout << it << ' ';
        cout << endl;
    }
}

vector<int> twoSum(vector<int>& nums, int target) {
    for(int i = 0; i < nums.size() - 1; i++){
        for(int j = i + 1; j < nums.size(); j++)
            if(nums[i] + nums[j] == target)
                return {i, j};
    }
    return {};
}



int removeElement(vector<int>& nums, int val) {
    if(nums.empty())
        return 0;
    auto left = nums.begin();
    auto right = nums.end() - 1;
    int count = 0;
    int replace = 0;

    for(; left <= right; ){
        if(*right == val) {
            count++;
            if (right == nums.begin())
                break;
            right--;
        }
        if(*left == val){
            replace = *left;
            *left = *right;
            *right = replace;
            left++;
            count++;
            if (right == nums.begin())
                break;
            right--;
        }
    }

    return nums.size() - count;
//    if(nums.empty())
//        return 0;
//    auto fast = nums.end() - 1;
//    auto slow = nums.end() - 1;
//    int count = 0;
//    int replace = 0;
//
//    for(; fast >= nums.begin(); fast--){
//        if(*fast == val){
//            replace = *fast;
//            *fast = *slow;
//            *slow = replace;
//            slow--;
//            count++;
//        }
//        if(fast == nums.begin())
//            break;
//    }
//
//    return nums.size() - count;
}

void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
    vector<int> nums;
    nums.reserve(m + n);
    int it1 = 0;
    int it2 = 0;
    for(int i = 0; i < m + n; i++){
        if(it1 == m){
            for(int j = it2; j < n; j++)
                nums.push_back(nums2[j]);
            break;
        }
        if(it2 == n){
            for(int j = it1; j < m; j++)
                nums.push_back(nums1[j]);
            break;
        }
        if(nums1[it1] <= nums2[it2]) {
            nums.push_back(nums1[it1]);
            it1++;
        } else {
            nums.push_back(nums2[it2]);
            it2++;
        }
    }
    nums1 = nums;
}

int removeDuplicates(vector<int>& nums) {
    vector<int> result;
    unordered_set<int> set;

    for(const auto &it : nums){
        if(set.find(it) != set.end()){
            continue;
        } else {
            set.insert(it);
            result.push_back(it);
        }
    }
    nums = result;
    return result.size();
}

void moveZeroes(vector<int>& nums) {
    vector<int> result;
    result.reserve(nums.size());

    for(const auto it : nums){
        if(it != 0)
            result.push_back(it);
    }
    while(result.size() < nums.size())
        result.push_back(0);
    nums = result;
}

vector<int> plusOne(vector<int>& digits) {
    digits.back()++;
    for(auto it = digits.rbegin(); it < digits.rend() - 1; it++)
        if(*it == 10){
            *it = 0;
            *(it + 1) += 1;
        }
    if(digits.front() == 10) {
        digits[0] = 0;
        digits.insert(digits.begin(), 1);
        return digits;
    } else {
        return digits;
    }
}

int maxProfit(vector<int>& prices) {
    int min = INT_MAX;
    int max = 0;
    int result = 0;
    for(int i = 0; i < prices.size(); i++) {
        if (prices[i] > max)
            max = prices[i];
        if (prices[i] < min) {
            min = prices[i];
            max = 0;
        }
        if(max - min > result)
            result = max - min;
    }
    return result;
}

int majorityElement(vector<int>& nums) {
    unordered_map<int, uint32_t> numTimes;
    for(const auto &it : nums) {
        numTimes[it]++;
    }
    uint32_t max = 0;
    int max_key = 0;
    for(const auto& [key, value] : numTimes)
        if(value > max) {
            max = value;
            max_key = key;
        }
    return max_key;
}

string longestCommonPrefix(vector<string>& strs) {
    string common = strs.front();
    for(const auto &str : strs)
        for(int i = 0; i < common.size(); i++) {
            if (common[i] == str[i])
                continue;
            else{
                common.substr(0, i);
                break;
            }
        }
    return common;
}

TEST(solution, test){

}

// 普通 TEST
TEST(SimpleTest, RunsWithoutFixture) {
    std::cout << "[TEST] Constructor" << std::endl;
    std::cout << "[TEST] Test Body" << std::endl;
    std::cout << "[TEST] Destructor" << std::endl;
}

// 定义一个 Fixture
class MyFixture : public ::testing::Test {
protected:
    MyFixture() {
        std::cout << "[TEST_F] Constructor" << std::endl;
    }
    ~MyFixture() override {
        std::cout << "[TEST_F] Destructor" << std::endl;
    }
    void SetUp() override {
        std::cout << "[TEST_F] SetUp" << std::endl;
    }
    void TearDown() override {
        std::cout << "[TEST_F] TearDown" << std::endl;
    }
};

// 使用 TEST_F
TEST_F(MyFixture, FirstTest) {
    std::cout << "[TEST_F] Test Body 1" << std::endl;
}

TEST_F(MyFixture, SecondTest) {
    std::cout << "[TEST_F] Test Body 2" << std::endl;
}