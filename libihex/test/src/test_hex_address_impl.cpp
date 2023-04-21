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

#include <stdexcept>
#include <gtest/gtest.h>
#include <ihex/hex_address.hpp>
#include <ihex_test/test_hex_address_impl.hpp>

BEGIN_NAMESPACE_LIBIHEX

HexAddressTest::HexAddressTest()
{

}

HexAddressTest::~HexAddressTest()
{

}

void HexAddressTest::SetUp()
{
    Test::SetUp();
    address = std::make_shared<HexAddress>();
}

void HexAddressTest::TearDown()
{
    address.reset();
    Test::TearDown();
}

void HexAddressTest::performConstructorTests()
{
    SCOPED_TRACE(__func__);

    // Default initialized HexAddress
    HexAddress address1{};
    ASSERT_EQ(address1.getAddressType(), HexAddressEnumType::ADDRESS_I8HEX);
    ASSERT_EQ(address1.getSegmentAddress(), 0);
    ASSERT_EQ(address1.getLinearAddress(), 0);

    // HexAddress initialized with address type and extended address
    HexAddress address2{HexAddressEnumType::ADDRESS_I8HEX, 0x1234};
    ASSERT_EQ(address2.getAddressType(), HexAddressEnumType::ADDRESS_I8HEX);
    ASSERT_EQ(address2.getSegmentAddress(), 0);
    ASSERT_EQ(address2.getLinearAddress(), 0);

    HexAddress address3{HexAddressEnumType::ADDRESS_I16HEX, 0x5678};
    ASSERT_EQ(address3.getAddressType(), HexAddressEnumType::ADDRESS_I16HEX);
    ASSERT_EQ(address3.getSegmentAddress(), 0x5678);
    ASSERT_EQ(address3.getLinearAddress(), 0);

    HexAddress address4{HexAddressEnumType::ADDRESS_I32HEX, 0x9ABC};
    ASSERT_EQ(address4.getAddressType(), HexAddressEnumType::ADDRESS_I32HEX);
    ASSERT_EQ(address4.getSegmentAddress(), 0);
    ASSERT_EQ(address4.getLinearAddress(), 0x9ABC);
}

void HexAddressTest::performPropertiesTests()
{
    SCOPED_TRACE(__func__);

    // Default values
    ASSERT_EQ(address->getAddressType(), HexAddressEnumType::ADDRESS_I8HEX);
    ASSERT_EQ(address->getSegmentAddress(), 0);
    ASSERT_EQ(address->getLinearAddress(), 0);

    // Test properties
    address->setAddressType(HexAddressEnumType::ADDRESS_I32HEX);
    ASSERT_EQ(address->getAddressType(), HexAddressEnumType::ADDRESS_I32HEX);

    address->setSegmentAddress(0xA1B2);
    ASSERT_EQ(address->getSegmentAddress(), 0xA1B2);

    address->setLinearAddress(0xC3D4);
    ASSERT_EQ(address->getLinearAddress(), 0xC3D4);

    address->setAddressType(HexAddressEnumType::ADDRESS_I16HEX);
    ASSERT_EQ(address->getAddressType(), HexAddressEnumType::ADDRESS_I16HEX);

    address->setSegmentAddress(0xE5F6);
    ASSERT_EQ(address->getSegmentAddress(), 0xE5F6);

    address->setLinearAddress(0x718);
    ASSERT_EQ(address->getLinearAddress(), 0x718);
}

