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
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <ihex/hex_address.hpp>
#include <ihex/hex_group.hpp>
#include <ihex/hex_record.hpp>
#include <ihex/hex_section.hpp>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

HexGroup::HexGroup() :
    sections{}, unusedDataFillValue(DEFAULT_HEX_RECORD_DATA_VALUE)
{

}

HexGroup::~HexGroup()
{

}

HexDataType HexGroup::operator[](HexAddressType address) const
{
    // Iterate through all the sections to find the data
    for (const auto& sect : sections)
    {
        if (sect.hasData() && sect.hasAbsoluteAddress(address))
            return sect[sect.getRelativeAddress(address)];
    }

    // No section found with the data
    throw std::out_of_range("Address is out of range");
}

HexDataType& HexGroup::operator[](HexAddressType address)
{
    // Iterate through all the sections to find the data
    for (auto& sect : sections)
    {
        if (sect.hasData() && sect.hasAbsoluteAddress(address))
            return sect[sect.getRelativeAddress(address)];
    }

    // No section found with the data
    throw std::out_of_range("Address is out of range");
}

size_t HexGroup::getSize() const
{
    return sections.size();
}

const HexSection& HexGroup::getSection(size_t index) const
{
    return sections.at(index);
}

HexSection& HexGroup::getSection(size_t index)
{
    return sections.at(index);
}

size_t HexGroup::findSection(HexAddressType address) const
{
    // Iterate through all the sections to find the address
    for (size_t index{0}; index < sections.size(); ++index)
    {
        // Continue if section does not have addresses
        const auto& sect{sections.at(index)};
        if (!sect.hasAddress())
            continue;

        // Return the index of the section containing the address
        if (sect.hasAbsoluteAddress(address))
            return index;
    }

    // Return if no section exist with the address
    return std::string::npos;
}

size_t HexGroup::findPreviousSection(HexAddressType address) const
{
    // Define result and build address map
    size_t result{std::string::npos};
    HexAddressMap map{getAddressMap()};

    // Find section which is lower by address than the provided address
    for (auto it{map.cbegin()}; it != map.cend(); ++it)
    {
        // Find section through the ordered address map with the last
        // lower address than the one provided by the parameter address
        if (address > (it->first + it->second - 1))
            result = findSection(it->first);
        else
            break;
    }

    return result;
}

size_t HexGroup::findNextSection(HexAddressType address) const
{
    // Define result and build address map
    size_t result{std::string::npos};
    HexAddressMap map{getAddressMap()};

    // Find section which is higher by address than the provided address
    for (auto it{map.crbegin()}; it != map.crend(); ++it)
    {
        // Find section through the ordered address map with the last
        // higher address than the one provided by the parameter address
        if (address < it->first)
            result = findSection(it->first);
        else
            break;
    }

    return result;
}

size_t HexGroup::checkIntersect(HexAddressEnumType type, const HexSection& section) const
{
    // Check if section supports addresses
    if (!section.hasAddress())
        throw std::domain_error("Section does not support addresses");

    // Check if section is of valid type to check for intersection
    switch (type)
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
            if (section.getSectionType() != HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS)
                throw std::domain_error("Section is not compatible with the group type");
            break;

        case HexAddressEnumType::ADDRESS_I32HEX:
            if (section.getSectionType() != HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS)
                throw std::domain_error("Section is not compatible with the group type");
            break;

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            if (section.getSectionType() != HexSectionType::SECTION_DATA)
                throw std::domain_error("Section is not compatible with the group type");
            break;
    }

    // Check intersect with every section
    for (size_t index{0}; index < sections.size(); ++index)
    {
        const auto& sect{sections.at(index)};
        if (sect.getSectionType() == section.getSectionType())
        {
            // Return index of the section that intersects the section
            if (sect.checkIntersect(section))
                return index;
        }
    }

    // Section does not intersect anything
    return std::string::npos;
}

size_t HexGroup::checkIntersect(const HexSection& section) const
{
    return checkIntersect(getGroupType(), section);
}

