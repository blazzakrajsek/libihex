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

#include <map>
#include <ihex/hex_address.hpp>
#include <ihex/hex_record.hpp>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

/**
 * @brief Hex section types
 *
 */
enum class HexSectionType : HexDataType
{
    /**
     * @brief Section of RECORD_DATA only (8-bit format only)
     *
     */
    SECTION_DATA = 0,

    /**
     * @brief Section of one RECORD_END_OF_FILE only (all formats)
     *
     */
    SECTION_END_OF_FILE = 1,

    /**
     * @brief Section of one RECORD_EXTENDED_SEGMENT_ADDRESS and RECORD_DATA (16-bit format only)
     *
     */
    SECTION_EXTENDED_SEGMENT_ADDRESS = 2,

    /**
     * @brief Section of one RECORD_START_SEGMENT_ADDRESS only (16-bit format only)
     *
     */
    SECTION_START_SEGMENT_ADDRESS = 3,

    /**
     * @brief Section of one RECORD_EXTENDED_LINEAR_ADDRESS and RECORD_DATA (32-bit format only)
     *
     */
    SECTION_EXTENDED_LINEAR_ADDRESS = 4,

    /**
     * @brief Section of one RECORD_START_LINEAR_ADDRESS only (32-bit format only)
     *
     */
    SECTION_START_LINEAR_ADDRESS = 5,

    SECTION_MIN = SECTION_DATA,
    SECTION_MAX = SECTION_START_LINEAR_ADDRESS
};

/**
 * @brief HexSection class
 *
 */
class HexSection final
{
    public:
        /**
         * @brief Construct a new HexSection object
         *
         */
        explicit HexSection();

        /**
         * @brief Construct a new HexSection object
         *
         * @param record Initial record
         * @throw std::runtime_error Failed to parse record in string format
         * @throw std::logic_error Record checksum is not valid
         */
        explicit HexSection(const std::string& record);

        /**
         * @brief Construct a new HexSection object
         *
         * @param record Initial record
         * @throw std::logic_error Record checksum is not valid
         */
        explicit HexSection(const HexRecord& record);

        /**
         * @brief Construct a new Hex Section object
         *
         * @param sectionType Section type
         */
        explicit HexSection(HexSectionType sectionType);

        /**
         * @brief Copy-construct a new HexSection object
         *
         * @param section Section to copy-construct from
         */
        HexSection(const HexSection& section);

        /**
         * @brief Move-construct a new HexSection object
         *
         * @param section Section to move-construct from
         */
        HexSection(HexSection&& section) = default;

        /**
         * @brief Copy-assignment operator
         *
         * @param section Section to copy-assign
         * @return HexSection& Assigned section
         */
        HexSection& operator=(const HexSection& section) = default;

        /**
         * @brief Move-assignment operator
         *
         * @param section Section to move-assign
         * @return HexSection& Assigned section
         */
        HexSection& operator=(HexSection&& section) = default;

        /**
         * @brief Array index operator
         *
         * @param address Relative address
         * @throw std::domain_error Section does not support addresses
         * @throw std::out_of_range address is out of range
         * @return HexDataType Data
         */
        HexDataType operator[](HexDataAddressType address) const;

        /**
         * @brief Array index operator
         *
         * @param address Relative address
         * @throw std::domain_error Section does not support addresses
         * @throw std::out_of_range address is out of range
         * @return HexDataType& Data
         */
        HexDataType& operator[](HexDataAddressType address);

        /**
         * @brief Make a SECTION_DATA record
         *
         * @param address Relative address
         * @param data Data of the record
         * @param dataSize Size of the data
         * @return HexSection Section
         */
        static HexSection makeDataSection(HexDataAddressType address, const HexDataType* data, HexDataSizeType dataSize);

        /**
         * @brief Make a SECTION_DATA section
         *
         * @param address Relative address
         * @param data Data
         * @param dataSize Data size
         * @param offset Offset
         * @return HexSection Section
         */
        static HexSection makeDataSection(HexDataAddressType address, const HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset = 0);

