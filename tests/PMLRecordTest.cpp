//
// Created by 79933 on 2025/8/27.
//

#include "PMLRecord.h"
#include "gtest/gtest.h"

#ifdef _WIN32
#define FILE_PATH "D:/test/RecordTest"
#else
#define FILE_PATH "/home/disheng/WorkProject/RecordTest"
#endif

using namespace std;

//class RecordTest : public ::testing::Test{
//protected:
//    unique_ptr<VarRecord> ptr;
//    RecordType type{};
//    uint16_t chnDblSize = 0;
//    uint16_t chnStrSize = 0;
//    uint16_t dataLength = 0;
//    string filePath;
//    uint16_t loopTime = 0;
//    uint16_t frequency = 1000;
//
//    void SetConfiguration(RecordType Type, uint16_t ChnDblSize, uint16_t ChnStrSize, uint16_t DataLength, uint16_t LoopTime, string FilePath = "D:/test/RecordTest"){
//        type = Type;
//        chnDblSize = ChnDblSize;
//        chnStrSize = ChnStrSize;
//        dataLength = DataLength;
//        loopTime = LoopTime;
//        filePath = std::move(FilePath);
//
//        vector<string> dbl_names;
//        vector<string> str_names;
//
//        for (uint32_t i = 0; i < chnDblSize; i++)
//            dbl_names.push_back("var_dbl_" + to_string(i));
//
//        for(uint32_t i = 0; i < chnStrSize; i++)
//            str_names.push_back("var_str_" + to_string(i));
//
//        switch(type){
//            case RecordType::TDMS :
//                cout << "|TDMS TEST|";
//                break;
//            case RecordType::CSV :
//                cout << "|CSV TEST|";
//                break;
//            case RecordType::CSVWithoutDes :
//                cout << "|CSVWITHOUTDES TEST|";
//                break;
//            case RecordType::CSVWithoutDesUnit :
//                cout << "|CSVWITHOUTDESUNIT TEST|";
//                break;
//            case RecordType::MDF3 :
//                cout << "|MDF3 TEST|";
//                break;
//            case RecordType::MDF4 :
//                cout << "|MDF4 TEST|";
//                break;
//        }
//
//        ptr = createRecord(type, 10, 10, 1000, filePath, dbl_names, str_names);
//        ptr->create_file();
//    }
//
//    void SetData(){
//        vector<chrono::system_clock::time_point> timePointPre;
//        vector<chrono::system_clock::time_point> timePointPost;
//        vector<vector<double>> vec_dbl;
//        vector<vector<string>> vec_str;
//        vector<double> dbl;
//        vector<string> str;
//
//        timePointPre.reserve(dataLength);
//        timePointPost.reserve(dataLength);
//        vec_dbl.reserve(dataLength);
//        vec_str.reserve(dataLength);
//        dbl.reserve(chnDblSize);
//        str.reserve(chnStrSize);
//
//        for (uint32_t i = 0; i < dataLength; i++) {
//            dbl.clear();
//            str.clear();
//            for (uint32_t j = 0; j < chnDblSize; j++)
//                dbl.push_back(0.123456 + i);
//            vec_dbl.push_back(dbl);
//            for (uint32_t j = 0; j < chnStrSize; j++)
//                str.push_back(to_string(123 + i));
//            vec_str.push_back(str);
//        }
//
//        auto beginTime = chrono::system_clock::now();
//        for (uint32_t i = 0; i < dataLength; i++)
//            timePointPre.push_back(beginTime + i * chrono::milliseconds(1000 / frequency));
//        auto stopTime = beginTime + chrono::milliseconds(1000 / frequency) * dataLength;
//        ptr->SetStopTime(stopTime);
//        for (uint32_t i = 0; i < dataLength; i++)
//            timePointPost.push_back(stopTime + i * chrono::milliseconds(1000 / frequency));
//
//        for(uint32_t l = 0; l < loopTime; l++){
//            if (type == RecordType::TDMS) {
//                ptr->set_data(timePointPre, vec_dbl, vec_str);
//                ptr->save();
//
//                ptr->set_isPre(false);
//                ptr->set_data(timePointPost, vec_dbl, vec_str);
//                ptr->save();
//            } else {
//                ptr->set_data(timePointPre, vec_dbl, vec_str);
//
//                ptr->set_data(timePointPost, vec_dbl, vec_str);
//            }
//        }
//    }
//};

