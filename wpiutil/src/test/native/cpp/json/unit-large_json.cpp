//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

#include "unit-json.h"
using wpi::json;

#include <algorithm>




TEST(TestsOnVeryLargeJSONsTest, Issue1419SegmentationFaultStackOverflowDueToUnboundedRecursion)
{
    const auto depth = 5000000;

    std::string s(static_cast<std::size_t>(2 * depth), '[');
    std::fill(s.begin() + depth, s.end(), ']');

    json _;
    CHECK_NOTHROW(_ = wpi::json::parse(s));
}

