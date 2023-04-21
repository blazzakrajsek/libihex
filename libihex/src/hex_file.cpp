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

#include <fstream>
#include <iostream>
#include <ihex/hex_file.hpp>
#include <ihex/hex_group.hpp>
#include <ihex/hex_section.hpp>

BEGIN_NAMESPACE_LIBIHEX

HexFile::HexFile() :
    HexGroup{}, enableThrowOnInvalidRecord{true}, enableThrowOnChecksumMismatch{true}
{

}

HexFile::~HexFile()
{

}

bool HexFile::load(const std::string& fileName)
{
    // Open stream for reading
    std::ifstream stream{fileName.c_str(), std::ios_base::in};

    // Load data from stream
    auto result{load(stream)};

    // Close stream and return result
    stream.close();
    return result;
}

bool HexFile::load(std::istream& stream)
{
    // Result
    bool result{stream};

    // Load records while stream is good
    while (stream)
    {
        // Read and load record
        std::string line;
        std::getline(stream, line);

        // Ignore empty lines
        if (line.size() == 0)
            continue;

        // Parse record
        HexRecord record;
        if (!record.setRecord(line))
        {
            if (enableThrowOnInvalidRecord)
                throw std::runtime_error(std::string("Invalid record found: " + line).c_str());
            else
            {
                result = false;
                continue;
            }
        }

        // Verify record checksum
        if (!record.isValidChecksum())
        {
            if (enableThrowOnChecksumMismatch)
                throw std::logic_error(std::string("Record checksum mismatch found: " + line).c_str());
            else
            {
                result = false;
                continue;
            }
        }

        // Create new section if none exist
        if (getSize() == 0)
        {
            HexSection section{record};
            pushSection(section);
            continue;
        }

        auto& section{getSection(getSize() - 1)};
        if (section.canPushRecord(record))
        {
            // Push record to the last section
            section.pushRecord(record);
            continue;
        }
        else
        {
            // If record can not be pushed to
            // the last section, create
            // a new section
            HexSection newSection{record};
            pushSection(newSection);
            continue;
        }
    }

    // Return result
    return result;
}

bool HexFile::save(const std::string& fileName)
{
    // Open stream for writing and truncate existing contents
    std::ofstream stream{fileName.c_str(), std::ios_base::out};

    // Save data to stream
    auto result{save(stream)};

    // Close stream and return result
    stream.close();
    return result;
}

bool HexFile::save(std::ostream& stream)
{
    // Write sections sequentially to a stream
    for (size_t sectionIndex{0}; sectionIndex < getSize(); sectionIndex++)
    {
        // Iterate through all the records
        const HexSection& section{getSection(sectionIndex)};
        for (size_t recordIndex{0}; recordIndex < section.getSize(); recordIndex++)
        {
            // Write every individual record to a stream
            std::string record;
            stream << section.getRecord(recordIndex).getRecord() << HEX_RECORD_LINE_SEPARATOR;
        }
    }

    // Return stream status
    return bool(stream);
}

bool HexFile::getThrowOnInvalidRecord()
{
    return enableThrowOnInvalidRecord;
}

void HexFile::setThrowOnInvalidRecord(bool enableThrowOnInvalidRecord)
{
    this->enableThrowOnInvalidRecord = enableThrowOnInvalidRecord;
}

bool HexFile::getThrowOnChecksumMismatch()
{
    return enableThrowOnChecksumMismatch;
}

void HexFile::setThrowOnChecksumMismatch(bool enableThrowOnChecksumMismatch)
{
    this->enableThrowOnChecksumMismatch = enableThrowOnChecksumMismatch;
}

END_NAMESPACE_LIBIHEX
