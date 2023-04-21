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
#include <ihex_test/test_hex_section_impl.hpp>

BEGIN_NAMESPACE_LIBIHEX

TEST_F(HexSectionTest, ConstructorTests)
{
    SCOPED_TRACE("ConstructorTests");
    performContructorTests();
}

TEST_F(HexSectionTest, MakeSectionTests)
{
    SCOPED_TRACE("MakeSectionTests");
    performMakeSectionTests();
}

TEST_F(HexSectionTest, ConvertToDataSectionTests)
{
    SCOPED_TRACE("ConvertToDataSectionTests");
    performConvertToDataSectionTests();
}

TEST_F(HexSectionTest, ConvertToEndOfFileSectionTests)
{
    SCOPED_TRACE("ConvertToEndOfFileSectionTests");
    performConvertToEndOfFileSectionTests();
}

TEST_F(HexSectionTest, ConvertToExtendedSegmentAddressSectionTests)
{
    SCOPED_TRACE("ConvertToExtendedSegmentAddressSectionTests");
    performConvertToExtendedSegmentAddressSectionTests();
}

TEST_F(HexSectionTest, ConvertToStartSegmentAddressSectionTests)
{
    SCOPED_TRACE("ConvertToStartSegmentAddressSectionTests");
    performConvertToStartSegmentAddressSectionTests();
}

TEST_F(HexSectionTest, ConvertToExtendedLinearAddressSectionTests)
{
    SCOPED_TRACE("ConvertToExtendedLinearAddressSectionTests");
    performConvertToExtendedLinearAddressSectionTests();
}

TEST_F(HexSectionTest, ConvertStartLinearAddressSectionTests)
{
    SCOPED_TRACE("ConvertToStartLinearAddressSectionTests");
    performConvertToStartLinearAddressSectionTests();
}

TEST_F(HexSectionTest, AddressMapTests)
{
    SCOPED_TRACE("AddressMapTests");
    performAddressMapTests();
}

TEST_F(HexSectionTest, DataMapTests)
{
    SCOPED_TRACE("DataMapTests");
    performDataMapTests();
}

TEST_F(HexSectionTest, PropertiesTests)
{
    SCOPED_TRACE("PropertiesTests");
    performPropertiesTests();
}

TEST_F(HexSectionTest, CanPushRecordTests)
{
    SCOPED_TRACE("CanPushRecordTests");
    performCanPushRecordTests();
}

TEST_F(HexSectionTest, PushRecordTests)
{
    SCOPED_TRACE("PushRecordTests");
    performPushRecordTests();
}

TEST_F(HexSectionTest, FindAddressTests)
{
    SCOPED_TRACE("FindAddressTests");
    performFindAddressTests();
}

TEST_F(HexSectionTest, AddressTests)
{
    SCOPED_TRACE("AddressTests");
    performAddressTests();
}

TEST_F(HexSectionTest, IntersectionTests)
{
    SCOPED_TRACE("IntersectionTests");
    performIntersectionTests();
}

TEST_F(HexSectionTest, FindPreviousRecordTests)
{
    SCOPED_TRACE("FindPreviousRecordTests");
    performFindPreviousRecordTests();
}

TEST_F(HexSectionTest, FindNextRecordTests)
{
    SCOPED_TRACE("FindNextRecordTests");
    performFindNextRecordTests();
}

TEST_F(HexSectionTest, ClearDataTests)
{
    SCOPED_TRACE("ClearDataTests");
    performClearDataTests();
}

TEST_F(HexSectionTest, GetDataTests)
{
    SCOPED_TRACE("GetDataTests");
    performGetDataTests();
}

TEST_F(HexSectionTest, GetDataArrayTests)
{
    SCOPED_TRACE("GetDataArrayTests");
    performGetDataArrayTests();
}

TEST_F(HexSectionTest, GetDataVectorTests)
{
    SCOPED_TRACE("GetDataVectorTests");
    performGetDataVectorTests();
}

TEST_F(HexSectionTest, SetDataTests)
{
    SCOPED_TRACE("SetDataTests");
    performSetDataTests();
}

TEST_F(HexSectionTest, SetDataArrayTests)
{
    SCOPED_TRACE("SetDataArrayTests");
    performSetDataArrayTests();
}

TEST_F(HexSectionTest, SetDataVectorTests)
{
    SCOPED_TRACE("SetDataVectorTests");
    performSetDataVectorTests();
}

TEST_F(HexSectionTest, FillDataTests)
{
    SCOPED_TRACE("FillDataTests");
    performFillDataTests();
}

TEST_F(HexSectionTest, CompactTests)
{
    SCOPED_TRACE("CompactTests");
    performCompactTests();
}

END_NAMESPACE_LIBIHEX