size_t HexGroup::createSection(HexAddressEnumType type, HexAddressType address)
{
    // Determine group type and if the address type is applicable with the group type
    const auto groupType{getGroupType()};
    if ((type != groupType) && hasDataSections())
        throw std::domain_error("Type of address not applicable to the existing sections of the group");

    // Try to create section depending on the group type
    switch (type)
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
        {
            // Try to find section which already contains the absolute address
            const auto sectionIndex{findSection(address)};
            if (sectionIndex != std::string::npos)
                return sectionIndex;

            // Find the extended segment address that fits the absolute address
            const auto segmentAddress{HexAddress::findExtendedSegmentAddress(address)};

            // Create a new section that includes the new address
            HexSection section{HexRecord::makeExtendedSegmentAddressRecord(segmentAddress)};

            // Check intersect with the existing sections
            if (checkIntersect(type, section) == std::string::npos)
            {
                // If no existing sections have the absolute address that we want,
                // push the new section
                return pushSection(section);
            }
            else
            {
                // Loop through all the sections trying to find adjacent section
                // containing the absolute address and not intersecting with
                // any existing sections at the same time
                bool allowPush{false};
                for (size_t index{0}; index < sections.size(); ++index)
                {
                    const auto& sect{sections.at(index)};
                    if (sect.getSectionType() == HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS)
                    {
                        // Check if forward-adjacent and backward-adjacent section contains the absolute address
                        if (HexAddress::containsAbsoluteSegmentAddress(address, HexAddress::getNextSegmentAddress(sect.getExtendedSegmentAddress())))
                        {
                            section.setExtendedSegmentAddress(HexAddress::getNextSegmentAddress(sect.getExtendedSegmentAddress()));
                            if (checkIntersect(type, section) == std::string::npos)
                            {
                                // Section contains the address and does not intersect any existing sections
                                allowPush = true;
                                break;
                            }
                        }
                        else if (HexAddress::containsAbsoluteSegmentAddress(address, HexAddress::getPreviousSegmentAddress(sect.getExtendedSegmentAddress())))
                        {
                            section.setExtendedSegmentAddress(HexAddress::getPreviousSegmentAddress(sect.getExtendedSegmentAddress()));
                            if (checkIntersect(type, section) == std::string::npos)
                            {
                                // Section contains the address and does not intersect any existing sections
                                allowPush = true;
                                break;
                            }
                        }
                    }
                }

                // Can we push the section from the previous step?
                if (allowPush)
                {
                    // Push the section
                    return pushSection(section);
                }
                else
                    // An unfortunate case where extended segment address sections
                    // are aligned such that we can not add the newly created section
                    // without intersecting with at least one of the existing sections
                    throw std::runtime_error("Unable to create aligned SECTION_EXTENDED_SEGMENT_ADDRESS section");
            }
        }

        case HexAddressEnumType::ADDRESS_I32HEX:
        {
            // Find the extended linear address that fits the absolute address
            const auto linearAddress{HexAddress::findExtendedLinearAddress(address)};

            // Create a new section that includes the new address
            HexSection section{HexRecord::makeExtendedLinearAddressRecord(linearAddress)};

            // Check that none of the existing sections intersect with the new section
            for (size_t index{0}; index < sections.size(); ++index)
            {
                const auto& sect{sections.at(index)};
                if (sect.getSectionType() == HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS)
                {
                    // Section that intersects with the newly created section, already has the address
                    // so we do not push the section and just return the existing section index
                    if (sect.checkIntersect(section))
                        return index;
                }
            }

            // If no existing sections have the absolute address that we want,
            // push the new section
            return pushSection(section);
            break;
        }

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
            if (!HexAddress::isValidAbsoluteDataRecordAddress(address))
                throw std::out_of_range("Absolute address is out of range for an ADDRESS_I8HEX address space");

            // Iterate through all the existing sections
            for (size_t index{0}; index < sections.size(); ++index)
            {
                // There can be only one SECTION_DATA so just return the section index
                const auto& sect{sections.at(index)};
                if (sect.getSectionType() == HexSectionType::SECTION_DATA)
                    return index;
            }

            // If there is no SECTION_DATA section, create one and push it
            HexSection section{};
            section.clearData();
            return pushSection(section);
            break;
    }
}

size_t HexGroup::createSection(HexAddressType address)
{
    return createSection(getGroupType(), address);
}

