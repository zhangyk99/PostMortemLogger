//
// Created by 79933 on 2025/6/23.
//

#ifndef POSTMORTEMLOGGER_PMLCIRCULARMAPPEDBUFFER_H
#define POSTMORTEMLOGGER_PMLCIRCULARMAPPEDBUFFER_H

//toc
const char TOC[8]{'M', 'D', 'A', 'T', 'A', 's', (char) 0xF8, (char) 0x7F};
#define CHECK_TOC(ptr) (std::memcmp(reinterpret_cast<const char*>(ptr), TOC, 8) == 0)

//error code
#define OPTION_OK                               0
#define WINDOWS_FILE_OPEN_FAILED                (-5001)
#define WINDOWS_FILE_SIZE_SETTING_FAILED        (-5002)
#define WINDOWS_MMAP_CREATE_FAILED              (-5003)
#define LINUX_FILE_OPEN_FAILED                  (-5004)
#define LINUX_FILE_SIZE_SETTING_FAILED          (-5005)
#define LINUX_MMAP_CREATE_FAILED                (-5006)
#define LINUX_MMAP_CLOSE_FAILED                 (-5007)
#define INPUT_CHANNEL_NAME_ERROR                (-5008)
#define TIME_POS_OVER                           (-5009)
#define BEGIN_TIME_ERROR                        (-5010)
#define TIME_POS_ERROR                          (-5011)
#define DATA_EMPTY                              (-5012)
#define OFFSET_GET_FAILED                       (-5013)
#ifdef _WIN32

#include <windows.h>

#else

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#endif

#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include <deque>
#include <cmath>
#include <algorithm>

#include "PMLTypes.h"
//#include "PMLError.h"

#pragma pack(push, 1)
struct DataPointDBL {
    uint32_t id;
    std::chrono::system_clock::time_point timestamp;
    double dbl;
};

struct DataPointStr {
    uint32_t id;
    std::chrono::system_clock::time_point timestamp;
    char str[64];
};
#pragma pack(pop)

struct ReadConfig {
    uint64_t fileOffset = 0;
    uint32_t windowOffset = 0;
    uint64_t startOffset = 0;
    std::chrono::system_clock::time_point posTime;
    bool isBack = false;
    void *readWindowsMap = nullptr;
};

