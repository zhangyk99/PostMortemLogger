cmake_minimum_required(VERSION 3.14)
project(PostMortemLogger)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    link_directories(tools/TDMS/win32 tools/serverlogger/win32 tools/varmanager/win32 tools/Common/win32 tools/FileTransfer/win32 tools/mdflib/win32)
elseif(UNIX AND NOT APPLE)
    link_directories(tools/TDMS/linux tools/serverlogger/linux tools/varmanager/linux tools/Common/linux tools/FileTransfer/linux tools/mdflib/linux)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

include_directories(src tools tools/MDF)

add_subdirectory(tests)

add_library(MTestServer.Device.PostMortemLogger SHARED mtestserver_device_PostMortemLogger.cpp)

target_link_libraries(MTestServer.Device.PostMortemLogger PUBLIC TDMS_master varmanager serverlogger MTestServer.Common MTestServer.FileTransfer mdflibrary)