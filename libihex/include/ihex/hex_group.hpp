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

#include <vector>
#include <ihex/hex_address.hpp>
#include <ihex/hex_record.hpp>
#include <ihex/hex_section.hpp>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

/**
 * @brief HexGroup class
 *
 */
class HexGroup
{
    public:
        /**
         * @brief Construct a new HexGroup object
         *
         */
        explicit HexGroup();

        /**
         * @brief Destroy the HexGroup object
         *
         */
        virtual ~HexGroup();

        /**
         * @brief Copy-construct a new HexGroup object
         *
         * @param group Group to copy-construct from
         */
        HexGroup(const HexGroup& group) = default;

        /**
         * @brief Move-construct a new HexGroup object
         *
         * @param group Group to move-construct from
         */
        HexGroup(HexGroup&& group) = default;

        /**
         * @brief Copy-assignment operator
         *
         * @param group Group to copy-assign
         * @return HexGroup& Assigned group
         */
        HexGroup& operator=(const HexGroup& group) = default;

        /**
         * @brief Move-assignment operator
         *
         * @param group Group to move-assign
         * @return HexGroup& Assigned group
         */
        HexGroup& operator=(HexGroup&& group) = default;

        /**
         * @brief Array index operator
         *
         * @param address Absolute address
         * @throw std::out_of_range address is out of range
         * @return HexDataType Data
         */
        virtual HexDataType operator[](HexAddressType address) const;

        /**
         * @brief Array index operator
         *
         * @param address Absolute address
         * @throw std::out_of_range address is out of range
         * @return HexDataType& Data
         */
        virtual HexDataType& operator[](HexAddressType address);

        /**
         * @brief Get the size of the section list in the group
         *
         * @return size_t Size of the section list
         */
        virtual size_t getSize() const;

        /**
         * @brief Get the section
         *
         * @param index Section index
         * @throw std::out_of_range index is out of range
         * @return const HexSection& Section
         */
        virtual const HexSection& getSection(size_t index) const;

        /**
         * @brief Get the section
         *
         * @warning Changing the section through the reference is discouraged,
         *   because it is possible to change and break the internal state of the HexGroup class.
         *   This is available for testing and unit tests only, and shall not be directly used in production code.
         *
         * @param index Section index
         * @throw std::out_of_range index is out of range
         * @return HexSection& Section
         */
        virtual HexSection& getSection(size_t index);

        /**
         * @brief Find section which contains the absolute address
         *
         * @param address Absolute address
         * @return size_t Section index
         * @note std::string::npos is returned for section that is not found in the group
         */
        virtual size_t findSection(HexAddressType address) const;

        /**
         * @brief Find section before the section which contains the absolute address
         *
         * @param address Absolute address
         * @return size_t Section index
         * @note std::string::npos is returned when no previous section is found
         */
        virtual size_t findPreviousSection(HexAddressType address) const;

        /**
         * @brief Find section after the section which contains the absolute address
         *
         * @param address Absolute address
         * @return size_t Section index
         * @note std::string::npos is returned when no next section is found
         */
        virtual size_t findNextSection(HexAddressType address) const;

        /**
         * @brief Check if the section intersects with any section of the group
         *
         * @param type Type of address
         * @param section Section to check intersect against
         * @throw std::domain_error Section does not support addresses
         * @throw std::domain_error Section is not compatible with the group type
         * @return size_t Section index of the section with the intersect
         * @note std::string::npos is returned if no intersect found
         */
        virtual size_t checkIntersect(HexAddressEnumType type, const HexSection& section) const;

        /**
         * @brief Check if the section intersect with any section of the group
         *
         * @param section Section to check intersect against
         * @throw std::domain_error Section does not support addresses
         * @throw std::domain_error Section is not compatible with the group type
         * @return size_t Section index of the section with the intersect
         * @note std::string::npos is returned if no intersect found
         */
        virtual size_t checkIntersect(const HexSection& section) const;

        /**
         * @brief Create section with the absolute address
         *
         * @param type Type of the address
         * @throw std::domain_error Type of address not applicable to the existing sections of the group
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @throw std::runtime_error Unable to create aligned SECTION_EXTENDED_SEGMENT_ADDRESS section (group type ADDRESS_I16HEX only)
         * @return size_t Section index
         * @note For the existing section with the absolute address the existing section index is returned
         */
        virtual size_t createSection(HexAddressEnumType type, HexAddressType address);

