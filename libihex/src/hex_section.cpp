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

#include <algorithm>
#include <stdexcept>
#include <string>
#include <ihex/hex_address.hpp>
#include <ihex/hex_section.hpp>

BEGIN_NAMESPACE_LIBIHEX

HexSection::HexSection() :
    section{}, blocks{}, unusedDataFillValue{DEFAULT_HEX_RECORD_DATA_VALUE}, defaultDataSize{DEFAULT_HEX_RECORD_DATA_SIZE}
{

}

HexSection::HexSection(const std::string& record) :
    section{}, blocks{}, unusedDataFillValue{DEFAULT_HEX_RECORD_DATA_VALUE}, defaultDataSize{DEFAULT_HEX_RECORD_DATA_SIZE}
{
    // Parse initial record
    HexRecord initial{record};

    // Push the record
    pushRecord(initial);
}

HexSection::HexSection(const HexRecord& record) :
    section{}, blocks{}, unusedDataFillValue{DEFAULT_HEX_RECORD_DATA_VALUE}, defaultDataSize{DEFAULT_HEX_RECORD_DATA_SIZE}
{
    // Check record checksum
    if (!record.isValidChecksum())
        throw std::logic_error("Record checksum is not valid");

    // Push the record
    pushRecord(record);
}

HexSection::HexSection(HexSectionType sectionType) :
    section{}, blocks{}, unusedDataFillValue{DEFAULT_HEX_RECORD_DATA_VALUE}, defaultDataSize{DEFAULT_HEX_RECORD_DATA_SIZE}
{
    // Configure HexSection depending on the section type
    switch (sectionType)
    {
        case HexSectionType::SECTION_END_OF_FILE:
            convertToEndOfFileSection();
            break;

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            convertToExtendedSegmentAddressSection(0);
            break;

        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
            convertToStartSegmentAddressSection(0, 0);
            break;

        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            convertToExtendedLinearAddressSection(0);
            break;

        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
            convertToStartLinearAddressSection(0);
            break;

        case HexSectionType::SECTION_DATA:
        default:
            convertToDataSection();
            break;
    }
}

HexSection::HexSection(const HexSection& section) :
    section{section.section},
    blocks{section.blocks},
    unusedDataFillValue{section.unusedDataFillValue},
    defaultDataSize{section.defaultDataSize}
{

}

HexDataType HexSection::operator[](HexDataAddressType address) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            for (auto it{blocks.crbegin()}; it != blocks.crend(); ++it)
            {
                if (it->first <= address)
                {
                    // The condition above guarantees the difference will always be positive
                    const auto index{static_cast<size_t>(address - it->first)};
                    if (index < it->second.size())
                        return it->second[index];

                    // Address is below the address of the block
                    break;
                }
            }

            throw std::out_of_range("Address does not exist");
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexDataType& HexSection::operator[](HexDataAddressType address)
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            for (auto it{blocks.rbegin()}; it != blocks.rend(); ++it)
            {
                if (it->first <= address)
                {
                    // The condition above guarantees the difference will always be positive
                    const auto index{static_cast<size_t>(address - it->first)};
                    if (index < it->second.size())
                        return it->second[index];

                    // Address is below the address of the block
                    break;
                }
            }

            throw std::out_of_range("Address does not exist");
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexSection HexSection::makeDataSection(HexDataAddressType address, const HexDataType* data, HexDataSizeType dataSize)
{
    HexSection result{};
    result.setData(address, data, dataSize);
    return result;
}

HexSection HexSection::makeDataSection(HexDataAddressType address, const HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset)
{
    HexSection result{};
    result.setData(address, data, dataSize, offset);
    return result;
}

HexSection HexSection::makeEndOfFileSection()
{
    return HexSection{HexSectionType::SECTION_END_OF_FILE};
}

HexSection HexSection::makeExtendedSegmentAddressSection(uint16_t segmentAddress)
{
    HexSection result{HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS};
    result.setExtendedSegmentAddress(segmentAddress);
    return result;
}

HexSection HexSection::makeExtendedSegmentAddressSection(uint16_t segmentAddress, HexDataAddressType address,
    const HexDataType* data, HexDataSizeType dataSize)
{
    HexSection result{HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS};
    result.setExtendedSegmentAddress(segmentAddress);
    result.setData(address, data, dataSize);
    return result;
}

HexSection HexSection::makeExtendedSegmentAddressSection(uint16_t segmentAddress, HexDataAddressType address,
    const HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset)
{
    HexSection result{HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS};
    result.setExtendedSegmentAddress(segmentAddress);
    result.setData(address, data, dataSize, offset);
    return result;
}

HexSection HexSection::makeExtendedSegmentAddressSection(uint16_t segmentAddress, const std::string& dataRecord)
{
    HexSection result{HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS};
    result.setExtendedSegmentAddress(segmentAddress);
    if (!result.pushRecord(dataRecord))
        throw std::logic_error("Failed to push record to the section");
    return result;
}

HexSection HexSection::makeExtendedSegmentAddressSection(uint16_t segmentAddress, const HexRecord& dataRecord)
{
    HexSection result{HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS};
    result.setExtendedSegmentAddress(segmentAddress);
    if (!result.pushRecord(dataRecord))
        throw std::logic_error("Failed to push record to the section");
    return result;
}

