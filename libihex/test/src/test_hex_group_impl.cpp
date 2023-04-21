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

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <ihex/hex_address.hpp>
#include <ihex/hex_record.hpp>
#include <ihex/hex_group.hpp>
#include <ihex/hex_section.hpp>
#include <ihex/version.hpp>
#include <ihex_test/test_hex_group_impl.hpp>

BEGIN_NAMESPACE_LIBIHEX

HexGroupTest::HexGroupTest()
{

}

HexGroupTest::~HexGroupTest()
{

}

void HexGroupTest::SetUp()
{
    Test::SetUp();
    group = std::make_shared<HexGroup>();
}

void HexGroupTest::TearDown()
{
    group.reset();
    Test::TearDown();
}

void HexGroupTest::performConstructorTests()
{
    SCOPED_TRACE(__func__);

    // Default contructor
    HexGroup group1{};
    ASSERT_EQ(group1.getSize(), 0);
    ASSERT_EQ(group1.getGroupType(), HexAddressEnumType::ADDRESS_I8HEX);
}

void HexGroupTest::performCanPushSectionTests()
{
    SCOPED_TRACE(__func__);

    size_t index{0};

    // ADDRESS_I8HEX group
    ASSERT_EQ(group->getSize(), 0);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I8HEX);

    ASSERT_TRUE(group->canPushSection(dataSection, index));
    ASSERT_EQ(index, 0);
    ASSERT_TRUE(group->canPushSection(endOfFileSection, index));
    ASSERT_EQ(index, 0);
    ASSERT_TRUE(group->canPushSection(extendedSegmentAddressSection1, index));
    ASSERT_EQ(index, 0);
    ASSERT_TRUE(group->canPushSection(startSegmentAddressSection, index));
    ASSERT_EQ(index, 0);
    ASSERT_TRUE(group->canPushSection(extendedLinearAddressSection1, index));
    ASSERT_EQ(index, 0);
    ASSERT_TRUE(group->canPushSection(startLinearAddressSection, index));
    ASSERT_EQ(index, 0);

    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I8HEX);

    ASSERT_FALSE(group->canPushSection(dataSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_TRUE(group->canPushSection(endOfFileSection, index));
    ASSERT_EQ(index, 1);
    ASSERT_FALSE(group->canPushSection(extendedSegmentAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(startSegmentAddressSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedLinearAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(startLinearAddressSection, index));
    ASSERT_EQ(index, std::string::npos);

    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I8HEX);

    ASSERT_FALSE(group->canPushSection(dataSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(endOfFileSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedSegmentAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(startSegmentAddressSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedLinearAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(startLinearAddressSection, index));
    ASSERT_EQ(index, std::string::npos);

    ASSERT_EQ(group->pushSection(dataSection), std::string::npos);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I8HEX);

    // ADDRESS_I16HEX group
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I16HEX);

    ASSERT_FALSE(group->canPushSection(dataSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_TRUE(group->canPushSection(endOfFileSection, index));
    ASSERT_EQ(index, 1);
    ASSERT_FALSE(group->canPushSection(extendedSegmentAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_TRUE(group->canPushSection(extendedSegmentAddressSection2, index));
    ASSERT_EQ(index, 1);
    ASSERT_TRUE(group->canPushSection(startSegmentAddressSection, index));
    ASSERT_EQ(index, 1);
    ASSERT_FALSE(group->canPushSection(extendedLinearAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(startLinearAddressSection, index));
    ASSERT_EQ(index, std::string::npos);

    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I16HEX);

    ASSERT_FALSE(group->canPushSection(dataSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(endOfFileSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedSegmentAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_TRUE(group->canPushSection(extendedSegmentAddressSection2, index));
    ASSERT_EQ(index, 1);
    ASSERT_TRUE(group->canPushSection(startSegmentAddressSection, index));
    ASSERT_EQ(index, 1);
    ASSERT_FALSE(group->canPushSection(extendedLinearAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(startLinearAddressSection, index));
    ASSERT_EQ(index, std::string::npos);

    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 1);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I16HEX);

    ASSERT_FALSE(group->canPushSection(dataSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(endOfFileSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedSegmentAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_TRUE(group->canPushSection(extendedSegmentAddressSection2, index));
    ASSERT_EQ(index, 2);
    ASSERT_FALSE(group->canPushSection(startSegmentAddressSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedLinearAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(startLinearAddressSection, index));
    ASSERT_EQ(index, std::string::npos);

    ASSERT_EQ(group->pushSection(startSegmentAddressSection), std::string::npos);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I16HEX);

    // ADDRESS_I32HEX group
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I32HEX);

    ASSERT_FALSE(group->canPushSection(dataSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_TRUE(group->canPushSection(endOfFileSection, index));
    ASSERT_EQ(index, 1);
    ASSERT_FALSE(group->canPushSection(extendedSegmentAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(startSegmentAddressSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedLinearAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_TRUE(group->canPushSection(extendedLinearAddressSection2, index));
    ASSERT_EQ(index, 1);
    ASSERT_TRUE(group->canPushSection(startLinearAddressSection, index));
    ASSERT_EQ(index, 1);

    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I32HEX);

    ASSERT_FALSE(group->canPushSection(dataSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(endOfFileSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedSegmentAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(startSegmentAddressSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedLinearAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_TRUE(group->canPushSection(extendedLinearAddressSection2, index));
    ASSERT_EQ(index, 1);
    ASSERT_TRUE(group->canPushSection(startLinearAddressSection, index));
    ASSERT_EQ(index, 1);

    ASSERT_EQ(group->pushSection(startLinearAddressSection), 1);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I32HEX);

    ASSERT_FALSE(group->canPushSection(dataSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(endOfFileSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedSegmentAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(startSegmentAddressSection, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_FALSE(group->canPushSection(extendedLinearAddressSection1, index));
    ASSERT_EQ(index, std::string::npos);
    ASSERT_TRUE(group->canPushSection(extendedLinearAddressSection2, index));
    ASSERT_EQ(index, 2);
    ASSERT_FALSE(group->canPushSection(startLinearAddressSection, index));
    ASSERT_EQ(index, std::string::npos);

    ASSERT_EQ(group->pushSection(startLinearAddressSection), std::string::npos);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I32HEX);

    // Test overloaded canPushSection function
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I8HEX);

    ASSERT_TRUE(group->canPushSection(dataSection));
    ASSERT_FALSE(group->canPushSection(endOfFileSection));
    ASSERT_TRUE(group->canPushSection(extendedSegmentAddressSection1));
    ASSERT_TRUE(group->canPushSection(startSegmentAddressSection));
    ASSERT_TRUE(group->canPushSection(extendedLinearAddressSection1));
    ASSERT_TRUE(group->canPushSection(startLinearAddressSection));
}

void HexGroupTest::performPushAndGetSectionTests()
{
    SCOPED_TRACE(__func__);

    const HexGroup& constGroup{*group};

    // ADDRESS_I16HEX
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 1);

    ASSERT_EQ(group->pushSection(endOfFileSection), std::string::npos);
    ASSERT_EQ(group->pushSection(dataSection), std::string::npos);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), std::string::npos);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), std::string::npos);

    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I16HEX);

    ASSERT_THROW(group->getSection(3), std::out_of_range);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(group->getSection(2).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    ASSERT_THROW(constGroup.getSection(3), std::out_of_range);
    ASSERT_EQ(constGroup.getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(constGroup.getSection(1).getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(constGroup.getSection(2).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection2), 2);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), std::string::npos);
    ASSERT_EQ(group->getSection(2).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(group->getSection(3).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    // ADDRESS_I32HEX
    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 1);

    ASSERT_EQ(group->pushSection(startSegmentAddressSection), std::string::npos);
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), std::string::npos);
    ASSERT_EQ(group->pushSection(dataSection), std::string::npos);
    ASSERT_EQ(group->pushSection(endOfFileSection), std::string::npos);

    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I32HEX);

    ASSERT_THROW(group->getSection(3), std::out_of_range);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(group->getSection(2).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    ASSERT_THROW(constGroup.getSection(3), std::out_of_range);
    ASSERT_EQ(constGroup.getSection(0).getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(constGroup.getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(constGroup.getSection(2).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection2), std::string::npos);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), 2);
    ASSERT_EQ(group->getSection(2).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(group->getSection(3).getSectionType(), HexSectionType::SECTION_END_OF_FILE);
}

void HexGroupTest::performRemoveSectionTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), 1);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 2);
    ASSERT_EQ(group->pushSection(endOfFileSection), 3);

    ASSERT_THROW(group->removeSection(5), std::out_of_range);
    ASSERT_EQ(group->getSize(), 4);

    ASSERT_NO_THROW(group->removeSection(3));
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(2).getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);

    ASSERT_NO_THROW(group->removeSection(1));
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getExtendedLinearAddress(), extendedLinearAddressSection1.getExtendedLinearAddress());

    ASSERT_NO_THROW(group->removeSection(0));
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);

    ASSERT_NO_THROW(group->removeSection(0));
    ASSERT_EQ(group->getSize(), 0);
    ASSERT_THROW(group->removeSection(0), std::out_of_range);
}

void HexGroupTest::performGetSetUnusedDataFillValueTests()
{
    SCOPED_TRACE(__func__);

    // Initial section
    const HexRecord record1{":101000005CB6DB55241E0B0A84248F6E4AB90DD7BB"};
    const HexRecord record2{":101020001913F8AB8FF77734D9230C64F629F267DC"};
    HexSection section1{record1};
    ASSERT_TRUE(section1.pushRecord(record2));
    section1.convertToExtendedLinearAddressSection(0x2000);

    ASSERT_EQ(group->getUnusedDataFillValue(), DEFAULT_HEX_RECORD_DATA_VALUE);
    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getUnusedDataFillValue(), unusedDataFillValue1);

    ASSERT_EQ(group->pushSection(section1), 0);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 1);
    ASSERT_EQ(group->pushSection(endOfFileSection), 2);

    for (size_t index{0}; index < (group->getSize() - 1); index++)
        ASSERT_EQ(group->getSection(index).getUnusedDataFillValue(), unusedDataFillValue1);

    ASSERT_EQ(group->getData(0), unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x20001000), 0x5C);
    ASSERT_EQ(group->getData(0x20001010), unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x2000102F), 0x67);
    ASSERT_EQ(group->getData(0x20001030), unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getUnusedDataFillValue(), unusedDataFillValue2);

    for (size_t index{0}; index < (group->getSize() - 1); index++)
        ASSERT_EQ(group->getSection(index).getUnusedDataFillValue(), unusedDataFillValue2);

    ASSERT_EQ(group->getData(0), unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x20001000), 0x5C);
    ASSERT_EQ(group->getData(0x20001010), unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x2000102F), 0x67);
    ASSERT_EQ(group->getData(0x20001030), unusedDataFillValue2);
}

void HexGroupTest::performAddressMapTests()
{
    SCOPED_TRACE(__func__);

    // Empty group
    const auto map1{group->getAddressMap()};
    ASSERT_EQ(map1.size(), 0);

    // SECTION_DATA
    ASSERT_EQ(group->pushSection(dataSection), 0);
    const auto map2{group->getAddressMap()};
    ASSERT_EQ(map2.size(), 1);
    ASSERT_EQ(map2.begin()->first, HexAddress::getMinAbsoluteDataRecordAddress());
    ASSERT_EQ(map2.begin()->second, HexAddress::getSectionMaxDataSize());

    // SECTION_END_OF_FILE
    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    const auto map3{group->getAddressMap()};
    ASSERT_EQ(map3.size(), 1);
    ASSERT_EQ(map3.begin()->first, HexAddress::getMinAbsoluteDataRecordAddress());
    ASSERT_EQ(map3.begin()->second, HexAddress::getSectionMaxDataSize());

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection2), 0);
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 1);
    const auto map4{group->getAddressMap()};
    ASSERT_EQ(map4.size(), 2);
    ASSERT_EQ(map4.begin()->first, extendedSegmentAddressSection1.getMinAbsoluteAddress());
    ASSERT_EQ(map4.begin()->second, HexAddress::getSectionMaxDataSize());
    ASSERT_EQ(std::next(map4.begin(), 1)->first, extendedSegmentAddressSection2.getMinAbsoluteAddress());
    ASSERT_EQ(std::next(map4.begin(), 1)->second, HexAddress::getSectionMaxDataSize());

    // SECTION_START_SEGMENT_ADDRESS
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 2);
    const auto map5{group->getAddressMap()};
    ASSERT_EQ(map5.size(), 2);
    ASSERT_EQ(map5.begin()->first, extendedSegmentAddressSection1.getMinAbsoluteAddress());
    ASSERT_EQ(map5.begin()->second, HexAddress::getSectionMaxDataSize());
    ASSERT_EQ(std::next(map5.begin(), 1)->first, extendedSegmentAddressSection2.getMinAbsoluteAddress());
    ASSERT_EQ(std::next(map5.begin(), 1)->second, HexAddress::getSectionMaxDataSize());

    // SECTION_EXTENDED_LINEAR_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), 1);
    const auto map6{group->getAddressMap()};
    ASSERT_EQ(map6.size(), 2);
    ASSERT_EQ(map6.begin()->first, extendedLinearAddressSection1.getMinAbsoluteAddress());
    ASSERT_EQ(map6.begin()->second, HexAddress::getSectionMaxDataSize());
    ASSERT_EQ(std::next(map6.begin(), 1)->first, extendedLinearAddressSection2.getMinAbsoluteAddress());
    ASSERT_EQ(std::next(map6.begin(), 1)->second, HexAddress::getSectionMaxDataSize());

    // SECTION_START_LINEAR_ADDRESS
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 2);
    const auto map7{group->getAddressMap()};
    ASSERT_EQ(map7.size(), 2);
    ASSERT_EQ(map7.begin()->first, extendedLinearAddressSection1.getMinAbsoluteAddress());
    ASSERT_EQ(map7.begin()->second, HexAddress::getSectionMaxDataSize());
    ASSERT_EQ(std::next(map7.begin(), 1)->first, extendedLinearAddressSection2.getMinAbsoluteAddress());
    ASSERT_EQ(std::next(map7.begin(), 1)->second, HexAddress::getSectionMaxDataSize());
}

