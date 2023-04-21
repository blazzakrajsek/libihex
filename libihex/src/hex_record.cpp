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
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <ihex/hex_record.hpp>

BEGIN_NAMESPACE_LIBIHEX

HexRecord::HexRecord() :
    address{0}, recordType{HexRecordType::RECORD_DATA}, data{}, checksum{0}
{
    // Reserve space for default data record and add single byte of data
    // for the record to be valid
    data.reserve(DEFAULT_HEX_RECORD_DATA_SIZE);
    data.push_back(DEFAULT_HEX_RECORD_DATA_VALUE);
    updateChecksum();
}

HexRecord::HexRecord(const std::string& record) :
    address{0}, recordType{HexRecordType::RECORD_DATA}, data{}, checksum{0}
{
    if (!setRecord(record))
        throw std::runtime_error("Failed to parse record in string format");

    if (!isValidChecksum())
        throw std::logic_error("Record checksum is not valid");
}

HexRecord::HexRecord(HexRecordType recordType) :
    address{0}, recordType{recordType}, data{}, checksum{0}
{
    switch (recordType)
    {
        case HexRecordType::RECORD_END_OF_FILE:
            prepareEndOfFileRecord();
            break;

        case HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS:
            prepareExtendedSegmentAddressRecord(0);
            break;

        case HexRecordType::RECORD_START_SEGMENT_ADDRESS:
            prepareStartSegmentAddressRecord(0, 0);
            break;

        case HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS:
            prepareExtendedLinearAddressRecord(0);
            break;

        case HexRecordType::RECORD_START_LINEAR_ADDRESS:
            prepareStartLinearAddressRecord(0);
            break;

        case HexRecordType::RECORD_DATA:
        default:
        {
            HexDataType data{DEFAULT_HEX_RECORD_DATA_VALUE};
            prepareDataRecord(0, &data, 1);
            break;
        }
    }
}

HexDataType HexRecord::operator[](HexDataSizeType index) const
{
    return data.at(index);
}

HexDataType& HexRecord::operator[](HexDataSizeType index)
{
    return data.at(index);
}

HexRecord HexRecord::makeDataRecord(HexDataAddressType address, const HexDataVector& data)
{
    // Make a RECORD_DATA record
    HexRecord result{};
    result.prepareDataRecord(address, data);
    return result;
}

HexRecord HexRecord::makeDataRecord(HexDataAddressType address, const HexDataType* data, HexDataSizeType dataSize)
{
    // Make a RECORD_DATA record
    HexRecord result{};
    result.prepareDataRecord(address, data, dataSize);
    return result;
}

HexRecord HexRecord::makeEndOfFileRecord()
{
    // Make a RECORD_END_OF_FILE record
    HexRecord result{};
    result.prepareEndOfFileRecord();
    return result;
}

HexRecord HexRecord::makeExtendedSegmentAddressRecord(uint16_t segmentAddress)
{
    // Make a RECORD_EXTENDED_SEGMENT_ADDRESS record
    HexRecord result{};
    result.prepareExtendedSegmentAddressRecord(segmentAddress);
    return result;
}

HexRecord HexRecord::makeStartSegmentAddressRecord(uint16_t codeSegment, uint16_t instructionPointer)
{
    // Make a RECORD_START_SEGMENT_ADDRESS record
    HexRecord result{};
    result.prepareStartSegmentAddressRecord(codeSegment, instructionPointer);
    return result;
}

HexRecord HexRecord::makeExtendedLinearAddressRecord(uint16_t linearAddress)
{
    // Make a RECORD_EXTENDED_LINEAR_ADDRESS record
    HexRecord result{};
    result.prepareExtendedLinearAddressRecord(linearAddress);
    return result;
}

HexRecord HexRecord::makeStartLinearAddressRecord(uint32_t extendedInstructionPointer)
{
    // Make a RECORD_START_LINEAR_ADDRESS record
    HexRecord result{};
    result.prepareStartLinearAddressRecord(extendedInstructionPointer);
    return result;
}

void HexRecord::prepareRecord(HexDataAddressType address, HexRecordType recordType, const HexDataVector& data, HexDataType checksum)
{
    if (!setDataSize(data.size()))
        throw std::out_of_range("Data size is out of range");

    this->address = address;
    this->recordType = recordType;

    for (HexDataSizeType index{0}; index < this->data.size(); ++index)
            this->data.at(index) = data.at(index);

    this->checksum = ((checksum == 0) ? calculateChecksum() : checksum);
}