//TEST_F(RecordTest, TDMSBoundary_100dbl_100str){
//    cout << "TDMS Boundary Test 100 double channel 100 string channel" << endl;
//    SetConfiguration(RecordType::TDMS, 100, 100, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_200dbl_200str){
//    cout << "TDMS Boundary Test 200 double channel 200 string channel" << endl;
//    SetConfiguration(RecordType::TDMS, 200, 200, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_500dbl_500str){
//    cout << "TDMS Boundary Test 500 double channel 500 string channel" << endl;
//    SetConfiguration(RecordType::TDMS, 500, 500, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_1000dbl_1000str){
//    cout << "TDMS Boundary Test 1000 double channel 1000 string channel" << endl;
//    SetConfiguration(RecordType::TDMS, 1000, 1000, 1000, 1);
//    SetData();
//}

//TEST_F(RecordTest, TDMSBoundary_2000dbl_2000str){
//    cout << "TDMS Boundary Test 2000 double channel 2000 string channel" << endl;
//    SetConfiguration(RecordType::TDMS, 2000, 2000, 10, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_5000dbl_5000str){
//    cout << "TDMS Boundary Test 5000 double channel 5000 string channel" << endl;
//    SetConfiguration(RecordType::TDMS, 5000, 5000, 10, 1);
//    SetData();
//}

//TEST_F(RecordTest, TDMSBoundary_100dbl){
//    cout << "TDMS Boundary Test 100 double channel" << endl;
//    SetConfiguration(RecordType::TDMS, 100, 0, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_200dbl){
//    cout << "TDMS Boundary Test 200 double channel" << endl;
//    SetConfiguration(RecordType::TDMS, 200, 0, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_500dbl){
//    cout << "TDMS Boundary Test 500 double channel" << endl;
//    SetConfiguration(RecordType::TDMS, 500, 0, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_1000dbl){
//    cout << "TDMS Boundary Test 1000 double channel" << endl;
//    SetConfiguration(RecordType::TDMS, 1000, 0, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_2000dbl){
//    cout << "TDMS Boundary Test 2000 double channel" << endl;
//    SetConfiguration(RecordType::TDMS, 2000, 0, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_5000dbl){
//    cout << "TDMS Boundary Test 5000 double channel" << endl;
//    SetConfiguration(RecordType::TDMS, 5000, 0, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_10000dbl){
//    cout << "TDMS Boundary Test 10000 double channel" << endl;
//    SetConfiguration(RecordType::TDMS, 10000, 0, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_100str){
//    cout << "TDMS Boundary Test 100 string channel" << endl;
//    SetConfiguration(RecordType::TDMS, 0, 100, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_200str){
//    cout << "TDMS Boundary Test 200 string channel" << endl;
//    SetConfiguration(RecordType::TDMS, 0, 200, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_500str){
//    cout << "TDMS Boundary Test 500 string channel" << endl;
//    SetConfiguration(RecordType::TDMS, 0, 500, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSBoundary_1000str){
//    cout << "TDMS Boundary Test 1000 string channel" << endl;
//    SetConfiguration(RecordType::TDMS, 0, 1000, 1000, 1);
//    SetData();
//}
//
//TEST_F(RecordTest, TDMSLoop_100dbl_100str_1000Hz){
//    cout << "TDMS Loop Test 100 double channel 100 string channel 1000Hz 100s" << endl;
//    SetConfiguration(RecordType::TDMS, 100, 100, 100, 100);
//    SetData();
//}

class RecordTDMSTest : public ::testing::Test {
protected:
    unique_ptr<Record> ptr;
    RecordType type = RecordType::TDMS;
    uint16_t chnSize = 200;
    uint16_t dataLength = 1000;
    string filePath = FILE_PATH;
    uint16_t LoopTime = 20;

    void SetUp() override {
        vector<string> dbl_names;
        vector<string> str_names;

        for (uint32_t i = 0; i < chnSize; i++) {
            dbl_names.push_back("var_dbl_" + to_string(i));
            str_names.push_back("var_str_" + to_string(i));
        }
        cout << "|TDMS TEST|" << endl;
        ptr = createRecord(type, 10, 10, 1000, filePath, dbl_names, str_names);
        ptr->create_file();
    }

    void TearDown() override {}
};

TEST_F(RecordTDMSTest, EfficiencyTest) {
    cout << "EFFICIENCY TEST" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    if (type == RecordType::TDMS) {
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();

        ptr->set_isPre(false);
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();
    } else {
        ptr->set_data(timePoint, vec_dbl, vec_str);

        ptr->set_data(timePoint, vec_dbl, vec_str);
    }
    auto end = chrono::system_clock::now();
    cout << "spend time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
}

TEST_F(RecordTDMSTest, LoopTest) {
    cout << "LOOP TEST" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    for (uint32_t i = 0; i < LoopTime; i++) {
        if (type == RecordType::TDMS) {
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();

            ptr->set_isPre(false);
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();
        } else {
            ptr->set_data(timePoint, vec_dbl, vec_str);

            ptr->set_data(timePoint, vec_dbl, vec_str);
        }
    }
    auto end = chrono::system_clock::now();
    cout << "Spend Time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    cout << "Average Time Consumption: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() / LoopTime << endl;
}