void HexAddressTest::performStaticConversionFunctionTests()
{
    SCOPED_TRACE(__func__);

    // Absolute address
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0), 0);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0x1A2B), 0x1A2B);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFF), 0xFFFF);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0, 0x3C4D), 0);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0x5E6F, 0xFFFF), 0x5E6F);

    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0), 0);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0x7081), 0x7081);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFF), 0xFFFF);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0, 0x92A3), 0x92A30);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xB4C5, 0xD6E7), 0xE2335);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFF, 0xFFFF), 0xFFEF);

    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0), 0);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xF809), 0xF809);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFF), 0xFFFF);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0, 0x1A2B), 0x1A2B0000);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x3C4D, 0x5E6F), 0x5E6F3C4D);
    ASSERT_EQ(HexAddress::getAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFF, 0xFFFF), 0xFFFFFFFF);

    ASSERT_EQ(HexAddress::getAbsoluteDataRecordAddress(0), 0);
    ASSERT_EQ(HexAddress::getAbsoluteDataRecordAddress(0x7081), 0x7081);
    ASSERT_EQ(HexAddress::getAbsoluteDataRecordAddress(0xFFFF), 0xFFFF);

    ASSERT_EQ(HexAddress::getAbsoluteExtendedSegmentAddress(0xA1B2, 0), 0xA1B2);
    ASSERT_EQ(HexAddress::getAbsoluteExtendedSegmentAddress(0xA1B2, 0xC3D4), 0xCDEF2);
    ASSERT_EQ(HexAddress::getAbsoluteExtendedSegmentAddress(0x10, 0xFFFF), 0);
    ASSERT_EQ(HexAddress::getAbsoluteExtendedSegmentAddress(0xFFFF, 0xFFFF), 0xFFEF);

    ASSERT_EQ(HexAddress::getAbsoluteExtendedLinearAddress(0xA1B2, 0), 0xA1B2);
    ASSERT_EQ(HexAddress::getAbsoluteExtendedLinearAddress(0xA1B2, 0xC3D4), 0xC3D4A1B2);
    ASSERT_EQ(HexAddress::getAbsoluteExtendedLinearAddress(0x10, 0xFFFF), 0xFFFF0010);
    ASSERT_EQ(HexAddress::getAbsoluteExtendedLinearAddress(0xFFFF, 0xFFFF), 0xFFFFFFFF);

    // Relative address
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I8HEX, 0), 0);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I8HEX, 0xA1B2), 0xA1B2);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFF), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I8HEX, 0x10000), std::out_of_range);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFFFFFF), std::out_of_range);

    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I8HEX, 0, 0x718), 0);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I8HEX, 0x293A, 0x3FFF), 0x293A);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFF, 0x4B5C), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I8HEX, 0x10000, 0xFFFF), std::out_of_range);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFFFFFF, 0xFFFF), std::out_of_range);

    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0), 0);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0x8F90), 0x8F90);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFF), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0x10000), std::out_of_range);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFFFFFF), std::out_of_range);

    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xE5F5F, 0xE5F6), std::out_of_range);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xE5F60, 0xE5F6), 0);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xF5F5F, 0xE5F6), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xF5F60, 0xE5F6), std::out_of_range);

    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFEF, 0xFFFF), std::out_of_range);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFF0, 0xFFFF), 0);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFEF, 0xFFFF), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFF0, 0xFFFF), std::out_of_range);

    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0), 0);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xF809), 0xF809);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFF), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x10000), std::out_of_range);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFFFFFF), std::out_of_range);

    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x1AB1FFFF, 0x1AB2), std::out_of_range);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x1AB20000, 0x1AB2), 0);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x1AB2FFFF, 0x1AB2), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x1AB30000, 0x1AB2), std::out_of_range);

    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFEFFFF, 0xFFFF), std::out_of_range);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFF0000, 0xFFFF), 0);
    ASSERT_EQ(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFFFFFF, 0xFFFF), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeAddress(HexAddressEnumType::ADDRESS_I32HEX, 0, 0xFFFF), std::out_of_range);

    ASSERT_EQ(HexAddress::getRelativeDataRecordAddress(0), 0);
    ASSERT_EQ(HexAddress::getRelativeDataRecordAddress(0x6D7F), 0x6D7F);
    ASSERT_EQ(HexAddress::getRelativeDataRecordAddress(0xFFFF), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeDataRecordAddress(0x10000), std::out_of_range);
    ASSERT_THROW(HexAddress::getRelativeDataRecordAddress(0xFFFFFFFF), std::out_of_range);

    ASSERT_EQ(HexAddress::getRelativeExtendedSegmentAddress(0, 0), 0);
    ASSERT_EQ(HexAddress::getRelativeExtendedSegmentAddress(0xA1B2, 0), 0xA1B2);
    ASSERT_EQ(HexAddress::getRelativeExtendedSegmentAddress(0xFFFF, 0), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeExtendedSegmentAddress(0x10000, 0), std::out_of_range);
    ASSERT_THROW(HexAddress::getRelativeExtendedSegmentAddress(0xFFFFFFFF, 0), std::out_of_range);

    ASSERT_THROW(HexAddress::getRelativeExtendedSegmentAddress(0xC3D3F, 0xC3D4), std::out_of_range);
    ASSERT_EQ(HexAddress::getRelativeExtendedSegmentAddress(0xC3D40, 0xC3D4), 0);
    ASSERT_EQ(HexAddress::getRelativeExtendedSegmentAddress(0xCDEF2, 0xC3D4), 0xA1B2);
    ASSERT_EQ(HexAddress::getRelativeExtendedSegmentAddress(0xD3D3F, 0xC3D4), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeExtendedSegmentAddress(0xD3D40, 0xC3D4), std::out_of_range);

    ASSERT_THROW(HexAddress::getRelativeExtendedSegmentAddress(0xFFFEF, 0xFFFF), std::out_of_range);
    ASSERT_EQ(HexAddress::getRelativeExtendedSegmentAddress(0xFFFF0, 0xFFFF), 0);
    ASSERT_EQ(HexAddress::getRelativeExtendedSegmentAddress(0xA1A2, 0xFFFF), 0xA1B2);
    ASSERT_EQ(HexAddress::getRelativeExtendedSegmentAddress(0xFFEF, 0xFFFF), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeExtendedSegmentAddress(0xFFF0, 0xFFFF), std::out_of_range);

    ASSERT_EQ(HexAddress::getRelativeExtendedLinearAddress(0, 0), 0);
    ASSERT_EQ(HexAddress::getRelativeExtendedLinearAddress(0xC3D4, 0), 0xC3D4);
    ASSERT_EQ(HexAddress::getRelativeExtendedLinearAddress(0xFFFF, 0), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeExtendedLinearAddress(0x10000, 0), std::out_of_range);
    ASSERT_THROW(HexAddress::getRelativeExtendedLinearAddress(0xFFFFFFFF, 0), std::out_of_range);

    ASSERT_THROW(HexAddress::getRelativeExtendedLinearAddress(0xE5F5FFFF, 0xE5F6), std::out_of_range);
    ASSERT_EQ(HexAddress::getRelativeExtendedLinearAddress(0xE5F60000, 0xE5F6), 0);
    ASSERT_EQ(HexAddress::getRelativeExtendedLinearAddress(0xE5F61122, 0xE5F6), 0x01122);
    ASSERT_EQ(HexAddress::getRelativeExtendedLinearAddress(0xE5F6FFFF, 0xE5F6), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeExtendedLinearAddress(0xE5F70000, 0xE5F6), std::out_of_range);

    ASSERT_THROW(HexAddress::getRelativeExtendedLinearAddress(0xFFFEFFFF, 0xFFFF), std::out_of_range);
    ASSERT_EQ(HexAddress::getRelativeExtendedLinearAddress(0xFFFF0000, 0xFFFF), 0);
    ASSERT_EQ(HexAddress::getRelativeExtendedLinearAddress(0xFFFF3344, 0xFFFF), 0x03344);
    ASSERT_EQ(HexAddress::getRelativeExtendedLinearAddress(0xFFFFFFFF, 0xFFFF), 0xFFFF);
    ASSERT_THROW(HexAddress::getRelativeExtendedLinearAddress(0, 0xFFFF), std::out_of_range);

    ASSERT_FALSE(HexAddress::hasExtendedSegmentAddressWraparound(0));
    ASSERT_FALSE(HexAddress::hasExtendedSegmentAddressWraparound(0xF000));
    ASSERT_TRUE(HexAddress::hasExtendedSegmentAddressWraparound(0xF001));
    ASSERT_TRUE(HexAddress::hasExtendedSegmentAddressWraparound(0xFFFF));
}