void HexRecord::prepareRecord(HexDataAddressType address, HexRecordType recordType, const HexDataType* data, HexDataSizeType dataSize, HexDataType checksum)
{
    if (!setDataSize(dataSize))
        throw std::out_of_range("Data size is out of range");

    this->address = address;
    this->recordType = recordType;

    for (HexDataSizeType index{0}; index < this->data.size(); ++index)
        this->data.at(index) = data[index];

    this->checksum = ((checksum == 0) ? calculateChecksum() : checksum);
}

void HexRecord::prepareDataRecord(HexDataAddressType address, const HexDataVector& data)
{
    if ((data.size() < 1) || (data.size() > MAXIMUM_HEX_RECORD_DATA_SIZE))
        throw std::out_of_range("Data size is out of range");

    prepareRecord(address, HexRecordType::RECORD_DATA, data);
}

void HexRecord::prepareDataRecord(HexDataAddressType address, const HexDataType* data, HexDataSizeType dataSize)
{
    if ((dataSize < 1) || (dataSize > MAXIMUM_HEX_RECORD_DATA_SIZE))
        throw std::out_of_range("Data size is out of range");

    prepareRecord(address, HexRecordType::RECORD_DATA, data, dataSize);
}

void HexRecord::prepareEndOfFileRecord()
{
    address = 0;
    recordType = HexRecordType::RECORD_END_OF_FILE;
    data.clear();
    data.shrink_to_fit();
    checksum = calculateChecksum();
}

void HexRecord::prepareExtendedSegmentAddressRecord(uint16_t segmentAddress)
{
    address = 0;
    recordType = HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS;
    data.resize(2);
    data.shrink_to_fit();
    setExtendedSegmentAddress(segmentAddress);
}

void HexRecord::prepareStartSegmentAddressRecord(uint16_t codeSegment, uint16_t instructionPointer)
{
    address = 0;
    recordType = HexRecordType::RECORD_START_SEGMENT_ADDRESS;
    data.resize(4);
    data.shrink_to_fit();
    setStartSegmentAddressCodeSegment(codeSegment);
    setStartSegmentAddressInstructionPointer(instructionPointer);
}

void HexRecord::prepareExtendedLinearAddressRecord(uint16_t linearAddress)
{
    address = 0;
    recordType = HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS;
    data.resize(2);
    data.shrink_to_fit();
    setExtendedLinearAddress(linearAddress);
}

void HexRecord::prepareStartLinearAddressRecord(uint32_t extendedInstructionPointer)
{
    address = 0;
    recordType = HexRecordType::RECORD_START_LINEAR_ADDRESS;
    data.resize(4);
    data.shrink_to_fit();
    setStartLinearAddressExtendedInstructionPointer(extendedInstructionPointer);
}

void HexRecord::updateChecksum()
{
    checksum = calculateChecksum();
}

HexDataType HexRecord::calculateChecksum() const
{
    // Size of the record
    auto checksum{static_cast<HexDataType>(data.size())};

    // Address of the record
    checksum += ((address >> 8) & 0xFF);
    checksum += ((address & 0xFF));

    // Type of the record
    checksum += static_cast<HexDataType>(recordType);

    // Data of the record
    for (const auto& dataItem : data)
        checksum += dataItem;

    // Checksum of the record, return 2's complement
    return -static_cast<int>(checksum);
}

bool HexRecord::isValidChecksum() const
{
    return (checksum == calculateChecksum());
}

std::string HexRecord::getRecord() const
{
    std::ostringstream stream;

    // Start of the record character
    stream << START_OF_HEX_RECORD_CHARACTER;

    // Prepare stream
    stream << std::setfill('0') << std::uppercase << std::hex;

    // Size of the record
    stream << std::setw(2) << static_cast<unsigned int>(data.size());

    // Address of the record
    stream << std::setw(4) << static_cast<unsigned int>(address);

    // Type of the record
    stream << std::setw(2) << static_cast<unsigned int>(recordType);

    // Data of the record
    for (const auto& dataItem : data)
        stream << std::setw(2) << static_cast<unsigned int>(dataItem);

    // Checksum of the record
    stream << std::setw(2) << static_cast<unsigned int>(checksum);

    // Return record as a string
    return stream.str();
}