        /**
         * @brief Create section with the absolute address
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @throw std::runtime_error Unable to create aligned SECTION_EXTENDED_SEGMENT_ADDRESS section (group type ADDRESS_I16HEX only)
         * @return size_t Section index
         * @note For the existing section with the absolute address the existing section index is returned
         */
        virtual size_t createSection(HexAddressType address);

        /**
         * @brief Create section with the range of absolute addresses
         *
         * @param type Type of the address
         * @throw std::domain_error Type of address not applicable to the existing sections of the group
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @throw std::runtime_error Unable to create aligned SECTION_EXTENDED_SEGMENT_ADDRESS section (group type ADDRESS_I16HEX only)
         * @param size Size of the range
         * @throw std::out_of_range size is zero
         * @note Absolute address range interval is [address, address + size - 1]
         * @return size_t Index of the last found or created section
         * @note For the existing absolute address within the existing section the existing section index is returned
         */
        virtual size_t createSection(HexAddressEnumType type, HexAddressType address, HexDataSizeType size);

        /**
         * @brief Create section with the range of absolute addresses
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @throw std::runtime_error Unable to create aligned SECTION_EXTENDED_SEGMENT_ADDRESS section (group type ADDRESS_I16HEX only)
         * @param size Size of the range
         * @throw std::out_of_range size is zero
         * @note Absolute address range interval is [address, address + size - 1]
         * @return size_t Index of the last found or created section
         * @note For the existing absolute address within the existing section the existing section index is returned
         */
        virtual size_t createSection(HexAddressType address, HexDataSizeType size);

        /**
         * @brief Remove section
         *
         * @param index Section index
         * @throw std::out_of_range index is out of range
         */
        virtual void removeSection(size_t index);

        /**
         * @brief Clear all sections in the group
         *
         */
        virtual void clearSections();

        /**
         * @brief Check if section can be pushed to the group
         *
         * @param section Section
         * @param index Suitable index of the section to be pushed to
         * @note index is set to std::string:npos if push of the section
         *   is not allowed or if suitable index is not found
         * @return true Section can be pushed to the group
         * @return false Section can not be pushed to the group
         */
        virtual bool canPushSection(const HexSection& section, size_t& index) const;

        /**
         * @brief Check if section can be pushed to the group
         *
         * @param section Section
         * @return true Section can be pushed to the group
         * @return false Section can not be pushed to the group
         */
        virtual bool canPushSection(const HexSection& section) const;

        /**
         * @brief Push section to the group
         *
         * @param section Section
         * @return size_t Section index
         * @note std::string:npos is returned if section is not pushed
         */
        virtual size_t pushSection(const HexSection& section);

        /**
         * @brief Get the absolute address map of all the sections
         *
         * @return HexAddressMap Absolute address map
         */
        virtual HexAddressMap getAddressMap() const;

        /**
         * @brief Get the used data absolute address map of the group
         *
         * @return HexAddressMap Used data absolute address map
         */
        HexAddressMap getDataMap() const;

        /**
         * @brief Clear all sections with data records
         *
         */
        virtual void clearData();

        /**
         * @brief Clear data from the data records
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         */
        virtual void clearData(HexAddressType address);

        /**
         * @brief Clear data from the data records
         *
         * @param address Absolute address
         * @param dataSize Size of the range
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @return HexDataSizeType Size of the data cleared
         */
        virtual HexDataSizeType clearData(HexAddressType address, HexDataSizeType dataSize);

        /**
         * @brief Get data from the data records
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @return HexDataType Data
         * @note unusedDataFillValue is returned for addresses which do not have data
         */
        virtual HexDataType getData(HexAddressType address) const;

        /**
         * @brief Get data from the data records
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @param data Data
         * @warning data must be large enough to fit dataSize amount of data
         * @note unusedDataFillValue is used for addresses which do not have data
         * @param dataSize Size of the data
         * @return HexDataSizeType Size of the actual data used from the data records
         * @note dataSize amount of data is updated in data, however returned data size can be less than dataSize
         */
        virtual HexDataSizeType getData(HexAddressType address, HexDataType* data, HexDataSizeType dataSize) const;

