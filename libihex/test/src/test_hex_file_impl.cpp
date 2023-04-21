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

#include <ihex_test/test_hex_file_assets.hpp>
#include <ihex_test/test_hex_file_impl.hpp>

BEGIN_NAMESPACE_LIBIHEX

HexFileTest::HexFileTest()
{

}

HexFileTest::~HexFileTest()
{

}

void HexFileTest::SetUp()
{
    Test::SetUp();
    file = std::make_shared<HexFile>();
}

void HexFileTest::TearDown()
{
    file.reset();
    Test::TearDown();
}

void HexFileTest::performConstructorTests()
{
    SCOPED_TRACE(__func__);

    // Default initialized file
    HexFile file1{};
    ASSERT_EQ(file1.getSize(), 0);
    ASSERT_TRUE(file1.getThrowOnInvalidRecord());
    ASSERT_TRUE(file1.getThrowOnChecksumMismatch());
}

void HexFileTest::performLoadStreamTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    const HexRecord dataRecord1{":04000000D8BCDA7816"};
    const HexRecord dataRecord2{":06700000505D6C94AFA589"};
    const HexRecord dataRecord3{":08FFF8005B1BD22C54046FDBEB"};
    const HexRecord endOfFileRecord{HexRecordType::RECORD_END_OF_FILE};
    const HexRecord extendedSegmentAddressRecord{HexRecord::makeExtendedSegmentAddressRecord(0x1000)};
    const HexRecord startSegmentAddressRecord{HexRecord::makeStartSegmentAddressRecord(0x1122, 0x3344)};
    const HexRecord extendedLinearAddressRecord{HexRecord::makeExtendedLinearAddressRecord(0x2000)};
    const HexRecord startLinearAddressRecord{HexRecord::makeStartLinearAddressRecord(0x55667788)};
    const std::string invalidRecord{":xyz"};
    HexRecord checksumMismatchRecord{};

    // ADDRESS_I8HEX (good stream)
    std::stringstream stream{};
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord3.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_TRUE(file->load(stream));
    ASSERT_EQ(file->getSize(), 2);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(file->getSection(0).getSize(), 3);
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord1.getAddress() + index), dataRecord1[index]);
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord2.getAddress() + index), dataRecord2[index]);
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord3.getAddress() + index), dataRecord3[index]);

    // ADDRESS_I8HEX (invalid record, exceptions disabled)
    stream.str("");
    stream.clear();
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << invalidRecord << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord3.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_FALSE(file->load(stream));
    ASSERT_EQ(file->getSize(), 2);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(file->getSection(0).getSize(), 2);
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord1.getAddress() + index), dataRecord1[index]);
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord2.getAddress() + index), file->getUnusedDataFillValue());
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord3.getAddress() + index), dataRecord3[index]);

    // ADDRESS_I8HEX (invalid record, exceptions enabled)
    stream.str("");
    stream.clear();
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << invalidRecord << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(true);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_THROW(file->load(stream), std::runtime_error);
    ASSERT_EQ(file->getSize(), 1);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(file->getSection(0).getSize(), 2);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord1.getAddress() + index), dataRecord1[index]);
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord2.getAddress() + index), dataRecord2[index]);
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord3.getAddress() + index), file->getUnusedDataFillValue());

    // ADDRESS_I8HEX (checksum mismatch, exceptions disabled)
    checksumMismatchRecord = dataRecord3;
    checksumMismatchRecord.setChecksum(checksumMismatchRecord.getChecksum() ^ 0x5A);

    stream.str("");
    stream.clear();
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << checksumMismatchRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_FALSE(file->load(stream));
    ASSERT_EQ(file->getSize(), 2);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(file->getSection(0).getSize(), 2);
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord1.getAddress() + index), dataRecord1[index]);
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord2.getAddress() + index), dataRecord2[index]);
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord3.getAddress() + index), file->getUnusedDataFillValue());

    // ADDRESS_I8HEX (checksum mismatch, exceptions enabled)
    stream.str("");
    stream.clear();
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << checksumMismatchRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(true);
    ASSERT_THROW(file->load(stream), std::logic_error);
    ASSERT_EQ(file->getSize(), 1);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_DATA);
    ASSERT_EQ(file->getSection(0).getSize(), 2);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord1.getAddress() + index), dataRecord1[index]);
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord2.getAddress() + index), dataRecord2[index]);
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
        ASSERT_EQ(file->getData(dataRecord3.getAddress() + index), file->getUnusedDataFillValue());

    // ADDRESS_I16HEX (good stream)
    stream.str("");
    stream.clear();
    stream << startSegmentAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << extendedSegmentAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord3.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_TRUE(file->load(stream));
    ASSERT_EQ(file->getSize(), 3);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(file->getCodeSegment(), startSegmentAddressRecord.getStartSegmentAddressCodeSegment());
    ASSERT_EQ(file->getInstructionPointer(), startSegmentAddressRecord.getStartSegmentAddressInstructionPointer());
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(file->getSection(1).getSize(), 4);
    ASSERT_EQ(file->getSection(1).getExtendedSegmentAddress(), extendedSegmentAddressRecord.getExtendedSegmentAddress());
    ASSERT_EQ(file->getSection(2).getSectionType(), HexSectionType::SECTION_END_OF_FILE);


    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord1.getAddress() + index), dataRecord1[index]);
    }
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord2.getAddress() + index), dataRecord2[index]);
    }
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord3.getAddress() + index), dataRecord3[index]);
    }

    // ADDRESS_I16HEX (invalid record, exceptions disabled)
    stream.str("");
    stream.clear();
    stream << extendedSegmentAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << invalidRecord << HEX_RECORD_LINE_SEPARATOR;
    stream << startSegmentAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_FALSE(file->load(stream));
    ASSERT_EQ(file->getSize(), 3);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(file->getSection(0).getSize(), 3);
    ASSERT_EQ(file->getSection(0).getExtendedSegmentAddress(), extendedSegmentAddressRecord.getExtendedSegmentAddress());
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(file->getCodeSegment(), startSegmentAddressRecord.getStartSegmentAddressCodeSegment());
    ASSERT_EQ(file->getInstructionPointer(), startSegmentAddressRecord.getStartSegmentAddressInstructionPointer());
    ASSERT_EQ(file->getSection(2).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord1.getAddress() + index), dataRecord1[index]);
    }
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord2.getAddress() + index), dataRecord2[index]);
    }
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord3.getAddress() + index), file->getUnusedDataFillValue());
    }

    // ADDRESS_I16HEX (invalid record, exceptions enabled)
    stream.str("");
    stream.clear();
    stream << startSegmentAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << extendedSegmentAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << invalidRecord << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord3.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(true);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_THROW(file->load(stream), std::runtime_error);
    ASSERT_EQ(file->getSize(), 2);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(file->getCodeSegment(), startSegmentAddressRecord.getStartSegmentAddressCodeSegment());
    ASSERT_EQ(file->getInstructionPointer(), startSegmentAddressRecord.getStartSegmentAddressInstructionPointer());
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(file->getSection(1).getSize(), 2);
    ASSERT_EQ(file->getSection(1).getExtendedSegmentAddress(), extendedSegmentAddressRecord.getExtendedSegmentAddress());

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord1.getAddress() + index), dataRecord1[index]);
    }
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord2.getAddress() + index), file->getUnusedDataFillValue());
    }
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord3.getAddress() + index), file->getUnusedDataFillValue());
    }

    // ADDRESS_I16HEX (checksum mismatch, exceptions disabled)
    checksumMismatchRecord = dataRecord2;
    checksumMismatchRecord.setChecksum(checksumMismatchRecord.getChecksum() ^ 0xA5);

    stream.str("");
    stream.clear();
    stream << extendedSegmentAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << checksumMismatchRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord3.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << startSegmentAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_FALSE(file->load(stream));
    ASSERT_EQ(file->getSize(), 3);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(file->getSection(0).getExtendedSegmentAddress(), extendedSegmentAddressRecord.getExtendedSegmentAddress());
    ASSERT_EQ(file->getSection(0).getSize(), 3);
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(file->getCodeSegment(), startSegmentAddressRecord.getStartSegmentAddressCodeSegment());
    ASSERT_EQ(file->getInstructionPointer(), startSegmentAddressRecord.getStartSegmentAddressInstructionPointer());
    ASSERT_EQ(file->getSection(2).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord1.getAddress() + index), dataRecord1[index]);
    }
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord2.getAddress() + index), file->getUnusedDataFillValue());
    }
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord3.getAddress() + index), dataRecord3[index]);
    }

    // ADDRESS_I16HEX (checksum mismatch, exceptions enabled)
    stream.str("");
    stream.clear();
    stream << startSegmentAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << extendedSegmentAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << checksumMismatchRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord3.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(true);
    ASSERT_THROW(file->load(stream), std::logic_error);
    ASSERT_EQ(file->getSize(), 2);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_START_SEGMENT_ADDRESS);
    ASSERT_EQ(file->getCodeSegment(), startSegmentAddressRecord.getStartSegmentAddressCodeSegment());
    ASSERT_EQ(file->getInstructionPointer(), startSegmentAddressRecord.getStartSegmentAddressInstructionPointer());
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS);
    ASSERT_EQ(file->getSection(1).getSize(), 2);
    ASSERT_EQ(file->getSection(1).getExtendedSegmentAddress(), extendedSegmentAddressRecord.getExtendedSegmentAddress());

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord1.getAddress() + index), dataRecord1[index]);
    }
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord2.getAddress() + index), file->getUnusedDataFillValue());
    }
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseSegmentAddress(
            extendedSegmentAddressRecord.getExtendedSegmentAddress()) +
            dataRecord3.getAddress() + index), file->getUnusedDataFillValue());
    }

    // ADDRESS_I32HEX (good stream)
    stream.str("");
    stream.clear();
    stream << extendedLinearAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord3.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << startLinearAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_TRUE(file->load(stream));
    ASSERT_EQ(file->getSize(), 3);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(file->getSection(0).getSize(), 4);
    ASSERT_EQ(file->getSection(0).getExtendedLinearAddress(), extendedLinearAddressRecord.getExtendedLinearAddress());
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(file->getExtendedInstructionPointer(), startLinearAddressRecord.getStartLinearAddressExtendedInstructionPointer());
    ASSERT_EQ(file->getSection(2).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord1.getAddress() + index), dataRecord1[index]);
    }
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord2.getAddress() + index), dataRecord2[index]);
    }
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord3.getAddress() + index), dataRecord3[index]);
    }

    // ADDRESS_I32HEX (invalid record, exceptions disabled)
    stream.str("");
    stream.clear();
    stream << extendedLinearAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << invalidRecord << HEX_RECORD_LINE_SEPARATOR;
    stream << startLinearAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_FALSE(file->load(stream));
    ASSERT_EQ(file->getSize(), 3);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(file->getSection(0).getSize(), 3);
    ASSERT_EQ(file->getSection(0).getExtendedLinearAddress(), extendedLinearAddressRecord.getExtendedLinearAddress());
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(file->getExtendedInstructionPointer(), startLinearAddressRecord.getStartLinearAddressExtendedInstructionPointer());
    ASSERT_EQ(file->getSection(2).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord1.getAddress() + index), dataRecord1[index]);
    }
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord2.getAddress() + index), dataRecord2[index]);
    }
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord3.getAddress() + index), file->getUnusedDataFillValue());
    }

    // ADDRESS_I32HEX (invalid record, exceptions enabled)
    stream.str("");
    stream.clear();
    stream << extendedLinearAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << invalidRecord << HEX_RECORD_LINE_SEPARATOR;
    stream << startLinearAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(true);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_THROW(file->load(stream), std::runtime_error);
    ASSERT_EQ(file->getSize(), 1);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(file->getSection(0).getSize(), 3);
    ASSERT_EQ(file->getSection(0).getExtendedLinearAddress(), extendedLinearAddressRecord.getExtendedLinearAddress());
    ASSERT_THROW(file->getExtendedInstructionPointer(), std::runtime_error);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord1.getAddress() + index), dataRecord1[index]);
    }
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord2.getAddress() + index), dataRecord2[index]);
    }
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord3.getAddress() + index), file->getUnusedDataFillValue());
    }

    // ADDRESS_I32HEX (checksum mismatch, exceptions disabled)
    checksumMismatchRecord = dataRecord3;
    checksumMismatchRecord.setChecksum(checksumMismatchRecord.getChecksum() ^ 0x5A);

    stream.str("");
    stream.clear();
    stream << extendedLinearAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << checksumMismatchRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << startLinearAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(false);
    ASSERT_FALSE(file->load(stream));
    ASSERT_EQ(file->getSize(), 3);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(file->getSection(0).getSize(), 3);
    ASSERT_EQ(file->getSection(0).getExtendedLinearAddress(), extendedLinearAddressRecord.getExtendedLinearAddress());
    ASSERT_EQ(file->getSection(1).getSectionType(), HexSectionType::SECTION_START_LINEAR_ADDRESS);
    ASSERT_EQ(file->getExtendedInstructionPointer(), startLinearAddressRecord.getStartLinearAddressExtendedInstructionPointer());
    ASSERT_EQ(file->getSection(2).getSectionType(), HexSectionType::SECTION_END_OF_FILE);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord1.getAddress() + index), dataRecord1[index]);
    }
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord2.getAddress() + index), dataRecord2[index]);
    }
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord3.getAddress() + index), file->getUnusedDataFillValue());
    }

    // ADDRESS_I32HEX (checksum mismatch, exceptions enabled)
    stream.str("");
    stream.clear();
    stream << extendedLinearAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord1.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << dataRecord2.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << checksumMismatchRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << startLinearAddressRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;
    stream << endOfFileRecord.getRecord() << HEX_RECORD_LINE_SEPARATOR;

    file->clearSections();
    file->setThrowOnInvalidRecord(false);
    file->setThrowOnChecksumMismatch(true);
    ASSERT_THROW(file->load(stream), std::logic_error);
    ASSERT_EQ(file->getSize(), 1);
    ASSERT_EQ(file->getSection(0).getSectionType(), HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS);
    ASSERT_EQ(file->getSection(0).getSize(), 3);
    ASSERT_EQ(file->getSection(0).getExtendedLinearAddress(), extendedLinearAddressRecord.getExtendedLinearAddress());
    ASSERT_THROW(file->getExtendedInstructionPointer(), std::runtime_error);

    for (size_t index{0}; index < dataRecord1.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord1.getAddress() + index), dataRecord1[index]);
    }
    for (size_t index{0}; index < dataRecord2.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord2.getAddress() + index), dataRecord2[index]);
    }
    for (size_t index{0}; index < dataRecord3.getDataSize(); ++index)
    {
        ASSERT_EQ(file->getData(HexAddress::getBaseLinearAddress(
            extendedLinearAddressRecord.getExtendedLinearAddress()) +
            dataRecord3.getAddress() + index), file->getUnusedDataFillValue());
    }
}

