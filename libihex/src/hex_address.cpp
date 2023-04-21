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

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <ihex/hex_address.hpp>
#include <ihex/hex_record.hpp>

BEGIN_NAMESPACE_LIBIHEX

HexAddress::HexAddress() :
    addressType(HexAddressEnumType::ADDRESS_I8HEX), segmentAddress(0), linearAddress(0)
{

}

HexAddress::HexAddress(HexAddressEnumType type, uint16_t extendedAddress) :
    addressType(type), segmentAddress(0), linearAddress(0)
{
    switch (type)
    {
        case HexAddressEnumType::ADDRESS_I32HEX:
            linearAddress = extendedAddress;
            break;

        case HexAddressEnumType::ADDRESS_I16HEX:
            segmentAddress = extendedAddress;
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            break;
    }
}

HexAddressType HexAddress::getAbsoluteAddress(HexAddressEnumType type, HexDataAddressType address, uint16_t extendedAddress)
{
    // Return absolute address depending on the address type
    switch (type)
    {
        case HexAddressEnumType::ADDRESS_I32HEX:
            return getAbsoluteExtendedLinearAddress(address, extendedAddress);
            break;

        case HexAddressEnumType::ADDRESS_I16HEX:
            return getAbsoluteExtendedSegmentAddress(address, extendedAddress);
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return getAbsoluteDataRecordAddress(address);
            break;
    }
}

HexAddressType HexAddress::getAbsoluteAddress(HexDataAddressType address) const
{
    // Return absolute address depending on the address type
    switch (addressType)
    {
        case HexAddressEnumType::ADDRESS_I32HEX:
            return getAbsoluteExtendedLinearAddress(address, linearAddress);
            break;

        case HexAddressEnumType::ADDRESS_I16HEX:
            return getAbsoluteExtendedSegmentAddress(address, segmentAddress);
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return getAbsoluteDataRecordAddress(address);
            break;
    }
}

HexDataAddressType HexAddress::getRelativeAddress(HexAddressEnumType type, HexAddressType address, uint16_t extendedAddress)
{
    // Return relative address depending on the address type
    switch (type)
    {
        case HexAddressEnumType::ADDRESS_I32HEX:
            return getRelativeExtendedLinearAddress(address, extendedAddress);
            break;

        case HexAddressEnumType::ADDRESS_I16HEX:
            return getRelativeExtendedSegmentAddress(address, extendedAddress);
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return getRelativeDataRecordAddress(address);
            break;
    }
}

HexDataAddressType HexAddress::getRelativeAddress(HexAddressType address) const
{
    // Return relative address depending on the address type
    switch (addressType)
    {
        case HexAddressEnumType::ADDRESS_I32HEX:
            return getRelativeExtendedLinearAddress(address, linearAddress);
            break;

        case HexAddressEnumType::ADDRESS_I16HEX:
            return getRelativeExtendedSegmentAddress(address, segmentAddress);
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return getRelativeDataRecordAddress(address);
            break;
    }
}

HexAddressType HexAddress::getAbsoluteDataRecordAddress(HexDataAddressType address)
{
    // ADDRESS_I8HEX address type is just a type-cast to larger address type
    return static_cast<HexAddressType>(address);
}

HexDataAddressType HexAddress::getRelativeDataRecordAddress(HexAddressType address)
{
    // Check absolute address limits
    // Since address is unsigned it can not be lower than 0
    if (address > getMaxAbsoluteDataRecordAddress())
        throw std::out_of_range("Absolute address is out of range for an ADDRESS_I8HEX address space");

    // ADDRESS_I8HEX address type is just a type-cast with
    // any extended address bits discarded
    return static_cast<HexDataAddressType>(address & std::numeric_limits<HexDataAddressType>::max());
}

HexAddressType HexAddress::getAbsoluteExtendedSegmentAddress(HexDataAddressType address, uint16_t segmentAddress)
{
    // Segment base address + (address % 2^16)
    return ((getBaseSegmentAddress(segmentAddress) + address) & maxExtendedSegmentAddress);
}