class RecordCSVTest : public ::testing::Test {
protected:
    unique_ptr<Record> ptr;
    RecordType type = RecordType::CSV;
    uint16_t chnSize = 200;
    uint16_t dataLength = 1000;
    string filePath = FILE_PATH;
    uint16_t LoopTime = 20;

    void SetUp() override {
        vector<string> dbl_names;
        vector<string> str_names;

        for (uint32_t i = 0; i < chnSize; i++) {
            dbl_names.push_back("var_dbl_" + to_string(i));
            str_names.push_back("var_str_" + to_string(i));
        }

        cout << "|CSV TEST|" << endl;
        ptr = createRecord(type, 10, 10, 1000, filePath, dbl_names, str_names);
        ptr->create_file();
    }

    void TearDown() override {}
};

TEST_F(RecordCSVTest, EfficiencyTest) {
    cout << "EFFICIENCY TEST" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    if (type == RecordType::TDMS) {
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();

        ptr->set_isPre(false);
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();
    } else {
        ptr->set_data(timePoint, vec_dbl, vec_str);

        ptr->set_data(timePoint, vec_dbl, vec_str);
    }
    auto end = chrono::system_clock::now();
    cout << "spend time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
}

TEST_F(RecordCSVTest, LoopTest) {
    cout << "Loop Test" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    for (uint32_t i = 0; i < LoopTime; i++) {
        if (type == RecordType::TDMS) {
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();

            ptr->set_isPre(false);
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();
        } else {
            ptr->set_data(timePoint, vec_dbl, vec_str);

            ptr->set_data(timePoint, vec_dbl, vec_str);
        }
    }
    auto end = chrono::system_clock::now();
    cout << "Spend Time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    cout << "Average Time Consumption: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() / LoopTime << endl;

}

class RecordCSVWithoutDesTest : public ::testing::Test {
protected:
    unique_ptr<Record> ptr;
    RecordType type = RecordType::CSVWithoutDes;
    uint16_t chnSize = 200;
    uint16_t dataLength = 1000;
    string filePath = FILE_PATH;
    uint16_t LoopTime = 20;

    void SetUp() override {
        vector<string> dbl_names;
        vector<string> str_names;

        for (uint32_t i = 0; i < chnSize; i++) {
            dbl_names.push_back("var_dbl_" + to_string(i));
            str_names.push_back("var_str_" + to_string(i));
        }

        cout << "|CSV WITHOUT DES TEST|" << endl;
        ptr = createRecord(type, 10, 10, 1000, filePath, dbl_names, str_names);
        ptr->create_file();
    }

    void TearDown() override {}
};

TEST_F(RecordCSVWithoutDesTest, EfficiencyTest) {
    cout << "EFFICIENCY TEST" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    if (type == RecordType::TDMS) {
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();

        ptr->set_isPre(false);
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();
    } else {
        ptr->set_data(timePoint, vec_dbl, vec_str);

        ptr->set_data(timePoint, vec_dbl, vec_str);
    }
    auto end = chrono::system_clock::now();
    cout << "spend time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
}

TEST_F(RecordCSVWithoutDesTest, LoopTest) {
    cout << "Loop Test" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    for (uint32_t i = 0; i < LoopTime; i++) {
        if (type == RecordType::TDMS) {
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();

            ptr->set_isPre(false);
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();
        } else {
            ptr->set_data(timePoint, vec_dbl, vec_str);

            ptr->set_data(timePoint, vec_dbl, vec_str);
        }
    }
    auto end = chrono::system_clock::now();
    cout << "Spend Time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    cout << "Average Time Consumption: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() / LoopTime << endl;

}

class RecordCSVWithoutDesUintTest : public ::testing::Test {
protected:
    unique_ptr<Record> ptr;
    RecordType type = RecordType::CSVWithoutDesUnit;
    uint16_t chnSize = 200;
    uint16_t dataLength = 1000;
    string filePath = FILE_PATH;
    uint16_t LoopTime = 20;

    void SetUp() override {
        vector<string> dbl_names;
        vector<string> str_names;

        for (uint32_t i = 0; i < chnSize; i++) {
            dbl_names.push_back("var_dbl_" + to_string(i));
            str_names.push_back("var_str_" + to_string(i));
        }

        cout << "|CSV WITHOUT DES AND UINT TEST|" << endl;
        ptr = createRecord(type, 10, 10, 1000, filePath, dbl_names, str_names);
        ptr->create_file();
    }

    void TearDown() override {}
};

