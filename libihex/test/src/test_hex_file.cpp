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

#include <gtest/gtest.h>
#include <ihex_test/test_hex_file_impl.hpp>

BEGIN_NAMESPACE_LIBIHEX

TEST_F(HexFileTest, ConstructorTests)
{
    SCOPED_TRACE("ConstructorTests");
    performConstructorTests();
}

TEST_F(HexFileTest, LoadStreamTests)
{
    SCOPED_TRACE("LoadStreamTests");
    performLoadStreamTests();
}

TEST_F(HexFileTest, SaveStreamTests)
{
    SCOPED_TRACE("SaveStreamTests");
    performSaveStreamTests();
}

TEST_F(HexFileTest, LoadSaveFileTests)
{
    SCOPED_TRACE("LoadSaveFileTests");
    performLoadSaveFileTests();
}

TEST_F(HexFileTest, PropertiesTests)
{
    SCOPED_TRACE("PropertiesTests");
    performPropertiesTests();
}

END_NAMESPACE_LIBIHEX