size_t HexGroup::createSection(HexAddressEnumType type, HexAddressType address, HexDataSizeType size)
{
    // Determine group type and if the address type is applicable with the group type
    const auto groupType{getGroupType()};
    if ((type != groupType) && hasDataSections())
        throw std::domain_error("Type of address not applicable to the existing sections of the group");

    // Check if address range is valid according to the address type
    if (!HexAddress::isValidAbsoluteAddressRange(type, address, size))
        throw std::out_of_range("Absolute address is out of range for the current group type");

    // Define last address
    HexAddressType last{address + static_cast<HexAddressType>(size) - 1};

    // Define current size and result
    size_t result{std::string::npos};
    HexDataSizeType currentSize{0};

    // Iterate through the address space
    while (currentSize < size)
    {
        // Try to create section depending on the group type
        switch (type)
        {
            case HexAddressEnumType::ADDRESS_I16HEX:
            {
                // Try to find section which already contains the absolute address
                const auto sectionIndex{findSection(address + currentSize)};
                if (sectionIndex != std::string::npos)
                {
                    // Find section
                    const auto sect{getSection(sectionIndex)};

                    // Calculate next address
                    const auto next{static_cast<HexDataSizeType>(std::min(last, sect.getMaxAbsoluteAddress()) + 1)};

                    // Update current size and result
                    currentSize += static_cast<HexDataSizeType>(next - (address + currentSize));
                    result = sectionIndex;
                    continue;
                }

                // Find the extended segment address that fits the absolute address
                const auto segmentAddress{HexAddress::findExtendedSegmentAddress(address + currentSize)};

                // Create a new section that includes the new address
                HexSection section{HexRecord::makeExtendedSegmentAddressRecord(segmentAddress)};

                // Check intersect with the existing sections
                if (checkIntersect(type, section) == std::string::npos)
                {
                    // If no existing sections have the absolute address that we want,
                    // push the new section
                    result = pushSection(section);

                    // Calculate next address
                    const auto next{static_cast<HexDataSizeType>(std::min(last, section.getMaxAbsoluteAddress()) + 1)};

                    // Update current size
                    currentSize += static_cast<HexDataSizeType>(next - (address + currentSize));
                    continue;
                }
                else
                {
                    // Loop through all the sections trying to find adjacent section
                    // containing the absolute address and not intersecting with
                    // any existing sections at the same time
                    bool allowPush{false};
                    for (size_t index{0}; index < sections.size(); ++index)
                    {
                        const auto& sect{sections.at(index)};
                        if (sect.getSectionType() == HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS)
                        {
                            // Check if forward-adjacent and backward-adjacent section contains the absolute address
                            if (HexAddress::containsAbsoluteSegmentAddress(address + currentSize, HexAddress::getNextSegmentAddress(sect.getExtendedSegmentAddress())))
                            {
                                section.setExtendedSegmentAddress(HexAddress::getNextSegmentAddress(sect.getExtendedSegmentAddress()));
                                if (checkIntersect(type, section) == std::string::npos)
                                {
                                    // Section contains the address and does not intersect any existing sections
                                    allowPush = true;
                                    break;
                                }
                            }
                            else if (HexAddress::containsAbsoluteSegmentAddress(address + currentSize, HexAddress::getPreviousSegmentAddress(sect.getExtendedSegmentAddress())))
                            {
                                section.setExtendedSegmentAddress(HexAddress::getPreviousSegmentAddress(sect.getExtendedSegmentAddress()));
                                if (checkIntersect(type, section) == std::string::npos)
                                {
                                    // Section contains the address and does not intersect any existing sections
                                    allowPush = true;
                                    break;
                                }
                            }
                        }
                    }

                    // Can we push the section from the previous step?
                    if (allowPush)
                    {
                        // Push the section
                        result = pushSection(section);

                        // Calculate next address
                        const auto next{static_cast<HexDataSizeType>(std::min(last, section.getMaxAbsoluteAddress()) + 1)};

                        // Update current size
                        currentSize += static_cast<HexDataSizeType>(next - (address + currentSize));
                        continue;
                    }
                    else
                        // An unfortunate case where extended segment address sections
                        // are aligned such that we can not add the newly created section
                        // without intersecting with at least one of the existing sections
                        throw std::runtime_error("Unable to create aligned SECTION_EXTENDED_SEGMENT_ADDRESS section");
                }
            }

            case HexAddressEnumType::ADDRESS_I32HEX:
            {
                // Find the extended linear address that fits the absolute address
                const auto linearAddress{HexAddress::findExtendedLinearAddress(address + currentSize)};

                // Create a new section that includes the new address
                HexSection section{HexRecord::makeExtendedLinearAddressRecord(linearAddress)};

                // Check that none of the existing sections intersect with the new section
                bool allowPush{true};
                for (size_t index{0}; index < sections.size(); ++index)
                {
                    const auto& sect{sections.at(index)};
                    if (sect.getSectionType() == HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS)
                    {
                        // Section that intersects with the newly created section, already has the address
                        // so we do not push the section
                        if (sect.checkIntersect(section))
                        {
                            // Do not push the newly created section
                            allowPush = false;

                            // Calculate next address
                            const auto next{static_cast<HexDataSizeType>(std::min(last, sect.getMaxAbsoluteAddress()) + 1)};

                            // Update current size and result
                            currentSize += static_cast<HexDataSizeType>(next - (address + currentSize));
                            result = index;
                            break;
                        }
                    }
                }

                if (allowPush)
                {
                    // If no existing sections have the absolute address that we want,
                    // push the new section
                    result = pushSection(section);

                    // Calculate next address
                    const auto next{static_cast<HexDataSizeType>(std::min(last, section.getMaxAbsoluteAddress()) + 1)};

                    // Update current size
                    currentSize += static_cast<HexDataSizeType>(next - (address + currentSize));
                    continue;
                }
                break;
            }

            case HexAddressEnumType::ADDRESS_I8HEX:
            default:
                // Iterate through all the existing sections
                for (size_t index{0}; index < sections.size(); ++index)
                {
                    // There can be only one SECTION_DATA so just return the section index
                    const auto& sect{sections.at(index)};
                    if (sect.getSectionType() == HexSectionType::SECTION_DATA)
                        return index;
                }

                // If there is no SECTION_DATA section, create one and push it
                HexSection section{};
                section.clearData();
                return pushSection(section);
                break;
        }
    }

    // Return last index of a found or created section
    return result;
}

