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
#include <ihex_test/test_hex_record_impl.hpp>

BEGIN_NAMESPACE_LIBIHEX

HexRecordTest::HexRecordTest()
{

}

HexRecordTest::~HexRecordTest()
{

}

void HexRecordTest::SetUp()
{
    Test::SetUp();
    record = std::make_shared<HexRecord>();
}

void HexRecordTest::TearDown()
{
    record.reset();
    Test::TearDown();
}

void HexRecordTest::performConstructorTests()
{
    SCOPED_TRACE(__func__);

    // HexRecord()

    const HexRecord record1{};
    ASSERT_TRUE(record1.isValidRecord());

    // HexRecord(record)

    HexRecord record2{record1};
    ASSERT_TRUE(record2.isValidRecord());

    // HexRecord(std::move(record))

    HexRecord record3(std::move(record2));
    ASSERT_TRUE(record3.isValidRecord());

    // HexRecord(std::string)

    // RECORD_DATA
    const HexRecord record4{":108E110096A055EBF7B49A5529853CF3998DE191CC"};
    ASSERT_TRUE(record4.isValidRecord());
    ASSERT_EQ(record4.getDataSize(), 16);
    ASSERT_EQ(record4.getRecordType(), HexRecordType::RECORD_DATA);
    ASSERT_EQ(record4.getAddress(), 0x8E11);
    ASSERT_EQ(record4.getData(0), 0x96);
    ASSERT_EQ(record4.getData(15), 0x91);
    ASSERT_EQ(record4.getChecksum(), 0xCC);
    ASSERT_TRUE(record4.isValidChecksum());

    ASSERT_THROW(HexRecord{":108E110096A055EBF7B49A5529853CF3998DE19133"}, std::logic_error);

    // RECORD_END_OF_FILE
    const HexRecord record5{":00000001FF"};
    ASSERT_TRUE(record5.isValidRecord());
    ASSERT_EQ(record5.getDataSize(), 0);
    ASSERT_EQ(record5.getRecordType(), HexRecordType::RECORD_END_OF_FILE);
    ASSERT_EQ(record5.getAddress(), 0);
    ASSERT_EQ(record5.getChecksum(), 0xFF);
    ASSERT_TRUE(record5.isValidChecksum());

    ASSERT_THROW(HexRecord{":0000000100"}, std::logic_error);

    // RECORD_EXTENDED_SEGMENT_ADDRESS
    const HexRecord record6{":02000002EFDE2F"};
    ASSERT_TRUE(record6.isValidRecord());
    ASSERT_EQ(record6.getDataSize(), 2);
    ASSERT_EQ(record6.getRecordType(), HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(record6.getAddress(), 0);
    ASSERT_EQ(record6.getData(0), 0xEF);
    ASSERT_EQ(record6.getData(1), 0xDE);
    ASSERT_EQ(record6.getChecksum(), 0x2F);
    ASSERT_TRUE(record6.isValidChecksum());

    ASSERT_THROW(HexRecord{":02000002EFDED0"}, std::logic_error);

    // RECORD_START_SEGMENT_ADDRESS
    const HexRecord record7{":04000003CDBCAB9A2B"};
    ASSERT_TRUE(record7.isValidRecord());
    ASSERT_EQ(record7.getDataSize(), 4);
    ASSERT_EQ(record7.getRecordType(), HexRecordType::RECORD_START_SEGMENT_ADDRESS);
    ASSERT_EQ(record7.getAddress(), 0);
    ASSERT_EQ(record7.getData(0), 0xCD);
    ASSERT_EQ(record7.getData(3), 0x9A);
    ASSERT_EQ(record7.getChecksum(), 0x2B);
    ASSERT_TRUE(record7.isValidChecksum());

    ASSERT_THROW(HexRecord{":04000003CDBCAB9AD4"}, std::logic_error);

    // RECORD_EXTENDED_LINEAR_ADDRESS
    const HexRecord record8{":020000048978F9"};
    ASSERT_TRUE(record8.isValidRecord());
    ASSERT_EQ(record8.getDataSize(), 2);
    ASSERT_EQ(record8.getRecordType(), HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(record8.getAddress(), 0);
    ASSERT_EQ(record8.getData(0), 0x89);
    ASSERT_EQ(record8.getData(1), 0x78);
    ASSERT_EQ(record8.getChecksum(), 0xF9);
    ASSERT_TRUE(record8.isValidChecksum());

    ASSERT_THROW(HexRecord{":02000004897806"}, std::logic_error);

    // RECORD_START_LINEAR_ADDRESS
    const HexRecord record9{":0400000567564534C1"};
    ASSERT_TRUE(record9.isValidRecord());
    ASSERT_EQ(record9.getDataSize(), 4);
    ASSERT_EQ(record9.getRecordType(), HexRecordType::RECORD_START_LINEAR_ADDRESS);
    ASSERT_EQ(record9.getAddress(), 0);
    ASSERT_EQ(record9.getData(0), 0x67);
    ASSERT_EQ(record9.getData(3), 0x34);
    ASSERT_EQ(record9.getChecksum(), 0xC1);
    ASSERT_TRUE(record9.isValidChecksum());

    ASSERT_THROW(HexRecord{":04000005675645343E"}, std::logic_error);

    ASSERT_THROW(HexRecord{""}, std::runtime_error);
    ASSERT_THROW(HexRecord{"abc"}, std::runtime_error);
    ASSERT_THROW(HexRecord{":"}, std::runtime_error);
    ASSERT_THROW(HexRecord{":10"}, std::runtime_error);
    ASSERT_THROW(HexRecord{":000000"}, std::runtime_error);
    ASSERT_THROW(HexRecord{":00000000"}, std::runtime_error);
    ASSERT_THROW(HexRecord{":0100000010"}, std::runtime_error);

    // HexRecord(type)

    // RECORD_DATA
    const HexRecord record10{HexRecordType::RECORD_DATA};
    ASSERT_TRUE(record10.isValidRecord());

    // RECORD_END_OF_FILE
    const HexRecord record11{HexRecordType::RECORD_END_OF_FILE};
    ASSERT_EQ(record11.getRecord(), std::string(":00000001FF"));
    ASSERT_TRUE(record11.isValidRecord());

    // RECORD_EXTENDED_SEGMENT_ADDRESS
    const HexRecord record12{HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS};
    ASSERT_EQ(record12.getRecord(), std::string(":020000020000FC"));
    ASSERT_TRUE(record12.isValidRecord());

    // RECORD_START_SEGMENT_ADDRESS
    const HexRecord record13{HexRecordType::RECORD_START_SEGMENT_ADDRESS};
    ASSERT_EQ(record13.getRecord(), std::string(":0400000300000000F9"));
    ASSERT_TRUE(record13.isValidRecord());

    // RECORD_EXTENDED_LINEAR_ADDRESS
    const HexRecord record14{HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS};
    ASSERT_EQ(record14.getRecord(), std::string(":020000040000FA"));
    ASSERT_TRUE(record14.isValidRecord());

    // RECORD_START_LINEAR_ADDRESS
    const HexRecord record15{HexRecordType::RECORD_START_LINEAR_ADDRESS};
    ASSERT_EQ(record15.getRecord(), std::string(":0400000500000000F7"));
    ASSERT_TRUE(record15.isValidRecord());
}

void HexRecordTest::performMakeRecordTests()
{
    SCOPED_TRACE(__func__);

    // Test data
    uint8_t data[16];
    for (size_t index{0}; index < 16; ++index)
        data[index] = (index);

    std::array<uint8_t, 16> dataArray;
    for (size_t index{0}; index < dataArray.size(); ++index)
        dataArray[index] = (2 * index);

    HexDataVector dataVector(16, DEFAULT_HEX_RECORD_DATA_VALUE);
    for (size_t index{0}; index < dataVector.size(); ++index)
        dataVector[index] = (3 * index);

    // RECORD_DATA
    const auto record1{HexRecord::makeDataRecord(0x1122, &data[0], 16)};
    ASSERT_EQ(record1.getRecord(), std::string(":10112200000102030405060708090A0B0C0D0E0F45"));

    const auto record2{HexRecord::makeDataRecord(0x3344, &dataArray[0], 16)};
    ASSERT_EQ(record2.getRecord(), std::string(":1033440000020406080A0C0E10121416181A1C1E89"));

    const auto record3{HexRecord::makeDataRecord(0x5566, dataVector)};
    ASSERT_EQ(record3.getRecord(), std::string(":10556600000306090C0F1215181B1E2124272A2DCD"));

    ASSERT_THROW(HexRecord::makeDataRecord(0, HexDataVector{}), std::out_of_range);
    ASSERT_THROW(HexRecord::makeDataRecord(0, nullptr, 0), std::out_of_range);

    // RECORD_END_OF_FILE
    const auto record4{HexRecord::makeEndOfFileRecord()};
    ASSERT_EQ(record4.getRecord(), std::string(":00000001FF"));

    // RECORD_EXTENDED_SEGMENT_ADDRESS
    const auto record5{HexRecord::makeExtendedSegmentAddressRecord(0x7788)};
    ASSERT_EQ(record5.getRecord(), std::string(":020000027788FD"));

    // RECORD_START_SEGMENT_ADDRESS
    const auto record6{HexRecord::makeStartSegmentAddressRecord(0x99AA, 0xBBCC)};
    ASSERT_EQ(record6.getRecord(), std::string(":0400000399AABBCC2F"));

    // RECORD_EXTENDED_LINEAR_ADDRESS
    const auto record7{HexRecord::makeExtendedLinearAddressRecord(0xDDEE)};
    ASSERT_EQ(record7.getRecord(), std::string(":02000004DDEE2F"));

    // RECORD_START_LINEAR_ADDRESS
    const auto record8{HexRecord::makeStartLinearAddressRecord(0xFF001122)};
    ASSERT_EQ(record8.getRecord(), std::string(":04000005FF001122C5"));
}

void HexRecordTest::performPrepareRecordTests()
{
    SCOPED_TRACE(__func__);

    // Test data
    uint8_t data[16];
    for (size_t index{0}; index < 16; ++index)
        data[index] = (2 * index);

    std::array<uint8_t, 16> dataArray;
    for (size_t index{0}; index < dataArray.size(); ++index)
        dataArray[index] = (3 * index);

    HexDataVector dataVector(16, DEFAULT_HEX_RECORD_DATA_VALUE);
    for (size_t index{0}; index < dataVector.size(); ++index)
        dataVector[index] = (4 * index);

    HexDataVector dataVector2(256, 0);

    // RECORD_DATA
    record->prepareRecord(0x1122, HexRecordType::RECORD_DATA, &data[0], 16, 0x33);
    ASSERT_EQ(record->getRecord(), std::string(":1011220000020406080A0C0E10121416181A1C1E33"));
    ASSERT_TRUE(record->isValidRecord());
    ASSERT_FALSE(record->isValidChecksum());

    record->prepareRecord(0x4455, HexRecordType::RECORD_DATA, &dataArray[0], 16, 0x66);
    ASSERT_EQ(record->getRecord(), std::string(":10445500000306090C0F1215181B1E2124272A2D66"));
    ASSERT_TRUE(record->isValidRecord());
    ASSERT_FALSE(record->isValidChecksum());

    record->prepareRecord(0x7788, HexRecordType::RECORD_DATA, dataVector, 0x99);
    ASSERT_EQ(record->getRecord(), std::string(":107788000004080C1014181C2024282C3034383C99"));
    ASSERT_TRUE(record->isValidRecord());
    ASSERT_FALSE(record->isValidChecksum());

    ASSERT_THROW(record->prepareRecord(0, HexRecordType::RECORD_DATA, &data[0], 256), std::out_of_range);
    ASSERT_THROW(record->prepareRecord(0, HexRecordType::RECORD_DATA, dataVector2), std::out_of_range);

    // RECORD_END_OF_FILE
    record->prepareRecord(0xAABB, HexRecordType::RECORD_END_OF_FILE, &data[0], 12);
    ASSERT_EQ(record->getRecord(), std::string(":0CAABB0100020406080A0C0E101214160A"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    record->prepareRecord(0xCCDD, HexRecordType::RECORD_END_OF_FILE, &dataArray[0], 12);
    ASSERT_EQ(record->getRecord(), std::string(":0CCCDD01000306090C0F1215181B1E2184"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    dataVector.resize(12);
    record->prepareRecord(0xEEFF, HexRecordType::RECORD_END_OF_FILE, dataVector);
    ASSERT_EQ(record->getRecord(), std::string(":0CEEFF010004080C1014181C2024282CFE"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    // RECORD_EXTENDED_SEGMENT_ADDRESS
    record->prepareRecord(0x0011, HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS, &data[0], 8);
    ASSERT_EQ(record->getRecord(), std::string(":0800110200020406080A0C0EAD"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    record->prepareRecord(0x2233, HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS, &dataArray[0], 8);
    ASSERT_EQ(record->getRecord(), std::string(":08223302000306090C0F12154D"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    dataVector.resize(8);
    record->prepareRecord(0x4455, HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS, dataVector);
    ASSERT_EQ(record->getRecord(), std::string(":084455020004080C1014181CED"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    // RECORD_START_SEGMENT_ADDRESS
    record->prepareRecord(0x3344, HexRecordType::RECORD_START_SEGMENT_ADDRESS, &data[0], 8);
    ASSERT_EQ(record->getRecord(), std::string(":0833440300020406080A0C0E46"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    record->prepareRecord(0, HexRecordType::RECORD_START_SEGMENT_ADDRESS, &dataArray[0], 4);
    ASSERT_EQ(record->getRecord(), std::string(":0400000300030609E7"));
    ASSERT_TRUE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    dataVector.resize(4);
    record->prepareRecord(0, HexRecordType::RECORD_START_SEGMENT_ADDRESS, dataVector);
    ASSERT_EQ(record->getRecord(), std::string(":040000030004080CE1"));
    ASSERT_TRUE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    // RECORD_EXTENDED_LINEAR_ADDRESS
    record->prepareRecord(0x6677, HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS, &data[0], 4);
    ASSERT_EQ(record->getRecord(), std::string(":04667704000204060F"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    record->prepareRecord(0x8899, HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS, &dataArray[0], 2);
    ASSERT_EQ(record->getRecord(), std::string(":028899040003D6"));
    ASSERT_TRUE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    dataVector.resize(2);
    record->prepareRecord(0xAABB, HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS, dataVector);
    ASSERT_EQ(record->getRecord(), std::string(":02AABB04000491"));
    ASSERT_TRUE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    // RECORD_START_LINEAR_ADDRESS
    record->prepareRecord(0xCCDD, HexRecordType::RECORD_START_LINEAR_ADDRESS, nullptr, 0);
    ASSERT_EQ(record->getRecord(), std::string(":00CCDD0552"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    record->prepareRecord(0xEEFF, HexRecordType::RECORD_START_LINEAR_ADDRESS, nullptr, 0);
    ASSERT_EQ(record->getRecord(), std::string(":00EEFF050E"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());

    dataVector.clear();
    record->prepareRecord(0x0011, HexRecordType::RECORD_START_LINEAR_ADDRESS, dataVector);
    ASSERT_EQ(record->getRecord(), std::string(":00001105EA"));
    ASSERT_FALSE(record->isValidRecord());
    ASSERT_TRUE(record->isValidChecksum());
}

void HexRecordTest::performPrepareDataRecordTests()
{
    SCOPED_TRACE(__func__);

    // Test data
    uint8_t data[16];
    for (size_t index{0}; index < 16; ++index)
        data[index] = (2 * index);

    std::array<uint8_t, 16> dataArray;
    for (size_t index{0}; index < dataArray.size(); ++index)
        dataArray[index] = (3 * index);

    HexDataVector dataVector(16, DEFAULT_HEX_RECORD_DATA_VALUE);
    for (size_t index{0}; index < dataVector.size(); ++index)
        dataVector[index] = (4 * index);

    HexDataVector dataVector2(256, 0), dataVector3{};

    // RECORD_DATA #1
    ASSERT_NO_THROW(record->prepareDataRecord(0x1A2B, &data[0], 16));
    ASSERT_EQ(record->getRecord(), std::string(":101A2B0000020406080A0C0E10121416181A1C1EBB"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_TRUE(record->isValidRecord());

    ASSERT_NO_THROW(record->prepareDataRecord(0x3C4D, &dataArray[0], 16));
    ASSERT_EQ(record->getRecord(), std::string(":103C4D00000306090C0F1215181B1E2124272A2DFF"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_TRUE(record->isValidRecord());

    ASSERT_NO_THROW(record->prepareDataRecord(0x5E6F, dataVector));
    ASSERT_EQ(record->getRecord(), std::string(":105E6F000004080C1014181C2024282C3034383C43"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_TRUE(record->isValidRecord());

    ASSERT_THROW(record->prepareDataRecord(0, &data[0], 256), std::out_of_range);
    ASSERT_THROW(record->prepareDataRecord(0, dataVector2), std::out_of_range);

    ASSERT_THROW(record->prepareDataRecord(0, &data[0], 0), std::out_of_range);
    ASSERT_THROW(record->prepareDataRecord(0, dataVector3), std::out_of_range);

    // RECORD_DATA #2
    ASSERT_NO_THROW(record->prepareDataRecord(0x7081, &data[0], 16));
    ASSERT_EQ(record->getRecord(), std::string(":1070810000020406080A0C0E10121416181A1C1E0F"));

    ASSERT_NO_THROW(record->prepareDataRecord(0x92A3, &dataArray[0], 16));
    ASSERT_EQ(record->getRecord(), std::string(":1092A300000306090C0F1215181B1E2124272A2D53"));

    ASSERT_NO_THROW(record->prepareDataRecord(0xB4C5, dataVector));
    ASSERT_EQ(record->getRecord(), std::string(":10B4C5000004080C1014181C2024282C3034383C97"));

    // RECORD_DATA #3
    dataVector.resize(8);

    ASSERT_NO_THROW(record->prepareDataRecord(0xD6E7, &data[0], 8));
    ASSERT_EQ(record->getRecord(), std::string(":08D6E70000020406080A0C0E03"));

    ASSERT_NO_THROW(record->prepareDataRecord(0xF819, &dataArray[0], 8));
    ASSERT_EQ(record->getRecord(), std::string(":08F81900000306090C0F121593"));

    ASSERT_NO_THROW(record->prepareDataRecord(0x2A3B, dataVector));
    ASSERT_EQ(record->getRecord(), std::string(":082A3B000004080C1014181C23"));

    // RECORD_DATA #4
    dataVector.resize(2);

    ASSERT_NO_THROW(record->prepareDataRecord(0xD6E7, &data[0], 2));
    ASSERT_EQ(record->getRecord(), std::string(":02D6E70000023F"));

    ASSERT_NO_THROW(record->prepareDataRecord(0xF819, &dataArray[0], 2));
    ASSERT_EQ(record->getRecord(), std::string(":02F819000003EA"));

    ASSERT_NO_THROW(record->prepareDataRecord(0x2A3B, dataVector));
    ASSERT_EQ(record->getRecord(), std::string(":022A3B00000495"));

    // RECORD_DATA #5
    dataVector.clear();

    ASSERT_THROW(record->prepareDataRecord(0, &data[0], 0), std::out_of_range);
    ASSERT_THROW(record->prepareDataRecord(0, dataVector), std::out_of_range);
}

void HexRecordTest::performPrepareEndOfFileRecordTests()
{
    SCOPED_TRACE(__func__);

    record->prepareEndOfFileRecord();
    ASSERT_EQ(record->getRecord(), std::string(":00000001FF"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_TRUE(record->isValidRecord());
}

void HexRecordTest::performPrepareExtendedSegmentAddressRecordTests()
{
    SCOPED_TRACE(__func__);

    record->prepareExtendedSegmentAddressRecord(0x4C5D);
    ASSERT_EQ(record->getRecord(), std::string(":020000024C5D53"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_TRUE(record->isValidRecord());
}

void HexRecordTest::performPrepareStartSegmentAddressRecordTests()
{
    SCOPED_TRACE(__func__);

    record->prepareStartSegmentAddressRecord(0x6E7F, 0x8091);
    ASSERT_EQ(record->getRecord(), std::string(":040000036E7F8091FB"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_TRUE(record->isValidRecord());
}

void HexRecordTest::performPrepareExtendedLinearAddressRecordTests()
{
    SCOPED_TRACE(__func__);

    record->prepareExtendedLinearAddressRecord(0xA2B3);
    ASSERT_EQ(record->getRecord(), std::string(":02000004A2B3A5"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_TRUE(record->isValidRecord());
}

void HexRecordTest::performPrepareStartLinearAddressRecordTests()
{
    SCOPED_TRACE(__func__);

    record->prepareStartLinearAddressRecord(0xC4D5E6F7);
    ASSERT_EQ(record->getRecord(), std::string(":04000005C4D5E6F781"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_TRUE(record->isValidRecord());
}

void HexRecordTest::performGetSetRecordPropertiesTests()
{
    SCOPED_TRACE(__func__);

    // Test data
    uint8_t data[16];
    for (size_t index{0}; index < 16; ++index)
        data[index] = (0x20 + (2 * index));

    // Function tests
    ASSERT_TRUE(record->setDataSize(MAXIMUM_HEX_RECORD_DATA_SIZE));
    ASSERT_FALSE(record->setDataSize(MAXIMUM_HEX_RECORD_DATA_SIZE + 1));

    ASSERT_TRUE(record->setRecordType(HexRecordType::RECORD_MIN));
    ASSERT_TRUE(record->setRecordType(HexRecordType::RECORD_MAX));
    ASSERT_FALSE(record->setRecordType(static_cast<HexRecordType>(static_cast<uint8_t>(HexRecordType::RECORD_MIN) - 1)));
    ASSERT_FALSE(record->setRecordType(static_cast<HexRecordType>(static_cast<uint8_t>(HexRecordType::RECORD_MAX) + 1)));

    // RECORD_DATA
    ASSERT_TRUE(record->setDataSize(16));
    ASSERT_EQ(record->getDataSize(), 16);

    record->setAddress(0x1829);
    ASSERT_EQ(record->getAddress(), 0x1829);

    for (uint8_t index{0}; index < 16; ++index)
    {
        record->setData(index, data[index]);
        ASSERT_EQ(record->getData(index), data[index]);
    }

    ASSERT_TRUE(record->setRecordType(HexRecordType::RECORD_DATA));
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_DATA);

    record->setChecksum(0xBF);
    ASSERT_EQ(record->getChecksum(), 0xBF);
    ASSERT_TRUE(record->isValidChecksum());

    ASSERT_EQ(record->getRecord(), std::string(":1018290020222426282A2C2E30323436383A3C3EBF"));

    // RECORD_END_OF_FILE
    ASSERT_TRUE(record->setDataSize(0));
    ASSERT_EQ(record->getDataSize(), 0);

    record->setAddress(0);
    ASSERT_EQ(record->getAddress(), 0);

    ASSERT_TRUE(record->setRecordType(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_END_OF_FILE);

    record->setChecksum(0xFF);
    ASSERT_EQ(record->getChecksum(), 0xFF);
    ASSERT_TRUE(record->isValidChecksum());

    ASSERT_EQ(record->getRecord(), std::string(":00000001FF"));

    // RECORD_EXTENDED_SEGMENT_ADDRESS
    ASSERT_TRUE(record->setDataSize(2));
    ASSERT_EQ(record->getDataSize(), 2);

    record->setAddress(0);
    ASSERT_EQ(record->getAddress(), 0);

    ASSERT_TRUE(record->setRecordType(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS);

    for (uint8_t index{0}; index < 2; ++index)
    {
        record->setData(index, data[index] + 1);
        ASSERT_EQ(record->getData(index), data[index] + 1);
    }

    record->setChecksum(0xB8);
    ASSERT_EQ(record->getChecksum(), 0xB8);
    ASSERT_TRUE(record->isValidChecksum());

    ASSERT_EQ(record->getRecord(), std::string(":020000022123B8"));

    // RECORD_START_SEGMENT_ADDRESS
    ASSERT_TRUE(record->setDataSize(4));
    ASSERT_EQ(record->getDataSize(), 4);

    record->setAddress(0);
    ASSERT_EQ(record->getAddress(), 0);

    ASSERT_TRUE(record->setRecordType(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_START_SEGMENT_ADDRESS);

    for (uint8_t index{0}; index < 4; ++index)
    {
        record->setData(index, data[index] + 2);
        ASSERT_EQ(record->getData(index), data[index] + 2);
    }

    record->setChecksum(0x65);
    ASSERT_EQ(record->getChecksum(), 0x65);
    ASSERT_TRUE(record->isValidChecksum());

    ASSERT_EQ(record->getRecord(), std::string(":040000032224262865"));

    // RECORD_EXTENDED_LINEAR_ADDRESS
    ASSERT_TRUE(record->setDataSize(2));
    ASSERT_EQ(record->getDataSize(), 2);

    record->setAddress(0);
    ASSERT_EQ(record->getAddress(), 0);

    ASSERT_TRUE(record->setRecordType(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS);

    for (uint8_t index{0}; index < 2; ++index)
    {
        record->setData(index, data[index] + 3);
        ASSERT_EQ(record->getData(index), data[index] + 3);
    }

    record->setChecksum(0xB2);
    ASSERT_EQ(record->getChecksum(), 0xB2);
    ASSERT_TRUE(record->isValidChecksum());

    ASSERT_EQ(record->getRecord(), std::string(":020000042325B2"));

    // RECORD_START_LINEAR_ADDRESS
    ASSERT_TRUE(record->setDataSize(4));
    ASSERT_EQ(record->getDataSize(), 4);

    record->setAddress(0);
    ASSERT_EQ(record->getAddress(), 0);

    ASSERT_TRUE(record->setRecordType(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_START_LINEAR_ADDRESS);

    for (uint8_t index{0}; index < 4; ++index)
    {
        record->setData(index, data[index] + 4);
        ASSERT_EQ(record->getData(index), data[index] + 4);
    }

    record->setChecksum(0x5B);
    ASSERT_EQ(record->getChecksum(), 0x5B);
    ASSERT_TRUE(record->isValidChecksum());

    ASSERT_EQ(record->getRecord(), std::string(":040000052426282A5B"));
}

void HexRecordTest::performGetDataPropertiesTests()
{
    SCOPED_TRACE(__func__);

    HexDataArray<4> dataInArray{};
    HexDataArray<8> dataOutArray{};
    HexDataType value{0x5A};

    // RECORD_DATA (HexDataType, operator [])
    for (size_t index{0}; index < dataInArray.size(); ++index)
        dataInArray[index] = (0x4B + (0x22 * index));

    ASSERT_NO_THROW(record->prepareDataRecord(0, &dataInArray[0], dataInArray.size()));

    for (HexDataSizeType index{0}; index < record->getDataSize(); ++index)
        ASSERT_EQ((*record)[index], dataInArray[index]);

    ASSERT_THROW(value = (*record)[record->getDataSize() + 1], std::out_of_range);

    // RECORD_DATA (HexDataType, operator [] const)
    const HexRecord record1{*record};
    for (HexDataSizeType index{0}; index < record1.getDataSize(); ++index)
        ASSERT_EQ(record1[index], dataInArray[index]);

    // RECORD_DATA (HexDataType)
    for (size_t index{0}; index < dataInArray.size(); ++index)
        dataInArray[index] = (0x3C + (0x33 * index));

    ASSERT_NO_THROW(record->prepareDataRecord(0, &dataInArray[0], dataInArray.size()));

    for (HexDataSizeType index{0}; index < record->getDataSize(); ++index)
        ASSERT_EQ(record->getData(index), dataInArray[index]);

    ASSERT_NO_THROW(value = record->getData(record->getDataSize() + 1));
    ASSERT_EQ(value, DEFAULT_HEX_RECORD_DATA_VALUE);

    // RECORD_DATA (HexDataArray)
    for (size_t index{0}; index < dataInArray.size(); ++index)
        dataInArray[index] = (0x5A + (0x11 * index));

    ASSERT_NO_THROW(record->prepareDataRecord(0, &dataInArray[0], dataInArray.size()));

    ASSERT_EQ(record->getData(&dataOutArray[0], 1), 1);
    ASSERT_EQ(dataOutArray[0], dataInArray[0]);
    ASSERT_EQ(dataOutArray[1], 0);

    ASSERT_EQ(record->getData(&dataOutArray[0], 1, 3), 1);
    ASSERT_EQ(dataOutArray[0], dataInArray[3]);
    ASSERT_EQ(dataOutArray[1], 0);

    ASSERT_EQ(record->getData(&dataOutArray[0], 2), 2);
    ASSERT_EQ(dataOutArray[0], dataInArray[0]);
    ASSERT_EQ(dataOutArray[1], dataInArray[1]);
    ASSERT_EQ(dataOutArray[2], 0);

    ASSERT_EQ(record->getData(&dataOutArray[0], 2, 2), 2);
    ASSERT_EQ(dataOutArray[0], dataInArray[2]);
    ASSERT_EQ(dataOutArray[1], dataInArray[3]);
    ASSERT_EQ(dataOutArray[2], 0);

    for (size_t index{0}; index < dataOutArray.size(); ++index)
        dataOutArray[index] = 0;

    ASSERT_THROW(record->getData(&dataOutArray[0], dataInArray.size() + 1), std::out_of_range);
    ASSERT_EQ(dataOutArray[0], 0);

    ASSERT_THROW(record->getData(&dataOutArray[0], dataInArray.size() + 1, 1), std::out_of_range);
    ASSERT_EQ(dataOutArray[0], 0);

    ASSERT_THROW(record->getData(&dataOutArray[0], dataInArray.size(), 1), std::out_of_range);
    ASSERT_EQ(dataOutArray[0], 0);

    ASSERT_THROW(record->getData(&dataOutArray[0], 1, dataInArray.size()), std::out_of_range);
    ASSERT_EQ(dataOutArray[0], 0);

    ASSERT_EQ(record->getData(&dataOutArray[0], 0), 0);
    ASSERT_EQ(dataOutArray[0], 0);

    ASSERT_EQ(record->getData(&dataOutArray[0], 0, 1), 0);
    ASSERT_EQ(dataOutArray[0], 0);

    ASSERT_EQ(record->getData(&dataOutArray[0], 0, dataInArray.size()), 0);
    ASSERT_EQ(dataOutArray[0], 0);

    ASSERT_TRUE(record->setDataSize(0));
    ASSERT_EQ(record->getData(&dataOutArray[0], 4), 0);

    // RECORD_DATA (HexDataVector)
    HexDataVector dataInVector(4, 0), dataOutVector(8, 0);
    for (size_t index{0}; index < dataInVector.size(); ++index)
        dataInVector[index] = (0xA5 + (0x22 * index));

    ASSERT_NO_THROW(record->prepareDataRecord(0, dataInVector));

    const auto vectorSizeDiff{dataOutVector.size() - dataInVector.size()};
    ASSERT_GE(vectorSizeDiff, 0);

    ASSERT_EQ(record->getData(dataOutVector, 1), 1);
    ASSERT_EQ(dataOutVector[0], dataInVector[0]);
    ASSERT_EQ(dataOutVector[1], 0);

    ASSERT_EQ(record->getData(dataOutVector, 1, 3), 1);
    ASSERT_EQ(dataOutVector[0], dataInVector[3]);
    ASSERT_EQ(dataOutVector[1], 0);

    ASSERT_EQ(record->getData(dataOutVector, 2), 2);
    ASSERT_EQ(dataOutVector[0], dataInVector[0]);
    ASSERT_EQ(dataOutVector[1], dataInVector[1]);
    ASSERT_EQ(dataOutVector[2], 0);

    ASSERT_EQ(record->getData(dataOutVector, 2, 2), 2);
    ASSERT_EQ(dataOutVector[0], dataInVector[2]);
    ASSERT_EQ(dataOutVector[1], dataInVector[3]);
    ASSERT_EQ(dataOutVector[2], 0);

    for (size_t index{0}; index < dataOutVector.size(); ++index)
        dataOutVector[index] = 0;

    ASSERT_EQ(record->getData(dataOutVector, 1, 0, 1), 1);
    ASSERT_EQ(dataOutVector[0], 0);
    ASSERT_EQ(dataOutVector[1], dataInVector[0]);
    ASSERT_EQ(dataOutVector[2], 0);

    ASSERT_EQ(record->getData(dataOutVector, 1, 0, 3), 1);
    ASSERT_EQ(dataOutVector[2], 0);
    ASSERT_EQ(dataOutVector[3], dataInVector[0]);
    ASSERT_EQ(dataOutVector[4], 0);

    for (size_t index{0}; index < dataOutVector.size(); ++index)
        dataOutVector[index] = 0;

    ASSERT_THROW(record->getData(dataOutVector, dataInVector.size() + 1), std::out_of_range);
    ASSERT_EQ(dataOutVector[0], 0);

    ASSERT_THROW(record->getData(dataOutVector, dataInVector.size() + 1, 1), std::out_of_range);
    ASSERT_EQ(dataOutVector[0], 0);

    ASSERT_THROW(record->getData(dataOutVector, dataInVector.size(), 1), std::out_of_range);
    ASSERT_EQ(dataOutVector[0], 0);

    ASSERT_THROW(record->getData(dataOutVector, dataInVector.size(), 0, vectorSizeDiff + 1), std::out_of_range);
    ASSERT_EQ(dataOutVector[vectorSizeDiff + 1], 0);

    ASSERT_THROW(record->getData(dataOutVector, 1, dataInVector.size()), std::out_of_range);
    ASSERT_EQ(dataOutVector[0], 0);

    ASSERT_THROW(record->getData(dataOutVector, 1, 0, dataOutVector.size()), std::out_of_range);
    ASSERT_EQ(dataOutVector[dataOutVector.size() - 1], 0);

    ASSERT_EQ(record->getData(dataOutVector, 0), 0);
    ASSERT_EQ(dataOutVector[0], 0);

    ASSERT_EQ(record->getData(dataOutVector, 0, 1), 0);
    ASSERT_EQ(dataOutVector[0], 0);

    ASSERT_EQ(record->getData(dataOutVector, 0, 0, 1), 0);
    ASSERT_EQ(dataOutVector[1], 0);

    ASSERT_EQ(record->getData(dataOutVector, 0, dataInVector.size()), 0);
    ASSERT_EQ(dataOutVector[0], 0);

    ASSERT_EQ(record->getData(dataOutVector, 0, 0, dataOutVector.size()), 0);
    ASSERT_EQ(dataOutVector[dataOutVector.size() - 1], 0);

    ASSERT_TRUE(record->setDataSize(0));
    ASSERT_EQ(record->getData(dataOutVector, 4), 0);
}

void HexRecordTest::performSetDataPropertiesTests()
{
    SCOPED_TRACE(__func__);

    HexDataArray<8> dataOriginal{}, dataNew{}, dataCompare{};
    HexDataType value{0x5A};

    for (size_t index{0}; index < dataOriginal.size(); ++index)
        dataOriginal[index] = (2 * index);

    for (size_t index{0}; index < dataNew.size(); ++index)
        dataNew[index] = (3 * index);

    // RECORD_DATA (HexDataType, operator [])
    ASSERT_NO_THROW(record->prepareDataRecord(0, &dataOriginal[0], dataOriginal.size()));

    for (HexDataSizeType index{0}; index < record->getDataSize(); ++index)
        ASSERT_NO_THROW((*record)[index] = dataNew[index]);

    ASSERT_THROW((*record)[record->getDataSize() + 1] = 0, std::out_of_range);

    for (HexDataSizeType index{0}; index < record->getDataSize(); ++index)
        ASSERT_EQ((*record)[index], dataNew[index]);

    ASSERT_THROW(value = (*record)[record->getDataSize() + 1], std::out_of_range);

    // RECORD_DATA (HexDataType)
    ASSERT_NO_THROW(record->prepareDataRecord(0, &dataOriginal[0], dataOriginal.size()));

    for (HexDataSizeType index{0}; index < record->getDataSize(); ++index)
        ASSERT_NO_THROW(record->setData(index, dataNew[index]));

    ASSERT_NO_THROW(record->setData(record->getDataSize() + 1, 0));

    for (HexDataSizeType index{0}; index < record->getDataSize(); ++index)
        ASSERT_EQ(record->getData(index), dataNew[index]);

    ASSERT_NO_THROW(value = record->getData(record->getDataSize() + 1));
    ASSERT_EQ(value, DEFAULT_HEX_RECORD_DATA_VALUE);

    // RECORD_DATA (HexDataArray)
    ASSERT_NO_THROW(record->prepareDataRecord(0, &dataOriginal[0], dataOriginal.size()));

    ASSERT_EQ(record->setData(&dataNew[0], 1), 1);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    ASSERT_EQ(dataCompare[0], dataNew[0]);
    ASSERT_EQ(dataCompare[1], dataOriginal[1]);

    ASSERT_EQ(record->setData(&dataNew[0], 1, 3), 1);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    ASSERT_EQ(dataCompare[3], dataNew[0]);
    ASSERT_EQ(dataCompare[4], dataOriginal[4]);

    ASSERT_EQ(record->setData(&dataNew[0], 2), 2);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    ASSERT_EQ(dataCompare[0], dataNew[0]);
    ASSERT_EQ(dataCompare[1], dataNew[1]);
    ASSERT_EQ(dataCompare[2], dataOriginal[2]);

    ASSERT_EQ(record->setData(&dataNew[0], 2, 2), 2);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    ASSERT_EQ(dataCompare[2], dataNew[0]);
    ASSERT_EQ(dataCompare[3], dataNew[1]);
    ASSERT_EQ(dataCompare[4], dataOriginal[4]);

    ASSERT_NO_THROW(record->prepareDataRecord(0, &dataOriginal[0], dataOriginal.size()));

    ASSERT_THROW(record->setData(&dataNew[0], dataNew.size() + 1), std::out_of_range);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    for (size_t index{0}; index < dataCompare.size(); ++index)
        ASSERT_EQ(dataCompare[index], dataOriginal[index]);

    ASSERT_THROW(record->setData(&dataNew[0], dataNew.size() + 1, 1), std::out_of_range);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    for (size_t index{0}; index < dataCompare.size(); ++index)
        ASSERT_EQ(dataCompare[index], dataOriginal[index]);

    ASSERT_THROW(record->setData(&dataNew[0], dataNew.size(), 1), std::out_of_range);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    for (size_t index{0}; index < dataCompare.size(); ++index)
        ASSERT_EQ(dataCompare[index], dataOriginal[index]);

    ASSERT_THROW(record->setData(&dataNew[0], 1, dataNew.size()), std::out_of_range);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    for (size_t index{0}; index < dataCompare.size(); ++index)
        ASSERT_EQ(dataCompare[index], dataOriginal[index]);

    ASSERT_EQ(record->setData(&dataNew[0], 0), 0);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    for (size_t index{0}; index < dataCompare.size(); ++index)
        ASSERT_EQ(dataCompare[index], dataOriginal[index]);

    ASSERT_EQ(record->setData(&dataNew[0], 0, 1), 0);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    for (size_t index{0}; index < dataCompare.size(); ++index)
        ASSERT_EQ(dataCompare[index], dataOriginal[index]);

    ASSERT_EQ(record->setData(&dataNew[0], 0, dataNew.size()), 0);
    ASSERT_EQ(record->getData(&dataCompare[0], dataCompare.size()), dataCompare.size());
    for (size_t index{0}; index < dataCompare.size(); ++index)
        ASSERT_EQ(dataCompare[index], dataOriginal[index]);

    ASSERT_TRUE(record->setDataSize(0));
    ASSERT_EQ(record->setData(&dataNew[0], 8), 0);

    // RECORD_DATA (HexDataVector)
    HexDataVector dataOriginalVector(8, 0), dataNewVector(8, 0), dataCompareVector(8, 0);
    for (size_t index{0}; index < dataOriginalVector.size(); ++index)
        dataOriginalVector[index] = (4 * index);
    for (size_t index{0}; index < dataNewVector.size(); ++index)
        dataNewVector[index] = (5 * index);

    ASSERT_NO_THROW(record->prepareDataRecord(0, dataOriginalVector));

    ASSERT_EQ(record->setData(dataNewVector, 1), 1);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    ASSERT_EQ(dataCompareVector[0], dataNewVector[0]);
    ASSERT_EQ(dataCompareVector[1], dataOriginalVector[1]);

    ASSERT_EQ(record->setData(dataNewVector, 1, 3), 1);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    ASSERT_EQ(dataCompareVector[0], dataNewVector[3]);
    ASSERT_EQ(dataCompareVector[1], dataOriginalVector[1]);

    ASSERT_EQ(record->setData(dataNewVector, 2), 2);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    ASSERT_EQ(dataCompareVector[0], dataNewVector[0]);
    ASSERT_EQ(dataCompareVector[1], dataNewVector[1]);
    ASSERT_EQ(dataCompareVector[2], dataOriginalVector[2]);

    ASSERT_EQ(record->setData(dataNewVector, 2, 2), 2);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    ASSERT_EQ(dataCompareVector[0], dataNewVector[2]);
    ASSERT_EQ(dataCompareVector[1], dataNewVector[3]);
    ASSERT_EQ(dataCompareVector[2], dataOriginalVector[2]);

    ASSERT_EQ(record->setData(dataOriginalVector, dataOriginalVector.size()), dataOriginalVector.size());
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_EQ(record->setData(dataNewVector, 1, 0, 1), 1);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    ASSERT_EQ(dataCompareVector[0], dataOriginalVector[0]);
    ASSERT_EQ(dataCompareVector[1], dataNewVector[0]);
    ASSERT_EQ(dataCompareVector[2], dataOriginalVector[2]);

    ASSERT_EQ(record->setData(dataNewVector, 1, 0, 3), 1);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    ASSERT_EQ(dataCompareVector[2], dataOriginalVector[2]);
    ASSERT_EQ(dataCompareVector[3], dataNewVector[0]);
    ASSERT_EQ(dataCompareVector[4], dataOriginalVector[4]);

    ASSERT_EQ(record->setData(dataOriginalVector, dataOriginalVector.size()), dataOriginalVector.size());

    ASSERT_THROW(record->getData(dataNewVector, dataNewVector.size() + 1), std::out_of_range);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_THROW(record->setData(dataNewVector, dataNewVector.size() + 1, 1), std::out_of_range);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_THROW(record->setData(dataNewVector, dataNewVector.size(), 1), std::out_of_range);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_THROW(record->setData(dataNewVector, dataNewVector.size(), 0, dataOriginalVector.size() + 1), std::out_of_range);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_THROW(record->setData(dataNewVector, 1, dataNewVector.size()), std::out_of_range);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_THROW(record->setData(dataNewVector, 1, 0, dataOriginalVector.size()), std::out_of_range);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_EQ(record->setData(dataNewVector, 0), 0);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_EQ(record->setData(dataNewVector, 0, 1), 0);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_EQ(record->setData(dataNewVector, 0, 0, 1), 0);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_EQ(record->setData(dataNewVector, 0, dataNewVector.size()), 0);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_EQ(record->setData(dataNewVector, 0, 0, dataOriginalVector.size()), 0);
    ASSERT_EQ(record->getData(dataCompareVector, dataCompareVector.size()), dataCompareVector.size());
    for (size_t index{0}; index < dataCompareVector.size(); ++index)
        ASSERT_EQ(dataCompareVector[index], dataOriginalVector[index]);

    ASSERT_TRUE(record->setDataSize(0));
    ASSERT_EQ(record->setData(dataNewVector, 8), 0);
}

void HexRecordTest::performSetRecordTests()
{
    SCOPED_TRACE(__func__);

    // RECORD_DATA
    ASSERT_TRUE(record->setRecord(":103A4B0000020406080A0C0E10121416181A1C1E7B"));
    ASSERT_EQ(record->getRecord(), std::string(":103A4B0000020406080A0C0E10121416181A1C1E7B"));

    ASSERT_EQ(record->getDataSize(), 16);
    ASSERT_EQ(record->getAddress(), 0x3A4B);
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_DATA);
    for (uint8_t index{0}; index < record->getDataSize(); ++index)
        ASSERT_EQ(record->getData(index), (2 * index));
    ASSERT_EQ(record->getChecksum(), 0x7B);

    // RECORD_END_OF_FILE
    ASSERT_TRUE(record->setRecord(":00000001FF"));
    ASSERT_EQ(record->getRecord(), std::string(":00000001FF"));

    ASSERT_EQ(record->getDataSize(), 0);
    ASSERT_EQ(record->getAddress(), 0);
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_END_OF_FILE);
    ASSERT_EQ(record->getChecksum(), 0xFF);

    // RECORD_EXTENDED_SEGMENT_ADDRESS
    ASSERT_TRUE(record->setRecord(":025C6D027E8F26"));
    ASSERT_EQ(record->getRecord(), std::string(":025C6D027E8F26"));

    ASSERT_EQ(record->getDataSize(), 2);
    ASSERT_EQ(record->getAddress(), 0x5C6D);
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(record->getData(0), 0x7E);
    ASSERT_EQ(record->getData(1), 0x8F);
    ASSERT_EQ(record->getExtendedSegmentAddress(), 0x7E8F);
    ASSERT_EQ(record->getChecksum(), 0x26);

    // RECORD_START_SEGMENT_ADDRESS
    ASSERT_TRUE(record->setRecord(":0400000390A1B2C353"));
    ASSERT_EQ(record->getRecord(), std::string(":0400000390A1B2C353"));

    ASSERT_EQ(record->getDataSize(), 4);
    ASSERT_EQ(record->getAddress(), 0);
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_START_SEGMENT_ADDRESS);
    ASSERT_EQ(record->getData(0), 0x90);
    ASSERT_EQ(record->getData(1), 0xA1);
    ASSERT_EQ(record->getData(2), 0xB2);
    ASSERT_EQ(record->getData(3), 0xC3);
    ASSERT_EQ(record->getStartSegmentAddressCodeSegment(), 0x90A1);
    ASSERT_EQ(record->getStartSegmentAddressInstructionPointer(), 0xB2C3);
    ASSERT_EQ(record->getChecksum(), 0x53);

    // RECORD_EXTENDED_LINEAR_ADDRESS
    ASSERT_TRUE(record->setRecord(":02D4E504F61734"));
    ASSERT_EQ(record->getRecord(), std::string(":02D4E504F61734"));

    ASSERT_EQ(record->getDataSize(), 2);
    ASSERT_EQ(record->getAddress(), 0xD4E5);
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(record->getData(0), 0xF6);
    ASSERT_EQ(record->getData(1), 0x17);
    ASSERT_EQ(record->getExtendedLinearAddress(), 0xF617);
    ASSERT_EQ(record->getChecksum(), 0x34);

    // RECORD_START_LINEAR_ADDRESS
    ASSERT_TRUE(record->setRecord(":0400000528394A5BF1"));
    ASSERT_EQ(record->getRecord(), std::string(":0400000528394A5BF1"));

    ASSERT_EQ(record->getDataSize(), 4);
    ASSERT_EQ(record->getAddress(), 0);
    ASSERT_EQ(record->getRecordType(), HexRecordType::RECORD_START_LINEAR_ADDRESS);
    ASSERT_EQ(record->getData(0), 0x28);
    ASSERT_EQ(record->getData(1), 0x39);
    ASSERT_EQ(record->getData(2), 0x4A);
    ASSERT_EQ(record->getData(3), 0x5B);
    ASSERT_EQ(record->getStartLinearAddressExtendedInstructionPointer(), 0x28394A5B);
    ASSERT_EQ(record->getChecksum(), 0xF1);

    // Invalid record tests
    ASSERT_FALSE(record->setRecord(""));
    ASSERT_FALSE(record->setRecord("AABBCCDDEEFF"));
    ASSERT_FALSE(record->setRecord(":"));
    ASSERT_FALSE(record->setRecord(":04"));
    ASSERT_FALSE(record->setRecord(":02000000"));
    ASSERT_FALSE(record->setRecord(":020000001A"));
    ASSERT_FALSE(record->setRecord(":020000001A2B"));
}

void HexRecordTest::performChecksumTests()
{
    SCOPED_TRACE(__func__);

    // Invalid checksum
    uint8_t data{0x4B};
    record->prepareRecord(0x1234, HexRecordType::RECORD_DATA, &data, 1, 0x5A);

    ASSERT_EQ(record->getChecksum(), 0x5A);
    ASSERT_FALSE(record->isValidChecksum());
    ASSERT_EQ(record->calculateChecksum(), 0x6E);
    record->updateChecksum();
    ASSERT_EQ(record->getChecksum(), 0x6E);
    ASSERT_TRUE(record->isValidChecksum());

    // Valid checksum
    record->prepareRecord(0x5678, HexRecordType::RECORD_DATA, &data, 1, 0xE6);

    ASSERT_EQ(record->getChecksum(), 0xE6);
    ASSERT_TRUE(record->isValidChecksum());
    ASSERT_EQ(record->calculateChecksum(), 0xE6);
    record->updateChecksum();
    ASSERT_EQ(record->getChecksum(), 0xE6);
    ASSERT_TRUE(record->isValidChecksum());
}

void HexRecordTest::performHasAddressFunctionTests()
{
    SCOPED_TRACE(__func__);

    const unsigned short baseAddress{200};
    record->setRecordType(HexRecordType::RECORD_DATA);
    record->setAddress(baseAddress);

    // Data record test #1
    ASSERT_TRUE(record->setDataSize(16));

    ASSERT_TRUE(record->hasAddress());
    ASSERT_EQ(record->getLastAddress(), baseAddress + 15);

    ASSERT_FALSE(record->hasAddress(baseAddress - 1));
    ASSERT_TRUE(record->hasAddress(baseAddress));
    ASSERT_TRUE(record->hasAddress(baseAddress + 15));
    ASSERT_FALSE(record->hasAddress(baseAddress + 16));

    // Data record test #2
    ASSERT_TRUE(record->setDataSize(8));

    ASSERT_TRUE(record->hasAddress());
    ASSERT_EQ(record->getLastAddress(), baseAddress + 7);

    ASSERT_FALSE(record->hasAddress(baseAddress - 1));
    ASSERT_TRUE(record->hasAddress(baseAddress));
    ASSERT_TRUE(record->hasAddress(baseAddress + 7));
    ASSERT_FALSE(record->hasAddress(baseAddress + 8));

    // Data record test #3
    ASSERT_TRUE(record->setDataSize(0));

    ASSERT_FALSE(record->hasAddress());
    ASSERT_THROW(record->getLastAddress(), std::domain_error);

    ASSERT_THROW(record->hasAddress(baseAddress - 1), std::domain_error);
    ASSERT_THROW(record->hasAddress(baseAddress), std::domain_error);
    ASSERT_THROW(record->hasAddress(baseAddress + 1), std::domain_error);

    // End of file record test #1
    record->setRecordType(HexRecordType::RECORD_END_OF_FILE);
    ASSERT_TRUE(record->setDataSize(16));

    ASSERT_FALSE(record->hasAddress());
    ASSERT_THROW(record->getLastAddress(), std::domain_error);

    ASSERT_THROW(record->hasAddress(baseAddress - 1), std::domain_error);
    ASSERT_THROW(record->hasAddress(baseAddress), std::domain_error);
    ASSERT_THROW(record->hasAddress(baseAddress + 1), std::domain_error);
}

void HexRecordTest::performIsValidRecordTests()
{
    SCOPED_TRACE(__func__);

    // Test data
    uint8_t data[4];
    for (size_t index{0}; index < 4; ++index)
        data[index] = ((0x11 * index) + 2);

    // RECORD_DATA
    ASSERT_NO_THROW(record->prepareRecord(0x1223, HexRecordType::RECORD_DATA, &data[0], 4, 0x34));
    ASSERT_EQ(record->getRecord(), std::string(":041223000213243534"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_TRUE(record->isValidRecord());

    ASSERT_NO_THROW(record->prepareRecord(0x4556, HexRecordType::RECORD_DATA, nullptr, 0, 0x67));
    ASSERT_EQ(record->getRecord(), std::string(":0045560067"));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord());

    // RECORD_END_OF_FILE
    ASSERT_NO_THROW(record->prepareRecord(0x7889, HexRecordType::RECORD_END_OF_FILE, &data[0], 2, 0x9A));
    ASSERT_EQ(record->getRecord(), std::string(":0278890102139A"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord());

    ASSERT_NO_THROW(record->prepareRecord(0xABBC, HexRecordType::RECORD_END_OF_FILE, nullptr, 0, 0xCD));
    ASSERT_EQ(record->getRecord(), std::string(":00ABBC01CD"));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_TRUE(record->isValidRecord());

    // RECORD_EXTENDED_SEGMENT_ADDRESS
    ASSERT_NO_THROW(record->prepareRecord(0xDEEF, HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS, &data[0], 4, 0xF0));
    ASSERT_EQ(record->getRecord(), std::string(":04DEEF0202132435F0"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord());

    ASSERT_NO_THROW(record->prepareRecord(0x0112, HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS, &data[0], 2, 0x23));
    ASSERT_EQ(record->getRecord(), std::string(":02011202021323"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_TRUE(record->isValidRecord());

    // RECORD_START_SEGMENT_ADDRESS
    ASSERT_NO_THROW(record->prepareRecord(0x3445, HexRecordType::RECORD_START_SEGMENT_ADDRESS, &data[0], 4, 0x56));
    ASSERT_EQ(record->getRecord(), std::string(":043445030213243556"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord());

    ASSERT_NO_THROW(record->prepareRecord(0, HexRecordType::RECORD_START_SEGMENT_ADDRESS, &data[0], 4, 0x9A));
    ASSERT_EQ(record->getRecord(), std::string(":04000003021324359A"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_TRUE(record->isValidRecord());

    // RECORD_EXTENDED_LINEAR_ADDRESS
    ASSERT_NO_THROW(record->prepareRecord(0xABBC, HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS, &data[0], 4, 0xCD));
    ASSERT_EQ(record->getRecord(), std::string(":04ABBC0402132435CD"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord());

    ASSERT_NO_THROW(record->prepareRecord(0xDEEF, HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS, &data[0], 2, 0xF0));
    ASSERT_EQ(record->getRecord(), std::string(":02DEEF040213F0"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_TRUE(record->isValidRecord());

    // RECORD_START_LINEAR_ADDRESS
    ASSERT_NO_THROW(record->prepareRecord(0x0112, HexRecordType::RECORD_START_LINEAR_ADDRESS, &data[0], 4, 0x23));
    ASSERT_EQ(record->getRecord(), std::string(":040112050213243523"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_FALSE(record->isValidRecord());

    ASSERT_NO_THROW(record->prepareRecord(0, HexRecordType::RECORD_START_LINEAR_ADDRESS, &data[0], 4, 0x34));
    ASSERT_EQ(record->getRecord(), std::string(":040000050213243534"));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_DATA));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_END_OF_FILE));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_START_SEGMENT_ADDRESS));
    ASSERT_FALSE(record->isValidRecord(HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS));
    ASSERT_TRUE(record->isValidRecord(HexRecordType::RECORD_START_LINEAR_ADDRESS));
    ASSERT_TRUE(record->isValidRecord());

    // Invalid record
    ASSERT_FALSE(record->isValidRecord(static_cast<HexRecordType>(static_cast<uint8_t>(HexRecordType::RECORD_MIN) - 1)));
    ASSERT_FALSE(record->isValidRecord(static_cast<HexRecordType>(static_cast<uint8_t>(HexRecordType::RECORD_MAX) + 1)));
}

void HexRecordTest::performExtendedSegmentAddressFunctionTests()
{
    SCOPED_TRACE(__func__);

    // Make a RECORD_DATA
    const uint8_t data[2]{0, 0};
    ASSERT_NO_THROW(record->prepareDataRecord(200, &data[0], 2));

    ASSERT_THROW(record->getExtendedSegmentAddress(), std::domain_error);
    ASSERT_FALSE(record->setExtendedSegmentAddress(0x1E2D));
    ASSERT_THROW(record->getExtendedSegmentAddress(), std::domain_error);

    // Make a valid RECORD_EXTENDED_SEGMENT_ADDRESS
    record->prepareExtendedSegmentAddressRecord(0x5C6D);

    ASSERT_EQ(record->getExtendedSegmentAddress(), 0x5C6D);
    ASSERT_TRUE(record->setExtendedSegmentAddress(0x1E2D));
    ASSERT_EQ(record->getExtendedSegmentAddress(), 0x1E2D);

    // Make an invalid RECORD_EXTENDED_SEGMENT_ADDRESS
    ASSERT_NO_THROW(record->prepareRecord(0, HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS, nullptr, 0));

    ASSERT_THROW(record->getExtendedSegmentAddress(), std::domain_error);
    ASSERT_FALSE(record->setExtendedSegmentAddress(0x1E2D));
    ASSERT_THROW(record->getExtendedSegmentAddress(), std::domain_error);
}

void HexRecordTest::performStartSegmentAddressFunctionTests()
{
    SCOPED_TRACE(__func__);

    // Make a RECORD_DATA
    const uint8_t data[2]{0, 0};
    ASSERT_NO_THROW(record->prepareDataRecord(200, &data[0], 2));

    ASSERT_THROW(record->getStartSegmentAddressCodeSegment(), std::domain_error);
    ASSERT_FALSE(record->setStartSegmentAddressCodeSegment(0x1E2D));
    ASSERT_THROW(record->getStartSegmentAddressCodeSegment(), std::domain_error);

    ASSERT_THROW(record->getStartSegmentAddressInstructionPointer(), std::domain_error);
    ASSERT_FALSE(record->setStartSegmentAddressInstructionPointer(0x3C4B));
    ASSERT_THROW(record->getStartSegmentAddressInstructionPointer(), std::domain_error);

    // Make a valid RECORD_START_SEGMENT_ADDRESS
    record->prepareStartSegmentAddressRecord(0x7E8F, 0x90A1);

    ASSERT_EQ(record->getStartSegmentAddressCodeSegment(), 0x7E8F);
    ASSERT_TRUE(record->setStartSegmentAddressCodeSegment(0x1E2D));
    ASSERT_EQ(record->getStartSegmentAddressCodeSegment(), 0x1E2D);

    ASSERT_EQ(record->getStartSegmentAddressInstructionPointer(), 0x90A1);
    ASSERT_TRUE(record->setStartSegmentAddressInstructionPointer(0x3C4B));
    ASSERT_EQ(record->getStartSegmentAddressInstructionPointer(), 0x3C4B);

    // Make an invalid RECORD_START_SEGMENT_ADDRESS
    ASSERT_NO_THROW(record->prepareRecord(0, HexRecordType::RECORD_START_SEGMENT_ADDRESS, nullptr, 0));

    ASSERT_THROW(record->getStartSegmentAddressCodeSegment(), std::domain_error);
    ASSERT_FALSE(record->setStartSegmentAddressCodeSegment(0x1E2D));
    ASSERT_THROW(record->getStartSegmentAddressCodeSegment(), std::domain_error);

    ASSERT_THROW(record->getStartSegmentAddressInstructionPointer(), std::domain_error);
    ASSERT_FALSE(record->setStartSegmentAddressInstructionPointer(0x3C4B));
    ASSERT_THROW(record->getStartSegmentAddressInstructionPointer(), std::domain_error);
}

void HexRecordTest::performExtendedLinearAddressFunctionTests()
{
    SCOPED_TRACE(__func__);

    // Make a RECORD_DATA
    const uint8_t data[2]{0, 0};
    ASSERT_NO_THROW(record->prepareDataRecord(200, &data[0], 2));

    ASSERT_THROW(record->getExtendedLinearAddress(), std::domain_error);
    ASSERT_FALSE(record->setExtendedLinearAddress(0x1E2D));
    ASSERT_THROW(record->getExtendedLinearAddress(), std::domain_error);

    // Make a valid RECORD_EXTENDED_LINEAR_ADDRESS
    record->prepareExtendedLinearAddressRecord(0xB2C3);

    ASSERT_EQ(record->getExtendedLinearAddress(), 0xB2C3);
    ASSERT_TRUE(record->setExtendedLinearAddress(0x1E2D));
    ASSERT_EQ(record->getExtendedLinearAddress(), 0x1E2D);

    // Make an invalid RECORD_EXTENDED_LINEAR_ADDRESS
    ASSERT_NO_THROW(record->prepareRecord(0, HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS, nullptr, 0));

    ASSERT_THROW(record->getExtendedLinearAddress(), std::domain_error);
    ASSERT_FALSE(record->setExtendedLinearAddress(0x1E2D));
    ASSERT_THROW(record->getExtendedLinearAddress(), std::domain_error);
}

void HexRecordTest::performStartLinearAddressFunctionTests()
{
    SCOPED_TRACE(__func__);

    // Make a RECORD_DATA
    const uint8_t data[2]{0, 0};
    ASSERT_NO_THROW(record->prepareDataRecord(200, &data[0], 2));

    ASSERT_THROW(record->getStartLinearAddressExtendedInstructionPointer(), std::domain_error);
    ASSERT_FALSE(record->setStartLinearAddressExtendedInstructionPointer(0x1E2D3C4B));
    ASSERT_THROW(record->getStartLinearAddressExtendedInstructionPointer(), std::domain_error);

    // Make a valid RECORD_START_LINEAR_ADDRESS
    record->prepareStartLinearAddressRecord(0xD4E5F607);

    ASSERT_EQ(record->getStartLinearAddressExtendedInstructionPointer(), 0xD4E5F607);
    ASSERT_TRUE(record->setStartLinearAddressExtendedInstructionPointer(0x1E2D3C4B));
    ASSERT_EQ(record->getStartLinearAddressExtendedInstructionPointer(), 0x1E2D3C4B);

    // Make an invalid RECORD_START_LINEAR_ADDRESS
    ASSERT_NO_THROW(record->prepareRecord(0, HexRecordType::RECORD_START_LINEAR_ADDRESS, nullptr, 0));

    ASSERT_THROW(record->getStartLinearAddressExtendedInstructionPointer(), std::domain_error);
    ASSERT_FALSE(record->setStartLinearAddressExtendedInstructionPointer(0x1E2D3C4B));
    ASSERT_THROW(record->getStartLinearAddressExtendedInstructionPointer(), std::domain_error);
}

END_NAMESPACE_LIBIHEX
