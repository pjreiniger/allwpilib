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




TEST(PointerAccessTest, PointerAccessToObjectT)
{
    using test_type = json::object_t;
    json value = {{"one", 1}, {"two", 2}};

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<json::object_t*>() != nullptr);
    CHECK(value.get_ptr<json::array_t*>() == nullptr);
    CHECK(value.get_ptr<json::string_t*>() == nullptr);
    CHECK(value.get_ptr<json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToConstObjectT)
{
    using test_type = const json::object_t;
    const json value = {{"one", 1}, {"two", 2}};

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<const json::object_t*>() != nullptr);
    CHECK(value.get_ptr<const json::array_t*>() == nullptr);
    CHECK(value.get_ptr<const json::string_t*>() == nullptr);
    CHECK(value.get_ptr<const json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<const json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToArray)
{
    using test_type = json::array_t;
    json value = {1, 2, 3, 4};

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<json::object_t*>() == nullptr);
    CHECK(value.get_ptr<json::array_t*>() != nullptr);
    CHECK(value.get_ptr<json::string_t*>() == nullptr);
    CHECK(value.get_ptr<json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToConstArray)
{
    using test_type = const json::array_t;
    const json value = {1, 2, 3, 4};

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<const json::object_t*>() == nullptr);
    CHECK(value.get_ptr<const json::array_t*>() != nullptr);
    CHECK(value.get_ptr<const json::string_t*>() == nullptr);
    CHECK(value.get_ptr<const json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<const json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToStringT)
{
    using test_type = json::string_t;
    json value = "hello";

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<json::object_t*>() == nullptr);
    CHECK(value.get_ptr<json::array_t*>() == nullptr);
    CHECK(value.get_ptr<json::string_t*>() != nullptr);
    CHECK(value.get_ptr<json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToConstStringT)
{
    using test_type = const json::string_t;
    const json value = "hello";

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<const json::object_t*>() == nullptr);
    CHECK(value.get_ptr<const json::array_t*>() == nullptr);
    CHECK(value.get_ptr<const json::string_t*>() != nullptr);
    CHECK(value.get_ptr<const json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<const json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToBooleanT)
{
    using test_type = json::boolean_t;
    json value = false;

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<json::object_t*>() == nullptr);
    CHECK(value.get_ptr<json::array_t*>() == nullptr);
    CHECK(value.get_ptr<json::string_t*>() == nullptr);
    CHECK(value.get_ptr<json::boolean_t*>() != nullptr);
    CHECK(value.get_ptr<json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToConstBooleanT)
{
    using test_type = const json::boolean_t;
    const json value = false;

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    //CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<const json::object_t*>() == nullptr);
    CHECK(value.get_ptr<const json::array_t*>() == nullptr);
    CHECK(value.get_ptr<const json::string_t*>() == nullptr);
    CHECK(value.get_ptr<const json::boolean_t*>() != nullptr);
    CHECK(value.get_ptr<const json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<const json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToNumberIntegerT)
{
    using test_type = json::number_integer_t;
    json value = 23;

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<json::object_t*>() == nullptr);
    CHECK(value.get_ptr<json::array_t*>() == nullptr);
    CHECK(value.get_ptr<json::string_t*>() == nullptr);
    CHECK(value.get_ptr<json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_integer_t*>() != nullptr);
    CHECK(value.get_ptr<json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToConstNumberIntegerT)
{
    using test_type = const json::number_integer_t;
    const json value = 23;

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<const json::object_t*>() == nullptr);
    CHECK(value.get_ptr<const json::array_t*>() == nullptr);
    CHECK(value.get_ptr<const json::string_t*>() == nullptr);
    CHECK(value.get_ptr<const json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_integer_t*>() != nullptr);
    CHECK(value.get_ptr<const json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<const json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToNumberUnsignedT)
{
    using test_type = json::number_unsigned_t;
    json value = 23u;

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<json::object_t*>() == nullptr);
    CHECK(value.get_ptr<json::array_t*>() == nullptr);
    CHECK(value.get_ptr<json::string_t*>() == nullptr);
    CHECK(value.get_ptr<json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_integer_t*>() != nullptr);
    CHECK(value.get_ptr<json::number_unsigned_t*>() != nullptr);
    CHECK(value.get_ptr<json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToConstNumberUnsignedT)
{
    using test_type = const json::number_unsigned_t;
    const json value = 23u;

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<const json::object_t*>() == nullptr);
    CHECK(value.get_ptr<const json::array_t*>() == nullptr);
    CHECK(value.get_ptr<const json::string_t*>() == nullptr);
    CHECK(value.get_ptr<const json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_integer_t*>() != nullptr);
    CHECK(value.get_ptr<const json::number_unsigned_t*>() != nullptr);
    CHECK(value.get_ptr<const json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<const json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToNumberFloatT)
{
    using test_type = json::number_float_t;
    json value = 42.23;

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    EXPECT_NEAR(*p1, value.get<test_type>(), 0.001);

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    EXPECT_NEAR(*p2, value.get<test_type>(), 0.001);

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    EXPECT_NEAR(*p3, value.get<test_type>(), 0.001);

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<json::object_t*>() == nullptr);
    CHECK(value.get_ptr<json::array_t*>() == nullptr);
    CHECK(value.get_ptr<json::string_t*>() == nullptr);
    CHECK(value.get_ptr<json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<json::number_float_t*>() != nullptr);
    CHECK(value.get_ptr<json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToConstNumberFloatT)
{
    using test_type = const json::number_float_t;
    const json value = 42.23;

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    EXPECT_NEAR(*p1, value.get<test_type>(), 0.001);

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    EXPECT_NEAR(*p2, value.get<test_type>(), 0.001);

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    EXPECT_NEAR(*p3, value.get<test_type>(), 0.001);

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<const json::object_t*>() == nullptr);
    CHECK(value.get_ptr<const json::array_t*>() == nullptr);
    CHECK(value.get_ptr<const json::string_t*>() == nullptr);
    CHECK(value.get_ptr<const json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_float_t*>() != nullptr);
    CHECK(value.get_ptr<const json::binary_t*>() == nullptr);
}

