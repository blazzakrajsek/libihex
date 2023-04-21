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

#pragma once

#include <array>
#include <climits>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

/**
 * @brief Hex record data type
 *
 */
typedef uint8_t HexDataType;

/**
 * @brief Hex record data address type
 *
 */
typedef uint16_t HexDataAddressType;

/**
 * @brief Hex record data size type
 *
 */
typedef size_t HexDataSizeType;

/**
 * @brief Maximum data size of the hex record
 *
 */
static constexpr HexDataSizeType MAXIMUM_HEX_RECORD_DATA_SIZE{std::numeric_limits<HexDataType>::max()};

/**
 * @brief Maximum hex record address
 *
 */
static constexpr HexDataSizeType MAXIMUM_HEX_RECORD_ADDRESS{std::numeric_limits<HexDataAddressType>::max()};

/**
 * @brief Default/typical hex record data size
 *
 */
static constexpr HexDataSizeType DEFAULT_HEX_RECORD_DATA_SIZE{0x10};

/**
 * @brief Default value of uninitialized or unused data
 *
 */
static constexpr HexDataType DEFAULT_HEX_RECORD_DATA_VALUE{std::numeric_limits<HexDataType>::max()};

/**
 * @brief Start of the hex record character
 *
 */
static constexpr unsigned char START_OF_HEX_RECORD_CHARACTER{':'};

/**
 * @brief Hex record line separator
 *
 */
static constexpr unsigned char HEX_RECORD_LINE_SEPARATOR{'\n'};

/**
 * @brief Hex record types
 *
 */
enum class HexRecordType : HexDataType
{
    /**
     * @brief Data record (8/16/32-bit formats)
     *
     */
    RECORD_DATA = 0,

    /**
     * @brief End of file record (8/16/32-bit formats)
     *
     */
    RECORD_END_OF_FILE = 1,

    /**
     * @brief Extended segment address record (16-bit format only)
     *
     */
    RECORD_EXTENDED_SEGMENT_ADDRESS = 2,

    /**
     * @brief Start segment address record (16-bit format only)
     *
     */
    RECORD_START_SEGMENT_ADDRESS = 3,

    /**
     * @brief Extended linear address record (32-bit format only)
     *
     */
    RECORD_EXTENDED_LINEAR_ADDRESS = 4,

    /**
     * @brief Start linear address record (32-bit format only)
     *
     */
    RECORD_START_LINEAR_ADDRESS = 5,

    RECORD_MIN = RECORD_DATA,
    RECORD_MAX = RECORD_START_LINEAR_ADDRESS,
};

/**
 * @brief Hex record data array
 *
 * @tparam T Size of the array
 */
template <HexDataType T = MAXIMUM_HEX_RECORD_DATA_SIZE>
struct HexDataArray : std::array<HexDataType, T>{};

/**
 * @brief Hex record data vector
 *
 */
typedef std::vector<HexDataType> HexDataVector;

/**
 * @brief HexRecord class
 *
 */
class HexRecord final
{
    public:
        /**
         * @brief Construct a new HexRecord object
         *
         */
        explicit HexRecord();

        /**
         * @brief Construct a new HexRecord object
         *
         * @param record Record in string format
         * @throw std::runtime_error Failed to parse record in string format
         * @throw std::logic_error Record checksum is not valid
         */
        explicit HexRecord(const std::string& record);

        /**
         * @brief Construct a new HexRecord object
         *
         * @param recordType Construct a record of the provided type
         */
        explicit HexRecord(HexRecordType recordType);

        /**
         * @brief Copy-construct a new HexRecord object
         *
         * @param record Record to copy-construct from
         */
        HexRecord(const HexRecord& record) = default;

        /**
         * @brief Move-construct a new HexRecord object
         *
         * @param record Record to move-construct from
         */
        HexRecord(HexRecord&& record) = default;

        /**
         * @brief Copy-assignment operator
         *
         * @param record Record to copy-assign
         * @return HexRecord& Assigned record
         */
        HexRecord& operator=(const HexRecord& record) = default;

