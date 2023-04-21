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
#include <ihex_test/test_hex_record_impl.hpp>

BEGIN_NAMESPACE_LIBIHEX

TEST_F(HexRecordTest, ConstructorTests)
{
    SCOPED_TRACE("ConstructorTests");
    performConstructorTests();
}

TEST_F(HexRecordTest, MakeRecordTests)
{
    SCOPED_TRACE("MakeRecordTests");
    performMakeRecordTests();
}

TEST_F(HexRecordTest, PrepareRecordTests)
{
    SCOPED_TRACE("PrepareRecordTests");
    performPrepareRecordTests();
}

TEST_F(HexRecordTest, PrepareDataRecordTests)
{
    SCOPED_TRACE("PrepareDataRecordTests");
    performPrepareDataRecordTests();
}

TEST_F(HexRecordTest, PrepareEndOfFileRecordTests)
{
    SCOPED_TRACE("PrepareEndOfFileRecordTests");
    performPrepareEndOfFileRecordTests();
}

TEST_F(HexRecordTest, PrepareExtendedSegmentAddressRecordTests)
{
    SCOPED_TRACE("PrepareExtendedSegmentAddressTests");
    performPrepareExtendedSegmentAddressRecordTests();
}

TEST_F(HexRecordTest, PrepareStartSegmentAddressRecordTests)
{
    SCOPED_TRACE("PrepareStartSegmentAddressTests");
    performPrepareStartSegmentAddressRecordTests();
}

TEST_F(HexRecordTest, PrepareExtendedLinearAddressRecordTests)
{
    SCOPED_TRACE("PrepareExtendedLinearAddressTests");
    performPrepareExtendedLinearAddressRecordTests();
}

TEST_F(HexRecordTest, PrepareStartLinearAddressRecordTests)
{
    SCOPED_TRACE("PrepareStartLinearAddressTests");
    performPrepareStartLinearAddressRecordTests();
}

TEST_F(HexRecordTest, GetSetRecordPropertiesTests)
{
    SCOPED_TRACE("GetSetRecordPropertiesTests");
    performGetSetRecordPropertiesTests();
}

TEST_F(HexRecordTest, GetDataPropertiesTests)
{
    SCOPED_TRACE("GetDataPropertiesTests");
    performGetDataPropertiesTests();
}

TEST_F(HexRecordTest, SetDataPropertiesTests)
{
    SCOPED_TRACE("SetDataPropertiesTests");
    performSetDataPropertiesTests();
}

TEST_F(HexRecordTest, SetRecordTests)
{
    SCOPED_TRACE("SetRecordTests");
    performSetRecordTests();
}

TEST_F(HexRecordTest, ChecksumTests)
{
    SCOPED_TRACE("ChecksumTests");
    performChecksumTests();
}

TEST_F(HexRecordTest, HasAddressFunctionTests)
{
    SCOPED_TRACE("HasAddressFunctionTests");
    performHasAddressFunctionTests();
}

TEST_F(HexRecordTest, IsValidRecordTests)
{
    SCOPED_TRACE("IsValidRecordTests");
    performIsValidRecordTests();
}

TEST_F(HexRecordTest, ExtendedSegmentAddressFunctionTests)
{
    SCOPED_TRACE("ExtendedSegmentAddressFunctionTests");
    performExtendedSegmentAddressFunctionTests();
}

TEST_F(HexRecordTest, StartSegmentAddressFunctionTests)
{
    SCOPED_TRACE("StartSegmentAddressFunctionTests");
    performStartSegmentAddressFunctionTests();
}

TEST_F(HexRecordTest, ExtendedLinearAddressFunctionTests)
{
    SCOPED_TRACE("ExtendedLinearAddressFunctionTests");
    performExtendedLinearAddressFunctionTests();
}

TEST_F(HexRecordTest, StartLinearAddressFunctionTests)
{
    SCOPED_TRACE("StartLinearAddressFunctionTests");
    performStartLinearAddressFunctionTests();
}

END_NAMESPACE_LIBIHEX