class CircularMappedBufferDBL {
private:
#ifdef _WIN32
    HANDLE fileHandle;
    HANDLE mapHandle;
#else
    int fileHandle;
#endif
    uint16_t chnNum = 0;
    uint16_t frequency = 0;
    uint64_t fileSize = 0;
    uint64_t fileOffset = 0;
    uint32_t windowSize = 64 * 1024;
    uint32_t windowOffset = 0;
    void *windowsMap = nullptr;
    int32_t errorCode = 0;
    std::vector<std::shared_ptr<ReadConfig>> readConfigList;
    std::vector<double> vec_buffer;
    bool isTime = false;

public:
    CircularMappedBufferDBL(uint16_t chnNum, uint16_t frequency, uint16_t recordTime, const std::string &filePath) : chnNum(chnNum), frequency(frequency) {
        vec_buffer.resize(chnNum);
#ifdef _WIN32
        fileHandle = CreateFileA(
                (filePath + ".dat").c_str(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                nullptr,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
        );
        if (fileHandle == INVALID_HANDLE_VALUE) {
            errorCode = WINDOWS_FILE_OPEN_FAILED;
            return;
        }

        //set file size
        uint32_t preSize = windowSize / (chnNum * sizeof(double));
        fileSize = static_cast<uint64_t>(frequency * recordTime / preSize * 1.2 + 1) * windowSize;

        LARGE_INTEGER size;
        size.QuadPart = fileSize;
        if (!SetFilePointerEx(fileHandle, size, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) {
            errorCode = WINDOWS_FILE_SIZE_SETTING_FAILED;
            return;
        }

        //create file map
        mapHandle = CreateFileMappingA(
                fileHandle,
                nullptr,
                PAGE_READWRITE,
                DWORD(fileOffset >> 32),
                DWORD(fileOffset & 0xFFFFFFFF),
                nullptr
        );
        if (!mapHandle) {
            errorCode = WINDOWS_MMAP_CREATE_FAILED;
            CloseHandle(fileHandle);
            return;
        }
#else
        fileHandle = open((filePath + ".dat").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fileHandle == -1) {
            errorCode = LINUX_FILE_OPEN_FAILED;
            std::cout << "open failed: " << strerror(errno) << std::endl;
            return;
        }

        std::cout << "finish file create" << std::endl;
        uint32_t preSize = windowSize / (chnNum * sizeof(double));
        fileSize = static_cast<uint64_t>(frequency * recordTime / preSize * 1.2 + 1) * windowSize;

        if (ftruncate(fileHandle, fileSize) == -1)
            errorCode = LINUX_FILE_SIZE_SETTING_FAILED;
#endif
    }

    void WriteData(std::vector<double> &vec_dbl) {
        if (vec_dbl.size() != chnNum + 2) {
            errorCode = INPUT_CHANNEL_NAME_ERROR;
            return;
        }

#ifdef _WIN32
        if (!windowsMap) {
            windowsMap = MapViewOfFile(
                    mapHandle,
                    FILE_MAP_ALL_ACCESS,
                    DWORD(fileOffset >> 32),
                    DWORD(fileOffset & 0xFFFFFFFF),
                    windowSize
            );
            if (!windowsMap) {
                errorCode = WINDOWS_MMAP_CREATE_FAILED;
                return;
            }
        }

        if (windowSize - windowOffset <= chnNum * sizeof(double)) {
            auto *buffer = reinterpret_cast<double *>(windowsMap);
            memcpy(buffer + windowOffset / sizeof(double), vec_dbl.data(), windowSize - windowOffset);
            fileOffset += windowSize;
            if (fileOffset >= fileSize) {
                fileOffset = 0;
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
                errorCode = WINDOWS_MMAP_CREATE_FAILED;
                return;
            }
            buffer = reinterpret_cast<double *>(windowsMap);

            memcpy(buffer, vec_dbl.data() + (windowSize - windowOffset) / sizeof(double), vec_dbl.size() * sizeof(double) - windowSize + windowOffset);
            windowOffset = vec_dbl.size() * sizeof(double) - windowSize + windowOffset; // reset windowOffset
        } else {
            auto *buffer = reinterpret_cast<double *>(windowsMap);
            memcpy(buffer + windowOffset / sizeof(double), vec_dbl.data(), vec_dbl.size() * sizeof(double));
            windowOffset += (chnNum + 2) * sizeof(double);
        }
#else
        if (!windowsMap) {
            windowsMap = mmap(
                    nullptr,
                    windowSize,
                    PROT_WRITE,
                    MAP_SHARED,
                    fileHandle,
                    fileOffset
            );
            if (!windowsMap) {
                std::cout << "mmap failed" << std::endl;
                errorCode = LINUX_MMAP_CREATE_FAILED;
                return;
            }
        }

        if (windowSize - windowOffset <= chnNum * sizeof(double)) {
            auto *buffer = reinterpret_cast<double *>(windowsMap);
            memcpy(buffer + windowOffset / sizeof(double), vec_dbl.data(), windowSize - windowOffset);
            fileOffset += windowSize;
            if (fileOffset >= fileSize) {
                fileOffset = 0;
            }
            if (msync(windowsMap, windowSize, MS_SYNC) == -1) {
                errorCode = LINUX_MMAP_CLOSE_FAILED;
                return;
            }
            if (munmap(windowsMap, windowSize) == -1) {
                errorCode = LINUX_MMAP_CLOSE_FAILED;
                return;
            }
            windowsMap = mmap(
                    nullptr,
                    windowSize,
                    PROT_WRITE,
                    MAP_SHARED,
                    fileHandle,
                    fileOffset
            );
            if (!windowsMap) {
                errorCode = LINUX_MMAP_CREATE_FAILED;
                return;
            }
            buffer = reinterpret_cast<double *>(windowsMap);

            memcpy(buffer, vec_dbl.data() + (windowSize - windowOffset) / sizeof(double), vec_dbl.size() * sizeof(double) - windowSize + windowOffset);
            windowOffset = vec_dbl.size() * sizeof(double) - windowSize + windowOffset; // reset windowOffset
        } else {
            auto *buffer = reinterpret_cast<double *>(windowsMap);
            memcpy(buffer + windowOffset / sizeof(double), vec_dbl.data(), vec_dbl.size() * sizeof(double));
            windowOffset += (chnNum + 2) * sizeof(double);
        }
#endif
    }

    int GetReadFileOffset(std::chrono::system_clock::time_point &beginTime, std::chrono::system_clock::time_point &stopTime) {
        std::shared_ptr<ReadConfig> readConfig = std::make_shared<ReadConfig>();
        readConfig->fileOffset = fileOffset;
        readConfig->posTime = beginTime;
        uint64_t beginOffset = fileOffset;

#ifdef _WIN32
        while (true) {
            if (fileOffset == windowSize) {
                auto mHandle = MapViewOfFile(
                        mapHandle,
                        FILE_MAP_READ,
                        DWORD(readConfig->fileOffset >> 32),
                        DWORD(readConfig->fileOffset & 0xFFFFFFFF),
                        windowSize
                );
                if(!mHandle)
                    return WINDOWS_MMAP_CREATE_FAILED;
                auto buffer = reinterpret_cast<double *>(mHandle);

                for(uint32_t i = 0; i < windowSize / sizeof(double); i++) {
                    if(CHECK_TOC(&buffer[i])){
                        i++;
                        if(i >= windowSize / sizeof(double))
                            break;
                        auto timePoint = deserialize_time_point(buffer[i]);
                        if (timePoint <= beginTime) {
                            while(timePoint < beginTime) {
                                i += chnNum + 2;
                                if(i >= windowSize / sizeof(double))
                                    break;
                                timePoint = deserialize_time_point(buffer[i]);
                            }
                            readConfig->windowOffset = i - chnNum - 3;
                            readConfigList.push_back(readConfig);
                            return static_cast<int>(readConfigList.size() - 1);
                        } else
                            i += chnNum;
                    }
                }
                if(mHandle)
                    UnmapViewOfFile(mHandle);
                return OFFSET_GET_FAILED;
            } else {
                auto mHandle = MapViewOfFile(
                        mapHandle,
                        FILE_MAP_READ,
                        DWORD(readConfig->fileOffset >> 32),
                        DWORD(readConfig->fileOffset & 0xFFFFFFFF),
                        windowSize
                );

                auto buffer = reinterpret_cast<double *>(mHandle);

                for (uint32_t i = 0; i < windowSize / sizeof(double); i++) {
                    if (CHECK_TOC(&buffer[i])) {
                        i++;
                        if(i >= windowSize / sizeof(double))
                            break;
                        auto timePoint = deserialize_time_point(buffer[i]);
                        auto begin = timePoint;
                        if (timePoint <= beginTime) {
                            while (timePoint < beginTime && timePoint >= begin) {
                                i += chnNum + 2;
                                if(i >= windowSize / sizeof(double))
                                    break;
                                timePoint = deserialize_time_point(buffer[i]);
                            }
                            if(i == chnNum + 2)
//                            std::cout << "window size: " << readConfig->windowOffset << std::endl;
                                readConfig->windowOffset = 0;
                            else
                                readConfig->windowOffset = i - chnNum - 3;
                            readConfigList.push_back(readConfig);
                            return static_cast<int>(readConfigList.size() - 1);
                        } else
                            i += chnNum;
                    }
                }
                if (mHandle) {
                    UnmapViewOfFile(mHandle);
                }
                if (readConfig->fileOffset == 0)
                    readConfig->fileOffset = fileSize;
                readConfig->fileOffset -= windowSize;
                if (readConfig->fileOffset == beginOffset)
                    return OFFSET_GET_FAILED;
            }
        }
#else
        while (true) {
            if (fileSize == windowSize) {
                auto mHandle = mmap(
                        nullptr,
                        windowSize,
                        PROT_READ,
                        MAP_SHARED,
                        fileHandle,
                        readConfig->fileOffset
                );
                if (!mHandle) {
                    if (msync(mHandle, windowSize, MS_SYNC) == -1)
                        return LINUX_MMAP_CLOSE_FAILED;
                    if (munmap(mHandle, windowSize) == -1)
                        return LINUX_MMAP_CLOSE_FAILED;
                }

                auto buffer = reinterpret_cast<double *>(mHandle);

                for (uint32_t i = 0; i < windowSize / sizeof(double); i++) {
                    if (CHECK_TOC(&buffer[i])) {
                        i++;
                        if (i >= windowSize / sizeof(double))
                            break;
                        auto timePoint = deserialize_time_point(buffer[i]);
                        if (timePoint <= beginTime) {
                            while (timePoint < beginTime) {
                                i += chnNum + 2;
                                if (i >= windowSize / sizeof(double))
                                    break;
                                timePoint = deserialize_time_point(buffer[i]);
                            }
                            readConfig->windowOffset = i - chnNum - 3;
                            readConfigList.push_back(readConfig);
                            return static_cast<int>(readConfigList.size() - 1);
                        } else
                            i += chnNum;
                    }
                }
                if (mHandle) {
                    if (msync(mHandle, windowSize, MS_SYNC) == -1)
                        return LINUX_MMAP_CLOSE_FAILED;
                    if (munmap(mHandle, windowSize) == -1)
                        return LINUX_MMAP_CLOSE_FAILED;
                }
                return OFFSET_GET_FAILED;
            } else {
                auto mHandle = mmap(
                        nullptr,
                        windowSize,
                        PROT_READ,
                        MAP_SHARED,
                        fileHandle,
                        readConfig->fileOffset
                );
                if (!mHandle)
                    return LINUX_MMAP_CREATE_FAILED;

                auto buffer = reinterpret_cast<double *>(mHandle);

                for (uint32_t i = 0; i < windowSize / sizeof(double); i++) {
                    if (CHECK_TOC(&buffer[i])) {
                        i++;
                        if (i >= windowSize / sizeof(double))
                            break;
                        auto timePoint = deserialize_time_point(buffer[i]);
                        if (timePoint <= beginTime) {
                            while (timePoint < beginTime) {
                                i += chnNum + 2;
                                if (i >= windowSize / sizeof(double))
                                    break;
                                timePoint = deserialize_time_point(buffer[i]);
                            }
                            std::cout << "window size: " << readConfig->windowOffset << std::endl;
                            readConfig->windowOffset = i - chnNum - 3;
                            readConfigList.push_back(readConfig);
                            return static_cast<int>(readConfigList.size() - 1);
                        } else
                            i += chnNum;
                    }
                }
                if (mHandle) {
                    if (msync(mHandle, windowSize, MS_SYNC) == -1)
                        return LINUX_MMAP_CLOSE_FAILED;
                    if (munmap(mHandle, windowSize) == -1)
                        return LINUX_MMAP_CLOSE_FAILED;
                }
                if (readConfig->fileOffset == 0)
                    readConfig->fileOffset = fileSize;
                else
                    readConfig->fileOffset -= windowSize;
                if (readConfig->fileOffset == beginOffset)
                    return OFFSET_GET_FAILED;
            }
        }
#endif
    }

    int ReadData(int handle, std::vector<std::vector<double>> &vec_dbl, uint32_t vec_size) {
//        std::cout << "double data read" << std::endl;
        std::chrono::system_clock::time_point timePoint;
        std::chrono::system_clock::time_point minTimePoint = std::chrono::system_clock::from_time_t(0);
//        std::cout << "posTime: " << TransformStrTimeMS(readConfigList[handle]->posTime) << std::endl;
        minTimePoint = std::chrono::system_clock::from_time_t(0);
        uint16_t remain = 0;
        bool isRunning = true;

#ifdef _WIN32
        while (isRunning) {
            if (!readConfigList[handle]->readWindowsMap) {
                readConfigList[handle]->readWindowsMap = MapViewOfFile(
                        mapHandle,
                        FILE_MAP_READ,
                        DWORD(readConfigList[handle]->fileOffset >> 32),
                        DWORD(readConfigList[handle]->fileOffset & 0xFFFFFFFF),
                        windowSize
                );
                if (!readConfigList[handle]->readWindowsMap)
                    return WINDOWS_MMAP_CREATE_FAILED;
            }

            auto *buffer = reinterpret_cast<double *>(readConfigList[handle]->readWindowsMap);

//            std::cout << "read config window size: " << readConfigList[handle]->windowOffset << std::endl;
            for (uint32_t i = readConfigList[handle]->windowOffset; i < windowSize / sizeof(double);) {
                if (remain) {
//                    std::cout << "remain: " << remain << std::endl;
                    readConfigList[handle]->windowOffset = remain;
                    if (isTime) {
                        timePoint = deserialize_time_point(buffer[i]);
                        if (timePoint > minTimePoint)
                            minTimePoint = timePoint;
                        else
                            return -2;
                        i++;
                        while (readConfigList[handle]->posTime < timePoint) {
                            vec_dbl.push_back(vec_buffer);
                            if (vec_dbl.size() >= vec_size)
                                return OPTION_OK;
                            readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
//                            std::cout << "posTime: " << TransformStrTimeMS(posTime) << std::endl;
                        }
                        vec_buffer.clear();
                        for (int j = 0; j < chnNum; j++)
                            vec_buffer.push_back(buffer[i + j]);
                        i += chnNum;
                    } else {
                        for (int j = 0; j < remain; j++)
                            vec_buffer.push_back(buffer[i + j]);
                    }
                }
                if (buffer[i] == 0) {
//                    std::cout << buffer[i] << std::endl;
                    while (vec_dbl.size() < vec_size) {
                        vec_dbl.push_back(vec_buffer);
                        readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
//                        std::cout << "posTime: " << TransformStrTimeMS(posTime) << std::endl;
                    }
                    return OPTION_OK;
                }
                if (isnan(buffer[i])) {
                    i++;
                    if (i < windowSize / sizeof(double)) {
                        isTime = false;
                        timePoint = deserialize_time_point(buffer[i]);
//                        std::cout << "timePoint" << TransformStrTimeMS(timePoint) << std::endl;
                        if (timePoint > minTimePoint)
                            minTimePoint = timePoint;
                        else
                            return -2;
                    } else {
                        isTime = true;
                        break;
                    }
                    i++;
                    if (i < windowSize / sizeof(double) - chnNum) {
                        while (readConfigList[handle]->posTime < timePoint) {
                            vec_dbl.push_back(vec_buffer);
                            if (vec_dbl.size() >= vec_size)
                                return OPTION_OK;
                            readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
//                            std::cout << "posTime: " << TransformStrTimeMS(posTime) << std::endl;
                        }
                        vec_buffer.clear();
                        for (int j = 0; j < chnNum; j++)
                            vec_buffer.push_back(buffer[i + j]);
                        i += chnNum;
                    } else {
                        while (readConfigList[handle]->posTime < timePoint) {
                            vec_dbl.push_back(vec_buffer);
                            if (vec_dbl.size() >= vec_size)
                                return OPTION_OK;
                            readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
//                            std::cout << "posTime: " << TransformStrTimeMS(posTime) << std::endl;
                        }
                        remain = windowSize / sizeof(double) - i;
                        vec_buffer.clear();
                        for (int j = 0; j < remain; j++)
                            vec_buffer.push_back(buffer[i + j]);
                        i += remain;
                        remain = chnNum - remain;
                    }
                } else {
                    i++;
                }
            }

            if (readConfigList[handle]->readWindowsMap) {
                UnmapViewOfFile(readConfigList[handle]->readWindowsMap);
                readConfigList[handle]->readWindowsMap = nullptr;
            }
            if(readConfigList[handle]->fileOffset != windowSize) {
                readConfigList[handle]->fileOffset += windowSize;
                if (readConfigList[handle]->fileOffset == fileSize)
                    readConfigList[handle]->fileOffset = 0;
            }
            readConfigList[handle]->windowOffset = 0;
        };
#else
        while (isRunning) {
            if (!readConfigList[handle]->readWindowsMap) {
                readConfigList[handle]->readWindowsMap = mmap(
                        nullptr,
                        windowSize,
                        PROT_READ,
                        MAP_SHARED,
                        fileHandle,
                        readConfigList[handle]->fileOffset
                );
                if (!readConfigList[handle]->readWindowsMap) {
                    return LINUX_MMAP_CREATE_FAILED;
                }
            }

            auto *buffer = reinterpret_cast<double *>(readConfigList[handle]->readWindowsMap);

            std::cout << "read config window size: " << readConfigList[handle]->windowOffset << std::endl;
            for (uint32_t i = readConfigList[handle]->windowOffset; i < windowSize / sizeof(double);) {
                if (remain) {
                    std::cout << "remain: " << remain << std::endl;
                    readConfigList[handle]->windowOffset = remain;
                    if (isTime) {
                        timePoint = deserialize_time_point(buffer[i]);
                        if (timePoint > minTimePoint)
                            minTimePoint = timePoint;
                        else
                            return TIME_POS_ERROR;
                        i++;
                        while (readConfigList[handle]->posTime < timePoint) {
                            vec_dbl.push_back(vec_buffer);
                            if (vec_dbl.size() >= vec_size)
                                return OPTION_OK;
                            readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
                        }
                        vec_buffer.clear();
                        for (int j = 0; j < chnNum; j++)
                            vec_buffer.push_back(buffer[i + j]);
                        i += chnNum;
                    } else {
                        for (int j = 0; j < remain; j++)
                            vec_buffer.push_back(buffer[i + j]);
                    }
                }
                if (buffer[i] == 0) {
                    std::cout << "buffer value: " << buffer << std::endl;
                    while (vec_dbl.size() < vec_size) {
                        vec_dbl.push_back(vec_buffer);
                        readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
                    }
                    return OPTION_OK;
                }
                if (std::isnan(buffer[i])) {
                    i++;
                    if (i < windowSize / sizeof(double)) {
                        isTime = false;
                        timePoint = deserialize_time_point(buffer[i]);
                        if (timePoint > minTimePoint)
                            minTimePoint = timePoint;
                        else
                            return TIME_POS_ERROR;
                    } else {
                        isTime = true;
                        break;
                    }
                    i++;
                    if (i < windowSize / sizeof(double) - chnNum) {
                        while (readConfigList[handle]->posTime < timePoint) {
                            vec_dbl.push_back(vec_buffer);
                            if (vec_dbl.size() >= vec_size)
                                return OPTION_OK;
                            readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
                        }
                        vec_buffer.clear();
                        for (int j = 0; j < chnNum; j++)
                            vec_buffer.push_back(buffer[i + j]);
                        i += chnNum;
                    } else {
                        while (readConfigList[handle]->posTime < timePoint) {
                            vec_dbl.push_back(vec_buffer);
                            if (vec_dbl.size() >= vec_size)
                                return OPTION_OK;
                            readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
                        }
                        remain = windowSize / sizeof(double) - i;
                        vec_buffer.clear();
                        for (int j = 0; j < remain; j++)
                            vec_buffer.push_back(buffer[i + j]);
                        i += remain;
                        remain = chnNum - remain;
                    }
                } else {
                    i++;
                }
            }
            if (readConfigList[handle]->readWindowsMap) {
                if (msync(readConfigList[handle]->readWindowsMap, windowSize, MS_SYNC) == -1)
                    return LINUX_MMAP_CLOSE_FAILED;
                if (munmap(readConfigList[handle]->readWindowsMap, windowSize) == -1)
                    return LINUX_MMAP_CLOSE_FAILED;
                readConfigList[handle]->readWindowsMap = nullptr;
            }

            if (readConfigList[handle]->fileOffset != windowSize) {
                readConfigList[handle]->fileOffset += windowSize;
                if (readConfigList[handle]->fileOffset == fileSize)
                    readConfigList[handle]->fileOffset = 0;
            }
            readConfigList[handle]->windowOffset = 0;
        }
#endif
        return OPTION_OK;
    }

    ~CircularMappedBufferDBL() {
#ifdef _WIN32
        for (const auto &rc: readConfigList)
            if (rc->readWindowsMap)
                UnmapViewOfFile(rc->readWindowsMap);
        if (windowsMap)
            UnmapViewOfFile(windowsMap);
        CloseHandle(mapHandle);
        CloseHandle(fileHandle);
#else
        for (const auto &rc: readConfigList)
            if (rc->readWindowsMap) {
                if (msync(rc->readWindowsMap, windowSize, MS_SYNC) == -1) {
                    errorCode = LINUX_MMAP_CLOSE_FAILED;
                    return;
                }
                if (munmap(rc->readWindowsMap, windowSize)) {
                    errorCode = LINUX_MMAP_CLOSE_FAILED;
                    return;
                }
            }
        if (msync(windowsMap, windowSize, MS_SYNC) == -1) {
            errorCode = LINUX_MMAP_CLOSE_FAILED;
            return;
        }
        if (munmap(windowsMap, windowSize)) {
            errorCode = LINUX_MMAP_CLOSE_FAILED;
            return;
        }
        close(fileHandle);
#endif
    }

    [[nodiscard]]int32_t GetErrorCord() const { return errorCode; }

    void ResetErrorCord() { errorCode = 0; }
};

class CircularMappedBufferSTR {
private:
#ifdef _WIN32
    HANDLE fileHandle;
    HANDLE mapHandle;
#else
    int fileHandle;
#endif
    uint16_t chnNum = 0;
    uint16_t frequency = 0;
    uint64_t fileSize = 0;
    uint64_t fileOffset = 0;
    uint32_t windowSize = 64 * 1024;
    uint32_t windowOffset = 0;
    void *windowsMap = nullptr;
    int32_t errorCode = 0;
    std::string toc{'M', 'D', 'A', 'T', 'A', 's', static_cast<char>(0xF8), static_cast<char>(0x7F)};
    std::vector<std::shared_ptr<ReadConfig>> readConfigList;
    std::vector<std::string> vec_buffer;

public:
    CircularMappedBufferSTR(uint16_t chnNum, uint16_t frequency, uint16_t recordTime, const std::string &filePath) : chnNum(chnNum), frequency(frequency) {
        //设置单条数据缓存大小
        vec_buffer.resize(chnNum);
        //创建内存映射文件
#ifdef _WIN32
        fileHandle = CreateFileA(
                (filePath + ".dat").c_str(),
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                nullptr,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                nullptr
        );
        if (fileHandle == INVALID_HANDLE_VALUE) {
            errorCode = WINDOWS_FILE_OPEN_FAILED;
            return;
        }

        //设置内存映射文件大小
        uint32_t preSize = windowSize / (chnNum * sizeof(char[64]));
        fileSize = static_cast<uint64_t>(frequency * recordTime / preSize * 1.2 + 1) * windowSize * 8;

        LARGE_INTEGER size;
        size.QuadPart = fileSize;
        if (!SetFilePointerEx(fileHandle, size, nullptr, FILE_BEGIN) || !SetEndOfFile(fileHandle)) {
            errorCode = WINDOWS_FILE_SIZE_SETTING_FAILED;
            return;
        }

        //创建文件映射
        mapHandle = CreateFileMappingA(
                fileHandle,
                nullptr,
                PAGE_READWRITE,
                DWORD(fileSize >> 32),
                DWORD(fileSize & 0xFFFFFFFF),
                nullptr
        );
        if (!mapHandle) {
            errorCode = WINDOWS_MMAP_CREATE_FAILED;
            CloseHandle(fileHandle);
            return;
        }
#else
        fileHandle = open((filePath + ".dat").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fileHandle == -1) {
            errorCode = LINUX_FILE_OPEN_FAILED;
            return;
        }

        uint32_t preSize = windowSize / (chnNum * sizeof(char[64]));
        fileSize = static_cast<uint64_t>(frequency * recordTime / preSize * 1.2 + 1) * windowSize * 8;

        if (ftruncate(fileHandle, fileSize) == -1) {
            errorCode = LINUX_FILE_SIZE_SETTING_FAILED;
            return;
        }
#endif
//        std::cout << "string buffer create finished" << std::endl;
    }

    //写入数据
    void WriteData(std::vector<std::string> &vec_str) {
        //确保输入数据的通道数量正确
        if (vec_str.size() != chnNum + 1) {
            errorCode = INPUT_CHANNEL_NAME_ERROR;
            return;
        }

#ifdef _WIN32
        //创建内存映射窗口
        if (!windowsMap) {
            windowsMap = MapViewOfFile(
                    mapHandle,
                    FILE_MAP_ALL_ACCESS,
                    DWORD(fileOffset >> 32),
                    DWORD(fileOffset & 0xFFFFFFFF),
                    windowSize
            );
            if (!windowsMap) {
                errorCode = WINDOWS_MMAP_CREATE_FAILED;
                return;
            }
        }

        if (windowSize - windowOffset <= (chnNum + 1) * sizeof(char[64])) {
            auto *buffer = reinterpret_cast<char *>(windowsMap);
            auto index = (windowSize - windowOffset) / sizeof(char[64]);

            for (uint32_t i = 0; windowOffset < windowSize; i++) {
                memset(buffer + windowOffset, 0, 64);
                memcpy(buffer + windowOffset, vec_str[i].c_str(), min(vec_str[i].size(), 63));
                windowOffset += 64;
            }
//            memcpy(buffer + windowOffset / sizeof(char[64]), vec_str.data()->c_str(), windowSize - windowOffset);
            fileOffset += windowSize;
            if (fileOffset >= fileSize)
                fileOffset = 0;

            UnmapViewOfFile(windowsMap);
            windowsMap = MapViewOfFile(
                    mapHandle,
                    FILE_MAP_ALL_ACCESS,
                    DWORD(fileOffset >> 32),
                    DWORD(fileOffset & 0xFFFFFFFF),
                    windowSize
            );
            if (!windowsMap) {
                errorCode = WINDOWS_MMAP_CREATE_FAILED;
                return;
            }
            buffer = reinterpret_cast<char *>(windowsMap);

            windowOffset = 0;
            for (uint32_t i = index; i < vec_str.size(); i++) {
                memset(buffer + windowOffset, 0, 64);
                memcpy(buffer + windowOffset, vec_str[i].c_str(), min(vec_str[i].size(), 63));
                windowOffset += 64;
            }
        } else {
            auto *buffer = reinterpret_cast<char *>(windowsMap);
            for (const auto &str: vec_str) {
                memset(buffer + windowOffset, 0, 64);
                memcpy(buffer + windowOffset, str.c_str(), min(str.size(), 63));
                windowOffset += 64;
            }
        }
#else
        if (!windowsMap) {
            windowsMap = mmap(
                    nullptr,
                    windowSize,
                    PROT_WRITE,
                    MAP_SHARED,
                    fileHandle,
                    fileOffset
            );
            if (windowsMap == MAP_FAILED) {
                errorCode = LINUX_MMAP_CREATE_FAILED;
                return;
            }
        }

        if (windowSize - windowOffset <= (chnNum + 1) * sizeof(char[64])) {
            auto *buffer = reinterpret_cast<char *>(windowsMap);
            auto index = (windowSize - windowOffset) / sizeof(char[64]);
            for (uint32_t i = 0; windowOffset < windowSize; i++) {
                memset(buffer + windowOffset, 0, 64);
                memcpy(buffer + windowOffset, vec_str[i].c_str(), std::min(vec_str[i].size(), static_cast<size_t>(63)));
                windowOffset += 64;
            }
//            memcpy(buffer + windowOffset / sizeof(char[64]), vec_str.data()->c_str(), windowSize - windowOffset);
            fileOffset += windowSize;
            if (fileOffset >= fileSize)
                fileOffset = 0;

            if (msync(windowsMap, windowSize, MS_SYNC)) {
                errorCode = LINUX_MMAP_CLOSE_FAILED;
                return;
            }
            if (munmap(windowsMap, windowSize)) {
                errorCode = LINUX_MMAP_CLOSE_FAILED;
                return;
            }
            windowsMap = mmap(
                    nullptr,
                    windowSize,
                    PROT_WRITE,
                    MAP_SHARED,
                    fileHandle,
                    fileOffset
            );
            if (windowsMap == MAP_FAILED) {
                errorCode = LINUX_MMAP_CREATE_FAILED;
                return;
            }
            buffer = reinterpret_cast<char *>(windowsMap);

            windowOffset = 0;
            for (uint32_t i = index; i < vec_str.size(); i++) {
                memset(buffer + windowOffset, 0, 64);
                memcpy(buffer + windowOffset, vec_str[i].c_str(), std::min(vec_str[i].size(), static_cast<size_t>(63)));
                windowOffset += 64;
            }
        } else {
            auto *buffer = reinterpret_cast<char *>(windowsMap);
            for (const auto &str: vec_str) {
                memset(buffer + windowOffset, 0, 64);
                memcpy(buffer + windowOffset, str.c_str(), std::min(str.size(), static_cast<size_t>(63)));
                windowOffset += 64;
            }
        }
#endif
    }

    //查找起始读取FileOffset
    int GetReadFileOffset(std::chrono::system_clock::time_point beginTime, std::chrono::system_clock::time_point stopTime) {
//        std::cout << "string offset begin time: " << TransformStrTimeMS(beginTime) << std::endl;
        std::shared_ptr<ReadConfig> readConfig = std::make_shared<ReadConfig>();
        readConfig->fileOffset = fileOffset;
        readConfig->posTime = beginTime;
        std::cout << "begin: " << TransformStrTimeMS(beginTime) << std::endl;
        uint64_t beginOffset = fileOffset;

#ifdef _WIN32
        while (true) {
//            if (fileSize == windowSize) {
//                auto mHandle = MapViewOfFile(
//                        mapHandle,
//                        FILE_MAP_ALL_ACCESS,
//                        DWORD(readConfig->fileOffset >> 32),
//                        DWORD(readConfig->fileOffset & 0xFFFFFFFF),
//                        windowSize
//                );
//                if(!mHandle) {
//                    UnmapViewOfFile(mHandle);
//                    return LINUX_MMAP_CREATE_FAILED;
//                }
//
//                auto buffer = reinterpret_cast<char *>(mHandle);
//
//                for(uint32_t i = 0; i < windowSize / sizeof(char[64]); i++) {
//                    if(CHECK_TOC(buffer + i * sizeof(char[64]))){
//                        int64_t tick{};
//                        memcpy(&tick, buffer + i * sizeof(char[64]) + 8, sizeof(tick));
//                        auto timePoint = std::chrono::system_clock::time_point(std::chrono::microseconds(tick));
////                        std::cout << "time point: " << TransformStrTimeMS(timePoint) << std::endl;
//                        if (timePoint <= beginTime) {
//                            while(timePoint < beginTime) {
//                                i += chnNum + 1;
//                                memcpy(&tick, buffer + i * sizeof(char[64]) + 8, sizeof(tick));
//                                timePoint = std::chrono::system_clock::time_point(std::chrono::microseconds(tick));
//                            }
//                            readConfig->windowOffset = i - chnNum - 2;
//                            readConfigList.push_back(readConfig);
//                            return static_cast<int>(readConfigList.size() - 1);
//                        } else
//                            i += chnNum;
//                    }
//                }
//                if(mHandle) {
//                    UnmapViewOfFile(mHandle);
//                }
//                return OFFSET_GET_FAILED;
//            } else {
                auto mHandle = MapViewOfFile(
                        mapHandle,
                        FILE_MAP_ALL_ACCESS,
                        DWORD(readConfig->fileOffset >> 32),
                        DWORD(readConfig->fileOffset & 0xFFFFFFFF),
                        windowSize
                );
                if(!mHandle)
                    return LINUX_MMAP_CREATE_FAILED;

                auto buffer = reinterpret_cast<char *>(mHandle);

                for (uint32_t i = 0; i < windowSize / sizeof(char[64]); i++) {
                    if (CHECK_TOC(buffer + i * sizeof(char[64]))) {
                        int64_t tick{};
                        memcpy(&tick, buffer + i * sizeof(char[64]) + 8, sizeof(tick));
                        auto timePoint = std::chrono::system_clock::time_point(std::chrono::microseconds(tick));
                        std::cout << "time point: " << TransformStrTimeMS(timePoint) << std::endl;
                        auto begin = timePoint;  // 防止越界
                        if (timePoint <= beginTime) {
                            while (timePoint < beginTime && timePoint >= begin) {
                                i += chnNum + 1;
                                memcpy(&tick, buffer + i * sizeof(char[64]) + 8, sizeof(tick));
                                timePoint = std::chrono::system_clock::time_point(std::chrono::microseconds(tick));
                                std::cout << "time point: " << TransformStrTimeMS(timePoint) << std::endl;
                            }
                            if(i == chnNum + 1)
                                readConfig->windowOffset = 0;
                            else
                                readConfig->windowOffset = i - chnNum - 2;
                            readConfigList.push_back(readConfig);
                            return static_cast<int>(readConfigList.size() - 1);
                        } else
                            i += chnNum;
                    }
                }
                if (mHandle) {
                    UnmapViewOfFile(mHandle);
                }
                if (readConfig->fileOffset == 0)
                    readConfig->fileOffset = fileSize;
                else
                    readConfig->fileOffset -= windowSize;
                if (readConfig->fileOffset == beginOffset)
                    return OFFSET_GET_FAILED;
//            }
        }
#else
        while (true) {
//            std::cout << "fileOffset: " << readConfig->fileOffset << std::endl;
//            if (fileSize == windowSize) {
//                auto mHandle = mmap(
//                        nullptr,
//                        windowSize,
//                        PROT_READ,
//                        MAP_SHARED,
//                        fileHandle,
//                        readConfig->fileOffset
//                );
//                std::cout << "mmap create finish" << std::endl;
//                if(!mHandle) {
//                    std::cout << "mmap create failed" << std::endl;
//                    if (msync(mHandle, windowSize, MS_SYNC) == -1)
//                        return LINUX_MMAP_CLOSE_FAILED;
//                    if (munmap(mHandle, windowSize) == -1)
//                        return LINUX_MMAP_CLOSE_FAILED;
//                }
//
//                auto buffer = reinterpret_cast<char *>(mHandle);
//
//                std::cout << "window size" << std::endl;
//                for(uint32_t i = 0; i < windowSize / sizeof(char[64]); i++) {
//                    std::cout << "i: " << i << std::endl;
//                    if(CHECK_TOC(buffer + i * sizeof(char[64]))){
//                        int64_t tick{};
//                        memcpy(&tick, buffer + i * sizeof(int64_t) + 8, sizeof(tick));
//                        auto timePoint = std::chrono::system_clock::time_point(std::chrono::microseconds(tick));
//                        if (timePoint <= beginTime) {
//                            while(timePoint < beginTime) {
//                                i += chnNum + 1;
//                                memcpy(&tick, buffer + i * sizeof(int64_t) + 8, sizeof(tick));
//                                timePoint = std::chrono::system_clock::time_point(std::chrono::microseconds(tick));
//                            }
//                            readConfig->windowOffset = i - chnNum - 2;
//                            readConfigList.push_back(readConfig);
//                            return static_cast<int>(readConfigList.size() - 1);
//                        } else
//                            i += chnNum;
//                    }
//                }
//                if(mHandle) {
//                    if (msync(mHandle, windowSize, MS_SYNC) == -1)
//                        return LINUX_MMAP_CLOSE_FAILED;
//                    if (munmap(mHandle, windowSize) == -1)
//                        return LINUX_MMAP_CLOSE_FAILED;
//                }
//                return OFFSET_GET_FAILED;
//            } else {
            auto mHandle = mmap(
                    nullptr,
                    windowSize,
                    PROT_READ,
                    MAP_SHARED,
                    fileHandle,
                    readConfig->fileOffset
            );
            if (mHandle == MAP_FAILED)
                return LINUX_MMAP_CREATE_FAILED;

            auto buffer = reinterpret_cast<char *>(mHandle);

            for (uint32_t i = 0; i < windowSize / sizeof(char[64]); i++) {
                if (CHECK_TOC(buffer + i * sizeof(char[64]))) {
                    int64_t tick{};
                    memcpy(&tick, buffer + i * sizeof(char[64]) + 8, sizeof(tick));
                    auto timePoint = std::chrono::system_clock::time_point(std::chrono::microseconds(tick));
                    if (timePoint <= beginTime) {
                        while (timePoint < std::chrono::floor<std::chrono::milliseconds>(beginTime)) {
                            i += chnNum + 1;
                            memcpy(&tick, buffer + i * sizeof(char[64]) + 8, sizeof(tick));
                            timePoint = std::chrono::system_clock::time_point(std::chrono::microseconds(tick));
                        }
                        if (i == 0)
                            readConfig->windowOffset = 0;
                        else
                            readConfig->windowOffset = i - chnNum - 2;
                        std::cout << "window offset: " << readConfig->windowOffset << std::endl;
                        readConfigList.push_back(readConfig);
                        return static_cast<int>(readConfigList.size() - 1);
                    } else
                        i += chnNum;
                }
            }
            if (mHandle) {
                if (msync(mHandle, windowSize, MS_SYNC) == -1)
                    return LINUX_MMAP_CLOSE_FAILED;
                if (munmap(mHandle, windowSize) == -1)
                    return LINUX_MMAP_CLOSE_FAILED;
            }
            if (readConfig->fileOffset == 0)
                readConfig->fileOffset = fileSize;
            else
                readConfig->fileOffset -= windowSize;
            if (readConfig->fileOffset == beginOffset)
                return OFFSET_GET_FAILED;
//            }
        }
#endif
        return static_cast<int32_t>(readConfigList.size()) - 1;
    }

    int ReadData(int handle, std::vector<std::vector<std::string>> &vec_str, uint32_t vec_size) {
//        std::cout << "string data read" << std::endl;
        std::chrono::system_clock::time_point timePoint;
        std::chrono::system_clock::time_point minTimePoint = std::chrono::system_clock::from_time_t(0);
        char str_buffer[64];
        uint16_t remain = 0;
        bool isRunning = true;

#ifdef _WIN32
        while (isRunning) {
            if (!readConfigList[handle]->readWindowsMap) {
                readConfigList[handle]->readWindowsMap = MapViewOfFile(
                        mapHandle,
                        FILE_MAP_READ,
                        DWORD(readConfigList[handle]->fileOffset >> 32),
                        DWORD(readConfigList[handle]->fileOffset & 0xFFFFFFFF),
                        windowSize
                );
                if (!readConfigList[handle]->readWindowsMap)
                    return WINDOWS_MMAP_CREATE_FAILED;
            }

            auto *buffer = reinterpret_cast<char *>(readConfigList[handle]->readWindowsMap);

            for (uint32_t i = readConfigList[handle]->windowOffset; i < windowSize / sizeof(char[64]);) {
                if (remain) {
                    readConfigList[handle]->windowOffset = remain;
                    for (int j = 0; j < remain; j++) {
                        memcpy(str_buffer, buffer + (i + j) * sizeof(char[64]), sizeof(str_buffer));
                        vec_buffer.emplace_back(str_buffer);
                    }
                }
                if (isStringNull(buffer + i * sizeof(char[64])) == 0) {
                    while (vec_str.size() < vec_size) {
                        vec_str.push_back(vec_buffer);
                        readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
                    }
                    return OPTION_OK;
                }
                std::string str_toc(buffer + i * sizeof(char[64]), buffer + i * sizeof(char[64]) + 8);
                if (str_toc == toc) {
                    int64_t tick{};
                    memcpy(&tick, buffer + i * sizeof(char[64]) + 8, sizeof(tick));
                    timePoint = std::chrono::system_clock::time_point(std::chrono::microseconds(tick));
                    if (timePoint > minTimePoint)
                        minTimePoint = timePoint;
                    else
                        return TIME_POS_ERROR;
                    i++;
                    if (i < windowSize / sizeof(char[64]) - chnNum) {
                        while (readConfigList[handle]->posTime < timePoint) {
                            vec_str.push_back(vec_buffer);
                            if (vec_str.size() >= vec_size)
                                return OPTION_OK;
                            readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
                        }
                        vec_buffer.clear();
                        for (int j = 0; j < chnNum; j++) {
                            memcpy(str_buffer, buffer + (i + j) * sizeof(char[64]), sizeof(str_buffer));
                            vec_buffer.emplace_back(str_buffer);
                        }
                        i += chnNum;
                        remain = 0;
                    } else {
                        while (readConfigList[handle]->posTime < timePoint) {
                            vec_str.push_back(vec_buffer);
                            if (vec_str.size() >= vec_size)
                                return OPTION_OK;
                            readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
                        }
                        remain = windowSize / sizeof(char[64]) - i;
                        vec_buffer.clear();
                        for (int j = 0; j < remain; j++) {
                            memcpy(str_buffer, buffer + (i + j) * sizeof(char[64]), sizeof(str_buffer));
                            vec_buffer.emplace_back(str_buffer);
                        }
                        i += remain;
                        remain = chnNum - remain;
                    }
                } else
                    i++;
            }

            if (readConfigList[handle]->readWindowsMap) {
                UnmapViewOfFile(readConfigList[handle]->readWindowsMap);
                readConfigList[handle]->readWindowsMap = nullptr;
            }
            if(readConfigList[handle]->fileOffset != windowSize) {
                readConfigList[handle]->fileOffset += windowSize;
                if(readConfigList[handle]->fileOffset == fileSize)
                    readConfigList[handle]->fileOffset = 0;
            }
            readConfigList[handle]->windowOffset = 0;
        }
#else
        while (isRunning) {
            if (!readConfigList[handle]->readWindowsMap) {
                readConfigList[handle]->readWindowsMap = mmap(
                        nullptr,
                        windowSize,
                        PROT_READ,
                        MAP_SHARED,
                        fileHandle,
                        readConfigList[handle]->fileOffset
                );
                if (readConfigList[handle]->readWindowsMap == MAP_FAILED)
                    return LINUX_FILE_OPEN_FAILED;
            }

            auto *buffer = reinterpret_cast<char *>(readConfigList[handle]->readWindowsMap);

            for (uint32_t i = readConfigList[handle]->windowOffset; i < windowSize / sizeof(char[64]);) {
                if (remain) {
                    readConfigList[handle]->windowOffset = remain;
                    for (int j = 0; j < remain; j++) {
                        memcpy(str_buffer, buffer + (i + j) * sizeof(char[64]), sizeof(str_buffer));
                        vec_buffer.emplace_back(str_buffer);
                    }
                }
                if (isStringNull(buffer + i * sizeof(char[64])) == 0) {
                    while (vec_str.size() < vec_size) {
                        vec_str.push_back(vec_buffer);
                        readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
                    }
                    return OPTION_OK;
                }
                std::string str_toc(buffer + i * sizeof(char[64]), buffer + i * sizeof(char[64]) + 8);
                if (str_toc == toc) {
                    int64_t tick{};
                    memcpy(&tick, buffer + i * sizeof(char[64]) + 8, sizeof(tick));
                    timePoint = std::chrono::system_clock::time_point(std::chrono::microseconds(tick));
//                    cout << TransformStrTimeMS(timePoint) << endl;
                    if (timePoint > minTimePoint) {
                        minTimePoint = timePoint;
                    } else {
                        return TIME_POS_ERROR;
                    }
                    i++;
                    if (i < windowSize / sizeof(char[64]) - chnNum) {
                        while (readConfigList[handle]->posTime < timePoint) {
                            vec_str.push_back(vec_buffer);
                            if (vec_str.size() >= vec_size)
                                return OPTION_OK;
                            readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
                        }
                        vec_buffer.clear();
                        for (int j = 0; j < chnNum; j++) {
                            memcpy(str_buffer, buffer + (i + j) * sizeof(char[64]), sizeof(str_buffer));
                            vec_buffer.emplace_back(str_buffer);
                        }
                        i += chnNum;
                    } else {
                        while (readConfigList[handle]->posTime < timePoint) {
                            vec_str.push_back(vec_buffer);
                            if (vec_str.size() >= vec_size)
                                return OPTION_OK;
                            readConfigList[handle]->posTime = readConfigList[handle]->posTime + std::chrono::milliseconds(1000 / frequency);
                        }
                        remain = windowSize / sizeof(char[64]) - i;
                        vec_buffer.clear();
                        for (int j = 0; j < remain; j++) {
                            memcpy(str_buffer, buffer + (i + j) * sizeof(char[64]), sizeof(str_buffer));
                            vec_buffer.emplace_back(str_buffer);
                        }
                        i += remain;
                        remain = chnNum - remain;
                    }
                } else {
                    i++;
                }
            }

            if (readConfigList[handle]->readWindowsMap) {
                if (msync(readConfigList[handle]->readWindowsMap, windowSize, MS_SYNC) == -1)
                    return LINUX_MMAP_CLOSE_FAILED;
                if (munmap(readConfigList[handle]->readWindowsMap, windowSize) == -1)
                    return LINUX_MMAP_CLOSE_FAILED;
                readConfigList[handle]->readWindowsMap = nullptr;
            }
            if (readConfigList[handle]->fileOffset != windowSize) {
                readConfigList[handle]->fileOffset += windowSize;
                if (readConfigList[handle]->fileOffset == fileSize)
                    readConfigList[handle]->fileOffset = 0;
            }
            readConfigList[handle]->windowOffset = 0;
        }
#endif
        return OPTION_OK;
    }

    int32_t GetErrorCode() const { return errorCode; }

    void ResetErrorCode() { errorCode = 0; }

    ~CircularMappedBufferSTR() {
#ifdef _WIN32
        for (const auto &rc: readConfigList)
            if (rc->readWindowsMap)
                UnmapViewOfFile(rc->readWindowsMap);
        if (windowsMap)
            UnmapViewOfFile(windowsMap);
        CloseHandle(mapHandle);
        CloseHandle(fileHandle);
#else
        for (const auto &rc: readConfigList)
            if (rc->readWindowsMap && rc->readWindowsMap != MAP_FAILED) {
                if (msync(rc->readWindowsMap, windowSize, MS_SYNC) == -1) {
                    errorCode = LINUX_MMAP_CLOSE_FAILED;
                    return;
                }
                if (munmap(rc->readWindowsMap, windowSize) == -1) {
                    errorCode = LINUX_MMAP_CLOSE_FAILED;
                    return;
                }
            }
        if(windowsMap && windowsMap != MAP_FAILED){
            if (msync(windowsMap, windowSize, MS_SYNC) == -1) {
                errorCode = LINUX_MMAP_CLOSE_FAILED;
                return;
            }
            if (munmap(windowsMap, windowSize)) {
                errorCode = LINUX_MMAP_CLOSE_FAILED;
                return;
            }
        }
        close(fileHandle);
#endif
    }
};

#endif //POSTMORTEMLOGGER_PMLCIRCULARMAPPEDBUFFER_H