        /**
         * @brief Move-assignment operator
         *
         * @param record Record to move-assign
         * @return HexRecord& Assigned record
         */
        HexRecord& operator=(HexRecord&& record) = default;

        /**
         * @brief Array index operator
         *
         * @param index Index
         * @throw std::out_of_range index is out of range
         * @return HexDataType Data
         */
        HexDataType operator[](HexDataSizeType index) const;

        /**
         * @brief Array index operator
         *
         * @param index Index
         * @throw std::out_of_range index is out of range
         * @warning Data change through this operator does not update the record checksum
         * @return HexDataType& Data
         */
        HexDataType& operator[](HexDataSizeType index);

        /**
         * @brief Make a RECORD_DATA record
         *
         * @param address Address
         * @param data Data of the record
         * @throw std::out_of_range data.size() is out of range
         * @note data.size() interval range is [1, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @return HexRecord Record
         */
        static HexRecord makeDataRecord(HexDataAddressType address, const HexDataVector& data);

        /**
         * @brief Make a RECORD_DATA record
         *
         * @param address Address
         * @param data Data of the record
         * @param dataSize Size of the data
         * @throw std::out_of_range dataSize is out of range
         * @note dataSize interval range is [1, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @return HexRecord Record
         */
        static HexRecord makeDataRecord(HexDataAddressType address, const HexDataType* data, HexDataSizeType dataSize);

        /**
         * @brief Make a RECORD_END_OF_FILE record
         *
         * @return HexRecord Record
         */
        static HexRecord makeEndOfFileRecord();

        /**
         * @brief Make a RECORD_EXTENDED_SEGMENT_ADDRESS record
         *
         * @param segmentAddress Segment address
         * @return HexRecord Record
         */
        static HexRecord makeExtendedSegmentAddressRecord(uint16_t segmentAddress);

        /**
         * @brief Make a RECORD_START_SEGMENT_ADDRESS record
         *
         * @param codeSegment Code segment (CS)
         * @param instructionPointer Instruction pointer (IP)
         * @return HexRecord Record
         */
        static HexRecord makeStartSegmentAddressRecord(uint16_t codeSegment, uint16_t instructionPointer);

        /**
         * @brief Make a RECORD_EXTENDED_LINEAR_ADDRESS record
         *
         * @param linearAddress Linear address
         * @return HexRecord Record
         */
        static HexRecord makeExtendedLinearAddressRecord(uint16_t linearAddress);

        /**
         * @brief Make a RECORD_START_LINEAR_ADDRESS record
         *
         * @param extendedInstructionPointer Extended instruction pointer (EIP)
         * @return HexRecord Record
         */
        static HexRecord makeStartLinearAddressRecord(uint32_t extendedInstructionPointer);

        /**
         * @brief Prepare a new record (with minimal parameter validation)
         *
         * @warning Preparing a new record with the use of this function is discouraged,
         *   because it is possible to make records which are invalid and do not follow
         *   the standards as per the Intel Hex Standard documentation.
         *   This is available for testing and unit tests only, and shall not be directly used in production code.
         *   Dedicated prepareRecord and makeRecord functions shall be used in production code.
         *
         * @param address Address
         * @param recordType Record type
         * @param data Data of the record
         * @throw std::out_of_range data.size() is out of range
         * @note data.size() interval range is [0, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @param checksum Checksum of the record
         * @note checksum value 0 recalculates and updates checksum
         */
        void prepareRecord(HexDataAddressType address, HexRecordType recordType, const HexDataVector& data, HexDataType checksum = 0);

        /**
         * @brief Prepare a new record (with minimal parameter validation)
         *
         * @warning Preparing a new record with the use of this function is discouraged,
         *   because it is possible to make records which are invalid and do not follow
         *   the standards as per the Intel Hex Standard documentation.
         *   This is available for testing and unit tests only, and shall not be directly used in production code.
         *   Dedicated prepareRecord and makeRecord functions shall be used in production code.
         *
         * @param address Address
         * @param recordType Record type
         * @param data Data of the record
         * @param dataSize Size of the data
         * @throw std::out_of_range dataSize is out of range
         * @note dataSize interval range is [0, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @param checksum Checksum of the record
         * @note checksum value 0 recalculates and updates checksum
         */
        void prepareRecord(HexDataAddressType address, HexRecordType recordType, const HexDataType* data, HexDataSizeType dataSize, HexDataType checksum = 0);