size_t HexGroup::createSection(HexAddressType address, HexDataSizeType size)
{
    return createSection(getGroupType(), address, size);
}

void HexGroup::removeSection(size_t index)
{
    if ((sections.size() > 0) && (index < sections.size()))
    {
        auto it{sections.cbegin()};
        std::advance(it, index);
        sections.erase(it);
    }
    else
        throw std::out_of_range("Section index is out of range");
}

void HexGroup::clearSections()
{
    sections.clear();
}

bool HexGroup::canPushSection(const HexSection& section, size_t& index) const
{
    index = std::string::npos;
    switch (getGroupType())
    {
        case HexAddressEnumType::ADDRESS_I16HEX:
        {
            switch (section.getSectionType())
            {
                case HexSectionType::SECTION_END_OF_FILE:
                    for (const auto& sect : sections)
                    {
                        // Only allow one section
                        if (sect.getSectionType() == HexSectionType::SECTION_END_OF_FILE)
                            return false;
                    }

                    index = getSectionDefaultPushIndex(section);
                    return true;
                    break;

                case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
                    for (const auto& sect : sections)
                    {
                        if (sect.getSectionType() == HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS)
                        {
                            // Check if section to be pushed intersects
                            // with any of the existing sections
                            if (sect.checkIntersect(section))
                                return false;
                        }
                    }

                    index = getSectionDefaultPushIndex(section);
                    return true;
                    break;

                case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
                    for (const auto& sect : sections)
                    {
                        // Only allow one section
                        if (sect.getSectionType() == HexSectionType::SECTION_START_SEGMENT_ADDRESS)
                            return false;
                    }

                    index = getSectionDefaultPushIndex(section);
                    return true;
                    break;

                case HexSectionType::SECTION_DATA:
                case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
                case HexSectionType::SECTION_START_LINEAR_ADDRESS:
                default:
                    return false;
                    break;
            }
            break;
        }

        case HexAddressEnumType::ADDRESS_I32HEX:
        {
            switch (section.getSectionType())
            {
                case HexSectionType::SECTION_END_OF_FILE:
                    for (const auto& sect : sections)
                    {
                        // Only allow one section
                        if (sect.getSectionType() == HexSectionType::SECTION_END_OF_FILE)
                            return false;
                    }

                    index = getSectionDefaultPushIndex(section);
                    return true;
                    break;

                case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
                    for (const auto& sect : sections)
                    {
                        if (sect.getSectionType() == HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS)
                        {
                            // Check if section to be pushed intersects
                            // with any of the existing sections
                            if (sect.checkIntersect(section))
                                return false;
                        }
                    }

                    index = getSectionDefaultPushIndex(section);
                    return true;
                    break;

                case HexSectionType::SECTION_START_LINEAR_ADDRESS:
                    for (const auto& sect : sections)
                    {
                        // Only allow one section
                        if (sect.getSectionType() == HexSectionType::SECTION_START_LINEAR_ADDRESS)
                            return false;
                    }

                    index = getSectionDefaultPushIndex(section);
                    return true;
                    break;

                case HexSectionType::SECTION_DATA:
                case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
                case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
                default:
                    return false;
                    break;
            }
            break;
        }

        case HexAddressEnumType::ADDRESS_I8HEX:
        default:
        {
            switch (section.getSectionType())
            {
                case HexSectionType::SECTION_END_OF_FILE:
                    for (const auto& sect : sections)
                    {
                        // Only allow one section
                        if (sect.getSectionType() == HexSectionType::SECTION_END_OF_FILE)
                            return false;
                    }

                    index = getSectionDefaultPushIndex(section);
                    return true;
                    break;

                case HexSectionType::SECTION_DATA:
                case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
                case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
                case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
                case HexSectionType::SECTION_START_LINEAR_ADDRESS:
                default:
                    for (const auto& sect : sections)
                    {
                        // Do not allow to push section if group has
                        // SECTION_DATA section already
                        if (sect.getSectionType() == HexSectionType::SECTION_DATA)
                            return false;
                    }

                    index = getSectionDefaultPushIndex(section);
                    return true;
                    break;
            }
            break;
        }
    }
}

bool HexGroup::canPushSection(const HexSection& section) const
{
    size_t index;
    return canPushSection(section, index);
}

size_t HexGroup::pushSection(const HexSection& section)
{
    size_t index{std::string::npos};
    const bool result{canPushSection(section, index)};

    if (result)
    {
        if (index >= sections.size())
        {
            sections.push_back(section);
            sections.back().setUnusedDataFillValue(unusedDataFillValue);
        }
        else
        {
            auto it{sections.begin()};
            std::advance(it, index);
            sections.insert(it, section);
            sections.at(index).setUnusedDataFillValue(unusedDataFillValue);
        }
    }

    return index;
}