void HexGroupTest::performDataMapTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexSection section{};
    ASSERT_TRUE(section.pushRecord(":02000000B36DDE"));
    ASSERT_TRUE(section.pushRecord(":047000003D5AB42A17"));
    ASSERT_TRUE(section.pushRecord(":08FFF800A6450026CA3809994C"));

    // Empty group
    HexAddressMap map{group->getDataMap()};
    ASSERT_EQ(map.size(), 0);

    // ADDRESS_I8HEX
    ASSERT_EQ(group->pushSection(section), 0);
    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    map = group->getDataMap();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(map.cbegin()->first, 0);
    ASSERT_EQ(map.cbegin()->second, 2);
    ASSERT_EQ(std::next(map.cbegin(), 1)->first, 0x7000);
    ASSERT_EQ(std::next(map.cbegin(), 1)->second, 4);
    ASSERT_EQ(std::next(map.cbegin(), 2)->first, 0xFFF8);
    ASSERT_EQ(std::next(map.cbegin(), 2)->second, 8);

    // ADDRESS_I16HEX
    section.convertToExtendedSegmentAddressSection(0x1234);
    group->clearSections();
    ASSERT_EQ(group->pushSection(section), 0);
    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    map = group->getDataMap();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(map.cbegin()->first, 0x12340);
    ASSERT_EQ(map.cbegin()->second, 2);
    ASSERT_EQ(std::next(map.cbegin(), 1)->first, 0x19340);
    ASSERT_EQ(std::next(map.cbegin(), 1)->second, 4);
    ASSERT_EQ(std::next(map.cbegin(), 2)->first, 0x22338);
    ASSERT_EQ(std::next(map.cbegin(), 2)->second, 8);

    // ADDRESS_I32HEX
    section.convertToExtendedLinearAddressSection(0x1234);
    group->clearSections();
    ASSERT_EQ(group->pushSection(section), 0);
    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    map = group->getDataMap();
    ASSERT_EQ(map.size(), 3);
    ASSERT_EQ(map.cbegin()->first, 0x12340000);
    ASSERT_EQ(map.cbegin()->second, 2);
    ASSERT_EQ(std::next(map.cbegin(), 1)->first, 0x12347000);
    ASSERT_EQ(std::next(map.cbegin(), 1)->second, 4);
    ASSERT_EQ(std::next(map.cbegin(), 2)->first, 0x1234FFF8);
    ASSERT_EQ(std::next(map.cbegin(), 2)->second, 8);
}

void HexGroupTest::performFindSectionTests()
{
    SCOPED_TRACE(__func__);

    // Empty group
    ASSERT_EQ(group->findSection(0), std::string::npos);
    ASSERT_EQ(group->findSection(0xFFFF), std::string::npos);
    ASSERT_EQ(group->findSection(0x100000), std::string::npos);
    ASSERT_EQ(group->findSection(0xFFFFFFFF), std::string::npos);

    // SECTION_DATA
    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_EQ(group->findSection(0), 0);
    ASSERT_EQ(group->findSection(0xFFFF), 0);
    ASSERT_EQ(group->findSection(0x10000), std::string::npos);

    // SECTION_END_OF_FILE
    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    ASSERT_EQ(group->findSection(0), 0);
    ASSERT_EQ(group->findSection(0xFFFF), 0);
    ASSERT_EQ(group->findSection(0x10000), std::string::npos);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection2), 0);
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 1);
    ASSERT_EQ(group->findSection(0), std::string::npos);
    ASSERT_EQ(group->findSection(0xFFF), std::string::npos);
    ASSERT_EQ(group->findSection(0x1000), 1);
    ASSERT_EQ(group->findSection(0x10FFF), 1);
    ASSERT_EQ(group->findSection(0x11000), 0);
    ASSERT_EQ(group->findSection(0x20FFF), 0);
    ASSERT_EQ(group->findSection(0x21000), std::string::npos);

    // SECTION_START_SEGMENT_ADDRESS
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 2);
    ASSERT_EQ(group->findSection(0), std::string::npos);
    ASSERT_EQ(group->findSection(0xFFF), std::string::npos);
    ASSERT_EQ(group->findSection(0x1000), 1);
    ASSERT_EQ(group->findSection(0x10FFF), 1);
    ASSERT_EQ(group->findSection(0x11000), 0);
    ASSERT_EQ(group->findSection(0x20FFF), 0);
    ASSERT_EQ(group->findSection(0x21000), std::string::npos);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), 1);
    ASSERT_EQ(group->findSection(0), std::string::npos);
    ASSERT_EQ(group->findSection(0x01FFFFFF), std::string::npos);
    ASSERT_EQ(group->findSection(0x02000000), 0);
    ASSERT_EQ(group->findSection(0x0200FFFF), 0);
    ASSERT_EQ(group->findSection(0x02010000), 1);
    ASSERT_EQ(group->findSection(0x0201FFFF), 1);
    ASSERT_EQ(group->findSection(0x02020000), std::string::npos);

    // SECTION_START_LINEAR_ADDRESS
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 2);
    ASSERT_EQ(group->findSection(0), std::string::npos);
    ASSERT_EQ(group->findSection(0x01FFFFFF), std::string::npos);
    ASSERT_EQ(group->findSection(0x02000000), 0);
    ASSERT_EQ(group->findSection(0x0200FFFF), 0);
    ASSERT_EQ(group->findSection(0x02010000), 1);
    ASSERT_EQ(group->findSection(0x0201FFFF), 1);
    ASSERT_EQ(group->findSection(0x02020000), std::string::npos);
}

void HexGroupTest::performFindPreviousSectionTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexSection section1{}, section2{};

    // Empty group
    ASSERT_EQ(group->findPreviousSection(0), std::string::npos);
    ASSERT_EQ(group->findPreviousSection(0x10000), std::string::npos);
    ASSERT_EQ(group->findPreviousSection(0x100000), std::string::npos);
    ASSERT_EQ(group->findPreviousSection(0xFFFFFFFF), std::string::npos);

    // ADDRESS_I8HEX
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_EQ(group->pushSection(dataSection), 0);

    ASSERT_EQ(group->findPreviousSection(0), std::string::npos);
    ASSERT_EQ(group->findPreviousSection(0xFFFF), std::string::npos);
    ASSERT_EQ(group->findPreviousSection(0x10000), 0);
    ASSERT_EQ(group->findPreviousSection(0xFFFFFFFF), 0);

    // ADDRESS_I16HEX
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0x9000);
    section2.convertToExtendedSegmentAddressSection(0x7000);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_EQ(group->pushSection(section1), 1);
    ASSERT_EQ(group->pushSection(section2), 2);
    ASSERT_EQ(group->pushSection(endOfFileSection), 3);

    ASSERT_EQ(group->findPreviousSection(0), std::string::npos);
    ASSERT_EQ(group->findPreviousSection(0x7FFFF), std::string::npos);
    ASSERT_EQ(group->findPreviousSection(0x80000), 2);
    ASSERT_EQ(group->findPreviousSection(0x9FFFF), 2);
    ASSERT_EQ(group->findPreviousSection(0xA0000), 1);
    ASSERT_EQ(group->findPreviousSection(0xFFFFFFFF), 1);

    // ADDRESS_I32HEX
    group->clearSections();
    section1.convertToExtendedLinearAddressSection(0x7000);
    section2.convertToExtendedLinearAddressSection(0x9000);
    ASSERT_EQ(group->pushSection(section1), 0);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 1);
    ASSERT_EQ(group->pushSection(section2), 2);
    ASSERT_EQ(group->pushSection(endOfFileSection), 3);

    ASSERT_EQ(group->findPreviousSection(0), std::string::npos);
    ASSERT_EQ(group->findPreviousSection(0x7000FFFF), std::string::npos);
    ASSERT_EQ(group->findPreviousSection(0x70010000), 0);
    ASSERT_EQ(group->findPreviousSection(0x9000FFFF), 0);
    ASSERT_EQ(group->findPreviousSection(0x90010000), 2);
    ASSERT_EQ(group->findPreviousSection(0xFFFFFFFF), 2);
}

void HexGroupTest::performFindNextSectionTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexSection section1{}, section2{};

    // Empty group

    ASSERT_EQ(group->findNextSection(0), std::string::npos);
    ASSERT_EQ(group->findNextSection(0x10000), std::string::npos);
    ASSERT_EQ(group->findNextSection(0x100000), std::string::npos);
    ASSERT_EQ(group->findNextSection(0xFFFFFFFF), std::string::npos);

    // ADDRESS_I8HEX
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_EQ(group->pushSection(dataSection), 0);

    ASSERT_EQ(group->findNextSection(0), std::string::npos);
    ASSERT_EQ(group->findNextSection(0xFFFF), std::string::npos);
    ASSERT_EQ(group->findNextSection(0x10000), std::string::npos);
    ASSERT_EQ(group->findNextSection(0xFFFFFFFF), std::string::npos);

    // ADDRESS_I16HEX
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0x9000);
    section2.convertToExtendedSegmentAddressSection(0x7000);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_EQ(group->pushSection(section1), 1);
    ASSERT_EQ(group->pushSection(section2), 2);
    ASSERT_EQ(group->pushSection(endOfFileSection), 3);

    ASSERT_EQ(group->findNextSection(0), 2);
    ASSERT_EQ(group->findNextSection(0x6FFFF), 2);
    ASSERT_EQ(group->findNextSection(0x70000), 1);
    ASSERT_EQ(group->findNextSection(0x8FFFF), 1);
    ASSERT_EQ(group->findNextSection(0x90000), std::string::npos);
    ASSERT_EQ(group->findNextSection(0xFFFFFFFF), std::string::npos);

    // ADDRESS_I32HEX
    group->clearSections();
    section1.convertToExtendedLinearAddressSection(0x7000);
    section2.convertToExtendedLinearAddressSection(0x9000);
    ASSERT_EQ(group->pushSection(section1), 0);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 1);
    ASSERT_EQ(group->pushSection(section2), 2);
    ASSERT_EQ(group->pushSection(endOfFileSection), 3);

    ASSERT_EQ(group->findNextSection(0), 0);
    ASSERT_EQ(group->findNextSection(0x6FFFFFFF), 0);
    ASSERT_EQ(group->findNextSection(0x70000000), 2);
    ASSERT_EQ(group->findNextSection(0x8FFFFFFF), 2);
    ASSERT_EQ(group->findNextSection(0x90000000), std::string::npos);
    ASSERT_EQ(group->findNextSection(0xFFFFFFFF), std::string::npos);
}

void HexGroupTest::performCheckIntersectTests()
{
    SCOPED_TRACE(__func__);

    // checkIntersect(section)

    ASSERT_THROW(group->checkIntersect(endOfFileSection), std::domain_error);

    // ADDRESS_I8HEX
    ASSERT_EQ(group->checkIntersect(dataSection), std::string::npos);
    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_THROW(group->checkIntersect(extendedLinearAddressSection1), std::domain_error);
    ASSERT_EQ(group->checkIntersect(dataSection), 0);

    // ADDRESS_I16HEX
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 1);
    ASSERT_EQ(group->checkIntersect(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->checkIntersect(extendedSegmentAddressSection2), std::string::npos);
    ASSERT_THROW(group->checkIntersect(extendedLinearAddressSection1), std::domain_error);
    ASSERT_THROW(group->checkIntersect(startLinearAddressSection), std::domain_error);

    // ADDRESS_I32HEX
    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), 1);
    ASSERT_EQ(group->checkIntersect(extendedLinearAddressSection1), std::string::npos);
    ASSERT_EQ(group->checkIntersect(extendedLinearAddressSection2), 1);
    ASSERT_THROW(group->checkIntersect(extendedSegmentAddressSection1), std::domain_error);
    ASSERT_THROW(group->checkIntersect(startSegmentAddressSection), std::domain_error);

    // checkIntersect(type, section)

    group->clearSections();
    ASSERT_EQ(group->checkIntersect(HexAddressEnumType::ADDRESS_I8HEX, dataSection), std::string::npos);
    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I16HEX, dataSection), std::domain_error);
    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I32HEX, dataSection), std::domain_error);

    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I8HEX, endOfFileSection), std::domain_error);
    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I16HEX, endOfFileSection), std::domain_error);
    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I32HEX, endOfFileSection), std::domain_error);

    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I8HEX, extendedSegmentAddressSection1), std::domain_error);
    ASSERT_EQ(group->checkIntersect(HexAddressEnumType::ADDRESS_I16HEX, extendedSegmentAddressSection1), std::string::npos);
    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I32HEX, extendedSegmentAddressSection1), std::domain_error);

    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I8HEX, startSegmentAddressSection), std::domain_error);
    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I16HEX, startSegmentAddressSection), std::domain_error);
    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I32HEX, startSegmentAddressSection), std::domain_error);

    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I8HEX, extendedLinearAddressSection1), std::domain_error);
    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I16HEX, extendedLinearAddressSection1), std::domain_error);
    ASSERT_EQ(group->checkIntersect(HexAddressEnumType::ADDRESS_I32HEX, extendedLinearAddressSection1), std::string::npos);

    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I8HEX, startLinearAddressSection), std::domain_error);
    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I16HEX, startLinearAddressSection), std::domain_error);
    ASSERT_THROW(group->checkIntersect(HexAddressEnumType::ADDRESS_I32HEX, startLinearAddressSection), std::domain_error);
}