TEST_F(RecordCSVWithoutDesUintTest, EfficiencyTest) {
    cout << "EFFICIENCY TEST" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    if (type == RecordType::TDMS) {
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();

        ptr->set_isPre(false);
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();
    } else {
        ptr->set_data(timePoint, vec_dbl, vec_str);

        ptr->set_data(timePoint, vec_dbl, vec_str);
    }
    auto end = chrono::system_clock::now();
    cout << "spend time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
}

TEST_F(RecordCSVWithoutDesUintTest, LoopTest) {
    cout << "Loop Test" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    for (uint32_t i = 0; i < LoopTime; i++) {
        if (type == RecordType::TDMS) {
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();

            ptr->set_isPre(false);
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();
        } else {
            ptr->set_data(timePoint, vec_dbl, vec_str);

            ptr->set_data(timePoint, vec_dbl, vec_str);
        }
    }
    auto end = chrono::system_clock::now();
    cout << "Spend Time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    cout << "Average Time Consumption: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() / LoopTime << endl;

}

class RecordMDF3Test : public ::testing::Test {
protected:
    unique_ptr<Record> ptr;
    RecordType type = RecordType::MDF3;
    uint16_t chnSize = 5000;
    uint16_t dataLength = 1000;
    string filePath = FILE_PATH;
    uint16_t LoopTime = 20;

    void SetUp() override {
        vector<string> dbl_names;
        vector<string> str_names;

        for (uint32_t i = 0; i < chnSize; i++) {
            dbl_names.push_back("var_dbl_" + to_string(i));
//            str_names.push_back("var_str_" + to_string(i));
        }

        cout << "|MDF3 TEST|" << endl;
        ptr = createRecord(type, 10, 10, 1000, filePath, dbl_names, {});
        ptr->create_file();
    }

    void TearDown() override {}
};

TEST_F(RecordMDF3Test, EfficiencyTest) {
    cout << "EFFICIENCY TEST" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
//        for (uint32_t j = 0; j < chnSize; j++)
//            str.push_back(to_string(123 + i));
//        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    if (type == RecordType::TDMS) {
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();

        ptr->set_isPre(false);
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();
    } else {
        ptr->set_data(timePoint, vec_dbl, {});

        ptr->set_data(timePoint, vec_dbl, {});
    }
    auto end = chrono::system_clock::now();
    cout << "spend time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
}

TEST_F(RecordMDF3Test, LoopTest) {
    cout << "Loop Test" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    for (uint32_t i = 0; i < LoopTime; i++) {
        if (type == RecordType::TDMS) {
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();

            ptr->set_isPre(false);
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();
        } else {
            ptr->set_data(timePoint, vec_dbl, vec_str);

            ptr->set_data(timePoint, vec_dbl, vec_str);
        }
    }
    auto end = chrono::system_clock::now();
    cout << "Spend Time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    cout << "Average Time Consumption: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() / LoopTime << endl;

}

class RecordMDF4Test : public ::testing::Test {
protected:
    unique_ptr<Record> ptr;
    RecordType type = RecordType::MDF4;
    uint16_t chnSize = 100;
    uint16_t dataLength = 10;
    string filePath = FILE_PATH;
    uint16_t LoopTime = 20;

    void SetUp() override {
        vector<string> dbl_names;
        vector<string> str_names;

        for (uint32_t i = 0; i < chnSize; i++) {
            dbl_names.push_back("var_dbl_" + to_string(i));
            str_names.push_back("var_str_" + to_string(i));
        }

        cout << "|MDF4 TEST|" << endl;
        ptr = createRecord(type, 10, 10, 1000, filePath, dbl_names, str_names);
        ptr->create_file();
    }

    void TearDown() override {}
};

TEST_F(RecordMDF4Test, EfficiencyTest) {
    cout << "EFFICIENCY TEST" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    if (type == RecordType::TDMS) {
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();

        ptr->set_isPre(false);
        ptr->set_data(timePoint, vec_dbl, vec_str);
        ptr->save();
    } else {
        ptr->set_data(timePoint, vec_dbl, vec_str);

        ptr->set_data(timePoint, vec_dbl, vec_str);
    }
    auto end = chrono::system_clock::now();
    cout << "spend time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
}