        /**
         * @brief Prepare a RECORD_DATA record
         *
         * @param address Address
         * @param data Data of the record
         * @throw std::out_of_range data.size() is out of range
         * @note data.size() interval range is [1, MAXIMUM_HEX_RECORD_DATA_SIZE]
         */
        void prepareDataRecord(HexDataAddressType address, const HexDataVector& data);

        /**
         * @brief Prepare a RECORD_DATA record
         *
         * @param address Address
         * @param data Data of the record
         * @param dataSize Size of the data
         * @throw std::out_of_range dataSize is out of range
         * @note dataSize interval range is [1, MAXIMUM_HEX_RECORD_DATA_SIZE]
         */
        void prepareDataRecord(HexDataAddressType address, const HexDataType* data, HexDataSizeType dataSize);

        /**
         * @brief Prepare a RECORD_END_OF_FILE record
         *
         */
        void prepareEndOfFileRecord();

        /**
         * @brief Prepare a RECORD_EXTENDED_SEGMENT_ADDRESS record
         *
         * @param segmentAddress Segment address
         */
        void prepareExtendedSegmentAddressRecord(uint16_t segmentAddress);

        /**
         * @brief Prepare a RECORD_START_SEGMENT_ADDRESS record
         *
         * @param codeSegment Code segment (CS)
         * @param instructionPointer Instruction pointer (IP)
         */
        void prepareStartSegmentAddressRecord(uint16_t codeSegment, uint16_t instructionPointer);

        /**
         * @brief Prepare a RECORD_EXTENDED_LINEAR_ADDRESS record
         *
         * @param linearAddress Linear address
         */
        void prepareExtendedLinearAddressRecord(uint16_t linearAddress);

        /**
         * @brief Prepare a RECORD_START_LINEAR_ADDRESS record
         *
         * @param extendedInstructionPointer Extended instruction pointer (EIP)
         */
        void prepareStartLinearAddressRecord(uint32_t extendedInstructionPointer);

        /**
         * @brief Recalculate and update checksum of the record
         *
         */
        void updateChecksum();

        /**
         * @brief Calculate checksum of the record
         *
         * @return HexDataType Checksum of the data
         */
        HexDataType calculateChecksum() const;

        /**
         * @brief Returns checksum validity
         *
         * @return true Checksum valid
         * @return false Checksum invalid
         */
        bool isValidChecksum() const;

        /**
         * @brief Get current record
         *
         * @return std::string Current record
         */
        std::string getRecord() const;

        /**
         * @brief Set new record and parse it
         *
         * @warning This function does not perform record checksum validation,
         *   to assert checksum validity as well, use isValidChecksum function
         *   after the call to this function.
         *
         * @param record New record
         * @return true Record successfully parsed
         * @return false Record failed to parse successfully
         */
        bool setRecord(const std::string& record);

        /**
         * @brief Get data size
         *
         * @return HexDataSizeType Data size
         */
        HexDataSizeType getDataSize() const;

        /**
         * @brief Set data size
         *
         * @param dataSize Data size
         * @note dataSize interval range is [0, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @return true Successfully set data size
         * @return false Failed to set data size
         */
        bool setDataSize(HexDataSizeType dataSize);

        /**
         * @brief Get address
         *
         * @return HexDataAddressType Address
         */
        HexDataAddressType getAddress() const;

        /**
         * @brief Set address
         *
         * @param address Address
         */
        void setAddress(HexDataAddressType address);

        /**
         * @brief Get the last address of the record
         *
         * @throw std::domain_error Record does not support or does not have addresses
         * @return HexDataAddressType Last data address of the record
         */
        HexDataAddressType getLastAddress() const;