void HexGroupTest::performCreateSectionTests()
{
    SCOPED_TRACE(__func__);

    // createSection(address)

    // SECTION_DATA
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_EQ(group->createSection(0), 0);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    ASSERT_EQ(group->createSection(0xFFFF), 0);
    ASSERT_EQ(group->getSize(), 2);

    ASSERT_THROW(group->createSection(0x10000), std::out_of_range);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 1);
    ASSERT_EQ(group->pushSection(endOfFileSection), 2);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(0).getExtendedSegmentAddress(), 0x100);

    ASSERT_EQ(group->createSection(0x1000), 0);
    ASSERT_EQ(group->createSection(0x10FFF), 0);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);

    ASSERT_EQ(group->createSection(0x117FF), 2);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0x1100);
    ASSERT_EQ(group->createSection(0x11000), 2);
    ASSERT_EQ(group->createSection(0x20FFF), 2);

    ASSERT_EQ(group->createSection(0xFFF), 3);
    ASSERT_EQ(group->getSize(), 5);
    ASSERT_EQ(group->getSection(3).getExtendedSegmentAddress(), 0xF100);
    ASSERT_EQ(group->createSection(0), 3);

    ASSERT_NO_THROW(group->removeSection(3));
    ASSERT_EQ(group->getSize(), 4);

    ASSERT_EQ(group->createSection(0xF0001), 3);
    ASSERT_EQ(group->getSize(), 5);
    ASSERT_EQ(group->getSection(3).getExtendedSegmentAddress(), 0xF000);

    ASSERT_THROW(group->createSection(0xFFF), std::runtime_error);
    ASSERT_EQ(group->createSection(0xFFFFF), 3);
    ASSERT_THROW(group->createSection(0), std::runtime_error);

    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_EQ(group->pushSection(HexSection::makeExtendedSegmentAddressSection(0xF100)), 1);
    ASSERT_EQ(group->pushSection(endOfFileSection), 2);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(1).getExtendedSegmentAddress(), 0xF100);

    ASSERT_EQ(group->createSection(0xF1000), 1);
    ASSERT_EQ(group->createSection(0xFFFFF), 1);
    ASSERT_EQ(group->createSection(0), 1);
    ASSERT_EQ(group->createSection(0xFFF), 1);
    ASSERT_EQ(group->getSize(), 3);

    ASSERT_EQ(group->createSection(0xF0FFF), 2);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0xE100);
    ASSERT_EQ(group->createSection(0xE1000), 2);
    ASSERT_EQ(group->createSection(0xE1FFF), 2);

    ASSERT_EQ(group->createSection(0x1000), 3);
    ASSERT_EQ(group->getSize(), 5);
    ASSERT_EQ(group->getSection(3).getExtendedSegmentAddress(), 0x100);
    ASSERT_EQ(group->createSection(0x1100), 3);
    ASSERT_EQ(group->createSection(0x10FFF), 3);

    ASSERT_NO_THROW(group->removeSection(3));
    ASSERT_NO_THROW(group->removeSection(1));
    ASSERT_EQ(group->getSize(), 3);

    ASSERT_EQ(group->createSection(1), 2);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0);

    ASSERT_THROW(group->createSection(0xF1000), std::runtime_error);
    ASSERT_THROW(group->createSection(0xF1FFF), std::runtime_error);

    group->clearSections();
    ASSERT_EQ(group->pushSection(HexSection::makeExtendedSegmentAddressSection(0x7000)), 0);

    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getExtendedSegmentAddress(), 0x7000);

    ASSERT_EQ(group->createSection(0x6FFFF), 1);
    ASSERT_EQ(group->createSection(0x80000), 2);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(1).getExtendedSegmentAddress(), 0x6000);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0x8000);

    ASSERT_EQ(group->createSection(0xFFFFF), 3);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(3).getExtendedSegmentAddress(), 0xF000);
    ASSERT_THROW(group->createSection(0x100000), std::out_of_range);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 1);
    ASSERT_EQ(group->createSection(0x02000000), 0);
    ASSERT_EQ(group->createSection(0x0200FFFF), 0);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);

    ASSERT_EQ(group->createSection(0x01FFFFFF), 2);
    ASSERT_EQ(group->createSection(0x01FF0000), 2);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(2).getExtendedLinearAddress(), 0x01FF);

    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), 3);
    ASSERT_EQ(group->createSection(0x0201FFFF), 3);
    ASSERT_EQ(group->createSection(0x02010000), 3);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(3).getExtendedLinearAddress(), 0x0201);

    ASSERT_EQ(group->createSection(0x02020000), 4);
    ASSERT_EQ(group->createSection(0x0202FFFF), 4);
    ASSERT_EQ(group->getSize(), 5);
    ASSERT_EQ(group->getSection(4).getExtendedLinearAddress(), 0x0202);

    ASSERT_EQ(group->createSection(0xFFFFFFFF), 5);
    ASSERT_EQ(group->getSize(), 6);
    ASSERT_EQ(group->getSection(5).getExtendedLinearAddress(), 0xFFFF);

    // createSection(type, address)

    group->clearSections();
    ASSERT_EQ(group->createSection(HexAddressEnumType::ADDRESS_I8HEX, 0), 0);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I16HEX, 0x10000), std::domain_error);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I32HEX, 0x10000000), std::domain_error);

    group->clearSections();
    ASSERT_EQ(group->createSection(HexAddressEnumType::ADDRESS_I16HEX, 0x10000), 0);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I8HEX, 0), std::domain_error);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I32HEX, 0x10000000), std::domain_error);

    group->clearSections();
    ASSERT_EQ(group->createSection(HexAddressEnumType::ADDRESS_I32HEX, 0x10000000), 0);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I8HEX, 0), std::domain_error);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I16HEX, 0x10000), std::domain_error);

    // createSection(address, size)

    // SECTION_DATA
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);

    ASSERT_EQ(group->createSection(0, 1), 0);
    ASSERT_EQ(group->createSection(0, 0x10000), 0);
    ASSERT_EQ(group->createSection(0xFFFF, 1), 0);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);

    ASSERT_THROW(group->createSection(0, 0), std::out_of_range);
    ASSERT_THROW(group->createSection(0xFFFF, 2), std::out_of_range);
    ASSERT_THROW(group->createSection(0x10000, 1), std::out_of_range);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 1);
    ASSERT_EQ(group->pushSection(endOfFileSection), 2);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(0).getExtendedSegmentAddress(), 0x100);

    ASSERT_THROW(group->createSection(0x1000, 0), std::out_of_range);
    ASSERT_EQ(group->createSection(0x1000, 1), 0);
    ASSERT_EQ(group->createSection(0x1000, 0x10000), 0);
    ASSERT_EQ(group->createSection(0x1FFF, 0xF001), 0);
    ASSERT_EQ(group->getSize(), 3);

    ASSERT_EQ(group->createSection(0x11000, 1), 2);
    ASSERT_EQ(group->createSection(0x11000, 0x10000), 2);
    ASSERT_EQ(group->createSection(0x20FFF, 1), 2);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0x1100);

    ASSERT_EQ(group->createSection(0xFFF, 1), 3);
    ASSERT_EQ(group->createSection(0, 0x1000), 3);
    ASSERT_EQ(group->getSize(), 5);
    ASSERT_EQ(group->getSection(3).getExtendedSegmentAddress(), 0xF100);

    ASSERT_NO_THROW(group->removeSection(3));
    ASSERT_EQ(group->getSize(), 4);

    ASSERT_EQ(group->createSection(0xF0000, 1), 3);
    ASSERT_EQ(group->getSize(), 5);
    ASSERT_EQ(group->getSection(3).getExtendedSegmentAddress(), 0xF000);

    ASSERT_THROW(group->createSection(0xFFF, 1), std::runtime_error);
    ASSERT_EQ(group->createSection(0xF0000, 0x10000), 3);
    ASSERT_THROW(group->createSection(0, 1), std::runtime_error);

    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_EQ(group->pushSection(HexSection::makeExtendedSegmentAddressSection(0xF100)), 1);
    ASSERT_EQ(group->pushSection(endOfFileSection), 2);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(1).getExtendedSegmentAddress(), 0xF100);

    ASSERT_EQ(group->createSection(0xF1000, 1), 1);
    ASSERT_EQ(group->createSection(0xF1000, 0xF000), 1);
    ASSERT_EQ(group->createSection(0, 0x1000), 1);
    ASSERT_THROW(group->createSection(0xF1000, 0xF001), std::out_of_range);
    ASSERT_EQ(group->getSize(), 3);

    ASSERT_EQ(group->createSection(0xF0FFF, 1), 2);
    ASSERT_EQ(group->createSection(0xE1000, 0x10000), 2);
    ASSERT_EQ(group->createSection(0xE1FFF, 1), 2);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0xE100);

    ASSERT_EQ(group->createSection(0, 0x1001), 3);
    ASSERT_EQ(group->createSection(0, 0x11000), 3);
    ASSERT_EQ(group->createSection(0x1100, 0xEF00), 3);
    ASSERT_EQ(group->getSize(), 5);
    ASSERT_EQ(group->getSection(3).getExtendedSegmentAddress(), 0x100);

    ASSERT_NO_THROW(group->removeSection(3));
    ASSERT_NO_THROW(group->removeSection(1));
    ASSERT_EQ(group->getSize(), 3);

    ASSERT_EQ(group->createSection(0, 1), 2);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0);

    ASSERT_THROW(group->createSection(0xF1000, 1), std::runtime_error);
    ASSERT_THROW(group->createSection(0xF1FFF, 1), std::runtime_error);

    group->clearSections();
    ASSERT_EQ(group->pushSection(HexSection::makeExtendedSegmentAddressSection(0x7000)), 0);

    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getExtendedSegmentAddress(), 0x7000);

    ASSERT_EQ(group->createSection(0x6FFFF, 0x10002), 2);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(1).getExtendedSegmentAddress(), 0x6000);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0x8000);

    ASSERT_EQ(group->createSection(0xFFFFF, 1), 3);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(3).getExtendedSegmentAddress(), 0xF000);
    ASSERT_THROW(group->createSection(0x100000, 1), std::out_of_range);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 1);
    ASSERT_THROW(group->createSection(0x02000000, 0), std::out_of_range);
    ASSERT_EQ(group->createSection(0x02000000, 1), 0);
    ASSERT_EQ(group->createSection(0x0200FFFF, 1), 0);
    ASSERT_EQ(group->getSize(), 2);

    ASSERT_EQ(group->createSection(0x02000000, 0x1FFFF), 2);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(2).getExtendedLinearAddress(), 0x0201);

    ASSERT_EQ(group->createSection(0x01FF0000, 0x10000), 3);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(3).getExtendedLinearAddress(), 0x01FF);

    ASSERT_EQ(group->createSection(0x01FEFFFF, 0x30002), 5);
    ASSERT_EQ(group->getSize(), 6);
    ASSERT_EQ(group->getSection(4).getExtendedLinearAddress(), 0x01FE);
    ASSERT_EQ(group->getSection(5).getExtendedLinearAddress(), 0x0202);

    ASSERT_EQ(group->createSection(0x01FE0000, 0x4FFFF), 5);
    ASSERT_EQ(group->getSize(), 6);

    ASSERT_EQ(group->createSection(0x01FD0000, 0x6FFFF), 7);
    ASSERT_EQ(group->getSize(), 8);
    ASSERT_EQ(group->getSection(6).getExtendedLinearAddress(), 0x01FD);
    ASSERT_EQ(group->getSection(7).getExtendedLinearAddress(), 0x0203);

    ASSERT_EQ(group->createSection(0xFFFFFFFF, 1), 8);
    ASSERT_EQ(group->getSize(), 9);
    ASSERT_EQ(group->getSection(8).getExtendedLinearAddress(), 0xFFFF);

    ASSERT_THROW(group->createSection(0xFFFFFFFF, 2), std::out_of_range);

    // createSection(type, address, size)

    group->clearSections();
    ASSERT_EQ(group->createSection(HexAddressEnumType::ADDRESS_I8HEX, 0, 1), 0);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I16HEX, 0x10000, 1), std::domain_error);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I32HEX, 0x10000000, 1), std::domain_error);

    group->clearSections();
    ASSERT_EQ(group->createSection(HexAddressEnumType::ADDRESS_I16HEX, 0x10000, 1), 0);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I8HEX, 0, 1), std::domain_error);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I32HEX, 0x10000000, 1), std::domain_error);

    group->clearSections();
    ASSERT_EQ(group->createSection(HexAddressEnumType::ADDRESS_I32HEX, 0x10000000, 1), 0);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I8HEX, 0, 1), std::domain_error);
    ASSERT_THROW(group->createSection(HexAddressEnumType::ADDRESS_I16HEX, 0x10000, 1), std::domain_error);
}

