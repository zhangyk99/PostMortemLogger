cmake_minimum_required(VERSION 3.14)

project(TestProject)

set(VCPKG ON)

if (VCPKG)
    find_package(GTest CONFIG REQUIRED)
else ()
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://gitee.com/mirrors/googletest.git
            GIT_TAG v1.13.0
    )
    FetchContent_MakeAvailable(googletest)
endif ()

include(GoogleTest)

add_executable(PMLTests PMLTests.cpp)

target_include_directories(PMLTests PRIVATE ../src ../tools gtest)

target_link_libraries(PMLTests PUBLIC GTest::gtest GTest::gtest_main TDMS_master varmanager serverlogger)

gtest_discover_tests(PMLTests)