        /**
         * @brief Get data from the data records
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @param data Data
         * @note Data is appended using std::vector::push_back()
         * @note unusedDataFillValue is used for addresses which do not have data
         * @param dataSize Size of the data
         * @param offset Offset of the data
         * @throw std::underflow_error Size of the data vector is smaller than the offset
         * @return HexDataSizeType Size of the actual data used from the data sections
         * @note dataSize amount of data is pushed-back to the data, however returned data size can be less than dataSize
         */
        virtual HexDataSizeType getData(HexAddressType address, HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset = 0) const;

        /**
         * @brief Set or append data in the data records
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @throw std::runtime_error Unable to create aligned SECTION_EXTENDED_SEGMENT_ADDRESS section (group type ADDRESS_I16HEX only)
         * @param data Data
         */
        virtual void setData(HexAddressType address, HexDataType data);

        /**
         * @brief Set or append data in the data records
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @throw std::runtime_error Unable to create aligned SECTION_EXTENDED_SEGMENT_ADDRESS section (group type ADDRESS_I16HEX only)
         * @param data Data
         * @param dataSize Size of the data
         * @return HexDataSizeType Size of the data set
         */
        virtual HexDataSizeType setData(HexAddressType address, HexDataType* data, HexDataSizeType dataSize);

        /**
         * @brief Set or append data in the data records
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @throw std::runtime_error Unable to create aligned SECTION_EXTENDED_SEGMENT_ADDRESS section (group type ADDRESS_I16HEX only)
         * @param data Data
         * @param dataSize Size of the data
         * @param offset Offset of the data
         * @return HexDataSizeType Size of the data set
         */
        virtual HexDataSizeType setData(HexAddressType address, HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset = 0);

        /**
         * @brief Fill data in the data records
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for the current group type
         * @throw std::runtime_error Unable to create aligned SECTION_EXTENDED_SEGMENT_ADDRESS section (group type ADDRESS_I16HEX only)
         * @param dataSize Size of the data
         * @param data Data value
         * @return HexDataSizeType Size of the data filled
         */
        virtual HexDataSizeType fillData(HexAddressType address, HexDataSizeType dataSize, HexDataType data);

        /**
         * @brief Get the group type
         *
         * @return HexAddressEnumType Group type
         */
        virtual HexAddressEnumType getGroupType() const;

        /**
         * @brief Check if group does have data sections
         *
         * @return true Group does have data sections
         * @return false Group does have data sections
         */
        virtual bool hasDataSections() const;

        /**
         * @brief Get the unused data fill value
         *
         * @return HexDataType Unused data fill value
         */
        virtual HexDataType getUnusedDataFillValue() const;

        /**
         * @brief Set the unused data fill value
         *
         * @param unusedDataFillValue Unused data fill value
         */
        virtual void setUnusedDataFillValue(HexDataType unusedDataFillValue = DEFAULT_HEX_RECORD_DATA_VALUE);

        /**
         * @brief Check if group can have SECTION_START_SEGMENT_ADDRESS section
         *
         * @return true Group can have SECTION_START_SEGMENT_ADDRESS section
         * @return false Group can not have SECTION_START_SEGMENT_ADDRESS section
         */
        virtual bool supportsStartSegmentAddress() const;

        /**
         * @brief Check if group has SECTION_START_SEGMENT_ADDRESS section present
         *
         * @return true SECTION_START_SEGMENT_ADDRESS section is present
         * @return false SECTION_START_SEGMENT_ADDRESS section is not present
         */
        virtual bool hasStartSegmentAddress() const;

        /**
         * @brief Clear SECTION_START_SEGMENT_ADDRESS section if present
         *
         */
        virtual void clearStartSegmentAddress();

        /**
         * @brief Get the SECTION_START_SEGMENT_ADDRESS section
         *
         * @throw std::runtime_error SECTION_START_SEGMENT_ADDRESS section not supported or found
         * @return const HexSection& SECTION_START_SEGMENT_ADDRESS section
         */
        virtual const HexSection& getStartSegmentAddressSection() const;

        /**
         * @brief Get the SECTION_START_SEGMENT_ADDRESS section
         *
         * @throw std::runtime_error SECTION_START_SEGMENT_ADDRESS section not supported or found
         * @return HexSection& SECTION_START_SEGMENT_ADDRESS section
         */
        virtual HexSection& getStartSegmentAddressSection();