        /**
         * @brief Make a SECTION_END_OF_FILE section
         *
         * @return HexSection Section
         */
        static HexSection makeEndOfFileSection();

        /**
         * @brief Make a SECTION_EXTENDED_SEGMENT_ADDRESS section
         *
         * @param segmentAddress Extended segment address
         * @return HexSection Section
         */
        static HexSection makeExtendedSegmentAddressSection(uint16_t segmentAddress);

        /**
         * @brief Make a SECTION_EXTENDED_SEGMENT_ADDRESS section
         *
         * @param segmentAddress Extended segment address
         * @param address Relative address
         * @param data Data
         * @param dataSize Data size
         * @return HexSection Section
         */
        static HexSection makeExtendedSegmentAddressSection(uint16_t segmentAddress, HexDataAddressType address,
            const HexDataType* data, HexDataSizeType dataSize);

        /**
         * @brief Make a SECTION_EXTENDED_SEGMENT_ADDRESS section
         *
         * @param segmentAddress Extended segment address
         * @param address Relative address
         * @param data Data
         * @param dataSize Data size
         * @param offset Offset
         * @return HexSection Section
         */
        static HexSection makeExtendedSegmentAddressSection(uint16_t segmentAddress, HexDataAddressType address,
            const HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset = 0);

        /**
         * @brief Make a SECTION_EXTENDED_SEGMENT_ADDRESS section
         *
         * @param segmentAddress Extended segment address
         * @param dataRecord Data record
         * @throw std::logic_error Failed to push record to the section
         * @return HexSection Section
         */
        static HexSection makeExtendedSegmentAddressSection(uint16_t segmentAddress, const std::string& dataRecord);

        /**
         * @brief Make a SECTION_EXTENDED_SEGMENT_ADDRESS section
         *
         * @param segmentAddress Extended segment address
         * @param dataRecord Data record
         * @throw std::logic_error Failed to push record to the section
         * @return HexSection Section
         */
        static HexSection makeExtendedSegmentAddressSection(uint16_t segmentAddress, const HexRecord& dataRecord);

        /**
         * @brief Make a SECTION_START_SEGMENT_ADDRESS section
         *
         * @param codeSegment Code segment (CS)
         * @param instructionPointer Instruction pointer (IP)
         * @return HexSection Section
         */
        static HexSection makeStartSegmentAddressSection(uint16_t codeSegment, uint16_t instructionPointer);

        /**
         * @brief Make a SECTION_EXTENDED_LINEAR_ADDRESS section
         *
         * @param linearAddress Extended linear address
         * @return HexSection Section
         */
        static HexSection makeExtendedLinearAddressSection(uint16_t linearAddress);

        /**
         * @brief Make a SECTION_EXTENDED_LINEAR_ADDRESS section
         *
         * @param linearAddress Extended linear address
         * @param address Relative address
         * @param data Data
         * @param dataSize Data size
         * @param offset Offset
         * @return HexSection Section
         */
        static HexSection makeExtendedLinearAddressSection(uint16_t linearAddress, HexDataAddressType address,
            const HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset = 0);

        /**
         * @brief Make a SECTION_EXTENDED_LINEAR_ADDRESS section
         *
         * @param linearAddress Extended linear address
         * @param address Relative address
         * @param data Data
         * @param dataSize Data size
         * @return HexSection Section
         */
        static HexSection makeExtendedLinearAddressSection(uint16_t linearAddress, HexDataAddressType address,
            const HexDataType* data, HexDataSizeType dataSize);

        /**
         * @brief Make a SECTION_EXTENDED_LINEAR_ADDRESS section
         *
         * @param linearAddress Extended linear address
         * @param dataRecord Data record
         * @throw std::logic_error Failed to push record to the section
         * @return HexSection Section
         */
        static HexSection makeExtendedLinearAddressSection(uint16_t linearAddress, const std::string& dataRecord);

