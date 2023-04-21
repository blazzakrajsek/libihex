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
#include <memory>
#include <stdint.h>
#include <ihex/hex_record.hpp>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

/**
 * @brief Hex address enum type class
 *
 */
enum class HexAddressEnumType : uint8_t
{
    /**
     * @brief Hex address for 8-bit formats
     *
     * @note Records of SECTION_DATA and SECTION_END_OF_FILE types
     */
    ADDRESS_I8HEX = 0,

    /**
     * @brief Hex address for 16-bit formats
     *
     * @note Records of SECTION_DATA, SECTION_END_OF_FILE,
     *   SECTION_EXTENDED_SEGMENT_ADDRESS and SECTION_START_SEGMENT_ADDRESS types
     */
    ADDRESS_I16HEX = 1,

    /**
     * @brief Hex address for 32-bit formats
     *
     * @note Records of SECTION_DATA, SECTION_END_OF_FILE,
     *   SECTION_EXTENDED_LINEAR_ADDRESS and SECTION_START_LINEAR_ADDRESS types
     */
    ADDRESS_I32HEX = 2,

    ADDRESS_MIN = ADDRESS_I8HEX,
    ADDRESS_MAX = ADDRESS_I32HEX
};

/**
 * @brief Absolute hex data address type
 *
 */
typedef uint32_t HexAddressType;

/**
 * @brief Map of hex addresses
 *
 */
typedef std::map<HexAddressType, HexDataSizeType> HexAddressMap;

/**
 * @brief HexAddress class
 *
 */
class HexAddress final
{
    public:
        /**
         * @brief Construct a new HexAddress object
         *
         */
        explicit HexAddress();

        /**
         * @brief Construct a new HexAddress object
         *
         * @param type Type of address
         * @param extendedAddress Extended address
         */
        explicit HexAddress(HexAddressEnumType type, uint16_t extendedAddress = 0);

        /**
         * @brief Copy-construct a new HexAddress object
         *
         * @param address Address to copy-construct from
         */
        HexAddress(const HexAddress& address) = default;

        /**
         * @brief Move-construct a new HexAddress object
         *
         * @param address Address to move-construct from
         */
        HexAddress(HexAddress&& address) = default;

        /**
         * @brief Copy-assignment operator
         *
         * @param address Address to copy-assign
         * @return HexAddress& Assigned address
         */
        HexAddress& operator=(const HexAddress& address) = default;

        /**
         * @brief Move-assignment operator
         *
         * @param address Address to move-assign
         * @return HexAddress& Assigned address
         */
        HexAddress& operator=(HexAddress&& address) = default;

        /**
         * @brief Get the absolute address
         *
         * @param type Type of address
         * @param address Relative address
         * @param extendedAddress Extended address
         * @return HexAddressType Absolute address
         */
        static HexAddressType getAbsoluteAddress(HexAddressEnumType type, HexDataAddressType address, uint16_t extendedAddress = 0);

        /**
         * @brief Get the absolute address
         *
         * @param address Relative address
         * @return HexAddressType Absolute address
         */
        HexAddressType getAbsoluteAddress(HexDataAddressType address) const;

        /**
         * @brief Get the relative address
         *
         * @param type Type of address
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for a particular address type
         * @param extendedAddress Extended address
         * @return HexDataAddressType Relative address
         */
        static HexDataAddressType getRelativeAddress(HexAddressEnumType type, HexAddressType address, uint16_t extendedAddress = 0);

        /**
         * @brief Get the relative address
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for a particular address type
         * @return HexDataAddressType Relative address
         */
        HexDataAddressType getRelativeAddress(HexAddressType address) const;

        /**
         * @brief Get the absolute data record address
         *
         * @param address Relative address
         * @return HexAddressType Absolute address
         */
        static HexAddressType getAbsoluteDataRecordAddress(HexDataAddressType address);

        /**
         * @brief Get the relative data record address
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for an ADDRESS_I8HEX address space
         * @return HexDataAddressType Relative address
         */
        static HexDataAddressType getRelativeDataRecordAddress(HexAddressType address);

