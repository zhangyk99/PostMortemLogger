//
// Created by 79933 on 2025/9/17.

#include <vector>

#include "gtest/gtest.h"
#include "mdflib/MdfExport.h"
#include "PMLTypes.h"

using namespace MdfLibrary;
using namespace MdfLibrary::ExportFunctions;

TEST(MDF3_DLL, test) {
    uint32_t channelNum = 2100;
    std::string filePath = "D:/test/test_c.mdf";
    auto *Writer = MdfWriterInit(MdfWriterType::Mdf3Basic, filePath.c_str());
    auto *Header = MdfWriterGetHeader(Writer);

    auto *dg = MdfWriterCreateDataGroup(Writer);
    auto *cg = MdfDataGroupCreateChannelGroup(dg);

    std::vector<mdf::IChannel*> channelList;
    for(uint32_t i = 0; i < channelNum; i++){
        auto *channel = MdfChannelGroupCreateChannel(cg);
        channelList.push_back(channel);
        MdfChannelSetName(channel, "channel0");
        MdfChannelSetDescription(channel, "double");
        MdfChannelSetType(channel, ChannelType::FixedLength);
        MdfChannelSetDataType(channel, ChannelDataType::FloatLe);
        MdfChannelSetDataBytes(channel, sizeof(float));
    }

    MdfWriterInitMeasurement(Writer);
    MdfWriterStartMeasurement(Writer, TimeStampToNs());

    for (uint32_t i = 0; i < 10; i++) {
        for(uint32_t j = 0; j < channelNum; j++)
            MdfChannelSetChannelValueAsFloat(channelList[j], i + 0.123456);
        MdfWriterSaveSample(Writer, cg, TimeStampToNs());
    }

    MdfWriterStopMeasurement(Writer, TimeStampToNs());
    MdfWriterFinalizeMeasurement(Writer);

//    auto Reader = MdfReaderInit(filePath.c_str());
//    MdfReaderReadEverythingButData(Reader);
//    auto *file = MdfReaderGetFile(Reader);
//    const mdf::IDataGroup* dg_list[channelNum];
//    MdfFileGetDataGroups((mdf::MdfFile*)file, dg_list);
//    MdfDataGroupGetChannelGroups(dg_list[0], )

}
