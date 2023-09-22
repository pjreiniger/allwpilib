//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

// avoid warning when assert does not abort

/// global variable to record side effect of assert calls
static int assert_counter;

/// set failure variable to true instead of calling assert(x)
#define JSON_ASSERT(x) {if (!(x)) ++assert_counter; }

#include "unit-json.h"
using wpi::json;

// the test assumes exceptions to work
#if !defined(JSON_NOEXCEPTION)



TEST(JSONASSERTXTest, BasicJsonFirstSecond)
{
    assert_counter = 0;
    CHECK(assert_counter == 0);

    json::iterator it;
    json j;

    // in case assertions do not abort execution, an exception is thrown
    CHECK_THROWS_WITH_AS(json(it, j.end()), "[json.exception.invalid_iterator.201] iterators are not compatible", json::invalid_iterator);

    // check that assertion actually happened
    CHECK(assert_counter == 1);
}
#endif