HexSection HexSection::makeStartSegmentAddressSection(uint16_t codeSegment, uint16_t instructionPointer)
{
    HexSection result{HexSectionType::SECTION_START_SEGMENT_ADDRESS};
    result.setStartSegmentAddressCodeSegment(codeSegment);
    result.setStartSegmentAddressInstructionPointer(instructionPointer);
    return result;
}

HexSection HexSection::makeExtendedLinearAddressSection(uint16_t linearAddress)
{
    HexSection result{HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS};
    result.setExtendedLinearAddress(linearAddress);
    return result;
}

HexSection HexSection::makeExtendedLinearAddressSection(uint16_t linearAddress, HexDataAddressType address,
    const HexDataType* data, HexDataSizeType dataSize)
{
    HexSection result{HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS};
    result.setExtendedLinearAddress(linearAddress);
    result.setData(address, data, dataSize);
    return result;
}

HexSection HexSection::makeExtendedLinearAddressSection(uint16_t linearAddress, HexDataAddressType address,
    const HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset)
{
    HexSection result{HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS};
    result.setExtendedLinearAddress(linearAddress);
    result.setData(address, data, dataSize, offset);
    return result;
}

HexSection HexSection::makeExtendedLinearAddressSection(uint16_t linearAddress, const std::string& dataRecord)
{
    HexSection result{HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS};
    result.setExtendedLinearAddress(linearAddress);
    if (!result.pushRecord(dataRecord))
        throw std::logic_error("Failed to push record to the section");
    return result;
}

HexSection HexSection::makeExtendedLinearAddressSection(uint16_t linearAddress, const HexRecord& dataRecord)
{
    HexSection result{HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS};
    result.setExtendedLinearAddress(linearAddress);
    if (!result.pushRecord(dataRecord))
        throw std::logic_error("Failed to push record to the section");
    return result;
}

HexSection HexSection::makeStartLinearAddressSection(uint32_t extendedInstructionPointer)
{
    HexSection result{HexSectionType::SECTION_START_LINEAR_ADDRESS};
    result.setStartLinearAddressExtendedInstructionPointer(extendedInstructionPointer);
    return result;
}

HexSectionType HexSection::getSectionType() const
{
    switch (section.getRecordType())
    {
        case HexRecordType::RECORD_END_OF_FILE:
            return HexSectionType::SECTION_END_OF_FILE;
            break;

        case HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS:
            return HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS;
            break;

        case HexRecordType::RECORD_START_SEGMENT_ADDRESS:
            return HexSectionType::SECTION_START_SEGMENT_ADDRESS;
            break;

        case HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS:
            return HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS;
            break;

        case HexRecordType::RECORD_START_LINEAR_ADDRESS:
            return HexSectionType::SECTION_START_LINEAR_ADDRESS;
            break;

        case HexRecordType::RECORD_DATA:
        default:
            return HexSectionType::SECTION_DATA;
            break;
    }
}

void HexSection::convertToDataSection()
{
    // Change to an unused dummy RECORD_DATA
    HexDataType data{0};
    section.prepareDataRecord(0, &data, 1);
}

void HexSection::convertToEndOfFileSection()
{
    // Change to a RECORD_END_OF_FILE and clear all data blocks
    section.prepareEndOfFileRecord();
    blocks.clear();
}

void HexSection::convertToExtendedSegmentAddressSection(uint16_t segmentAddress)
{
    // Change to a RECORD_EXTENDED_SEGMENT_ADDRESS
    section.prepareExtendedSegmentAddressRecord(segmentAddress);
}

void HexSection::convertToStartSegmentAddressSection(uint16_t codeSegment, uint16_t instructionPointer)
{
    // Change to a RECORD_START_SEGMENT_ADDRESS and clear all data blocks
    section.prepareStartSegmentAddressRecord(codeSegment, instructionPointer);
    blocks.clear();
}

void HexSection::convertToExtendedLinearAddressSection(uint16_t linearAddress)
{
    // Change to a RECORD_EXTENDED_LINEAR_ADDRESS
    section.prepareExtendedLinearAddressRecord(linearAddress);
}

void HexSection::convertToStartLinearAddressSection(uint32_t extendedInstructionPointer)
{
    // Change to a RECORD_START_LINEAR_ADDRESS and clear all data blocks
    section.prepareStartLinearAddressRecord(extendedInstructionPointer);
    blocks.clear();
}

uint16_t HexSection::getExtendedSegmentAddress() const
{
    return section.getExtendedSegmentAddress();
}

bool HexSection::setExtendedSegmentAddress(uint16_t segmentAddress)
{
    return section.setExtendedSegmentAddress(segmentAddress);
}

uint16_t HexSection::getStartSegmentAddressCodeSegment() const
{
    return section.getStartSegmentAddressCodeSegment();
}

bool HexSection::setStartSegmentAddressCodeSegment(uint16_t codeSegment)
{
    return section.setStartSegmentAddressCodeSegment(codeSegment);
}

uint16_t HexSection::getStartSegmentAddressInstructionPointer() const
{
    return section.getStartSegmentAddressInstructionPointer();
}

bool HexSection::setStartSegmentAddressInstructionPointer(uint16_t instructionPointer)
{
    return section.setStartSegmentAddressInstructionPointer(instructionPointer);
}

uint16_t HexSection::getExtendedLinearAddress() const
{
    return section.getExtendedLinearAddress();
}