TEST_F(RecordMDF4Test, LoopTest) {
    cout << "Loop Test" << endl;
    vector<chrono::system_clock::time_point> timePoint;
    vector<vector<double>> vec_dbl;
    vector<vector<string>> vec_str;
    vector<double> dbl;
    vector<string> str;

    timePoint.reserve(dataLength);
    vec_dbl.reserve(dataLength);
    vec_str.reserve(dataLength);
    dbl.reserve(chnSize);
    str.reserve(chnSize);
    auto beginTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < dataLength; i++)
        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
    for (uint32_t i = 0; i < dataLength; i++) {
        dbl.clear();
        str.clear();
        for (uint32_t j = 0; j < chnSize; j++)
            dbl.push_back(0.123456 + i);
        vec_dbl.push_back(dbl);
        for (uint32_t j = 0; j < chnSize; j++)
            str.push_back(to_string(123 + i));
        vec_str.push_back(str);
    }

    auto begin = chrono::system_clock::now();
    for (uint32_t i = 0; i < LoopTime; i++) {
        if (type == RecordType::TDMS) {
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();

            ptr->set_isPre(false);
            ptr->set_data(timePoint, vec_dbl, vec_str);
            ptr->save();
        } else {
            ptr->set_data(timePoint, vec_dbl, vec_str);

            ptr->set_data(timePoint, vec_dbl, vec_str);
        }
    }
    auto end = chrono::system_clock::now();
    cout << "Spend Time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    cout << "Average Time Consumption: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() / LoopTime << endl;

}

//class RecordTest : public ::testing::Test {
//protected:
//    unique_ptr<VarRecord> ptr;
//    uint16_t type = 1;
//    uint16_t chnSize = 200;
//    uint16_t dataLength = 1000;
//    string filePath = "D:/test/RecordTest";
//    uint16_t LoopTime = 20;
//
//    void SetUp() override {
//        vector<string> dbl_names;
//        vector<string> str_names;
//
//        for (uint32_t i = 0; i < chnSize; i++) {
//            dbl_names.push_back("var_dbl_" + to_string(i));
//            str_names.push_back("var_str_" + to_string(i));
//        }
//
//        ptr = createRecord(type, 10, 10, 1000, filePath, dbl_names, str_names);
//        ptr->create_file();
//    }
//
//    void TearDown() override {}
//};
//
//TEST_F(RecordTest, EfficiencyTest) {
//    cout << "EFFICIENCY TEST" << endl;
//    vector<chrono::system_clock::time_point> timePoint;
//    vector<vector<double>> vec_dbl;
//    vector<vector<string>> vec_str;
//    vector<double> dbl;
//    vector<string> str;
//
//    timePoint.reserve(dataLength);
//    vec_dbl.reserve(dataLength);
//    vec_str.reserve(dataLength);
//    dbl.reserve(chnSize);
//    str.reserve(chnSize);
//    auto beginTime = chrono::system_clock::now();
//    for (uint32_t i = 0; i < dataLength; i++)
//        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
//    for (uint32_t i = 0; i < dataLength; i++) {
//        dbl.clear();
//        str.clear();
//        for (uint32_t j = 0; j < chnSize; j++)
//            dbl.push_back(0.123456 + i);
//        vec_dbl.push_back(dbl);
//        for (uint32_t j = 0; j < chnSize; j++)
//            str.push_back(to_string(123 + i));
//        vec_str.push_back(str);
//    }
//
//    auto begin = chrono::system_clock::now();
//    if (type == 0) {
//        ptr->set_data(timePoint, vec_dbl, vec_str);
//        ptr->save();
//
//        ptr->set_isPre(false);
//        ptr->set_data(timePoint, vec_dbl, vec_str);
//        ptr->save();
//    } else {
//        ptr->set_data(timePoint, vec_dbl, vec_str);
//
//        ptr->set_data(timePoint, vec_dbl, vec_str);
//    }
//    auto end = chrono::system_clock::now();
//    cout << "spend time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
//}
//
//TEST_F(RecordTest, LoopTest) {
//    cout << "Loop Test" << endl;
//    vector<chrono::system_clock::time_point> timePoint;
//    vector<vector<double>> vec_dbl;
//    vector<vector<string>> vec_str;
//    vector<double> dbl;
//    vector<string> str;
//
//    timePoint.reserve(dataLength);
//    vec_dbl.reserve(dataLength);
//    vec_str.reserve(dataLength);
//    dbl.reserve(chnSize);
//    str.reserve(chnSize);
//    auto beginTime = chrono::system_clock::now();
//    for (uint32_t i = 0; i < dataLength; i++)
//        timePoint.push_back(beginTime + i * chrono::milliseconds(1));
//    for (uint32_t i = 0; i < dataLength; i++) {
//        dbl.clear();
//        str.clear();
//        for (uint32_t j = 0; j < chnSize; j++)
//            dbl.push_back(0.123456 + i);
//        vec_dbl.push_back(dbl);
//        for (uint32_t j = 0; j < chnSize; j++)
//            str.push_back(to_string(123 + i));
//        vec_str.push_back(str);
//    }
//
//    auto begin = chrono::system_clock::now();
//    for (uint32_t i = 0; i < LoopTime; i++) {
//        if (type == 0) {
//            ptr->set_data(timePoint, vec_dbl, vec_str);
//            ptr->save();
//
//            ptr->set_isPre(false);
//            ptr->set_data(timePoint, vec_dbl, vec_str);
//            ptr->save();
//        } else {
//            ptr->set_data(timePoint, vec_dbl, vec_str);
//
//            ptr->set_data(timePoint, vec_dbl, vec_str);
//        }
//    }
//    auto end = chrono::system_clock::now();
//    cout << "Spend Time: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
//    cout << "Average Time Consumption: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() / LoopTime << endl;
//}

