cmake_minimum_required(VERSION 3.14)
project(PostMortemLogger)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    link_directories(tools/varmanager/win32 tools/serverlogger/win32)
elseif(UNIX AND NOT APPLE)
    link_directories(tools/varmanager/linux tools/serverlogger/linux)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

include_directories(src tools)

add_subdirectory(tests)

add_library(MTestServer.Device.PostMortemLogger SHARED mtestserver_device_PostMortemLogger.cpp)

target_link_libraries(MTestServer.Device.PostMortemLogger PUBLIC varmanager serverlogger)