void HexAddressTest::performConversionFunctionTests()
{
    SCOPED_TRACE(__func__);

    // Absolute address
    address->setAddressType(HexAddressEnumType::ADDRESS_I8HEX);
    address->setSegmentAddress(0);
    address->setLinearAddress(0);
    ASSERT_EQ(address->getAbsoluteAddress(0), 0);
    ASSERT_EQ(address->getAbsoluteAddress(0x1A2B), 0x1A2B);
    ASSERT_EQ(address->getAbsoluteAddress(0xFFFF), 0xFFFF);

    address->setSegmentAddress(0x1122);
    address->setLinearAddress(0x3344);
    ASSERT_EQ(address->getAbsoluteAddress(0), 0);
    ASSERT_EQ(address->getAbsoluteAddress(0x5E6F), 0x5E6F);

    address->setAddressType(HexAddressEnumType::ADDRESS_I16HEX);
    address->setSegmentAddress(0);
    ASSERT_EQ(address->getAbsoluteAddress(0), 0);
    ASSERT_EQ(address->getAbsoluteAddress(0x7081), 0x7081);
    ASSERT_EQ(address->getAbsoluteAddress(0xFFFF), 0xFFFF);

    address->setSegmentAddress(0x3344);
    ASSERT_EQ(address->getAbsoluteAddress(0), 0x33440);
    ASSERT_EQ(address->getAbsoluteAddress(0xB4C5), 0x3E905);
    ASSERT_EQ(address->getAbsoluteAddress(0xFFFF), 0x4343F);

    address->setSegmentAddress(0xF7FF);
    ASSERT_EQ(address->getAbsoluteAddress(0), 0xF7FF0);
    ASSERT_EQ(address->getAbsoluteAddress(0x800F), 0xFFFFF);
    ASSERT_EQ(address->getAbsoluteAddress(0x8010), 0);
    ASSERT_EQ(address->getAbsoluteAddress(0xFFFF), 0x7FEF);

    address->setAddressType(HexAddressEnumType::ADDRESS_I32HEX);
    address->setLinearAddress(0);
    ASSERT_EQ(address->getAbsoluteAddress(0), 0);
    ASSERT_EQ(address->getAbsoluteAddress(0xF809), 0xF809);
    ASSERT_EQ(address->getAbsoluteAddress(0xFFFF), 0xFFFF);

    address->setLinearAddress(0x5566);
    ASSERT_EQ(address->getAbsoluteAddress(0), 0x55660000);
    ASSERT_EQ(address->getAbsoluteAddress(0x3C4D), 0x55663C4D);
    ASSERT_EQ(address->getAbsoluteAddress(0xFFFF), 0x5566FFFF);

    // Absolute address (data record)
    ASSERT_EQ(address->getAbsoluteDataRecordAddress(0), 0);
    ASSERT_EQ(address->getAbsoluteDataRecordAddress(0x1A2B), 0x1A2B);
    ASSERT_EQ(address->getAbsoluteDataRecordAddress(0xFFFF), 0xFFFF);

    // Absolute address (extended segment address)
    address->setSegmentAddress(0);
    ASSERT_EQ(address->getAbsoluteExtendedSegmentAddress(0x1A2B), 0x1A2B);

    address->setSegmentAddress(0x3C4D);
    ASSERT_EQ(address->getAbsoluteExtendedSegmentAddress(0x1A2B), 0x3DEFB);

    address->setSegmentAddress(0xFCFF);
    ASSERT_EQ(address->getAbsoluteExtendedSegmentAddress(0x20), 0xFD010);
    ASSERT_EQ(address->getAbsoluteExtendedSegmentAddress(0xFFDF), 0xCFCF);

    // Absolute address (extended linear address)
    address->setLinearAddress(0);
    ASSERT_EQ(address->getAbsoluteExtendedLinearAddress(0x1A2B), 0x1A2B);

    address->setLinearAddress(0x3C4D);
    ASSERT_EQ(address->getAbsoluteExtendedLinearAddress(0x1A2B), 0x3C4D1A2B);

    address->setLinearAddress(0xFFFF);
    ASSERT_EQ(address->getAbsoluteExtendedLinearAddress(0x20), 0xFFFF0020);
    ASSERT_EQ(address->getAbsoluteExtendedLinearAddress(0xFFFF), 0xFFFFFFFF);

    // Relative address
    address->setAddressType(HexAddressEnumType::ADDRESS_I8HEX);
    address->setSegmentAddress(0);
    address->setLinearAddress(0);
    ASSERT_EQ(address->getRelativeAddress(0), 0);
    ASSERT_EQ(address->getRelativeAddress(0xA1B2), 0xA1B2);
    ASSERT_EQ(address->getRelativeAddress(0xFFFF), 0xFFFF);
    ASSERT_THROW(address->getRelativeAddress(0xC3D4E5F6), std::out_of_range);

    address->setSegmentAddress(0x7788);
    address->setLinearAddress(0x99AA);
    ASSERT_EQ(address->getRelativeAddress(0), 0);
    ASSERT_EQ(address->getRelativeAddress(0x293A), 0x293A);
    ASSERT_THROW(address->getRelativeAddress(0x4B5C6D7E), std::out_of_range);

    address->setAddressType(HexAddressEnumType::ADDRESS_I16HEX);
    address->setSegmentAddress(0);
    ASSERT_EQ(address->getRelativeAddress(0), 0);
    ASSERT_EQ(address->getRelativeAddress(0x8F90), 0x8F90);
    ASSERT_EQ(address->getRelativeAddress(0xFFFF), 0xFFFF);
    ASSERT_THROW(address->getRelativeAddress(0xA1B2C3D4), std::out_of_range);

    address->setSegmentAddress(0xBBCC);
    ASSERT_THROW(address->getRelativeAddress(0xBBCBF), std::out_of_range);
    ASSERT_EQ(address->getRelativeAddress(0xBBCC0), 0);
    ASSERT_EQ(address->getRelativeAddress(0xC0000), 0x4340);
    ASSERT_EQ(address->getRelativeAddress(0xCBCBF), 0xFFFF);
    ASSERT_THROW(address->getRelativeAddress(0xCBCC0), std::out_of_range);

    address->setAddressType(HexAddressEnumType::ADDRESS_I32HEX);
    address->setLinearAddress(0);
    ASSERT_EQ(address->getRelativeAddress(0), 0);
    ASSERT_EQ(address->getRelativeAddress(0xF809), 0xF809);
    ASSERT_EQ(address->getRelativeAddress(0xFFFF), 0xFFFF);
    ASSERT_THROW(address->getRelativeAddress(0x1A2B3C4D), std::out_of_range);

    address->setLinearAddress(0xDDEE);
    ASSERT_THROW(address->getRelativeAddress(0xDDEDFFFF), std::out_of_range);
    ASSERT_EQ(address->getRelativeAddress(0xDDEE0000), 0);
    ASSERT_EQ(address->getRelativeAddress(0xDDEE1A2B), 0x1A2B);
    ASSERT_EQ(address->getRelativeAddress(0xDDEEFFFF), 0xFFFF);
    ASSERT_THROW(address->getRelativeAddress(0xDDEF0000), std::out_of_range);

    // Relative address (data record)
    ASSERT_EQ(address->getRelativeDataRecordAddress(0), 0);
    ASSERT_EQ(address->getRelativeDataRecordAddress(0x1A2B), 0x1A2B);
    ASSERT_EQ(address->getRelativeDataRecordAddress(0xFFFF), 0xFFFF);
    ASSERT_THROW(address->getRelativeDataRecordAddress(0x3C4D5E6F), std::out_of_range);

    // Relative address (extended segment address)
    address->setSegmentAddress(0);
    ASSERT_EQ(address->getRelativeExtendedSegmentAddress(0x1A2B), 0x1A2B);

    address->setSegmentAddress(0x3C4D);
    ASSERT_EQ(address->getRelativeExtendedSegmentAddress(0x3DEFB), 0x1A2B);

    address->setSegmentAddress(0xFCFF);
    ASSERT_EQ(address->getRelativeExtendedSegmentAddress(0xFD010), 0x20);
    ASSERT_EQ(address->getRelativeExtendedSegmentAddress(0xCFCF), 0xFFDF);

    // Relative address (extended linear address)
    address->setLinearAddress(0);
    ASSERT_EQ(address->getRelativeExtendedLinearAddress(0x1A2B), 0x1A2B);

    address->setLinearAddress(0x3C4D);
    ASSERT_EQ(address->getRelativeExtendedLinearAddress(0x3C4D1A2B), 0x1A2B);

    address->setLinearAddress(0xFFFF);
    ASSERT_EQ(address->getRelativeExtendedLinearAddress(0xFFFF0020), 0x20);
    ASSERT_EQ(address->getRelativeExtendedLinearAddress(0xFFFFFFFF), 0xFFFF);
}