HexAddressType HexAddress::getAbsoluteExtendedSegmentAddress(HexDataAddressType address) const
{
    return getAbsoluteExtendedSegmentAddress(address, segmentAddress);
}

HexDataAddressType HexAddress::getRelativeExtendedSegmentAddress(HexAddressType address, uint16_t segmentAddress)
{
    // Check absolute address limits
    if (!containsAbsoluteSegmentAddress(address, segmentAddress))
        throw std::out_of_range("Absolute address is out of range for an ADDRESS_I16HEX address space");

    // (address % 2^16) - Segment base address
    return static_cast<HexDataAddressType>(address - getBaseSegmentAddress(segmentAddress));
}

HexDataAddressType HexAddress::getRelativeExtendedSegmentAddress(HexAddressType address) const
{
    return getRelativeExtendedSegmentAddress(address, segmentAddress);
}

HexAddressType HexAddress::getAbsoluteExtendedLinearAddress(HexDataAddressType address, uint16_t linearAddress)
{
    // (Linear base address + address) % 2^32
    return (getBaseLinearAddress(linearAddress) + address);
}

HexAddressType HexAddress::getAbsoluteExtendedLinearAddress(HexDataAddressType address) const
{
    return getAbsoluteExtendedLinearAddress(address, linearAddress);
}

HexDataAddressType HexAddress::getRelativeExtendedLinearAddress(HexAddressType address, uint16_t linearAddress)
{
    // Check absolute address limits
    if (!containsAbsoluteLinearAddress(address, linearAddress))
        throw std::out_of_range("Absolute address is out of range for an ADDRESS_I32HEX address space");

    // (address - Linear base address) % 2^32
    return static_cast<HexDataAddressType>(address - getBaseLinearAddress(linearAddress));
}

HexDataAddressType HexAddress::getRelativeExtendedLinearAddress(HexAddressType address) const
{
    return getRelativeExtendedLinearAddress(address, linearAddress);
}

HexAddressType HexAddress::getMinAbsoluteAddress() const
{
    switch (addressType)
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
            return getMinAbsoluteExtendedSegmentAddress(segmentAddress);
            break;

        case HexAddressEnumType::ADDRESS_I32HEX:
            return getMinAbsoluteExtendedLinearAddress(linearAddress);
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return getMinAbsoluteDataRecordAddress();
            break;
    }
}

HexAddressType HexAddress::getMinAbsoluteDataRecordAddress()
{
    return std::numeric_limits<HexDataAddressType>::min();
}

HexAddressType HexAddress::getMinAbsoluteExtendedSegmentAddress(uint16_t segmentAddress)
{
    return getAbsoluteExtendedSegmentAddress(std::numeric_limits<HexDataAddressType>::min(), segmentAddress);
}

HexAddressType HexAddress::getMinAbsoluteExtendedSegmentAddress() const
{
    return getMinAbsoluteExtendedSegmentAddress(segmentAddress);
}

HexAddressType HexAddress::getMinAbsoluteExtendedLinearAddress(uint16_t linearAddress)
{
    return getAbsoluteExtendedLinearAddress(std::numeric_limits<HexDataAddressType>::min(), linearAddress);
}

HexAddressType HexAddress::getMinAbsoluteExtendedLinearAddress() const
{
    return getMinAbsoluteExtendedLinearAddress(linearAddress);
}

HexAddressType HexAddress::getMaxAbsoluteAddress() const
{
    switch (addressType)
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
            return getMaxAbsoluteExtendedSegmentAddress(segmentAddress);
            break;

        case HexAddressEnumType::ADDRESS_I32HEX:
            return getMaxAbsoluteExtendedLinearAddress(linearAddress);
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return getMaxAbsoluteDataRecordAddress();
            break;
    }
}

HexAddressType HexAddress::getMaxAbsoluteDataRecordAddress()
{
    return std::numeric_limits<HexDataAddressType>::max();
}