void HexFileTest::performSaveStreamTests()
{
    SCOPED_TRACE(__func__);

    // Initial data
    HexSection dataSection{HexSectionType::SECTION_DATA};
    ASSERT_TRUE(dataSection.pushRecord(":100000004E796C1E0A3E190C08221703534FF3DD7C"));
    ASSERT_TRUE(dataSection.pushRecord(":10001000AF45C803BFA089DBE880F192566F14B2E8"));
    ASSERT_TRUE(dataSection.pushRecord(":10700000F2F4BF54772195D32A5D8D92C853F112C3"));
    ASSERT_TRUE(dataSection.pushRecord(":107010008C9E10425B34C59F876D190510A6B85031"));
    ASSERT_TRUE(dataSection.pushRecord(":107FF0005885F040BD546FF44FE940474C78041069"));
    std::stringstream stream;
    std::string record;
    std::vector<std::string> records;

    // Empty
    file->save(stream);
    ASSERT_EQ(stream.str(), "");

    // ADDRESS_I8HEX
    ASSERT_EQ(file->pushSection(HexSection::makeEndOfFileSection()), 0);
    ASSERT_EQ(file->pushSection(dataSection), 0);

    ASSERT_TRUE(file->save(stream));

    while (std::getline(stream, record, static_cast<char>(HEX_RECORD_LINE_SEPARATOR)))
        records.push_back(record);

    ASSERT_EQ(records.size(), 6);
    ASSERT_EQ(records.at(0), ":100000004E796C1E0A3E190C08221703534FF3DD7C");
    ASSERT_EQ(records.at(1), ":10001000AF45C803BFA089DBE880F192566F14B2E8");
    ASSERT_EQ(records.at(2), ":10700000F2F4BF54772195D32A5D8D92C853F112C3");
    ASSERT_EQ(records.at(3), ":107010008C9E10425B34C59F876D190510A6B85031");
    ASSERT_EQ(records.at(4), ":107FF0005885F040BD546FF44FE940474C78041069");
    ASSERT_EQ(records.at(5), ":00000001FF");

    // ADDRESS_I16HEX
    file->clearSections();
    ASSERT_EQ(file->pushSection(HexSection::makeEndOfFileSection()), 0);
    dataSection.convertToExtendedSegmentAddressSection(0x1234);
    ASSERT_EQ(file->pushSection(dataSection), 0);
    ASSERT_EQ(file->pushSection(HexSection::makeStartSegmentAddressSection(0xFEDC, 0xBA98)), 1);
    dataSection.convertToExtendedSegmentAddressSection(0x2345);
    ASSERT_EQ(file->pushSection(dataSection), 2);

    stream.str("");
    stream.clear();
    ASSERT_TRUE(file->save(stream));

    records.clear();
    while (std::getline(stream, record, static_cast<char>(HEX_RECORD_LINE_SEPARATOR)))
        records.push_back(record);

    ASSERT_EQ(records.size(), 14);
    ASSERT_EQ(records.at(0), ":020000021234B6");
    ASSERT_EQ(records.at(1), ":100000004E796C1E0A3E190C08221703534FF3DD7C");
    ASSERT_EQ(records.at(2), ":10001000AF45C803BFA089DBE880F192566F14B2E8");
    ASSERT_EQ(records.at(3), ":10700000F2F4BF54772195D32A5D8D92C853F112C3");
    ASSERT_EQ(records.at(4), ":107010008C9E10425B34C59F876D190510A6B85031");
    ASSERT_EQ(records.at(5), ":107FF0005885F040BD546FF44FE940474C78041069");
    ASSERT_EQ(records.at(6), ":04000003FEDCBA98CD");
    ASSERT_EQ(records.at(7), ":02000002234594");
    ASSERT_EQ(records.at(8), ":100000004E796C1E0A3E190C08221703534FF3DD7C");
    ASSERT_EQ(records.at(9), ":10001000AF45C803BFA089DBE880F192566F14B2E8");
    ASSERT_EQ(records.at(10), ":10700000F2F4BF54772195D32A5D8D92C853F112C3");
    ASSERT_EQ(records.at(11), ":107010008C9E10425B34C59F876D190510A6B85031");
    ASSERT_EQ(records.at(12), ":107FF0005885F040BD546FF44FE940474C78041069");
    ASSERT_EQ(records.at(13), ":00000001FF");

    // ADDRESS_I32HEX
    file->clearSections();
    ASSERT_EQ(file->pushSection(HexSection::makeEndOfFileSection()), 0);
    ASSERT_EQ(file->pushSection(HexSection::makeStartLinearAddressSection(0x76543210)), 0);
    dataSection.convertToExtendedLinearAddressSection(0x1234);
    ASSERT_EQ(file->pushSection(dataSection), 1);
    dataSection.convertToExtendedLinearAddressSection(0x2345);
    ASSERT_EQ(file->pushSection(dataSection), 2);

    stream.str("");
    stream.clear();
    ASSERT_TRUE(file->save(stream));

    records.clear();
    while (std::getline(stream, record, static_cast<char>(HEX_RECORD_LINE_SEPARATOR)))
        records.push_back(record);

    ASSERT_EQ(records.size(), 14);
    ASSERT_EQ(records.at(0), ":0400000576543210EB");
    ASSERT_EQ(records.at(1), ":020000041234B4");
    ASSERT_EQ(records.at(2), ":100000004E796C1E0A3E190C08221703534FF3DD7C");
    ASSERT_EQ(records.at(3), ":10001000AF45C803BFA089DBE880F192566F14B2E8");
    ASSERT_EQ(records.at(4), ":10700000F2F4BF54772195D32A5D8D92C853F112C3");
    ASSERT_EQ(records.at(5), ":107010008C9E10425B34C59F876D190510A6B85031");
    ASSERT_EQ(records.at(6), ":107FF0005885F040BD546FF44FE940474C78041069");
    ASSERT_EQ(records.at(7), ":02000004234592");
    ASSERT_EQ(records.at(8), ":100000004E796C1E0A3E190C08221703534FF3DD7C");
    ASSERT_EQ(records.at(9), ":10001000AF45C803BFA089DBE880F192566F14B2E8");
    ASSERT_EQ(records.at(10), ":10700000F2F4BF54772195D32A5D8D92C853F112C3");
    ASSERT_EQ(records.at(11), ":107010008C9E10425B34C59F876D190510A6B85031");
    ASSERT_EQ(records.at(12), ":107FF0005885F040BD546FF44FE940474C78041069");
    ASSERT_EQ(records.at(13), ":00000001FF");
}

