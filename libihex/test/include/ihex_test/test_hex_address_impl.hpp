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
#include <ihex/hex_address.hpp>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

/**
 * @brief HexAddressTest class
 *
 */
class HexAddressTest : public testing::Test
{
    public:
        /**
         * @brief Construct a new HexAddressTest object
         *
         */
        HexAddressTest();

        /**
         * @brief Destroy the HexAddressTest object
         *
         */
        virtual ~HexAddressTest();

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
         * @brief Perform tests on constructor functions
         *
         */
        void performConstructorTests();

        /**
         * @brief Perform tests on property functions
         *
         */
        void performPropertiesTests();

        /**
         * @brief Perform tests on static conversion functions
         *
         */
        void performStaticConversionFunctionTests();

        /**
         * @brief Perform tests on conversion functions
         *
         */
        void performConversionFunctionTests();

        /**
         * @brief Perform tests on min/max absolute address functions
         *
         */
        void performMinMaxAbsoluteAddressFunctionTests();

        /**
         * @brief Perform tests on extended address functions
         *
         */
        void performFindExtendedAddressTests();

        /**
         * @brief Perform tests on base address functions
         *
         */
        void performBaseAddressFunctionTests();

        /**
         * @brief Perform tests on previous/next address functions
         *
         */
        void performPreviousNextAddressTests();

        /**
         * @brief Perform tests on containsAbsoluteAddress() functions
         *
         */
        void performContainsAbsoluteAddressFunctionTests();

        /**
         * @brief Perform tests on isValidAbsoluteAddress() functions
         *
         */
        void performIsValidAbsoluteAddressFunctionTests();

        /**
         * @brief Perform tests on isValidAbsoluteAddressRange() functions
         *
         */
        void performIsValidAbsoluteAddressRangeFunctionTests();

        /**
         * @brief Perform tests on isValidRelativeAddressRange() function
         *
         */
        void performIsValidRelativeAddressRangeFunctionTests();

        /**
         * @brief Perform tests on intersection functions
         *
         */
        void performAddressIntersectionTests();

        /**
         * @brief Perform tests on checkAddressMapIntersect() function
         *
         */
        void performAddressMapIntersectTests();

        /**
         * @brief Perform tests on address map functions
         *
         */
        void performAddressMapTests();

        /**
         * @brief Perform tests on address map compact functions
         *
         */
        void performCompactAddressMapTests();

        /**
         * @brief Pointer to a test address
         *
         */
        HexAddressPtr address;
};

END_NAMESPACE_LIBIHEX