HexAddressType HexAddress::getMaxAbsoluteExtendedSegmentAddress(uint16_t segmentAddress)
{
    return getAbsoluteExtendedSegmentAddress(std::numeric_limits<HexDataAddressType>::max(), segmentAddress);
}

HexAddressType HexAddress::getMaxAbsoluteExtendedSegmentAddress() const
{
    return getMaxAbsoluteExtendedSegmentAddress(segmentAddress);
}

HexAddressType HexAddress::getMaxAbsoluteExtendedLinearAddress(uint16_t linearAddress)
{
    return getAbsoluteExtendedLinearAddress(std::numeric_limits<HexDataAddressType>::max(), linearAddress);
}

HexAddressType HexAddress::getMaxAbsoluteExtendedLinearAddress() const
{
    return getMaxAbsoluteExtendedLinearAddress(linearAddress);
}

bool HexAddress::hasExtendedSegmentAddressWraparound(uint16_t segmentAddress)
{
    return (segmentAddress > findExtendedSegmentAddress(maxExtendedSegmentAddress));
}

uint16_t HexAddress::findExtendedSegmentAddress(HexAddressType address)
{
    // Range check the extended segment address
    if (!isValidAbsoluteSegmentAddress(address))
        throw std::out_of_range("Absolute address is out of range for an ADDRESS_I16HEX address space");

    // Calculate the remainder to the base address (aligned to the HexDataAddressType boundary)
    HexAddressType remainder{address % (static_cast<HexAddressType>(std::numeric_limits<HexDataAddressType>::max()) + 1)};

    // Return base extended segment address
    return static_cast<uint16_t>((address - remainder) >> 4);
}

uint16_t HexAddress::findExtendedLinearAddress(HexAddressType address)
{
    // Return the base extended linear address
    return ((address >> 16) & std::numeric_limits<uint16_t>::max());
}

HexAddressType HexAddress::getBaseSegmentAddress(uint16_t segmentAddress)
{
    return (static_cast<HexAddressType>(segmentAddress) << 4);
}

HexAddressType HexAddress::getBaseLinearAddress(uint16_t linearAddress)
{
    return (static_cast<HexAddressType>(linearAddress) << 16);
}

uint16_t HexAddress::getPreviousSegmentAddress(uint16_t segmentAddress)
{
    return (segmentAddress - 0x1000);
}

uint16_t HexAddress::getNextSegmentAddress(uint16_t segmentAddress)
{
    return (segmentAddress + 0x1000);
}

uint16_t HexAddress::getPreviousLinearAddress(uint16_t linearAddress)
{
    return (linearAddress - 1);
}

uint16_t HexAddress::getNextLinearAddress(uint16_t linearAddress)
{
    return (linearAddress + 1);
}

bool HexAddress::containsAbsoluteAddress(HexAddressEnumType type, HexAddressType address, uint16_t extendedAddress)
{
    // Return result depending on the address type
    switch (type)
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
            return containsAbsoluteSegmentAddress(address, extendedAddress);
            break;

        case HexAddressEnumType::ADDRESS_I32HEX:
            return containsAbsoluteLinearAddress(address, extendedAddress);
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return containsAbsoluteDataRecordAddress(address);
            break;
    }
}

bool HexAddress::containsAbsoluteAddress(HexAddressType address) const
{
    // Return result depending on the address type
    switch (addressType)
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
            return containsAbsoluteSegmentAddress(address, segmentAddress);
            break;

        case HexAddressEnumType::ADDRESS_I32HEX:
            return containsAbsoluteLinearAddress(address, linearAddress);
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return containsAbsoluteDataRecordAddress(address);
            break;
    }
}

bool HexAddress::containsAbsoluteDataRecordAddress(HexAddressType address)
{
    return isValidAbsoluteDataRecordAddress(address);
}

