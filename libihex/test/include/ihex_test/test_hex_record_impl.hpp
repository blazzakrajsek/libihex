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

#include <gtest/gtest.h>
#include <ihex/hex_record.hpp>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

/**
 * @brief HexRecordTest class
 *
 */
class HexRecordTest : public testing::Test
{
    public:
        /**
         * @brief Construct a new HexRecordTest object
         *
         */
        HexRecordTest();

        /**
         * @brief Destroy the HexRecordTest object
         *
         */
        virtual ~HexRecordTest();

    protected:
        /**
         * @brief Set up the test
         *
         */
        virtual void SetUp() override;

        /**
         * @brief Tear down the test
         *
         */
        virtual void TearDown() override;

        /**
         * @brief Perform tests on contructor functions
         *
         */
        void performConstructorTests();

        /**
         * @brief Perform tests on makeRecord() functions
         *
         */
        void performMakeRecordTests();

        /**
         * @brief Perform tests on prepareRecord() functions
         *
         */
        void performPrepareRecordTests();

        /**
         * @brief Perform tests on prepareDataRecord() functions
         *
         */
        void performPrepareDataRecordTests();

        /**
         * @brief Perform tests on prepareEndOfFileRecord() function
         *
         */
        void performPrepareEndOfFileRecordTests();

        /**
         * @brief Perform tests on prepareExtendedSegmentAddressRecord() function
         *
         */
        void performPrepareExtendedSegmentAddressRecordTests();

        /**
         * @brief Perform tests on prepareStartSegmentAddressRecord() function
         *
         */
        void performPrepareStartSegmentAddressRecordTests();

        /**
         * @brief Perform tests on prepareExtendedLinearAddressRecord() function
         *
         */
        void performPrepareExtendedLinearAddressRecordTests();

        /**
         * @brief Perform tests on prepareStartLinearAddressRecord() function
         *
         */
        void performPrepareStartLinearAddressRecordTests();

        /**
         * @brief Perform tests on record property functions
         *
         */
        void performGetSetRecordPropertiesTests();

        /**
         * @brief Perform tests on getData() functions
         *
         */
        void performGetDataPropertiesTests();

        /**
         * @brief Perform tests on setData() functions
         *
         */
        void performSetDataPropertiesTests();

        /**
         * @brief Perform tests on setRecord() function
         *
         */
        void performSetRecordTests();

        /**
         * @brief Perform tests on checksum functions
         *
         */
        void performChecksumTests();

        /**
         * @brief Perform tests on hasAddress() function
         *
         */
        void performHasAddressFunctionTests();

        /**
         * @brief Perform tests on isValidRecord() functions
         *
         */
        void performIsValidRecordTests();

        /**
         * @brief Perform tests on extended segment address related functions
         *
         */
        void performExtendedSegmentAddressFunctionTests();

        /**
         * @brief Perform tests on start segment address related functions
         *
         */
        void performStartSegmentAddressFunctionTests();

        /**
         * @brief Perform tests on extended linear address related functions
         *
         */
        void performExtendedLinearAddressFunctionTests();

        /**
         * @brief Perform tests on start linear address related functions
         *
         */
        void performStartLinearAddressFunctionTests();

        /**
         * @brief Pointer to a test record
         *
         */
        HexRecordPtr record;
};

END_NAMESPACE_LIBIHEX