bool HexSection::setExtendedLinearAddress(uint16_t linearAddress)
{
    return section.setExtendedLinearAddress(linearAddress);
}

uint32_t HexSection::getStartLinearAddressExtendedInstructionPointer() const
{
    return section.getStartLinearAddressExtendedInstructionPointer();
}

bool HexSection::setStartLinearAddressExtendedInstructionPointer(uint32_t extendedInstructionPointer)
{
    return section.setStartLinearAddressExtendedInstructionPointer(extendedInstructionPointer);
}

HexAddressMap HexSection::getAddressMap() const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
            return HexAddress::getDataRecordAddressMap();
            break;

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            return HexAddress::getExtendedSegmentAddressMap(getExtendedSegmentAddress());
            break;

        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            return HexAddress::getExtendedLinearAddressMap(getExtendedLinearAddress());
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexAddressMap HexSection::getDataMap() const
{
    // Set up address type or throw exception
    // if section does not suppport addresses
    HexAddress address{};
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
            address.setAddressType(HexAddressEnumType::ADDRESS_I8HEX);
            break;

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            address.setAddressType(HexAddressEnumType::ADDRESS_I16HEX);
            address.setSegmentAddress(getExtendedSegmentAddress());
            break;

        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            address.setAddressType(HexAddressEnumType::ADDRESS_I32HEX);
            address.setLinearAddress(getExtendedLinearAddress());
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }

    // Fill all records' absolute addresses and sizes into the result
    HexAddressMap result{};
    for (auto it{blocks.cbegin()}; it != blocks.cend(); ++it)
        result[address.getAbsoluteAddress(it->first)] = it->second.size();

    // Return result
    return result;
}

size_t HexSection::getSize() const
{
    switch (getSectionType())
    {
        // Size of the section is size of all the blocks
        case HexSectionType::SECTION_DATA:
            return blocks.size();
            break;

        // Size of the section is size of all the blocks including
        // the section record
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            return (blocks.size() + 1);
            break;

        // Section contains a single record
        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            return 1;
            break;
    }
}

bool HexSection::hasData() const
{
    switch (getSectionType())
    {
        // Sections have addresses only if there are any blocks
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            return (blocks.size() > 0);
            break;

        // Sections without data
        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            return false;
            break;
    }
}

HexDataSizeType HexSection::getDataSize() const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            HexDataSizeType result{0};
            for (const auto& block : blocks)
                result += block.second.size();

            return result;
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            return 0;
            break;
    }
}

HexDataType HexSection::getUnusedDataFillValue() const
{
    return unusedDataFillValue;
}

void HexSection::setUnusedDataFillValue(HexDataType unusedDataFillValue)
{
    this->unusedDataFillValue = unusedDataFillValue;
}

HexDataSizeType HexSection::getDefaultDataSize() const
{
    return defaultDataSize;
}

void HexSection::setDefaultDataSize(HexDataSizeType defaultDataSize)
{
    if (defaultDataSize <= 1)
        throw std::out_of_range("Default data size must be greater than one");

    this->defaultDataSize = defaultDataSize;
}

bool HexSection::getRecord(HexRecord& record, size_t index) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        {
            // Return section record
            if (index == 0)
            {
                record = section;
                return true;
            }
            break;
        }

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            if (index == 0)
            {
                // Return section record
                record = section;
                return true;
            }
            else if (index <= blocks.size())
            {
                // Make a data record
                const auto it{std::next(blocks.cbegin(), index - 1)};
                record.prepareDataRecord(it->first, it->second);
                return true;
            }
            break;
        }

        case HexSectionType::SECTION_DATA:
        default:
        {
            if ((blocks.size() > 0) && (index < blocks.size()))
            {
                // Make a data record
                const auto it{std::next(blocks.cbegin(), index)};
                record.prepareDataRecord(it->first, it->second);
                return true;
            }
            break;
        }
    }

    return false;
}

bool HexSection::getRecord(std::string& record, size_t index) const
{
    HexRecord hexRecord{};
    bool result{getRecord(hexRecord, index)};

    if (result)
        record = hexRecord.getRecord();

    return result;
}

HexRecord HexSection::getRecord(size_t index) const
{
    HexRecord record{};
    if (!getRecord(record, index))
        throw std::out_of_range("Index out of range");

    return record;
}

bool HexSection::canPushRecord(const HexRecord& record) const
{
    if (!record.isValidChecksum())
        return false;

    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        {
            switch (record.getRecordType())
            {
                case HexRecordType::RECORD_DATA:
                    // Can push RECORD_DATA to SECTION_DATA
                    return true;
                    break;

                case HexRecordType::RECORD_END_OF_FILE:
                case HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS:
                case HexRecordType::RECORD_START_SEGMENT_ADDRESS:
                case HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS:
                case HexRecordType::RECORD_START_LINEAR_ADDRESS:
                    // If record is any other record than RECORD_DATA and
                    // section is empty SECTION_DATA, then allow pushing by
                    // setting the section to the new type
                    return (blocks.size() == 0);
                    break;

                default:
                    // Unsupported record type
                    return false;
                    break;
            }
            break;
        }

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            // Only allow pushing of data records
            return (record.getRecordType() == HexRecordType::RECORD_DATA);
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            // Sections not supporting data records
            return false;
            break;
    }
}