bool HexRecord::setRecord(const std::string& record)
{
    HexDataSizeType position{1};
    if (record.size() < (position + 3))
        return false;

    // Parse size of data
    HexDataSizeType dataSize{static_cast<HexDataSizeType>(strtol(record.substr(position, 2).c_str(), nullptr, 16))};
    position += 2;

    // Check length
    if (record.size() < (10U + (2 * dataSize)))
        return false;

    // Set size of data
    setDataSize(dataSize);

    // Parse address
    address = static_cast<HexDataAddressType>(strtol(record.substr(position, 4).c_str(), nullptr, 16));
    position += 4;

    // Parse record type
    recordType = static_cast<HexRecordType>(strtol(record.substr(position, 2).c_str(), nullptr, 16));
    recordType = std::min(recordType, HexRecordType::RECORD_MAX);
    position += 2;

    // Parse record data
    for (auto& dataByte : data)
    {
        dataByte = static_cast<HexDataType>(strtol(record.substr(position, 2).c_str(), nullptr, 16));
        position += 2;
    }

    // Parse checksum
    checksum = static_cast<HexDataType>(strtol(record.substr(position, 2).c_str(), nullptr, 16));

    return isValidRecord();
}

HexDataSizeType HexRecord::getDataSize() const
{
    return data.size();
}

bool HexRecord::setDataSize(HexDataSizeType dataSize)
{
    if (dataSize > MAXIMUM_HEX_RECORD_DATA_SIZE)
        return false;

    data.reserve(std::max(dataSize, DEFAULT_HEX_RECORD_DATA_SIZE));
    data.resize(dataSize, DEFAULT_HEX_RECORD_DATA_VALUE);

    updateChecksum();
    return true;
}

HexDataAddressType HexRecord::getAddress() const
{
    return address;
}

void HexRecord::setAddress(HexDataAddressType address)
{
    this->address = address;
    updateChecksum();
}

HexDataAddressType HexRecord::getLastAddress() const
{
    if (hasAddress())
        return (address + static_cast<HexDataAddressType>(data.size()) - 1);
    else
        throw std::domain_error("Record does not support or does not have addresses");
}

bool HexRecord::hasAddress() const
{
    return ((recordType == HexRecordType::RECORD_DATA) && (data.size() > 0));
}

bool HexRecord::hasAddress(HexDataAddressType address) const
{
    if (hasAddress())
        return (!((address < getAddress()) || (address > getLastAddress())));
    else
        throw std::domain_error("Record does not support or does not have addresses");
}

HexRecordType HexRecord::getRecordType() const
{
    return recordType;
}

bool HexRecord::setRecordType(HexRecordType recordType)
{
    if (recordType > HexRecordType::RECORD_MAX)
        return false;

    this->recordType = recordType;
    updateChecksum();
    return true;
}

HexDataType HexRecord::getData(HexDataSizeType index) const
{
    if (index < data.size())
        return data.at(index);
    else
        return DEFAULT_HEX_RECORD_DATA_VALUE;
}

HexDataSizeType HexRecord::getData(HexDataType* data, HexDataSizeType dataSize, HexDataSizeType offset) const
{
    // No data
    if (this->data.size() == 0)
        return 0;

    // Offset range check
    if ((offset + dataSize) > this->data.size())
        throw std::out_of_range("dataSize and/or offset out of range");

    // Limit size
    auto size{std::min(dataSize, this->data.size() - offset)};

    // Copy data
    for (HexDataSizeType index{0}; index < size; ++index)
        data[index] = this->data.at(index + offset);

    // Return copied size
    return size;
}

HexDataSizeType HexRecord::getData(HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType inOffset, HexDataSizeType outOffset) const
{
    // No data
    if (this->data.size() == 0)
        return 0;

    // Input offset range check
    if ((inOffset + dataSize) > this->data.size())
        throw std::out_of_range("dataSize and/or inOffset out of range");

    // Output offset range check
    if ((outOffset + dataSize) > data.size())
        throw std::out_of_range("dataSize and/or outOffset out of range");

    // Limit input size
    auto inputSize{std::min(dataSize, this->data.size() - inOffset)};

    // Limit output size
    auto outputSize{std::min(dataSize, data.size() - outOffset)};

    // Limit size
    auto size{std::min(inputSize, outputSize)};

    // Copy data
    for (HexDataSizeType index{0}; index < size; ++index)
        data.at(index + outOffset) = this->data.at(index + inOffset);

    // Return size
    return size;
}