        /**
         * @brief Make a SECTION_EXTENDED_LINEAR_ADDRESS section
         *
         * @param linearAddress Extended linear address
         * @param dataRecord Data record
         * @throw std::logic_error Failed to push record to the section
         * @return HexSection Section
         */
        static HexSection makeExtendedLinearAddressSection(uint16_t linearAddress, const HexRecord& dataRecord);

        /**
         * @brief Make a SECTION_START_LINEAR_ADDRESS section
         *
         * @param extendedInstructionPointer Extended instruction pointer (EIP)
         * @return HexSection Section
         */
        static HexSection makeStartLinearAddressSection(uint32_t extendedInstructionPointer);

        /**
         * @brief Get the section type
         *
         * @return HexSectionType Hex section type
         */
        HexSectionType getSectionType() const;

        /**
         * @brief Convert section to SECTION_DATA
         *
         */
        void convertToDataSection();

        /**
         * @brief Convert section to SECTION_END_OF_FILE
         *
         * @note All data blocks are deleted when section is converted
         */
        void convertToEndOfFileSection();

        /**
         * @brief Convert section to SECTION_EXTENDED_SEGMENT_ADDRESS
         *
         * @param segmentAddress Extended segment address
         */
        void convertToExtendedSegmentAddressSection(uint16_t segmentAddress);

        /**
         * @brief Convert section to SECTION_START_SEGMENT_ADDRESS
         *
         * @param codeSegment Code segment (CS)
         * @param instructionPointer Instruction pointer (IP)
         *
         * @note All data blocks are deleted when section is converted
         */
        void convertToStartSegmentAddressSection(uint16_t codeSegment, uint16_t instructionPointer);

        /**
         * @brief Convert section to SECTION_EXTENDED_LINEAR_ADDRESS
         *
         * @param linearAddress Extended linear address
         */
        void convertToExtendedLinearAddressSection(uint16_t linearAddress);

        /**
         * @brief Convert section to SECTION_START_LINEAR_ADDRESS
         *
         * @param extendedInstructionPointer Extended instruction pointer (EIP)
         *
         * @note All data blocks are deleted when section is converted
         */
        void convertToStartLinearAddressSection(uint32_t extendedInstructionPointer);

        /**
         * @brief Get the extended segment address
         *
         * @throw std::domain_error Section is not a SECTION_EXTENDED_SEGMENT_ADDRESS section
         * @return uint16_t Extended segment address
         */
        uint16_t getExtendedSegmentAddress() const;

        /**
         * @brief Set the extended segment address
         *
         * @param segmentAddress Extended segment address
         * @return true Successfully set extended segment address
         * @return false Section is not a SECTION_EXTENDED_SEGMENT_ADDRESS section
         */
        bool setExtendedSegmentAddress(uint16_t segmentAddress);

        /**
         * @brief Get the code segment of the start segment address
         *
         * @throw std::domain_error Section is not a SECTION_START_SEGMENT_ADDRESS section
         * @return uint16_t Code segment (CS)
         */
        uint16_t getStartSegmentAddressCodeSegment() const;

        /**
         * @brief Set the code segment of the start segment address
         *
         * @param codeSegment Code segment
         * @return true Successfully set code segment
         * @return false Section is not a SECTION_START_SEGMENT_ADDRESS section
         */
        bool setStartSegmentAddressCodeSegment(uint16_t codeSegment);

        /**
         * @brief Get the instruction pointer of the start segment address
         *
         * @throw std::domain_error Section is not a SECTION_START_SEGMENT_ADDRESS section
         * @return uint16_t Instruction pointer (IP)
         */
        uint16_t getStartSegmentAddressInstructionPointer() const;

        /**
         * @brief Set the instruction pointer of the start segment address
         *
         * @param instructionPointer Instruction pointer
         * @return true Successfully set instruction pointer
         * @return false Section is not a SECTION_START_SEGMENT_ADDRESS section
         */
        bool setStartSegmentAddressInstructionPointer(uint16_t instructionPointer);