        /**
         * @brief Check if record has address
         *
         * @return true Record has address (record is RECORD_DATA and size of data is > 0)
         * @return false Record does not have address
         */
        bool hasAddress() const;

        /**
         * @brief Check if record has address
         *
         * @param address Address
         * @throw std::domain_error Record does not support or does not have addresses
         * @return true Record has the address
         * @return false Record does not have the address
         */
        bool hasAddress(HexDataAddressType address) const;

        /**
         * @brief Get record type
         *
         * @return HexRecordType Record type
         */
        HexRecordType getRecordType() const;

        /**
         * @brief Set record type
         *
         * @param recordType Record type
         * @return true Successfully set record type
         * @return false Failed to set record type
         */
        bool setRecordType(HexRecordType recordType);

        /**
         * @brief Get data
         *
         * @param index Index of the data
         * @note index interval range is [0, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @return HexDataType Data
         * @note If index is out of range DEFAULT_HEX_RECORD_DATA_VALUE is returned
         */
        HexDataType getData(HexDataSizeType index) const;

        /**
         * @brief Get data
         *
         * @param data Pointer to the data to copy to
         * @param dataSize Size of the data to copy
         * @throw std::out_of_range dataSize is out of range
         * @param offset Offset of the data in range
         * @throw std::out_of_range offset is out of range
         * @note offset interval range is [0, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @return HexDataSizeType Size of the data copied
         */
        HexDataSizeType getData(HexDataType* data, HexDataSizeType dataSize, HexDataSizeType offset = 0) const;

        /**
         * @brief Get data
         *
         * @param data Vector to copy the data to
         * @param dataSize Size of the data to copy
         * @throw std::out_of_range dataSize is out of range
         * @param inOffset Input offset of the data in range
         * @note inOffset interval range is [0, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @throw std::out_of_range inOffset is out of range
         * @param outOffset Output offset of the data to copy to
         * @throw std::out_of_range outOffset is out of range
         * @return HexDataSizeType Size of the data copied
         */
        HexDataSizeType getData(HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType inOffset = 0, HexDataSizeType outOffset = 0) const;

        /**
         * @brief Set data
         *
         * @param index Offset of the data
         * @throw std::out_of_range index is out of range
         * @note index interval range is [0, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @param data Data
         */
        void setData(HexDataSizeType index, HexDataType data);

        /**
         * @brief Set data
         *
         * @param data Pointer to the data to copy from
         * @param dataSize Size of the data to copy
         * @throw std::out_of_range dataSize is out of range
         * @param offset Offset of the data
         * @throw std::out_of_range offset is out of range
         * @note offset interval range is [0, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @return HexDataSizeType Size of the data copied
         */
        HexDataSizeType setData(const HexDataType* data, HexDataSizeType dataSize, HexDataSizeType offset = 0);

        /**
         * @brief Set data
         *
         * @param data Vector to the data to copy from
         * @param dataSize Size of the data to copy
         * @throw std::out_of_range dataSize is out of range
         * @param inOffset Input offset of the data in range
         * @throw std::out_of_range inOffset is out of range
         * @note inOffset interval range is [0, MAXIMUM_HEX_RECORD_DATA_SIZE]
         * @param outOffset Output offset of the data to copy to
         * @throw std::out_of_range outOffset is out of range
         * @return HexDataSizeType Size of the data copied
         */
        HexDataSizeType setData(const HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType inOffset = 0, HexDataSizeType outOffset = 0);

        /**
         * @brief Get checksum
         *
         * @return HexDataType Checksum
         */
        HexDataType getChecksum() const;

        /**
         * @brief Set checksum
         *
         * @warning To update the record checksum correctly
         *   and safely, use updateChecksum function instead.
         *
         * @param checksum Checksum
         */
        void setChecksum(HexDataType checksum);

        /**
         * @brief Check validity of the record
         *
         * @warning This function does not perform record checksum validation,
         *   to assert checksum validity as well, use isValidChecksum function
         *   after the call to this function.
         *
         * @return true Record is valid
         * @return false Record is invalid
         */
        bool isValidRecord() const;