        /**
         * @brief Get the absolute extended segment address
         *
         * @param address Relative address
         * @param segmentAddress Extended segment address
         * @return HexAddressType Absolute extended segment address
         */
        static HexAddressType getAbsoluteExtendedSegmentAddress(HexDataAddressType address, uint16_t segmentAddress);

        /**
         * @brief Get the absolute extended segment address
         *
         * @param address Relative address
         * @return HexAddressType Absolute extended segment address
         */
        HexAddressType getAbsoluteExtendedSegmentAddress(HexDataAddressType address) const;

        /**
         * @brief Get the relative extended segment address
         *
         * @param address Absolute extended segment address
         * @throw std::out_of_range Absolute address is out of range for an ADDRESS_I16HEX address space
         * @param segmentAddress Extended segment address
         * @return HexDataAddressType Relative address
         */
        static HexDataAddressType getRelativeExtendedSegmentAddress(HexAddressType address, uint16_t segmentAddress);

        /**
         * @brief Get the relative extended segment address
         *
         * @param address Absolute extended segment address
         * @throw std::out_of_range Absolute address is out of range for an ADDRESS_I16HEX address space
         * @return HexDataAddressType Relative address
         */
        HexDataAddressType getRelativeExtendedSegmentAddress(HexAddressType address) const;

        /**
         * @brief Get the absolute extended linear address
         *
         * @param address Relative address
         * @param linearAddress Extended linear address
         * @return HexAddressType Absolute extended linear address
         */
        static HexAddressType getAbsoluteExtendedLinearAddress(HexDataAddressType address, uint16_t linearAddress);

        /**
         * @brief Get the absolute extended linear address
         *
         * @param address Relative address
         * @return HexAddressType Absolute extended linear address
         */
        HexAddressType getAbsoluteExtendedLinearAddress(HexDataAddressType address) const;

        /**
         * @brief Get the relative extended linear address
         *
         * @param address Absolute extended linear address
         * @throw std::out_of_range Absolute address is out of range for an ADDRESS_I32HEX address space
         * @param linearAddress Extended linear address
         * @return HexDataAddressType Relative address
         */
        static HexDataAddressType getRelativeExtendedLinearAddress(HexAddressType address, uint16_t linearAddress);

        /**
         * @brief Get the relative extended linear address
         *
         * @param address Absolute extended linear address
         * @throw std::out_of_range Absolute address is out of range for an ADDRESS_I32HEX address space
         * @return HexDataAddressType Relative address
         */
        HexDataAddressType getRelativeExtendedLinearAddress(HexAddressType address) const;

        /**
         * @brief Get the minimum absolute address
         *
         * @return HexAddressType Minimum absolute address
         */
        HexAddressType getMinAbsoluteAddress() const;

        /**
         * @brief Get the minimum absolute data record address
         *
         * @return HexAddressType Minimum absolute address
         */
        static HexAddressType getMinAbsoluteDataRecordAddress();

        /**
         * @brief Get the minimum absolute extended segment address
         *
         * @param segmentAddress Extended segment address
         * @return HexAddressType Minimum absolute address
         */
        static HexAddressType getMinAbsoluteExtendedSegmentAddress(uint16_t segmentAddress);

        /**
         * @brief Get the minimum absolute extended segment address
         *
         * @return HexAddressType Minimum absolute extended segment address
         */
        HexAddressType getMinAbsoluteExtendedSegmentAddress() const;

        /**
         * @brief Get the minimum absolute extended linear address
         *
         * @param linearAddress Extended linear address
         * @return HexAddressType Minimum absolute address
         */
        static HexAddressType getMinAbsoluteExtendedLinearAddress(uint16_t linearAddress);

        /**
         * @brief Get the minimum absolute extended linear address
         *
         * @return HexAddressType Minimum absolute extended linear address
         */
        HexAddressType getMinAbsoluteExtendedLinearAddress() const;

        /**
         * @brief Get the maximum absolute address
         *
         * @return HexAddressType Maximum absolute address
         */
        HexAddressType getMaxAbsoluteAddress() const;

