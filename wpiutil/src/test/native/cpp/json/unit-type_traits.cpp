//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

#if JSON_TEST_USING_MULTIPLE_HEADERS
    #include "wpi/detail/meta/type_traits.h"
#else
    #include "unit-json.h"
#endif

using wpi::detail::is_c_string;
using wpi::detail::is_c_string_uncvref;







TEST(TypeTraitsIsCStringTest, CharPointer)
{
    CHECK(is_c_string<char*>::value);
    CHECK(is_c_string<const char*>::value);
    CHECK(is_c_string<char* const>::value);
    CHECK(is_c_string<const char* const>::value);

    CHECK_FALSE(is_c_string<char*&>::value);
    CHECK_FALSE(is_c_string<const char*&>::value);
    CHECK_FALSE(is_c_string<char* const&>::value);
    CHECK_FALSE(is_c_string<const char* const&>::value);

    CHECK(is_c_string_uncvref<char*&>::value);
    CHECK(is_c_string_uncvref<const char*&>::value);
    CHECK(is_c_string_uncvref<char* const&>::value);
    CHECK(is_c_string_uncvref<const char* const&>::value);
}

TEST(TypeTraitsIsCStringTest, Char)
{
    // NOLINTBEGIN(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    CHECK(is_c_string<char[]>::value);
    CHECK(is_c_string<const char[]>::value);

    CHECK_FALSE(is_c_string<char(&)[]>::value);
    CHECK_FALSE(is_c_string<const char(&)[]>::value);

    CHECK(is_c_string_uncvref<char(&)[]>::value);
    CHECK(is_c_string_uncvref<const char(&)[]>::value);
    // NOLINTEND(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
}