void HexGroupTest::performClearDataTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexAddressType base;
    HexSection section1;
    ASSERT_TRUE(section1.pushRecord(":100000000440D5E9D2869DDEACCB9BBEF3206ADBF3"));
    ASSERT_TRUE(section1.pushRecord(":1010000045747F698EF5C4B738EF71F9BB56C6F8E1"));
    ASSERT_TRUE(section1.pushRecord(":107FF80072D42A610F477D265BE4A3254EAFF59521"));
    ASSERT_TRUE(section1.pushRecord(":10FFF000C96F3B75D0BAF25462C0EDE0301B678D1B"));

    // clearData()

    // ADDRESS_I8HEX
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_EQ(group->getSize(), 2);

    group->clearData();
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    // ADDRESS_I16HEX
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection2), 1);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 2);
    ASSERT_EQ(group->getSize(), 4);

    group->clearData();
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    // ADDRESS_I32HEX
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), 1);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 2);
    ASSERT_EQ(group->getSize(), 4);

    group->clearData();
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    // clearData(address)

    // Empty group
    group->setUnusedDataFillValue(unusedDataFillValue1);
    group->clearSections();

    base = 0x1000;
    ASSERT_EQ(group->getData(base), unusedDataFillValue1);
    group->clearData();
    ASSERT_EQ(group->getData(base), unusedDataFillValue1);

    // ADDRESS_I8HEX
    group->setUnusedDataFillValue(unusedDataFillValue2);
    group->clearSections();
    ASSERT_EQ(group->pushSection(section1), 0);
    ASSERT_EQ(group->pushSection(endOfFileSection), 1);

    base = 0;
    group->clearData(base);
    group->clearData(base + 0x1);
    group->clearData(base + 0x3);

    ASSERT_EQ(group->getData(base), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x1), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x2), 0xD5);
    ASSERT_EQ(group->getData(base + 0x3), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x4), 0xD2);

    base = 0x1008;
    group->clearData(base + 0x1);
    group->clearData(base + 0x2);
    group->clearData(base + 0x3);

    ASSERT_EQ(group->getData(base), 0x38);
    ASSERT_EQ(group->getData(base + 0x1), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x2), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x3), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x4), 0xBB);

    base = 0xFFF8;
    group->clearData(base);
    group->clearData(base + 0x2);
    group->clearData(base + 0x4);

    ASSERT_EQ(group->getData(base), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x1), 0xC0);
    ASSERT_EQ(group->getData(base + 0x2), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x3), 0xE0);
    ASSERT_EQ(group->getData(base + 0x4), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x5), 0x1B);

    base = 0xFFFF;
    ASSERT_NO_THROW(group->setData(base, unusedDataFillValue1));
    ASSERT_EQ(group->getData(base), unusedDataFillValue1);
    ASSERT_NO_THROW(group->clearData(base));
    ASSERT_EQ(group->getData(base), unusedDataFillValue2);

    ASSERT_THROW(group->clearData(0x10000), std::out_of_range);

    // ADDRESS_I16HEX
    group->setUnusedDataFillValue(unusedDataFillValue1);
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0x8000);
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection2), 1);
    ASSERT_EQ(group->pushSection(section1), 2);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 3);
    ASSERT_EQ(group->pushSection(endOfFileSection), 4);

    base = 0x80000;
    group->clearData(base);
    group->clearData(base + 0x1);
    group->clearData(base + 0x3);

    ASSERT_EQ(group->getData(base), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x2), 0xD5);
    ASSERT_EQ(group->getData(base + 0x3), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x4), 0xD2);

    base = 0x81008;
    group->clearData(base + 0x1);
    group->clearData(base + 0x2);
    group->clearData(base + 0x3);

    ASSERT_EQ(group->getData(base), 0x38);
    ASSERT_EQ(group->getData(base + 0x1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x2), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x3), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x4), 0xBB);

    base = 0x8FFF8;
    group->clearData(base);
    group->clearData(base + 0x2);
    group->clearData(base + 0x4);

    ASSERT_EQ(group->getData(base), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x1), 0xC0);
    ASSERT_EQ(group->getData(base + 0x2), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x3), 0xE0);
    ASSERT_EQ(group->getData(base + 0x4), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x5), 0x1B);

    base = 0xFFFFF;
    ASSERT_NO_THROW(group->setData(base, unusedDataFillValue2));
    ASSERT_EQ(group->getData(base), unusedDataFillValue2);
    ASSERT_NO_THROW(group->clearData(base));
    ASSERT_EQ(group->getData(base), unusedDataFillValue1);

    ASSERT_THROW(group->clearData(0x100000), std::out_of_range);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0xF800);
    ASSERT_EQ(group->pushSection(section1), 0);
    section1.convertToExtendedSegmentAddressSection(0x800);
    ASSERT_EQ(group->pushSection(section1), 1);

    base = 0xFFFFE;
    ASSERT_EQ(group->getData(base), 0x7D);
    group->clearData(base);
    ASSERT_EQ(group->getData(base), unusedDataFillValue1);

    ASSERT_EQ(group->getData(base + 0x1), 0x26);
    group->clearData(base + 0x1);
    ASSERT_EQ(group->getData(base + 0x1), unusedDataFillValue1);

    base = 0;
    ASSERT_EQ(group->getData(base), 0x5B);
    group->clearData(base);
    ASSERT_EQ(group->getData(base), unusedDataFillValue1);

    ASSERT_EQ(group->getData(base + 0x1), 0xE4);
    group->clearData(base + 0x1);
    ASSERT_EQ(group->getData(base + 0x1), unusedDataFillValue1);

    // ADDRESS_I32HEX
    group->setUnusedDataFillValue(unusedDataFillValue2);
    group->clearSections();
    section1.convertToExtendedLinearAddressSection(0x8000);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 1);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), 2);
    ASSERT_EQ(group->pushSection(section1), 3);
    ASSERT_EQ(group->pushSection(endOfFileSection), 4);

    base = 0x80000000;
    group->clearData(base);
    group->clearData(base + 0x1);
    group->clearData(base + 0x3);

    ASSERT_EQ(group->getData(base), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x1), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x2), 0xD5);
    ASSERT_EQ(group->getData(base + 0x3), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x4), 0xD2);

    base = 0x80001008;
    group->clearData(base + 0x1);
    group->clearData(base + 0x2);
    group->clearData(base + 0x3);

    ASSERT_EQ(group->getData(base), 0x38);
    ASSERT_EQ(group->getData(base + 0x1), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x2), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x3), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x4), 0xBB);

    base = 0x8000FFF8;
    group->clearData(base);
    group->clearData(base + 0x2);
    group->clearData(base + 0x4);

    ASSERT_EQ(group->getData(base), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x1), 0xC0);
    ASSERT_EQ(group->getData(base + 0x2), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x3), 0xE0);
    ASSERT_EQ(group->getData(base + 0x4), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x5), 0x1B);

    base = 0xFFFFFFFF;
    ASSERT_NO_THROW(group->setData(base, unusedDataFillValue1));
    ASSERT_EQ(group->getData(base), unusedDataFillValue1);
    ASSERT_NO_THROW(group->clearData(base));
    ASSERT_EQ(group->getData(base), unusedDataFillValue2);

    // clearData(address, size)

    // Empty group
    group->clearSections();
    ASSERT_THROW(group->clearData(0, 0), std::out_of_range);
    ASSERT_EQ(group->clearData(0, 1), 1);
    ASSERT_EQ(group->clearData(0, 0xFFFF), 0xFFFF);

    // ADDRESS_I8HEX
    group->setUnusedDataFillValue(unusedDataFillValue1);
    group->clearSections();
    section1.convertToDataSection();
    ASSERT_EQ(group->pushSection(section1), 0);
    ASSERT_EQ(group->pushSection(endOfFileSection), 1);

    base = 0;
    ASSERT_EQ(group->clearData(base + 0x4, 8), 8);
    ASSERT_EQ(group->getData(base + 0x3), 0xE9);
    ASSERT_EQ(group->getData(base + 0x4), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0xB), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0xC), 0xF3);

    base = 0xFF8;
    ASSERT_EQ(group->clearData(base, 32), 32);
    ASSERT_EQ(group->getData(base), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x8), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x17), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x1F), unusedDataFillValue1);

    base = 0xFFF0;
    ASSERT_EQ(group->clearData(base + 0x8, 8), 8);
    ASSERT_EQ(group->getData(base + 0x7), 0x54);
    ASSERT_EQ(group->getData(base + 0x8), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0xF), unusedDataFillValue1);
    ASSERT_THROW(group->getData(base + 0x10), std::out_of_range);

    base = 0xFFFF;
    ASSERT_THROW(group->clearData(base + 0x1, 1), std::out_of_range);
    ASSERT_THROW(group->clearData(base + 0x1, 2), std::out_of_range);
    ASSERT_THROW(group->clearData(base, 2), std::out_of_range);

    // ADDRESS_I16HEX
    group->setUnusedDataFillValue(unusedDataFillValue2);
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0x8000);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_EQ(group->pushSection(section1), 1);
    ASSERT_EQ(group->pushSection(endOfFileSection), 2);

    base = 0x7FFFC;
    ASSERT_EQ(group->clearData(base, 6), 6);
    ASSERT_EQ(group->getData(base), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x5), unusedDataFillValue2);

    base = 0x80000;
    ASSERT_EQ(group->clearData(base + 0x4, 8), 8);
    ASSERT_EQ(group->getData(base + 0x3), 0xE9);
    ASSERT_EQ(group->getData(base + 0x4), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0xB), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0xC), 0xF3);

    base = 0x80FF8;
    ASSERT_EQ(group->clearData(base, 32), 32);
    ASSERT_EQ(group->getData(base), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x8), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x17), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x1F), unusedDataFillValue2);

    base = 0x8FFF0;
    ASSERT_EQ(group->clearData(base + 0x8, 8), 8);
    ASSERT_EQ(group->getData(base + 0x7), 0x54);
    ASSERT_EQ(group->getData(base + 0x8), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0xF), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x10), unusedDataFillValue2);

    base = 0xFFFFF;
    ASSERT_THROW(group->clearData(base + 0x1, 1), std::out_of_range);
    ASSERT_THROW(group->clearData(base + 0x1, 2), std::out_of_range);
    ASSERT_THROW(group->clearData(base, 2), std::out_of_range);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0xF800);
    ASSERT_EQ(group->pushSection(section1), 0);
    section1.convertToExtendedSegmentAddressSection(0x800);
    ASSERT_EQ(group->pushSection(section1), 1);

    ASSERT_EQ(group->getData(0xFFFFE), 0x7D);
    ASSERT_EQ(group->getData(0xFFFFF), 0x26);
    group->clearData(0xFFFFE, 2);
    ASSERT_EQ(group->getData(0xFFFFE), unusedDataFillValue1);
    ASSERT_EQ(group->getData(0xFFFFF), unusedDataFillValue1);

    ASSERT_EQ(group->getData(0), 0x5B);
    ASSERT_EQ(group->getData(0x1), 0xE4);
    group->clearData(0, 2);
    ASSERT_EQ(group->getData(0), unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x1), unusedDataFillValue1);

    // ADDRESS_I32HEX
    group->setUnusedDataFillValue(unusedDataFillValue2);
    group->clearSections();
    section1.convertToExtendedLinearAddressSection(0x8000);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_EQ(group->pushSection(section1), 1);
    ASSERT_EQ(group->pushSection(endOfFileSection), 2);

    base = 0x7FFFFFFC;
    ASSERT_EQ(group->clearData(base, 6), 6);
    ASSERT_EQ(group->getData(base), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x5), unusedDataFillValue2);

    base = 0x80000000;
    ASSERT_EQ(group->clearData(base + 0x4, 8), 8);
    ASSERT_EQ(group->getData(base + 0x3), 0xE9);
    ASSERT_EQ(group->getData(base + 0x4), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0xB), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0xC), 0xF3);

    base = 0x80000FF8;
    ASSERT_EQ(group->clearData(base, 32), 32);
    ASSERT_EQ(group->getData(base), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x8), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x17), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x1F), unusedDataFillValue2);

    base = 0x8000FFF0;
    ASSERT_EQ(group->clearData(base + 0x8, 8), 8);
    ASSERT_EQ(group->getData(base + 0x7), 0x54);
    ASSERT_EQ(group->getData(base + 0x8), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0xF), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x10), unusedDataFillValue2);
}

void HexGroupTest::performGetDataTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexSection section1;
    ASSERT_TRUE(section1.pushRecord(":101000007CF2C28D1B420B4E950E2C63AB6858C30D"));
    ASSERT_TRUE(section1.pushRecord(":101010004531F2E18042AD54A5F9D27A25F4A1A27E"));
    ASSERT_TRUE(section1.pushRecord(":101030000E6B8A6DE0829D1F89F531F0E46AC84429"));
    HexAddressType base;

    // Group references
    auto& groupRef{*group};
    const auto& groupConstRef{*group};

    // SECTION_DATA
    group->setUnusedDataFillValue(unusedDataFillValue1);
    group->clearSections();
    ASSERT_EQ(group->pushSection(section1), 0);
    ASSERT_EQ(group->pushSection(endOfFileSection), 1);

    base = 0x1000;
    ASSERT_THROW(groupRef[base - 1], std::out_of_range);
    ASSERT_EQ(groupRef[base], 0x7C);
    ASSERT_EQ(groupRef[base + 0x1F], 0xA2);
    ASSERT_THROW(groupRef[base + 0x20], std::out_of_range);
    ASSERT_THROW(groupRef[base + 0x2F], std::out_of_range);
    ASSERT_EQ(groupRef[base + 0x30], 0x0E);
    ASSERT_EQ(groupRef[base + 0x3F], 0x44);
    ASSERT_THROW(groupRef[base + 0x40], std::out_of_range);
    ASSERT_THROW(groupRef[0x10000], std::out_of_range);

    ASSERT_THROW(groupConstRef[base - 1], std::out_of_range);
    ASSERT_EQ(groupConstRef[base], 0x7C);
    ASSERT_EQ(groupConstRef[base + 0x1F], 0xA2);
    ASSERT_THROW(groupConstRef[base + 0x20], std::out_of_range);
    ASSERT_THROW(groupConstRef[base + 0x2F], std::out_of_range);
    ASSERT_EQ(groupConstRef[base + 0x30], 0x0E);
    ASSERT_EQ(groupConstRef[base + 0x3F], 0x44);
    ASSERT_THROW(groupConstRef[base + 0x40], std::out_of_range);
    ASSERT_THROW(groupConstRef[0x10000], std::out_of_range);

    ASSERT_EQ(group->getData(base - 1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base), 0x7C);
    ASSERT_EQ(group->getData(base + 0x1F), 0xA2);
    ASSERT_EQ(group->getData(base + 0x20), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x2F), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x30), 0x0E);
    ASSERT_EQ(group->getData(base + 0x3F), 0x44);
    ASSERT_EQ(group->getData(base + 0x40), unusedDataFillValue1);
    ASSERT_THROW(group->getData(0x10000), std::out_of_range);

    // SECTION_END_OF_FILE
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);

    ASSERT_THROW(groupRef[0], std::out_of_range);
    ASSERT_THROW(groupConstRef[0], std::out_of_range);
    ASSERT_EQ(group->getData(0), unusedDataFillValue1);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    group->setUnusedDataFillValue(unusedDataFillValue2);
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0xA9B0);
    ASSERT_EQ(group->pushSection(section1), 0);

    base = 0xAAB00;
    ASSERT_THROW(groupRef[base - 0x1000], std::out_of_range);
    ASSERT_THROW(groupRef[base - 1], std::out_of_range);
    ASSERT_EQ(groupRef[base], 0x7C);
    ASSERT_EQ(groupRef[base + 0x1F], 0xA2);
    ASSERT_THROW(groupRef[base + 0x20], std::out_of_range);
    ASSERT_THROW(groupRef[base + 0x2F], std::out_of_range);
    ASSERT_EQ(groupRef[base + 0x30], 0x0E);
    ASSERT_EQ(groupRef[base + 0x3F], 0x44);
    ASSERT_THROW(groupRef[base + 0x40], std::out_of_range);
    ASSERT_THROW(groupRef[base + 0x1000], std::out_of_range);
    ASSERT_THROW(groupRef[0x100000], std::out_of_range);

    ASSERT_THROW(groupConstRef[base - 0x1000], std::out_of_range);
    ASSERT_THROW(groupConstRef[base - 1], std::out_of_range);
    ASSERT_EQ(groupConstRef[base], 0x7C);
    ASSERT_EQ(groupConstRef[base + 0x1F], 0xA2);
    ASSERT_THROW(groupConstRef[base + 0x20], std::out_of_range);
    ASSERT_THROW(groupConstRef[base + 0x2F], std::out_of_range);
    ASSERT_EQ(groupConstRef[base + 0x30], 0x0E);
    ASSERT_EQ(groupConstRef[base + 0x3F], 0x44);
    ASSERT_THROW(groupConstRef[base + 0x40], std::out_of_range);
    ASSERT_THROW(groupConstRef[base + 0x1000], std::out_of_range);
    ASSERT_THROW(groupConstRef[0x100000], std::out_of_range);

    ASSERT_EQ(group->getData(base - 0x1000), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base - 1), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base), 0x7C);
    ASSERT_EQ(group->getData(base + 0x1F), 0xA2);
    ASSERT_EQ(group->getData(base + 0x20), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x2F), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x30), 0x0E);
    ASSERT_EQ(group->getData(base + 0x3F), 0x44);
    ASSERT_EQ(group->getData(base + 0x40), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base + 0x1000), unusedDataFillValue2);
    ASSERT_THROW(group->getData(0x100000), std::out_of_range);

    // SECTION_START_SEGMENT_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);

    ASSERT_THROW(groupRef[0], std::out_of_range);
    ASSERT_THROW(groupConstRef[0], std::out_of_range);
    ASSERT_EQ(group->getData(0), unusedDataFillValue2);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    group->setUnusedDataFillValue(unusedDataFillValue1);
    group->clearSections();
    section1.convertToExtendedLinearAddressSection(0xC1D2);
    ASSERT_EQ(group->pushSection(section1), 0);

    base = 0xC1D21000;
    ASSERT_THROW(groupRef[base - 0x10000], std::out_of_range);
    ASSERT_THROW(groupRef[base - 1], std::out_of_range);
    ASSERT_EQ(groupRef[base], 0x7C);
    ASSERT_EQ(groupRef[base + 0x1F], 0xA2);
    ASSERT_THROW(groupRef[base + 0x20], std::out_of_range);
    ASSERT_THROW(groupRef[base + 0x2F], std::out_of_range);
    ASSERT_EQ(groupRef[base + 0x30], 0x0E);
    ASSERT_EQ(groupRef[base + 0x3F], 0x44);
    ASSERT_THROW(groupRef[base + 0x40], std::out_of_range);
    ASSERT_THROW(groupRef[base + 0x10000], std::out_of_range);

    ASSERT_THROW(groupConstRef[base - 0x10000], std::out_of_range);
    ASSERT_THROW(groupConstRef[base - 1], std::out_of_range);
    ASSERT_EQ(groupConstRef[base], 0x7C);
    ASSERT_EQ(groupConstRef[base + 0x1F], 0xA2);
    ASSERT_THROW(groupConstRef[base + 0x20], std::out_of_range);
    ASSERT_THROW(groupConstRef[base + 0x2F], std::out_of_range);
    ASSERT_EQ(groupConstRef[base + 0x30], 0x0E);
    ASSERT_EQ(groupConstRef[base + 0x3F], 0x44);
    ASSERT_THROW(groupConstRef[base + 0x40], std::out_of_range);
    ASSERT_THROW(groupConstRef[base + 0x10000], std::out_of_range);
    ASSERT_THROW(groupConstRef[0x100000], std::out_of_range);

    ASSERT_EQ(group->getData(base - 0x10000), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base - 1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base), 0x7C);
    ASSERT_EQ(group->getData(base + 0x1F), 0xA2);
    ASSERT_EQ(group->getData(base + 0x20), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x2F), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x30), 0x0E);
    ASSERT_EQ(group->getData(base + 0x3F), 0x44);
    ASSERT_EQ(group->getData(base + 0x40), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base + 0x10000), unusedDataFillValue1);

    // SECTION_START_LINEAR_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);

    ASSERT_THROW(groupRef[0], std::out_of_range);
    ASSERT_THROW(groupConstRef[0], std::out_of_range);
    ASSERT_EQ(group->getData(0), unusedDataFillValue1);
}

