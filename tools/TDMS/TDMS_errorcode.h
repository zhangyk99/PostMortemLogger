//
// Created by 79933 on 2025/3/17.
//

#ifndef TMDS_DLL_TEST_TDMS_ERRORCODE_H
#define TMDS_DLL_TEST_TDMS_ERRORCODE_H

#ifdef __linux__
#include <cstdint>
#endif

#define OPTION_OK                      0
#define FILE_HANDLE_FULL               (-5001)
#define FILE_PATH_ERROR                (-5002)
#define FILE_TAG_ERROR                 (-5003)
#define FILE_TOC_ERROR                 (-5004)
#define FILE_VERSION_ERROR             (-5005)
#define FILE_HANDLE_INVALID            (-5006)
#define FILE_READ_FAILED               (-5007)
#define FILE_PROPERTY_NAME_INVALID     (-5008)
#define FILE_PROPERTY_NAME_REPEATED    (-5009)
#define FILE_DELETE_FAILED             (-5010)
#define INDEX_ERROR                    (-5011)
#define GROUP_HANDLE_INVALID           (-5012)
#define GROUP_HANDLE_FULL              (-5013)
#define GROUP_NAME_REPEATED            (-5014)
#define GROUP_PROPERTY_NAME_INVALID    (-5015)
#define GROUP_PROPERTY_NAME_REPEATED   (-5016)
#define CHANNEL_HANDLE_INVALID         (-5017)
#define CHANNEL_READ_FAILED            (-5018)
#define CHANNEL_NAME_REPEATED          (-5019)
#define CHANNEL_PROPERTY_NAME_INVALID  (-5020)
#define CHANNEL_PROPERTY_NAME_REPEATED (-5021)
#define CHANNEL_DATATYPE_ERROR         (-5022)
#define CHANNEL_DATA_READ_FAILED       (-5023)
#define CHANNEL_DATANUM_READ_FAILED    (-5024)

enum class DataType : uint32_t {
    tdsTypeVoid,
    tdsTypeI8,
    tdsTypeI16,
    tdsTypeI32,
    tdsTypeI64,
    tdsTypeU8,
    tdsTypeU16,
    tdsTypeU32,
    tdsTypeU64,
    tdsTypeSingleFloat,
    tdsTypeDoubleFloat,
    tdsTypeExtendedFloat,
    tdsTypeDoubleFloatWithUint,
    tdsTypeExtendedFloatWithUint,
    tdsTypeSingleFloatWithUint = 0x19,
    tdsTypeString = 0x20,
    tdsTypeBoolean = 0x21,
    tdsTypeTimeStamp = 0x44,
    tdsTypeFixedPoint = 0x40,
    tdsTypeComplexSingleFloat = 0x08000c,
    tdsTypeComplexDoubleFloat = 0x10000d,
    tdsTypeDAQmxRawData = 0xFFFFFFFF
};

#endif //TMDS_DLL_TEST_TDMS_ERRORCODE_H