void HexAddressTest::performMinMaxAbsoluteAddressFunctionTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_EQ(HexAddress::getMinAbsoluteDataRecordAddress(), 0x00);
    ASSERT_EQ(HexAddress::getMinAbsoluteExtendedSegmentAddress(0x7F00), 0x7F000);
    ASSERT_EQ(HexAddress::getMinAbsoluteExtendedLinearAddress(0x7F00), 0x7F000000);

    ASSERT_EQ(HexAddress::getMaxAbsoluteDataRecordAddress(), 0xFFFF);
    ASSERT_EQ(HexAddress::getMaxAbsoluteExtendedSegmentAddress(0x7F00), 0x8EFFF);
    ASSERT_EQ(HexAddress::getMaxAbsoluteExtendedLinearAddress(0x7F00), 0x7F00FFFF);

    address->setSegmentAddress(0x7F00);
    ASSERT_EQ(address->getMinAbsoluteExtendedSegmentAddress(), 0x7F000);
    ASSERT_EQ(address->getMaxAbsoluteExtendedSegmentAddress(), 0x8EFFF);

    address->setLinearAddress(0x7F00);
    ASSERT_EQ(address->getMinAbsoluteExtendedLinearAddress(), 0x7F000000);
    ASSERT_EQ(address->getMaxAbsoluteExtendedLinearAddress(), 0x7F00FFFF);

    address->setAddressType(HexAddressEnumType::ADDRESS_I8HEX);
    ASSERT_EQ(address->getMinAbsoluteAddress(), 0x00);
    ASSERT_EQ(address->getMaxAbsoluteAddress(), 0xFFFF);

    address->setAddressType(HexAddressEnumType::ADDRESS_I16HEX);
    ASSERT_EQ(address->getMinAbsoluteAddress(), 0x7F000);
    ASSERT_EQ(address->getMaxAbsoluteAddress(), 0x8EFFF);

    address->setAddressType(HexAddressEnumType::ADDRESS_I32HEX);
    ASSERT_EQ(address->getMinAbsoluteAddress(), 0x7F000000);
    ASSERT_EQ(address->getMaxAbsoluteAddress(), 0x7F00FFFF);
}

void HexAddressTest::performFindExtendedAddressTests()
{
    SCOPED_TRACE(__func__);

    // Tests for extended segment address
    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0), 0);
    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0x7000), 0);
    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0xFFFF), 0);

    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0x10000), 0x1000);
    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0x17000), 0x1000);
    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0x1FFFF), 0x1000);

    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0x70000), 0x7000);
    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0x77000), 0x7000);
    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0x7FFFF), 0x7000);

    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0xF0000), 0xF000);
    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0xF7000), 0xF000);
    ASSERT_EQ(HexAddress::findExtendedSegmentAddress(0xFFFFF), 0xF000);

    ASSERT_THROW(HexAddress::findExtendedSegmentAddress(0x100000), std::out_of_range);
    ASSERT_THROW(HexAddress::findExtendedSegmentAddress(0xFFFFFFFF), std::out_of_range);

    // Tests for extended linear address
    ASSERT_EQ(HexAddress::findExtendedLinearAddress(0), 0);
    ASSERT_EQ(HexAddress::findExtendedLinearAddress(0x7000), 0);
    ASSERT_EQ(HexAddress::findExtendedLinearAddress(0xFFFF), 0);

    ASSERT_EQ(HexAddress::findExtendedLinearAddress(0x10000), 0x0001);
    ASSERT_EQ(HexAddress::findExtendedLinearAddress(0x27000), 0x0002);
    ASSERT_EQ(HexAddress::findExtendedLinearAddress(0x3FFFF), 0x0003);

    ASSERT_EQ(HexAddress::findExtendedLinearAddress(0x70000000), 0x7000);
    ASSERT_EQ(HexAddress::findExtendedLinearAddress(0x77770000), 0x7777);
    ASSERT_EQ(HexAddress::findExtendedLinearAddress(0x7FFF0000), 0x7FFF);
}

void HexAddressTest::performBaseAddressFunctionTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_EQ(HexAddress::getBaseSegmentAddress(0), 0);
    ASSERT_EQ(HexAddress::getBaseSegmentAddress(0x1A2B), 0x1A2B0);
    ASSERT_EQ(HexAddress::getBaseSegmentAddress(0xFFFF), 0xFFFF0);

    ASSERT_EQ(HexAddress::getBaseLinearAddress(0), 0);
    ASSERT_EQ(HexAddress::getBaseLinearAddress(0x3C4D), 0x3C4D0000);
    ASSERT_EQ(HexAddress::getBaseLinearAddress(0xFFFF), 0xFFFF0000);
}