        /**
         * @brief Get the code segment from the SECTION_START_SEGMENT_ADDRESS section
         *
         * @throw std::runtime_error SECTION_START_SEGMENT_ADDRESS section not supported or found
         * @return uint16_t Code segment (CS)
         */
        virtual uint16_t getCodeSegment() const;

        /**
         * @brief Set the code segment in the SECTION_START_SEGMENT_ADDRESS section
         *
         * @throw std::runtime_error SECTION_START_SEGMENT_ADDRESS section not supported in current group type
         * @param codeSegment Code segment (CS)
         */
        virtual void setCodeSegment(uint16_t codeSegment);

        /**
         * @brief Get the instruction pointer from the SECTION_START_SEGMENT_ADDRESS section
         *
         * @throw std::runtime_error SECTION_START_SEGMENT_ADDRESS section not supported or found
         * @return uint16_t Instruction pointer (IP)
         */
        virtual uint16_t getInstructionPointer() const;

        /**
         * @brief Set the instruction pointer in the SECTION_START_SEGMENT_ADDRESS section
         *
         * @throw std::runtime_error SECTION_START_SEGMENT_ADDRESS section not supported in current group type
         * @param instructionPointer Instruction pointer (IP)
         */
        virtual void setInstructionPointer(uint16_t instructionPointer);

        /**
         * @brief Check if group can have SECTION_START_LINEAR_ADDRESS section
         *
         * @return true Group can have SECTION_START_LINEAR_ADDRESS section
         * @return false Group can not have SECTION_START_LINEAR_ADDRESS section
         */
        virtual bool supportsStartLinearAddress() const;

        /**
         * @brief Check if group has SECTION_START_LINEAR_ADDRESS section present
         *
         * @return true SECTION_START_LINEAR_ADDRESS section is present
         * @return false SECTION_START_LINEAR_ADDRESS section is not present
         */
        virtual bool hasStartLinearAddress() const;

        /**
         * @brief Clear SECTION_START_LINEAR_ADDRESS section if present
         *
         */
        virtual void clearStartLinearAddress();

        /**
         * @brief Get the SECTION_START_LINEAR_ADDRESS section
         *
         * @throw std::runtime_error SECTION_START_LINEAR_ADDRESS section not supported or found
         * @return const HexSection& SECTION_START_LINEAR_ADDRESS section
         */
        virtual const HexSection& getStartLinearAddressSection() const;

        /**
         * @brief Get the SECTION_START_LINEAR_ADDRESS section
         *
         * @throw std::runtime_error SECTION_START_LINEAR_ADDRESS section not supported or found
         * @return HexSection& SECTION_START_LINEAR_ADDRESS section
         */
        virtual HexSection& getStartLinearAddressSection();

        /**
         * @brief Get the extended instruction pointer from the SECTION_START_LINEAR_ADDRESS section
         *
         * @throw std::runtime_error SECTION_START_LINEAR_ADDRESS section not supported or found
         * @return uint32_t Extended instruction pointer (EIP)
         */
        virtual uint32_t getExtendedInstructionPointer() const;

        /**
         * @brief Set the extended instruction pointer in the SECTION_START_LINEAR_ADDRESS section
         *
         * @throw std::runtime_error SECTION_START_LINEAR_ADDRESS section not supported in current group type
         * @param extendedInstructionPointer Extended instruction pointer (EIP)
         */
        virtual void setExtendedInstructionPointer(uint32_t extendedInstructionPointer);
    protected:
        /**
         * @brief Get the section default push index
         *
         * @param section Section being pushed
         * @return size_t Default push index of the provided section
         */
        virtual size_t getSectionDefaultPushIndex(const HexSection& section) const;

        /**
         * @brief Hex sections
         *
         */
        HexSectionVector sections;

        /**
         * @brief Unused data fill value
         *
         */
        HexDataType unusedDataFillValue;
};

/**
 * @brief Shared pointer of HexGroup
 *
 */
typedef std::shared_ptr<HexGroup> HexGroupPtr;

/**
 * @brief Unique pointer of HexGroup
 *
 */
typedef std::unique_ptr<HexGroup> HexGroupUniquePtr;

END_NAMESPACE_LIBIHEX