void HexGroupTest::performGetDataArrayTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexSection section1;
    ASSERT_TRUE(section1.pushRecord(":100000007FFE7E55335FD3E68F00AEF3C02A31C743"));
    ASSERT_TRUE(section1.pushRecord(":10100000CF18BCC27AA7F81B9B666E583E8B844FE4"));
    ASSERT_TRUE(section1.pushRecord(":10101000649EEC8644DA2129C2FAFAFA6C9E0188B1"));
    ASSERT_TRUE(section1.pushRecord(":101030009F668D49BE83A0C80F8EF0C0BB3758D3C2"));
    ASSERT_TRUE(section1.pushRecord(":107FF8000A63524DAC79D408112074AADB912B2660"));
    ASSERT_TRUE(section1.pushRecord(":10FFD0002CD224518491BC6A62F8D360B1E2201E15"));
    ASSERT_TRUE(section1.pushRecord(":10FFF00094A9BCE3B585DA625DFEBCFB6889268006"));

    HexDataArray<MAXIMUM_HEX_RECORD_DATA_SIZE> data{};

    // SECTION_DATA
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_EQ(group->pushSection(section1), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0x7F);
    ASSERT_EQ(data[0xF], 0xC7);
    ASSERT_EQ(data[0x10], unusedDataFillValue1);
    ASSERT_EQ(data[0x1F], unusedDataFillValue1);

    ASSERT_EQ(group->getData(0x1000, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0xCF);
    ASSERT_EQ(data[0xF], 0x4F);
    ASSERT_EQ(data[0x10], 0x64);
    ASSERT_EQ(data[0x1F], 0x88);

    ASSERT_EQ(group->getData(0x1002, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0xBC);
    ASSERT_EQ(data[0xD], 0x4F);
    ASSERT_EQ(data[0xE], 0x64);
    ASSERT_EQ(data[0x1D], 0x88);
    ASSERT_EQ(data[0x1E], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0xFFE, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0x1], unusedDataFillValue2);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x1F], 0x9E);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0xFFE, &data[0], 0x23), 0x23);
    ASSERT_EQ(data[0x1], unusedDataFillValue1);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x21], 0x88);
    ASSERT_EQ(data[0x22], unusedDataFillValue1);

    ASSERT_THROW(group->getData(0xFFFF, &data[0], 2), std::out_of_range);
    ASSERT_THROW(group->getData(0x10000, &data[0], 1), std::out_of_range);

    // SECTION_END_OF_FILE
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0, &data[0], 0x20), 0);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0x1000);
    ASSERT_EQ(group->pushSection(section1), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10000, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0x7F);
    ASSERT_EQ(data[0xF], 0xC7);
    ASSERT_EQ(data[0x10], unusedDataFillValue1);
    ASSERT_EQ(data[0x1F], unusedDataFillValue1);

    ASSERT_EQ(group->getData(0x11000, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0xCF);
    ASSERT_EQ(data[0xF], 0x4F);
    ASSERT_EQ(data[0x10], 0x64);
    ASSERT_EQ(data[0x1F], 0x88);

    ASSERT_EQ(group->getData(0x11002, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0xBC);
    ASSERT_EQ(data[0xD], 0x4F);
    ASSERT_EQ(data[0xE], 0x64);
    ASSERT_EQ(data[0x1D], 0x88);
    ASSERT_EQ(data[0x1E], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x10FFE, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0x1], unusedDataFillValue2);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x1F], 0x9E);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10FFE, &data[0], 0x23), 0x23);
    ASSERT_EQ(data[0x1], unusedDataFillValue1);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x21], 0x88);
    ASSERT_EQ(data[0x22], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x1FFEE, &data[0], 0x14), 0x12);
    ASSERT_EQ(data[0x1], unusedDataFillValue2);
    ASSERT_EQ(data[0x2], 0x94);
    ASSERT_EQ(data[0x11], 0x80);
    ASSERT_EQ(data[0x12], unusedDataFillValue2);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x1000, &data[0], 0x20), 0);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x21000, &data[0], 0x20), 0);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue2);

    ASSERT_THROW(group->getData(0xFFFFF, &data[0], 2), std::out_of_range);
    ASSERT_THROW(group->getData(0x100000, &data[0], 1), std::out_of_range);

    section1.convertToExtendedSegmentAddressSection(0x2000);
    ASSERT_EQ(group->pushSection(section1), 1);

    ASSERT_EQ(group->getData(0x1FFF0, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0x94);
    ASSERT_EQ(data[0xF], 0x80);
    ASSERT_EQ(data[0x10], 0x7F);
    ASSERT_EQ(data[0x1F], 0xC7);

    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0xF800);
    ASSERT_EQ(group->pushSection(section1), 0);
    section1.convertToExtendedSegmentAddressSection(0x800);
    ASSERT_EQ(group->pushSection(section1), 1);

    ASSERT_EQ(group->getData(0xFFFF8, &data[0], 0x8), 0x8);
    ASSERT_EQ(data[0], 0x0A);
    ASSERT_EQ(data[0x7], 0x08);

    ASSERT_EQ(group->getData(0, &data[0], 0x8), 0x8);
    ASSERT_EQ(data[0], 0x11);
    ASSERT_EQ(data[0x7], 0x26);

    // SECTION_START_SEGMENT_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10000, &data[0], 0x20), 0);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    group->clearSections();
    section1.convertToExtendedLinearAddressSection(0x1000);
    ASSERT_EQ(group->pushSection(section1), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10000000, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0x7F);
    ASSERT_EQ(data[0xF], 0xC7);
    ASSERT_EQ(data[0x10], unusedDataFillValue1);
    ASSERT_EQ(data[0x1F], unusedDataFillValue1);

    ASSERT_EQ(group->getData(0x10001000, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0xCF);
    ASSERT_EQ(data[0xF], 0x4F);
    ASSERT_EQ(data[0x10], 0x64);
    ASSERT_EQ(data[0x1F], 0x88);

    ASSERT_EQ(group->getData(0x10001002, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0xBC);
    ASSERT_EQ(data[0xD], 0x4F);
    ASSERT_EQ(data[0xE], 0x64);
    ASSERT_EQ(data[0x1D], 0x88);
    ASSERT_EQ(data[0x1E], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x10000FFE, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0x1], unusedDataFillValue2);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x1F], 0x9E);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10000FFE, &data[0], 0x23), 0x23);
    ASSERT_EQ(data[0x1], unusedDataFillValue1);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x21], 0x88);
    ASSERT_EQ(data[0x22], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x1000FFEE, &data[0], 0x14), 0x12);
    ASSERT_EQ(data[0x1], unusedDataFillValue2);
    ASSERT_EQ(data[0x2], 0x94);
    ASSERT_EQ(data[0x11], 0x80);
    ASSERT_EQ(data[0x12], unusedDataFillValue2);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x20001000, &data[0], 0x20), 0);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x20001000, &data[0], 0x20), 0);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue2);

    section1.convertToExtendedLinearAddressSection(0x1001);
    ASSERT_EQ(group->pushSection(section1), 1);

    ASSERT_EQ(group->getData(0x1000FFF0, &data[0], 0x20), 0x20);
    ASSERT_EQ(data[0], 0x94);
    ASSERT_EQ(data[0xF], 0x80);
    ASSERT_EQ(data[0x10], 0x7F);
    ASSERT_EQ(data[0x1F], 0xC7);

    // SECTION_START_LINEAR_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10000000, &data[0], 0x20), 0);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);
}