HexAddressMap HexGroup::getAddressMap() const
{
    // Iterate through all the sections
    HexAddressMap result{};
    for (const auto& sect : sections)
    {
        // Add mapping of the data sections only
        if (sect.hasAddress())
            result.merge(sect.getAddressMap());
    }

    // Return address map
    return result;
}

HexAddressMap HexGroup::getDataMap() const
{
    // Iterate through all the sections
    HexAddressMap result{};
    for (const auto& sect : sections)
    {
        // Add mapping of the data sections only
        if (sect.hasAddress())
            result.merge(sect.getDataMap());
    }

    // Return address map
    return result;
}

void HexGroup::clearData()
{
    // Delete sections with data
    for (auto it{sections.begin()}; it != sections.end();)
    {
        if (it->hasAddress())
            it = sections.erase(it);
        else
            ++it;
    }
}

void HexGroup::clearData(HexAddressType address)
{
    // Check if address is valid according to the address type
    if (!HexAddress::isValidAbsoluteAddress(getGroupType(), address))
        throw std::out_of_range("Absolute address is out of range for the current group type");

    // Iterate through all the sections to find the data
    for (auto& sect : sections)
    {
        if (sect.hasData() && sect.hasAbsoluteAddress(address))
            sect.clearData(sect.getRelativeAddress(address));
    }
}

HexDataSizeType HexGroup::clearData(HexAddressType address, HexDataSizeType dataSize)
{
    // Check if address range is valid according to the address type
    if (!HexAddress::isValidAbsoluteAddressRange(getGroupType(), address, dataSize))
        throw std::out_of_range("Absolute address is out of range for the current group type");

    // Build address map
    HexAddressMap map{getAddressMap()};

    // Define result
    HexDataSizeType result{0};
    while (result < dataSize)
    {
        // Current address
        const auto current{address + result};

        // Find range in the address map
        auto it{map.cbegin()};
        for (; it != map.cend(); ++it)
        {
            // Range contains the current address or is range before the first range?
            if ((current >= it->first) && (current < (it->first + it->second)))
                break;
            else if (current < it->first)
                break;
        }

        if (it == map.cend())
        {
            // There is no data to clear, increment result
            result += (dataSize - result);
        }
        else if (current < it->first)
        {
            // Current address is before the first range, so
            // calculate the difference from the current address
            // to the start of the range
            const HexDataSizeType difference{it->first - current};

            // Calculate the size with which result needs to be incremented
            const HexDataSizeType size{std::min(dataSize - result, difference)};
            result += size;
        }
        else
        {
            // Find section from the range
            HexSection& sect{getSection(findSection(current))};

            // Calculate the size of the data to get from the section
            const HexDataSizeType size{std::min(dataSize - result, ((it->first + it->second) - current))};

            // Clear data and increment the result
            result += sect.clearData(sect.getRelativeAddress(current), size);
        }
    }

    // Return result
    return result;
}

HexDataType HexGroup::getData(HexAddressType address) const
{
    // Check if address is valid according to the address type
    if (!HexAddress::isValidAbsoluteAddress(getGroupType(), address))
        throw std::out_of_range("Absolute address is out of range for the current group type");

    // Iterate through all the sections to find the data
    for (const auto& sect : sections)
    {
        if (sect.hasData() && sect.hasAbsoluteAddress(address))
            return sect.getData(sect.getRelativeAddress(address));
    }

    // No section found with the data
    return unusedDataFillValue;
}

HexDataSizeType HexGroup::getData(HexAddressType address, HexDataType* data, HexDataSizeType dataSize) const
{
    // Check if address range is valid according to the address type
    if (!HexAddress::isValidAbsoluteAddressRange(getGroupType(), address, dataSize))
        throw std::out_of_range("Absolute address is out of range for the current group type");

    // Build address map
    HexAddressMap map{getAddressMap()};

    // Define result and processed counts
    HexDataSizeType result{0}, processed{0};
    while (processed < dataSize)
    {
        // Current address
        const auto current{address + processed};

        // Find range in the address map
        auto it{map.cbegin()};
        for (; it != map.cend(); ++it)
        {
            // Range contains the current address or range is
            // after the current range?
            if ((current >= it->first) && (current < (it->first + it->second)))
                break;
            else if (current < it->first)
                break;
        }

        if (it == map.cend())
        {
            // No ranges at or after the current address, use unused data fill value
            for (HexDataSizeType index{processed}; index < dataSize; ++index)
                data[index] = unusedDataFillValue;

            // Increment processed count, but not the result
            processed += (dataSize - processed);
        }
        else if (it->first > current)
        {
            // Calculate difference from the current address to the start of the range
            const HexDataSizeType difference{it->first - current};

            // Calculate size for unused data fill value
            const HexDataSizeType size{std::min(dataSize - processed, difference)};

            // Use unused data fill value
            for (HexDataSizeType index{0}; index < size; ++index)
                data[processed + index] = unusedDataFillValue;

            // Increment processed count, but not the result
            processed += size;
        }
        else
        {
            // Find section from the range
            const HexSection& sect{getSection(findSection(it->first))};

            // Calculate size of the data to get from the section
            const HexDataSizeType size{std::min(dataSize - processed, ((it->first + it->second) - current))};

            // Get data, increment the result and processed count
            result += sect.getData(sect.getRelativeAddress(current), &data[processed], size);
            processed += size;
        }
    }

    // Return result
    return result;
}