void HexRecord::setData(HexDataSizeType index, HexDataType data)
{
    if (index < this->data.size())
        this->data.at(index) = data;

    updateChecksum();
}

HexDataSizeType HexRecord::setData(const HexDataType* data, HexDataSizeType dataSize, HexDataSizeType offset)
{
    // No data
    if (this->data.size() == 0)
        return 0;

    // Offset range check
    if ((offset + dataSize) > this->data.size())
        throw std::out_of_range("dataSize and/or offset out of range");

    // Limit size
    auto size{std::min(dataSize, this->data.size() - offset)};

    // Copy data
    for (HexDataSizeType index{0}; index < size; ++index)
        this->data.at(index + offset) = data[index];

    // Update checksum and return size
    updateChecksum();
    return size;
}

HexDataSizeType HexRecord::setData(const HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType inOffset, HexDataSizeType outOffset)
{
    // No data
    if (this->data.size() == 0)
        return 0;

    // Input offset range check
    if ((inOffset + dataSize) > data.size())
        throw std::out_of_range("dataSize and/or inOffset out of range");

    // Output offset range check
    if ((outOffset + dataSize) > this->data.size())
        throw std::out_of_range("dataSize and/or outOffset out of range");

    // Limit input size
    auto inputSize{std::min(dataSize, data.size() - inOffset)};

    // Limit output size
    auto outputSize{std::min(dataSize, this->data.size() - outOffset)};

    // Limit size
    auto size{std::min(inputSize, outputSize)};

    // Copy data
    for (HexDataSizeType index{0}; index < size; ++index)
        this->data.at(index + outOffset) = data.at(index + inOffset);

    // Update checksum and return size
    updateChecksum();
    return size;
}

HexDataType HexRecord::getChecksum() const
{
    return checksum;
}

void HexRecord::setChecksum(HexDataType checksum)
{
    this->checksum = checksum;
}

bool HexRecord::isValidRecord() const
{
    return isValidRecord(recordType);
}

bool HexRecord::isValidRecord(HexRecordType recordType) const
{
    switch (recordType)
    {
        case HexRecordType::RECORD_DATA:
            return (data.size() > 0);
            break;

        case HexRecordType::RECORD_END_OF_FILE:
            return (data.size() == 0);
            break;

        case HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS:
        case HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS:
            return (data.size() == 2);
            break;

        case HexRecordType::RECORD_START_SEGMENT_ADDRESS:
        case HexRecordType::RECORD_START_LINEAR_ADDRESS:
            return ((data.size() == 4) && (address == 0));
            break;

        default:
            // Unsupported record type
            return false;
            break;
    }
}

uint16_t HexRecord::getExtendedSegmentAddress() const
{
    if ((recordType == HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS) && isValidRecord())
    {
        uint16_t result{0};
        result |= ((static_cast<uint16_t>(data.at(0)) << 8) & 0xFF00);
        result |= (static_cast<uint16_t>(data.at(1)) & 0xFF);
        return result;
    }
    else
        throw std::domain_error("Record is not valid or is not a RECORD_EXTENDED_SEGMENT_ADDRESS record");
}

bool HexRecord::setExtendedSegmentAddress(uint16_t segmentAddress)
{
    bool result{false};
    if ((recordType == HexRecordType::RECORD_EXTENDED_SEGMENT_ADDRESS) && isValidRecord())
    {
        data.at(0) = static_cast<HexDataType>((segmentAddress >> 8) & 0xFF);
        data.at(1) = static_cast<HexDataType>(segmentAddress & 0xFF);
        updateChecksum();
        result = true;
    }
    return result;
}

uint16_t HexRecord::getStartSegmentAddressCodeSegment() const
{
    if ((recordType == HexRecordType::RECORD_START_SEGMENT_ADDRESS) && isValidRecord())
    {
        uint16_t result{0};
        result |= ((static_cast<uint16_t>(data.at(0)) << 8) & 0xFF00);
        result |= (static_cast<uint16_t>(data.at(1)) & 0xFF);
        return result;
    }
    else
        throw std::domain_error("Record is not valid or is not a RECORD_START_SEGMENT_ADDRESS record");
}

