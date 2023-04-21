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
#include <ihex/hex_group.hpp>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

/**
 * @brief HexGroupTest class
 *
 */
class HexGroupTest : public testing::Test
{
    public:
        /**
         * @brief Construct a new HexGroupTest object
         *
         */
        HexGroupTest();

        /**
         * @brief Destroy the HexGroupTest object
         *
         */
        virtual ~HexGroupTest();

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
         * @brief Perform contructor tests
         *
         */
        void performConstructorTests();

        /**
         * @brief Perform tests on canPushRecord() function
         *
         */
        void performCanPushSectionTests();

        /**
         * @brief Perform tests on pushSection() and getSection() functions
         *
         */
        void performPushAndGetSectionTests();

        /**
         * @brief Perform tests on removeSection() functions
         *
         */
        void performRemoveSectionTests();

        /**
         * @brief Perform tests on get/setUnusedDataFillValue() functions
         *
         */
        void performGetSetUnusedDataFillValueTests();

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
         * @brief Perform tests on findSection() functions
         *
         */
        void performFindSectionTests();

        /**
         * @brief Perform tests on findPreviousSection() function
         *
         */
        void performFindPreviousSectionTests();

        /**
         * @brief Perform tests on findNextSection() function
         *
         */
        void performFindNextSectionTests();

        /**
         * @brief Perform tests on checkIntersect() functions
         *
         */
        void performCheckIntersectTests();

        /**
         * @brief Perform tests on createSection() functions
         *
         */
        void performCreateSectionTests();

        /**
         * @brief Perform tests on clearData() functions
         *
         */
        void performClearDataTests();

        /**
         * @brief Perform tests on array index operator
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

        /*
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
         * @brief Perform tests on properties and helper functions
         *
         */
        void performPropertiesTests();

        /**
         * @brief Perform tests on start segment address related functions
         *
         */
        void performStartSegmentAddressTests();

        /**
         * @brief Perform tests on start linear address related functions
         *
         */
        void performStartLinearAddressTests();

        /**
         * @brief Pointer to a test group
         *
         */
        HexGroupPtr group;

        /**
         * @brief Example SECTION_DATA section
         *
         */
        const HexSection dataSection{":08001000A1B2C3D4E5F6071804"};

        /**
         * @brief Example SECTION_END_OF_FILE section
         *
         */
        const HexSection endOfFileSection{":00000001FF"};

        /**
         * @brief First example of SECTION_EXTENDED_SEGMENT_ADDRESS section
         *
         */
        const HexSection extendedSegmentAddressSection1{":020000020100FB"};

        /**
         * @brief Second example of SECTION_EXTENDED_SEGMENT_ADDRESS section
         *
         */
        const HexSection extendedSegmentAddressSection2{":020000021100EB"};

        /**
         * @brief Example SECTION_START_SEGMENT_ADDRESS section
         *
         */
        const HexSection startSegmentAddressSection{":040000031021324353"};

        /**
         * @brief First example of SECTION_EXTENDED_LINEAR_ADDRESS section
         *
         */
        const HexSection extendedLinearAddressSection1{":020000040200F8"};

        /**
         * @brief Second example of SECTION_EXTENDED_LINEAR_ADDRESS section
         *
         */
        const HexSection extendedLinearAddressSection2{":020000040201F7"};

        /**
         * @brief Example SECTION_START_LINEAR_ADDRESS section
         *
         */
        const HexSection startLinearAddressSection{":040000055465768741"};

        /**
         * @brief First example of unused data fill value
         *
         */
        const HexDataType unusedDataFillValue1{0x5A};

        /**
         * @brief Second example of unused data fill value
         *
         */
        const HexDataType unusedDataFillValue2{0xA5};
};

END_NAMESPACE_LIBIHEX