void HexAddressTest::performPreviousNextAddressTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_EQ(HexAddress::getPreviousSegmentAddress(0), 0xF000);
    ASSERT_EQ(HexAddress::getPreviousSegmentAddress(0x0FFF), 0xFFFF);
    ASSERT_EQ(HexAddress::getPreviousSegmentAddress(0x1000), 0);
    ASSERT_EQ(HexAddress::getPreviousSegmentAddress(0x2000), 0x1000);
    ASSERT_EQ(HexAddress::getPreviousSegmentAddress(0x1A2B), 0x0A2B);
    ASSERT_EQ(HexAddress::getPreviousSegmentAddress(0xFFFF), 0xEFFF);

    ASSERT_EQ(HexAddress::getNextSegmentAddress(0), 0x1000);
    ASSERT_EQ(HexAddress::getNextSegmentAddress(0x1000), 0x2000);
    ASSERT_EQ(HexAddress::getNextSegmentAddress(0x3C4D), 0x4C4D);
    ASSERT_EQ(HexAddress::getNextSegmentAddress(0xEFFF), 0xFFFF);
    ASSERT_EQ(HexAddress::getNextSegmentAddress(0xF000), 0);
    ASSERT_EQ(HexAddress::getNextSegmentAddress(0xFFFF), 0xFFF);

    ASSERT_EQ(HexAddress::getPreviousLinearAddress(0), 0xFFFF);
    ASSERT_EQ(HexAddress::getPreviousLinearAddress(1), 0);
    ASSERT_EQ(HexAddress::getPreviousLinearAddress(0x5E6F), 0x5E6E);
    ASSERT_EQ(HexAddress::getPreviousLinearAddress(0xFFFF), 0xFFFE);

    ASSERT_EQ(HexAddress::getNextLinearAddress(0), 1);
    ASSERT_EQ(HexAddress::getNextLinearAddress(0x7081), 0x7082);
    ASSERT_EQ(HexAddress::getNextLinearAddress(0xFFFE), 0xFFFF);
    ASSERT_EQ(HexAddress::getNextLinearAddress(0xFFFF), 0);
}

void HexAddressTest::performContainsAbsoluteAddressFunctionTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_TRUE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0));
    ASSERT_TRUE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0x7FFF));
    ASSERT_TRUE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFF));
    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0x10000));
    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFFFFFF));

    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0, 0x1000));
    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFF, 0x1000));
    ASSERT_TRUE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0x10000, 0x1000));
    ASSERT_TRUE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0x1FFFF, 0x1000));
    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0x20000, 0x1000));
    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFFFFFF, 0x1000));

    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xF0FFF, 0xF100));
    ASSERT_TRUE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xF1000, 0xF100));
    ASSERT_TRUE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xF8FFF, 0xF100));
    ASSERT_TRUE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFF, 0xF100));
    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0x1000, 0xF100));
    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0x100000, 0xF100));

    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0, 0x2000));
    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x1FFFFFFF, 0x2000));
    ASSERT_TRUE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x20000000, 0x2000));
    ASSERT_TRUE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x2000FFFF, 0x2000));
    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x20010000, 0x2000));
    ASSERT_FALSE(HexAddress::containsAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFFFFFF, 0x2000));

    address->setAddressType(HexAddressEnumType::ADDRESS_I8HEX);
    ASSERT_TRUE(address->containsAbsoluteAddress(0));
    ASSERT_TRUE(address->containsAbsoluteAddress(0x7FFF));
    ASSERT_TRUE(address->containsAbsoluteAddress(0xFFFF));
    ASSERT_FALSE(address->containsAbsoluteAddress(0x10000));
    ASSERT_FALSE(address->containsAbsoluteAddress(0xFFFFFFFF));

    address->setAddressType(HexAddressEnumType::ADDRESS_I16HEX);
    address->setSegmentAddress(0x3000);
    ASSERT_FALSE(address->containsAbsoluteAddress(0));
    ASSERT_FALSE(address->containsAbsoluteAddress(0x2FFFF));
    ASSERT_TRUE(address->containsAbsoluteAddress(0x30000));
    ASSERT_TRUE(address->containsAbsoluteAddress(0x3FFFF));
    ASSERT_FALSE(address->containsAbsoluteAddress(0x40000));
    ASSERT_FALSE(address->containsAbsoluteAddress(0xFFFFFFFF));

    address->setSegmentAddress(0xF200);
    ASSERT_FALSE(address->containsAbsoluteAddress(0xF1FFF));
    ASSERT_TRUE(address->containsAbsoluteAddress(0xF2000));
    ASSERT_TRUE(address->containsAbsoluteAddress(0xF9FFF));
    ASSERT_TRUE(address->containsAbsoluteAddress(0x1FFF));
    ASSERT_FALSE(address->containsAbsoluteAddress(0x2000));
    ASSERT_FALSE(address->containsAbsoluteAddress(0x100000));

    address->setAddressType(HexAddressEnumType::ADDRESS_I32HEX);
    address->setLinearAddress(0x4000);
    ASSERT_FALSE(address->containsAbsoluteAddress(0));
    ASSERT_FALSE(address->containsAbsoluteAddress(0x3FFFFFFF));
    ASSERT_TRUE(address->containsAbsoluteAddress(0x40000000));
    ASSERT_TRUE(address->containsAbsoluteAddress(0x4000FFFF));
    ASSERT_FALSE(address->containsAbsoluteAddress(0x40010000));
    ASSERT_FALSE(address->containsAbsoluteAddress(0xFFFFFFFF));

    ASSERT_TRUE(HexAddress::containsAbsoluteDataRecordAddress(0));
    ASSERT_TRUE(HexAddress::containsAbsoluteDataRecordAddress(0x7FFF));
    ASSERT_TRUE(HexAddress::containsAbsoluteDataRecordAddress(0xFFFF));
    ASSERT_FALSE(HexAddress::containsAbsoluteDataRecordAddress(0x10000));
    ASSERT_FALSE(HexAddress::containsAbsoluteDataRecordAddress(0xFFFFFFFF));

    ASSERT_FALSE(HexAddress::containsAbsoluteSegmentAddress(0, 0x5000));
    ASSERT_FALSE(HexAddress::containsAbsoluteSegmentAddress(0x4FFFF, 0x5000));
    ASSERT_TRUE(HexAddress::containsAbsoluteSegmentAddress(0x50000, 0x5000));
    ASSERT_TRUE(HexAddress::containsAbsoluteSegmentAddress(0x5FFFF, 0x5000));
    ASSERT_FALSE(HexAddress::containsAbsoluteSegmentAddress(0x60000, 0x5000));
    ASSERT_FALSE(HexAddress::containsAbsoluteSegmentAddress(0xFFFFFFFF, 0x5000));

    ASSERT_FALSE(HexAddress::containsAbsoluteSegmentAddress(0xF2FFF, 0xF300));
    ASSERT_TRUE(HexAddress::containsAbsoluteSegmentAddress(0xF3000, 0xF300));
    ASSERT_TRUE(HexAddress::containsAbsoluteSegmentAddress(0xFAFFF, 0xF300));
    ASSERT_TRUE(HexAddress::containsAbsoluteSegmentAddress(0x2FFF, 0xF300));
    ASSERT_FALSE(HexAddress::containsAbsoluteSegmentAddress(0x3000, 0xF300));
    ASSERT_FALSE(HexAddress::containsAbsoluteSegmentAddress(0x100000, 0xF300));

    ASSERT_FALSE(HexAddress::containsAbsoluteLinearAddress(0, 0x6000));
    ASSERT_FALSE(HexAddress::containsAbsoluteLinearAddress(0x5FFFFFFF, 0x6000));
    ASSERT_TRUE(HexAddress::containsAbsoluteLinearAddress(0x60000000, 0x6000));
    ASSERT_TRUE(HexAddress::containsAbsoluteLinearAddress(0x6000FFFF, 0x6000));
    ASSERT_FALSE(HexAddress::containsAbsoluteLinearAddress(0x60010000, 0x6000));
    ASSERT_FALSE(HexAddress::containsAbsoluteLinearAddress(0xFFFFFFFF, 0x6000));
}