void HexFileTest::performLoadSaveFileTests()
{
    SCOPED_TRACE(__func__);

    // Sample 1
    ASSERT_TRUE(file->load(std::string(TEST_HEX_FILE_ASSET_PATH) + "/sample1.hex"));
    ASSERT_TRUE(file->save(std::string(TEST_HEX_FILE_ASSET_PATH) + "/sample1_out.hex"));
    file->clearSections();

    // Sample 2
    ASSERT_TRUE(file->load(std::string(TEST_HEX_FILE_ASSET_PATH) + "/sample2.hex"));
    ASSERT_TRUE(file->save(std::string(TEST_HEX_FILE_ASSET_PATH) + "/sample2_out.hex"));
    file->clearSections();

    // Invalid sample
    ASSERT_FALSE(file->load(std::string(TEST_HEX_FILE_ASSET_PATH) + "/invalid_sample.hex"));
    ASSERT_TRUE(file->save(std::string(TEST_HEX_FILE_ASSET_PATH) + "/invalid_sample_out.hex"));
}

void HexFileTest::performPropertiesTests()
{
    SCOPED_TRACE(__func__);

    file->setThrowOnInvalidRecord(false);
    ASSERT_FALSE(file->getThrowOnInvalidRecord());

    file->setThrowOnInvalidRecord(true);
    ASSERT_TRUE(file->getThrowOnInvalidRecord());

    file->setThrowOnChecksumMismatch(false);
    ASSERT_FALSE(file->getThrowOnChecksumMismatch());

    file->setThrowOnChecksumMismatch(true);
    ASSERT_TRUE(file->getThrowOnChecksumMismatch());
}

END_NAMESPACE_LIBIHEX