using namespace std;

//TEST(MDFRead, test) {
////    using namespace mdf;
////    MdfReader oRead("D:/test/mdf3_test500.mdf");
////    const auto start = steady_clock::now();
////    oRead.ReadMeasurementInfo();
////    const auto stop = steady_clock::now();
////    std::chrono::duration<double> diff = duration_cast<milliseconds>(stop - start);
////    std::cout << "Read Measurement Info TrueNas: " << diff.count()
////              << " ms" << std::endl;
//
//    const auto start = steady_clock::now();
//    MdfReader oRead("D:/test/mdf3_test.mdf");
//    oRead.ReadEverythingButData();
//    const auto *file = oRead.GetFile();
//    DataGroupList dg_list;
//    file->DataGroups(dg_list);
//    for (auto *dg: dg_list) {
//        ChannelObserverList observer_list;
//        auto cg_list = dg->ChannelGroups();
//        for (const auto *cgc: cg_list) {
//            CreateChannelObserverForChannelGroup(*dg, *cgc, observer_list);
//        }
//        oRead.ReadData(*dg);
//
//        double eng_value = 0;
//        bool valid = true;
//        for (const auto &channel: observer_list) {
//            size_t samples = channel->NofSamples();
//            cout << channel->Name() + ':' << endl;
//            for (size_t sample = 0; sample < samples; ++sample) {
//                valid = channel->GetEngValue(sample, eng_value);
//                cout << eng_value << ' ';
//            }
//            cout << endl;
//        }
//
//    }
//
//    const auto stop = steady_clock::now();
//    std::chrono::duration<double> diff = duration_cast<milliseconds>(stop - start);
//    std::cout << "Everything + Conversion (TrueNas): " << diff.count()
//              << " ms" << std::endl;
////        MdfReader reader("D:/test/RecordTest.mdf");  // Note the file is now open.
////
////        // Read all blocks but not the raw data and attachments.
////        // This reads in the block information into memory.
////        reader.ReadEverythingButData();
////
////        const auto* mdf_file = reader.GetFile(); // Get the file interface.
////        DataGroupList dg_list;                   // Get all measurements.
////        mdf_file->DataGroups(dg_list);
////
////        // In this example, we read in all sample data and fetch all values.
////        for (auto* dg4 : dg_list) {
////            // Subscribers holds the sample data for a channel.
////            // You should normally only subscribe on some channels.
////            // We need a list to hold them.
////            ChannelObserverList subscriber_list;
////            const auto cg_list = dg4->ChannelGroups();
////            for (const auto* cg4 : cg_list ) {
////                const auto cn_list = cg4->Channels();
////                for (const auto* cn4 : cn_list) {
////                    // Create a subscriber and add it to the temporary list
////                    auto sub = CreateChannelObserver(*dg4, *cg4, *cn4);
////                    subscriber_list.emplace_back(std::move(sub));
////                }
////            }
////
////            // Now it is time to read in all samples
////            reader.ReadData(*dg4); // Read raw data from file
////            double channel_value = 0.0; // Channel value (no scaling)
////            double eng_value = 0.0; // Engineering value
////            for (auto& obs : subscriber_list) {
////                for (size_t sample = 0; sample < obs->NofSamples(); ++sample) {
////                    const auto channel_valid = obs->GetChannelValue(sample, channel_value);
////                    const auto eng_valid = obs->GetEngValue(sample, eng_value);
////                    // You should do something with data here
////                }
////            }
////
////            // Not needed in this example as we delete the subscribers,
////            // but it is good practise to remove samples data from memory
////            // when it is no longer needed.
////            dg4->ClearData();
////        }
////        reader.Close(); // Close the file
//}
//
//TEST(MDF3Error, test) {
//    uint16_t channelNum = 2000;
//    string filePath = "D:/test/test_c.mdf";
//    ifstream f(filePath);
//    if (f.good()) {
//        f.close();
//        ASSERT_EQ(remove(filePath.c_str()), 0);
//    }
//    auto writer = MdfFactory::CreateMdfWriterEx(MdfWriterType::Mdf3Basic);
//    writer->Init(filePath);
//
//    auto *header = writer->Header();
//    ASSERT_TRUE(header != nullptr);
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
//    for (uint32_t i = 0; i < channelNum; i++) {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("channel_double_" + to_string(i));
//        channel->Description("float");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::FloatLe);
//        channel->DataBytes(sizeof(float));
//    }
////    for (uint32_t i = 0; i < channelNum; i++) {
////        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
////        channel->Name("channel_string_" + to_string(i));
////        channel->Description("string");
////        channel->Type(ChannelType::FixedLength);
////        channel->DataType(ChannelDataType::StringAscii);
////        channel->DataBytes(64);
////    }
//
//    writer->InitMeasurement();
//    writer->StartMeasurement(TimeStampToNs());
//    for (size_t sample = 0; sample < 10; ++sample) {
//        auto cn_list = cg3->Channels();
//        cn_list[0]->SetChannelValue(0.01 * static_cast<double>(sample));
//        for (uint32_t i = 0; i < channelNum; i++)
//            cn_list[i + 1]->SetChannelValue(11.1 * static_cast<double>(sample));
////        for (uint32_t i = 0; i < channelNum; i++)
////            cn_list[i + channelNum + 1]->SetChannelValue(std::to_string(sample));
//
//        auto ns70 = TimeStampToNs();
//
//        writer->SaveSample(*cg3, ns70);
//        this_thread::sleep_for(10ms);
//    }
//    writer->StopMeasurement(TimeStampToNs());
//    writer->FinalizeMeasurement();
//
//    auto begin = chrono::system_clock::now();
//    MdfReader oRead(filePath);
//    oRead.ReadEverythingButData();
//    const auto *file = oRead.GetFile();
//    DataGroupList dg_list;
//    file->DataGroups(dg_list);
//    for (auto *dg: dg_list) {
//        ChannelObserverList observer_list;
//        auto cg_list = dg->ChannelGroups();
//        for (const auto *cgc: cg_list) {
//            CreateChannelObserverForChannelGroup(*dg, *cgc, observer_list);
//        }
//        oRead.ReadData(*dg);
//
//        double value = 0;
//        uint32_t count = 0;
//        bool valid = true;
//        std::vector<double> values_exp{};
//        std::vector<double> values_read{};
//        for (const auto &channel: observer_list) {
//            if (count == 0) {
//                count++;
//                continue;
//            } else if (count < channelNum + 1) {
//                size_t samples = channel->NofSamples();
//                values_exp.clear();
//                values_read.clear();
//                bool fail = false;
//                for (size_t sample = 0; sample < samples; ++sample) {
//                    values_exp.push_back(0.123456 + static_cast<double>(sample));
//                    channel->GetEngValue(sample, value);
//                    values_read.push_back(value);
//                    if(0.123456 + static_cast<double>(sample) - value > 0.00001)
//                        fail = true;
//                }
//                if(fail)
//                    EXPECT_EQ(values_read, values_exp);
//
//            } else {
//                size_t samples = channel->NofSamples();
//                values_exp.clear();
//                values_read.clear();
//                bool fail = false;
//                for (size_t sample = 0; sample < samples; ++sample) {
//                    values_exp.push_back(sample);
//                    channel->GetEngValue(sample, value);
//                    values_read.push_back(value);
//                    if(value == sample)
//                        fail = true;
//                }
//                if(fail)
//                    EXPECT_EQ(values_read, values_exp);
//            }
//            count++;
//        }
//    }
//    auto end = chrono::system_clock::now();
//    cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
//}
//
//TEST(MDF3ErrorCout, test) {
//    uint16_t channelNum = 200;
//    string filePath = "D:/test/mdf3_test.mdf";
//    ifstream f(filePath);
//    if (f.good()) {
//        f.close();
//        ASSERT_EQ(remove(filePath.c_str()), 0);
//    }
//    auto writer = MdfFactory::CreateMdfWriterEx(MdfWriterType::Mdf3Basic);
//    writer->Init(filePath);
//
//    auto *header = writer->Header();
//    ASSERT_TRUE(header != nullptr);
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
//    for (uint32_t i = 0; i < channelNum; i++) {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("channel_double_" + to_string(i));
//        channel->Description("float");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::FloatLe);
//        channel->DataBytes(sizeof(float));
//    }
//    for (uint32_t i = 0; i < channelNum; i++) {
//        auto *channel = mdf::MdfWriter::CreateChannel(cg3);
//        channel->Name("channel_string_" + to_string(i));
//        channel->Description("string");
//        channel->Type(ChannelType::FixedLength);
//        channel->DataType(ChannelDataType::StringAscii);
//        channel->DataBytes(64);
//    }
//
//    writer->InitMeasurement();
//    writer->StartMeasurement(TimeStampToNs());
//    for (size_t sample = 0; sample < 10; ++sample) {
//        auto cn_list = cg3->Channels();
//        cn_list[0]->SetChannelValue(0.01 * static_cast<double>(sample));
//        for (uint32_t i = 0; i < channelNum; i++)
//            cn_list[i + 1]->SetChannelValue(11.1 * static_cast<double>(sample));
//        for (uint32_t i = 0; i < channelNum; i++)
//            cn_list[i + channelNum + 1]->SetChannelValue(std::to_string(sample));
//
//        auto ns70 = TimeStampToNs();
//
//        writer->SaveSample(*cg3, ns70);
//        this_thread::sleep_for(10ms);
//    }
//    writer->StopMeasurement(TimeStampToNs());
//    writer->FinalizeMeasurement();
//
//    MdfReader oRead(filePath);
//    oRead.ReadEverythingButData();
//    const auto *file = oRead.GetFile();
//    DataGroupList dg_list;
//    file->DataGroups(dg_list);
//    for (auto *dg: dg_list) {
//        ChannelObserverList observer_list;
//        auto cg_list = dg->ChannelGroups();
//        for (const auto *cgc: cg_list) {
//            CreateChannelObserverForChannelGroup(*dg, *cgc, observer_list);
//        }
//        oRead.ReadData(*dg);
//
//        double value = 0;
//        bool valid = true;
//        for (const auto &channel: observer_list) {
//            size_t samples = channel->NofSamples();
//            cout << channel->Name() + ':' << endl;
//            for (size_t sample = 0; sample < samples; ++sample) {
//                valid = channel->GetEngValue(sample, value);
//                cout << value << ' ';
//            }
//            cout << endl;
//        }
//    }
//}