void HexAddressTest::performIsValidAbsoluteAddressFunctionTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_TRUE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0));
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0x7FFF));
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFF));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0x10000));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFFFFFF));

    ASSERT_TRUE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0));
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFF));
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFFF));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0x100000));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFFFFFF));

    ASSERT_TRUE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0));
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0x7FFFFFFF));
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddress(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFFFFFF));

    address->setAddressType(HexAddressEnumType::ADDRESS_I8HEX);
    ASSERT_TRUE(address->isValidAbsoluteAddress(0));
    ASSERT_TRUE(address->isValidAbsoluteAddress(0x7FFF));
    ASSERT_TRUE(address->isValidAbsoluteAddress(0xFFFF));
    ASSERT_FALSE(address->isValidAbsoluteAddress(0x10000));
    ASSERT_FALSE(address->isValidAbsoluteAddress(0xFFFFFFFF));

    address->setAddressType(HexAddressEnumType::ADDRESS_I16HEX);
    address->setSegmentAddress(0x1000);
    ASSERT_TRUE(address->isValidAbsoluteAddress(0));
    ASSERT_TRUE(address->isValidAbsoluteAddress(0xFFFF));
    ASSERT_TRUE(address->isValidAbsoluteAddress(0xFFFFF));
    ASSERT_FALSE(address->isValidAbsoluteAddress(0x100000));
    ASSERT_FALSE(address->isValidAbsoluteAddress(0xFFFFFFFF));

    address->setAddressType(HexAddressEnumType::ADDRESS_I32HEX);
    address->setLinearAddress(0x2000);
    ASSERT_TRUE(address->isValidAbsoluteAddress(0));
    ASSERT_TRUE(address->isValidAbsoluteAddress(0x7FFFFFFF));
    ASSERT_TRUE(address->isValidAbsoluteAddress(0xFFFFFFFF));

    ASSERT_TRUE(HexAddress::isValidAbsoluteDataRecordAddress(0));
    ASSERT_TRUE(HexAddress::isValidAbsoluteDataRecordAddress(0x7FFF));
    ASSERT_TRUE(HexAddress::isValidAbsoluteDataRecordAddress(0xFFFF));
    ASSERT_FALSE(HexAddress::isValidAbsoluteDataRecordAddress(0x10000));
    ASSERT_FALSE(HexAddress::isValidAbsoluteDataRecordAddress(0xFFFFFFFF));

    ASSERT_TRUE(HexAddress::isValidAbsoluteSegmentAddress(0));
    ASSERT_TRUE(HexAddress::isValidAbsoluteSegmentAddress(0xFFFF));
    ASSERT_TRUE(HexAddress::isValidAbsoluteSegmentAddress(0xFFFFF));
    ASSERT_FALSE(HexAddress::isValidAbsoluteSegmentAddress(0x100000));
    ASSERT_FALSE(HexAddress::isValidAbsoluteSegmentAddress(0xFFFFFFFF));
}