        /**
         * @brief Get the extended linear address
         *
         * @throw std::domain_error Section is not a SECTION_EXTENDED_LINEAR_ADDRESS section
         * @return uint16_t Extended linear address
         */
        uint16_t getExtendedLinearAddress() const;

        /**
         * @brief Set the extended linear address
         *
         * @param linearAddress Extended linear address
         * @return true Successfully set extended linear address
         * @return false Section is not a SECTION_EXTENDED_LINEAR_ADDRESS section
         */
        bool setExtendedLinearAddress(uint16_t linearAddress);

        /**
         * @brief Get the extended instruction pointer of the start linear address
         *
         * @throw std::domain_error Section is not a SECTION_START_LINEAR_ADDRESS section
         * @return uint32_t Extended instruction pointer
         */
        uint32_t getStartLinearAddressExtendedInstructionPointer() const;

        /**
         * @brief Set the extended instruction pointer of the start linear address
         *
         * @param extendedInstructionPointer
         * @return true Successfully set extended instruction pointer
         * @return false Section is not a SECTION_START_LINEAR_ADDRESS section
         */
        bool setStartLinearAddressExtendedInstructionPointer(uint32_t extendedInstructionPointer);

        /**
         * @brief Get the absolute address map of the section
         *
         * @throw std::domain_error Section does not supported addresses
         * @return HexAddressMap Absolute address map
         */
        HexAddressMap getAddressMap() const;

        /**
         * @brief Get the used absolute address data map of the section
         *
         * @throw std::domain_error Section does not support addresses
         * @return HexAddressMap Used absolute data address map
         */
        HexAddressMap getDataMap() const;

        /**
         * @brief Get the size of records
         *
         * @return size_t Size of records
         */
        size_t getSize() const;

        /**
         * @brief Does section have data?
         *
         * @return true Section contains data
         * @return false Section does not contain data
         */
        bool hasData() const;

        /**
         * @brief Get the size of data
         *
         * @return HexDataSizeType Size of the data
         */
        HexDataSizeType getDataSize() const;

        /**
         * @brief Get the unused data fill value
         *
         * @return HexDataType Unused data fill value
         */
        HexDataType getUnusedDataFillValue() const;

        /**
         * @brief Set the unused data fill value
         *
         * @param unusedDataFillValue Unused data fill value
         */
        void setUnusedDataFillValue(HexDataType unusedDataFillValue = DEFAULT_HEX_RECORD_DATA_VALUE);

        /**
         * @brief Get the default data size of the data records
         *
         * @return HexDataSizeType Default data size
         */
        HexDataSizeType getDefaultDataSize() const;

        /**
         * @brief Set the default data size of the data records
         *
         * @param defaultDataSize Default data size
         * @throw std::out_of_range Default data size is less than 2
         */
        void setDefaultDataSize(HexDataSizeType defaultDataSize = DEFAULT_HEX_RECORD_DATA_SIZE);

        /**
         * @brief Get the record
         *
         * @param record Record
         * @param index Index of the record
         * @return true Getting the record successful
         * @return false Getting the record failed
         */
        bool getRecord(HexRecord& record, size_t index = 0) const;

        /**
         * @brief Get the record
         *
         * @param record Record
         * @param index Index of the record
         * @return true Getting the record successful
         * @return false Getting the record failed
         */
        bool getRecord(std::string& record, size_t index = 0) const;

        /**
         * @brief Get the record
         *
         * @param index Index of the record
         * @throw std::out_of_range index out of range
         * @return HexRecord Record
         */
        HexRecord getRecord(size_t index = 0) const;

        /**
         * @brief Check if record can be pushed to the section
         *
         * @param record Record
         * @return true Record can be pushed to the section
         * @return false Record can not be pushed to the section
         */
        bool canPushRecord(const HexRecord& record) const;

        /**
         * @brief Check if record can be pushed to the section
         *
         * @param record Record
         * @return true Record can be pushed to the section
         * @return false Record can not be pushed to the section
         */
        bool canPushRecord(const std::string& record) const;

        /**
         * @brief Push record to the section
         *
         * @param record Record
         * @return true Push successful
         * @return false Push failed
         */
        bool pushRecord(const HexRecord& record);