HexDataSizeType HexGroup::getData(HexAddressType address, HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset) const
{
    // Data vector should be at least the size of the offset
    if (data.size() < offset)
        throw std::underflow_error("Size of the data vector is smaller than the offset");

    // Check if address range is valid according to the address type
    if (!HexAddress::isValidAbsoluteAddressRange(getGroupType(), address, dataSize))
        throw std::out_of_range("Absolute address is out of range for the current group type");

    // Build address map
    HexAddressMap map{getAddressMap()};

    // Define result and processed counts
    HexDataSizeType result{0}, processed{0};
    while (processed < dataSize)
    {
        // Current address
        const auto current{address + processed};

        // Find range in the address map
        auto it{map.cbegin()};
        for (; it != map.cend(); ++it)
        {
            // Range contains the current address or range is
            // after the current range?
            if ((current >= it->first) && (current < (it->first + it->second)))
                break;
            else if (current < it->first)
                break;
        }

        if (it == map.cend())
        {
            // No ranges at or after the current address, use unused data fill value
            for (HexDataSizeType index{processed}; index < dataSize; ++index)
            {
                if (data.size() <= (index + offset))
                    data.push_back(unusedDataFillValue);
                else
                    data[index + offset] = unusedDataFillValue;
            }

            // Increment processed count, but not the result
            processed += (dataSize - processed);
        }
        else if (it->first > current)
        {
            // Calculate difference from the current address to the start of the range
            const HexDataSizeType difference{it->first - current};

            // Calculate size for unused data fill value
            const HexDataSizeType size{std::min(dataSize - processed, difference)};

            // Use unused data fill value
            for (HexDataSizeType index{0}; index < size; ++index)
            {
                if (data.size() <= (processed + index + offset))
                    data.push_back(unusedDataFillValue);
                else
                    data[processed + index + offset] = unusedDataFillValue;
            }

            // Increment processed count, but not the result
            processed += size;
        }
        else
        {
            // Find section from the range
            const HexSection& sect{getSection(findSection(it->first))};

            // Calculate size of the data to get from the section
            const HexDataSizeType size{std::min(dataSize - processed, ((it->first + it->second) - current))};

            // Get data, increment the result and processed count
            result += sect.getData(sect.getRelativeAddress(current), data, size, processed + offset);
            processed += size;
        }
    }

    // Return result
    return result;
}

void HexGroup::setData(HexAddressType address, HexDataType data)
{
    // Create section or find index if already existing
    const auto index{createSection(address)};
    auto& section{sections.at(index)};

    // Set data
    section.setData(section.getRelativeAddress(address), data);
}

HexDataSizeType HexGroup::setData(HexAddressType address, HexDataType* data, HexDataSizeType dataSize)
{
    // Create section(s) for all the addresses
    createSection(address, dataSize);

    // Build address map
    HexAddressMap map{getAddressMap()};

    // Define result and find range in the address map
    HexDataSizeType result{0};
    for (auto it{map.cbegin()}; it != map.cend(); ++it)
    {
        // Are we done?
        if (result == dataSize)
            break;

        // Range contains the current address?
        const auto current{address + result};
        if (current < (it->first + it->second))
        {
            // Find section from the range
            HexSection& sect{getSection(findSection(current))};

            // Calculate size of the data to set in the section
            const HexDataSizeType size{std::min(dataSize - result, ((it->first + it->second) - current))};

            // Set data and increment the result
            result += sect.setData(sect.getRelativeAddress(current), &data[result], size);
        }
    }

    // Return result
    return result;
}