void HexAddressTest::performIsValidAbsoluteAddressRangeFunctionTests()
{
    SCOPED_TRACE(__func__);

    // isValidAbsoluteDataRecordRange(address, size)

    ASSERT_TRUE(HexAddress::isValidAbsoluteDataRecordAddressRange(0, 0x10000));
    ASSERT_FALSE(HexAddress::isValidAbsoluteDataRecordAddressRange(0, 0x10001));
    ASSERT_TRUE(HexAddress::isValidAbsoluteDataRecordAddressRange(0xFFFF, 1));
    ASSERT_FALSE(HexAddress::isValidAbsoluteDataRecordAddressRange(0xFFFF, 2));
    ASSERT_FALSE(HexAddress::isValidAbsoluteDataRecordAddressRange(0x10000, 0));
    ASSERT_FALSE(HexAddress::isValidAbsoluteDataRecordAddressRange(0x10000, 1));
    ASSERT_FALSE(HexAddress::isValidAbsoluteDataRecordAddressRange(0x1234, 0));

    // isValidAbsoluteSegmentAddressRange(address, size)

    ASSERT_TRUE(HexAddress::isValidAbsoluteSegmentAddressRange(0, 0x100000));
    ASSERT_FALSE(HexAddress::isValidAbsoluteSegmentAddressRange(0, 0x100001));
    ASSERT_TRUE(HexAddress::isValidAbsoluteSegmentAddressRange(0xFFFFF, 1));
    ASSERT_FALSE(HexAddress::isValidAbsoluteSegmentAddressRange(0xFFFFF, 2));
    ASSERT_FALSE(HexAddress::isValidAbsoluteSegmentAddressRange(0x100000, 0));
    ASSERT_FALSE(HexAddress::isValidAbsoluteSegmentAddressRange(0x100000, 1));
    ASSERT_FALSE(HexAddress::isValidAbsoluteSegmentAddressRange(0x12345, 0));

    // isValidAbsoluteLinearAddressRange(address, size)

    ASSERT_TRUE(HexAddress::isValidAbsoluteLinearAddressRange(0, 0x100000000));
    ASSERT_FALSE(HexAddress::isValidAbsoluteLinearAddressRange(0, 0x100000001));
    ASSERT_TRUE(HexAddress::isValidAbsoluteLinearAddressRange(0xFFFFFFFF, 1));
    ASSERT_FALSE(HexAddress::isValidAbsoluteLinearAddressRange(0xFFFFFFFF, 2));
    ASSERT_FALSE(HexAddress::isValidAbsoluteSegmentAddressRange(0x12345678, 0));

    // isValidAbsoluteAddressRange(type, address, size)

    // ADDRESS_I8HEX
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I8HEX, 0, 0x10000));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I8HEX, 0, 0x10001));
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFF, 1));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I8HEX, 0xFFFF, 2));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I8HEX, 0x10000, 0));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I8HEX, 0x10000, 1));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I8HEX, 0x1234, 0));

    // ADDRESS_I16HEX
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I16HEX, 0, 0x100000));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I16HEX, 0, 0x100001));
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFFF, 1));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I16HEX, 0xFFFFF, 2));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I16HEX, 0x100000, 0));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I16HEX, 0x100000, 1));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I16HEX, 0x12345, 0));

    // ADDRESS_I32HEX
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I32HEX, 0, 0x100000000));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I32HEX, 0, 0x100000001));
    ASSERT_TRUE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFFFFFF, 1));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I32HEX, 0xFFFFFFFF, 2));
    ASSERT_FALSE(HexAddress::isValidAbsoluteAddressRange(HexAddressEnumType::ADDRESS_I32HEX, 0x12345678, 0));

    // isValidAbsoluteAddressRange(address, size)

    // ADDRESS_I8HEX
    address->setAddressType(HexAddressEnumType::ADDRESS_I8HEX);
    ASSERT_TRUE(address->isValidAbsoluteAddressRange(0, 0x10000));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0, 0x10001));
    ASSERT_TRUE(address->isValidAbsoluteAddressRange(0xFFFF, 1));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0xFFFF, 2));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0x10000, 0));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0x10000, 1));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0x1234, 0));

    // ADDRESS_I16HEX
    address->setAddressType(HexAddressEnumType::ADDRESS_I16HEX);
    ASSERT_TRUE(address->isValidAbsoluteAddressRange(0, 0x100000));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0, 0x100001));
    ASSERT_TRUE(address->isValidAbsoluteAddressRange(0xFFFFF, 1));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0xFFFFF, 2));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0x100000, 0));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0x100000, 1));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0x12345, 0));

    // ADDRESS_I32HEX
    address->setAddressType(HexAddressEnumType::ADDRESS_I32HEX);
    ASSERT_TRUE(address->isValidAbsoluteAddressRange(0, 0x100000000));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0, 0x100000001));
    ASSERT_TRUE(address->isValidAbsoluteAddressRange(0xFFFFFFFF, 1));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0xFFFFFFFF, 2));
    ASSERT_FALSE(address->isValidAbsoluteAddressRange(0x12345678, 0));
}

void HexAddressTest::performIsValidRelativeAddressRangeFunctionTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_TRUE(HexAddress::isValidRelativeAddressRange(0, 0x10000));
    ASSERT_FALSE(HexAddress::isValidRelativeAddressRange(0, 0x10001));
    ASSERT_TRUE(HexAddress::isValidRelativeAddressRange(0xFFFF, 1));
    ASSERT_FALSE(HexAddress::isValidRelativeAddressRange(0xFFFF, 2));
    ASSERT_FALSE(HexAddress::isValidRelativeAddressRange(0x1234, 0));
}

void HexAddressTest::performAddressIntersectionTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_TRUE(HexAddress::checkExtendedSegmentAddressIntersect(0, 0));
    ASSERT_TRUE(HexAddress::checkExtendedSegmentAddressIntersect(0, 0xFFF));
    ASSERT_FALSE(HexAddress::checkExtendedSegmentAddressIntersect(0, 0x1000));
    ASSERT_TRUE(HexAddress::checkExtendedSegmentAddressIntersect(0x6001, 0x7000));
    ASSERT_FALSE(HexAddress::checkExtendedSegmentAddressIntersect(0x6001, 0x7001));
    ASSERT_FALSE(HexAddress::checkExtendedSegmentAddressIntersect(0xF000, 0));
    ASSERT_TRUE(HexAddress::checkExtendedSegmentAddressIntersect(0xF001, 0));
    ASSERT_FALSE(HexAddress::checkExtendedSegmentAddressIntersect(0xF001, 1));
    ASSERT_TRUE(HexAddress::checkExtendedSegmentAddressIntersect(0xFFFF, 0xFFE));
    ASSERT_FALSE(HexAddress::checkExtendedSegmentAddressIntersect(0xFFFF, 0xFFF));

    ASSERT_TRUE(HexAddress::checkExtendedLinearAddressIntersect(0, 0));
    ASSERT_FALSE(HexAddress::checkExtendedLinearAddressIntersect(0, 1));
    ASSERT_FALSE(HexAddress::checkExtendedLinearAddressIntersect(0xFFFE, 0xFFFF));
    ASSERT_TRUE(HexAddress::checkExtendedLinearAddressIntersect(0xFFFF, 0xFFFF));
}

void HexAddressTest::performAddressMapIntersectTests()
{
    SCOPED_TRACE(__func__);

    // ADDRESS_I8HEX
    HexAddressMap map1{}, map2{};
    map1 = HexAddress::getDataRecordAddressMap();
    map2 = HexAddress::getDataRecordAddressMap();
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    // ADDRESS_I16HEX
    map1 = HexAddress::getExtendedSegmentAddressMap(0x1100);
    map2 = HexAddress::getExtendedSegmentAddressMap(0x1F00);
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map1 = HexAddress::getExtendedSegmentAddressMap(0x2F00);
    map2 = HexAddress::getExtendedSegmentAddressMap(0x3F00);
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map2, map1));

    map1 = HexAddress::getExtendedSegmentAddressMap(0xF000);
    map2 = HexAddress::getExtendedSegmentAddressMap(0xFFFF);
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map1 = HexAddress::getExtendedSegmentAddressMap(0xF001);
    map2 = HexAddress::getExtendedSegmentAddressMap(0);
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map1 = HexAddress::getExtendedSegmentAddressMap(0xF001);
    map2 = HexAddress::getExtendedSegmentAddressMap(0x0001);
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map2, map1));

    // ADDRESS_I32HEX
    map1 = HexAddress::getExtendedLinearAddressMap(0);
    map2 = HexAddress::getExtendedLinearAddressMap(1);
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map2, map1));

    map1 = HexAddress::getExtendedLinearAddressMap(0x7FFF);
    map2 = HexAddress::getExtendedLinearAddressMap(0x7FFF);
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    // Test edge cases
    map1.clear();
    map1[0x70000] = 0x10000;
    map2.clear();
    map2[0x60000] = 0x8000;
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2[0x60000] = 0x10000;
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2[0x60000] = 0x10001;
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2[0x60000] = 0x20000;
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2[0x60000] = 0x20001;
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2[0x60000] = 0x30000;
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2.clear();
    map2[0x6FFFF] = 0x20000;
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2.clear();
    map2[0x70000] = 0x1FFFF;
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2.clear();
    map2[0x7FFFF] = 0x10000;
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2.clear();
    map2[0x80000] = 0xFFFF;
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2.clear();
    map2[0x87FFF] = 0x8000;
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2.erase(0x87FFF);
    map2[0x73FFF] = 0x4000;
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));

    map1.clear();
    map1[0x70000] = 0;
    map2.clear();
    map2[0x70000] = 1;
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map2, map1));

    map1.clear();
    map1[0xFFFB0000] = 0x10000;
    map1[0xFFFD0000] = 0x10000;
    map1[0xFFFF0000] = 0x10000;
    map2.clear();
    map2[0xFFFA0000] = 0x10000;
    map2[0xFFFC0000] = 0x10000;
    map2[0xFFFE0000] = 0x10000;
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_FALSE(HexAddress::checkAddressMapIntersect(map2, map1));

    map2[0xFFFFFFFF] = 1;
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map1, map2));
    ASSERT_TRUE(HexAddress::checkAddressMapIntersect(map2, map1));
}