        /**
         * @brief Push record to the section
         *
         * @param record Record
         * @return true Push successful
         * @return false Push failed
         */
        bool pushRecord(const std::string& record);

        /**
         * @brief Check if section supports addresses
         *
         * @return true Section supports addresses
         * @return false Section does not support addresses
         */
        bool hasAddress() const;

        /**
         * @brief Get the lower relative address of the section
         *
         * @throw std::domain_error Section does not support addresses
         * @throw std::out_of_range Section does not have any addresses
         *
         * @return HexDataAddressType Lower relative address of the section
         */
        HexDataAddressType getLowerAddress() const;

        /**
         * @brief Get the upper relative address of the section
         *
         * @throw std::domain_error Section does not support addresses
         * @throw std::out_of_range Section does not have any addresses
         *
         * @return HexDataAddressType Upper relative address of the section
         */
        HexDataAddressType getUpperAddress() const;

        /**
         * @brief Get the minimum absolute address of the section
         *
         * @throw std::domain_error Section does not support addresses
         *
         * @return HexAddressType Minimum absolute address of the section
         */
        HexAddressType getMinAbsoluteAddress() const;

        /**
         * @brief Get the maximum absolute address of the section
         *
         * @throw std::domain_error Section does not support addresses
         *
         * @return HexAddressType Maximum absolute address of the section
         */
        HexAddressType getMaxAbsoluteAddress() const;

        /**
         * @brief Check if absolute address exists in the section
         *
         * @param address Absolute address
         * @throw std::domain_error Section does not support addresses
         * @return true Address is a valid absolute address in the section
         * @return false Address is not a valid absolute address in the section
         */
        bool hasAbsoluteAddress(HexAddressType address) const;

        /**
         * @brief Convert the relative address to the absolute address
         *
         * @param address Relative address
         * @throw std::domain_error Section does not support addresses
         * @return HexAddressType Absolute address
         */
        HexAddressType getAbsoluteAddress(HexDataAddressType address) const;

        /**
         * @brief Convert the absolute address to the relative address
         *
         * @param address Absolute address
         * @throw std::domain_error Section does not support addresses
         * @throw std::out_of_range Absolute address does not exist in the section
         * @return HexDataAddressType Relative address
         */
        HexDataAddressType getRelativeAddress(HexAddressType address) const;

        /**
         * @brief Check if addresses of two sections intersect
         *
         * @param section Section
         * @throw std::domain_error Section does not support addresses
         * @throw std::domain_error Section provided by the parameter does not support addresses
         * @throw std::domain_error Both sections are not of the equal type
         * @return true Addresses intersect
         * @return false Addresses do not intersect
         */
        bool checkIntersect(const HexSection& section) const;

        /**
         * @brief Find index of the record which contains a relative address
         *
         * @param address Relative address
         * @throw std::domain_error Section does not support addresses
         * @return size_t Index of the record which contains the record
         * @note std::string::npos is returned for address that is not found in the section which supports addresses
         */
        size_t findAddress(HexDataAddressType address) const;

        /**
         * @brief Find index of record which contains relative address before the provided address
         *
         * @param address Relative address
         * @throw std::domain_error Section does not support addresses
         * @return size_t Index of the record which contains relative address before the provided address
         * @note std::string::npos is returned for address that is not found in the section which supports addresses
         */
        size_t findPreviousRecord(HexDataAddressType address) const;

        /**
         * @brief Find index of record which contains relative address after the provided address
         *
         * @param address Relative address
         * @throw std::domain_error Section does not support addresses
         * @return size_t Index of the record which contains relative address after the provided address
         * @note std::string::npos is returned for address that is not found in the section which supports addresses
         */
        size_t findNextRecord(HexDataAddressType address) const;

        /**
         * @brief Clear all data records
         *
         */
        void clearData();

        /**
         * @brief Clear relative data address
         *
         * @param address Relative address
         * @throw std::domain_error Section does not support addresses
         */
        void clearData(HexDataAddressType address);