bool HexSection::canPushRecord(const std::string& record) const
{
    HexRecord hexRecord{};
    return (hexRecord.setRecord(record) && canPushRecord(hexRecord));
}

bool HexSection::pushRecord(const HexRecord& record)
{
    if (!record.isValidChecksum())
        return false;

    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        {
            switch (record.getRecordType())
            {
                case HexRecordType::RECORD_DATA:
                {
                    // Can push RECORD_DATA to SECTION_DATA
                    for (size_t index{0}; index < record.getDataSize(); ++index)
                        setData(record.getAddress() + static_cast<HexDataAddressType>(index), record.getData(index));

                    return true;
                    break;
                }

                case HexRecordType::RECORD_END_OF_FILE:
                case HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS:
                case HexRecordType::RECORD_START_SEGMENT_ADDRESS:
                case HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS:
                case HexRecordType::RECORD_START_LINEAR_ADDRESS:
                {
                    // If record is any other record than RECORD_DATA and
                    // section is empty SECTION_DATA, then allow pushing by
                    // setting the section to the new type
                    const bool canPush{(blocks.size() == 0)};
                    if (canPush)
                        section = record;

                    return canPush;
                    break;
                }

                default:
                    // Unsupported record type
                    return false;
                    break;
            }
            break;
        }

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            // Only allow pushing of data records
            const bool canPush{(record.getRecordType() == HexRecordType::RECORD_DATA)};
            if (canPush)
            {
                for (size_t index{0}; index < record.getDataSize(); ++index)
                    setData(record.getAddress() + static_cast<HexDataAddressType>(index), record.getData(index));
            }
            return canPush;
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            // Sections not supporting data records
            return false;
            break;
    }
}

bool HexSection::pushRecord(const std::string& record)
{
    HexRecord hexRecord{};
    return (hexRecord.setRecord(record) && pushRecord(hexRecord));
}

bool HexSection::hasAddress() const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            return true;
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            return false;
            break;
    }
}

HexDataAddressType HexSection::getLowerAddress() const
{
    // Supports addresses?
    if (!hasAddress())
        throw std::domain_error("Section does not support addresses");

    // Does section have any data?
    if (blocks.size() == 0)
        throw std::out_of_range("Section has no data");

    // Return address of the first address of the first block
    return blocks.cbegin()->first;
}

HexDataAddressType HexSection::getUpperAddress() const
{
    // Supports addresses?
    if (!hasAddress())
        throw std::domain_error("Section does not support addresses");

    // Does section have any data?
    if (blocks.size() == 0)
        throw std::out_of_range("Section has no data");

    // Return address of the last block of the last address
    const auto it{blocks.crbegin()};
    return (it->first + static_cast<HexDataAddressType>(it->second.size() - 1));
}

HexAddressType HexSection::getMinAbsoluteAddress() const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
            return HexAddress::getMinAbsoluteDataRecordAddress();
            break;

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            return HexAddress::getMinAbsoluteExtendedSegmentAddress(section.getExtendedSegmentAddress());
            break;

        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            return HexAddress::getMinAbsoluteExtendedLinearAddress(section.getExtendedLinearAddress());
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexAddressType HexSection::getMaxAbsoluteAddress() const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
            return HexAddress::getMaxAbsoluteDataRecordAddress();
            break;

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            return HexAddress::getMaxAbsoluteExtendedSegmentAddress(section.getExtendedSegmentAddress());
            break;

        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            return HexAddress::getMaxAbsoluteExtendedLinearAddress(section.getExtendedLinearAddress());
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

bool HexSection::hasAbsoluteAddress(HexAddressType address) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
            return HexAddress::containsAbsoluteDataRecordAddress(address);
            break;

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            return HexAddress::containsAbsoluteSegmentAddress(address, getExtendedSegmentAddress());
            break;

        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            return HexAddress::containsAbsoluteLinearAddress(address, getExtendedLinearAddress());
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexAddressType HexSection::getAbsoluteAddress(HexDataAddressType address) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
            return HexAddress::getAbsoluteDataRecordAddress(address);
            break;

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            return HexAddress::getAbsoluteExtendedSegmentAddress(address, section.getExtendedSegmentAddress());
            break;

        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            return HexAddress::getAbsoluteExtendedLinearAddress(address, section.getExtendedLinearAddress());
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexDataAddressType HexSection::getRelativeAddress(HexAddressType address) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
            return HexAddress::getRelativeDataRecordAddress(address);
            break;

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            return HexAddress::getRelativeExtendedSegmentAddress(address, section.getExtendedSegmentAddress());
            break;

        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            return HexAddress::getRelativeExtendedLinearAddress(address, section.getExtendedLinearAddress());
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

