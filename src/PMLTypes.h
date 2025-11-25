//
// Created by 79933 on 2025/3/27.
//

#ifndef POSTMORTEMLOGGER_PMLTYPES_H
#define POSTMORTEMLOGGER_PMLTYPES_H

#include "varmanager/varmanager.h"

#include <cstdint>
#include <string>
#include <cstring>
#include <utility>
#include <variant>
#include <vector>
#include <memory>
#include <chrono>
#include <iomanip>
#include <sstream>

struct Channel {
    std::string statisticsType;
    std::string channelName;
    std::string varName;
    std::string varType;
    std::string varUnit;
    std::string channelType;
    std::string channelDes;
};

struct DeviceConfig {
    double preStopTime;
    double postStopTime;
    double frequency;
    uint16_t fileType;
    std::vector<std::shared_ptr<Channel>> channels;
};

struct Property {
    bool type;
    std::string name;
    double p_dbl{};
    std::string p_str;

    Property(std::string name, const double p_dbl) : type(true), name(std::move(name)), p_dbl(p_dbl) {}

    Property(std::string name, std::string value) : type(false), name(std::move(name)), p_str(std::move(value)) {}
};

std::string two_digit(int num) {
    return (num < 10 ? "0" : "") + std::to_string(num);
}

std::string six_digit(uint64_t num){
    std::stringstream ss;
    ss << std::setw(6) << std::setfill('0') << num;
    return ss.str();
}

std::string transform_dbl(double num){
    std::ostringstream out;
    out << std::fixed << std::setprecision(6) << num;
    return out.str();
}

std::chrono::system_clock::time_point TransformTime(const lvtime *time) {
    if (time == nullptr) return std::chrono::system_clock::now();
    return std::chrono::system_clock::time_point(std::chrono::seconds(time->time_s - 2082844800) + std::chrono::microseconds((uint64_t) ((double) time->time_us / 18446744073709.551615)));
}

std::string TransformStrTimeS(const std::chrono::system_clock::time_point &t) {
    time_t tp = std::chrono::system_clock::to_time_t(t);
    tm nowPoint;
#ifdef _WIN32
    localtime_s(
            &nowPoint, &tp);
#else
    localtime_r(&tp, &nowPoint);
#endif
    return std::to_string(nowPoint.tm_year + 1900) + two_digit(nowPoint.tm_mon + 1) + two_digit(nowPoint.tm_mday) + '_' + two_digit(nowPoint.tm_hour) + two_digit(nowPoint.tm_min)
           + two_digit(nowPoint.tm_sec);
}

std::string TransformStrTimeMS(const std::chrono::system_clock::time_point &t) {
    time_t tp = std::chrono::system_clock::to_time_t(t);
    tm nowPoint;
#ifdef _WIN32
    localtime_s(&nowPoint, &tp);
#else
    localtime_r(&tp, &nowPoint);
#endif
    return std::to_string(nowPoint.tm_year + 1900) + two_digit(nowPoint.tm_mon + 1) + two_digit(nowPoint.tm_mday) + '_' + two_digit(nowPoint.tm_hour) + two_digit(nowPoint.tm_min)
           + two_digit(nowPoint.tm_sec) + '.' + six_digit((std::chrono::duration_cast<std::chrono::microseconds>(t.time_since_epoch()) % std::chrono::seconds(1)).count());
}

uint64_t TimeStampToNs(std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now()) {
    // 1.na since midnight
    // 2 sec since 1970
    // 3 Add them
    const auto ns_midnight = std::chrono::duration_cast<std::chrono::nanoseconds>(
            timestamp.time_since_epoch()) %
                             1'000'000'000;
    const auto sec_1970 = std::chrono::system_clock::to_time_t(timestamp);
    uint64_t ns = sec_1970;
    ns *= 1'000'000'000;
    ns += ns_midnight.count();
    return ns;
}

// 序列化：time_point -> double（位精确）
double serialize_time_point(std::chrono::system_clock::time_point tp) {
    int64_t ticks = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch()).count();
    double result;
    static_assert(sizeof(result) == sizeof(ticks), "Size mismatch!");
    std::memcpy(&result, &ticks, sizeof(double)); // 精确 bit 拷贝
    return result;
}

// 反序列化：double -> time_point（位精确）
std::chrono::system_clock::time_point deserialize_time_point(double d) {
    int64_t ticks;
    std::memcpy(&ticks, &d, sizeof(double)); // 恢复原始 ticks
    return std::chrono::system_clock::time_point{std::chrono::microseconds{ticks}};
}

std::string TransformTimeToStr(std::chrono::system_clock::time_point tp){
    char time[9]{};
    int64_t timeInt = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch()).count();
    std::string str{'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    memcpy(&time, &timeInt, sizeof(int64_t));
    std::string str_time(std::begin(time), std::end(time));
    str += str_time;
    return str;
}

template <class T>
std::vector<std::vector<T>> transMatrix(const std::vector<std::vector<T>> &matrix) {
    if (matrix.empty()) return {};

    const uint64_t rows = matrix.size();
    const uint64_t cols = matrix[0].size();

    std::vector<std::vector<T>> result(cols, std::vector<T>(rows));
    for(uint64_t i = 0; i < rows; i++)
        for(uint64_t j = 0; j < cols; j++)
            result[j][i] = matrix[i][j];

    return result;
}

bool isStringNull(const char *ptr){
    char stringNull[64] = {0};
    return memcmp(ptr, stringNull, sizeof(stringNull));
}

#endif //POSTMORTEMLOGGER_PMLTYPES_H
