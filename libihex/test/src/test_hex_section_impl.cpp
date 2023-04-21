/*
    Copyright (C) 2021-2023  Blaž Zakrajšek

    This file is part of libihex.

    libihex is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libihex is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libihex.  If not, see <https://www.gnu.org/licenses/>.

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include <stdexcept>
#include <gtest/gtest.h>
#include <ihex/hex_record.hpp>
#include <ihex/hex_section.hpp>
#include <ihex_test/test_hex_section_impl.hpp>

BEGIN_NAMESPACE_LIBIHEX

HexSectionTest::HexSectionTest()
{

}

HexSectionTest::~HexSectionTest()
{

}

void HexSectionTest::SetUp()
{
    Test::SetUp();
    section = std::make_shared<HexSection>();
}

void HexSectionTest::TearDown()
{
    section.reset();
    Test::TearDown();
}

void HexSectionTest::performContructorTests()
{
    SCOPED_TRACE(__func__);

    // HexSection()

    HexSection section1{};
    ASSERT_EQ(section1.getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(section1.getUnusedDataFillValue(), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(section1.getDefaultDataSize(), DEFAULT_HEX_RECORD_DATA_SIZE);
    ASSERT_EQ(section1.getSize(), 0);
    ASSERT_EQ(section1.getDataSize(), 0);

    // HexSection(section)

    section1.convertToEndOfFileSection();

    const HexSection section2{section1};
    ASSERT_EQ(section2.getSectionType(), HexSectionType::SECTION_END_OF_FILE);
    ASSERT_EQ(section2.getUnusedDataFillValue(), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(section2.getDefaultDataSize(), DEFAULT_HEX_RECORD_DATA_SIZE);
    ASSERT_EQ(section2.getSize(), 1);
    ASSERT_EQ(section2.getRecord(0).getRecord(), std::string(":00000001FF"));
    ASSERT_EQ(section2.getDataSize(), 0);

    // HexSection(std::move(section))

    section1.convertToExtendedSegmentAddressSection(0x1234);

    HexSection section3(std::move(section1));
    ASSERT_EQ(section3.getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(section3.getUnusedDataFillValue(), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(section3.getDefaultDataSize(), DEFAULT_HEX_RECORD_DATA_SIZE);
    ASSERT_EQ(section3.getSize(), 1);
    ASSERT_EQ(section3.getRecord(0).getRecord(), std::string(":020000021234B6"));
    ASSERT_EQ(section3.getDataSize(), 0);
    ASSERT_EQ(section3.getExtendedSegmentAddress(), 0x1234);

    // HexSection(std::string)

    // SECTION_DATA
    const HexSection section4{":108E110096A055EBF7B49A5529853CF3998DE191CC"};
    ASSERT_EQ(section4.getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(section4.getSize(), 1);
    ASSERT_EQ(section4.getRecord(0).getRecord(), std::string(":108E110096A055EBF7B49A5529853CF3998DE191CC"));
    ASSERT_EQ(section4.getDataSize(), 16);

    ASSERT_THROW(HexSection{":108E110096A055EBF7B49A5529853CF3998DE19133"}, std::logic_error);

    // SECTION_END_OF_FILE
    const HexSection section5{":00000001FF"};
    ASSERT_EQ(section5.getSectionType(), HexSectionType::SECTION_END_OF_FILE);
    ASSERT_EQ(section5.getSize(), 1);
    ASSERT_EQ(section5.getRecord(0).getRecord(), std::string(":00000001FF"));
    ASSERT_EQ(section5.getDataSize(), 0);

    ASSERT_THROW(HexSection{":0000000100"}, std::logic_error);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    const HexSection section6{":020000021A2BB7"};
    ASSERT_EQ(section6.getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(section6.getSize(), 1);
    ASSERT_EQ(section6.getRecord(0).getRecord(), std::string(":020000021A2BB7"));
    ASSERT_EQ(section6.getDataSize(), 0);
    ASSERT_EQ(section6.getExtendedSegmentAddress(), 0x1A2B);

    ASSERT_THROW(HexSection{":020000021A2B48"}, std::logic_error);

    // SECTION_START_SEGMENT_ADDRESS
    const HexSection section7{":040000033C4D5E6FA3"};
    ASSERT_EQ(section7.getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(section7.getSize(), 1);
    ASSERT_EQ(section7.getRecord(0).getRecord(), std::string(":040000033C4D5E6FA3"));
    ASSERT_EQ(section7.getDataSize(), 0);
    ASSERT_EQ(section7.getStartSegmentAddressCodeSegment(), 0x3C4D);
    ASSERT_EQ(section7.getStartSegmentAddressInstructionPointer(), 0x5E6F);

    ASSERT_THROW(HexSection{":040000033C4D5E6F5C"}, std::logic_error);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    const HexSection section8{":02000004607228"};
    ASSERT_EQ(section8.getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(section8.getSize(), 1);
    ASSERT_EQ(section8.getRecord(0).getRecord(), std::string(":02000004607228"));
    ASSERT_EQ(section8.getDataSize(), 0);
    ASSERT_EQ(section8.getExtendedLinearAddress(), 0x6072);

    ASSERT_THROW(HexSection{":020000046072D7"}, std::logic_error);

    // SECTION_START_LINEAR_ADDRESS
    const HexSection section9{":040000058394A5B685"};
    ASSERT_EQ(section9.getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(section9.getSize(), 1);
    ASSERT_EQ(section9.getRecord(0).getRecord(), std::string(":040000058394A5B685"));
    ASSERT_EQ(section9.getDataSize(), 0);
    ASSERT_EQ(section9.getStartLinearAddressExtendedInstructionPointer(), 0x8394A5B6);

    ASSERT_THROW(HexSection{":040000058394A5B67A"}, std::logic_error);

    // Invalid records
    ASSERT_THROW(HexSection{""}, std::runtime_error);
    ASSERT_THROW(HexSection{"abc"}, std::runtime_error);
    ASSERT_THROW(HexSection{":"}, std::runtime_error);
    ASSERT_THROW(HexSection{":10"}, std::runtime_error);
    ASSERT_THROW(HexSection{":000000"}, std::runtime_error);
    ASSERT_THROW(HexSection{":00000000"}, std::runtime_error);
    ASSERT_THROW(HexSection{":0100000010"}, std::runtime_error);

    // HexSection(HexRecord)

    // SECTION_DATA
    const HexRecord record1(":102ABF0099A1D1205750EF18F3D5302721EBA73527");
    const HexSection section10{record1};
    ASSERT_EQ(section10.getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(section10.getSize(), 1);
    ASSERT_EQ(section10.getRecord(0).getRecord(), std::string(":102ABF0099A1D1205750EF18F3D5302721EBA73527"));
    ASSERT_EQ(section10.getDataSize(), 16);

    // SECTION_END_OF_FILE
    const HexRecord record2{":00000001FF"};
    const HexSection section11{record2};
    ASSERT_EQ(section11.getSectionType(), HexSectionType::SECTION_END_OF_FILE);
    ASSERT_EQ(section11.getSize(), 1);
    ASSERT_EQ(section11.getRecord(0).getRecord(), std::string(":00000001FF"));
    ASSERT_EQ(section11.getDataSize(), 0);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    const HexRecord record3{":020000027F80FD"};
    const HexSection section12{record3};
    ASSERT_EQ(section12.getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(section12.getSize(), 1);
    ASSERT_EQ(section12.getRecord(0).getRecord(), std::string(":020000027F80FD"));
    ASSERT_EQ(section12.getDataSize(), 0);
    ASSERT_EQ(section12.getExtendedSegmentAddress(), 0x7F80);

    // SECTION_START_SEGMENT_ADDRESS
    const HexRecord record4{":0400000391A2B3C44F"};
    const HexSection section13{record4};
    ASSERT_EQ(section13.getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(section13.getSize(), 1);
    ASSERT_EQ(section13.getRecord(0).getRecord(), std::string(":0400000391A2B3C44F"));
    ASSERT_EQ(section13.getDataSize(), 0);
    ASSERT_EQ(section13.getStartSegmentAddressCodeSegment(), 0x91A2);
    ASSERT_EQ(section13.getStartSegmentAddressInstructionPointer(), 0xB3C4);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    const HexRecord record5{":02000004D5E63F"};
    const HexSection section14{record5};
    ASSERT_EQ(section14.getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(section14.getSize(), 1);
    ASSERT_EQ(section14.getRecord().getRecord(), std::string(":02000004D5E63F"));
    ASSERT_EQ(section14.getDataSize(), 0);
    ASSERT_EQ(section14.getExtendedLinearAddress(), 0xD5E6);

    // SECTION_START_LINEAR_ADDRESS
    const HexRecord record6{":040000056F70819205"};
    const HexSection section15{record6};
    ASSERT_EQ(section15.getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(section15.getSize(), 1);
    ASSERT_EQ(section15.getRecord(0).getRecord(), std::string(":040000056F70819205"));
    ASSERT_EQ(section15.getDataSize(), 0);
    ASSERT_EQ(section15.getStartLinearAddressExtendedInstructionPointer(), 0x6F708192);

    // Invalid records
    HexRecord record7{record1};
    record7.setChecksum(record7.getChecksum() ^ 0x5A);
    ASSERT_THROW(HexSection{record7}, std::logic_error);

    // HexSection(HexSectionType)

    // SECTION_DATA
    const HexSection section16{HexSectionType::SECTION_DATA};
    ASSERT_EQ(section16.getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(section16.getSize(), 0);
    ASSERT_EQ(section16.getDataSize(), 0);

    // SECTION_END_OF_FILE
    const HexSection section17{HexSectionType::SECTION_END_OF_FILE};
    ASSERT_EQ(section17.getSectionType(), HexSectionType::SECTION_END_OF_FILE);
    ASSERT_EQ(section17.getSize(), 1);
    ASSERT_EQ(section17.getRecord(0).getRecord(), std::string(":00000001FF"));
    ASSERT_EQ(section17.getDataSize(), 0);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    const HexSection section18{HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS};
    ASSERT_EQ(section18.getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(section18.getSize(), 1);
    ASSERT_EQ(section18.getRecord(0).getRecord(), std::string(":020000020000FC"));
    ASSERT_EQ(section18.getDataSize(), 0);
    ASSERT_EQ(section18.getExtendedSegmentAddress(), 0);

    // SECTION_START_SEGMENT_ADDRESS
    const HexSection section19{HexSectionType::SECTION_START_SEGMENT_ADDRESS};
    ASSERT_EQ(section19.getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(section19.getSize(), 1);
    ASSERT_EQ(section19.getRecord(0).getRecord(), std::string(":0400000300000000F9"));
    ASSERT_EQ(section19.getDataSize(), 0);
    ASSERT_EQ(section19.getStartSegmentAddressCodeSegment(), 0);
    ASSERT_EQ(section19.getStartSegmentAddressInstructionPointer(), 0);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    const HexSection section20{HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS};
    ASSERT_EQ(section20.getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(section20.getSize(), 1);
    ASSERT_EQ(section20.getRecord(0).getRecord(), std::string(":020000040000FA"));
    ASSERT_EQ(section20.getDataSize(), 0);
    ASSERT_EQ(section20.getExtendedLinearAddress(), 0);

    // SECTION_START_LINEAR_ADDRESS
    const HexSection section21{HexSectionType::SECTION_START_LINEAR_ADDRESS};
    ASSERT_EQ(section21.getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(section21.getSize(), 1);
    ASSERT_EQ(section21.getRecord(0).getRecord(), std::string(":0400000500000000F7"));
    ASSERT_EQ(section21.getDataSize(), 0);
    ASSERT_EQ(section21.getStartLinearAddressExtendedInstructionPointer(), 0);
}

void HexSectionTest::performMakeSectionTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexDataArray<MAXIMUM_HEX_RECORD_DATA_SIZE> arrayData{};
    for (size_t index{0}; index < arrayData.size(); ++index)
        arrayData[index] = ((2 * index) + 2);

    HexDataVector vectorData{};
    for (size_t index{0}; index < (MAXIMUM_HEX_RECORD_DATA_SIZE + 1); ++index)
        vectorData.push_back((2 * index) + 2);

    // SECTION_DATA
    const auto sectionData1{HexSection::makeDataSection(0x1234, &arrayData[0], 8)};
    ASSERT_EQ(sectionData1.getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(sectionData1.getSize(), 1);
    ASSERT_EQ(sectionData1.getRecord(0).getRecord(), std::string(":08123400020406080A0C0E106A"));
    ASSERT_EQ(sectionData1.getDataSize(), 8);

    const auto sectionData2{HexSection::makeDataSection(0x2345, &arrayData[0], MAXIMUM_HEX_RECORD_DATA_SIZE)};
    ASSERT_EQ(sectionData2.getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(sectionData2.getSize(), 16);
    ASSERT_EQ(sectionData2.getRecord(0).getRecord(), std::string(":10234500020406080A0C0E10121416181A1C1E2078"));
    ASSERT_EQ(sectionData2.getDataSize(), MAXIMUM_HEX_RECORD_DATA_SIZE);

    const auto sectionData3{HexSection::makeDataSection(0x1234, vectorData, 8)};
    ASSERT_EQ(sectionData3.getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(sectionData3.getSize(), 1);
    ASSERT_EQ(sectionData3.getRecord(0).getRecord(), std::string(":08123400020406080A0C0E106A"));
    ASSERT_EQ(sectionData3.getDataSize(), 8);

    const auto sectionData4{HexSection::makeDataSection(0x2345, vectorData, MAXIMUM_HEX_RECORD_DATA_SIZE - 2, 2)};
    ASSERT_EQ(sectionData4.getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(sectionData4.getSize(), 16);
    ASSERT_EQ(sectionData4.getRecord(0).getRecord(), std::string(":1023450006080A0C0E10121416181A1C1E20222438"));
    ASSERT_EQ(sectionData4.getDataSize(), MAXIMUM_HEX_RECORD_DATA_SIZE - 2);

    // SECTION_END_OF_FILE
    const auto sectionEndOfFile1{HexSection::makeEndOfFileSection()};
    ASSERT_EQ(sectionEndOfFile1.getSectionType(), HexSectionType::SECTION_END_OF_FILE);
    ASSERT_EQ(sectionEndOfFile1.getSize(), 1);
    ASSERT_EQ(sectionEndOfFile1.getRecord(0).getRecord(), std::string(":00000001FF"));
    ASSERT_EQ(sectionEndOfFile1.getDataSize(), 0);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    const auto sectionExtendedSegmentAddress1{HexSection::makeExtendedSegmentAddressSection(0x1A2B)};
    ASSERT_EQ(sectionExtendedSegmentAddress1.getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(sectionExtendedSegmentAddress1.getSize(), 1);
    ASSERT_EQ(sectionExtendedSegmentAddress1.getRecord(0).getRecord(), std::string(":020000021A2BB7"));
    ASSERT_EQ(sectionExtendedSegmentAddress1.getDataSize(), 0);
    ASSERT_EQ(sectionExtendedSegmentAddress1.getExtendedSegmentAddress(), 0x1A2B);

    const auto sectionExtendedSegmentAddress2{HexSection::makeExtendedSegmentAddressSection(0xC3D4, 0x1122, &arrayData[0], 8)};
    ASSERT_EQ(sectionExtendedSegmentAddress2.getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(sectionExtendedSegmentAddress2.getSize(), 2);
    ASSERT_EQ(sectionExtendedSegmentAddress2.getRecord(0).getRecord(), std::string(":02000002C3D465"));
    ASSERT_EQ(sectionExtendedSegmentAddress2.getRecord(1).getRecord(), std::string(":08112200020406080A0C0E107D"));
    ASSERT_EQ(sectionExtendedSegmentAddress2.getDataSize(), 8);
    ASSERT_EQ(sectionExtendedSegmentAddress2.getExtendedSegmentAddress(), 0xC3D4);

    const auto sectionExtendedSegmentAddress3{HexSection::makeExtendedSegmentAddressSection(0xE5F6, 0x3344, vectorData, 8, 0)};
    ASSERT_EQ(sectionExtendedSegmentAddress3.getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(sectionExtendedSegmentAddress3.getSize(), 2);
    ASSERT_EQ(sectionExtendedSegmentAddress3.getRecord(0).getRecord(), std::string(":02000002E5F621"));
    ASSERT_EQ(sectionExtendedSegmentAddress3.getRecord(1).getRecord(), std::string(":08334400020406080A0C0E1039"));
    ASSERT_EQ(sectionExtendedSegmentAddress3.getDataSize(), 8);
    ASSERT_EQ(sectionExtendedSegmentAddress3.getExtendedSegmentAddress(), 0xE5F6);

    const auto sectionExtendedSegmentAddress4{HexSection::makeExtendedSegmentAddressSection(0x1728, ":08556600641A1BBD9649DF1415")};
    ASSERT_EQ(sectionExtendedSegmentAddress4.getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(sectionExtendedSegmentAddress4.getSize(), 2);
    ASSERT_EQ(sectionExtendedSegmentAddress4.getRecord(0).getRecord(), std::string(":020000021728BD"));
    ASSERT_EQ(sectionExtendedSegmentAddress4.getRecord(1).getRecord(), std::string(":08556600641A1BBD9649DF1415"));
    ASSERT_EQ(sectionExtendedSegmentAddress4.getDataSize(), 8);
    ASSERT_EQ(sectionExtendedSegmentAddress4.getExtendedSegmentAddress(), 0x1728);

    ASSERT_THROW(HexSection::makeExtendedSegmentAddressSection(0x1122, ":"), std::logic_error);
    ASSERT_THROW(HexSection::makeExtendedSegmentAddressSection(0x1122, HexRecord::makeEndOfFileRecord()), std::logic_error);

    const auto sectionExtendedSegmentAddress5{HexSection::makeExtendedSegmentAddressSection(0x394A, HexRecord{":08778800D9272B219AD7F4B395"})};
    ASSERT_EQ(sectionExtendedSegmentAddress5.getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(sectionExtendedSegmentAddress5.getSize(), 2);
    ASSERT_EQ(sectionExtendedSegmentAddress5.getRecord(0).getRecord(), std::string(":02000002394A79"));
    ASSERT_EQ(sectionExtendedSegmentAddress5.getRecord(1).getRecord(), std::string(":08778800D9272B219AD7F4B395"));
    ASSERT_EQ(sectionExtendedSegmentAddress5.getDataSize(), 8);
    ASSERT_EQ(sectionExtendedSegmentAddress5.getExtendedSegmentAddress(), 0x394A);

    ASSERT_THROW(HexSection::makeExtendedSegmentAddressSection(0x3344, HexRecord{HexRecordType::RECORD_END_OF_FILE}), std::logic_error);

    // SECTION_START_SEGMENT_ADDRESS
    const auto sectionStartSegmentAddress1{HexSection::makeStartSegmentAddressSection(0x3C4D, 0x5E6F)};
    ASSERT_EQ(sectionStartSegmentAddress1.getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(sectionStartSegmentAddress1.getSize(), 1);
    ASSERT_EQ(sectionStartSegmentAddress1.getRecord(0).getRecord(), std::string(":040000033C4D5E6FA3"));
    ASSERT_EQ(sectionStartSegmentAddress1.getDataSize(), 0);
    ASSERT_EQ(sectionStartSegmentAddress1.getStartSegmentAddressCodeSegment(), 0x3C4D);
    ASSERT_EQ(sectionStartSegmentAddress1.getStartSegmentAddressInstructionPointer(), 0x5E6F);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    const auto sectionExtendedLinearAddress1{HexSection::makeExtendedLinearAddressSection(0x7081)};
    ASSERT_EQ(sectionExtendedLinearAddress1.getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(sectionExtendedLinearAddress1.getSize(), 1);
    ASSERT_EQ(sectionExtendedLinearAddress1.getRecord().getRecord(), std::string(":02000004708109"));
    ASSERT_EQ(sectionExtendedLinearAddress1.getDataSize(), 0);
    ASSERT_EQ(sectionExtendedLinearAddress1.getExtendedLinearAddress(), 0x7081);

    const auto sectionExtendedLinearAddress2{HexSection::makeExtendedLinearAddressSection(0x92A3, 0x99AA, &arrayData[0], 8)};
    ASSERT_EQ(sectionExtendedLinearAddress2.getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(sectionExtendedLinearAddress2.getSize(), 2);
    ASSERT_EQ(sectionExtendedLinearAddress2.getRecord(0).getRecord(), std::string(":0200000492A3C5"));
    ASSERT_EQ(sectionExtendedLinearAddress2.getRecord(1).getRecord(), std::string(":0899AA00020406080A0C0E106D"));
    ASSERT_EQ(sectionExtendedLinearAddress2.getDataSize(), 8);
    ASSERT_EQ(sectionExtendedLinearAddress2.getExtendedLinearAddress(), 0x92A3);

    const auto sectionExtendedLinearAddress3{HexSection::makeExtendedLinearAddressSection(0xB4C5, 0xBBCC, vectorData, 8, 0)};
    ASSERT_EQ(sectionExtendedLinearAddress3.getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(sectionExtendedLinearAddress3.getSize(), 2);
    ASSERT_EQ(sectionExtendedLinearAddress3.getRecord(0).getRecord(), std::string(":02000004B4C581"));
    ASSERT_EQ(sectionExtendedLinearAddress3.getRecord(1).getRecord(), std::string(":08BBCC00020406080A0C0E1029"));
    ASSERT_EQ(sectionExtendedLinearAddress3.getDataSize(), 8);
    ASSERT_EQ(sectionExtendedLinearAddress3.getExtendedLinearAddress(), 0xB4C5);

    const auto sectionExtendedLinearAddress4{HexSection::makeExtendedLinearAddressSection(0xD6E7, ":08DDEE00E09C4598220A4D99C2")};
    ASSERT_EQ(sectionExtendedLinearAddress4.getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(sectionExtendedLinearAddress4.getSize(), 2);
    ASSERT_EQ(sectionExtendedLinearAddress4.getRecord(0).getRecord(), std::string(":02000004D6E73D"));
    ASSERT_EQ(sectionExtendedLinearAddress4.getRecord(1).getRecord(), std::string(":08DDEE00E09C4598220A4D99C2"));
    ASSERT_EQ(sectionExtendedLinearAddress4.getDataSize(), 8);
    ASSERT_EQ(sectionExtendedLinearAddress4.getExtendedLinearAddress(), 0xD6E7);

    ASSERT_THROW(HexSection::makeExtendedLinearAddressSection(0x3344, ":"), std::logic_error);
    ASSERT_THROW(HexSection::makeExtendedLinearAddressSection(0x3344, HexRecord::makeEndOfFileRecord()), std::logic_error);

    const auto sectionExtendedLinearAddress5{HexSection::makeExtendedLinearAddressSection(0xF809, HexRecord{":08FF0000EA0BD12782441379BA"})};
    ASSERT_EQ(sectionExtendedLinearAddress5.getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(sectionExtendedLinearAddress5.getSize(), 2);
    ASSERT_EQ(sectionExtendedLinearAddress5.getRecord(0).getRecord(), std::string(":02000004F809F9"));
    ASSERT_EQ(sectionExtendedLinearAddress5.getRecord(1).getRecord(), std::string(":08FF0000EA0BD12782441379BA"));
    ASSERT_EQ(sectionExtendedLinearAddress5.getDataSize(), 8);
    ASSERT_EQ(sectionExtendedLinearAddress5.getExtendedLinearAddress(), 0xF809);

    ASSERT_THROW(HexSection::makeExtendedLinearAddressSection(0x5566, HexRecord{HexRecordType::RECORD_END_OF_FILE}), std::logic_error);

    // SECTION_START_LINEAR_ADDRESS
    const auto sectionStartLinearAddress1{HexSection::makeStartLinearAddressSection(0x92A3B4C5)};
    ASSERT_EQ(sectionStartLinearAddress1.getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(sectionStartLinearAddress1.getSize(), 1);
    ASSERT_EQ(sectionStartLinearAddress1.getRecord(0).getRecord(), std::string(":0400000592A3B4C549"));
    ASSERT_EQ(sectionStartLinearAddress1.getDataSize(), 0);
    ASSERT_EQ(sectionStartLinearAddress1.getStartLinearAddressExtendedInstructionPointer(), 0x92A3B4C5);
}

void HexSectionTest::performConvertToDataSectionTests()
{
    SCOPED_TRACE(__func__);

    section->convertToEndOfFileSection();

    // SECTION_DATA
    section->convertToDataSection();
    ASSERT_EQ(section->getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(section->getSize(), 0);
    ASSERT_EQ(section->getDataSize(), 0);
}

void HexSectionTest::performConvertToEndOfFileSectionTests()
{
    SCOPED_TRACE(__func__);

    section->convertToDataSection();

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();
    ASSERT_EQ(section->getSectionType(), HexSectionType::SECTION_END_OF_FILE);
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getRecord(0).getRecord(), std::string(":00000001FF"));
    ASSERT_EQ(section->getDataSize(), 0);
}

void HexSectionTest::performConvertToExtendedSegmentAddressSectionTests()
{
    SCOPED_TRACE(__func__);

    section->convertToDataSection();

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    ASSERT_FALSE(section->setExtendedSegmentAddress(0x1A2B));
    ASSERT_THROW(section->getExtendedSegmentAddress(), std::domain_error);

    section->convertToExtendedSegmentAddressSection(0xE1D2);
    ASSERT_EQ(section->getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getRecord(0).getRecord(), std::string(":02000002E1D249"));
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_EQ(section->getExtendedSegmentAddress(), 0xE1D2);
    ASSERT_TRUE(section->setExtendedSegmentAddress(0x3C4D));
    ASSERT_EQ(section->getExtendedSegmentAddress(), 0x3C4D);

    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getRecord(0).getRecord(), std::string(":020000023C4D73"));
    ASSERT_EQ(section->getDataSize(), 0);
}

void HexSectionTest::performConvertToStartSegmentAddressSectionTests()
{
    SCOPED_TRACE(__func__);

    section->convertToDataSection();

    // SECTION_START_SEGMENT_ADDRESS
    ASSERT_FALSE(section->setStartSegmentAddressCodeSegment(0x5E6F));
    ASSERT_THROW(section->getStartSegmentAddressCodeSegment(), std::domain_error);

    ASSERT_FALSE(section->setStartSegmentAddressInstructionPointer(0x7081));
    ASSERT_THROW(section->getStartSegmentAddressInstructionPointer(), std::domain_error);

    section->convertToStartSegmentAddressSection(0xC3B4, 0xA596);
    ASSERT_EQ(section->getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getRecord(0).getRecord(), std::string(":04000003C3B4A59647"));
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_EQ(section->getStartSegmentAddressCodeSegment(), 0xC3B4);
    ASSERT_TRUE(section->setStartSegmentAddressCodeSegment(0x92A3));
    ASSERT_EQ(section->getStartSegmentAddressCodeSegment(), 0x92A3);

    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getRecord(0).getRecord(), std::string(":0400000392A3A59689"));
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_EQ(section->getStartSegmentAddressInstructionPointer(), 0xA596);
    ASSERT_TRUE(section->setStartSegmentAddressInstructionPointer(0xB4C5));
    ASSERT_EQ(section->getStartSegmentAddressInstructionPointer(), 0xB4C5);

    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getRecord(0).getRecord(), std::string(":0400000392A3B4C54B"));
    ASSERT_EQ(section->getDataSize(), 0);
}

void HexSectionTest::performConvertToExtendedLinearAddressSectionTests()
{
    SCOPED_TRACE(__func__);

    section->convertToDataSection();

    // SECTION_EXTENDED_LINEAR_ADDRESS
    ASSERT_FALSE(section->setExtendedLinearAddress(0xD6E7));
    ASSERT_THROW(section->getExtendedLinearAddress(), std::domain_error);

    section->convertToExtendedLinearAddressSection(0x8778);
    ASSERT_EQ(section->getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getRecord(0).getRecord(), std::string(":020000048778FB"));
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_EQ(section->getExtendedLinearAddress(), 0x8778);
    ASSERT_TRUE(section->setExtendedLinearAddress(0xF809));
    ASSERT_EQ(section->getExtendedLinearAddress(), 0xF809);

    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getRecord(0).getRecord(), std::string(":02000004F809F9"));
    ASSERT_EQ(section->getDataSize(), 0);
}

void HexSectionTest::performConvertToStartLinearAddressSectionTests()
{
    SCOPED_TRACE(__func__);

    section->convertToDataSection();

    // SECTION_START_LINEAR_ADDRESS
    ASSERT_FALSE(section->setStartLinearAddressExtendedInstructionPointer(0x1A2B3C4D));
    ASSERT_THROW(section->getStartLinearAddressExtendedInstructionPointer(), std::domain_error);

    section->convertToStartLinearAddressSection(0x695A4B3C);
    ASSERT_EQ(section->getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getRecord(0).getRecord(), std::string(":04000005695A4B3CAD"));
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_EQ(section->getStartLinearAddressExtendedInstructionPointer(), 0x695A4B3C);
    ASSERT_TRUE(section->setStartLinearAddressExtendedInstructionPointer(0x5E7F8091));
    ASSERT_EQ(section->getStartLinearAddressExtendedInstructionPointer(), 0x5E7F8091);

    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getRecord(0).getRecord(), std::string(":040000055E7F809109"));
    ASSERT_EQ(section->getDataSize(), 0);
}

void HexSectionTest::performAddressMapTests()
{
    SCOPED_TRACE(__func__);

    HexAddressMap map{};

    section->convertToDataSection();
    ASSERT_NO_THROW(map = section->getAddressMap());
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    section->convertToEndOfFileSection();
    ASSERT_THROW(section->getAddressMap(), std::domain_error);

    section->convertToExtendedSegmentAddressSection(0x1234);
    ASSERT_NO_THROW(map = section->getAddressMap());
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0x12340);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    section->convertToStartSegmentAddressSection(0x5678, 0x9ABC);
    ASSERT_THROW(section->getAddressMap(), std::domain_error);

    section->convertToExtendedLinearAddressSection(0xDEF0);
    ASSERT_NO_THROW(map = section->getAddressMap());
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0xDEF00000);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    section->convertToStartLinearAddressSection(0x12345678);
    ASSERT_THROW(section->getAddressMap(), std::domain_error);
}

void HexSectionTest::performDataMapTests()
{
    SCOPED_TRACE(__func__);

    HexAddressMap map{};

    section->convertToDataSection();
    ASSERT_NO_THROW(map = section->getDataMap());
    ASSERT_EQ(map.size(), 0);

    ASSERT_TRUE(section->pushRecord(":1012340000000000000000000000000000000000AA"));
    ASSERT_NO_THROW(map = section->getDataMap());
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0x1234);
    ASSERT_EQ(map.cbegin()->second, 16);

    section->convertToEndOfFileSection();
    ASSERT_THROW(section->getDataMap(), std::domain_error);

    section->convertToExtendedSegmentAddressSection(0x1234);
    ASSERT_NO_THROW(map = section->getDataMap());
    ASSERT_EQ(map.size(), 0);

    ASSERT_TRUE(section->pushRecord(":102345000000000000000000000000000000000088"));
    ASSERT_TRUE(section->pushRecord(":0F23550000000000000000000000000000000079"));
    ASSERT_NO_THROW(map = section->getDataMap());
    ASSERT_EQ(map.size(), 2);
    ASSERT_EQ(map.cbegin()->first, 0x14685);
    ASSERT_EQ(map.cbegin()->second, 16);
    ASSERT_EQ(std::next(map.cbegin(), 1)->first, 0x14695);
    ASSERT_EQ(std::next(map.cbegin(), 1)->second, 15);

    section->convertToStartSegmentAddressSection(0x5678, 0x9ABC);
    ASSERT_THROW(section->getDataMap(), std::domain_error);

    section->convertToExtendedLinearAddressSection(0xDEF0);
    ASSERT_NO_THROW(map = section->getDataMap());
    ASSERT_EQ(map.size(), 0);

    ASSERT_TRUE(section->pushRecord(":103456000000000000000000000000000000000066"));
    ASSERT_TRUE(section->pushRecord(":0F34660000000000000000000000000000000057"));
    ASSERT_TRUE(section->pushRecord(":0E347600000000000000000000000000000048"));
    ASSERT_NO_THROW(map = section->getDataMap());
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(map.cbegin()->first, 0xDEF03456);
    ASSERT_EQ(map.cbegin()->second, 16);
    ASSERT_EQ(std::next(map.cbegin(), 1)->first, 0xDEF03466);
    ASSERT_EQ(std::next(map.cbegin(), 1)->second, 15);
    ASSERT_EQ(std::next(map.cbegin(), 2)->first, 0xDEF03476);
    ASSERT_EQ(std::next(map.cbegin(), 2)->second, 14);

    section->convertToStartLinearAddressSection(0x12345678);
    ASSERT_THROW(section->getDataMap(), std::domain_error);
}

void HexSectionTest::performPropertiesTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_EQ(section->getUnusedDataFillValue(), DEFAULT_HEX_RECORD_DATA_VALUE);
    section->setUnusedDataFillValue(0x5A);
    ASSERT_EQ(section->getUnusedDataFillValue(), 0x5A);

    ASSERT_EQ(section->getDefaultDataSize(), DEFAULT_HEX_RECORD_DATA_SIZE);
    ASSERT_NO_THROW(section->setDefaultDataSize(0x20));
    ASSERT_EQ(section->getDefaultDataSize(), 0x20);
    ASSERT_THROW(section->setDefaultDataSize(1), std::out_of_range);
    ASSERT_THROW(section->setDefaultDataSize(0), std::out_of_range);
    ASSERT_EQ(section->getDefaultDataSize(), 0x20);
}

void HexSectionTest::performCanPushRecordTests()
{
    SCOPED_TRACE(__func__);

    // Test records
    const HexRecord recordData{":10AED100A8A955C5DC8B35EB45FF3CE11957370C6B"};
    const HexRecord recordEndOfFile{HexRecordType::RECORD_END_OF_FILE};
    const HexRecord recordExtendedSegmentAddress{HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS};
    const HexRecord recordStartSegmentAddress{HexRecordType::RECORD_START_SEGMENT_ADDRESS};
    const HexRecord recordExtendedLinearAddress{HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS};
    const HexRecord recordStartLinearAddress{HexRecordType::RECORD_START_LINEAR_ADDRESS};

    HexRecord recordUnknown{};
    ASSERT_NO_THROW(recordUnknown.prepareRecord(0, static_cast<HexRecordType>(static_cast<uint8_t>(HexRecordType::RECORD_MAX) + 1), nullptr, 0));
    ASSERT_EQ(recordUnknown.getRecord(), std::string(":00000006FA"));

    HexRecord recordChecksum{recordData};
    recordChecksum.setChecksum(recordChecksum.getChecksum() ^ 0x5A);

    // SECTION_DATA (empty)
    section->convertToDataSection();

    ASSERT_TRUE(section->canPushRecord(recordData));
    ASSERT_TRUE(section->canPushRecord(recordData.getRecord()));

    ASSERT_TRUE(section->canPushRecord(recordEndOfFile));
    ASSERT_TRUE(section->canPushRecord(recordEndOfFile.getRecord()));

    ASSERT_TRUE(section->canPushRecord(recordExtendedSegmentAddress));
    ASSERT_TRUE(section->canPushRecord(recordExtendedSegmentAddress.getRecord()));

    ASSERT_TRUE(section->canPushRecord(recordStartSegmentAddress));
    ASSERT_TRUE(section->canPushRecord(recordStartSegmentAddress.getRecord()));

    ASSERT_TRUE(section->canPushRecord(recordExtendedLinearAddress));
    ASSERT_TRUE(section->canPushRecord(recordExtendedLinearAddress.getRecord()));

    ASSERT_TRUE(section->canPushRecord(recordStartLinearAddress));
    ASSERT_TRUE(section->canPushRecord(recordStartLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordUnknown));
    ASSERT_FALSE(section->canPushRecord(recordUnknown.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordChecksum));
    ASSERT_FALSE(section->canPushRecord(recordChecksum.getRecord()));

    // SECTION_DATA (with single data record)
    section->pushRecord(":102B8900690EC2CB337A957A4B3F4C6905BBEA088B");

    ASSERT_TRUE(section->canPushRecord(recordData));
    ASSERT_TRUE(section->canPushRecord(recordData.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordEndOfFile));
    ASSERT_FALSE(section->canPushRecord(recordEndOfFile.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordUnknown));
    ASSERT_FALSE(section->canPushRecord(recordUnknown.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordChecksum));
    ASSERT_FALSE(section->canPushRecord(recordChecksum.getRecord()));

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_FALSE(section->canPushRecord(recordData));
    ASSERT_FALSE(section->canPushRecord(recordData.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordEndOfFile));
    ASSERT_FALSE(section->canPushRecord(recordEndOfFile.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordUnknown));
    ASSERT_FALSE(section->canPushRecord(recordUnknown.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordChecksum));
    ASSERT_FALSE(section->canPushRecord(recordChecksum.getRecord()));

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    section->convertToExtendedSegmentAddressSection(0x1829);

    ASSERT_TRUE(section->canPushRecord(recordData));
    ASSERT_TRUE(section->canPushRecord(recordData.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordEndOfFile));
    ASSERT_FALSE(section->canPushRecord(recordEndOfFile.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordUnknown));
    ASSERT_FALSE(section->canPushRecord(recordUnknown.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordChecksum));
    ASSERT_FALSE(section->canPushRecord(recordChecksum.getRecord()));

    // SECTION_START_SEGMENT_ADDRESS
    section->convertToStartSegmentAddressSection(0x3A4B, 0x5C6D);

    ASSERT_FALSE(section->canPushRecord(recordData));
    ASSERT_FALSE(section->canPushRecord(recordData.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordEndOfFile));
    ASSERT_FALSE(section->canPushRecord(recordEndOfFile.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordUnknown));
    ASSERT_FALSE(section->canPushRecord(recordUnknown.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordChecksum));
    ASSERT_FALSE(section->canPushRecord(recordChecksum.getRecord()));

    // SECTION_EXTENDED_LINEAR_ADDRESS
    section->convertToExtendedLinearAddressSection(0x7E8F);

    ASSERT_TRUE(section->canPushRecord(recordData));
    ASSERT_TRUE(section->canPushRecord(recordData.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordEndOfFile));
    ASSERT_FALSE(section->canPushRecord(recordEndOfFile.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordUnknown));
    ASSERT_FALSE(section->canPushRecord(recordUnknown.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordChecksum));
    ASSERT_FALSE(section->canPushRecord(recordChecksum.getRecord()));

    // SECTION_START_LINEAR_ADDRESS
    section->convertToStartLinearAddressSection(0x90A1B2C3);

    ASSERT_FALSE(section->canPushRecord(recordData));
    ASSERT_FALSE(section->canPushRecord(recordData.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordEndOfFile));
    ASSERT_FALSE(section->canPushRecord(recordEndOfFile.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartSegmentAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordExtendedLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress));
    ASSERT_FALSE(section->canPushRecord(recordStartLinearAddress.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordUnknown));
    ASSERT_FALSE(section->canPushRecord(recordUnknown.getRecord()));

    ASSERT_FALSE(section->canPushRecord(recordChecksum));
    ASSERT_FALSE(section->canPushRecord(recordChecksum.getRecord()));

    // Invalid records
    section->convertToDataSection();

    ASSERT_FALSE(section->canPushRecord(""));
    ASSERT_FALSE(section->canPushRecord("AABBCCDDEEFF"));
    ASSERT_FALSE(section->canPushRecord(":"));
    ASSERT_FALSE(section->canPushRecord(":04"));
    ASSERT_FALSE(section->canPushRecord(":02000000"));
    ASSERT_FALSE(section->canPushRecord(":020000FF"));
    ASSERT_FALSE(section->canPushRecord(":020000001A"));
    ASSERT_FALSE(section->canPushRecord(":020000001A2B"));
}

void HexSectionTest::performPushRecordTests()
{
    SCOPED_TRACE(__func__);

    // Test data
    const HexRecord recordData1{":10AED100A8A955C5DC8B35EB45FF3CE11957370C6B"};
    const HexRecord recordData2{":10AEE1000D14DB46AB50DDC3653C35A35335FDACDA"};
    const HexRecord recordEndOfData{HexRecordType::RECORD_END_OF_FILE};
    const HexRecord recordExtendedSegmentAddress{HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS};
    const HexRecord recordStartSegmentAddress{HexRecordType::RECORD_START_SEGMENT_ADDRESS};
    const HexRecord recordExtendedLinearAddress{HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS};
    const HexRecord recordStartLinearAddress{HexRecordType::RECORD_START_LINEAR_ADDRESS};

    HexRecord recordUnknown{};
    ASSERT_NO_THROW(recordUnknown.prepareRecord(0, static_cast<HexRecordType>(static_cast<uint8_t>(HexRecordType::RECORD_MAX) + 2), nullptr, 0));
    ASSERT_EQ(recordUnknown.getRecord(), std::string(":00000007F9"));

    HexRecord recordChecksum{recordData1};
    recordChecksum.setChecksum(recordChecksum.getChecksum() ^ 0xA5);

    HexRecord recordCompare{};

    // SECTION_DATA
    section->convertToDataSection();
    ASSERT_EQ(section->getSize(), 0);
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_FALSE(section->getRecord(recordCompare, 0));

    ASSERT_TRUE(section->pushRecord(recordData1));
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getDataSize(), 16);

    ASSERT_TRUE(section->pushRecord(recordData2.getRecord()));
    ASSERT_EQ(section->getSize(), 2);
    ASSERT_EQ(section->getDataSize(), 32);

    ASSERT_FALSE(section->pushRecord(recordEndOfData));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordExtendedSegmentAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordStartSegmentAddress));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordExtendedLinearAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordStartLinearAddress));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordUnknown));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordChecksum));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_TRUE(section->getRecord(recordCompare, 0));
    ASSERT_EQ(recordCompare.getRecord(), recordData1.getRecord());

    ASSERT_TRUE(section->getRecord(recordCompare, 1));
    ASSERT_EQ(recordCompare.getRecord(), recordData2.getRecord());

    ASSERT_FALSE(section->getRecord(recordCompare, 2));

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_TRUE(section->getRecord(recordCompare, 0));
    ASSERT_EQ(recordCompare.getRecord(), recordEndOfData.getRecord());

    ASSERT_FALSE(section->pushRecord(recordData1));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordEndOfData.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordExtendedSegmentAddress));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordStartSegmentAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordExtendedLinearAddress));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordStartLinearAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordUnknown.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordChecksum.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->getRecord(recordCompare, 1));

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    section->convertToExtendedSegmentAddressSection(0x1728);
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_TRUE(section->getRecord(recordCompare, 0));
    ASSERT_EQ(recordCompare.getRecord(), std::string(":020000021728BD"));

    ASSERT_TRUE(section->pushRecord(recordData1.getRecord()));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordEndOfData));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordExtendedSegmentAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordStartSegmentAddress));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordExtendedLinearAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordStartLinearAddress));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordUnknown));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_FALSE(section->pushRecord(recordChecksum));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_TRUE(section->getRecord(recordCompare, 1));
    ASSERT_EQ(recordCompare.getRecord(), recordData1.getRecord());

    ASSERT_FALSE(section->getRecord(recordCompare, 2));

    // SECTION_START_SEGMENT_ADDRESS
    section->convertToStartSegmentAddressSection(0x394A, 0x5B6C);
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_TRUE(section->getRecord(recordCompare, 0));
    ASSERT_EQ(recordCompare.getRecord(), std::string(":04000003394A5B6CAF"));

    ASSERT_FALSE(section->pushRecord(recordData1));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordEndOfData.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordExtendedSegmentAddress));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordStartSegmentAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordExtendedLinearAddress));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordStartLinearAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordUnknown.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordChecksum.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->getRecord(recordCompare, 1));

    // SECTION_EXTENDED_LINEAR_ADDRESS
    section->convertToExtendedLinearAddressSection(0x7D8E);
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_TRUE(section->getRecord(recordCompare, 0));
    ASSERT_EQ(recordCompare.getRecord(), std::string(":020000047D8EEF"));

    ASSERT_TRUE(section->pushRecord(recordData1));
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_TRUE(section->pushRecord(recordData2.getRecord()));
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_FALSE(section->pushRecord(recordEndOfData));
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_FALSE(section->pushRecord(recordExtendedSegmentAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_FALSE(section->pushRecord(recordStartSegmentAddress));
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_FALSE(section->pushRecord(recordExtendedLinearAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_FALSE(section->pushRecord(recordStartLinearAddress));
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_FALSE(section->pushRecord(recordUnknown));
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_FALSE(section->pushRecord(recordChecksum));
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_TRUE(section->getRecord(recordCompare, 1));
    ASSERT_EQ(recordCompare.getRecord(), recordData1.getRecord());

    ASSERT_TRUE(section->getRecord(recordCompare, 2));
    ASSERT_EQ(recordCompare.getRecord(), recordData2.getRecord());

    ASSERT_FALSE(section->getRecord(recordCompare, 3));

    // SECTION_START_LINEAR_ADDRESS
    section->convertToStartLinearAddressSection(0x9FA0B1C2);
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getDataSize(), 0);

    ASSERT_TRUE(section->getRecord(recordCompare, 0));
    ASSERT_EQ(recordCompare.getRecord(), std::string(":040000059FA0B1C245"));

    ASSERT_FALSE(section->pushRecord(recordData2));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordEndOfData.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordExtendedSegmentAddress));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordStartSegmentAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordExtendedLinearAddress));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordStartLinearAddress.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordUnknown.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->pushRecord(recordChecksum.getRecord()));
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_FALSE(section->getRecord(recordCompare, 1));

    // Invalid records
    section->convertToDataSection();

    ASSERT_FALSE(section->pushRecord(""));
    ASSERT_FALSE(section->pushRecord("AABBCCDDEEFF"));
    ASSERT_FALSE(section->pushRecord(":"));
    ASSERT_FALSE(section->pushRecord(":04"));
    ASSERT_FALSE(section->pushRecord(":02000000"));
    ASSERT_FALSE(section->pushRecord(":020000FF"));
    ASSERT_FALSE(section->pushRecord(":020000001A"));
    ASSERT_FALSE(section->pushRecord(":020000001A2B"));
}

void HexSectionTest::performFindAddressTests()
{
    SCOPED_TRACE(__func__);

    // SECTION_DATA
    section->convertToDataSection();

    ASSERT_TRUE(section->hasAddress());
    ASSERT_FALSE(section->hasData());
    ASSERT_EQ(section->findAddress(0x1000), std::string::npos);

    ASSERT_TRUE(section->pushRecord(":081000000D3B8C10A0438A2F68"));
    ASSERT_TRUE(section->hasData());
    ASSERT_EQ(section->getLowerAddress(), 0x1000);
    ASSERT_EQ(section->getUpperAddress(), 0x1007);

    ASSERT_EQ(section->findAddress(0x0FFF), std::string::npos);
    ASSERT_EQ(section->findAddress(0x1000), 0);
    ASSERT_EQ(section->findAddress(0x1007), 0);
    ASSERT_EQ(section->findAddress(0x1008), std::string::npos);

    ASSERT_TRUE(section->pushRecord(":08200000050D43E25144404785"));
    ASSERT_EQ(section->getLowerAddress(), 0x1000);
    ASSERT_EQ(section->getUpperAddress(), 0x2007);

    ASSERT_EQ(section->findAddress(0x1FFF), std::string::npos);
    ASSERT_EQ(section->findAddress(0x1004), 0);
    ASSERT_EQ(section->findAddress(0x2000), 1);
    ASSERT_EQ(section->findAddress(0x2007), 1);
    ASSERT_EQ(section->findAddress(0x2008), std::string::npos);

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_FALSE(section->hasAddress());
    ASSERT_FALSE(section->hasData());

    ASSERT_THROW(section->findAddress(0x0), std::domain_error);
    ASSERT_THROW(section->findAddress(0x7FFF), std::domain_error);
    ASSERT_THROW(section->findAddress(0xFFFF), std::domain_error);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    section->convertToExtendedSegmentAddressSection(0x899A);

    ASSERT_TRUE(section->hasAddress());
    ASSERT_FALSE(section->hasData());
    ASSERT_EQ(section->findAddress(0x1000), std::string::npos);

    ASSERT_TRUE(section->pushRecord(":08200000AE74333F34F2B088E6"));
    ASSERT_TRUE(section->hasAddress());
    ASSERT_TRUE(section->hasData());
    ASSERT_EQ(section->getLowerAddress(), 0x2000);
    ASSERT_EQ(section->getUpperAddress(), 0x2007);

    ASSERT_EQ(section->findAddress(0x1FFF), std::string::npos);
    ASSERT_EQ(section->findAddress(0x2000), 0);
    ASSERT_EQ(section->findAddress(0x2007), 0);
    ASSERT_EQ(section->findAddress(0x2008), std::string::npos);

    ASSERT_TRUE(section->pushRecord(":08300000E3A81CF7172CF0F8FF"));
    ASSERT_EQ(section->getLowerAddress(), 0x2000);
    ASSERT_EQ(section->getUpperAddress(), 0x3007);

    ASSERT_EQ(section->findAddress(0x1FFF), std::string::npos);
    ASSERT_EQ(section->findAddress(0x2004), 0);
    ASSERT_EQ(section->findAddress(0x3000), 1);
    ASSERT_EQ(section->findAddress(0x3007), 1);
    ASSERT_EQ(section->findAddress(0x3008), std::string::npos);

    // SECTION_START_SEGMENT_ADDRESS
    section->convertToStartSegmentAddressSection(0xABBC, 0xCDDE);

    ASSERT_FALSE(section->hasAddress());
    ASSERT_FALSE(section->hasData());

    ASSERT_THROW(section->findAddress(0x0), std::domain_error);
    ASSERT_THROW(section->findAddress(0x7FFF), std::domain_error);
    ASSERT_THROW(section->findAddress(0xFFFF), std::domain_error);

    // SECTION_EXTENDED_LINEAR_SEGMENT
    section->convertToExtendedLinearAddressSection(0xEFF0);

    ASSERT_TRUE(section->hasAddress());
    ASSERT_FALSE(section->hasData());
    ASSERT_EQ(section->findAddress(0x1000), std::string::npos);

    ASSERT_TRUE(section->pushRecord(":10300000CC1662BE3CD54B0E1DC2AB8D6072BF406C"));
    ASSERT_TRUE(section->hasAddress());
    ASSERT_TRUE(section->hasData());
    ASSERT_EQ(section->getLowerAddress(), 0x3000);
    ASSERT_EQ(section->getUpperAddress(), 0x300F);

    ASSERT_EQ(section->findAddress(0x2FFF), std::string::npos);
    ASSERT_EQ(section->findAddress(0x3000), 0);
    ASSERT_EQ(section->findAddress(0x300F), 0);
    ASSERT_EQ(section->findAddress(0x3010), std::string::npos);

    ASSERT_TRUE(section->pushRecord(":0430110075AC183151"));
    ASSERT_EQ(section->getLowerAddress(), 0x3000);
    ASSERT_EQ(section->getUpperAddress(), 0x3014);

    ASSERT_EQ(section->findAddress(0x2FFF), std::string::npos);
    ASSERT_EQ(section->findAddress(0x300F), 0);
    ASSERT_EQ(section->findAddress(0x3010), std::string::npos);
    ASSERT_EQ(section->findAddress(0x3011), 1);
    ASSERT_EQ(section->findAddress(0x3014), 1);
    ASSERT_EQ(section->findAddress(0x3015), std::string::npos);

    // SECTION_LINEAR_SEGMENT_ADDRESS
    section->convertToStartLinearAddressSection(0x1122334);

    ASSERT_FALSE(section->hasAddress());
    ASSERT_FALSE(section->hasData());

    ASSERT_THROW(section->findAddress(0x0), std::domain_error);
    ASSERT_THROW(section->findAddress(0x7FFF), std::domain_error);
    ASSERT_THROW(section->findAddress(0xFFFF), std::domain_error);
}

void HexSectionTest::performAddressTests()
{
    SCOPED_TRACE(__func__);

    // SECTION_DATA
    const HexSection section1{":108E110096A055EBF7B49A5529853CF3998DE191CC"};
    ASSERT_TRUE(section1.hasAddress());
    ASSERT_TRUE(section1.hasData());
    ASSERT_EQ(section1.getLowerAddress(), 0x8E11);
    ASSERT_EQ(section1.getUpperAddress(), 0x8E20);

    ASSERT_EQ(section1.getMinAbsoluteAddress(), 0);
    ASSERT_EQ(section1.getMaxAbsoluteAddress(), 0xFFFF);

    ASSERT_TRUE(section1.hasAbsoluteAddress(0));
    ASSERT_TRUE(section1.hasAbsoluteAddress(0x8990));
    ASSERT_TRUE(section1.hasAbsoluteAddress(0xFFFF));
    ASSERT_FALSE(section1.hasAbsoluteAddress(0x10000));

    ASSERT_EQ(section1.getAbsoluteAddress(0), 0);
    ASSERT_EQ(section1.getAbsoluteAddress(0xA1B2), 0xA1B2);
    ASSERT_EQ(section1.getAbsoluteAddress(0xFFFF), 0xFFFF);

    ASSERT_EQ(section1.getRelativeAddress(0), 0);
    ASSERT_EQ(section1.getRelativeAddress(0xC3D4), 0xC3D4);
    ASSERT_EQ(section1.getRelativeAddress(0xFFFF), 0xFFFF);
    ASSERT_THROW(section1.getRelativeAddress(0x10000), std::out_of_range);
    ASSERT_THROW(section1.getRelativeAddress(0xFFFFFFFF), std::out_of_range);

    // SECTION_END_OF_FILE
    const HexSection section2{":00000001FF"};
    ASSERT_FALSE(section2.hasAddress());
    ASSERT_FALSE(section2.hasData());
    ASSERT_THROW(section2.getLowerAddress(), std::domain_error);
    ASSERT_THROW(section2.getUpperAddress(), std::domain_error);

    ASSERT_THROW(section2.getMinAbsoluteAddress(), std::domain_error);
    ASSERT_THROW(section2.getMaxAbsoluteAddress(), std::domain_error);

    ASSERT_THROW(section2.hasAbsoluteAddress(0), std::domain_error);
    ASSERT_THROW(section2.getAbsoluteAddress(0), std::domain_error);
    ASSERT_THROW(section2.getRelativeAddress(0), std::domain_error);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    HexSection section3{":020000021A2BB7"};
    ASSERT_TRUE(section3.hasAddress());
    ASSERT_FALSE(section3.hasData());

    ASSERT_THROW(section3.getLowerAddress(), std::out_of_range);
    ASSERT_THROW(section3.getUpperAddress(), std::out_of_range);

    section3.pushRecord(":101200000899B5A65CF554A78623B5042551F001CD");
    ASSERT_TRUE(section3.hasData());
    ASSERT_EQ(section3.getLowerAddress(), 0x1200);
    ASSERT_EQ(section3.getUpperAddress(), 0x120F);

    ASSERT_EQ(section3.getMinAbsoluteAddress(), 0x1A2B0);
    ASSERT_EQ(section3.getMaxAbsoluteAddress(), 0x2A2AF);

    ASSERT_FALSE(section3.hasAbsoluteAddress(0x1A2AF));
    ASSERT_TRUE(section3.hasAbsoluteAddress(0x1A2B0));
    ASSERT_TRUE(section3.hasAbsoluteAddress(0x2A2AF));
    ASSERT_FALSE(section3.hasAbsoluteAddress(0x2A2B0));

    ASSERT_EQ(section3.getAbsoluteAddress(0), 0x1A2B0);
    ASSERT_EQ(section3.getAbsoluteAddress(0xB0C1), 0x25371);
    ASSERT_EQ(section3.getAbsoluteAddress(0xFFFF), 0x2A2AF);

    ASSERT_THROW(section3.getRelativeAddress(0x1A2AF), std::out_of_range);
    ASSERT_EQ(section3.getRelativeAddress(0x1A2B0), 0);
    ASSERT_EQ(section3.getRelativeAddress(0x26684), 0xC3D4);
    ASSERT_EQ(section3.getRelativeAddress(0x2A2AF), 0xFFFF);
    ASSERT_THROW(section3.getRelativeAddress(0x2A2B0), std::out_of_range);

    // SECTION_START_SEGMENT_ADDRESS
    const HexSection section4{":040000033C4D5E6FA3"};
    ASSERT_FALSE(section4.hasAddress());
    ASSERT_FALSE(section4.hasData());
    ASSERT_THROW(section4.getLowerAddress(), std::domain_error);
    ASSERT_THROW(section4.getUpperAddress(), std::domain_error);

    ASSERT_THROW(section4.getMinAbsoluteAddress(), std::domain_error);
    ASSERT_THROW(section4.getMaxAbsoluteAddress(), std::domain_error);

    ASSERT_THROW(section4.hasAbsoluteAddress(0), std::domain_error);
    ASSERT_THROW(section4.getAbsoluteAddress(0), std::domain_error);
    ASSERT_THROW(section4.getRelativeAddress(0), std::domain_error);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    HexSection section5{":02000004607228"};
    ASSERT_TRUE(section5.hasAddress());
    ASSERT_FALSE(section5.hasData());
    ASSERT_THROW(section5.getLowerAddress(), std::out_of_range);
    ASSERT_THROW(section5.getUpperAddress(), std::out_of_range);

    section5.pushRecord(":1018F0004518BE2D7C452B37063D7369B73A7A4CA7");
    ASSERT_TRUE(section5.hasData());
    ASSERT_EQ(section5.getLowerAddress(), 0x18F0);
    ASSERT_EQ(section5.getUpperAddress(), 0x18FF);

    ASSERT_EQ(section5.getMinAbsoluteAddress(), 0x60720000);
    ASSERT_EQ(section5.getMaxAbsoluteAddress(), 0x6072FFFF);

    ASSERT_FALSE(section5.hasAbsoluteAddress(0x6071FFFF));
    ASSERT_TRUE(section5.hasAbsoluteAddress(0x60720000));
    ASSERT_TRUE(section5.hasAbsoluteAddress(0x6072FFFF));
    ASSERT_FALSE(section5.hasAbsoluteAddress(0x60730000));

    ASSERT_EQ(section5.getAbsoluteAddress(0), 0x60720000);
    ASSERT_EQ(section5.getAbsoluteAddress(0x8394), 0x60728394);
    ASSERT_EQ(section5.getAbsoluteAddress(0xFFFF), 0x6072FFFF);

    ASSERT_THROW(section5.getRelativeAddress(0x6071FFFF), std::out_of_range);
    ASSERT_EQ(section5.getRelativeAddress(0x60720000), 0);
    ASSERT_EQ(section5.getRelativeAddress(0x6072A5B6), 0xA5B6);
    ASSERT_EQ(section5.getRelativeAddress(0x6072FFFF), 0xFFFF);
    ASSERT_THROW(section5.getRelativeAddress(0x60730000), std::out_of_range);

    // SECTION_START_LINEAR_ADDRESS
    const HexSection section6{":040000058394A5B685"};
    ASSERT_FALSE(section6.hasAddress());
    ASSERT_FALSE(section6.hasData());
    ASSERT_THROW(section6.getLowerAddress(), std::domain_error);
    ASSERT_THROW(section6.getUpperAddress(), std::domain_error);

    ASSERT_THROW(section6.getMinAbsoluteAddress(), std::domain_error);
    ASSERT_THROW(section6.getMaxAbsoluteAddress(), std::domain_error);

    ASSERT_THROW(section6.hasAbsoluteAddress(0), std::domain_error);
    ASSERT_THROW(section6.getAbsoluteAddress(0), std::domain_error);
    ASSERT_THROW(section6.getRelativeAddress(0), std::domain_error);
}

void HexSectionTest::performIntersectionTests()
{
    SCOPED_TRACE(__func__);

    // SECTION_DATA
    const HexSection sectionData1{":10100000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0"};
    const HexSection sectionData2{":10200000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0"};
    ASSERT_TRUE(sectionData1.checkIntersect(sectionData2));
    ASSERT_TRUE(sectionData2.checkIntersect(sectionData1));

    // SECTION_END_OF_FILE
    const HexSection sectionEndOfFile{":00000001FF"};
    ASSERT_THROW(sectionEndOfFile.checkIntersect(sectionEndOfFile), std::domain_error);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    const HexSection sectionExtendedSegmentAddress1{":020000020000FC"};
    const HexSection sectionExtendedSegmentAddress2{":020000020800F4"};
    const HexSection sectionExtendedSegmentAddress3{":020000021000EC"};
    const HexSection sectionExtendedSegmentAddress4{":020000021800E4"};
    const HexSection sectionExtendedSegmentAddress5{":020000022000DC"};

    ASSERT_TRUE(sectionExtendedSegmentAddress1.checkIntersect(sectionExtendedSegmentAddress1));
    ASSERT_TRUE(sectionExtendedSegmentAddress1.checkIntersect(sectionExtendedSegmentAddress2));
    ASSERT_FALSE(sectionExtendedSegmentAddress1.checkIntersect(sectionExtendedSegmentAddress3));
    ASSERT_FALSE(sectionExtendedSegmentAddress1.checkIntersect(sectionExtendedSegmentAddress4));
    ASSERT_FALSE(sectionExtendedSegmentAddress1.checkIntersect(sectionExtendedSegmentAddress5));

    ASSERT_TRUE(sectionExtendedSegmentAddress2.checkIntersect(sectionExtendedSegmentAddress1));
    ASSERT_TRUE(sectionExtendedSegmentAddress2.checkIntersect(sectionExtendedSegmentAddress2));
    ASSERT_TRUE(sectionExtendedSegmentAddress2.checkIntersect(sectionExtendedSegmentAddress3));
    ASSERT_FALSE(sectionExtendedSegmentAddress2.checkIntersect(sectionExtendedSegmentAddress4));
    ASSERT_FALSE(sectionExtendedSegmentAddress2.checkIntersect(sectionExtendedSegmentAddress5));

    ASSERT_FALSE(sectionExtendedSegmentAddress3.checkIntersect(sectionExtendedSegmentAddress1));
    ASSERT_TRUE(sectionExtendedSegmentAddress3.checkIntersect(sectionExtendedSegmentAddress2));
    ASSERT_TRUE(sectionExtendedSegmentAddress3.checkIntersect(sectionExtendedSegmentAddress3));
    ASSERT_TRUE(sectionExtendedSegmentAddress3.checkIntersect(sectionExtendedSegmentAddress4));
    ASSERT_FALSE(sectionExtendedSegmentAddress3.checkIntersect(sectionExtendedSegmentAddress5));

    ASSERT_FALSE(sectionExtendedSegmentAddress4.checkIntersect(sectionExtendedSegmentAddress1));
    ASSERT_FALSE(sectionExtendedSegmentAddress4.checkIntersect(sectionExtendedSegmentAddress2));
    ASSERT_TRUE(sectionExtendedSegmentAddress4.checkIntersect(sectionExtendedSegmentAddress3));
    ASSERT_TRUE(sectionExtendedSegmentAddress4.checkIntersect(sectionExtendedSegmentAddress4));
    ASSERT_TRUE(sectionExtendedSegmentAddress4.checkIntersect(sectionExtendedSegmentAddress5));

    ASSERT_FALSE(sectionExtendedSegmentAddress5.checkIntersect(sectionExtendedSegmentAddress1));
    ASSERT_FALSE(sectionExtendedSegmentAddress5.checkIntersect(sectionExtendedSegmentAddress2));
    ASSERT_FALSE(sectionExtendedSegmentAddress5.checkIntersect(sectionExtendedSegmentAddress3));
    ASSERT_TRUE(sectionExtendedSegmentAddress5.checkIntersect(sectionExtendedSegmentAddress4));
    ASSERT_TRUE(sectionExtendedSegmentAddress5.checkIntersect(sectionExtendedSegmentAddress5));

    // SECTION_START_SEGMENT_ADDRESS
    const HexSection sectionStartSegmentAddress{":040000032435465703"};
    ASSERT_THROW(sectionStartSegmentAddress.checkIntersect(sectionStartSegmentAddress), std::domain_error);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    const HexSection sectionExtendedLinearAddress1{":020000040000FA"};
    const HexSection sectionExtendedLinearAddress2{":020000040001F9"};
    ASSERT_TRUE(sectionExtendedLinearAddress1.checkIntersect(sectionExtendedLinearAddress1));
    ASSERT_FALSE(sectionExtendedLinearAddress1.checkIntersect(sectionExtendedLinearAddress2));
    ASSERT_TRUE(sectionExtendedLinearAddress2.checkIntersect(sectionExtendedLinearAddress2));
    ASSERT_FALSE(sectionExtendedLinearAddress2.checkIntersect(sectionExtendedLinearAddress1));

    // SECTION_START_LINEAR_ADDRESS
    const HexSection sectionStartLinearAddress{":040000058394A5B685"};
    ASSERT_THROW(sectionStartLinearAddress.checkIntersect(sectionStartLinearAddress), std::domain_error);

    // Mixed sections
    ASSERT_THROW(sectionData1.checkIntersect(sectionEndOfFile), std::domain_error);
    ASSERT_THROW(sectionEndOfFile.checkIntersect(sectionExtendedSegmentAddress1), std::domain_error);
    ASSERT_THROW(sectionExtendedSegmentAddress1.checkIntersect(sectionStartSegmentAddress), std::domain_error);
    ASSERT_THROW(sectionStartSegmentAddress.checkIntersect(sectionExtendedLinearAddress1), std::domain_error);
    ASSERT_THROW(sectionExtendedLinearAddress1.checkIntersect(sectionStartLinearAddress), std::domain_error);
    ASSERT_THROW(sectionStartLinearAddress.checkIntersect(sectionData1), std::domain_error);
}

void HexSectionTest::performFindPreviousRecordTests()
{
    SCOPED_TRACE(__func__);

    // SECTION_DATA
    section->convertToDataSection();

    ASSERT_TRUE(section->pushRecord(":10100000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0"));
    ASSERT_TRUE(section->pushRecord(":10200000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0"));
    ASSERT_TRUE(section->pushRecord(":10201000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD0"));
    ASSERT_TRUE(section->pushRecord(":08300000FFFFFFFFFFFFFFFFD0"));

    ASSERT_EQ(section->findPreviousRecord(0), std::string::npos);
    ASSERT_EQ(section->findPreviousRecord(0x0FFF), std::string::npos);

    ASSERT_EQ(section->findPreviousRecord(0x1000), std::string::npos);
    ASSERT_EQ(section->findPreviousRecord(0x1001), 0);
    ASSERT_EQ(section->findPreviousRecord(0x100F), 0);

    ASSERT_EQ(section->findPreviousRecord(0x1010), 0);
    ASSERT_EQ(section->findPreviousRecord(0x1FFF), 0);

    ASSERT_EQ(section->findPreviousRecord(0x2000), 0);
    ASSERT_EQ(section->findPreviousRecord(0x2001), 1);
    ASSERT_EQ(section->findPreviousRecord(0x200F), 1);

    ASSERT_EQ(section->findPreviousRecord(0x2010), 1);
    ASSERT_EQ(section->findPreviousRecord(0x2011), 2);
    ASSERT_EQ(section->findPreviousRecord(0x201F), 2);

    ASSERT_EQ(section->findPreviousRecord(0x2020), 2);
    ASSERT_EQ(section->findPreviousRecord(0x2FFF), 2);

    ASSERT_EQ(section->findPreviousRecord(0x3000), 2);
    ASSERT_EQ(section->findPreviousRecord(0x3001), 3);
    ASSERT_EQ(section->findPreviousRecord(0x3007), 3);
    ASSERT_EQ(section->findPreviousRecord(0x3008), 3);

    ASSERT_EQ(section->findPreviousRecord(0xFFFF), 3);

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_THROW(section->findPreviousRecord(0), std::domain_error);
    ASSERT_THROW(section->findPreviousRecord(0x7FFF), std::domain_error);
    ASSERT_THROW(section->findPreviousRecord(0xFFFF), std::domain_error);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    section->convertToExtendedSegmentAddressSection(0x1021);

    ASSERT_TRUE(section->pushRecord(":10100000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0"));
    ASSERT_TRUE(section->pushRecord(":10200000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0"));
    ASSERT_TRUE(section->pushRecord(":10201000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD0"));
    ASSERT_TRUE(section->pushRecord(":08300000FFFFFFFFFFFFFFFFD0"));

    ASSERT_EQ(section->findPreviousRecord(0), std::string::npos);
    ASSERT_EQ(section->findPreviousRecord(0x0FFF), std::string::npos);

    ASSERT_EQ(section->findPreviousRecord(0x1000), std::string::npos);
    ASSERT_EQ(section->findPreviousRecord(0x1001), 0);
    ASSERT_EQ(section->findPreviousRecord(0x100F), 0);

    ASSERT_EQ(section->findPreviousRecord(0x1010), 0);
    ASSERT_EQ(section->findPreviousRecord(0x1FFF), 0);

    ASSERT_EQ(section->findPreviousRecord(0x2000), 0);
    ASSERT_EQ(section->findPreviousRecord(0x2001), 1);
    ASSERT_EQ(section->findPreviousRecord(0x200F), 1);

    ASSERT_EQ(section->findPreviousRecord(0x2010), 1);
    ASSERT_EQ(section->findPreviousRecord(0x2011), 2);
    ASSERT_EQ(section->findPreviousRecord(0x201F), 2);

    ASSERT_EQ(section->findPreviousRecord(0x2020), 2);
    ASSERT_EQ(section->findPreviousRecord(0x2FFF), 2);

    ASSERT_EQ(section->findPreviousRecord(0x3000), 2);
    ASSERT_EQ(section->findPreviousRecord(0x3001), 3);
    ASSERT_EQ(section->findPreviousRecord(0x3007), 3);
    ASSERT_EQ(section->findPreviousRecord(0x3008), 3);

    ASSERT_EQ(section->findPreviousRecord(0xFFFF), 3);

    // SECTION_START_SEGMENT_ADDRESS
    section->convertToStartSegmentAddressSection(0x3243, 0x5465);

    ASSERT_THROW(section->findPreviousRecord(0), std::domain_error);
    ASSERT_THROW(section->findPreviousRecord(0x7FFF), std::domain_error);
    ASSERT_THROW(section->findPreviousRecord(0xFFFF), std::domain_error);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    section->convertToExtendedLinearAddressSection(0x7687);

    ASSERT_TRUE(section->pushRecord(":10100000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0"));
    ASSERT_TRUE(section->pushRecord(":10200000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0"));
    ASSERT_TRUE(section->pushRecord(":10201000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD0"));
    ASSERT_TRUE(section->pushRecord(":08300000FFFFFFFFFFFFFFFFD0"));

    ASSERT_EQ(section->findPreviousRecord(0), std::string::npos);
    ASSERT_EQ(section->findPreviousRecord(0x0FFF), std::string::npos);

    ASSERT_EQ(section->findPreviousRecord(0x1000), std::string::npos);
    ASSERT_EQ(section->findPreviousRecord(0x1001), 0);
    ASSERT_EQ(section->findPreviousRecord(0x100F), 0);

    ASSERT_EQ(section->findPreviousRecord(0x1010), 0);
    ASSERT_EQ(section->findPreviousRecord(0x1FFF), 0);

    ASSERT_EQ(section->findPreviousRecord(0x2000), 0);
    ASSERT_EQ(section->findPreviousRecord(0x2001), 1);
    ASSERT_EQ(section->findPreviousRecord(0x200F), 1);

    ASSERT_EQ(section->findPreviousRecord(0x2010), 1);
    ASSERT_EQ(section->findPreviousRecord(0x2011), 2);
    ASSERT_EQ(section->findPreviousRecord(0x201F), 2);

    ASSERT_EQ(section->findPreviousRecord(0x2020), 2);
    ASSERT_EQ(section->findPreviousRecord(0x2FFF), 2);

    ASSERT_EQ(section->findPreviousRecord(0x3000), 2);
    ASSERT_EQ(section->findPreviousRecord(0x3001), 3);
    ASSERT_EQ(section->findPreviousRecord(0x3007), 3);
    ASSERT_EQ(section->findPreviousRecord(0x3008), 3);

    ASSERT_EQ(section->findPreviousRecord(0xFFFF), 3);

    // SECTION_START_LINEAR_ADDRESS
    section->convertToStartLinearAddressSection(0x98A9BACB);

    ASSERT_THROW(section->findPreviousRecord(0), std::domain_error);
    ASSERT_THROW(section->findPreviousRecord(0x7FFF), std::domain_error);
    ASSERT_THROW(section->findPreviousRecord(0xFFFF), std::domain_error);
}

void HexSectionTest::performFindNextRecordTests()
{
    SCOPED_TRACE(__func__);

    // SECTION_DATA
    section->convertToDataSection();

    ASSERT_TRUE(section->pushRecord(":10100000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0"));
    ASSERT_TRUE(section->pushRecord(":10200000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0"));
    ASSERT_TRUE(section->pushRecord(":10201000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD0"));
    ASSERT_TRUE(section->pushRecord(":08300000FFFFFFFFFFFFFFFFD0"));

    ASSERT_EQ(section->findNextRecord(0), 0);
    ASSERT_EQ(section->findNextRecord(0x0FFF), 0);

    ASSERT_EQ(section->findNextRecord(0x1000), 0);
    ASSERT_EQ(section->findNextRecord(0x100E), 0);
    ASSERT_EQ(section->findNextRecord(0x100F), 1);

    ASSERT_EQ(section->findNextRecord(0x1010), 1);
    ASSERT_EQ(section->findNextRecord(0x1FFF), 1);

    ASSERT_EQ(section->findNextRecord(0x2000), 1);
    ASSERT_EQ(section->findNextRecord(0x200E), 1);
    ASSERT_EQ(section->findNextRecord(0x200F), 2);

    ASSERT_EQ(section->findNextRecord(0x2010), 2);
    ASSERT_EQ(section->findNextRecord(0x201E), 2);
    ASSERT_EQ(section->findNextRecord(0x201F), 3);

    ASSERT_EQ(section->findNextRecord(0x2020), 3);
    ASSERT_EQ(section->findNextRecord(0x2FFF), 3);

    ASSERT_EQ(section->findNextRecord(0x3000), 3);
    ASSERT_EQ(section->findNextRecord(0x3006), 3);
    ASSERT_EQ(section->findNextRecord(0x3007), std::string::npos);
    ASSERT_EQ(section->findNextRecord(0x3008), std::string::npos);

    ASSERT_EQ(section->findNextRecord(0xFFFF), std::string::npos);

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_THROW(section->findNextRecord(0), std::domain_error);
    ASSERT_THROW(section->findNextRecord(0x7FFF), std::domain_error);
    ASSERT_THROW(section->findNextRecord(0xFFFF), std::domain_error);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    section->convertToExtendedSegmentAddressSection(0x0112);

    ASSERT_TRUE(section->pushRecord(":10100000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0"));
    ASSERT_TRUE(section->pushRecord(":10200000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0"));
    ASSERT_TRUE(section->pushRecord(":10201000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD0"));
    ASSERT_TRUE(section->pushRecord(":08300000FFFFFFFFFFFFFFFFD0"));

    ASSERT_EQ(section->findNextRecord(0), 0);
    ASSERT_EQ(section->findNextRecord(0x0FFF), 0);

    ASSERT_EQ(section->findNextRecord(0x1000), 0);
    ASSERT_EQ(section->findNextRecord(0x100E), 0);
    ASSERT_EQ(section->findNextRecord(0x100F), 1);

    ASSERT_EQ(section->findNextRecord(0x1010), 1);
    ASSERT_EQ(section->findNextRecord(0x1FFF), 1);

    ASSERT_EQ(section->findNextRecord(0x2000), 1);
    ASSERT_EQ(section->findNextRecord(0x200E), 1);
    ASSERT_EQ(section->findNextRecord(0x200F), 2);

    ASSERT_EQ(section->findNextRecord(0x2010), 2);
    ASSERT_EQ(section->findNextRecord(0x201E), 2);
    ASSERT_EQ(section->findNextRecord(0x201F), 3);

    ASSERT_EQ(section->findNextRecord(0x2020), 3);
    ASSERT_EQ(section->findNextRecord(0x2FFF), 3);

    ASSERT_EQ(section->findNextRecord(0x3000), 3);
    ASSERT_EQ(section->findNextRecord(0x3006), 3);
    ASSERT_EQ(section->findNextRecord(0x3007), std::string::npos);
    ASSERT_EQ(section->findNextRecord(0x3008), std::string::npos);

    ASSERT_EQ(section->findNextRecord(0xFFFF), std::string::npos);

    // SECTION_START_SEGMENT_ADDRESS
    section->convertToStartSegmentAddressSection(0x2334, 0x4556);

    ASSERT_THROW(section->findNextRecord(0), std::domain_error);
    ASSERT_THROW(section->findNextRecord(0x7FFF), std::domain_error);
    ASSERT_THROW(section->findNextRecord(0xFFFF), std::domain_error);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    section->convertToExtendedLinearAddressSection(0x6778);

    ASSERT_TRUE(section->pushRecord(":10100000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0"));
    ASSERT_TRUE(section->pushRecord(":10200000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0"));
    ASSERT_TRUE(section->pushRecord(":10201000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD0"));
    ASSERT_TRUE(section->pushRecord(":08300000FFFFFFFFFFFFFFFFD0"));

    ASSERT_EQ(section->findNextRecord(0), 0);
    ASSERT_EQ(section->findNextRecord(0x0FFF), 0);

    ASSERT_EQ(section->findNextRecord(0x1000), 0);
    ASSERT_EQ(section->findNextRecord(0x100E), 0);
    ASSERT_EQ(section->findNextRecord(0x100F), 1);

    ASSERT_EQ(section->findNextRecord(0x1010), 1);
    ASSERT_EQ(section->findNextRecord(0x1FFF), 1);

    ASSERT_EQ(section->findNextRecord(0x2000), 1);
    ASSERT_EQ(section->findNextRecord(0x200E), 1);
    ASSERT_EQ(section->findNextRecord(0x200F), 2);

    ASSERT_EQ(section->findNextRecord(0x2010), 2);
    ASSERT_EQ(section->findNextRecord(0x201E), 2);
    ASSERT_EQ(section->findNextRecord(0x201F), 3);

    ASSERT_EQ(section->findNextRecord(0x2020), 3);
    ASSERT_EQ(section->findNextRecord(0x2FFF), 3);

    ASSERT_EQ(section->findNextRecord(0x3000), 3);
    ASSERT_EQ(section->findNextRecord(0x3006), 3);
    ASSERT_EQ(section->findNextRecord(0x3007), std::string::npos);
    ASSERT_EQ(section->findNextRecord(0x3008), std::string::npos);

    ASSERT_EQ(section->findNextRecord(0xFFFF), std::string::npos);

    // SECTION_START_LINEAR_ADDRESS
    section->convertToStartLinearAddressSection(0x899AABBC);

    ASSERT_THROW(section->findNextRecord(0), std::domain_error);
    ASSERT_THROW(section->findNextRecord(0x7FFF), std::domain_error);
    ASSERT_THROW(section->findNextRecord(0xFFFF), std::domain_error);
}

void HexSectionTest::performClearDataTests()
{
    SCOPED_TRACE(__func__);

    std::string record{};
    section->convertToDataSection();

    ASSERT_FALSE(section->getRecord(record, 0));

    ASSERT_TRUE(section->pushRecord(":1010000027C5B86473D2A5769701D11A9C6F9AEB65"));
    ASSERT_TRUE(section->pushRecord(":101010007C7728D755D9C20CD9C048BA4FA2EC1357"));
    ASSERT_EQ(section->getSize(), 2);
    ASSERT_EQ(section->getDataSize(), 32);

    // SECTION_DATA (clearData(address))
    section->clearData(0xFFF);
    section->clearData(0x1020);
    section->clearData(0x100F);

    ASSERT_EQ(section->getSize(), 2);
    ASSERT_EQ(section->getDataSize(), 31);
    ASSERT_TRUE(section->getRecord(record, 0));
    ASSERT_EQ(record, std::string(":0F10000027C5B86473D2A5769701D11A9C6F9A51"));
    ASSERT_TRUE(section->getRecord(record, 1));
    ASSERT_EQ(record, std::string(":101010007C7728D755D9C20CD9C048BA4FA2EC1357"));

    section->clearData(0x1007);

    ASSERT_EQ(section->getSize(), 3);
    ASSERT_EQ(section->getDataSize(), 30);
    ASSERT_TRUE(section->getRecord(record, 0));
    ASSERT_EQ(record, std::string(":0710000027C5B86473D2A5F7"));
    ASSERT_TRUE(section->getRecord(record, 1));
    ASSERT_EQ(record, std::string(":071008009701D11A9C6F9AB9"));
    ASSERT_TRUE(section->getRecord(record, 2));
    ASSERT_EQ(record, std::string(":101010007C7728D755D9C20CD9C048BA4FA2EC1357"));

    section->clearData(0x1000);

    ASSERT_EQ(section->getSize(), 3);
    ASSERT_EQ(section->getDataSize(), 29);
    ASSERT_TRUE(section->getRecord(record, 0));
    ASSERT_EQ(record, std::string(":06100100C5B86473D2A51E"));
    ASSERT_TRUE(section->getRecord(record, 1));
    ASSERT_EQ(record, std::string(":071008009701D11A9C6F9AB9"));
    ASSERT_TRUE(section->getRecord(record, 2));
    ASSERT_EQ(record, std::string(":101010007C7728D755D9C20CD9C048BA4FA2EC1357"));

    // SECTION_DATA (clearData())
    section->clearData();
    ASSERT_EQ(section->getSize(), 0);
    ASSERT_EQ(section->getDataSize(), 0);

    // SECTION_DATA (clearData(address, size))
    ASSERT_TRUE(section->pushRecord(":1020000005A21139144A264CBDFEDD54C2F9CE0694"));
    ASSERT_TRUE(section->pushRecord(":1020100017281FCE2203CD2547784AEE7B39ABD354"));
    ASSERT_TRUE(section->pushRecord(":1020300070F76074A4BFC196176793423E733289EC"));
    ASSERT_EQ(section->getSize(), 3);
    ASSERT_EQ(section->getDataSize(), 48);

    ASSERT_EQ(section->clearData(0x1000, 0xFFF), 0xFFF);
    ASSERT_EQ(section->clearData(0x2040, 0xFBF), 0xFBF);
    ASSERT_EQ(section->clearData(0x2080, 16), 16);
    ASSERT_EQ(section->getSize(), 3);
    ASSERT_EQ(section->getDataSize(), 48);

    ASSERT_EQ(section->clearData(0x2020, 16), 16);
    ASSERT_EQ(section->getSize(), 3);
    ASSERT_EQ(section->getDataSize(), 48);

    ASSERT_EQ(section->clearData(0x2004, 4), 4);
    ASSERT_EQ(section->getSize(), 4);
    ASSERT_EQ(section->getDataSize(), 44);
    ASSERT_TRUE(section->getRecord(record, 0));
    ASSERT_EQ(record, std::string(":0420000005A21139EB"));
    ASSERT_TRUE(section->getRecord(record, 1));
    ASSERT_EQ(record, std::string(":08200800BDFEDD54C2F9CE0655"));

    ASSERT_EQ(section->clearData(0x2008, 4), 4);
    ASSERT_EQ(section->getSize(), 4);
    ASSERT_EQ(section->getDataSize(), 40);
    ASSERT_TRUE(section->getRecord(record, 1));
    ASSERT_EQ(record, std::string(":04200C00C2F9CE0641"));

    ASSERT_EQ(section->clearData(0x200C, 4), 4);
    ASSERT_EQ(section->getDataSize(), 36);
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_EQ(section->clearData(0x201C, 24), 24);
    ASSERT_EQ(section->getSize(), 3);
    ASSERT_EQ(section->getDataSize(), 28);
    ASSERT_TRUE(section->getRecord(record, 1));
    ASSERT_EQ(record, std::string(":0C20100017281FCE2203CD2547784AEE8A"));
    ASSERT_TRUE(section->getRecord(record, 2));
    ASSERT_EQ(record, std::string(":0C203400A4BFC196176793423E73328927"));

    ASSERT_EQ(section->clearData(0x2000, 2), 2);
    ASSERT_EQ(section->getSize(), 3);
    ASSERT_EQ(section->getDataSize(), 26);
    ASSERT_TRUE(section->getRecord(record, 0));
    ASSERT_EQ(record, std::string(":02200200113992"));

    ASSERT_THROW(section->clearData(0, 0x10001), std::out_of_range);
    ASSERT_THROW(section->clearData(0x2008, 0), std::out_of_range);
    ASSERT_EQ(section->getSize(), 3);
    ASSERT_EQ(section->getDataSize(), 26);

    ASSERT_EQ(section->clearData(0, 0x10000), 0x10000);
    ASSERT_EQ(section->getSize(), 0);
    ASSERT_EQ(section->getDataSize(), 0);

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_THROW(section->clearData(0), std::domain_error);
    ASSERT_THROW(section->clearData(0, 0xFFFF), std::domain_error);
}

void HexSectionTest::performGetDataTests()
{
    SCOPED_TRACE(__func__);

    // Initialize
    section->convertToDataSection();

    ASSERT_TRUE(section->pushRecord(":02000200E60B0B"));
    ASSERT_TRUE(section->pushRecord(":0200060019B32C"));
    ASSERT_TRUE(section->pushRecord(":02000A00FF41B4"));
    ASSERT_TRUE(section->pushRecord(":02000E00DA0A0C"));

    const auto& sectionConstRef{*section};
    auto& sectionRef{*section};

    // SECTION_DATA #1 (array index operator)
    ASSERT_THROW(sectionConstRef[0], std::out_of_range);
    ASSERT_THROW(sectionConstRef[1], std::out_of_range);
    ASSERT_EQ(sectionConstRef[2], 0xE6);
    ASSERT_EQ(sectionConstRef[3], 0x0B);
    ASSERT_THROW(sectionConstRef[4], std::out_of_range);
    ASSERT_THROW(sectionConstRef[5], std::out_of_range);
    ASSERT_EQ(sectionConstRef[6], 0x19);
    ASSERT_EQ(sectionConstRef[7], 0xB3);
    ASSERT_THROW(sectionConstRef[8], std::out_of_range);
    ASSERT_THROW(sectionConstRef[9], std::out_of_range);
    ASSERT_EQ(sectionConstRef[10], 0xFF);
    ASSERT_EQ(sectionConstRef[11], 0x41);
    ASSERT_THROW(sectionConstRef[12], std::out_of_range);
    ASSERT_THROW(sectionConstRef[13], std::out_of_range);
    ASSERT_EQ(sectionConstRef[14], 0xDA);
    ASSERT_EQ(sectionConstRef[15], 0x0A);

    ASSERT_THROW(sectionRef[0], std::out_of_range);
    ASSERT_THROW(sectionRef[1], std::out_of_range);
    ASSERT_EQ(sectionRef[2], 0xE6);
    ASSERT_EQ(sectionRef[3], 0x0B);
    ASSERT_THROW(sectionRef[4], std::out_of_range);
    ASSERT_THROW(sectionRef[5], std::out_of_range);
    ASSERT_EQ(sectionRef[6], 0x19);
    ASSERT_EQ(sectionRef[7], 0xB3);
    ASSERT_THROW(sectionRef[8], std::out_of_range);
    ASSERT_THROW(sectionRef[9], std::out_of_range);
    ASSERT_EQ(sectionRef[10], 0xFF);
    ASSERT_EQ(sectionRef[11], 0x41);
    ASSERT_THROW(sectionRef[12], std::out_of_range);
    ASSERT_THROW(sectionRef[13], std::out_of_range);
    ASSERT_EQ(sectionRef[14], 0xDA);
    ASSERT_EQ(sectionRef[15], 0x0A);

    // SECTION_DATA #2 (getData)
    ASSERT_EQ(sectionConstRef.getData(0), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(1), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(2), 0xE6);
    ASSERT_EQ(sectionConstRef.getData(3), 0x0B);
    ASSERT_EQ(sectionConstRef.getData(4), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(5), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(6), 0x19);
    ASSERT_EQ(sectionConstRef.getData(7), 0xB3);
    ASSERT_EQ(sectionConstRef.getData(8), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(9), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(10), 0xFF);
    ASSERT_EQ(sectionConstRef.getData(11), 0x41);
    ASSERT_EQ(sectionConstRef.getData(12), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(13), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(14), 0xDA);
    ASSERT_EQ(sectionConstRef.getData(15), 0x0A);

    ASSERT_EQ(sectionRef.getData(0), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(1), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(2), 0xE6);
    ASSERT_EQ(sectionRef.getData(3), 0x0B);
    ASSERT_EQ(sectionRef.getData(4), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(5), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(6), 0x19);
    ASSERT_EQ(sectionRef.getData(7), 0xB3);
    ASSERT_EQ(sectionRef.getData(8), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(9), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(10), 0xFF);
    ASSERT_EQ(sectionRef.getData(11), 0x41);
    ASSERT_EQ(sectionRef.getData(12), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(13), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(14), 0xDA);
    ASSERT_EQ(sectionRef.getData(15), 0x0A);

    section->convertToExtendedSegmentAddressSection(0xA1B2);

    // SECTION_EXTENDED_SEGMENT_ADDRESS #1 (array index operator)
    ASSERT_THROW(sectionConstRef[0], std::out_of_range);
    ASSERT_THROW(sectionConstRef[1], std::out_of_range);
    ASSERT_EQ(sectionConstRef[2], 0xE6);
    ASSERT_EQ(sectionConstRef[3], 0x0B);
    ASSERT_THROW(sectionConstRef[4], std::out_of_range);
    ASSERT_THROW(sectionConstRef[5], std::out_of_range);
    ASSERT_EQ(sectionConstRef[6], 0x19);
    ASSERT_EQ(sectionConstRef[7], 0xB3);
    ASSERT_THROW(sectionConstRef[8], std::out_of_range);
    ASSERT_THROW(sectionConstRef[9], std::out_of_range);
    ASSERT_EQ(sectionConstRef[10], 0xFF);
    ASSERT_EQ(sectionConstRef[11], 0x41);
    ASSERT_THROW(sectionConstRef[12], std::out_of_range);
    ASSERT_THROW(sectionConstRef[13], std::out_of_range);
    ASSERT_EQ(sectionConstRef[14], 0xDA);
    ASSERT_EQ(sectionConstRef[15], 0x0A);

    ASSERT_THROW(sectionRef[0], std::out_of_range);
    ASSERT_THROW(sectionRef[1], std::out_of_range);
    ASSERT_EQ(sectionRef[2], 0xE6);
    ASSERT_EQ(sectionRef[3], 0x0B);
    ASSERT_THROW(sectionRef[4], std::out_of_range);
    ASSERT_THROW(sectionRef[5], std::out_of_range);
    ASSERT_EQ(sectionRef[6], 0x19);
    ASSERT_EQ(sectionRef[7], 0xB3);
    ASSERT_THROW(sectionRef[8], std::out_of_range);
    ASSERT_THROW(sectionRef[9], std::out_of_range);
    ASSERT_EQ(sectionRef[10], 0xFF);
    ASSERT_EQ(sectionRef[11], 0x41);
    ASSERT_THROW(sectionRef[12], std::out_of_range);
    ASSERT_THROW(sectionRef[13], std::out_of_range);
    ASSERT_EQ(sectionRef[14], 0xDA);
    ASSERT_EQ(sectionRef[15], 0x0A);

    // SECTION_EXTENDED_SEGMENT_ADDRESS #2 (getData)
    ASSERT_EQ(sectionConstRef.getData(0), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(1), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(2), 0xE6);
    ASSERT_EQ(sectionConstRef.getData(3), 0x0B);
    ASSERT_EQ(sectionConstRef.getData(4), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(5), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(6), 0x19);
    ASSERT_EQ(sectionConstRef.getData(7), 0xB3);
    ASSERT_EQ(sectionConstRef.getData(8), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(9), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(10), 0xFF);
    ASSERT_EQ(sectionConstRef.getData(11), 0x41);
    ASSERT_EQ(sectionConstRef.getData(12), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(13), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(14), 0xDA);
    ASSERT_EQ(sectionConstRef.getData(15), 0x0A);

    ASSERT_EQ(sectionRef.getData(0), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(1), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(2), 0xE6);
    ASSERT_EQ(sectionRef.getData(3), 0x0B);
    ASSERT_EQ(sectionRef.getData(4), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(5), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(6), 0x19);
    ASSERT_EQ(sectionRef.getData(7), 0xB3);
    ASSERT_EQ(sectionRef.getData(8), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(9), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(10), 0xFF);
    ASSERT_EQ(sectionRef.getData(11), 0x41);
    ASSERT_EQ(sectionRef.getData(12), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(13), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(14), 0xDA);
    ASSERT_EQ(sectionRef.getData(15), 0x0A);

    section->convertToExtendedLinearAddressSection(0xC3D4);

    // SECTION_EXTENDED_LINEAR_ADDRESS #1 (array index operator)
    ASSERT_THROW(sectionConstRef[0], std::out_of_range);
    ASSERT_THROW(sectionConstRef[1], std::out_of_range);
    ASSERT_EQ(sectionConstRef[2], 0xE6);
    ASSERT_EQ(sectionConstRef[3], 0x0B);
    ASSERT_THROW(sectionConstRef[4], std::out_of_range);
    ASSERT_THROW(sectionConstRef[5], std::out_of_range);
    ASSERT_EQ(sectionConstRef[6], 0x19);
    ASSERT_EQ(sectionConstRef[7], 0xB3);
    ASSERT_THROW(sectionConstRef[8], std::out_of_range);
    ASSERT_THROW(sectionConstRef[9], std::out_of_range);
    ASSERT_EQ(sectionConstRef[10], 0xFF);
    ASSERT_EQ(sectionConstRef[11], 0x41);
    ASSERT_THROW(sectionConstRef[12], std::out_of_range);
    ASSERT_THROW(sectionConstRef[13], std::out_of_range);
    ASSERT_EQ(sectionConstRef[14], 0xDA);
    ASSERT_EQ(sectionConstRef[15], 0x0A);

    ASSERT_THROW(sectionRef[0], std::out_of_range);
    ASSERT_THROW(sectionRef[1], std::out_of_range);
    ASSERT_EQ(sectionRef[2], 0xE6);
    ASSERT_EQ(sectionRef[3], 0x0B);
    ASSERT_THROW(sectionRef[4], std::out_of_range);
    ASSERT_THROW(sectionRef[5], std::out_of_range);
    ASSERT_EQ(sectionRef[6], 0x19);
    ASSERT_EQ(sectionRef[7], 0xB3);
    ASSERT_THROW(sectionRef[8], std::out_of_range);
    ASSERT_THROW(sectionRef[9], std::out_of_range);
    ASSERT_EQ(sectionRef[10], 0xFF);
    ASSERT_EQ(sectionRef[11], 0x41);
    ASSERT_THROW(sectionRef[12], std::out_of_range);
    ASSERT_THROW(sectionRef[13], std::out_of_range);
    ASSERT_EQ(sectionRef[14], 0xDA);
    ASSERT_EQ(sectionRef[15], 0x0A);

    // SECTION_EXTENDED_LINEAR_ADDRESS #2 (getData)
    ASSERT_EQ(sectionConstRef.getData(0), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(1), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(2), 0xE6);
    ASSERT_EQ(sectionConstRef.getData(3), 0x0B);
    ASSERT_EQ(sectionConstRef.getData(4), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(5), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(6), 0x19);
    ASSERT_EQ(sectionConstRef.getData(7), 0xB3);
    ASSERT_EQ(sectionConstRef.getData(8), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(9), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(10), 0xFF);
    ASSERT_EQ(sectionConstRef.getData(11), 0x41);
    ASSERT_EQ(sectionConstRef.getData(12), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(13), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionConstRef.getData(14), 0xDA);
    ASSERT_EQ(sectionConstRef.getData(15), 0x0A);

    ASSERT_EQ(sectionRef.getData(0), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(1), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(2), 0xE6);
    ASSERT_EQ(sectionRef.getData(3), 0x0B);
    ASSERT_EQ(sectionRef.getData(4), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(5), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(6), 0x19);
    ASSERT_EQ(sectionRef.getData(7), 0xB3);
    ASSERT_EQ(sectionRef.getData(8), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(9), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(10), 0xFF);
    ASSERT_EQ(sectionRef.getData(11), 0x41);
    ASSERT_EQ(sectionRef.getData(12), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(13), DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(sectionRef.getData(14), 0xDA);
    ASSERT_EQ(sectionRef.getData(15), 0x0A);

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_THROW(sectionConstRef[0], std::domain_error);
    ASSERT_THROW(sectionRef[0], std::domain_error);

    ASSERT_THROW(sectionConstRef.getData(0), std::domain_error);
    ASSERT_THROW(sectionRef.getData(0), std::domain_error);

    // SECTION_START_SEGMENT_ADDRESS
    section->convertToStartSegmentAddressSection(0xE5F6, 0x0718);

    ASSERT_THROW(sectionConstRef[0], std::domain_error);
    ASSERT_THROW(sectionRef[0], std::domain_error);

    ASSERT_THROW(sectionConstRef.getData(0), std::domain_error);
    ASSERT_THROW(sectionRef.getData(0), std::domain_error);

    // SECTION_START_LINEAR_ADDRESS
    section->convertToStartLinearAddressSection(0x293A);

    ASSERT_THROW(sectionConstRef[0], std::domain_error);
    ASSERT_THROW(sectionRef[0], std::domain_error);

    ASSERT_THROW(sectionConstRef.getData(0), std::domain_error);
    ASSERT_THROW(sectionRef.getData(0), std::domain_error);
}

void HexSectionTest::performGetDataArrayTests()
{
    SCOPED_TRACE(__func__);

    // SECTION_DATA #1
    section->convertToDataSection();

    const uint8_t defaultValue1{0xA5};
    section->setUnusedDataFillValue(defaultValue1);
    ASSERT_EQ(section->getUnusedDataFillValue(), defaultValue1);

    HexDataArray<16> data{0};

    ASSERT_EQ(section->getData(0, &data[0], data.size()), data.size());
    for (HexDataSizeType index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], defaultValue1);

    ASSERT_TRUE(section->pushRecord(":020002000718DD"));
    ASSERT_TRUE(section->pushRecord(":02000600293A95"));
    ASSERT_TRUE(section->pushRecord(":02000A004B5C4D"));
    ASSERT_TRUE(section->pushRecord(":02000E006D7E05"));

    ASSERT_EQ(section->getData(0, &data[0], data.size()), data.size());

    ASSERT_EQ(data[0], defaultValue1);
    ASSERT_EQ(data[1], defaultValue1);
    ASSERT_EQ(data[2], 0x07);
    ASSERT_EQ(data[3], 0x18);
    ASSERT_EQ(data[4], defaultValue1);
    ASSERT_EQ(data[5], defaultValue1);
    ASSERT_EQ(data[6], 0x29);
    ASSERT_EQ(data[7], 0x3A);
    ASSERT_EQ(data[8], defaultValue1);
    ASSERT_EQ(data[9], defaultValue1);
    ASSERT_EQ(data[10], 0x4B);
    ASSERT_EQ(data[11], 0x5C);
    ASSERT_EQ(data[12], defaultValue1);
    ASSERT_EQ(data[13], defaultValue1);
    ASSERT_EQ(data[14], 0x6D);
    ASSERT_EQ(data[15], 0x7E);

    // SECTION_DATA #2
    ASSERT_TRUE(section->pushRecord(":101000007896653BC31F9DD80C226BC26A20AC4BFF"));

    const uint8_t defaultValue2{0x11};
    section->setUnusedDataFillValue(defaultValue2);
    ASSERT_EQ(section->getUnusedDataFillValue(), defaultValue2);

    HexDataArray<8> data2{0};
    ASSERT_EQ(section->getData(0xFFE, &data2[0], data2.size()), data2.size());

    ASSERT_EQ(data2[0], defaultValue2);
    ASSERT_EQ(data2[1], defaultValue2);
    ASSERT_EQ(data2[2], 0x78);
    ASSERT_EQ(data2[3], 0x96);
    ASSERT_EQ(data2[4], 0x65);
    ASSERT_EQ(data2[5], 0x3B);
    ASSERT_EQ(data2[6], 0xC3);
    ASSERT_EQ(data2[7], 0x1F);

    // SECTION_DATA #3
    ASSERT_TRUE(section->pushRecord(":10200000DBA29746B00C7682EF0FA8DF15BACB049F"));

    const uint8_t defaultValue3{0xDD};
    section->setUnusedDataFillValue(defaultValue3);
    ASSERT_EQ(section->getUnusedDataFillValue(), defaultValue3);

    HexDataArray<8> data3{0};
    ASSERT_EQ(section->getData(0x200A, &data3[0], data3.size()), data3.size());

    ASSERT_EQ(data3[0], 0xA8);
    ASSERT_EQ(data3[1], 0xDF);
    ASSERT_EQ(data3[2], 0x15);
    ASSERT_EQ(data3[3], 0xBA);
    ASSERT_EQ(data3[4], 0xCB);
    ASSERT_EQ(data3[5], 0x04);
    ASSERT_EQ(data3[6], defaultValue3);
    ASSERT_EQ(data3[7], defaultValue3);

    // SECTION_DATA #4
    ASSERT_TRUE(section->pushRecord(":103000000E03CEA3ED42F8FDEFC873D7347E2DB783"));
    ASSERT_TRUE(section->pushRecord(":10301000D06B50E0E0C47DD3E52178929DD5A1D559"));

    const uint8_t defaultValue4{0x5A};
    section->setUnusedDataFillValue(defaultValue4);
    ASSERT_EQ(section->getUnusedDataFillValue(), defaultValue4);

    HexDataArray<8> data4{0};
    ASSERT_EQ(section->getData(0x300C, &data4[0], data4.size()), data4.size());

    ASSERT_EQ(data4[0], 0x34);
    ASSERT_EQ(data4[1], 0x7E);
    ASSERT_EQ(data4[2], 0x2D);
    ASSERT_EQ(data4[3], 0xB7);
    ASSERT_EQ(data4[4], 0xD0);
    ASSERT_EQ(data4[5], 0x6B);
    ASSERT_EQ(data4[6], 0x50);
    ASSERT_EQ(data4[7], 0xE0);

    // SECTION_DATA #5
    section->setUnusedDataFillValue();
    ASSERT_EQ(section->getUnusedDataFillValue(), DEFAULT_HEX_RECORD_DATA_VALUE);

    HexDataArray<16> data5{0};
    ASSERT_EQ(section->getData(0xFFF0, &data5[0], data5.size()), data5.size());
    for (HexDataSizeType index{0}; index < data5.size(); ++index)
        ASSERT_EQ(data5[index], DEFAULT_HEX_RECORD_DATA_VALUE);

    ASSERT_TRUE(section->pushRecord(":08FFF8007F2215BC5E5C5F1E58"));

    ASSERT_EQ(section->getData(0xFFF0, &data5[0], data5.size()), data5.size());

    ASSERT_EQ(data5[0x0], DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(data5[0x7], DEFAULT_HEX_RECORD_DATA_VALUE);
    ASSERT_EQ(data5[0x8], 0x7F);
    ASSERT_EQ(data5[0x9], 0x22);
    ASSERT_EQ(data5[0xA], 0x15);
    ASSERT_EQ(data5[0xB], 0xBC);
    ASSERT_EQ(data5[0xC], 0x5E);
    ASSERT_EQ(data5[0xD], 0x5C);
    ASSERT_EQ(data5[0xE], 0x5F);
    ASSERT_EQ(data5[0xF], 0x1E);

    ASSERT_THROW(section->getData(0xFFF0, &data5[0], data5.size() + 1), std::out_of_range);
    ASSERT_THROW(section->getData(0xFFFF, &data5[0], 2), std::out_of_range);

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_THROW(section->getData(0, &data[0], 0), std::domain_error);
}

void HexSectionTest::performGetDataVectorTests()
{
    SCOPED_TRACE(__func__);

    // SECTION_DATA #1
    section->convertToDataSection();

    const uint8_t defaultValue1{0x66};
    section->setUnusedDataFillValue(defaultValue1);
    ASSERT_EQ(section->getUnusedDataFillValue(), defaultValue1);

    HexDataVector data(8, 0xAA);

    ASSERT_EQ(section->getData(0, data, data.size()), data.size());
    for (HexDataSizeType index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], defaultValue1);

    ASSERT_TRUE(section->pushRecord(":020002008F90DD"));
    ASSERT_TRUE(section->pushRecord(":02000600A1B2A5"));
    ASSERT_TRUE(section->pushRecord(":02000A00C3D45D"));
    ASSERT_TRUE(section->pushRecord(":02000E00E5F615"));

    ASSERT_EQ(section->getData(0, data, 16), 16);

    ASSERT_EQ(data[0], defaultValue1);
    ASSERT_EQ(data[1], defaultValue1);
    ASSERT_EQ(data[2], 0x8F);
    ASSERT_EQ(data[3], 0x90);
    ASSERT_EQ(data[4], defaultValue1);
    ASSERT_EQ(data[5], defaultValue1);
    ASSERT_EQ(data[6], 0xA1);
    ASSERT_EQ(data[7], 0xB2);
    ASSERT_EQ(data[8], defaultValue1);
    ASSERT_EQ(data[9], defaultValue1);
    ASSERT_EQ(data[10], 0xC3);
    ASSERT_EQ(data[11], 0xD4);
    ASSERT_EQ(data[12], defaultValue1);
    ASSERT_EQ(data[13], defaultValue1);
    ASSERT_EQ(data[14], 0xE5);
    ASSERT_EQ(data[15], 0xF6);

    // SECTION_DATA #2
    ASSERT_TRUE(section->pushRecord(":10100000A8B556245B10D7E7B7276693683C122231"));

    const uint8_t defaultValue2{0x77};
    section->setUnusedDataFillValue(defaultValue2);
    ASSERT_EQ(section->getUnusedDataFillValue(), defaultValue2);

    data.resize(4, 0xBB);
    ASSERT_EQ(section->getData(0xFFE, data, 8), 8);

    ASSERT_EQ(data[0], defaultValue2);
    ASSERT_EQ(data[1], defaultValue2);
    ASSERT_EQ(data[2], 0xA8);
    ASSERT_EQ(data[3], 0xB5);
    ASSERT_EQ(data[4], 0x56);
    ASSERT_EQ(data[5], 0x24);
    ASSERT_EQ(data[6], 0x5B);
    ASSERT_EQ(data[7], 0x10);

    // SECTION_DATA #3
    ASSERT_TRUE(section->pushRecord(":1020000036AC8E1160477E3AE7A97C3A7B4CE503FB"));

    const uint8_t defaultValue3{0x88};
    section->setUnusedDataFillValue(defaultValue3);
    ASSERT_EQ(section->getUnusedDataFillValue(), defaultValue3);

    data.resize(8, 0xCC);
    ASSERT_EQ(section->getData(0x200A, data, 10), 10);

    ASSERT_EQ(data[0], 0x7C);
    ASSERT_EQ(data[1], 0x3A);
    ASSERT_EQ(data[2], 0x7B);
    ASSERT_EQ(data[3], 0x4C);
    ASSERT_EQ(data[4], 0xE5);
    ASSERT_EQ(data[5], 0x03);
    ASSERT_EQ(data[6], defaultValue3);
    ASSERT_EQ(data[7], defaultValue3);
    ASSERT_EQ(data[8], defaultValue3);
    ASSERT_EQ(data[9], defaultValue3);

    // SECTION_DATA #4
    ASSERT_TRUE(section->pushRecord(":10300000DE67870F21093484F073D66FB6EE6C96B5"));
    ASSERT_TRUE(section->pushRecord(":10301000EA1A01CE2EA781ECFE9C62E78F6B63D883"));

    section->setUnusedDataFillValue();
    ASSERT_EQ(section->getUnusedDataFillValue(), DEFAULT_HEX_RECORD_DATA_VALUE);

    data.resize(4, 0xDD);
    ASSERT_EQ(section->getData(0x300C, data, 8), 8);

    ASSERT_EQ(data[0], 0xB6);
    ASSERT_EQ(data[1], 0xEE);
    ASSERT_EQ(data[2], 0x6C);
    ASSERT_EQ(data[3], 0x96);
    ASSERT_EQ(data[4], 0xEA);
    ASSERT_EQ(data[5], 0x1A);
    ASSERT_EQ(data[6], 0x01);
    ASSERT_EQ(data[7], 0xCE);

    // SECTION_DATA #5
    section->setUnusedDataFillValue(defaultValue1);
    ASSERT_EQ(section->getUnusedDataFillValue(), defaultValue1);

    data.clear();
    ASSERT_THROW(section->getData(0, data, 16, data.size() + 1), std::underflow_error);
    data.resize(4, 0xAA);
    ASSERT_THROW(section->getData(0, data, 16, data.size() + 1), std::underflow_error);

    ASSERT_EQ(section->getData(0, data, 16, 2), 16);
    ASSERT_EQ(data.size(), 18);
    ASSERT_EQ(data[1], 0xAA);
    ASSERT_EQ(data[2], defaultValue1);
    ASSERT_EQ(data[3], defaultValue1);
    ASSERT_EQ(data[4], 0x8F);
    ASSERT_EQ(data[5], 0x90);
    ASSERT_EQ(data[6], defaultValue1);
    ASSERT_EQ(data[7], defaultValue1);
    ASSERT_EQ(data[8], 0xA1);
    ASSERT_EQ(data[9], 0xB2);
    ASSERT_EQ(data[10], defaultValue1);
    ASSERT_EQ(data[11], defaultValue1);
    ASSERT_EQ(data[12], 0xC3);
    ASSERT_EQ(data[13], 0xD4);
    ASSERT_EQ(data[14], defaultValue1);
    ASSERT_EQ(data[15], defaultValue1);
    ASSERT_EQ(data[16], 0xE5);
    ASSERT_EQ(data[17], 0xF6);

    section->setUnusedDataFillValue(defaultValue2);
    ASSERT_EQ(section->getUnusedDataFillValue(), defaultValue2);

    ASSERT_EQ(section->getData(0, data, 16, 4), 16);
    ASSERT_EQ(data.size(), 20);
    ASSERT_EQ(data[3], defaultValue1);
    ASSERT_EQ(data[4], defaultValue2);
    ASSERT_EQ(data[5], defaultValue2);
    ASSERT_EQ(data[6], 0x8F);
    ASSERT_EQ(data[7], 0x90);
    ASSERT_EQ(data[8], defaultValue2);
    ASSERT_EQ(data[9], defaultValue2);
    ASSERT_EQ(data[10], 0xA1);
    ASSERT_EQ(data[11], 0xB2);
    ASSERT_EQ(data[12], defaultValue2);
    ASSERT_EQ(data[13], defaultValue2);
    ASSERT_EQ(data[14], 0xC3);
    ASSERT_EQ(data[15], 0xD4);
    ASSERT_EQ(data[16], defaultValue2);
    ASSERT_EQ(data[17], defaultValue2);
    ASSERT_EQ(data[18], 0xE5);
    ASSERT_EQ(data[19], 0xF6);

    // SECTION_DATA #6
    section->setUnusedDataFillValue(defaultValue3);
    ASSERT_EQ(section->getUnusedDataFillValue(), defaultValue3);

    data.resize(16, 0xBB);
    ASSERT_EQ(section->getData(0xFFF0, data, data.size()), data.size());
    for (HexDataSizeType index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], defaultValue3);

    ASSERT_TRUE(section->pushRecord(":08FFF80014E43F7FC88D835E15"));

    ASSERT_EQ(section->getData(0xFFF0, data, data.size()), data.size());

    ASSERT_EQ(data.at(0x0), defaultValue3);
    ASSERT_EQ(data.at(0x7), defaultValue3);
    ASSERT_EQ(data.at(0x8), 0x14);
    ASSERT_EQ(data.at(0x9), 0xE4);
    ASSERT_EQ(data.at(0xA), 0x3F);
    ASSERT_EQ(data.at(0xB), 0x7F);
    ASSERT_EQ(data.at(0xC), 0xC8);
    ASSERT_EQ(data.at(0xD), 0x8D);
    ASSERT_EQ(data.at(0xE), 0x83);
    ASSERT_EQ(data.at(0xF), 0x5E);

    ASSERT_THROW(section->getData(0xFFF0, data, data.size() + 1), std::out_of_range);
    ASSERT_THROW(section->getData(0xFFFF, data, 2), std::out_of_range);

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_THROW(section->getData(0, data, 0), std::domain_error);
}

void HexSectionTest::performSetDataTests()
{
    SCOPED_TRACE(__func__);

    // Initialize
    section->convertToDataSection();

    ASSERT_TRUE(section->pushRecord(":02000200E60B0B"));
    ASSERT_TRUE(section->pushRecord(":0200060019B32C"));
    ASSERT_TRUE(section->pushRecord(":02000A00FF41B4"));
    ASSERT_TRUE(section->pushRecord(":02000E00DA0A0C"));

    auto& sectionRef{*section};

    // SECTION_DATA #1 (array index operator)
    ASSERT_THROW(sectionRef[0] = 0, std::out_of_range);
    ASSERT_THROW(sectionRef[1] = 0, std::out_of_range);
    ASSERT_NO_THROW(sectionRef[2] = 0x19);
    ASSERT_NO_THROW(sectionRef[3] = 0xF4);
    ASSERT_THROW(sectionRef[4] = 0, std::out_of_range);
    ASSERT_THROW(sectionRef[5] = 0, std::out_of_range);
    ASSERT_NO_THROW(sectionRef[6] = 0xE6);
    ASSERT_NO_THROW(sectionRef[7] = 0xC4);
    ASSERT_THROW(sectionRef[8] = 0, std::out_of_range);
    ASSERT_THROW(sectionRef[9] = 0, std::out_of_range);
    ASSERT_NO_THROW(sectionRef[10] = 0x00);
    ASSERT_NO_THROW(sectionRef[11] = 0xBE);
    ASSERT_THROW(sectionRef[12] = 0, std::out_of_range);
    ASSERT_THROW(sectionRef[13] = 0, std::out_of_range);
    ASSERT_NO_THROW(sectionRef[14] = 0x25);
    ASSERT_NO_THROW(sectionRef[15] = 0xF5);

    ASSERT_THROW(sectionRef[0] = 0, std::out_of_range);
    ASSERT_THROW(sectionRef[1] = 0, std::out_of_range);
    ASSERT_EQ(sectionRef[2], 0x19);
    ASSERT_EQ(sectionRef[3], 0xF4);
    ASSERT_THROW(sectionRef[4] = 0, std::out_of_range);
    ASSERT_THROW(sectionRef[5] = 0, std::out_of_range);
    ASSERT_EQ(sectionRef[6], 0xE6);
    ASSERT_EQ(sectionRef[7], 0xC4);
    ASSERT_THROW(sectionRef[8] = 0, std::out_of_range);
    ASSERT_THROW(sectionRef[9] = 0, std::out_of_range);
    ASSERT_EQ(sectionRef[10], 0x00);
    ASSERT_EQ(sectionRef[11], 0xBE);
    ASSERT_THROW(sectionRef[12] = 0, std::out_of_range);
    ASSERT_THROW(sectionRef[13] = 0, std::out_of_range);
    ASSERT_EQ(sectionRef[14], 0x25);
    ASSERT_EQ(sectionRef[15], 0xF5);

    // SECTION_DATA #2 (setData)
    ASSERT_NO_THROW(section->setData(0x00, 0x1A));
    ASSERT_NO_THROW(section->setData(0x01, 0x2B));
    ASSERT_NO_THROW(section->setData(0x02, 0x3C));
    ASSERT_NO_THROW(section->setData(0x03, 0x4D));
    ASSERT_NO_THROW(section->setData(0x04, 0x5E));
    ASSERT_NO_THROW(section->setData(0x05, 0x6F));
    ASSERT_NO_THROW(section->setData(0x06, 0x70));
    ASSERT_NO_THROW(section->setData(0x07, 0x81));
    ASSERT_NO_THROW(section->setData(0x08, 0x92));
    ASSERT_NO_THROW(section->setData(0x09, 0xA3));
    ASSERT_NO_THROW(section->setData(0x0A, 0xB4));
    ASSERT_NO_THROW(section->setData(0x0B, 0xC5));
    ASSERT_NO_THROW(section->setData(0x0C, 0xD6));
    ASSERT_NO_THROW(section->setData(0x0D, 0xE7));
    ASSERT_NO_THROW(section->setData(0x0E, 0xF8));
    ASSERT_NO_THROW(section->setData(0x0F, 0x09));

    ASSERT_EQ(section->getData(0x00), 0x1A);
    ASSERT_EQ(section->getData(0x01), 0x2B);
    ASSERT_EQ(section->getData(0x02), 0x3C);
    ASSERT_EQ(section->getData(0x03), 0x4D);
    ASSERT_EQ(section->getData(0x04), 0x5E);
    ASSERT_EQ(section->getData(0x05), 0x6F);
    ASSERT_EQ(section->getData(0x06), 0x70);
    ASSERT_EQ(section->getData(0x07), 0x81);
    ASSERT_EQ(section->getData(0x08), 0x92);
    ASSERT_EQ(section->getData(0x09), 0xA3);
    ASSERT_EQ(section->getData(0x0A), 0xB4);
    ASSERT_EQ(section->getData(0x0B), 0xC5);
    ASSERT_EQ(section->getData(0x0C), 0xD6);
    ASSERT_EQ(section->getData(0x0D), 0xE7);
    ASSERT_EQ(section->getData(0x0E), 0xF8);
    ASSERT_EQ(section->getData(0x0F), 0x09);

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_THROW(section->setData(0, 0), std::domain_error);
}

void HexSectionTest::performSetDataArrayTests()
{
    SCOPED_TRACE(__func__);

    section->convertToDataSection();

    HexRecord inputRecord, outputRecord;
    HexDataArray<16> inputData;

    // SECTION_DATA #1
    inputRecord.setRecord(":10100000283DB7A343EB088663DAC27C001CEADC08");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x1000, &inputData[0], inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), inputRecord.getRecord());

    // SECTION_DATA #2
    inputRecord.setRecord(":10100000329608A58A9DE6BAE6760AADD3A1AB6E04");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x1000, &inputData[0], inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), inputRecord.getRecord());

    // SECTION_DATA #3
    inputRecord.setRecord(":10100000A8F5623B7B49B79D2D0786BB40E6A8D873");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x1004, &inputData[0], inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), ":10100000329608A5A8F5623B7B49B79D2D0786BBA4");
    ASSERT_TRUE(section->getRecord(outputRecord, 1));
    ASSERT_EQ(outputRecord.getRecord(), ":0410100040E6A8D836");

    // SECTION_DATA #4
    inputRecord.setRecord(":10100000609662EE6FD7E795B779E6DF39DE12A713");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x1008, &inputData[0], inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), ":10100000329608A5A8F5623B609662EE6FD7E79529");
    ASSERT_TRUE(section->getRecord(outputRecord, 1));
    ASSERT_EQ(outputRecord.getRecord(), ":08101000B779E6DF39DE12A713");

    // SECTION_DATA #5
    inputRecord.setRecord(":10100000F2F6013D5D6A8DD969D71EC08C58F8B8DB");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x100C, &inputData[0], inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), ":10100000329608A5A8F5623B609662EEF2F6013DC5");
    ASSERT_TRUE(section->getRecord(outputRecord, 1));
    ASSERT_EQ(outputRecord.getRecord(), ":0C1010005D6A8DD969D71EC08C58F8B8F5");

    // SECTION_DATA #6
    inputRecord.setRecord(":101000007180EC4D254AF71623587035E5881A7A19");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x1020, &inputData[0], inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_TRUE(section->getRecord(outputRecord, 2));
    ASSERT_EQ(outputRecord.getRecord(), ":101020007180EC4D254AF71623587035E5881A7AF9");

    // SECTION_DATA #7
    inputRecord.setRecord(":10100000B832AFF632C433749E27A273AF519169E0");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x1018, &inputData[0], inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_TRUE(section->getRecord(outputRecord, 1));
    ASSERT_EQ(outputRecord.getRecord(), ":101010005D6A8DD969D71EC0B832AFF632C4337459");
    ASSERT_TRUE(section->getRecord(outputRecord, 2));
    ASSERT_EQ(outputRecord.getRecord(), ":101020009E27A273AF51916923587035E5881A7ACB");

    // SECTION_DATA #8
    inputRecord.setRecord(":101000004D53FFA9BD93E17E2C6B69C924EF4F447A");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    section->setDefaultDataSize(8);
    ASSERT_EQ(section->getDefaultDataSize(), 8);

    ASSERT_EQ(section->setData(0x1030, &inputData[0], inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 5);

    ASSERT_TRUE(section->getRecord(outputRecord, 3));
    ASSERT_EQ(outputRecord.getRecord(), ":081030004D53FFA9BD93E17EC1");
    ASSERT_TRUE(section->getRecord(outputRecord, 4));
    ASSERT_EQ(outputRecord.getRecord(), ":081038002C6B69C924EF4F4441");

    // SECTION_DATA #9
    inputRecord.setRecord(":10100000AE7293BFE82EF5DCF94D80837FD7B63101");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    section->setDefaultDataSize(DEFAULT_HEX_RECORD_DATA_SIZE);
    ASSERT_EQ(section->getDefaultDataSize(), DEFAULT_HEX_RECORD_DATA_SIZE);

    ASSERT_EQ(section->setData(0x1034, &inputData[8], 8), 8);
    ASSERT_EQ(section->getSize(), 5);

    ASSERT_TRUE(section->getRecord(outputRecord, 3));
    ASSERT_EQ(outputRecord.getRecord(), ":081030004D53FFA9F94D808327");
    ASSERT_TRUE(section->getRecord(outputRecord, 4));
    ASSERT_EQ(outputRecord.getRecord(), ":081038007FD7B63124EF4F44CD");

    // SECTION_DATA #10
    inputRecord.setRecord(":1010000040A60712C13A07B71A59DE4D4505C6205A");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0xFFF8, &inputData[0], 8), 8);
    ASSERT_THROW(section->setData(0xFFF0, &inputData[0], inputData.size() + 1), std::out_of_range);
    ASSERT_THROW(section->setData(0xFFFF, &inputData[0], 2), std::out_of_range);
    ASSERT_THROW(section->setData(0, nullptr, 0x10001), std::out_of_range);

    ASSERT_EQ(section->getSize(), 6);
    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), ":10100000329608A5A8F5623B609662EEF2F6013DC5");
    ASSERT_TRUE(section->getRecord(outputRecord, 5));
    ASSERT_EQ(outputRecord.getRecord(), ":08FFF80040A60712C13A07B749");

    // SECTION_DATA #11
    inputRecord.setRecord(":10000000DCD1224680FCD154F10DB7B07F33B20E63");
    EXPECT_EQ(inputRecord.getData(&inputData[0], inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0, &inputData[0], inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 7);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), ":10000000DCD1224680FCD154F10DB7B07F33B20E63");
    ASSERT_TRUE(section->getRecord(outputRecord, 1));
    ASSERT_EQ(outputRecord.getRecord(), ":10100000329608A5A8F5623B609662EEF2F6013DC5");

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_THROW(section->setData(0, &inputData[0], 0), std::domain_error);
}

void HexSectionTest::performSetDataVectorTests()
{
    SCOPED_TRACE(__func__);

    section->convertToDataSection();

    HexRecord inputRecord, outputRecord;
    HexDataVector inputData(16, DEFAULT_HEX_RECORD_DATA_VALUE);

    // SECTION_DATA #1
    inputRecord.setRecord(":10200000283DB7A343EB088663DAC27C001CEADCF8");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x2000, inputData, inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), inputRecord.getRecord());

    // SECTION_DATA #2
    inputRecord.setRecord(":10200000329608A58A9DE6BAE6760AADD3A1AB6EF4");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x2000, inputData, inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 1);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), inputRecord.getRecord());

    // SECTION_DATA #3
    inputRecord.setRecord(":10200000A8F5623B7B49B79D2D0786BB40E6A8D863");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x2004, inputData, inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), ":10200000329608A5A8F5623B7B49B79D2D0786BB94");
    ASSERT_TRUE(section->getRecord(outputRecord, 1));
    ASSERT_EQ(outputRecord.getRecord(), ":0420100040E6A8D826");

    // SECTION_DATA #4
    inputRecord.setRecord(":10200000609662EE6FD7E795B779E6DF39DE12A703");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x2008, inputData, inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), ":10200000329608A5A8F5623B609662EE6FD7E79519");
    ASSERT_TRUE(section->getRecord(outputRecord, 1));
    ASSERT_EQ(outputRecord.getRecord(), ":08201000B779E6DF39DE12A703");

    // SECTION_DATA #5
    inputRecord.setRecord(":10200000F2F6013D5D6A8DD969D71EC08C58F8B8CB");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x200C, inputData, inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 2);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), ":10200000329608A5A8F5623B609662EEF2F6013DB5");
    ASSERT_TRUE(section->getRecord(outputRecord, 1));
    ASSERT_EQ(outputRecord.getRecord(), ":0C2010005D6A8DD969D71EC08C58F8B8E5");

    // SECTION_DATA #6
    inputRecord.setRecord(":102000007180EC4D254AF71623587035E5881A7A09");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x2020, inputData, inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_TRUE(section->getRecord(outputRecord, 2));
    ASSERT_EQ(outputRecord.getRecord(), ":102020007180EC4D254AF71623587035E5881A7AE9");

    // SECTION_DATA #7
    inputRecord.setRecord(":10200000B832AFF632C433749E27A273AF519169D0");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x2018, inputData, inputData.size()), inputData.size());
    ASSERT_EQ(section->getSize(), 3);

    ASSERT_TRUE(section->getRecord(outputRecord, 1));
    ASSERT_EQ(outputRecord.getRecord(), ":102010005D6A8DD969D71EC0B832AFF632C4337449");
    ASSERT_TRUE(section->getRecord(outputRecord, 2));
    ASSERT_EQ(outputRecord.getRecord(), ":102020009E27A273AF51916923587035E5881A7ABB");

    // SECTION_DATA #8
    inputRecord.setRecord(":102000004D53FFA9BD93E17E2C6B69C924EF4F446A");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    section->setDefaultDataSize(8);
    ASSERT_EQ(section->getDefaultDataSize(), 8);

    ASSERT_EQ(section->setData(0x2030, inputData, inputData.size(), 0), inputData.size());
    ASSERT_EQ(section->getSize(), 5);

    ASSERT_TRUE(section->getRecord(outputRecord, 3));
    ASSERT_EQ(outputRecord.getRecord(), ":082030004D53FFA9BD93E17EB1");
    ASSERT_TRUE(section->getRecord(outputRecord, 4));
    ASSERT_EQ(outputRecord.getRecord(), ":082038002C6B69C924EF4F4431");

    // SECTION_DATA #9
    inputRecord.setRecord(":10200000AE7293BFE82EF5DCF94D80837FD7B631F1");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    section->setDefaultDataSize(DEFAULT_HEX_RECORD_DATA_SIZE);
    ASSERT_EQ(section->getDefaultDataSize(), DEFAULT_HEX_RECORD_DATA_SIZE);

    ASSERT_EQ(section->setData(0x2034, inputData, 8, 8), 8);
    ASSERT_EQ(section->getSize(), 5);

    ASSERT_TRUE(section->getRecord(outputRecord, 3));
    ASSERT_EQ(outputRecord.getRecord(), ":082030004D53FFA9F94D808317");
    ASSERT_TRUE(section->getRecord(outputRecord, 4));
    ASSERT_EQ(outputRecord.getRecord(), ":082038007FD7B63124EF4F44BD");

    // SECTION_DATA #10
    inputRecord.setRecord(":1020000040A60712C13A07B71A59DE4D4505C6204A");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0xFFF8, inputData, 8), 8);
    ASSERT_THROW(section->setData(0xFFF0, inputData, inputData.size() + 1), std::out_of_range);
    ASSERT_THROW(section->setData(0xFFFF, inputData, 2), std::out_of_range);
    ASSERT_THROW(section->setData(0, nullptr, 0x10001), std::out_of_range);

    ASSERT_EQ(section->getSize(), 6);
    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), ":10200000329608A5A8F5623B609662EEF2F6013DB5");
    ASSERT_TRUE(section->getRecord(outputRecord, 5));
    ASSERT_EQ(outputRecord.getRecord(), ":08FFF80040A60712C13A07B749");

    // SECTION_DATA #11
    inputRecord.setRecord(":10200000AE7293BFE82EF5DCF94D80837FD7B631F1");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x2034, inputData, inputData.size(), 8), 8);
    ASSERT_EQ(section->getSize(), 6);

    ASSERT_TRUE(section->getRecord(outputRecord, 3));
    ASSERT_EQ(outputRecord.getRecord(), ":082030004D53FFA9F94D808317");
    ASSERT_TRUE(section->getRecord(outputRecord, 4));
    ASSERT_EQ(outputRecord.getRecord(), ":082038007FD7B63124EF4F44BD");

    // SECTION_DATA #12
    inputRecord.setRecord(":10200000060EEC2DACA6300DA19DAFD6760F2516B1");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0x2034, inputData, inputData.size(), 16), 0);
    ASSERT_EQ(section->getSize(), 6);

    ASSERT_EQ(section->setData(0x2034, inputData, inputData.size(), 32), 0);
    ASSERT_EQ(section->getSize(), 6);

    ASSERT_TRUE(section->getRecord(outputRecord, 3));
    ASSERT_EQ(outputRecord.getRecord(), ":082030004D53FFA9F94D808317");
    ASSERT_TRUE(section->getRecord(outputRecord, 4));
    ASSERT_EQ(outputRecord.getRecord(), ":082038007FD7B63124EF4F44BD");

    // SECTION_DATA #13
    inputRecord.setRecord(":10000000691969B7112B8003F0797ACDA5ACC2AD1F");
    EXPECT_EQ(inputRecord.getData(inputData, inputData.size()), inputData.size());

    ASSERT_EQ(section->setData(0, inputData, inputData.size()), 16);
    ASSERT_EQ(section->getSize(), 7);

    ASSERT_TRUE(section->getRecord(outputRecord, 0));
    ASSERT_EQ(outputRecord.getRecord(), ":10000000691969B7112B8003F0797ACDA5ACC2AD1F");
    ASSERT_TRUE(section->getRecord(outputRecord, 1));
    ASSERT_EQ(outputRecord.getRecord(), ":10200000329608A5A8F5623B609662EEF2F6013DB5");

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_THROW(section->setData(0, inputData, 0), std::domain_error);
}

void HexSectionTest::performFillDataTests()
{
    SCOPED_TRACE(__func__);

    section->convertToDataSection();

    HexDataAddressType address{0x2000};
    HexDataSizeType size{MAXIMUM_HEX_RECORD_DATA_SIZE};
    HexDataType data{0x5A};

    // SECTION_DATA #1
    ASSERT_EQ(section->fillData(address, size, data), size);
    ASSERT_EQ(section->getData(address - 1), section->getUnusedDataFillValue());
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(section->getData(address + index), data);
    ASSERT_EQ(section->getData(address + size), section->getUnusedDataFillValue());

    // SECTION_DATA #2
    address = 0x2008;
    ASSERT_EQ(section->fillData(address, size, data), size);
    ASSERT_EQ(section->getData(address - 1), data);
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(section->getData(address + index), data);
    ASSERT_EQ(section->getData(address + size), section->getUnusedDataFillValue());

    // SECTION_DATA #3
    address = 0x1FF8;
    ASSERT_EQ(section->fillData(address, size, data), size);
    ASSERT_EQ(section->getData(address - 1), section->getUnusedDataFillValue());
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(section->getData(address + index), data);
    ASSERT_EQ(section->getData(address + size), data);

    // SECTION_DATA #4
    address = 0x2000;
    ASSERT_EQ(section->fillData(address, size, data), size);
    ASSERT_EQ(section->getData(address - 1), data);
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(section->getData(address + index), data);
    ASSERT_EQ(section->getData(address + size), data);

    // SECTION_DATA #5
    address = 0x2010;
    ASSERT_EQ(section->fillData(address, size, data), size);
    ASSERT_EQ(section->getData(address - 1), data);
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(section->getData(address + index), data);
    ASSERT_EQ(section->getData(address + size), section->getUnusedDataFillValue());

    // SECTION_DATA #6
    address = 0xFFF8;
    size = 8;
    ASSERT_EQ(section->fillData(address, size, data), size);
    ASSERT_EQ(section->getData(address - 1), section->getUnusedDataFillValue());
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(section->getData(address + index), data);

    // SECTION_DATA #7
    size += 1;
    ASSERT_THROW(section->fillData(address, size, (data ^ 0x5A)), std::out_of_range);
    ASSERT_EQ(section->getData(address - 1), section->getUnusedDataFillValue());
    for (size_t index{0}; index < (size - 1); ++index)
        ASSERT_EQ(section->getData(address + index), data);

    // SECTION_END_OF_FILE
    section->convertToEndOfFileSection();

    ASSERT_THROW(section->fillData(0, 0, DEFAULT_HEX_RECORD_DATA_VALUE), std::domain_error);
}

void HexSectionTest::performCompactTests()
{
    SCOPED_TRACE(__func__);

    // Initialize
    section->convertToDataSection();
    HexRecord compareRecord;

    ASSERT_THROW(compareRecord = section->getRecord(0), std::out_of_range);

    section->compact();

    // SECTION_DATA #1
    section->setData(0x10, 0xAA);
    section->setData(0x12, 0xBB);
    section->setData(0x14, 0xCC);
    section->setData(0x16, 0xDD);

    section->setData(0x11, 0xEE);
    section->setData(0x13, 0xFF);
    section->setData(0x15, 0x00);
    section->setData(0x17, 0x11);

    ASSERT_EQ(section->getSize(), 4);
    ASSERT_EQ(section->getDataSize(), 8);
    section->compact();
    ASSERT_EQ(section->getSize(), 1);
    ASSERT_EQ(section->getDataSize(), 8);

    compareRecord = section->getRecord(0);
    ASSERT_EQ(compareRecord.getRecord(), ":08001000AAEEBBFFCC00DD11DC");

    // SECTION_DATA #2
    ASSERT_TRUE(section->pushRecord(":07002000A0B9A3FCBF499841"));
    ASSERT_TRUE(section->pushRecord(":0800280099735110F82FA2D0CA"));
    section->setData(0x27, 0xB4);

    ASSERT_EQ(section->getSize(), 3);
    ASSERT_EQ(section->getDataSize(), 24);
    section->compact();
    ASSERT_EQ(section->getSize(), 2);
    ASSERT_EQ(section->getDataSize(), 24);

    compareRecord = section->getRecord(1);
    ASSERT_EQ(compareRecord.getRecord(), ":10002000A0B9A3FCBF4998B499735110F82FA2D07E");

    // SECTION_DATA #3
    ASSERT_TRUE(section->pushRecord(":0D004000B88413098B86860A65A1FC6285D1"));
    ASSERT_TRUE(section->pushRecord(":0D004E00D29CF1C2A961314E4E3791902233"));
    ASSERT_TRUE(section->pushRecord(":0D005C00380A1C533044267CFFBE30AC7FB8"));
    section->setData(0x4D, 0xA9);
    section->setData(0x5B, 0x11);

    ASSERT_EQ(section->getSize(), 5);
    ASSERT_EQ(section->getDataSize(), 65);
    section->compact();
    ASSERT_EQ(section->getSize(), 5);
    ASSERT_EQ(section->getDataSize(), 65);

    compareRecord = section->getRecord(2);
    ASSERT_EQ(compareRecord.getRecord(), ":10004000B88413098B86860A65A1FC6285A9D29CB7");

    compareRecord = section->getRecord(3);
    ASSERT_EQ(compareRecord.getRecord(), ":10005000F1C2A961314E4E3791902211380A1C53DA");

    compareRecord = section->getRecord(4);
    ASSERT_EQ(compareRecord.getRecord(), ":090060003044267CFFBE30AC7F69");
}

END_NAMESPACE_LIBIHEX
