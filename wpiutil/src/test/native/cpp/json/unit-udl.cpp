//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

#include "unit-json.h"

class UserDefinedStringLiteralsTest : public ::testing::Test {
 protected:
    UserDefinedStringLiteralsTest() : j_expected(wpi::json::parse(R"({"foo": "bar", "baz": 42})")), ptr_expected(wpi::json::json_pointer("/foo/bar")){}
    wpi::json j_expected;
    wpi::json::json_pointer ptr_expected;
};


TEST_F(UserDefinedStringLiteralsTest, UsingNamespaceNlohmannLiteralsJsonLiterals)
{
    using namespace wpi::literals::json_literals; // NOLINT(google-build-using-namespace)

    CHECK(R"({"foo": "bar", "baz": 42})"_json == j_expected);
    CHECK("/foo/bar"_json_pointer == ptr_expected);
}

TEST_F(UserDefinedStringLiteralsTest, UsingNamespaceNlohmannJsonLiterals)
{
    using namespace wpi::json_literals; // NOLINT(google-build-using-namespace)

    CHECK(R"({"foo": "bar", "baz": 42})"_json == j_expected);
    CHECK("/foo/bar"_json_pointer == ptr_expected);
}

TEST_F(UserDefinedStringLiteralsTest, UsingNamespaceNlohmannLiterals)
{
    using namespace wpi::literals; // NOLINT(google-build-using-namespace)

    CHECK(R"({"foo": "bar", "baz": 42})"_json == j_expected);
    CHECK("/foo/bar"_json_pointer == ptr_expected);
}

TEST_F(UserDefinedStringLiteralsTest, UsingNamespaceNlohmann)
{
    using namespace wpi; // NOLINT(google-build-using-namespace)

    CHECK(R"({"foo": "bar", "baz": 42})"_json == j_expected);
    CHECK("/foo/bar"_json_pointer == ptr_expected);
}

#ifndef JSON_TEST_NO_GLOBAL_UDLS

TEST_F(UserDefinedStringLiteralsTest, GlobalNamespace)
{
    CHECK(R"({"foo": "bar", "baz": 42})"_json == j_expected);
    CHECK("/foo/bar"_json_pointer == ptr_expected);
}

#endif

