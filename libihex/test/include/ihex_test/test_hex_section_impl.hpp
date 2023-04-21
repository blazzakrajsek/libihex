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
#include <ihex/hex_section.hpp>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

/**
 * @brief HexSectionTest class
 *
 */
class HexSectionTest : public testing::Test
{
    public:
        /**
         * @brief Construct a new HexSectionTest object
         *
         */
        HexSectionTest();

        /**
         * @brief Destroy the HexSectionTest object
         *
         */
        virtual ~HexSectionTest();

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
        void performContructorTests();

        /**
         * @brief Perform tests on makeSection() functions
         *
         */
        void performMakeSectionTests();

        /**
         * @brief Perform tests on convertToDataSection() function
         *
         */
        void performConvertToDataSectionTests();

        /**
         * @brief Perform tests on convertToEndOfFileSection() function
         *
         */
        void performConvertToEndOfFileSectionTests();

        /**
         * @brief Perform tests on convertToExtendedSegmentAddressSection() function
         *
         */
        void performConvertToExtendedSegmentAddressSectionTests();

        /**
         * @brief Perform tests on convertToStartSegmentAddressSection() function
         *
         */
        void performConvertToStartSegmentAddressSectionTests();

        /**
         * @brief Perform tests on convertToExtendedLinearAddressSection() function
         *
         */
        void performConvertToExtendedLinearAddressSectionTests();

        /**
         * @brief Perform tests on convertToStartLinearAddressSection() function
         *
         */
        void performConvertToStartLinearAddressSectionTests();

        /**
         * @brief Perform tests on getAddressMap() function
         *
         */
        void performAddressMapTests();

        /**
         * @brief Perform tests on getDataMap() function
         *
         */
        void performDataMapTests();

        /**
         * @brief Perform tests on property functions
         *
         */
        void performPropertiesTests();

        /**
         * @brief Perform tests on canPushRecord() functions
         *
         */
        void performCanPushRecordTests();

        /**
         * @brief Perform tests on pushRecord() functions
         *
         */
        void performPushRecordTests();

        /**
         * @brief Perform tests on findAddress() function
         *
         */
        void performFindAddressTests();

        /**
         * @brief Perform tests on address functions
         *
         */
        void performAddressTests();

        /**
         * @brief Perform tests on checkIntersect() function
         *
         */
        void performIntersectionTests();

        /**
         * @brief Perform tests on findPreviousRecord() function
         *
         */
        void performFindPreviousRecordTests();

        /**
         * @brief Perform tests on findNextRecord() function
         *
         */
        void performFindNextRecordTests();

        /**
         * @brief Perform tests on clearData() functions
         *
         */
        void performClearDataTests();

        /**
         * @brief Perform tests on getData(address) function
         *
         */
        void performGetDataTests();

        /**
         * @brief Perform tests on getData(array) function
         *
         */
        void performGetDataArrayTests();

        /**
         * @brief Perform tests on getData(vector) function
         *
         */
        void performGetDataVectorTests();

        /**
         * @brief Perform tests on setData(address) function
         *
         */
        void performSetDataTests();

        /**
         * @brief Perform tests on setData(array) function
         *
         */
        void performSetDataArrayTests();

        /**
         * @brief Perform tests on setData(vector) function
         *
         */
        void performSetDataVectorTests();

        /**
         * @brief Perform tests on fillData() function
         *
         */
        void performFillDataTests();

        /**
         * @brief Perform tests on compact function
         *
         */
        void performCompactTests();

        /**
         * @brief Pointer to a test section
         *
         */
        HexSectionPtr section;
};

END_NAMESPACE_LIBIHEX
