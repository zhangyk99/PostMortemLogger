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

#add_executable(PMLTests PMLTests.cpp)
#target_include_directories(PMLTests PRIVATE ../src ../tools gtest)
#if (WIN32)
#    target_link_libraries(PMLTests PUBLIC GTest::gtest GTest::gtest_main TDMS_master varmanager serverlogger MTestServer.Common MTestServer.FileTransfer mdflibrary Ws2_32 winmm)
#elseif (UNIX AND NOT APPLE)
#    target_link_libraries(PMLTests PUBLIC GTest::gtest GTest::gtest_main TDMS_master varmanager serverlogger MTestServer.Common MTestServer.FileTransfer mdflibrary dl)
#endif ()
#gtest_discover_tests(PMLTests)

add_executable(PMLRecordTest PMLRecordTest.cpp)
target_include_directories(PMLRecordTest PRIVATE ../src ../tools gtest)
if (WIN32)
    target_link_libraries(PMLRecordTest PUBLIC GTest::gtest GTest::gtest_main TDMS_master mdflibrary Ws2_32 winmm)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(PMLRecordTest PUBLIC GTest::gtest GTest::gtest_main TDMS_master mdflibrary dl)
endif()
gtest_discover_tests(PMLRecordTest)

#add_executable(PMLBufferTest PMLCircularMappedBufferTest.cpp)
#target_include_directories(PMLBufferTest PRIVATE ../src ../tools gtest)
#if (WIN32)
#    target_link_libraries(PMLBufferTest PUBLIC GTest::gtest GTest::gtest_main TDMS_master varmanager serverlogger MTestServer.Common MTestServer.FileTransfer mdflibrary Ws2_32 winmm)
#elseif(UNIX AND NOT APPLE)
#    target_link_libraries(PMLBufferTest PUBLIC GTest::gtest GTest::gtest_main TDMS_master varmanager serverlogger MTestServer.Common MTestServer.FileTransfer mdflibrary dl)
#endif()
#gtest_discover_tests(PMLBufferTest)
#
add_executable(PMLVariableTest PMLVariableTest.cpp)
target_include_directories(PMLVariableTest PRIVATE ../src ../tools gtest)
if (WIN32)
    target_link_libraries(PMLVariableTest PUBLIC GTest::gtest GTest::gtest_main TDMS_master varmanager serverlogger mdflibrary Ws2_32 winmm)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(PMLVariableTest PUBLIC GTest::gtest GTest::gtest_main TDMS_master varmanager serverlogger mdflibrary dl)
endif ()
gtest_discover_tests(PMLVariableTest)
#
#add_executable(PMLMdflibraryTest PMLMdflibraryTest.cpp)
#target_include_directories(PMLMdflibraryTest PRIVATE ../src ../tools gtest)
#if (WIN32)
#    target_link_libraries(PMLMdflibraryTest PUBLIC GTest::gtest GTest::gtest_main mdflibrary Ws2_32 winmm)
#elseif (UNIX AND NOT APPLE)
#    target_link_libraries(PMLMdflibraryTest PUBLIC GTest::gtest GTest::gtest_main mdflibrary dl)
#endif ()
#gtest_discover_tests(PMLMdflibraryTest)