bool HexAddress::containsAbsoluteSegmentAddress(HexAddressType address, uint16_t segmentAddress)
{
    const auto min{getMinAbsoluteExtendedSegmentAddress(segmentAddress)};
    const auto max{getMaxAbsoluteExtendedSegmentAddress(segmentAddress)};
    if (hasExtendedSegmentAddressWraparound(segmentAddress))
        return ((!((address < min) && (address > max))) && (address <= maxExtendedSegmentAddress));
    else
        return (!((address < min) || (address > max)));
}

bool HexAddress::containsAbsoluteLinearAddress(HexAddressType address, uint16_t linearAddress)
{
    return (!((address < getMinAbsoluteExtendedLinearAddress(linearAddress)) || (address > getMaxAbsoluteExtendedLinearAddress(linearAddress))));
}

bool HexAddress::isValidAbsoluteAddress(HexAddressEnumType type, HexAddressType address)
{
    // Return result depending on the address type
    switch (type)
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
            return isValidAbsoluteSegmentAddress(address);
            break;

        case HexAddressEnumType::ADDRESS_I32HEX:
            // Every ADDRESS_I32HEX is a valid address
            return true;
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return isValidAbsoluteDataRecordAddress(address);
            break;
    }
}

bool HexAddress::isValidAbsoluteAddress(HexAddressType address) const
{
    // Return result depending on the address type
    switch (addressType)
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
            return isValidAbsoluteSegmentAddress(address);
            break;

        case HexAddressEnumType::ADDRESS_I32HEX:
            // Every ADDRESS_I32HEX is a valid address
            return true;
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return isValidAbsoluteDataRecordAddress(address);
            break;
    }
}

bool HexAddress::isValidAbsoluteDataRecordAddress(HexAddressType address)
{
    return (address <= getMaxAbsoluteDataRecordAddress());
}

bool HexAddress::isValidAbsoluteSegmentAddress(HexAddressType address)
{
    return (address <= maxExtendedSegmentAddress);
}

bool HexAddress::isValidAbsoluteDataRecordAddressRange(HexAddressType address, HexDataSizeType size)
{
    return ((size > 0) &&
        (address <= getMaxAbsoluteDataRecordAddress()) &&
        ((static_cast<HexDataSizeType>(address) + size - 1) <= getMaxAbsoluteDataRecordAddress()));
}

bool HexAddress::isValidAbsoluteSegmentAddressRange(HexAddressType address, HexDataSizeType size)
{
    return ((size > 0) &&
        (address <= maxExtendedSegmentAddress) &&
        ((static_cast<HexDataSizeType>(address) + size - 1) <= maxExtendedSegmentAddress));
}

bool HexAddress::isValidAbsoluteLinearAddressRange(HexAddressType address, HexDataSizeType size)
{
    return ((size > 0) &&
        (static_cast<HexDataSizeType>(address) + size - 1) <= std::numeric_limits<HexAddressType>::max());
}

bool HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType type, HexAddressType address, HexDataSizeType size)
{
    // Return result depending on the address type
    switch (type)
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
            return isValidAbsoluteSegmentAddressRange(address, size);
            break;

        case HexAddressEnumType::ADDRESS_I32HEX:
            return isValidAbsoluteLinearAddressRange(address, size);
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return isValidAbsoluteDataRecordAddressRange(address, size);
            break;
    }
}

bool HexAddress::isValidAbsoluteAddressRange(HexAddressType address, HexDataSizeType size) const
{
    return isValidAbsoluteAddressRange(addressType, address, size);
}

bool HexAddress::isValidRelativeAddressRange(HexDataAddressType address, HexDataSizeType size)
{
    return isValidAbsoluteDataRecordAddressRange(address, size);
}

bool HexAddress::checkExtendedSegmentAddressIntersect(uint16_t first, uint16_t second)
{
    // Find intersection of the two address maps
    const HexAddressMap firstMap{getExtendedSegmentAddressMap(first)};
    const HexAddressMap secondMap{getExtendedSegmentAddressMap(second)};
    return checkAddressMapIntersect(firstMap, secondMap);
}

