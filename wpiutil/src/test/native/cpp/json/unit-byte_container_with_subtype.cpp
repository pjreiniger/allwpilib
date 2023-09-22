//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// Copyright (c) 2013-2022 Niels Lohmann <http://nlohmann.me>.
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

#include "unit-json.h"
using wpi::json;

class ByteContainerWithSubtypeTest : public ::testing::Test {
 protected:
    using subtype_type = wpi::byte_container_with_subtype<std::vector<std::uint8_t>>::subtype_type;
};


TEST_F(ByteContainerWithSubtypeTest, EmptyContainer)
{
    wpi::byte_container_with_subtype<std::vector<std::uint8_t>> container;

    CHECK(!container.has_subtype());
    CHECK(container.subtype() == static_cast<subtype_type>(-1));

    container.clear_subtype();
    CHECK(!container.has_subtype());
    CHECK(container.subtype() == static_cast<subtype_type>(-1));

    container.set_subtype(42);
    CHECK(container.has_subtype());
    CHECK(container.subtype() == 42);
}

TEST_F(ByteContainerWithSubtypeTest, SubtypedContainer)
{
    wpi::byte_container_with_subtype<std::vector<std::uint8_t>> container({}, 42);
    CHECK(container.has_subtype());
    CHECK(container.subtype() == 42);

    container.clear_subtype();
    CHECK(!container.has_subtype());
    CHECK(container.subtype() == static_cast<subtype_type>(-1));
}

TEST_F(ByteContainerWithSubtypeTest, Comparisons)
{
    std::vector<std::uint8_t> bytes = {{0xCA, 0xFE, 0xBA, 0xBE}};
    wpi::byte_container_with_subtype<std::vector<std::uint8_t>> container1;
    wpi::byte_container_with_subtype<std::vector<std::uint8_t>> container2({}, 42);
    wpi::byte_container_with_subtype<std::vector<std::uint8_t>> container3(bytes);
    wpi::byte_container_with_subtype<std::vector<std::uint8_t>> container4(bytes, 42);

    CHECK(container1 == container1);
    CHECK(container1 != container2);
    CHECK(container1 != container3);
    CHECK(container1 != container4);
    CHECK(container2 != container1);
    CHECK(container2 == container2);
    CHECK(container2 != container3);
    CHECK(container2 != container4);
    CHECK(container3 != container1);
    CHECK(container3 != container2);
    CHECK(container3 == container3);
    CHECK(container3 != container4);
    CHECK(container4 != container1);
    CHECK(container4 != container2);
    CHECK(container4 != container3);
    CHECK(container4 == container4);

    container3.clear();
    container4.clear();

    CHECK(container1 == container3);
    CHECK(container2 == container4);
}