TEST(PointerAccessTest, PointerAccessToConstBinaryT)
{
    using test_type = const json::binary_t;
    const json value = json::binary({1, 2, 3});

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<const json::object_t*>() == nullptr);
    CHECK(value.get_ptr<const json::array_t*>() == nullptr);
    CHECK(value.get_ptr<const json::string_t*>() == nullptr);
    CHECK(value.get_ptr<const json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<const json::binary_t*>() != nullptr);
}

TEST(PointerAccessTest, PointerAccessToConstBinaryT2)
{
    using test_type = const json::binary_t;
    const json value = json::binary({});

    // check if pointers are returned correctly
    test_type* p1 = value.get_ptr<test_type*>();
    CHECK(p1 == value.get_ptr<test_type*>());
    CHECK(*p1 == value.get<test_type>());

    const test_type* p2 = value.get_ptr<const test_type*>();
    CHECK(p2 == value.get_ptr<const test_type*>());
    CHECK(*p2 == value.get<test_type>());

    const test_type* const p3 = value.get_ptr<const test_type* const>();
    CHECK(p3 == value.get_ptr<const test_type* const>());
    CHECK(*p3 == value.get<test_type>());

    // check if null pointers are returned correctly
    CHECK(value.get_ptr<const json::object_t*>() == nullptr);
    CHECK(value.get_ptr<const json::array_t*>() == nullptr);
    CHECK(value.get_ptr<const json::string_t*>() == nullptr);
    CHECK(value.get_ptr<const json::boolean_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_integer_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_unsigned_t*>() == nullptr);
    CHECK(value.get_ptr<const json::number_float_t*>() == nullptr);
    CHECK(value.get_ptr<const json::binary_t*>() != nullptr);
}