void HexGroupTest::performGetDataVectorTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexSection section1;
    ASSERT_TRUE(section1.pushRecord(":100000007FFE7E55335FD3E68F00AEF3C02A31C743"));
    ASSERT_TRUE(section1.pushRecord(":10100000CF18BCC27AA7F81B9B666E583E8B844FE4"));
    ASSERT_TRUE(section1.pushRecord(":10101000649EEC8644DA2129C2FAFAFA6C9E0188B1"));
    ASSERT_TRUE(section1.pushRecord(":101030009F668D49BE83A0C80F8EF0C0BB3758D3C2"));
    ASSERT_TRUE(section1.pushRecord(":107FF8000A63524DAC79D408112074AADB912B2660"));
    ASSERT_TRUE(section1.pushRecord(":10FFD0002CD224518491BC6A62F8D360B1E2201E15"));
    ASSERT_TRUE(section1.pushRecord(":10FFF00094A9BCE3B585DA625DFEBCFB6889268006"));

    HexDataVector data{};
    data.reserve(MAXIMUM_HEX_RECORD_DATA_SIZE);
    HexDataSizeType offset{0};

    // SECTION_DATA
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_EQ(group->pushSection(section1), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0x7F);
    ASSERT_EQ(data[0xF], 0xC7);
    ASSERT_EQ(data[0x10], unusedDataFillValue1);
    ASSERT_EQ(data[0x1F], unusedDataFillValue1);

    ASSERT_EQ(group->getData(0x1000, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0xCF);
    ASSERT_EQ(data[0xF], 0x4F);
    ASSERT_EQ(data[0x10], 0x64);
    ASSERT_EQ(data[0x1F], 0x88);

    ASSERT_EQ(group->getData(0x1002, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0xBC);
    ASSERT_EQ(data[0xD], 0x4F);
    ASSERT_EQ(data[0xE], 0x64);
    ASSERT_EQ(data[0x1D], 0x88);
    ASSERT_EQ(data[0x1E], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0xFFE, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0x1], unusedDataFillValue2);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x1F], 0x9E);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0xFFE, data, 0x23), 0x23);
    ASSERT_EQ(data.size(), 0x23);
    ASSERT_EQ(data[0x1], unusedDataFillValue1);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x21], 0x88);
    ASSERT_EQ(data[0x22], unusedDataFillValue1);

    offset = 8;
    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0xFFE, data, 0x23, offset), 0x23);
    ASSERT_EQ(data.size(), offset + 0x23);
    ASSERT_EQ(data[offset + 0x1], unusedDataFillValue2);
    ASSERT_EQ(data[offset + 0x2], 0xCF);
    ASSERT_EQ(data[offset + 0x11], 0x4F);
    ASSERT_EQ(data[offset + 0x12], 0x64);
    ASSERT_EQ(data[offset + 0x21], 0x88);
    ASSERT_EQ(data[offset + 0x22], unusedDataFillValue2);

    ASSERT_THROW(group->getData(0xFFFF, data, 2), std::out_of_range);
    ASSERT_THROW(group->getData(0x10000, data, 1), std::out_of_range);

    data.clear();
    data.resize(0x8, unusedDataFillValue2);
    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_THROW(group->getData(0, data, 32, 0x9), std::underflow_error);
    ASSERT_EQ(data.size(), 0x8);
    for (HexDataSizeType index{0}; index < 0x8; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue2);

    // SECTION_END_OF_FILE
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0, data, 0x20), 0);
    ASSERT_EQ(data.size(), 0x20);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0x1000);
    ASSERT_EQ(group->pushSection(section1), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10000, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0x7F);
    ASSERT_EQ(data[0xF], 0xC7);
    ASSERT_EQ(data[0x10], unusedDataFillValue1);
    ASSERT_EQ(data[0x1F], unusedDataFillValue1);

    ASSERT_EQ(group->getData(0x11000, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0xCF);
    ASSERT_EQ(data[0xF], 0x4F);
    ASSERT_EQ(data[0x10], 0x64);
    ASSERT_EQ(data[0x1F], 0x88);

    ASSERT_EQ(group->getData(0x11002, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0xBC);
    ASSERT_EQ(data[0xD], 0x4F);
    ASSERT_EQ(data[0xE], 0x64);
    ASSERT_EQ(data[0x1D], 0x88);
    ASSERT_EQ(data[0x1E], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x10FFE, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0x1], unusedDataFillValue2);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x1F], 0x9E);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10FFE, data, 0x23), 0x23);
    ASSERT_EQ(data.size(), 0x23);
    ASSERT_EQ(data[0x1], unusedDataFillValue1);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x21], 0x88);
    ASSERT_EQ(data[0x22], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    data.clear();
    ASSERT_EQ(group->getData(0x1FFEE, data, 0x14), 0x12);
    ASSERT_EQ(data.size(), 0x14);
    ASSERT_EQ(data[0x1], unusedDataFillValue2);
    ASSERT_EQ(data[0x2], 0x94);
    ASSERT_EQ(data[0x11], 0x80);
    ASSERT_EQ(data[0x12], unusedDataFillValue2);

    offset = 8;
    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x1FFEE, data, 0x14, offset), 0x12);
    ASSERT_EQ(data.size(), 0x1C);
    ASSERT_EQ(data[offset + 0x1], unusedDataFillValue1);
    ASSERT_EQ(data[offset + 0x2], 0x94);
    ASSERT_EQ(data[offset + 0x11], 0x80);
    ASSERT_EQ(data[offset + 0x12], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x1000, data, 0x20), 0);
    ASSERT_EQ(data.size(), 0x20);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x21000, data, 0x20), 0);
    ASSERT_EQ(data.size(), 0x20);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue2);

    ASSERT_THROW(group->getData(0xFFFFF, data, 2), std::out_of_range);
    ASSERT_THROW(group->getData(0x100000, data, 1), std::out_of_range);

    section1.convertToExtendedSegmentAddressSection(0x2000);
    ASSERT_EQ(group->pushSection(section1), 1);

    ASSERT_EQ(group->getData(0x1FFF0, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0x94);
    ASSERT_EQ(data[0xF], 0x80);
    ASSERT_EQ(data[0x10], 0x7F);
    ASSERT_EQ(data[0x1F], 0xC7);

    data.clear();
    data.resize(0x8, unusedDataFillValue1);
    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_THROW(group->getData(0, data, 32, 0x9), std::underflow_error);
    ASSERT_EQ(data.size(), 0x8);
    for (HexDataSizeType index{0}; index < 0x8; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0xF800);
    ASSERT_EQ(group->pushSection(section1), 0);
    section1.convertToExtendedSegmentAddressSection(0x800);
    ASSERT_EQ(group->pushSection(section1), 1);

    data.clear();
    ASSERT_EQ(group->getData(0xFFFF8, data, 0x8), 0x8);
    ASSERT_EQ(data.size(), 0x8);
    ASSERT_EQ(data[0], 0x0A);
    ASSERT_EQ(data[0x7], 0x08);

    ASSERT_EQ(group->getData(0, data, 0x8), 0x8);
    ASSERT_EQ(data.size(), 0x8);
    ASSERT_EQ(data[0], 0x11);
    ASSERT_EQ(data[0x7], 0x26);

    // SECTION_START_SEGMENT_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10000, data, 0x20), 0);
    ASSERT_EQ(data.size(), 0x20);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    group->clearSections();
    section1.convertToExtendedLinearAddressSection(0x1000);
    ASSERT_EQ(group->pushSection(section1), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10000000, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0x7F);
    ASSERT_EQ(data[0xF], 0xC7);
    ASSERT_EQ(data[0x10], unusedDataFillValue1);
    ASSERT_EQ(data[0x1F], unusedDataFillValue1);

    ASSERT_EQ(group->getData(0x10001000, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0xCF);
    ASSERT_EQ(data[0xF], 0x4F);
    ASSERT_EQ(data[0x10], 0x64);
    ASSERT_EQ(data[0x1F], 0x88);

    ASSERT_EQ(group->getData(0x10001002, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0xBC);
    ASSERT_EQ(data[0xD], 0x4F);
    ASSERT_EQ(data[0xE], 0x64);
    ASSERT_EQ(data[0x1D], 0x88);
    ASSERT_EQ(data[0x1E], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x10000FFE, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0x1], unusedDataFillValue2);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x1F], 0x9E);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10000FFE, data, 0x23), 0x23);
    ASSERT_EQ(data.size(), 0x23);
    ASSERT_EQ(data[0x1], unusedDataFillValue1);
    ASSERT_EQ(data[0x2], 0xCF);
    ASSERT_EQ(data[0x11], 0x4F);
    ASSERT_EQ(data[0x12], 0x64);
    ASSERT_EQ(data[0x21], 0x88);
    ASSERT_EQ(data[0x22], unusedDataFillValue1);

    data.clear();
    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x1000FFEE, data, 0x14), 0x12);
    ASSERT_EQ(data.size(), 0x14);
    ASSERT_EQ(data[0x1], unusedDataFillValue2);
    ASSERT_EQ(data[0x2], 0x94);
    ASSERT_EQ(data[0x11], 0x80);
    ASSERT_EQ(data[0x12], unusedDataFillValue2);

    offset = 8;
    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x1000FFEE, data, 0x14, offset), 0x12);
    ASSERT_EQ(data.size(), offset + 0x14);
    ASSERT_EQ(data[offset + 0x1], unusedDataFillValue1);
    ASSERT_EQ(data[offset + 0x2], 0x94);
    ASSERT_EQ(data[offset + 0x11], 0x80);
    ASSERT_EQ(data[offset + 0x12], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x20001000, data, 0x20), 0);
    ASSERT_EQ(data.size(), 0x20);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_EQ(group->getData(0x20001000, data, 0x20), 0);
    ASSERT_EQ(data.size(), 0x20);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue2);

    section1.convertToExtendedLinearAddressSection(0x1001);
    ASSERT_EQ(group->pushSection(section1), 1);

    ASSERT_EQ(group->getData(0x1000FFF0, data, 0x20), 0x20);
    ASSERT_EQ(data.size(), 0x20);
    ASSERT_EQ(data[0], 0x94);
    ASSERT_EQ(data[0xF], 0x80);
    ASSERT_EQ(data[0x10], 0x7F);
    ASSERT_EQ(data[0x1F], 0xC7);

    data.clear();
    data.resize(0x8, unusedDataFillValue1);
    group->setUnusedDataFillValue(unusedDataFillValue2);
    ASSERT_THROW(group->getData(0, data, 32, 0x9), std::underflow_error);
    ASSERT_EQ(data.size(), 0x8);
    for (HexDataSizeType index{0}; index < 0x8; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    // SECTION_START_LINEAR_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10000000, data, 0x20), 0);
    ASSERT_EQ(data.size(), 0x20);
    for (HexDataSizeType index{0}; index < 0x20; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);
}

void HexGroupTest::performSetDataTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexSection section1;
    ASSERT_TRUE(section1.pushRecord(":10000000BEB29870998BAACBDE43C61EE648CBCD14"));
    ASSERT_TRUE(section1.pushRecord(":10100000EFAAF492DF073FF9C77CE5908DF7C0FDAA"));
    ASSERT_TRUE(section1.pushRecord(":107FF800588F5A6540755325B054B812F72BDAAC30"));
    ASSERT_TRUE(section1.pushRecord(":10FFF0001C385D34490BFE8DB89D18A8E8D71B90BE"));

    // Empty group
    ASSERT_NO_THROW(group->setData(0x1234, 0x5A));
    ASSERT_EQ(group->getData(0x1234), 0x5A);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);

    // SECTION_DATA
    group->clearSections();
    group->setUnusedDataFillValue(unusedDataFillValue1);
    ASSERT_EQ(group->pushSection(section1), 0);

    ASSERT_NO_THROW(group->setData(0, 0x41));
    ASSERT_NO_THROW(group->setData(0xF, 0x32));
    ASSERT_NO_THROW(group->setData(0x100, 0x5A));
    ASSERT_NO_THROW(group->setData(0x10F, 0xA5));
    ASSERT_NO_THROW(group->setData(0x1000, 0x10));
    ASSERT_NO_THROW(group->setData(0x100F, 0x02));
    ASSERT_NO_THROW(group->setData(0xFFF0, 0xE3));
    ASSERT_NO_THROW(group->setData(0xFFFF, 0x6F));

    ASSERT_EQ(group->getData(0), 0x41);
    ASSERT_EQ(group->getData(1), 0xB2);
    ASSERT_EQ(group->getData(0xE), 0xCB);
    ASSERT_EQ(group->getData(0xF), 0x32);
    ASSERT_EQ(group->getData(0x10), unusedDataFillValue1);

    ASSERT_EQ(group->getData(0xFF), unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x100), 0x5A);
    ASSERT_EQ(group->getData(0x101), unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10E), unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x10F), 0xA5);
    ASSERT_EQ(group->getData(0x110), unusedDataFillValue1);

    ASSERT_EQ(group->getData(0xFFF), unusedDataFillValue1);
    ASSERT_EQ(group->getData(0x1000), 0x10);
    ASSERT_EQ(group->getData(0x1001), 0xAA);
    ASSERT_EQ(group->getData(0x100E), 0xC0);
    ASSERT_EQ(group->getData(0x100F), 0x02);
    ASSERT_EQ(group->getData(0x1010), unusedDataFillValue1);

    ASSERT_EQ(group->getData(0xFFEF), unusedDataFillValue1);
    ASSERT_EQ(group->getData(0xFFF0), 0xE3);
    ASSERT_EQ(group->getData(0xFFF1), 0x38);
    ASSERT_EQ(group->getData(0xFFFE), 0x1B);
    ASSERT_EQ(group->getData(0xFFFF), 0x6F);

    ASSERT_NO_THROW(group->setData(0x1234, 0xA5));
    ASSERT_EQ(group->getData(0x1234), 0xA5);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);

    ASSERT_THROW(group->setData(0x10000, 0), std::out_of_range);
    ASSERT_THROW(group->getData(0x10000), std::out_of_range);

    // SECTION_END_OF_FILE
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);

    ASSERT_NO_THROW(group->setData(0, 0x5A));
    ASSERT_NO_THROW(group->setData(0xFFFF, 0xA5));
    ASSERT_EQ(group->getData(0), 0x5A);
    ASSERT_EQ(group->getData(0xFFFF), 0xA5);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);

    ASSERT_THROW(group->setData(0x10000, 0), std::out_of_range);
    ASSERT_THROW(group->getData(0x10000), std::out_of_range);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    group->clearSections();
    group->setUnusedDataFillValue(unusedDataFillValue2);
    section1.convertToExtendedSegmentAddressSection(0x1000);
    ASSERT_EQ(group->pushSection(section1), 0);
    section1.convertToExtendedSegmentAddressSection(0x3000);
    ASSERT_EQ(group->pushSection(section1), 1);

    for (HexDataSizeType offset : std::vector{0x10000, 0x30000})
    {
        ASSERT_NO_THROW(group->setData(offset, 0x41));
        ASSERT_NO_THROW(group->setData(offset + 0xF, 0x32));
        ASSERT_NO_THROW(group->setData(offset + 0x100, 0x5A));
        ASSERT_NO_THROW(group->setData(offset + 0x10F, 0xA5));
        ASSERT_NO_THROW(group->setData(offset + 0x1000, 0x10));
        ASSERT_NO_THROW(group->setData(offset + 0x100F, 0x02));
        ASSERT_NO_THROW(group->setData(offset + 0xFFF0, 0xE3));
        ASSERT_NO_THROW(group->setData(offset + 0xFFFF, 0x6F));

        ASSERT_EQ(group->getData(offset + 0), 0x41);
        ASSERT_EQ(group->getData(offset + 1), 0xB2);
        ASSERT_EQ(group->getData(offset + 0xE), 0xCB);
        ASSERT_EQ(group->getData(offset + 0xF), 0x32);
        ASSERT_EQ(group->getData(offset + 0x10), unusedDataFillValue2);

        ASSERT_EQ(group->getData(offset + 0xFF), unusedDataFillValue2);
        ASSERT_EQ(group->getData(offset + 0x100), 0x5A);
        ASSERT_EQ(group->getData(offset + 0x101), unusedDataFillValue2);
        ASSERT_EQ(group->getData(offset + 0x10E), unusedDataFillValue2);
        ASSERT_EQ(group->getData(offset + 0x10F), 0xA5);
        ASSERT_EQ(group->getData(offset + 0x110), unusedDataFillValue2);

        ASSERT_EQ(group->getData(offset + 0xFFF), unusedDataFillValue2);
        ASSERT_EQ(group->getData(offset + 0x1000), 0x10);
        ASSERT_EQ(group->getData(offset + 0x1001), 0xAA);
        ASSERT_EQ(group->getData(offset + 0x100E), 0xC0);
        ASSERT_EQ(group->getData(offset + 0x100F), 0x02);
        ASSERT_EQ(group->getData(offset + 0x1010), unusedDataFillValue2);

        ASSERT_EQ(group->getData(offset + 0xFFEF), unusedDataFillValue2);
        ASSERT_EQ(group->getData(offset + 0xFFF0), 0xE3);
        ASSERT_EQ(group->getData(offset + 0xFFF1), 0x38);
        ASSERT_EQ(group->getData(offset + 0xFFFE), 0x1B);
        ASSERT_EQ(group->getData(offset + 0xFFFF), 0x6F);
    }

    ASSERT_NO_THROW(group->setData(0x51234, 0x5A));
    ASSERT_EQ(group->getData(0x51234), 0x5A);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(2).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0x5000);

    section1.convertToExtendedSegmentAddressSection(0xF800);
    ASSERT_EQ(group->pushSection(section1), 3);

    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0xF800);
    ASSERT_EQ(group->pushSection(section1), 0);
    section1.convertToExtendedSegmentAddressSection(0x800);
    ASSERT_EQ(group->pushSection(section1), 1);
    ASSERT_EQ(group->getData(0xFFFFE), 0x53);
    ASSERT_EQ(group->getData(0xFFFFF), 0x25);
    ASSERT_EQ(group->getData(0), 0xB0);
    ASSERT_EQ(group->getData(1), 0x54);

    ASSERT_THROW(group->setData(0x100000, 0), std::out_of_range);
    ASSERT_THROW(group->getData(0x100000), std::out_of_range);

    // SECTION_START_SEGMENT_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);

    ASSERT_NO_THROW(group->setData(0x10000, 0x5A));
    ASSERT_NO_THROW(group->setData(0x1FFFF, 0xA5));
    ASSERT_EQ(group->getData(0x10000), 0x5A);
    ASSERT_EQ(group->getData(0x1FFFF), 0xA5);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);

    ASSERT_THROW(group->setData(0x100000, 0), std::out_of_range);
    ASSERT_THROW(group->getData(0x100000), std::out_of_range);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    group->clearSections();
    group->setUnusedDataFillValue(unusedDataFillValue1);
    section1.convertToExtendedLinearAddressSection(0xD000);
    ASSERT_EQ(group->pushSection(section1), 0);
    section1.convertToExtendedLinearAddressSection(0xF000);
    ASSERT_EQ(group->pushSection(section1), 1);

    for (HexDataSizeType offset : std::vector{0xD0000000, 0xF0000000})
    {
        ASSERT_NO_THROW(group->setData(offset, 0x41));
        ASSERT_NO_THROW(group->setData(offset + 0xF, 0x32));
        ASSERT_NO_THROW(group->setData(offset + 0x100, 0x5A));
        ASSERT_NO_THROW(group->setData(offset + 0x10F, 0xA5));
        ASSERT_NO_THROW(group->setData(offset + 0x1000, 0x10));
        ASSERT_NO_THROW(group->setData(offset + 0x100F, 0x02));
        ASSERT_NO_THROW(group->setData(offset + 0xFFF0, 0xE3));
        ASSERT_NO_THROW(group->setData(offset + 0xFFFF, 0x6F));

        ASSERT_EQ(group->getData(offset + 0), 0x41);
        ASSERT_EQ(group->getData(offset + 1), 0xB2);
        ASSERT_EQ(group->getData(offset + 0xE), 0xCB);
        ASSERT_EQ(group->getData(offset + 0xF), 0x32);
        ASSERT_EQ(group->getData(offset + 0x10), unusedDataFillValue1);

        ASSERT_EQ(group->getData(offset + 0xFF), unusedDataFillValue1);
        ASSERT_EQ(group->getData(offset + 0x100), 0x5A);
        ASSERT_EQ(group->getData(offset + 0x101), unusedDataFillValue1);
        ASSERT_EQ(group->getData(offset + 0x10E), unusedDataFillValue1);
        ASSERT_EQ(group->getData(offset + 0x10F), 0xA5);
        ASSERT_EQ(group->getData(offset + 0x110), unusedDataFillValue1);

        ASSERT_EQ(group->getData(offset + 0xFFF), unusedDataFillValue1);
        ASSERT_EQ(group->getData(offset + 0x1000), 0x10);
        ASSERT_EQ(group->getData(offset + 0x1001), 0xAA);
        ASSERT_EQ(group->getData(offset + 0x100E), 0xC0);
        ASSERT_EQ(group->getData(offset + 0x100F), 0x02);
        ASSERT_EQ(group->getData(offset + 0x1010), unusedDataFillValue1);

        ASSERT_EQ(group->getData(offset + 0xFFEF), unusedDataFillValue1);
        ASSERT_EQ(group->getData(offset + 0xFFF0), 0xE3);
        ASSERT_EQ(group->getData(offset + 0xFFF1), 0x38);
        ASSERT_EQ(group->getData(offset + 0xFFFE), 0x1B);
        ASSERT_EQ(group->getData(offset + 0xFFFF), 0x6F);
    }

    ASSERT_NO_THROW(group->setData(0x51234567, 0xA5));
    ASSERT_EQ(group->getData(0x51234567), 0xA5);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(2).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(group->getSection(2).getExtendedLinearAddress(), 0x5123);

    // SECTION_START_LINEAR_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);

    ASSERT_NO_THROW(group->setData(0x10000000, 0x5A));
    ASSERT_NO_THROW(group->setData(0x1000FFFF, 0xA5));
    ASSERT_EQ(group->getData(0x10000000), 0x5A);
    ASSERT_EQ(group->getData(0x1000FFFF), 0xA5);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
}