void HexAddressTest::performAddressMapTests()
{
    SCOPED_TRACE(__func__);

    // Initialize
    HexAddressMap map{};

    ASSERT_EQ(HexAddress::getSectionMaxDataSize(), 0x10000);

    // ADDRESS_I8HEX
    map = HexAddress::getDataRecordAddressMap();
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    address->setAddressType(HexAddressEnumType::ADDRESS_I8HEX);
    map = address->getAddressMap();
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    // ADDRESS_I16HEX
    map = HexAddress::getExtendedSegmentAddressMap(0x1234);
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0x12340);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    map = HexAddress::getExtendedSegmentAddressMap(0xF000);
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0xF0000);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    map = HexAddress::getExtendedSegmentAddressMap(0xF001);
    ASSERT_EQ(map.size(), 2);
    ASSERT_EQ(map.cbegin()->first, 0);
    ASSERT_EQ(map.cbegin()->second, 0x10);
    ASSERT_EQ(std::next(map.cbegin(), 1)->first, 0xF0010);
    ASSERT_EQ(std::next(map.cbegin(), 1)->second, 0xFFF0);

    map = HexAddress::getExtendedSegmentAddressMap(0xF7FF);
    ASSERT_EQ(map.size(), 2);
    ASSERT_EQ(map.cbegin()->first, 0);
    ASSERT_EQ(map.cbegin()->second, 0x7FF0);
    ASSERT_EQ(std::next(map.cbegin(), 1)->first, 0xF7FF0);
    ASSERT_EQ(std::next(map.cbegin(), 1)->second, 0x8010);

    address->setAddressType(HexAddressEnumType::ADDRESS_I16HEX);
    address->setSegmentAddress(0x2345);
    map = address->getAddressMap();
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0x23450);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    address->setSegmentAddress(0xE000);
    map = address->getAddressMap();
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0xE0000);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    address->setSegmentAddress(0xF020);
    map = address->getAddressMap();
    ASSERT_EQ(map.size(), 2);
    ASSERT_EQ(map.cbegin()->first, 0);
    ASSERT_EQ(map.cbegin()->second, 0x200);
    ASSERT_EQ(std::next(map.cbegin(), 1)->first, 0xF0200);
    ASSERT_EQ(std::next(map.cbegin(), 1)->second, 0xFE00);

    address->setSegmentAddress(0xF8FF);
    map = address->getAddressMap();
    ASSERT_EQ(map.size(), 2);
    ASSERT_EQ(map.cbegin()->first, 0);
    ASSERT_EQ(map.cbegin()->second, 0x8FF0);
    ASSERT_EQ(std::next(map.cbegin(), 1)->first, 0xF8FF0);
    ASSERT_EQ(std::next(map.cbegin(), 1)->second, 0x7010);

    // ADDRESS_I32HEX
    map = HexAddress::getExtendedLinearAddressMap(0x1234);
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0x12340000);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    map = HexAddress::getExtendedLinearAddressMap(0xF000);
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0xF0000000);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    address->setAddressType(HexAddressEnumType::ADDRESS_I32HEX);
    address->setLinearAddress(0x2345);
    map = address->getAddressMap();
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0x23450000);
    ASSERT_EQ(map.cbegin()->second, 0x10000);

    address->setLinearAddress(0xFFFE);
    map = address->getAddressMap();
    ASSERT_EQ(map.size(), 1);
    ASSERT_EQ(map.cbegin()->first, 0xFFFE0000);
    ASSERT_EQ(map.cbegin()->second, 0x10000);
}

void HexAddressTest::performCompactAddressMapTests()
{
    SCOPED_TRACE(__func__);

    HexAddressMap map{};
    HexAddress::compactAddressMap(map);
    ASSERT_EQ(map.size(), 0);

    map[0] = 0x10;
    map[0x10] = 0x20;
    map[0x30] = 0xF;
    map[0x1000] = 0x1000;
    map[0x2000] = 0x2000;
    map[0x4000] = 0xFFF;
    map[0x5000] = 0x1000;
    map[0x5001] = 0x1000;
    map[0xFFFF0000] = 0x1;
    map[0xFFFF0001] = 0xFFF;
    map[0xFFFF1000] = 0xEFFF;
    HexAddress::compactAddressMap(map);
    ASSERT_EQ(map.size(), 5);
    ASSERT_EQ(map.begin()->first, 0);
    ASSERT_EQ(map.begin()->second, 0x3F);
    ASSERT_EQ(std::next(map.begin(), 1)->first, 0x1000);
    ASSERT_EQ(std::next(map.begin(), 1)->second, 0x3FFF);
    ASSERT_EQ(std::next(map.begin(), 2)->first, 0x5000);
    ASSERT_EQ(std::next(map.begin(), 2)->second, 0x1000);
    ASSERT_EQ(std::next(map.begin(), 3)->first, 0x5001);
    ASSERT_EQ(std::next(map.begin(), 3)->second, 0x1000);
    ASSERT_EQ(std::next(map.begin(), 4)->first, 0xFFFF0000);
    ASSERT_EQ(std::next(map.begin(), 4)->second, 0xFFFF);
}

END_NAMESPACE_LIBIHEX