        /**
         * @brief Get the maximum absolute data record address
         *
         * @return HexAddressType Maximum absolute address
         */
        static HexAddressType getMaxAbsoluteDataRecordAddress();

        /**
         * @brief Get the maximum absolute extended segment address
         *
         * @param segmentAddress Extended segment address
         * @return HexAddressType Maximum absolute address
         */
        static HexAddressType getMaxAbsoluteExtendedSegmentAddress(uint16_t segmentAddress);

        /**
         * @brief Get the maximum absolute extended segment address
         *
         * @return HexAddressType Maximum absolute extended segment address
         */
        HexAddressType getMaxAbsoluteExtendedSegmentAddress() const;

        /**
         * @brief Get the maximum absolute extended linear address
         *
         * @param linearAddress Extended linear address
         * @return HexAddressType Maximum absolute address
         */
        static HexAddressType getMaxAbsoluteExtendedLinearAddress(uint16_t linearAddress);

        /**
         * @brief Get the maximum absolute extended linear address
         *
         * @return HexAddressType Maximum absolute extended linear address
         */
        HexAddressType getMaxAbsoluteExtendedLinearAddress() const;

        /**
         * @brief Check if an extended segment address has wraparound
         *
         * @param segmentAddress Extended segment address
         * @return true Extended segment address has wraparound
         * @return false Extended segment address has wraparound
         */
        static bool hasExtendedSegmentAddressWraparound(uint16_t segmentAddress);

        /**
         * @brief Find extended segment address from absolute address
         *
         * @param address Absolute address
         * @throw std::out_of_range Absolute address is out of range for an ADDRESS_I16HEX address space
         * @return uint16_t Extended segment address
         */
        static uint16_t findExtendedSegmentAddress(HexAddressType address);

        /**
         * @brief Find extended linear address from absolute address
         *
         * @param address Absolute address
         * @return uint16_t Extended linear address
         */
        static uint16_t findExtendedLinearAddress(HexAddressType address);

        /**
         * @brief Get the base segment address
         *
         * @param segmentAddress Segment address
         * @return HexAddressType Base of segment address
         */
        static HexAddressType getBaseSegmentAddress(uint16_t segmentAddress);

        /**
         * @brief Get the base linear address
         *
         * @param linearAddress Linear address
         * @return HexAddressType Base of linear address
         */
        static HexAddressType getBaseLinearAddress(uint16_t linearAddress);

        /**
         * @brief Get the previous segment address
         *
         * @param segmentAddress Segment address
         * @return uint16_t Previous segment address
         */
        static uint16_t getPreviousSegmentAddress(uint16_t segmentAddress);

        /**
         * @brief Get the next segment address
         *
         * @param segmentAddress Segment address
         * @return uint16_t Next segment address
         */
        static uint16_t getNextSegmentAddress(uint16_t segmentAddress);

        /**
         * @brief Get the previous linear address
         *
         * @param linearAddress Linear address
         * @return uint16_t Previous linear address
         */
        static uint16_t getPreviousLinearAddress(uint16_t linearAddress);

        /**
         * @brief Get the next linear address
         *
         * @param linearAddress Linear address
         * @return uint16_t Next linear address
         */
        static uint16_t getNextLinearAddress(uint16_t linearAddress);

        /**
         * @brief Check if absolute address is contained in a given address type
         *
         * @param type Type of address
         * @param address Absolute address
         * @param extendedAddress Extended address
         * @return true Absolute address is contained in a given address type
         * @return false Absolute address is not contained in a given address type
         */
        static bool containsAbsoluteAddress(HexAddressEnumType type, HexAddressType address, uint16_t extendedAddress = 0);

        /**
         * @brief Check if absolute address is contained in a given address type
         *
         * @param address Absolute address
         * @return true Absolute address is contained in a given address type
         * @return false Absolute address is not contained in a given address type
         */
        bool containsAbsoluteAddress(HexAddressType address) const;

