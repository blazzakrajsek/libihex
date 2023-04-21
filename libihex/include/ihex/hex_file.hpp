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

#include <memory>
#include <ihex/hex_group.hpp>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

/**
 * @brief HexFile class
 *
 */
class HexFile final : public HexGroup
{
    public:
        /**
         * @brief Construct a new HexFile object
         *
         */
        HexFile();

        /**
         * @brief Destroy the HexFile object
         *
         */
        virtual ~HexFile();

        /**
         * @brief Copy-construct a new HexFile object
         *
         * @param file File to copy-construct from
         */
        HexFile(const HexFile& file) = default;

        /**
         * @brief Move-construct a new HexFile object
         *
         * @param file File to move-construct from
         */
        HexFile(HexFile&& file) = default;

        /**
         * @brief Copy-assignment operator
         *
         * @param file File to copy-assign
         * @return HexFile& Assigned file
         */
        HexFile& operator=(const HexFile& file) = default;

        /**
         * @brief Move-assignment operator
         *
         * @param file File to move-assign
         * @return HexFile& Assigned file
         */
        HexFile& operator=(HexFile&& file) = default;

        /**
         * @brief Load data from file
         *
         * @throw std::runtime_error Invalid record found (throws only if enableThrowOnInvalidRecord is set to true)
         * @throw std::logic_error Record checksum mismatch found (throws only if enableThrowOnChecksumMismatch is set to true)
         *
         * @param fileName File name
         * @return true Successfully loaded data from file
         * @return false Failed to load data from file
         */
        virtual bool load(const std::string& fileName);

        /**
         * @brief Load data from stream
         *
         * @throw std::runtime_error Invalid record found (throws only if enableThrowOnInvalidRecord is set to true)
         * @throw std::logic_error Record checksum mismatch found (throws only if enableThrowOnChecksumMismatch is set to true)
         *
         * @param stream Stream
         * @return true Successfully loaded data from stream
         * @return false Failed to load data from stream
         */
        virtual bool load(std::istream& stream);

        /**
         * @brief Save data to file
         *
         * @param fileName File name
         * @return true Successfully saved data to file
         * @return false Failed to save data to file
         */
        virtual bool save(const std::string& fileName);

        /**
         * @brief Save data to stream
         *
         * @param stream Stream
         * @return true Successfully saved data to stream
         * @return false Failed to save data to stream
         */
        virtual bool save(std::ostream& stream);

        /**
         * @brief Get the option to throw on invalid record
         *
         * @return true Throw on invalid record
         * @return false Do not throw on invalid record
         */
        virtual bool getThrowOnInvalidRecord();

        /**
         * @brief Set the option to throw on invalid record
         *
         * @param enableThrowOnInvalidRecord Enable throwing on invalid record
         */
        virtual void setThrowOnInvalidRecord(bool enableThrowOnInvalidRecord);

        /**
         * @brief Get the option to throw on record checksum mismatch
         *
         * @return true Throw on record checksum mismatch
         * @return false Do not throw on record checksum mismatch
         */
        virtual bool getThrowOnChecksumMismatch();

        /**
         * @brief Set the option to throw on record checksum mismatch
         *
         * @param enableThrowOnChecksumMismatch Enable throwing on record checksum mismatch
         */
        virtual void setThrowOnChecksumMismatch(bool enableThrowOnChecksumMismatch);
    protected:
        /**
         * @brief Enable throwing exception on invalid record
         *
         */
        bool enableThrowOnInvalidRecord;

        /**
         * @brief Enable throwing exception on record checksum mismatch
         *
         */
        bool enableThrowOnChecksumMismatch;
};

/**
 * @brief Shared pointer of HexFile
 *
 */
typedef std::shared_ptr<HexFile> HexFilePtr;

/**
 * @brief Unique pointer of HexFile
 *
 */
typedef std::unique_ptr<HexFile> HexFileUniquePtr;

END_NAMESPACE_LIBIHEX
