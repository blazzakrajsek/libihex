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
#include <ihex_test/test_hex_group_impl.hpp>

BEGIN_NAMESPACE_LIBIHEX

TEST_F(HexGroupTest, ContructorTests)
{
    SCOPED_TRACE("ConstructorTests");
    performConstructorTests();
}

TEST_F(HexGroupTest, CanPushSectionTests)
{
    SCOPED_TRACE("CanPushSectionTests");
    performCanPushSectionTests();
}

TEST_F(HexGroupTest, PushAndGetSectionTests)
{
    SCOPED_TRACE("PushAndGetSectionTests");
    performPushAndGetSectionTests();
}

TEST_F(HexGroupTest, RemoveSectionTests)
{
    SCOPED_TRACE("RemoveSectionTests");
    performRemoveSectionTests();
}

TEST_F(HexGroupTest, GetSetUnusedDataFillValueTests)
{
    SCOPED_TRACE("GetSetUnusedDataFillValueTests");
    performGetSetUnusedDataFillValueTests();
}

TEST_F(HexGroupTest, AddressMapTests)
{
    SCOPED_TRACE("AddressMapTests");
    performAddressMapTests();
}

TEST_F(HexGroupTest, DataMapTests)
{
    SCOPED_TRACE("DataMapTests");
    performDataMapTests();
}

TEST_F(HexGroupTest, FindSectionTests)
{
    SCOPED_TRACE("FindSectionTests");
    performFindSectionTests();
}

TEST_F(HexGroupTest, FindPreviousSectionTests)
{
    SCOPED_TRACE("FindPreviousSectionTests");
    performFindPreviousSectionTests();
}

TEST_F(HexGroupTest, FindNextSectionTests)
{
    SCOPED_TRACE("FindNextSectionTests");
    performFindNextSectionTests();
}

TEST_F(HexGroupTest, CheckIntersectTests)
{
    SCOPED_TRACE("CheckIntersectTests");
    performCheckIntersectTests();
}

TEST_F(HexGroupTest, CreateSectionTests)
{
    SCOPED_TRACE("CreateSectionTests");
    performCreateSectionTests();
}

TEST_F(HexGroupTest, ClearDataTests)
{
    SCOPED_TRACE("ClearDataTests");
    performClearDataTests();
}

TEST_F(HexGroupTest, GetDataTests)
{
    SCOPED_TRACE("GetDataTests");
    performGetDataTests();
}

TEST_F(HexGroupTest, GetDataArrayTests)
{
    SCOPED_TRACE("GetDataArrayTests");
    performGetDataArrayTests();
}

TEST_F(HexGroupTest, GetDataVectorTests)
{
    SCOPED_TRACE("GetDataVectorTests");
    performGetDataVectorTests();
}

TEST_F(HexGroupTest, SetDataTests)
{
    SCOPED_TRACE("SetDataTests");
    performSetDataTests();
}

TEST_F(HexGroupTest, SetDataArrayTests)
{
    SCOPED_TRACE("SetDataArrayTests");
    performSetDataArrayTests();
}

TEST_F(HexGroupTest, SetDataVectorTests)
{
    SCOPED_TRACE("SetDataVectorTests");
    performSetDataVectorTests();
}

TEST_F(HexGroupTest, FillDataTests)
{
    SCOPED_TRACE("FillDataTests");
    performFillDataTests();
}

TEST_F(HexGroupTest, PropertiesTests)
{
    SCOPED_TRACE("PropertiesTests");
    performPropertiesTests();
}

TEST_F(HexGroupTest, StartSegmentAddressTests)
{
    SCOPED_TRACE("StartSegmentAddressTests");
    performStartSegmentAddressTests();
}

TEST_F(HexGroupTest, StartLinearAddressTests)
{
    SCOPED_TRACE("StartLinearAddressTests");
    performStartLinearAddressTests();
}

END_NAMESPACE_LIBIHEX