        /**
         * @brief Check if absolute address is contained in the SECTION_DATA
         *
         * @param address Absolute address
         * @return true Address is contained in the SECTION_DATA
         * @return false Address in not contained in the SECTION_DATA
         */
        static bool containsAbsoluteDataRecordAddress(HexAddressType address);

        /**
         * @brief Check if absolute address is contained in the SECTION_EXTENDED_SEGMENT_ADDRESS
         *
         * @param address Absolute address
         * @param segmentAddress Extended segment address
         * @return true Address is contained in the SECTION_EXTENDED_SEGMENT_ADDRESS
         * @return false Address is not contained in the SECTION_EXTENDED_SEGMENT_ADDRESS
         */
        static bool containsAbsoluteSegmentAddress(HexAddressType address, uint16_t segmentAddress);

        /**
         * @brief Check if absolute address is contained in the SECTION_EXTENDED_LINEAR_ADDRESS
         *
         * @param address Absolute address
         * @param linearAddress Extended linear address
         * @return true Address is contained in the SECTION_EXTENDED_LINEAR_ADDRESS
         * @return false Address is not contained in the SECTION_EXTENDED_LINEAR_ADDRESS
         */
        static bool containsAbsoluteLinearAddress(HexAddressType address, uint16_t linearAddress);

        /**
         * @brief Check if absolute address is valid
         *
         * @param type Type of address
         * @param address Absolute address
         * @return true Address is valid
         * @return false Address is not valid
         */
        static bool isValidAbsoluteAddress(HexAddressEnumType type, HexAddressType address);

        /**
         * @brief Check if absolute address is valid
         *
         * @param address Absolute address
         * @return true Address is valid
         * @return false Address is not valid
         */
        bool isValidAbsoluteAddress(HexAddressType address) const;

        /**
         * @brief Check if absolute address is valid in ADDRESS_I8HEX address space
         *
         * @param address Absolute address
         * @return true Address is valid
         * @return false Address is invalid
         */
        static bool isValidAbsoluteDataRecordAddress(HexAddressType address);

        /**
         * @brief Check if absolute address is valid in ADDRESS_I16HEX address space
         *
         * @param address Absolute address
         * @return true Address is valid
         * @return false Address is invalid
         */
        static bool isValidAbsoluteSegmentAddress(HexAddressType address);

        /**
         * @brief Check if absolute address range is valid in ADDRESS_I8HEX address space
         *
         * @param address Absolute address
         * @param size Size
         * @note Address range interval is [address, address + size - 1]
         * @return true Absolute address is valid
         * @return false Absolute address is not valid
         * @note false is returned if size is zero
         */
        static bool isValidAbsoluteDataRecordAddressRange(HexAddressType address, HexDataSizeType size);

        /**
         * @brief Check if absolute address range is valid in ADDRESS_I16HEX address space
         *
         * @param address Absolute address
         * @param size Size
         * @note Address range interval is [address, address + size - 1]
         * @return true Absolute address is valid
         * @return false Absolute address is not valid
         * @note false is returned if size is zero
         */
        static bool isValidAbsoluteSegmentAddressRange(HexAddressType address, HexDataSizeType size);

        /**
         * @brief Check if absolute address range is valid in ADDRESS_I32HEX address range
         *
         * @param address Absolute address
         * @param size Size
         * @note Address range interval is [address, address + size - 1]
         * @return true Absolute address is valid
         * @return false Absolute address is not valid
         * @note false is returned if size is zero
         */
        static bool isValidAbsoluteLinearAddressRange(HexAddressType address, HexDataSizeType size);

        /**
         * @brief Check if absolute address range is valid
         *
         * @param type Type of address
         * @param address Absolute address
         * @param size Size
         * @note Address range interval is [address, address + size - 1]
         * @return true Absolute address range is valid
         * @return false Absolute address range is not valid
         * @note false is returned if size is zero
         */
        static bool isValidAbsoluteAddressRange(HexAddressEnumType type, HexAddressType address, HexDataSizeType size);