bool HexSection::checkIntersect(const HexSection& section) const
{
    const auto sectionType{getSectionType()};
    if (section.getSectionType() != sectionType)
        throw std::domain_error("Sections are not of the equal type");

    switch (sectionType)
    {
        case HexSectionType::SECTION_DATA:
            // Any two SECTION_DATA sections would always intersect
            return true;
            break;

        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        {
            return HexAddress::checkExtendedSegmentAddressIntersect(getExtendedSegmentAddress(), section.getExtendedSegmentAddress());
            break;
        }

        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            return HexAddress::checkExtendedLinearAddressIntersect(getExtendedLinearAddress(), section.getExtendedLinearAddress());
            break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

size_t HexSection::findAddress(HexDataAddressType address) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            for (auto it{blocks.crbegin()}; it != blocks.crend(); ++it)
            {
                if (it->first <= address)
                {
                    // The condition above guarantees the difference will always be positive
                    const auto index{static_cast<size_t>(address - it->first)};
                    if (index < it->second.size())
                    {
                        // Return index from the beginning
                        return (std::distance(it, blocks.crend()) - 1);
                    }

                    // Address is below the address of the block
                    break;
                }
            }

            // Address not found
            return std::string::npos;
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

size_t HexSection::findPreviousRecord(HexDataAddressType address) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            // No previous address before the first available one
            if (address == 0)
                return std::string::npos;

            for (auto it{blocks.crbegin()}; it != blocks.crend(); ++it)
            {
                // Previous block is the one for which address is bigger than the first address of the block
                if (address > it->first)
                {
                    // Return index from the beginning
                    return (std::distance(it, blocks.crend()) - 1);

                    // Address is below the address of the block
                    break;
                }
            }

            // Previous address not found
            return std::string::npos;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

size_t HexSection::findNextRecord(HexDataAddressType address) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            // No next address after the last available one
            if (address == MAXIMUM_HEX_RECORD_ADDRESS)
                return std::string::npos;

            for (auto it{blocks.cbegin()}; it != blocks.cend(); ++it)
            {
                // Next block is the one for which address is lower than the last address of the block
                if (address < (it->first + it->second.size() - 1))
                {
                    // Return index from the beginning
                    return (std::distance(blocks.cbegin(), it));

                    // Address is above the address of the block
                    break;
                }
            }

            // Next address not found
            return std::string::npos;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

void HexSection::clearData()
{
    blocks.clear();
}

void HexSection::clearData(HexDataAddressType address)
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            for (auto it{blocks.begin()}; it != blocks.end(); ++it)
            {
                // Address is below the block
                if (address < it->first)
                    break;

                // Address is above this block
                const auto last{it->first + it->second.size()};
                if (address >= last)
                    continue;

                if ((last - address) > 1)
                {
                    // Start address of the new block
                    const auto offset{address - it->first + 1};

                    // Create new block and add trailing data
                    blocks[address + 1].assign(it->second.cbegin() + offset, it->second.cend());
                }

                // Truncate or delete existing block
                const auto size{address - it->first};
                if (size > 0)
                    it->second.resize(size);
                else
                    blocks.erase(it);

                // Exit the loop
                break;
            }
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexDataSizeType HexSection::clearData(HexDataAddressType address, HexDataSizeType size)
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            // Check if relative address range is valid
            if (!HexAddress::isValidRelativeAddressRange(address, size))
                throw std::out_of_range("Relative address range is out of range");

            // Define current address and result
            HexDataAddressType currentAddress{address};
            HexDataSizeType result{0};

            // Clear data
            while (result < size)
            {
                // Find block
                auto it{blocks.begin()};
                while (it != blocks.end())
                {
                    // Find block which either contains address or is before it
                    if ((it->first + it->second.size()) > currentAddress)
                        break;

                    // Increment block
                    ++it;
                }

                if (it == blocks.end())
                {
                    // Nothing to clear since there are no more data blocks
                    // after the current address
                    result = size;
                    currentAddress = static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }

                if (currentAddress < it->first)
                {
                    // The condition above guarantees the difference will always be positive
                    const auto difference{std::min((size - result), static_cast<HexDataSizeType>(it->first - currentAddress))};

                    // Address is before the first address of the block
                    // so there is nothing to clear up to the beginning of the block
                    result += difference;
                    currentAddress += static_cast<HexDataAddressType>(difference);

                    // Continue loop
                    continue;
                }
                else
                {
                    // Address is inside the block

                    // Offset of the address in the block
                    // The condition above guarantees the difference will always be positive
                    const auto offset{static_cast<HexDataSizeType>(currentAddress - it->first)};

                    // Size of the block that can be cleared
                    // The condition above guarantees the difference will always be positive
                    const HexDataSizeType clearSize{std::min((size - result), static_cast<HexDataSizeType>(it->second.size() - offset))};

                    // Offset of the new block after the cleared part of the block
                    const HexDataSizeType offsetNew{offset + clearSize};

                    // Size of the new block after the cleared part of the block
                    const HexDataSizeType sizeNew{it->second.size() - offsetNew};

                    // Is there a need to create new block after the cleared part?
                    if (sizeNew > 0)
                    {
                        // Create new block
                        auto& block{blocks[it->first + static_cast<HexDataAddressType>(offsetNew)]};
                        block.reserve(sizeNew);

                        // Copy data
                        for (HexDataSizeType index{0}; index < sizeNew; ++index)
                            block.push_back(it->second.at(offsetNew + index));

                        // Clear copied data from the current block
                        for (HexDataSizeType index{0}; index < sizeNew; ++index)
                            it->second.pop_back();
                    }

                    // Clear data
                    if (it->second.size() == clearSize)
                    {
                        // Clear whole block
                        blocks.erase(it->first);
                    }
                    else
                    {
                        // Clear part of the block
                        for (HexDataSizeType index{0}; index < clearSize; ++index)
                            it->second.pop_back();
                    }

                    // Increment result and address
                    result += clearSize;
                    currentAddress += static_cast<HexDataAddressType>(clearSize);

                    // Continue loop
                    continue;
                }
            }

            // Return count of cleared addresses
            return result;
        }
        break;

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexDataType HexSection::getData(HexDataAddressType address) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            for (auto it{blocks.crbegin()}; it != blocks.crend(); ++it)
            {
                if (it->first <= address)
                {
                    // The condition above guarantees the difference will always be positive
                    const auto index{static_cast<size_t>(address - it->first)};
                    if (index < it->second.size())
                        return it->second[index];

                    // Address is below the address of the block
                    break;
                }
            }

            // Return unused data fill value
            return unusedDataFillValue;
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexDataSizeType HexSection::getData(HexDataAddressType address, HexDataType* data, HexDataSizeType dataSize) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            // Check if relative address range is valid
            if (!HexAddress::isValidRelativeAddressRange(address, dataSize))
                throw std::out_of_range("Relative address range is out of range");

            // Find block
            auto it{blocks.cbegin()};
            while (it != blocks.cend())
            {
                // Find block which either contains address or is before it
                if ((it->first + it->second.size()) > address)
                    break;

                // Increment block
                ++it;
            }

            // Result is data size
            HexDataSizeType result{0};

            // Copy data or use unused data fill value
            while (result < dataSize)
            {
                if (it == blocks.cend())
                {
                    // Use unused data fill value if there is no block at or after the address
                    for (; result < dataSize; ++result)
                        data[result] = unusedDataFillValue;

                    // Continue loop
                    continue;
                }
                else if (address < it->first)
                {
                    // Fill data to the next block

                    // The condition above guarantees the difference will always be positive
                    const HexDataSizeType size{std::min((dataSize - result), static_cast<HexDataSizeType>(it->first - address))};

                    // Use unused data fill value
                    for (HexDataSizeType index{0}; index < size; ++index)
                        data[result + index] = unusedDataFillValue;

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
                else
                {
                    // Copy data from the block

                    // Offset of the data in the block
                    // The condition above guarantees the difference will always be positive
                    const auto offset{static_cast<HexDataSizeType>(address - it->first)};

                    // Size of data to copy from the block
                    const HexDataSizeType size{std::min((dataSize - result), (it->second.size() - offset))};

                    // Copy data
                    for (size_t index{0}; index < size; ++index)
                        data[result + index] = it->second.at(offset + index);

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);
                }

                // Increment block
                ++it;
            }

            // Return count of copied addresses
            return result;
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexDataSizeType HexSection::getData(HexDataAddressType address, HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset) const
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            // Check if relative address range is valid
            if (!HexAddress::isValidRelativeAddressRange(address, dataSize))
                throw std::out_of_range("Relative address range is out of range");

            if (data.size() < offset)
                throw std::underflow_error("Size of the data vector is smaller than the offset");

            // Find block
            auto it{blocks.cbegin()};
            while (it != blocks.cend())
            {
                // Find block which either contains address or is before it
                if ((it->first + it->second.size()) > address)
                    break;

                // Increment block
                ++it;
            }

            // Result is data size
            HexDataSizeType result{0};

            // Copy data or use unused data fill value
            while (result < dataSize)
            {
                if (it == blocks.cend())
                {
                    // Use unused data fill value if there is no block at or after the address
                    for (; result < dataSize; ++result)
                    {
                        // Set or push unused data fill value
                        if (offset + result < data.size())
                            data[offset + result] = unusedDataFillValue;
                        else
                            data.push_back(unusedDataFillValue);
                    }

                    // Continue loop
                    continue;
                }
                else if (address < it->first)
                {
                    // Fill data to the next block

                    // The condition above guarantees the difference will always be positive
                    const HexDataSizeType size{std::min((dataSize - result), static_cast<HexDataSizeType>(it->first - address))};

                    // Use unused data fill value
                    for (HexDataSizeType index{0}; index < size; ++index)
                    {
                        // Set or push unused data fill value
                        if ((offset + result + index) < data.size())
                            data[offset + result + index] = unusedDataFillValue;
                        else
                            data.push_back(unusedDataFillValue);
                    }

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
                else
                {
                    // Copy data from the block

                    // Offset of the data in the block
                    // The condition above guarantees the difference will always be positive
                    const auto blockOffset{static_cast<HexDataSizeType>(address - it->first)};

                    // Size of data to copy from the block
                    const HexDataSizeType size{std::min((dataSize - result), (it->second.size() - blockOffset))};

                    // Copy data
                    for (size_t index{0}; index < size; ++index)
                    {
                        // Set data
                        if ((offset + result + index) < data.size())
                            data[offset + result + index] = it->second.at(blockOffset + index);
                        else
                            data.push_back(it->second.at(blockOffset + index));
                    }

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);
                }

                // Increment block
                ++it;
            }

            // Return count of copied addresses
            return result;
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

void HexSection::setData(HexDataAddressType address, HexDataType data)
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            for (auto it{blocks.rbegin()}; it != blocks.rend(); ++it)
            {
                if (it->first <= address)
                {
                    // The condition above guarantees the difference will always be positive
                    const auto index{static_cast<size_t>(address - it->first)};
                    if (index < it->second.size())
                    {
                        // Use this block if it contains address
                        it->second[index] = data;
                        return;
                    }
                    else if (index == it->second.size())
                    {
                        // If block size is of default size then
                        // make a new block instead
                        if ((it->second.size() % defaultDataSize) == 0)
                            break;

                        // Append to the existing block if an address
                        // naturally follows it
                        it->second.push_back(data);
                        return;
                    }

                    // Address is below the address of the block
                    break;
                }
            }

            // Create new block
            blocks[address].push_back(data);
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexDataSizeType HexSection::setData(HexDataAddressType address, const HexDataType* data, HexDataSizeType dataSize)
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            // Check if relative address range is valid
            if (!HexAddress::isValidRelativeAddressRange(address, dataSize))
                throw std::out_of_range("Relative address range is out of range");

            // Result is data size
            HexDataSizeType result{0};
            while (result < dataSize)
            {
                // Find block
                auto it{blocks.begin()};
                while (it != blocks.end())
                {
                    // Find block which either contains address or is before it
                    if ((it->first + it->second.size()) > address)
                        break;

                    // Increment block
                    ++it;
                }

                if (it == blocks.end())
                {
                    // Create block if there is no block at or after the address

                    // Size of data to append to the block
                    const HexDataSizeType size{std::min((dataSize - result), defaultDataSize)};

                    // Create new block
                    auto& block{blocks[address]};
                    block.reserve(size);

                    // Append data
                    for (HexDataSizeType index{0}; index < size; ++index)
                        block.push_back(data[result + index]);

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
                else if (address < it->first)
                {
                    // Create data to the next block

                    // Size of data to append to the block
                    HexDataSizeType size{(dataSize - result)};

                    // The condition above guarantees the difference will always be positive
                    // Limit size to the block which comes next
                    size = std::min(size, static_cast<HexDataSizeType>(it->first - address));

                    // Limit to the maximum block size
                    size = std::min(size, defaultDataSize);

                    // Create new block
                    auto& block{blocks[address]};
                    block.reserve(size);

                    // Append data
                    for (HexDataSizeType index{0}; index < size; ++index)
                        block.push_back(data[result + index]);

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
                else
                {
                    // Overwrite or append data to the existing block

                    // Offset of the data in the block
                    // The condition above guarantees the difference will always be positive
                    const auto offset{static_cast<HexDataSizeType>(address - it->first)};

                    // Size of data to overwrite or append to the block
                    // Consider also the offset with the maximum record size limit
                    HexDataSizeType size{std::min((dataSize - result), (defaultDataSize - offset))};

                    // Find the next block to determine how much data can be appended to
                    // the current block
                    const auto next{std::next(it)};

                    // Limit amount of data that could be appended
                    // to the current block to reach the next block
                    if (next != blocks.end())
                    {
                        // Difference of addresses between the current and next block
                        // considering the offset
                        const auto diff{next->first - (it->first + offset)};

                        // Further limit size of data that could be appended
                        size = std::min(size, diff);
                    };

                    // Overwrite or append to the block
                    for (size_t index{0}; index < size; ++index)
                    {
                        if (offset + index < it->second.size())
                            it->second[offset + index] = data[result + index];
                        else
                            it->second.push_back(data[result + index]);
                    }

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
            }

            // Return count of changed addresses
            return result;
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexDataSizeType HexSection::setData(HexDataAddressType address, const HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset)
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            // Check if relative address range is valid
            if (!HexAddress::isValidRelativeAddressRange(address, dataSize))
                throw std::out_of_range("Relative address range is out of range");

            // Offset range check
            if (offset > data.size())
                return 0;

            // Size range check
            if ((offset + dataSize) > data.size())
                dataSize = (data.size() - offset);

            // Result is data size
            HexDataSizeType result{0};
            while (result < dataSize)
            {
                // Find block
                auto it{blocks.begin()};
                while (it != blocks.end())
                {
                    // Find block which either contains address or is before it
                    if ((it->first + it->second.size()) > address)
                        break;

                    // Increment block
                    ++it;
                }

                if (it == blocks.end())
                {
                    // Create block if there is no block at or after the address

                    // Size of data to append to the block
                    const HexDataSizeType size{std::min((dataSize - result), defaultDataSize)};

                    // Create new block
                    auto& block{blocks[address]};
                    block.reserve(size);

                    // Append data
                    for (HexDataSizeType index{0}; index < size; ++index)
                        block.push_back(data.at(offset + result + index));

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
                else if (address < it->first)
                {
                    // Create data to the next block

                    // Size of data to append to the block
                    HexDataSizeType size{(dataSize - result)};

                    // The condition above guarantees the difference will always be positive
                    // Limit size to the block which comes next
                    size = std::min(size, static_cast<HexDataSizeType>(it->first - address));

                    // Limit to the maximum block size
                    size = std::min(size, defaultDataSize);

                    // Create new block
                    auto& block{blocks[address]};
                    block.reserve(size);

                    // Append data
                    for (HexDataSizeType index{0}; index < size; ++index)
                        block.push_back(data.at(offset + result + index));

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
                else
                {
                    // Overwrite or append data to the existing block

                    // Offset of the data in the block
                    // The condition above guarantees the difference will always be positive
                    const auto outOffset{static_cast<HexDataSizeType>(address - it->first)};

                    // Size of data to overwrite or append to the block
                    // Consider also the offset with the maximum record size limit
                    HexDataSizeType size{std::min((dataSize - result), (defaultDataSize - outOffset))};

                    // Find the next block to determine how much data can be appended to
                    // the current block
                    const auto next{std::next(it)};

                    // Limit amount of data that could be appended
                    // to the current block to reach the next block
                    if (next != blocks.end())
                    {
                        // Difference of addresses between the current and next block
                        // considering the offset
                        const auto diff{next->first - (it->first + outOffset)};

                        // Further limit size of data that could be appended
                        size = std::min(size, diff);
                    };

                    // Overwrite or append to the block
                    for (size_t index{0}; index < size; ++index)
                    {
                        if (outOffset + index < it->second.size())
                            it->second[outOffset + index] = data.at(offset + result + index);
                        else
                            it->second.push_back(data.at(offset + result + index));
                    }

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
            }

            // Return count of changed addresses
            return result;
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

HexDataSizeType HexSection::fillData(HexDataAddressType address, HexDataSizeType dataSize, HexDataType data)
{
    switch (getSectionType())
    {
        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        {
            // Check if relative address range is valid
            if (!HexAddress::isValidRelativeAddressRange(address, dataSize))
                throw std::out_of_range("Relative address range is out of range");

            // Result is data size
            HexDataSizeType result{0};
            while (result < dataSize)
            {
                // Find block
                auto it{blocks.begin()};
                while (it != blocks.end())
                {
                    // Find block which either contains address or is before it
                    if ((it->first + it->second.size()) > address)
                        break;

                    // Increment block
                    ++it;
                }

                if (it == blocks.end())
                {
                    // Create block if there is no block at or after the address

                    // Size of data to append to the block
                    const HexDataSizeType size{std::min((dataSize - result), defaultDataSize)};

                    // Create new block
                    auto& block{blocks[address]};
                    block.reserve(size);

                    // Append data
                    for (HexDataSizeType index{0}; index < size; ++index)
                        block.push_back(data);

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
                else if (address < it->first)
                {
                    // Create data to the next block

                    // Size of data to append to the block
                    HexDataSizeType size{(dataSize - result)};

                    // The condition above guarantees the difference will always be positive
                    // Limit size to the block which comes next
                    size = std::min(size, static_cast<HexDataSizeType>(it->first - address));

                    // Limit to the maximum block size
                    size = std::min(size, defaultDataSize);

                    // Create new block
                    auto& block{blocks[address]};
                    block.reserve(size);

                    // Append data
                    for (HexDataSizeType index{0}; index < size; ++index)
                        block.push_back(data);

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
                else
                {
                    // Overwrite or append data to the existing block

                    // Offset of the data in the block
                    // The condition above guarantees the difference will always be positive
                    const auto outOffset{static_cast<HexDataSizeType>(address - it->first)};

                    // Size of data to overwrite or append to the block
                    // Consider also the offset with the maximum record size limit
                    HexDataSizeType size{std::min((dataSize - result), (defaultDataSize - outOffset))};

                    // Find the next block to determine how much data can be appended to
                    // the current block
                    const auto next{std::next(it)};

                    // Limit amount of data that could be appended
                    // to the current block to reach the next block
                    if (next != blocks.end())
                    {
                        // Difference of addresses between the current and next block
                        // considering the offset
                        const auto diff{next->first - (it->first + outOffset)};

                        // Further limit size of data that could be appended
                        size = std::min(size, diff);
                    };

                    // Overwrite or append to the block
                    for (size_t index{0}; index < size; ++index)
                    {
                        if (outOffset + index < it->second.size())
                            it->second[outOffset + index] = data;
                        else
                            it->second.push_back(data);
                    }

                    // Increment result and address
                    result += size;
                    address += static_cast<HexDataAddressType>(size);

                    // Continue loop
                    continue;
                }
            }

            // Return count of changed addresses
            return result;
            break;
        }

        case HexSectionType::SECTION_END_OF_FILE:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            throw std::domain_error("Section does not support addresses");
            break;
    }
}

void HexSection::compact()
{
    // Iterate through blocks
    auto it{blocks.begin()};
    while (it != blocks.end())
    {
        // Next block
        auto next{std::next(it)};

        // Does the next block exist?
        if (next == blocks.end())
            break;

        // Is next block adjacent to the current one?
        if (it->first + it->second.size() != next->first)
        {
            ++it;
            continue;
        }

        // Is block already of the default data size?
        if (it->second.size() >= defaultDataSize)
        {
            ++it;
            continue;
        }

        // Size of the data to move to the current block
        // The condition above guarantees the difference will always be positive
        const HexDataSizeType size{std::min((defaultDataSize - it->second.size()), next->second.size())};

        // Size of the data to move to the new block
        // The condition above guarantees the difference will always be positive
        const HexDataSizeType sizeNew{next->second.size() - size};

        // Create new block
        if (sizeNew > 0)
        {
            // Address of the new block
            const auto address{static_cast<HexDataAddressType>(next->first + size)};

            // Create new block
            auto& block{blocks[address]};
            block.reserve(sizeNew);

            // Populate new block
            for (HexDataSizeType index{0}; index < sizeNew; ++index)
                block.push_back(next->second.at(size + index));

            // Truncate existing block
            for (HexDataSizeType index{0}; index < sizeNew; ++index)
                next->second.pop_back();
        }

        // Append to the current block from the next block
        for (HexDataSizeType index{0}; index < size; ++index)
            it->second.push_back(next->second.at(index));

        // Erase next block
        blocks.erase(next);
    }
}

END_NAMESPACE_LIBIHEX
