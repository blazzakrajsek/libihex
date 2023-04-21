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
#include <ihex_test/test_hex_address_impl.hpp>

BEGIN_NAMESPACE_LIBIHEX

TEST_F(HexAddressTest, ConstructorTests)
{
    SCOPED_TRACE("ConstructorTests");
    performConstructorTests();
}

TEST_F(HexAddressTest, PropertiesTests)
{
    SCOPED_TRACE("PropertiesTests");
    performPropertiesTests();
}

TEST_F(HexAddressTest, StaticConversionFunctionTests)
{
    SCOPED_TRACE("StaticConversionFunctionTests");
    performStaticConversionFunctionTests();
}

TEST_F(HexAddressTest, ConversionFunctionTests)
{
    SCOPED_TRACE("ConversionFunctionTests");
    performConversionFunctionTests();
}

TEST_F(HexAddressTest, MinMaxAbsoluteAddressFunctionTests)
{
    SCOPED_TRACE("MinMaxAbsoluteAddressFunctionTests");
    performMinMaxAbsoluteAddressFunctionTests();
}

TEST_F(HexAddressTest, FindExtendedAddressTests)
{
    SCOPED_TRACE("FindExtendedAddressTests");
    performFindExtendedAddressTests();
}

TEST_F(HexAddressTest, BaseAddressFunctionTests)
{
    SCOPED_TRACE("BaseAddressFunctionTests");
    performBaseAddressFunctionTests();
}

TEST_F(HexAddressTest, PreviousNextAddressTests)
{
    SCOPED_TRACE("PreviousNextAddressTests");
    performPreviousNextAddressTests();
}

TEST_F(HexAddressTest, ContainsAbsoluteAddressFunctionTests)
{
    SCOPED_TRACE("ContainsAbsoluteAddressFunctionTests");
    performContainsAbsoluteAddressFunctionTests();
}

TEST_F(HexAddressTest, IsValidAbsoluteAddressFunctionTests)
{
    SCOPED_TRACE("IsValidAbsoluteAddressFunctionTests");
    performIsValidAbsoluteAddressFunctionTests();
}

TEST_F(HexAddressTest, IsValidAbsoluteAddressRangeFunctionTests)
{
    SCOPED_TRACE("IsValidAbsoluteAddressRangeFunctionTests");
    performIsValidAbsoluteAddressRangeFunctionTests();
}

TEST_F(HexAddressTest, IsValidRelativeAddressRangeFunctionTests)
{
    SCOPED_TRACE("IsValidRelativeAddressRangeFunctionTests");
    performIsValidRelativeAddressRangeFunctionTests();
}

TEST_F(HexAddressTest, AddressIntersectionTests)
{
    SCOPED_TRACE("AddressIntersectionTests");
    performAddressIntersectionTests();
}

TEST_F(HexAddressTest, AddressMapIntersectTests)
{
    SCOPED_TRACE("AddressMapIntersectTests");
    performAddressMapIntersectTests();
}

TEST_F(HexAddressTest, AddressMapTests)
{
    SCOPED_TRACE("AddressMapTests");
    performAddressMapTests();
}

TEST_F(HexAddressTest, CompactAddressMapTests)
{
    SCOPED_TRACE("CompactAddressMapTests");
    performCompactAddressMapTests();
}

END_NAMESPACE_LIBIHEX