bool HexRecord::setStartSegmentAddressCodeSegment(uint16_t codeSegment)
{
    bool result{false};
    if ((recordType == HexRecordType::RECORD_START_SEGMENT_ADDRESS) && isValidRecord())
    {
        data.at(0) = static_cast<HexDataType>((codeSegment >> 8) & 0xFF);
        data.at(1) = static_cast<HexDataType>(codeSegment & 0xFF);
        updateChecksum();
        result = true;
    }
    return result;
}

uint16_t HexRecord::getStartSegmentAddressInstructionPointer() const
{
    if ((recordType == HexRecordType::RECORD_START_SEGMENT_ADDRESS) && isValidRecord())
    {
        uint16_t result{0};
        result |= ((static_cast<uint16_t>(data.at(2)) << 8) & 0xFF00);
        result |= (static_cast<uint16_t>(data.at(3)) & 0xFF);
        return result;
    }
    else
        throw std::domain_error("Record is not valid or is not a RECORD_START_SEGMENT_ADDRESS record");
}

bool HexRecord::setStartSegmentAddressInstructionPointer(uint16_t instructionPointer)
{
    bool result{false};
    if ((recordType == HexRecordType::RECORD_START_SEGMENT_ADDRESS) && isValidRecord())
    {
        data.at(2) = static_cast<HexDataType>((instructionPointer >> 8) & 0xFF);
        data.at(3) = static_cast<HexDataType>(instructionPointer & 0xFF);
        updateChecksum();
        result = true;
    }
    return result;
}

uint16_t HexRecord::getExtendedLinearAddress() const
{
    if ((recordType == HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS) && isValidRecord())
    {
        uint16_t result{0};
        result |= ((static_cast<uint16_t>(data.at(0)) << 8) & 0xFF00);
        result |= (static_cast<uint16_t>(data.at(1)) & 0xFF);
        return result;
    }
    else
        throw std::domain_error("Record is not valid or is not a RECORD_EXTENDED_LINEAR_ADDRESS record");
}

bool HexRecord::setExtendedLinearAddress(uint16_t linearAddress)
{
    bool result{false};
    if ((recordType == HexRecordType::RECORD_EXTENDED_LINEAR_ADDRESS) && isValidRecord())
    {
        data.at(0) = static_cast<HexDataType>((linearAddress >> 8) & 0xFF);
        data.at(1) = static_cast<HexDataType>(linearAddress & 0xFF);
        updateChecksum();
        result = true;
    }
    return result;
}

uint32_t HexRecord::getStartLinearAddressExtendedInstructionPointer() const
{
    if ((recordType == HexRecordType::RECORD_START_LINEAR_ADDRESS) && isValidRecord())
    {
        uint32_t result{0};
        result |= ((static_cast<uint32_t>(data.at(0)) << 24) & 0xFF000000);
        result |= ((static_cast<uint32_t>(data.at(1)) << 16) & 0xFF0000);
        result |= ((static_cast<uint32_t>(data.at(2)) << 8) & 0xFF00);
        result |= (static_cast<uint32_t>(data.at(3)) & 0xFF);
        return result;
    }
    else
        throw std::domain_error("Record is not valid or is not a RECORD_START_LINEAR_ADDRESS record");
}

bool HexRecord::setStartLinearAddressExtendedInstructionPointer(uint32_t extendedInstructionPointer)
{
    bool result{false};
    if ((recordType == HexRecordType::RECORD_START_LINEAR_ADDRESS) && isValidRecord())
    {
        data.at(0) = static_cast<HexDataType>((extendedInstructionPointer >> 24) & 0xFF);
        data.at(1) = static_cast<HexDataType>((extendedInstructionPointer >> 16) & 0xFF);
        data.at(2) = static_cast<HexDataType>((extendedInstructionPointer >> 8) & 0xFF);
        data.at(3) = static_cast<HexDataType>(extendedInstructionPointer & 0xFF);
        updateChecksum();
        result = true;
    }
    return result;
}

END_NAMESPACE_LIBIHEX