HexDataSizeType HexGroup::setData(HexAddressType address, HexDataVector& data, HexDataSizeType dataSize, HexDataSizeType offset)
{
    // Offset range check
    if (offset > data.size())
        return 0;

    // Size range check
    if ((offset + dataSize) > data.size())
        dataSize = (data.size() - offset);

    // Create section(s) for all the addresses
    createSection(address, dataSize);

    // Build address map
    HexAddressMap map{getAddressMap()};

    // Define result and find range in the address map
    HexDataSizeType result{0};
    for (auto it{map.cbegin()}; it != map.cend(); ++it)
    {
        // Are we done?
        if (result == dataSize)
            break;

        // Range contains the current address?
        const auto current{address + result};
        if (current < (it->first + it->second))
        {
            // Find section from the range
            HexSection& sect{getSection(findSection(current))};

            // Calculate size of the data to set in the section
            const HexDataSizeType size{std::min(dataSize - result, ((it->first + it->second) - current))};

            // Set data and increment the result
            result += sect.setData(sect.getRelativeAddress(current), data, size, result + offset);
        }
    }

    // Return result
    return result;
}

HexDataSizeType HexGroup::fillData(HexAddressType address, HexDataSizeType dataSize, HexDataType data)
{
    // Create section(s) for all the addresses
    createSection(address, dataSize);

    // Build address map
    HexAddressMap map{getAddressMap()};

    // Define result and find range in the address map
    HexDataSizeType result{0};
    for (auto it{map.cbegin()}; it != map.cend(); ++it)
    {
        // Are we done?
        if (result == dataSize)
            break;

        // Range contains the current address?
        const auto current{address + result};
        if (current < (it->first + it->second))
        {
            // Find section from the range
            HexSection& sect{getSection(findSection(current))};

            // Calculate size of the data to set in the section
            const HexDataSizeType size{std::min(dataSize - result, ((it->first + it->second) - current))};

            // Set data and increment the result
            result += sect.fillData(sect.getRelativeAddress(current), size, data);
        }
    }

    // Return result
    return result;
}

HexAddressEnumType HexGroup::getGroupType() const
{
    HexAddressEnumType result{HexAddressEnumType::ADDRESS_I8HEX};
    for (const auto& section : sections)
    {
        switch (section.getSectionType())
        {
            case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
                return HexAddressEnumType::ADDRESS_I16HEX;
                break;


            case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            case HexSectionType::SECTION_START_LINEAR_ADDRESS:
                return HexAddressEnumType::ADDRESS_I32HEX;
                break;

            case HexSectionType::SECTION_DATA:
            case HexSectionType::SECTION_END_OF_FILE:
            default:
                break;
        }
    }
    return result;
}

bool HexGroup::hasDataSections() const
{
    for (const auto& sect : sections)
    {
        switch (sect.getSectionType())
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
                break;
        }
    }

    return false;
}

HexDataType HexGroup::getUnusedDataFillValue() const
{
    return unusedDataFillValue;
}

void HexGroup::setUnusedDataFillValue(HexDataType unusedDataFillValue)
{
    this->unusedDataFillValue = unusedDataFillValue;
    for (auto& sect : sections)
        sect.setUnusedDataFillValue(unusedDataFillValue);
}

bool HexGroup::supportsStartSegmentAddress() const
{
    for (const auto& sect : sections)
    {
        switch (sect.getSectionType())
        {
            case HexSectionType::SECTION_END_OF_FILE:
            case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
                break;

            case HexSectionType::SECTION_DATA:
            case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            case HexSectionType::SECTION_START_LINEAR_ADDRESS:
            default:
                return false;
                break;
        }
    }

    return true;
}

bool HexGroup::hasStartSegmentAddress() const
{
    for (const auto& sect : sections)
        if (sect.getSectionType() == HexSectionType::SECTION_START_SEGMENT_ADDRESS)
            return true;

    return false;
}

void HexGroup::clearStartSegmentAddress()
{
    for (auto it{sections.begin()}; it != sections.end();)
    {
        if (it->getSectionType() == HexSectionType::SECTION_START_SEGMENT_ADDRESS)
            it = sections.erase(it);
        else
            ++it;
    }
}

const HexSection& HexGroup::getStartSegmentAddressSection() const
{
    for (const auto& sect : sections)
        if (sect.getSectionType() == HexSectionType::SECTION_START_SEGMENT_ADDRESS)
            return sect;

    throw std::runtime_error("SECTION_START_SEGMENT_ADDRESS section not supported or found");
}

HexSection& HexGroup::getStartSegmentAddressSection()
{
    for (auto& sect : sections)
        if (sect.getSectionType() == HexSectionType::SECTION_START_SEGMENT_ADDRESS)
            return sect;

    throw std::runtime_error("SECTION_START_SEGMENT_ADDRESS section not supported or found");
}

uint16_t HexGroup::getCodeSegment() const
{
    if (supportsStartSegmentAddress() && hasStartSegmentAddress())
    {
        const auto& sect{getStartSegmentAddressSection()};
        return sect.getStartSegmentAddressCodeSegment();
    }
    else
        throw std::runtime_error("SECTION_START_SEGMENT_ADDRESS section not supported or found");
}

