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

class ReferenceAccessTest : public ::testing::Test {
 protected:
    // create a JSON value with different types
    json json_types =
    {
        {"boolean", true},
        {
            "number", {
                {"integer", 42},
                {"floating-point", 17.23}
            }
        },
        {"string", "Hello, world!"},
        {"array", {1, 2, 3, 4, 5}},
        {"null", nullptr}
    };
};


TEST_F(ReferenceAccessTest, ReferenceAccessToObjectT)
{
    using test_type = json::object_t;
    json value = {{"one", 1}, {"two", 2}};

    // check if references are returned correctly
    auto& p1 = value.get_ref<test_type&>();
    CHECK(&p1 == value.get_ptr<test_type*>());
    CHECK(p1 == value.get<test_type>());

    const auto& p2 = value.get_ref<const test_type&>();
    CHECK(&p2 == value.get_ptr<const test_type*>());
    CHECK(p2 == value.get<test_type>());

    // check if mismatching references throw correctly
    CHECK_NOTHROW(value.get_ref<json::object_t&>());
    CHECK_THROWS_WITH_AS(value.get_ref<json::array_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is object", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::string_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is object", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::boolean_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is object", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_integer_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is object", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_unsigned_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is object", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_float_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is object", json::type_error&);
}

TEST_F(ReferenceAccessTest, ConstReferenceAccessToConstObjectT)
{
    using test_type = json::object_t;
    const json value = {{"one", 1}, {"two", 2}};

    // this should not compile
    // test_type& p1 = value.get_ref<test_type&>();

    // check if references are returned correctly
    const auto& p2 = value.get_ref<const test_type&>();
    CHECK(&p2 == value.get_ptr<const test_type*>());
    CHECK(p2 == value.get<test_type>());
}

TEST_F(ReferenceAccessTest, ReferenceAccessToArray)
{
    using test_type = json::array_t;
    json value = {1, 2, 3, 4};

    // check if references are returned correctly
    auto& p1 = value.get_ref<test_type&>();
    CHECK(&p1 == value.get_ptr<test_type*>());
    CHECK(p1 == value.get<test_type>());

    const auto& p2 = value.get_ref<const test_type&>();
    CHECK(&p2 == value.get_ptr<const test_type*>());
    CHECK(p2 == value.get<test_type>());

    // check if mismatching references throw correctly
    CHECK_THROWS_WITH_AS(value.get_ref<json::object_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is array", json::type_error&);
    CHECK_NOTHROW(value.get_ref<json::array_t&>());
    CHECK_THROWS_WITH_AS(value.get_ref<json::string_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is array", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::boolean_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is array", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_integer_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is array", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_unsigned_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is array", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_float_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is array", json::type_error&);
}

TEST_F(ReferenceAccessTest, ReferenceAccessToStringT)
{
    using test_type = json::string_t;
    json value = "hello";

    // check if references are returned correctly
    auto& p1 = value.get_ref<test_type&>();
    CHECK(&p1 == value.get_ptr<test_type*>());
    CHECK(p1 == value.get<test_type>());

    const auto& p2 = value.get_ref<const test_type&>();
    CHECK(&p2 == value.get_ptr<const test_type*>());
    CHECK(p2 == value.get<test_type>());

    // check if mismatching references throw correctly
    CHECK_THROWS_WITH_AS(value.get_ref<json::object_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is string", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::array_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is string", json::type_error&);
    CHECK_NOTHROW(value.get_ref<json::string_t&>());
    CHECK_THROWS_WITH_AS(value.get_ref<json::boolean_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is string", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_integer_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is string", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_unsigned_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is string", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_float_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is string", json::type_error&);
}

TEST_F(ReferenceAccessTest, ReferenceAccessToBooleanT)
{
    using test_type = json::boolean_t;
    json value = false;

    // check if references are returned correctly
    auto& p1 = value.get_ref<test_type&>();
    CHECK(&p1 == value.get_ptr<test_type*>());
    CHECK(p1 == value.get<test_type>());

    const auto& p2 = value.get_ref<const test_type&>();
    CHECK(&p2 == value.get_ptr<const test_type*>());
    CHECK(p2 == value.get<test_type>());

    // check if mismatching references throw correctly
    CHECK_THROWS_WITH_AS(value.get_ref<json::object_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::array_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::string_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is boolean", json::type_error&);
    CHECK_NOTHROW(value.get_ref<json::boolean_t&>());
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_integer_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_unsigned_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_float_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is boolean", json::type_error&);
}

TEST_F(ReferenceAccessTest, ReferenceAccessToNumberIntegerT)
{
    using test_type = json::number_integer_t;
    json value = -23;

    // check if references are returned correctly
    auto& p1 = value.get_ref<test_type&>();
    CHECK(&p1 == value.get_ptr<test_type*>());
    CHECK(p1 == value.get<test_type>());

    const auto& p2 = value.get_ref<const test_type&>();
    CHECK(&p2 == value.get_ptr<const test_type*>());
    CHECK(p2 == value.get<test_type>());

    // check if mismatching references throw correctly
    CHECK_THROWS_WITH_AS(value.get_ref<json::object_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::array_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::string_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::boolean_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_NOTHROW(value.get_ref<json::number_integer_t&>());
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_unsigned_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_float_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
}

TEST_F(ReferenceAccessTest, ReferenceAccessToNumberUnsignedT)
{
    using test_type = json::number_unsigned_t;
    json value = 23u;

    // check if references are returned correctly
    auto& p1 = value.get_ref<test_type&>();
    CHECK(&p1 == value.get_ptr<test_type*>());
    CHECK(p1 == value.get<test_type>());

    const auto& p2 = value.get_ref<const test_type&>();
    CHECK(&p2 == value.get_ptr<const test_type*>());
    CHECK(p2 == value.get<test_type>());

    // check if mismatching references throw correctly
    CHECK_THROWS_WITH_AS(value.get_ref<json::object_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::array_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::string_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::boolean_t&>(),
                         "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    //CHECK_THROWS_WITH_AS(value.get_ref<json::number_integer_t&>(),
    //    "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_NOTHROW(value.get_ref<json::number_unsigned_t&>());
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_float_t&>(), "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
}

TEST_F(ReferenceAccessTest, ReferenceAccessToNumberFloatT)
{
    using test_type = json::number_float_t;
    json value = 42.23;

    // check if references are returned correctly
    auto& p1 = value.get_ref<test_type&>();
    CHECK(&p1 == value.get_ptr<test_type*>());
    CHECK(p1 == value.get<test_type>());

    const auto& p2 = value.get_ref<const test_type&>();
    CHECK(&p2 == value.get_ptr<const test_type*>());
    CHECK(p2 == value.get<test_type>());

    // check if mismatching references throw correctly
    CHECK_THROWS_WITH_AS(value.get_ref<json::object_t&>(), "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::array_t&>(), "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::string_t&>(), "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::boolean_t&>(), "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_integer_t&>(), "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_THROWS_WITH_AS(value.get_ref<json::number_unsigned_t&>(), "[json.exception.type_error.303] incompatible ReferenceType for get_ref, actual type is number", json::type_error&);
    CHECK_NOTHROW(value.get_ref<json::number_float_t&>());
}