        /**
         * @brief Clear relative data addresses
         *
         * @param address Relative address
         * @param size Size of the addresses
         * @throw std::domain_error Section does not support addresses
         * @return HexDataSizeType Size of addresses cleared
         */
        HexDataSizeType clearData(HexDataAddressType address, HexDataSizeType size);

        /**
         * @brief Get data from the data records
         *
         * @param address Relative address
         * @throw std::domain_error Section does not support addresses
         * @return HexDataType Data
         * @note unusedDataFillValue is returned for addresses which are unused in the section
         */
        HexDataType getData(HexDataAddressType address) const;

        /**
         * @brief Get data from the data records
         *
         * @param address Relative address
         * @param data Data
         * @warning data must be large enough to fit dataSize amount of data
         * @note unusedDataFillValue is returned for addresses which are unused in the section
         * @param dataSize Size of the data
         * @throw std::domain_error Section does not support addresses
         * @return HexDataSizeType Size of the data copied
         */
        HexDataSizeType getData(HexDataAddressType address, HexDataType* data, HexDataSizeType dataSize) const;

        /**
         * @brief Get data from the data records
         *
         * @param address Relative address
         * @param data Data
         * @note unusedDataFillValue is returned for addresses which are unused in the section
         * @note Data is appended using std::vector::push_back()
         * @param dataSize Size of the data
         * @param offset Offset of the data
         * @throw std::underflow_error Size of the data vector is smaller than the offset
         * @throw std::domain_error Section does not support addresses
         * @return HexDataSizeType Size of the data copied
         */
        HexDataSizeType getData(HexDataAddressType address, HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset = 0) const;

        /**
         * @brief Set or append data in the data records
         *
         * @param address Relative address
         * @param data Data
         * @throw std::domain_error Section does not support addresses
         */
        void setData(HexDataAddressType address, HexDataType data);

        /**
         * @brief Set or append data in the data records
         *
         * @param address Relative address
         * @param data Data
         * @param dataSize Size of the data
         * @warning data must be large enough to fit dataSize amount of data
         * @throw std::domain_error Section does not support addresses
         * @return HexDataSizeType Size of the data set or appended
         */
        HexDataSizeType setData(HexDataAddressType address, const HexDataType* data, HexDataSizeType dataSize);

        /**
         * @brief Set or append data in the data records
         *
         * @param address Relative address
         * @param data Data
         * @param dataSize Size of the data
         * @param offset Offset of the data
         * @throw std::domain_error Section does not support addresses
         * @return HexDataSizeType Size of the data set or appended
         */
        HexDataSizeType setData(HexDataAddressType address, const HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset = 0);

        /**
         * @brief Fill data in the data records
         *
         * @param address Relative address
         * @param dataSize Size of the data
         * @param data Data
         * @return HexDataSizeType Size of the data filled
         */
        HexDataSizeType fillData(HexDataAddressType address, HexDataSizeType dataSize, HexDataType data);

        /**
         * @brief Compact adjacent data records
         *
         */
        void compact();
    protected:
        /**
         * @brief Hex data map
         *
         */
        typedef std::map<HexDataAddressType, HexDataVector> HexDataMap;

        /**
         * @brief Main record of the section
         *
         */
        HexRecord section;

        /**
         * @brief Hex data blocks
         *
         */
        HexDataMap blocks;

        /**
         * @brief Unused data fill value
         *
         */
        HexDataType unusedDataFillValue;

        /**
         * @brief Default data record size
         *
         */
        HexDataSizeType defaultDataSize;
};

/**
 * @brief Shared pointer of HexSection
 *
 */
typedef std::shared_ptr<HexSection> HexSectionPtr;

/**
 * @brief Unique pointer of HexSection
 *
 */
typedef std::unique_ptr<HexSection> HexSectionUniquePtr;

/**
 * @brief Hex section vector
 *
 */
typedef std::vector<HexSection> HexSectionVector;

END_NAMESPACE_LIBIHEX
