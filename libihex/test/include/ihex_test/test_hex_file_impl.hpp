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
#include <ihex/hex_file.hpp>
#include <ihex/version.hpp>

BEGIN_NAMESPACE_LIBIHEX

/**
 * @brief HexFileTest class
 *
 */
class HexFileTest : public testing::Test
{
    public:
        /**
         * @brief Construct a new HexFileTest object
         *
         */
        HexFileTest();

        /**
         * @brief Destroy the HexFileTest object
         *
         */
        virtual ~HexFileTest();

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
         * @brief Perform tests on load(std::istream) function
         *
         */
        void performLoadStreamTests();

        /**
         * @brief Perform tests on save(std::ostream) function
         *
         */
        void performSaveStreamTests();

        /**
         * @brief Perform load and save file function tests
         *
         */
        void performLoadSaveFileTests();

        /**
         * @brief Perform tests on property functions
         *
         */
        void performPropertiesTests();

        /**
         * @brief Pointer to a test file
         *
         */
        HexFilePtr file;
};

END_NAMESPACE_LIBIHEX