        /**
         * @brief Check if absolute address range is valid
         *
         * @param address Absolute address
         * @param size Size
         * @note Address range interval is [address, address + size - 1]
         * @return true Absolute address range is valid
         * @return false Absolute address range is not valid
         * @note false is returned if size is zero
         */
        bool isValidAbsoluteAddressRange(HexAddressType address, HexDataSizeType size) const;

        /**
         * @brief Check if relative address range is valid
         *
         * @param address Relative address
         * @param size Size
         * @return true Relative address is valid
         * @return false Relative address is not valid
         */
        static bool isValidRelativeAddressRange(HexDataAddressType address, HexDataSizeType size);

        /**
         * @brief Check for intersect between two extended segment addresses
         *
         * @param first First extended segment address
         * @param second Second extended segment address
         * @return true Addresses intersect
         * @return false Addresses do not intersect
         */
        static bool checkExtendedSegmentAddressIntersect(uint16_t first, uint16_t second);

        /**
         * @brief Check for intersect between two extended linear addresses
         *
         * @param first First extended linear address
         * @param second Second extended linear address
         * @return true Addresses intersect
         * @return false Addresses do not intersect
         */
        static bool checkExtendedLinearAddressIntersect(uint16_t first, uint16_t second);

        /**
         * @brief Check intersect between two address maps
         *
         * @param first First address map
         * @param second Second address map
         * @return true Maps intersect
         * @return false Maps do not intersect
         */
        static bool checkAddressMapIntersect(const HexAddressMap& first, const HexAddressMap& second);

        /**
         * @brief Get the section maximum data size
         *
         * @return HexDataSizeType Section maximum data size
         */
        static HexDataSizeType getSectionMaxDataSize();

        /**
         * @brief Get the data record address map
         *
         * @return HexAddressMap Address map
         */
        static HexAddressMap getDataRecordAddressMap();

        /**
         * @brief Compact address map
         *
         */
        static void compactAddressMap(HexAddressMap& map);

        /**
         * @brief Get the extended segment address map
         *
         * @param segmentAddress Extended segment address
         * @return HexAddressMap Address map
         */
        static HexAddressMap getExtendedSegmentAddressMap(uint16_t segmentAddress);

        /**
         * @brief Get the extended linear address map
         *
         * @param linearAddress Extended linear address
         * @return HexAddressMap Address map
         */
        static HexAddressMap getExtendedLinearAddressMap(uint16_t linearAddress);

        /**
         * @brief Get the address map
         *
         * @return HexAddressMap Address map
         */
        HexAddressMap getAddressMap() const;

        /**
         * @brief Get the address type
         *
         * @return HexAddressEnumType Address type
         */
        HexAddressEnumType getAddressType() const;

        /**
         * @brief Set the address type
         *
         * @param addressType Address type
         */
        void setAddressType(HexAddressEnumType addressType);

        /**
         * @brief Get the segment address
         *
         * @return uint16_t Segment address
         */
        uint16_t getSegmentAddress() const;

        /**
         * @brief Set the segment address
         *
         * @param segmentAddress Segment address
         */
        void setSegmentAddress(uint16_t segmentAddress);

        /**
         * @brief Get the linear address
         *
         * @return uint16_t Linear address
         */
        uint16_t getLinearAddress() const;

        /**
         * @brief Set the linear address
         *
         * @param linearAddress Linear address
         */
        void setLinearAddress(uint16_t linearAddress);
    protected:
        /**
         * @brief Address type
         *
         */
        HexAddressEnumType addressType;

        /**
         * @brief Extended segment address
         *
         */
        uint16_t segmentAddress;

        /**
         * @brief Extended linear address
         *
         */
        uint16_t linearAddress;

        /**
         * @brief Maximum extended segment address
         *
         */
        static constexpr HexAddressType maxExtendedSegmentAddress{0xFFFFF};
};

/**
 * @brief Shared pointer of HexAddress
 *
 */
typedef std::shared_ptr<HexAddress> HexAddressPtr;

/**
 * @brief Unique pointer of HexAddress
 *
 */
typedef std::unique_ptr<HexAddress> HexAddressUniquePtr;

END_NAMESPACE_LIBIHEX