void HexGroup::setCodeSegment(uint16_t codeSegment)
{
    if (supportsStartSegmentAddress())
    {
        if (hasStartSegmentAddress())
        {
            auto& sect{getStartSegmentAddressSection()};
            sect.setStartSegmentAddressCodeSegment(codeSegment);
        }
        else
        {
            auto sect{HexSection::makeStartSegmentAddressSection(codeSegment, 0)};
            pushSection(sect);
        }
    }
    else
        throw std::runtime_error("SECTION_START_SEGMENT_ADDRESS section not supported in current group type");
}

uint16_t HexGroup::getInstructionPointer() const
{
    if (supportsStartSegmentAddress() && hasStartSegmentAddress())
    {
        const auto& sect{getStartSegmentAddressSection()};
        return sect.getStartSegmentAddressInstructionPointer();
    }
    else
        throw std::runtime_error("SECTION_START_SEGMENT_ADDRESS section not supported or found");
}

void HexGroup::setInstructionPointer(uint16_t instructionPointer)
{
    if (supportsStartSegmentAddress())
    {
        if (hasStartSegmentAddress())
        {
            auto& sect{getStartSegmentAddressSection()};
            sect.setStartSegmentAddressInstructionPointer(instructionPointer);
        }
        else
        {
            auto sect{HexSection::makeStartSegmentAddressSection(0, instructionPointer)};
            pushSection(sect);
        }
    }
    else
        throw std::runtime_error("SECTION_START_SEGMENT_ADDRESS section not supported in current group type");
}

bool HexGroup::supportsStartLinearAddress() const
{
    for (const auto& sect : sections)
    {
        switch (sect.getSectionType())
        {
            case HexSectionType::SECTION_END_OF_FILE:
            case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
            case HexSectionType::SECTION_START_LINEAR_ADDRESS:
                break;

            case HexSectionType::SECTION_DATA:
            case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
            case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
            default:
                return false;
                break;
        }
    }

    return true;
}

bool HexGroup::hasStartLinearAddress() const
{
    for (const auto& sect : sections)
        if (sect.getSectionType() == HexSectionType::SECTION_START_LINEAR_ADDRESS)
            return true;

    return false;
}

void HexGroup::clearStartLinearAddress()
{
    for (auto it{sections.begin()}; it != sections.end();)
    {
        if (it->getSectionType() == HexSectionType::SECTION_START_LINEAR_ADDRESS)
            it = sections.erase(it);
        else
            ++it;
    }
}

const HexSection& HexGroup::getStartLinearAddressSection() const
{
    for (const auto& sect : sections)
        if (sect.getSectionType() == HexSectionType::SECTION_START_LINEAR_ADDRESS)
            return sect;

    throw std::runtime_error("SECTION_START_LINEAR_ADDRESS section not supported or found");
}

HexSection& HexGroup::getStartLinearAddressSection()
{
    for (auto& sect : sections)
        if (sect.getSectionType() == HexSectionType::SECTION_START_LINEAR_ADDRESS)
            return sect;

    throw std::runtime_error("SECTION_START_LINEAR_ADDRESS section not supported or found");
}

uint32_t HexGroup::getExtendedInstructionPointer() const
{
    if (supportsStartLinearAddress() && hasStartLinearAddress())
    {
        const auto& sect{getStartLinearAddressSection()};
        return sect.getStartLinearAddressExtendedInstructionPointer();
    }
    else
        throw std::runtime_error("SECTION_START_LINEAR_ADDRESS section not supported or found");
}

void HexGroup::setExtendedInstructionPointer(uint32_t extendedInstructionPointer)
{
    if (supportsStartLinearAddress())
    {
        if (hasStartLinearAddress())
        {
            auto& sect{getStartLinearAddressSection()};
            sect.setStartLinearAddressExtendedInstructionPointer(extendedInstructionPointer);
        }
        else
        {
            auto sect{HexSection::makeStartLinearAddressSection(extendedInstructionPointer)};
            pushSection(sect);
        }
    }
    else
        throw std::runtime_error("SECTION_START_LINEAR_ADDRESS section not supported in current group type");
}

size_t HexGroup::getSectionDefaultPushIndex(const HexSection& section) const
{
    switch (section.getSectionType())
    {
        case HexSectionType::SECTION_END_OF_FILE:
            return sections.size();
            break;

        case HexSectionType::SECTION_DATA:
        case HexSectionType::SECTION_EXTENDED_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_START_SEGMENT_ADDRESS:
        case HexSectionType::SECTION_EXTENDED_LINEAR_ADDRESS:
        case HexSectionType::SECTION_START_LINEAR_ADDRESS:
        default:
            // If the last section is SECTION_END_OF_FILE return the index before it
            if ((sections.size() > 0) && (sections.back().getSectionType() == HexSectionType::SECTION_END_OF_FILE))
                return (sections.size() - 1);
            else
                return sections.size();

            break;
    }
}

END_NAMESPACE_LIBIHEX