void HexGroupTest::performSetDataArrayTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexSection section1;
    ASSERT_TRUE(section1.pushRecord(":10000000CD55A960EC0D3FA39E8EE2E07F97766B05"));
    ASSERT_TRUE(section1.pushRecord(":10010000854EE78E361A41D76E7D4DE9638B6B7E47"));
    ASSERT_TRUE(section1.pushRecord(":107FF800F1FD5E82138CC6CCDBA39A3F9978E4A08E"));

    HexAddressType base;
    HexDataArray<MAXIMUM_HEX_RECORD_DATA_SIZE> data{}, testData{};
    for (size_t index{0}; index < data.size(); ++index)
        data[index] = ((2 * index) + 2);

    // Empty group
    base = 0x1000;
    ASSERT_EQ(group->setData(base, &data[0], data.size()), data.size());
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getDataSize(), data.size());

    ASSERT_EQ(group->getData(base, &testData[0], testData.size()), testData.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);

    // SECTION_DATA
    group->clearSections();
    ASSERT_EQ(group->pushSection(section1), 0);

    base = 0x10;
    ASSERT_EQ(group->setData(base, &data[0], data.size()), data.size());
    ASSERT_EQ(group->getSize(), 1);

    ASSERT_EQ(group->getData(base - 1), 0x6B);
    ASSERT_EQ(group->getData(base, &testData[0], testData.size()), testData.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getData(base + data.size()), 0x7E);

    base = 0xFFFF;
    ASSERT_EQ(group->setData(base - 1, &data[0], 2), 2);
    ASSERT_EQ(group->getData(base - 1), data[0]);
    ASSERT_EQ(group->getData(base), data[1]);
    ASSERT_THROW(group->setData(base, &data[0], 2), std::out_of_range);

    // SECTION_END_OF_FILE
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);

    ASSERT_EQ(group->setData(0, &data[0], data.size()), data.size());
    ASSERT_EQ(group->getData(0, &testData[0], data.size()), data.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);

    ASSERT_THROW(group->setData(0xFFFF, &data[0], 2), std::out_of_range);
    ASSERT_THROW(group->setData(0x10000, &data[0], 1), std::out_of_range);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    group->setUnusedDataFillValue(unusedDataFillValue1);
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0x1000);
    ASSERT_EQ(group->pushSection(section1), 0);

    base = 0x10010;
    ASSERT_EQ(group->setData(base, &data[0], data.size()), data.size());
    ASSERT_EQ(group->getSize(), 1);

    ASSERT_EQ(group->getData(base - 1), 0x6B);
    ASSERT_EQ(group->getData(base, &testData[0], testData.size()), testData.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getData(base + data.size()), 0x7E);

    base = 0x20010;
    ASSERT_EQ(group->setData(base, &data[0], data.size()), data.size());
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(1).getExtendedSegmentAddress(), 0x2000);

    ASSERT_EQ(group->getData(base - 1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base, &testData[0], testData.size()), testData.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getData(base + data.size()), unusedDataFillValue1);

    base = 0xFFFFF;
    ASSERT_EQ(group->setData(base - 1, &data[0], 2), 2);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0xF000);
    ASSERT_EQ(group->getData(base - 1), data[0]);
    ASSERT_EQ(group->getData(base), data[1]);
    ASSERT_THROW(group->setData(base, &data[0], 2), std::out_of_range);

    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0xF800);
    ASSERT_EQ(group->pushSection(section1), 0);
    section1.convertToExtendedSegmentAddressSection(0x800);
    ASSERT_EQ(group->pushSection(section1), 1);

    ASSERT_EQ(group->setData(0xFFFF8, &data[0], 8), 8);
    ASSERT_EQ(group->getData(0xFFFF8, &testData[0], 8), 8);
    for (size_t index{0}; index < 8; ++index)
        ASSERT_EQ(data[index], testData[index]);

    ASSERT_EQ(group->setData(0, &data[0], 8), 8);
    ASSERT_EQ(group->getData(0, &testData[0], 8), 8);
    for (size_t index{0}; index < 8; ++index)
        ASSERT_EQ(data[index], testData[index]);

    // SECTION_START_SEGMENT_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);

    base = 0xF0000;
    ASSERT_EQ(group->setData(base, &data[0], data.size()), data.size());
    ASSERT_EQ(group->getData(base, &testData[0], data.size()), data.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(group->getSection(1).getExtendedSegmentAddress(), 0xF000);

    ASSERT_THROW(group->setData(0xFFFFF, &data[0], 2), std::out_of_range);
    ASSERT_THROW(group->setData(0x100000, &data[0], 1), std::out_of_range);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    group->clearSections();
    section1.convertToExtendedLinearAddressSection(0x1000);
    ASSERT_EQ(group->pushSection(section1), 0);

    base = 0x10000010;
    ASSERT_EQ(group->setData(base, &data[0], data.size()), data.size());
    ASSERT_EQ(group->getSize(), 1);

    ASSERT_EQ(group->getData(base - 1), 0x6B);
    ASSERT_EQ(group->getData(base, &testData[0], testData.size()), testData.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getData(base + data.size()), 0x7E);

    base = 0xFFFFFFFF;
    ASSERT_EQ(group->setData(base - 1, &data[0], 2), 2);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(1).getExtendedLinearAddress(), 0xFFFF);
    ASSERT_EQ(group->getData(base - 1), data[0]);
    ASSERT_EQ(group->getData(base), data[1]);
    ASSERT_THROW(group->setData(base, &data[0], 2), std::out_of_range);

    // SECTION_START_LINEAR_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);

    base = 0xF0000000;
    ASSERT_EQ(group->setData(base, &data[0], data.size()), data.size());
    ASSERT_EQ(group->getData(base, &testData[0], data.size()), data.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(group->getSection(1).getExtendedLinearAddress(), 0xF000);
}

void HexGroupTest::performSetDataVectorTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexSection section1;
    ASSERT_TRUE(section1.pushRecord(":100000005AAEC48FCA2B5D578960EA3A1184846264"));
    ASSERT_TRUE(section1.pushRecord(":10010000D19B8FCC2519547170DDA615D2EA351F0D"));
    ASSERT_TRUE(section1.pushRecord(":107FF800388523A0263B63EC3E57A66F0351B44354"));

    HexAddressType base;
    HexDataSizeType offset;
    HexDataVector data, testData;
    data.resize(MAXIMUM_HEX_RECORD_DATA_SIZE, DEFAULT_HEX_RECORD_DATA_VALUE);
    testData.resize(MAXIMUM_HEX_RECORD_DATA_SIZE, DEFAULT_HEX_RECORD_DATA_VALUE);
    for (size_t index{0}; index < data.size(); ++index)
        data[index] = ((3 * index) + 2);

    // Empty group
    group->setUnusedDataFillValue(unusedDataFillValue1);
    base = 0x1000;
    ASSERT_EQ(group->setData(base, data, data.size(), 0), data.size());
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getDataSize(), data.size());

    ASSERT_EQ(group->getData(base - 1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base, testData, testData.size(), 0), testData.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data.at(index), testData.at(index));
    ASSERT_EQ(group->getData(base + data.size()), unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    base = 0x2000;
    offset = 0x10;
    ASSERT_EQ(group->setData(base, data, data.size(), offset), data.size() - offset);
    ASSERT_EQ(group->getSection(0).getDataSize(), data.size() + (data.size() - offset));

    ASSERT_EQ(group->getData(base - 1), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base, testData, testData.size(), 0), testData.size());
    for (size_t index{0}; index < (data.size() - offset); ++index)
        ASSERT_EQ(data.at(index + offset), testData.at(index));
    ASSERT_EQ(group->getData(base + (data.size() - offset)), unusedDataFillValue2);

    ASSERT_EQ(group->setData(0x3000, data, data.size(), data.size() + 1), 0);
    ASSERT_EQ(group->getSection(0).getDataSize(), data.size() + (data.size() - offset));

    // SECTION_DATA
    group->clearSections();
    ASSERT_EQ(group->pushSection(section1), 0);

    base = 0x10;
    ASSERT_EQ(group->setData(base, data, data.size(), 0), data.size());
    ASSERT_EQ(group->getSize(), 1);

    ASSERT_EQ(group->getData(base - 1), 0x62);
    ASSERT_EQ(group->getData(base, testData, testData.size(), 0), testData.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getData(base + data.size()), 0x1F);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    base = 0x2000;
    offset = 0x10;
    ASSERT_EQ(group->setData(base, data, data.size(), offset), data.size() - offset);

    ASSERT_EQ(group->getData(base - 1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base, testData, testData.size(), 0), testData.size());
    for (size_t index{0}; index < (data.size() - offset); ++index)
        ASSERT_EQ(data.at(index + offset), testData.at(index));
    ASSERT_EQ(group->getData(base + (data.size() - offset)), unusedDataFillValue1);

    ASSERT_EQ(group->setData(0x3000, data, data.size(), data.size() + 1), 0);

    base = 0xFFFF;
    offset = 0x20;
    ASSERT_EQ(group->setData(base - 1, data, 2, offset), 2);
    ASSERT_EQ(group->getData(base - 1), data[offset]);
    ASSERT_EQ(group->getData(base), data[offset + 1]);
    ASSERT_THROW(group->setData(base, data, 2, 0), std::out_of_range);

    // SECTION_END_OF_FILE
    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);

    ASSERT_EQ(group->setData(0, data, data.size()), data.size());
    ASSERT_EQ(group->getData(0, testData, data.size()), data.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);

    ASSERT_THROW(group->setData(0xFFFF, data, 2), std::out_of_range);
    ASSERT_THROW(group->setData(0x10000, data, 1), std::out_of_range);

    // SECTION_EXTENDED_SEGMENT_ADDRESS
    group->setUnusedDataFillValue(unusedDataFillValue1);
    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0x1000);
    ASSERT_EQ(group->pushSection(section1), 0);

    base = 0x10010;
    ASSERT_EQ(group->setData(base, data, data.size(), 0), data.size());
    ASSERT_EQ(group->getSize(), 1);

    ASSERT_EQ(group->getData(base - 1), 0x62);
    ASSERT_EQ(group->getData(base, testData, testData.size(), 0), testData.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getData(base + data.size()), 0x1F);

    base = 0x20010;
    ASSERT_EQ(group->setData(base, data, data.size(), 0), data.size());
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(1).getExtendedSegmentAddress(), 0x2000);

    ASSERT_EQ(group->getData(base - 1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base, testData, testData.size(), 0), testData.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getData(base + data.size()), unusedDataFillValue1);

    group->setUnusedDataFillValue(unusedDataFillValue2);
    base = 0x2FFF0;
    offset = 0x30;
    ASSERT_EQ(group->setData(base, data, data.size(), offset), data.size() - offset);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(2).getExtendedSegmentAddress(), 0x3000);

    ASSERT_EQ(group->getData(base - 1), unusedDataFillValue2);
    ASSERT_EQ(group->getData(base, testData, testData.size(), 0), testData.size());
    for (size_t index{0}; index < (data.size() - offset); ++index)
        ASSERT_EQ(data.at(index + offset), testData.at(index));
    ASSERT_EQ(group->getData(base + (data.size() - offset)), unusedDataFillValue2);

    ASSERT_EQ(group->setData(0x30000, data, data.size(), data.size() + 1), 0);

    base = 0xFFFFF;
    offset = 0x40;
    ASSERT_EQ(group->setData(base - 1, data, 2, offset), 2);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(3).getExtendedSegmentAddress(), 0xF000);
    ASSERT_EQ(group->getData(base - 1), data[offset]);
    ASSERT_EQ(group->getData(base), data[offset + 1]);
    ASSERT_THROW(group->setData(base, data, 2, 0), std::out_of_range);

    group->clearSections();
    section1.convertToExtendedSegmentAddressSection(0xF800);
    ASSERT_EQ(group->pushSection(section1), 0);
    section1.convertToExtendedSegmentAddressSection(0x800);
    ASSERT_EQ(group->pushSection(section1), 1);

    ASSERT_EQ(group->setData(0xFFFF8, data, 8), 8);
    ASSERT_EQ(group->getData(0xFFFF8, testData, 8), 8);
    for (size_t index{0}; index < 8; ++index)
        ASSERT_EQ(data[index], testData[index]);

    ASSERT_EQ(group->setData(0, data, 8), 8);
    ASSERT_EQ(group->getData(0, testData, 8), 8);
    for (size_t index{0}; index < 8; ++index)
        ASSERT_EQ(data[index], testData[index]);

    // SECTION_START_SEGMENT_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);

    base = 0xF0000;
    ASSERT_EQ(group->setData(base, data, data.size()), data.size());
    ASSERT_EQ(group->getData(base, testData, data.size()), data.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(group->getSection(1).getExtendedSegmentAddress(), 0xF000);

    ASSERT_THROW(group->setData(0xFFFFF, data, 2), std::out_of_range);
    ASSERT_THROW(group->setData(0x100000, data, 1), std::out_of_range);

    // SECTION_EXTENDED_LINEAR_ADDRESS
    group->clearSections();
    section1.convertToExtendedLinearAddressSection(0x1000);
    ASSERT_EQ(group->pushSection(section1), 0);

    base = 0x10000010;
    ASSERT_EQ(group->setData(base, data, data.size(), 0), data.size());
    ASSERT_EQ(group->getSize(), 1);

    ASSERT_EQ(group->getData(base - 1), 0x62);
    ASSERT_EQ(group->getData(base, testData, testData.size(), 0), testData.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getData(base + data.size()), 0x1F);

    group->setUnusedDataFillValue(unusedDataFillValue1);
    base = 0x2FFFFFF0;
    offset = 0x50;
    ASSERT_EQ(group->setData(base, data, data.size(), offset), data.size() - offset);
    ASSERT_EQ(group->getSize(), 3);
    ASSERT_EQ(group->getSection(1).getExtendedLinearAddress(), 0x2FFF);
    ASSERT_EQ(group->getSection(2).getExtendedLinearAddress(), 0x3000);

    ASSERT_EQ(group->getData(base - 1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(base, testData, testData.size(), 0), testData.size());
    for (size_t index{0}; index < (data.size() - offset); ++index)
        ASSERT_EQ(data.at(index + offset), testData.at(index));
    ASSERT_EQ(group->getData(base + (data.size() - offset)), unusedDataFillValue1);

    ASSERT_EQ(group->setData(0x30000000, data, data.size(), data.size() + 1), 0);

    base = 0xFFFFFFFF;
    offset = 0x60;
    ASSERT_EQ(group->setData(base - 1, data, 2, offset), 2);
    ASSERT_EQ(group->getSize(), 4);
    ASSERT_EQ(group->getSection(3).getExtendedLinearAddress(), 0xFFFF);
    ASSERT_EQ(group->getData(base - 1), data[offset]);
    ASSERT_EQ(group->getData(base), data[offset + 1]);
    ASSERT_THROW(group->setData(base, data, 2, 0), std::out_of_range);

    // SECTION_START_LINEAR_ADDRESS
    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);

    base = 0xF0000000;
    ASSERT_EQ(group->setData(base, data, data.size()), data.size());
    ASSERT_EQ(group->getData(base, testData, data.size()), data.size());
    for (size_t index{0}; index < data.size(); ++index)
        ASSERT_EQ(data[index], testData[index]);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(group->getSection(1).getExtendedLinearAddress(), 0xF000);
}