bool HexAddress::checkExtendedLinearAddressIntersect(uint16_t first, uint16_t second)
{
    // Any two extended linear addresses would have intersection only
    // if extended linear address is the same
    return (first == second);
}

bool HexAddress::checkAddressMapIntersect(const HexAddressMap& first, const HexAddressMap& second)
{
    // Loop through all the ranges of the first map
    for (auto& firstRange : first)
    {
        // If range has size of zero, there is nothing to compare
        if (firstRange.second < 1)
            continue;

        // Loop through all the ranges of the second map
        for (auto& secondRange : second)
        {
            // If range has size of zero, there is nothing to compare
            if (secondRange.second < 1)
                continue;

            // The same base addresses have intersect
            // Range check the addresses to find intersect
            if ((firstRange.first == secondRange.first))
                return true;
            else if ((!(firstRange.first > secondRange.first)) && (!(secondRange.first > (firstRange.first + firstRange.second - 1))))
                return true;
            else if ((!(firstRange.first < secondRange.first)) && (!(firstRange.first > (secondRange.first + secondRange.second - 1))))
                return true;
        }
    }

    // No intersect between addresses
    return false;
}

HexDataSizeType HexAddress::getSectionMaxDataSize()
{
    // Size of any data section is the maximum address including the zero address
    return (static_cast<HexDataSizeType>(std::numeric_limits<HexDataAddressType>::max()) + 1);
}

HexAddressMap HexAddress::getDataRecordAddressMap()
{
    return HexAddressMap{HexAddressMap::value_type{0, HexAddress::getSectionMaxDataSize()}};
}

void HexAddress::compactAddressMap(HexAddressMap& map)
{
    auto it{map.begin()};
    while (it != map.end())
    {
        // Next block
        auto next{std::next(it)};

        // Does the next block exist?
        if (next == map.end())
            break;

        // Is next block adjacent to the current one?
        if ((it->first + it->second) != next->first)
        {
            ++it;
            continue;
        }

        // Append next block to the current one
        it->second += next->second;

        // Erase next block
        map.erase(next);
    }
}

HexAddressMap HexAddress::getExtendedSegmentAddressMap(uint16_t segmentAddress)
{
    const auto base{getBaseSegmentAddress(segmentAddress)};
    if (hasExtendedSegmentAddressWraparound(segmentAddress))
    {
        HexAddressMap map{};
        map[0] = (getMaxAbsoluteExtendedSegmentAddress(segmentAddress) + 1);
        map[base] = (maxExtendedSegmentAddress - base + 1);
        return map;
    }
    else
        return HexAddressMap{HexAddressMap::value_type{base, HexAddress::getSectionMaxDataSize()}};
}

HexAddressMap HexAddress::getExtendedLinearAddressMap(uint16_t linearAddress)
{
    return HexAddressMap{HexAddressMap::value_type{getBaseLinearAddress(linearAddress), HexAddress::getSectionMaxDataSize()}};
}

HexAddressMap HexAddress::getAddressMap() const
{
    // Return result depending on the address type
    switch (addressType)
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
            return getExtendedSegmentAddressMap(segmentAddress);
            break;

        case HexAddressEnumType::ADDRESS_I32HEX:
            return getExtendedLinearAddressMap(linearAddress);
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            return getDataRecordAddressMap();
            break;
    }
}

HexAddressEnumType HexAddress::getAddressType() const
{
    return addressType;
}

void HexAddress::setAddressType(HexAddressEnumType addressType)
{
    this->addressType = addressType;
}

uint16_t HexAddress::getSegmentAddress() const
{
    return segmentAddress;
}

void HexAddress::setSegmentAddress(uint16_t segmentAddress)
{
    this->segmentAddress = segmentAddress;
}

uint16_t HexAddress::getLinearAddress() const
{
    return linearAddress;
}

void HexAddress::setLinearAddress(uint16_t linearAddress)
{
    this->linearAddress = linearAddress;
}

END_NAMESPACE_LIBIHEX