        /**
         * @brief Check validity of the record if it were of the provided type
         *
         * @warning This function does not perform record checksum validation.
         *
         * @param recordType Record type
         * @return true Record would be valid
         * @return false Record would not be valid
         */
        bool isValidRecord(HexRecordType recordType) const;

        /**
         * @brief Get the extended segment address
         *
         * @throw std::domain_error Record is not valid or is not a RECORD_EXTENDED_SEGMENT_ADDRESS record
         * @return uint16_t Extended segment address
         */
        uint16_t getExtendedSegmentAddress() const;

        /**
         * @brief Set the extended segment address
         *
         * @param segmentAddress Extended segment address
         * @return true Successfully set extended segment address
         * @return false Record is not valid or is not a RECORD_EXTENDED_SEGMENT_ADDRESS record
         */
        bool setExtendedSegmentAddress(uint16_t segmentAddress);

        /**
         * @brief Get the code segment of the start segment address
         *
         * @throw std::domain_error Record is not valid or is not a RECORD_START_SEGMENT_ADDRESS record
         * @return uint16_t Code segment (CS)
         */
        uint16_t getStartSegmentAddressCodeSegment() const;

        /**
         * @brief Set the code segment of the start segment address
         *
         * @param codeSegment Code segment
         * @return true Successfully set code segment
         * @return false Record is not valid or is not a RECORD_START_SEGMENT_ADDRESS record
         */
        bool setStartSegmentAddressCodeSegment(uint16_t codeSegment);

        /**
         * @brief Get the instruction pointer of the start segment address
         *
         * @throw std::domain_error Record is not valid or is not a RECORD_START_SEGMENT_ADDRESS record
         * @return uint16_t Instruction pointer (IP)
         */
        uint16_t getStartSegmentAddressInstructionPointer() const;

        /**
         * @brief Set the instruction pointer of the start segment address
         *
         * @param instructionPointer Instruction pointer
         * @return true Successfully set instruction pointer
         * @return false Record is not valid or is not a RECORD_START_SEGMENT_ADDRESS record
         */
        bool setStartSegmentAddressInstructionPointer(uint16_t instructionPointer);

        /**
         * @brief Get the extended linear address
         *
         * @throw std::domain_error Record is not valid or is not a RECORD_EXTENDED_LINEAR_ADDRESS record
         * @return uint16_t Extended linear address
         */
        uint16_t getExtendedLinearAddress() const;

        /**
         * @brief Set the extended linear address
         *
         * @param linearAddress Extended linear address
         * @return true Successfully set extended linear address
         * @return false Record is not valid or is not a RECORD_EXTENDED_LINEAR_ADDRESS record
         */
        bool setExtendedLinearAddress(uint16_t linearAddress);

        /**
         * @brief Get the extended instruction pointer of the start linear address
         *
         * @throw std::domain_error Record is not valid or is not a RECORD_START_LINEAR_ADDRESS record
         * @return uint32_t Extended instruction pointer
         */
        uint32_t getStartLinearAddressExtendedInstructionPointer() const;

        /**
         * @brief Set the extended instruction pointer of the start linear address
         *
         * @param extendedInstructionPointer
         * @return true Successfully set extended instruction pointer
         * @return false Record is not valid or is not a RECORD_START_LINEAR_ADDRESS record
         */
        bool setStartLinearAddressExtendedInstructionPointer(uint32_t extendedInstructionPointer);
    protected:
        /**
         * @brief Address offset
         *
         */
        HexDataAddressType address;

        /**
         * @brief Record type of type HexRecordType
         *
         */
        HexRecordType recordType;

        /**
         * @brief Data of the record
         *
         */
        HexDataVector data;

        /**
         * @brief Checksum of the record
         *
         */
        HexDataType checksum;
};

/**
 * @brief Shared pointer of HexRecord
 *
 */
typedef std::shared_ptr<HexRecord> HexRecordPtr;

/**
 * @brief Unique pointer of HexRecord
 *
 */
typedef std::unique_ptr<HexRecord> HexRecordUniquePtr;

END_NAMESPACE_LIBIHEX