void HexGroupTest::performFillDataTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexAddressType address{};
    HexDataSizeType size{};
    HexDataArray data{};

    // Empty group
    address = 0xFFE0;
    size = 0x20;
    ASSERT_EQ(group->fillData(address, size, unusedDataFillValue1), size);
    ASSERT_EQ(group->getData(address - 0x1), group->getUnusedDataFillValue());
    ASSERT_EQ(group->getData(address, &data[0], size), size);
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);

    // ADDRESS_I8HEX
    address = 0xFFC0;
    ASSERT_EQ(group->fillData(address, size, unusedDataFillValue2), size);
    ASSERT_EQ(group->getData(address - 0x1), group->getUnusedDataFillValue());
    ASSERT_EQ(group->getData(address, &data[0], size), size);
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue2);
    ASSERT_EQ(group->getData(address + size), unusedDataFillValue1);

    // ADDRESS_I16HEX
    group->clearSections();
    address = 0xFFE0;
    size = 0x20;
    group->createSection(HexAddressEnumType::ADDRESS_I16HEX, address, size + 1);
    ASSERT_EQ(group->fillData(address, size, unusedDataFillValue1), size);
    ASSERT_EQ(group->getData(address - 0x1), group->getUnusedDataFillValue());
    ASSERT_EQ(group->getData(address, &data[0], size), size);
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);
    ASSERT_EQ(group->getData(address + size), group->getUnusedDataFillValue());

    address = 0x10000;
    ASSERT_EQ(group->fillData(address, size, unusedDataFillValue2), size);
    ASSERT_EQ(group->getData(address - 0x1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(address, &data[0], size), size);
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue2);
    ASSERT_EQ(group->getData(address + size), group->getUnusedDataFillValue());

    // ADDRESS_I32HEX
    group->clearSections();
    address = 0x1000FFE0;
    size = 0x20;
    group->createSection(HexAddressEnumType::ADDRESS_I32HEX, address, size + 1);
    ASSERT_EQ(group->fillData(address, size, unusedDataFillValue1), size);
    ASSERT_EQ(group->getData(address - 0x1), group->getUnusedDataFillValue());
    ASSERT_EQ(group->getData(address, &data[0], size), size);
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue1);
    ASSERT_EQ(group->getData(address + size), group->getUnusedDataFillValue());

    address = 0x10010000;
    ASSERT_EQ(group->fillData(address, size, unusedDataFillValue2), size);
    ASSERT_EQ(group->getData(address - 0x1), unusedDataFillValue1);
    ASSERT_EQ(group->getData(address, &data[0], size), size);
    for (size_t index{0}; index < size; ++index)
        ASSERT_EQ(data[index], unusedDataFillValue2);
    ASSERT_EQ(group->getData(address + size), group->getUnusedDataFillValue());
}

void HexGroupTest::performPropertiesTests()
{
    SCOPED_TRACE(__func__);

    // hasDataSections()

    ASSERT_FALSE(group->hasDataSections());

    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_FALSE(group->hasDataSections());
    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_TRUE(group->hasDataSections());

    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_FALSE(group->hasDataSections());
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 1);
    ASSERT_TRUE(group->hasDataSections());

    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_FALSE(group->hasDataSections());
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 1);
    ASSERT_TRUE(group->hasDataSections());
}

void HexGroupTest::performStartSegmentAddressTests()
{
    SCOPED_TRACE(__func__);

    // supportsStartSegmentAddress()

    group->clearSections();
    ASSERT_TRUE(group->supportsStartSegmentAddress());

    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_FALSE(group->supportsStartSegmentAddress());

    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_TRUE(group->supportsStartSegmentAddress());

    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection2), 1);
    ASSERT_TRUE(group->supportsStartSegmentAddress());

    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_TRUE(group->supportsStartSegmentAddress());

    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_FALSE(group->supportsStartSegmentAddress());

    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_FALSE(group->supportsStartSegmentAddress());

    // hasStartSegmentAddress()

    group->clearSections();
    ASSERT_FALSE(group->hasStartSegmentAddress());
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection2), 1);
    ASSERT_FALSE(group->hasStartSegmentAddress());
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 2);
    ASSERT_TRUE(group->hasStartSegmentAddress());

    // clearStartSegmentAddress()

    group->clearStartSegmentAddress();
    ASSERT_FALSE(group->hasStartSegmentAddress());

    // getStartSegmentAddressSection() const

    const HexGroup& constGroup{*group};
    group->clearSections();
    ASSERT_THROW(constGroup.getStartSegmentAddressSection(), std::runtime_error);

    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_THROW(constGroup.getStartSegmentAddressSection(), std::runtime_error);

    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 1);
    ASSERT_EQ(constGroup.getStartSegmentAddressSection().getRecord(0).getRecord(), startSegmentAddressSection.getRecord(0).getRecord());

    // getStartSegmentAddressSection()

    group->clearSections();
    ASSERT_THROW(group->getStartSegmentAddressSection(), std::runtime_error);

    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_THROW(group->getStartSegmentAddressSection(), std::runtime_error);

    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 1);
    ASSERT_EQ(group->getStartSegmentAddressSection().getRecord(0).getRecord(), startSegmentAddressSection.getRecord(0).getRecord());

    // getCodeSegment(), getInstructionPointer()

    group->clearSections();
    ASSERT_THROW(group->getCodeSegment(), std::runtime_error);
    ASSERT_THROW(group->getInstructionPointer(), std::runtime_error);

    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_THROW(group->getCodeSegment(), std::runtime_error);
    ASSERT_THROW(group->getInstructionPointer(), std::runtime_error);

    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_EQ(group->getCodeSegment(), startSegmentAddressSection.getStartSegmentAddressCodeSegment());
    ASSERT_EQ(group->getInstructionPointer(), startSegmentAddressSection.getStartSegmentAddressInstructionPointer());

    // setCodeSegment()

    group->clearSections();
    ASSERT_NO_THROW(group->setCodeSegment(0x1122));
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I16HEX);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getRecord(0).getRecord(), std::string(":0400000311220000C6"));
    ASSERT_EQ(group->getCodeSegment(), 0x1122);
    ASSERT_EQ(group->getInstructionPointer(), 0);

    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    ASSERT_NO_THROW(group->setCodeSegment(0x3344));
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I16HEX);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getRecord(0).getRecord(), std::string(":040000033344000082"));
    ASSERT_EQ(group->getCodeSegment(), 0x3344);
    ASSERT_EQ(group->getInstructionPointer(), 0);

    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_THROW(group->setCodeSegment(0), std::runtime_error);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I32HEX);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getRecord(0).getRecord(), startLinearAddressSection.getRecord(0).getRecord());

    // setInstructionPointer()

    group->clearSections();
    ASSERT_NO_THROW(group->setInstructionPointer(0x1122));
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I16HEX);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getRecord(0).getRecord(), std::string(":0400000300001122C6"));
    ASSERT_EQ(group->getCodeSegment(), 0);
    ASSERT_EQ(group->getInstructionPointer(), 0x1122);

    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    ASSERT_NO_THROW(group->setInstructionPointer(0x3344));
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I16HEX);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getRecord(0).getRecord(), std::string(":040000030000334482"));
    ASSERT_EQ(group->getCodeSegment(), 0);
    ASSERT_EQ(group->getInstructionPointer(), 0x3344);

    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_THROW(group->setInstructionPointer(0), std::runtime_error);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I32HEX);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getRecord(0).getRecord(), startLinearAddressSection.getRecord(0).getRecord());
}

void HexGroupTest::performStartLinearAddressTests()
{
    SCOPED_TRACE(__func__);

    // supportsStartLinearAddress()

    group->clearSections();
    ASSERT_TRUE(group->supportsStartLinearAddress());

    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_FALSE(group->supportsStartLinearAddress());

    group->clearSections();
    ASSERT_EQ(group->pushSection(endOfFileSection), 0);
    ASSERT_TRUE(group->supportsStartLinearAddress());

    ASSERT_EQ(group->pushSection(extendedSegmentAddressSection1), 0);
    ASSERT_FALSE(group->supportsStartLinearAddress());

    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_FALSE(group->supportsStartLinearAddress());

    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), 1);
    ASSERT_TRUE(group->supportsStartLinearAddress());

    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_TRUE(group->supportsStartLinearAddress());

    // hasStartLinearAddress()

    group->clearSections();
    ASSERT_FALSE(group->hasStartLinearAddress());
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection2), 1);
    ASSERT_FALSE(group->hasStartLinearAddress());
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 2);
    ASSERT_TRUE(group->hasStartLinearAddress());

    // clearStartLinearAddress()

    group->clearStartLinearAddress();
    ASSERT_FALSE(group->hasStartLinearAddress());

    // getStartLinearAddressSection() const

    const HexGroup& constGroup{*group};
    group->clearSections();
    ASSERT_THROW(constGroup.getStartLinearAddressSection(), std::runtime_error);

    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_THROW(constGroup.getStartLinearAddressSection(), std::runtime_error);

    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 1);
    ASSERT_EQ(constGroup.getStartLinearAddressSection().getRecord(0).getRecord(), startLinearAddressSection.getRecord(0).getRecord());

    // getStartLinearAddressSection()

    group->clearSections();
    ASSERT_THROW(group->getStartLinearAddressSection(), std::runtime_error);

    ASSERT_EQ(group->pushSection(dataSection), 0);
    ASSERT_THROW(group->getStartLinearAddressSection(), std::runtime_error);

    group->clearSections();
    ASSERT_EQ(group->pushSection(extendedLinearAddressSection1), 0);
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 1);
    ASSERT_EQ(group->getStartLinearAddressSection().getRecord(0).getRecord(), startLinearAddressSection.getRecord(0).getRecord());

    // getExtendedInstructionPointer()

    group->clearSections();
    ASSERT_THROW(group->getExtendedInstructionPointer(), std::runtime_error);

    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_THROW(group->getExtendedInstructionPointer(), std::runtime_error);

    group->clearSections();
    ASSERT_EQ(group->pushSection(startLinearAddressSection), 0);
    ASSERT_EQ(group->getExtendedInstructionPointer(), startLinearAddressSection.getStartLinearAddressExtendedInstructionPointer());

    // setExtendedInstructionPointer()

    group->clearSections();
    ASSERT_NO_THROW(group->setExtendedInstructionPointer(0x11223344));
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I32HEX);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getRecord(0).getRecord(), std::string(":04000005112233444D"));
    ASSERT_EQ(group->getExtendedInstructionPointer(), 0x11223344);

    ASSERT_EQ(group->pushSection(endOfFileSection), 1);
    ASSERT_NO_THROW(group->setExtendedInstructionPointer(0x55667788));
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I32HEX);
    ASSERT_EQ(group->getSize(), 2);
    ASSERT_EQ(group->getSection(0).getRecord(0).getRecord(), std::string(":04000005556677883D"));
    ASSERT_EQ(group->getExtendedInstructionPointer(), 0x55667788);

    group->clearSections();
    ASSERT_EQ(group->pushSection(startSegmentAddressSection), 0);
    ASSERT_THROW(group->setExtendedInstructionPointer(0), std::runtime_error);
    ASSERT_EQ(group->getGroupType(), HexAddressEnumType::ADDRESS_I16HEX);
    ASSERT_EQ(group->getSize(), 1);
    ASSERT_EQ(group->getSection(0).getRecord(0).getRecord(), startSegmentAddressSection.getRecord(0).getRecord());
}

END_NAMESPACE_LIBIHEX