//TEST(test, double_to_string){
//    double x = 3.141592653589793;
//    std::ostringstream oss;
//    oss << std::setprecision(15) << x;
//    std::string s = oss.str();
//    cout << s << endl;
//}

using namespace std;
using namespace std::chrono;

#include <charconv>
#include <utility>

constexpr size_t N = 5000000;

TEST(test, double_to_string) {
    vector<double> data(N);
    for (size_t i = 0; i < N; i++) {
        data[i] = 3.141592653589793 * (i + 1); // 生成一些数据
    }

    // --- 1. std::to_string ---
    {
        auto start = high_resolution_clock::now();
        vector<string> results;
        results.reserve(N);
        for (double v: data) {
            results.push_back(to_string(v));
        }
        auto end = high_resolution_clock::now();
        cout << "std::to_string: "
             << duration_cast<milliseconds>(end - start).count()
             << " ms" << endl;
    }

    // --- 2. ostringstream ---
    {
        auto start = high_resolution_clock::now();
        vector<string> results;
        results.reserve(N);
        for (double v: data) {
            ostringstream oss;
            oss << setprecision(15) << v;
            results.push_back(oss.str());
        }
        auto end = high_resolution_clock::now();
        cout << "ostringstream: "
             << duration_cast<milliseconds>(end - start).count()
             << " ms" << endl;
    }

    // --- 3. snprintf ---
    {
        auto start = high_resolution_clock::now();
        vector<string> results;
        results.reserve(N);
        char buf[64];
        for (double v: data) {
            snprintf(buf, sizeof(buf), "%.15f", v);
            results.push_back(string(buf));
        }
        auto end = high_resolution_clock::now();
        cout << "snprintf: "
             << duration_cast<milliseconds>(end - start).count()
             << " ms" << endl;
    }

    // --- 4. std::to_chars (C++17) ---
    {
        auto start = high_resolution_clock::now();
        vector<string> results;
        results.reserve(N);
        array<char, 64> buf;
        for (double v: data) {
            auto [ptr, ec] = to_chars(buf.data(), buf.data() + buf.size(),
                                      v, chars_format::general, 15);
            results.push_back(string(buf.data(), ptr));
        }
        auto end = high_resolution_clock::now();
        cout << "std::to_chars: "
             << duration_cast<milliseconds>(end - start).count()
             << " ms" << endl;
    }
}

TEST(timePoint, test) {
    uint16_t postTime = 3;
    uint16_t frequency = 1000;
    vector<chrono::system_clock::time_point> vec_time;
    auto stopTime = chrono::system_clock::now();
    for (uint32_t i = 0; i < postTime; i++) {
        vec_time.clear();
        for (uint64_t j = 0; j < 1000; j++) {
            vec_time.push_back(stopTime + chrono::seconds(i) + chrono::microseconds(j * 1'000'000 / frequency));
            cout << TransformStrTimeMS(stopTime + chrono::seconds(i) + chrono::microseconds(j * 1'000'000 / frequency)) << endl;
        }
    }
}

class ClassTest{
private:
    int a = 0;
    double b = 0;
    string c = 0;

public:
    void set(int a , double b , const string& c){
        this->a = a;
        this->b = b;
        this->c = c;
    }

    void display(){
        cout << a << b << c;
    }
};

TEST(ClassTest, test){
    ClassTest ct;
    int a = 1;
    double b = 1.2345;
    string c = "hello";
//    set(&ct, a, b, c);
}