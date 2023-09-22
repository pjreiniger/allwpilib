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
#ifdef JSON_TEST_NO_GLOBAL_UDLS
    using namespace wpi::literals; // NOLINT(google-build-using-namespace)
#endif

// build test with C++14
// JSON_HAS_CPP_14

using Json = wpi::json;


class ElementAccess2ObjectTest : public ::testing::Test {
 protected:
    Json j = {{"integer", 1}, {"unsigned", 1u}, {"floating", 42.23}, {"null", nullptr}, {"string", "hello world"}, {"boolean", true}, {"object", Json::object()}, {"array", {1, 2, 3}}};
    const Json j_const = j;
};


class ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingTest, AccessWithinBounds)
{
    CHECK(j.at("integer") == Json(1));
    CHECK(j.at("unsigned") == Json(1u));
    CHECK(j.at("boolean") == Json(true));
    CHECK(j.at("null") == Json(nullptr));
    CHECK(j.at("string") == Json("hello world"));
    CHECK(j.at("floating") == Json(42.23));
    CHECK(j.at("object") == Json::object());
    CHECK(j.at("array") == Json({1, 2, 3}));

    CHECK(j_const.at("integer") == Json(1));
    CHECK(j_const.at("unsigned") == Json(1u));
    CHECK(j_const.at("boolean") == Json(true));
    CHECK(j_const.at("null") == Json(nullptr));
    CHECK(j_const.at("string") == Json("hello world"));
    CHECK(j_const.at("floating") == Json(42.23));
    CHECK(j_const.at("object") == Json::object());
    CHECK(j_const.at("array") == Json({1, 2, 3}));

#ifdef JSON_HAS_CPP_17
    CHECK(j.at(std::string_view("integer")) == Json(1));
    CHECK(j.at(std::string_view("unsigned")) == Json(1u));
    CHECK(j.at(std::string_view("boolean")) == Json(true));
    CHECK(j.at(std::string_view("null")) == Json(nullptr));
    CHECK(j.at(std::string_view("string")) == Json("hello world"));
    CHECK(j.at(std::string_view("floating")) == Json(42.23));
    CHECK(j.at(std::string_view("object")) == Json::object());
    CHECK(j.at(std::string_view("array")) == Json({1, 2, 3}));

    CHECK(j_const.at(std::string_view("integer")) == Json(1));
    CHECK(j_const.at(std::string_view("unsigned")) == Json(1u));
    CHECK(j_const.at(std::string_view("boolean")) == Json(true));
    CHECK(j_const.at(std::string_view("null")) == Json(nullptr));
    CHECK(j_const.at(std::string_view("string")) == Json("hello world"));
    CHECK(j_const.at(std::string_view("floating")) == Json(42.23));
    CHECK(j_const.at(std::string_view("object")) == Json::object());
    CHECK(j_const.at(std::string_view("array")) == Json({1, 2, 3}));
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingTest, AccessOutsideBounds)
{
    CHECK_THROWS_WITH_AS(j.at("foo"), "[json.exception.out_of_range.403] key 'foo' not found", typename Json::out_of_range&);
    CHECK_THROWS_WITH_AS(j_const.at("foo"), "[json.exception.out_of_range.403] key 'foo' not found", typename Json::out_of_range&);


#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j.at(std::string_view("foo")), "[json.exception.out_of_range.403] key 'foo' not found", typename Json::out_of_range&);
    CHECK_THROWS_WITH_AS(j_const.at(std::string_view("foo")), "[json.exception.out_of_range.403] key 'foo' not found", typename Json::out_of_range&);
#endif
}

class ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingAccessOnNonObjectTypeTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingAccessOnNonObjectTypeTest, Null)
{
    Json j_nonobject(Json::value_t::null);
    const Json j_nonobject_const(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject.at("foo"), "[json.exception.type_error.304] cannot use at() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at("foo"), "[json.exception.type_error.304] cannot use at() with null", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.at(std::string_view(std::string_view("foo"))), "[json.exception.type_error.304] cannot use at() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at(std::string_view(std::string_view("foo"))), "[json.exception.type_error.304] cannot use at() with null", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingAccessOnNonObjectTypeTest, Boolean)
{
    Json j_nonobject(Json::value_t::boolean);
    const Json j_nonobject_const(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject.at("foo"), "[json.exception.type_error.304] cannot use at() with boolean", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at("foo"), "[json.exception.type_error.304] cannot use at() with boolean", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with boolean", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with boolean", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingAccessOnNonObjectTypeTest, String)
{
    Json j_nonobject(Json::value_t::string);
    const Json j_nonobject_const(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject.at("foo"), "[json.exception.type_error.304] cannot use at() with string", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at("foo"), "[json.exception.type_error.304] cannot use at() with string", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with string", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with string", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingAccessOnNonObjectTypeTest, Array)
{
    Json j_nonobject(Json::value_t::array);
    const Json j_nonobject_const(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject.at("foo"), "[json.exception.type_error.304] cannot use at() with array", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at("foo"), "[json.exception.type_error.304] cannot use at() with array", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with array", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with array", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingAccessOnNonObjectTypeTest, NumberInteger)
{
    Json j_nonobject(Json::value_t::number_integer);
    const Json j_nonobject_const(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject.at("foo"), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at("foo"), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingAccessOnNonObjectTypeTest, NumberUnsigned)
{
    Json j_nonobject(Json::value_t::number_unsigned);
    const Json j_nonobject_const(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject.at("foo"), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at("foo"), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithBoundsCheckingAccessOnNonObjectTypeTest, NumberFloatingPoint)
{
    Json j_nonobject(Json::value_t::number_float);
    const Json j_nonobject_const(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject.at("foo"), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at("foo"), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.at(std::string_view("foo")), "[json.exception.type_error.304] cannot use at() with number", typename Json::type_error&);
#endif
}

class ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueTest : public ElementAccess2ObjectTest{};


class ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyTest, AccessExistingValue)
{
    CHECK(j.value("integer", 2) == 1);
    EXPECT_NEAR(j.value("integer", 1.0), 1, 0.001);
    CHECK(j.value("unsigned", 2) == 1u);
    EXPECT_NEAR(j.value("unsigned", 1.0), 1u, 0.001);
    CHECK(j.value("null", Json(1)) == Json());
    CHECK(j.value("boolean", false) == true);
    CHECK(j.value("string", "bar") == "hello world");
    CHECK(j.value("string", std::string("bar")) == "hello world");
    EXPECT_NEAR(j.value("floating", 12.34), 42.23, 0.001);
    CHECK(j.value("floating", 12) == 42);
    CHECK(j.value("object", Json({{"foo", "bar"}})) == Json::object());
    CHECK(j.value("array", Json({10, 100})) == Json({1, 2, 3}));

    CHECK(j_const.value("integer", 2) == 1);
    EXPECT_NEAR(j_const.value("integer", 1.0), 1, 0.001);
    CHECK(j_const.value("unsigned", 2) == 1u);
    EXPECT_NEAR(j_const.value("unsigned", 1.0), 1u, 0.001);
    CHECK(j_const.value("boolean", false) == true);
    CHECK(j_const.value("string", "bar") == "hello world");
    CHECK(j_const.value("string", std::string("bar")) == "hello world");
    EXPECT_NEAR(j_const.value("floating", 12.34), 42.23, 0.001);
    CHECK(j_const.value("floating", 12) == 42);
    CHECK(j_const.value("object", Json({{"foo", "bar"}})) == Json::object());
    CHECK(j_const.value("array", Json({10, 100})) == Json({1, 2, 3}));

#ifdef JSON_HAS_CPP_17
    CHECK(j.value(std::string_view("integer"), 2) == 1);
    EXPECT_NEAR(j.value(std::string_view("integer"), 1.0), 1, 0.001);
    CHECK(j.value(std::string_view("unsigned"), 2) == 1u);
    EXPECT_NEAR(j.value(std::string_view("unsigned"), 1.0), 1u, 0.001);
    CHECK(j.value(std::string_view("null"), Json(1)) == Json());
    CHECK(j.value(std::string_view("boolean"), false) == true);
    CHECK(j.value(std::string_view("string"), "bar") == "hello world");
    CHECK(j.value(std::string_view("string"), std::string("bar")) == "hello world");
    EXPECT_NEAR(j.value(std::string_view("floating"), 12.34), 42.23, 0.001);
    CHECK(j.value(std::string_view("floating"), 12) == 42);
    CHECK(j.value(std::string_view("object"), Json({{"foo", "bar"}})) == Json::object());
    CHECK(j.value(std::string_view("array"), Json({10, 100})) == Json({1, 2, 3}));

    CHECK(j_const.value(std::string_view("integer"), 2) == 1);
    EXPECT_NEAR(j_const.value(std::string_view("integer"), 1.0), 1, 0.001);
    CHECK(j_const.value(std::string_view("unsigned"), 2) == 1u);
    EXPECT_NEAR(j_const.value(std::string_view("unsigned"), 1.0), 1u, 0.001);
    CHECK(j_const.value(std::string_view("boolean"), false) == true);
    CHECK(j_const.value(std::string_view("string"), "bar") == "hello world");
    CHECK(j_const.value(std::string_view("string"), std::string("bar")) == "hello world");
    EXPECT_NEAR(j_const.value(std::string_view("floating"), 12.34), 42.23, 0.001);
    CHECK(j_const.value(std::string_view("floating"), 12) == 42);
    CHECK(j_const.value(std::string_view("object"), Json({{"foo", "bar"}})) == Json::object());
    CHECK(j_const.value(std::string_view("array"), Json({10, 100})) == Json({1, 2, 3}));
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyTest, AccessNonExistingValue)
{
    CHECK(j.value("_", 2) == 2);
    CHECK(j.value("_", 2u) == 2u);
    CHECK(j.value("_", false) == false);
    CHECK(j.value("_", "bar") == "bar");
    EXPECT_NEAR(j.value("_", 12.34), 12.34, 0.001);
    CHECK(j.value("_", Json({{"foo", "bar"}})) == Json({{"foo", "bar"}}));
    CHECK(j.value("_", Json({10, 100})) == Json({10, 100}));

    CHECK(j_const.value("_", 2) == 2);
    CHECK(j_const.value("_", 2u) == 2u);
    CHECK(j_const.value("_", false) == false);
    CHECK(j_const.value("_", "bar") == "bar");
    EXPECT_NEAR(j_const.value("_", 12.34), 12.34, 0.001);
    CHECK(j_const.value("_", Json({{"foo", "bar"}})) == Json({{"foo", "bar"}}));
    CHECK(j_const.value("_", Json({10, 100})) == Json({10, 100}));

#ifdef JSON_HAS_CPP_17
    CHECK(j.value(std::string_view("_"), 2) == 2);
    CHECK(j.value(std::string_view("_"), 2u) == 2u);
    CHECK(j.value(std::string_view("_"), false) == false);
    CHECK(j.value(std::string_view("_"), "bar") == "bar");
    EXPECT_NEAR(j.value(std::string_view("_"), 12.34), 12.34, 0.001);
    CHECK(j.value(std::string_view("_"), Json({{"foo", "bar"}})) == Json({{"foo", "bar"}}));
    CHECK(j.value(std::string_view("_"), Json({10, 100})) == Json({10, 100}));

    CHECK(j_const.value(std::string_view("_"), 2) == 2);
    CHECK(j_const.value(std::string_view("_"), 2u) == 2u);
    CHECK(j_const.value(std::string_view("_"), false) == false);
    CHECK(j_const.value(std::string_view("_"), "bar") == "bar");
    EXPECT_NEAR(j_const.value(std::string_view("_"), 12.34), 12.34, 0.001);
    CHECK(j_const.value(std::string_view("_"), Json({{"foo", "bar"}})) == Json({{"foo", "bar"}}));
    CHECK(j_const.value(std::string_view("_"), Json({10, 100})) == Json({10, 100}));
#endif
}

class ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyAccessOnNonObjectTypeTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyAccessOnNonObjectTypeTest, Null)
{
    Json j_nonobject(Json::value_t::null);
    const Json j_nonobject_const(Json::value_t::null);
    CHECK_THROWS_WITH_AS(j_nonobject.value("foo", 1), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("foo", 1), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyAccessOnNonObjectTypeTest, Boolean)
{
    Json j_nonobject(Json::value_t::boolean);
    const Json j_nonobject_const(Json::value_t::boolean);
    CHECK_THROWS_WITH_AS(j_nonobject.value("foo", 1), "[json.exception.type_error.306] cannot use value() with boolean", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("foo", 1), "[json.exception.type_error.306] cannot use value() with boolean", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with boolean", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with boolean", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyAccessOnNonObjectTypeTest, String)
{
    Json j_nonobject(Json::value_t::string);
    const Json j_nonobject_const(Json::value_t::string);
    CHECK_THROWS_WITH_AS(j_nonobject.value("foo", 1), "[json.exception.type_error.306] cannot use value() with string", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("foo", 1), "[json.exception.type_error.306] cannot use value() with string", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with string", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with string", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyAccessOnNonObjectTypeTest, Array)
{
    Json j_nonobject(Json::value_t::array);
    const Json j_nonobject_const(Json::value_t::array);
    CHECK_THROWS_WITH_AS(j_nonobject.value("foo", 1), "[json.exception.type_error.306] cannot use value() with array", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("foo", 1), "[json.exception.type_error.306] cannot use value() with array", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with array", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with array", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyAccessOnNonObjectTypeTest, NumberInteger)
{
    Json j_nonobject(Json::value_t::number_integer);
    const Json j_nonobject_const(Json::value_t::number_integer);
    CHECK_THROWS_WITH_AS(j_nonobject.value("foo", 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("foo", 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyAccessOnNonObjectTypeTest, NumberUnsigned)
{
    Json j_nonobject(Json::value_t::number_unsigned);
    const Json j_nonobject_const(Json::value_t::number_unsigned);
    CHECK_THROWS_WITH_AS(j_nonobject.value("foo", 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("foo", 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAKeyAccessOnNonObjectTypeTest, NumberFloatingPoint)
{
    Json j_nonobject(Json::value_t::number_float);
    const Json j_nonobject_const(Json::value_t::number_float);
    CHECK_THROWS_WITH_AS(j_nonobject.value("foo", 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("foo", 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value(std::string_view("foo"), 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
#endif
}

class ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerTest, AccessExistingValue)
{
    CHECK(j.value("/integer"_json_pointer, 2) == 1);
    EXPECT_NEAR(j.value("/integer"_json_pointer, 1.0), 1, 0.001);
    CHECK(j.value("/unsigned"_json_pointer, 2) == 1u);
    EXPECT_NEAR(j.value("/unsigned"_json_pointer, 1.0), 1u, 0.001);
    CHECK(j.value("/null"_json_pointer, Json(1)) == Json());
    CHECK(j.value("/boolean"_json_pointer, false) == true);
    CHECK(j.value("/string"_json_pointer, "bar") == "hello world");
    CHECK(j.value("/string"_json_pointer, std::string("bar")) == "hello world");
    EXPECT_NEAR(j.value("/floating"_json_pointer, 12.34), 42.23, 0.001);
    CHECK(j.value("/floating"_json_pointer, 12) == 42);
    CHECK(j.value("/object"_json_pointer, Json({{"foo", "bar"}})) == Json::object());
    CHECK(j.value("/array"_json_pointer, Json({10, 100})) == Json({1, 2, 3}));

    CHECK(j_const.value("/integer"_json_pointer, 2) == 1);
    EXPECT_NEAR(j_const.value("/integer"_json_pointer, 1.0), 1, 0.001);
    CHECK(j_const.value("/unsigned"_json_pointer, 2) == 1u);
    EXPECT_NEAR(j_const.value("/unsigned"_json_pointer, 1.0), 1u, 0.001);
    CHECK(j_const.value("/boolean"_json_pointer, false) == true);
    CHECK(j_const.value("/string"_json_pointer, "bar") == "hello world");
    CHECK(j_const.value("/string"_json_pointer, std::string("bar")) == "hello world");
    EXPECT_NEAR(j_const.value("/floating"_json_pointer, 12.34), 42.23, 0.001);
    CHECK(j_const.value("/floating"_json_pointer, 12) == 42);
    CHECK(j_const.value("/object"_json_pointer, Json({{"foo", "bar"}})) == Json::object());
    CHECK(j_const.value("/array"_json_pointer, Json({10, 100})) == Json({1, 2, 3}));
}

class ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerAccessOnNonObjectTypeTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerAccessOnNonObjectTypeTest, Null)
{
    Json j_nonobject(Json::value_t::null);
    const Json j_nonobject_const(Json::value_t::null);
    CHECK_THROWS_WITH_AS(j_nonobject.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerAccessOnNonObjectTypeTest, Boolean)
{
    Json j_nonobject(Json::value_t::boolean);
    const Json j_nonobject_const(Json::value_t::boolean);
    CHECK_THROWS_WITH_AS(j_nonobject.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with boolean", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with boolean", typename Json::type_error&);
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerAccessOnNonObjectTypeTest, String)
{
    Json j_nonobject(Json::value_t::string);
    const Json j_nonobject_const(Json::value_t::string);
    CHECK_THROWS_WITH_AS(j_nonobject.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with string", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with string", typename Json::type_error&);
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerAccessOnNonObjectTypeTest, Array)
{
    Json j_nonobject(Json::value_t::array);
    const Json j_nonobject_const(Json::value_t::array);
    CHECK_THROWS_WITH_AS(j_nonobject.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with array", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with array", typename Json::type_error&);
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerAccessOnNonObjectTypeTest, NumberInteger)
{
    Json j_nonobject(Json::value_t::number_integer);
    const Json j_nonobject_const(Json::value_t::number_integer);
    CHECK_THROWS_WITH_AS(j_nonobject.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerAccessOnNonObjectTypeTest, NumberUnsigned)
{
    Json j_nonobject(Json::value_t::number_unsigned);
    const Json j_nonobject_const(Json::value_t::number_unsigned);
    CHECK_THROWS_WITH_AS(j_nonobject.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerAccessOnNonObjectTypeTest, NumberFloatingPoint)
{
    Json j_nonobject(Json::value_t::number_float);
    const Json j_nonobject_const(Json::value_t::number_float);
    CHECK_THROWS_WITH_AS(j_nonobject.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject_const.value("/foo"_json_pointer, 1), "[json.exception.type_error.306] cannot use value() with number", typename Json::type_error&);
}

TEST_F(ElementAccess2ObjectTest, NonConstOperator)
{
    {
        Json j_null;
        CHECK(j_null.is_null());
        j_null["key"] = 1;
        CHECK(j_null.is_object());
        CHECK(j_null.size() == 1);
        j_null["key"] = 2;
        CHECK(j_null.size() == 1);
    }
#ifdef JSON_HAS_CPP_17
    {
        std::string_view key = "key";
        Json j_null;
        CHECK(j_null.is_null());
        j_null[key] = 1;
        CHECK(j_null.is_object());
        CHECK(j_null.size() == 1);
        j_null[key] = 2;
        CHECK(j_null.size() == 1);
    }
#endif
}

TEST_F(ElementAccess2ObjectTest, FrontAndBack)
{
    if (std::is_same<Json, wpi::ordered_json>::value)
    {
        // "integer" is the first key
        CHECK(j.front() == Json(1));
        CHECK(j_const.front() == Json(1));
        // "array" is last key
        CHECK(j.back() == Json({1, 2, 3}));
        CHECK(j_const.back() == Json({1, 2, 3}));
    }
    else
    {
        // "array" is the smallest key
        CHECK(j.front() == Json({1, 2, 3}));
        CHECK(j_const.front() == Json({1, 2, 3}));
        // "unsigned" is the largest key
        CHECK(j.back() == Json(1u));
        CHECK(j_const.back() == Json(1u));
    }
}

class ElementAccess2ObjectAccessSpecifiedElementTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectAccessSpecifiedElementTest, AccessWithinBounds)
{
    CHECK(j["integer"] == Json(1));
    CHECK(j[typename Json::object_t::key_type("integer")] == j["integer"]);

    CHECK(j["unsigned"] == Json(1u));
    CHECK(j[typename Json::object_t::key_type("unsigned")] == j["unsigned"]);

    CHECK(j["boolean"] == Json(true));
    CHECK(j[typename Json::object_t::key_type("boolean")] == j["boolean"]);

    CHECK(j["null"] == Json(nullptr));
    CHECK(j[typename Json::object_t::key_type("null")] == j["null"]);

    CHECK(j["string"] == Json("hello world"));
    CHECK(j[typename Json::object_t::key_type("string")] == j["string"]);

    CHECK(j["floating"] == Json(42.23));
    CHECK(j[typename Json::object_t::key_type("floating")] == j["floating"]);

    CHECK(j["object"] == Json::object());
    CHECK(j[typename Json::object_t::key_type("object")] == j["object"]);

    CHECK(j["array"] == Json({1, 2, 3}));
    CHECK(j[typename Json::object_t::key_type("array")] == j["array"]);

    CHECK(j_const["integer"] == Json(1));
    CHECK(j_const[typename Json::object_t::key_type("integer")] == j["integer"]);

    CHECK(j_const["boolean"] == Json(true));
    CHECK(j_const[typename Json::object_t::key_type("boolean")] == j["boolean"]);

    CHECK(j_const["null"] == Json(nullptr));
    CHECK(j_const[typename Json::object_t::key_type("null")] == j["null"]);

    CHECK(j_const["string"] == Json("hello world"));
    CHECK(j_const[typename Json::object_t::key_type("string")] == j["string"]);

    CHECK(j_const["floating"] == Json(42.23));
    CHECK(j_const[typename Json::object_t::key_type("floating")] == j["floating"]);

    CHECK(j_const["object"] == Json::object());
    CHECK(j_const[typename Json::object_t::key_type("object")] == j["object"]);

    CHECK(j_const["array"] == Json({1, 2, 3}));
    CHECK(j_const[typename Json::object_t::key_type("array")] == j["array"]);
}

#ifdef JSON_HAS_CPP_17

TEST_F(ElementAccess2ObjectAccessSpecifiedElementTest, AccessWithinBoundsStringView)
{
    CHECK(j["integer"] == Json(1));
    CHECK(j[std::string_view("integer")] == j["integer"]);

    CHECK(j["unsigned"] == Json(1u));
    CHECK(j[std::string_view("unsigned")] == j["unsigned"]);

    CHECK(j["boolean"] == Json(true));
    CHECK(j[std::string_view("boolean")] == j["boolean"]);

    CHECK(j["null"] == Json(nullptr));
    CHECK(j[std::string_view("null")] == j["null"]);

    CHECK(j["string"] == Json("hello world"));
    CHECK(j[std::string_view("string")] == j["string"]);

    CHECK(j["floating"] == Json(42.23));
    CHECK(j[std::string_view("floating")] == j["floating"]);

    CHECK(j["object"] == Json::object());
    CHECK(j[std::string_view("object")] == j["object"]);

    CHECK(j["array"] == Json({1, 2, 3}));
    CHECK(j[std::string_view("array")] == j["array"]);

    CHECK(j_const["integer"] == Json(1));
    CHECK(j_const[std::string_view("integer")] == j["integer"]);

    CHECK(j_const["boolean"] == Json(true));
    CHECK(j_const[std::string_view("boolean")] == j["boolean"]);

    CHECK(j_const["null"] == Json(nullptr));
    CHECK(j_const[std::string_view("null")] == j["null"]);

    CHECK(j_const["string"] == Json("hello world"));
    CHECK(j_const[std::string_view("string")] == j["string"]);

    CHECK(j_const["floating"] == Json(42.23));
    CHECK(j_const[std::string_view("floating")] == j["floating"]);

    CHECK(j_const["object"] == Json::object());
    CHECK(j_const[std::string_view("object")] == j["object"]);

    CHECK(j_const["array"] == Json({1, 2, 3}));
    CHECK(j_const[std::string_view("array")] == j["array"]);
}

#endif

class ElementAccess2ObjectAccessSpecifiedElementAccessOnNonObjectTypeTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectAccessSpecifiedElementAccessOnNonObjectTypeTest, Null)
{
    Json j_nonobject(Json::value_t::null);
    Json j_nonobject2(Json::value_t::null);
    const Json j_const_nonobject(j_nonobject);

    CHECK_NOTHROW(j_nonobject["foo"]);
    CHECK_NOTHROW(j_nonobject2[typename Json::object_t::key_type("foo")]);
    CHECK_THROWS_WITH_AS(j_const_nonobject["foo"], "[json.exception.type_error.305] cannot use operator[] with a string argument with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[typename Json::object_t::key_type("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with null", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_NOTHROW(j_nonobject2[std::string_view("foo")]);
    CHECK_THROWS_WITH_AS(j_const_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with null", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementAccessOnNonObjectTypeTest, Boolean)
{
    Json j_nonobject(Json::value_t::boolean);
    const Json j_const_nonobject(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with boolean", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with boolean", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with boolean", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with boolean", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with boolean", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with boolean", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementAccessOnNonObjectTypeTest, String)
{
    Json j_nonobject(Json::value_t::string);
    const Json j_const_nonobject(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with string", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with string", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with string", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with string", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with string", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with string", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementAccessOnNonObjectTypeTest, Array)
{
    Json j_nonobject(Json::value_t::array);
    const Json j_const_nonobject(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with array", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject[typename Json::object_t::key_type("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with array", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with array", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with array", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with array", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with array", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementAccessOnNonObjectTypeTest, NumberInteger)
{
    Json j_nonobject(Json::value_t::number_integer);
    const Json j_const_nonobject(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementAccessOnNonObjectTypeTest, NumberUnsigned)
{
    Json j_nonobject(Json::value_t::number_unsigned);
    const Json j_const_nonobject(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectAccessSpecifiedElementAccessOnNonObjectTypeTest, NumberFloatingPoint)
{
    Json j_nonobject(Json::value_t::number_float);
    const Json j_const_nonobject(j_nonobject);
    CHECK_THROWS_WITH_AS(j_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject["foo"],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[typename Json::object_t::key_type("foo")],
                         "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(j_const_nonobject[std::string_view("foo")], "[json.exception.type_error.305] cannot use operator[] with a string argument with number", typename Json::type_error&);
#endif
}

class ElementAccess2ObjectRemoveSpecifiedElementTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectRemoveSpecifiedElementTest, RemoveElementByKey)
{
    CHECK(j.find("integer") != j.end());
    CHECK(j.erase("integer") == 1);
    CHECK(j.find("integer") == j.end());
    CHECK(j.erase("integer") == 0);

    CHECK(j.find("unsigned") != j.end());
    CHECK(j.erase("unsigned") == 1);
    CHECK(j.find("unsigned") == j.end());
    CHECK(j.erase("unsigned") == 0);

    CHECK(j.find("boolean") != j.end());
    CHECK(j.erase("boolean") == 1);
    CHECK(j.find("boolean") == j.end());
    CHECK(j.erase("boolean") == 0);

    CHECK(j.find("null") != j.end());
    CHECK(j.erase("null") == 1);
    CHECK(j.find("null") == j.end());
    CHECK(j.erase("null") == 0);

    CHECK(j.find("string") != j.end());
    CHECK(j.erase("string") == 1);
    CHECK(j.find("string") == j.end());
    CHECK(j.erase("string") == 0);

    CHECK(j.find("floating") != j.end());
    CHECK(j.erase("floating") == 1);
    CHECK(j.find("floating") == j.end());
    CHECK(j.erase("floating") == 0);

    CHECK(j.find("object") != j.end());
    CHECK(j.erase("object") == 1);
    CHECK(j.find("object") == j.end());
    CHECK(j.erase("object") == 0);

    CHECK(j.find("array") != j.end());
    CHECK(j.erase("array") == 1);
    CHECK(j.find("array") == j.end());
    CHECK(j.erase("array") == 0);
}

#ifdef JSON_HAS_CPP_17

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementTest, RemoveElementByKeyStringView)
{
    CHECK(j.find(std::string_view("integer")) != j.end());
    CHECK(j.erase(std::string_view("integer")) == 1);
    CHECK(j.find(std::string_view("integer")) == j.end());
    CHECK(j.erase(std::string_view("integer")) == 0);

    CHECK(j.find(std::string_view("unsigned")) != j.end());
    CHECK(j.erase(std::string_view("unsigned")) == 1);
    CHECK(j.find(std::string_view("unsigned")) == j.end());
    CHECK(j.erase(std::string_view("unsigned")) == 0);

    CHECK(j.find(std::string_view("boolean")) != j.end());
    CHECK(j.erase(std::string_view("boolean")) == 1);
    CHECK(j.find(std::string_view("boolean")) == j.end());
    CHECK(j.erase(std::string_view("boolean")) == 0);

    CHECK(j.find(std::string_view("null")) != j.end());
    CHECK(j.erase(std::string_view("null")) == 1);
    CHECK(j.find(std::string_view("null")) == j.end());
    CHECK(j.erase(std::string_view("null")) == 0);

    CHECK(j.find(std::string_view("string")) != j.end());
    CHECK(j.erase(std::string_view("string")) == 1);
    CHECK(j.find(std::string_view("string")) == j.end());
    CHECK(j.erase(std::string_view("string")) == 0);

    CHECK(j.find(std::string_view("floating")) != j.end());
    CHECK(j.erase(std::string_view("floating")) == 1);
    CHECK(j.find(std::string_view("floating")) == j.end());
    CHECK(j.erase(std::string_view("floating")) == 0);

    CHECK(j.find(std::string_view("object")) != j.end());
    CHECK(j.erase(std::string_view("object")) == 1);
    CHECK(j.find(std::string_view("object")) == j.end());
    CHECK(j.erase(std::string_view("object")) == 0);

    CHECK(j.find(std::string_view("array")) != j.end());
    CHECK(j.erase(std::string_view("array")) == 1);
    CHECK(j.find(std::string_view("array")) == j.end());
    CHECK(j.erase(std::string_view("array")) == 0);
}

#endif

class ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByIteratorTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByIteratorTest, EraseBegin)
{
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        typename Json::iterator it2 = jobject.erase(jobject.begin());
        CHECK(jobject == Json({{"b", 1}, {"c", 17u}}));
        CHECK(*it2 == Json(1));
    }
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        typename Json::const_iterator it2 = jobject.erase(jobject.cbegin());
        CHECK(jobject == Json({{"b", 1}, {"c", 17u}}));
        CHECK(*it2 == Json(1));
    }
}

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByIteratorTest, EraseBeginEnd)
{
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        typename Json::iterator it2 = jobject.erase(jobject.begin(), jobject.end());
        CHECK(jobject == Json::object());
        CHECK(it2 == jobject.end());
    }
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        typename Json::const_iterator it2 = jobject.erase(jobject.cbegin(), jobject.cend());
        CHECK(jobject == Json::object());
        CHECK(it2 == jobject.cend());
    }
}

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByIteratorTest, EraseBeginBegin)
{
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        typename Json::iterator it2 = jobject.erase(jobject.begin(), jobject.begin());
        CHECK(jobject == Json({{"a", "a"}, {"b", 1}, {"c", 17u}}));
        CHECK(*it2 == Json("a"));
    }
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        typename Json::const_iterator it2 = jobject.erase(jobject.cbegin(), jobject.cbegin());
        CHECK(jobject == Json({{"a", "a"}, {"b", 1}, {"c", 17u}}));
        CHECK(*it2 == Json("a"));
    }
}

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByIteratorTest, EraseAtOffset)
{
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        typename Json::iterator it = jobject.find("b");
        typename Json::iterator it2 = jobject.erase(it);
        CHECK(jobject == Json({{"a", "a"}, {"c", 17u}}));
        CHECK(*it2 == Json(17));
    }
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        typename Json::const_iterator it = jobject.find("b");
        typename Json::const_iterator it2 = jobject.erase(it);
        CHECK(jobject == Json({{"a", "a"}, {"c", 17u}}));
        CHECK(*it2 == Json(17));
    }
}

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByIteratorTest, EraseSubrange)
{
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}, {"d", false}, {"e", true}};
        typename Json::iterator it2 = jobject.erase(jobject.find("b"), jobject.find("e"));
        CHECK(jobject == Json({{"a", "a"}, {"e", true}}));
        CHECK(*it2 == Json(true));
    }
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}, {"d", false}, {"e", true}};
        typename Json::const_iterator it2 = jobject.erase(jobject.find("b"), jobject.find("e"));
        CHECK(jobject == Json({{"a", "a"}, {"e", true}}));
        CHECK(*it2 == Json(true));
    }
}

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByIteratorTest, DifferentObjects)
{
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}, {"d", false}, {"e", true}};
        Json jobject2 = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        CHECK_THROWS_WITH_AS(jobject.erase(jobject2.begin()),
                             "[json.exception.invalid_iterator.202] iterator does not fit current value", typename Json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jobject.erase(jobject.begin(), jobject2.end()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", typename Json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jobject.erase(jobject2.begin(), jobject.end()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", typename Json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jobject.erase(jobject2.begin(), jobject2.end()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", typename Json::invalid_iterator&);
    }
    {
        Json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}, {"d", false}, {"e", true}};
        Json jobject2 = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        CHECK_THROWS_WITH_AS(jobject.erase(jobject2.cbegin()),
                             "[json.exception.invalid_iterator.202] iterator does not fit current value", typename Json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jobject.erase(jobject.cbegin(), jobject2.cend()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", typename Json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jobject.erase(jobject2.cbegin(), jobject.cend()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", typename Json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jobject.erase(jobject2.cbegin(), jobject2.cend()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", typename Json::invalid_iterator&);
    }
}

class ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByKeyInNonObjectTypeTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByKeyInNonObjectTypeTest, Null)
{
    Json j_nonobject(Json::value_t::null);
    CHECK_THROWS_WITH_AS(j_nonobject.erase("foo"), "[json.exception.type_error.307] cannot use erase() with null", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.erase(std::string_view("foo")), "[json.exception.type_error.307] cannot use erase() with null", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByKeyInNonObjectTypeTest, Boolean)
{
    Json j_nonobject(Json::value_t::boolean);
    CHECK_THROWS_WITH_AS(j_nonobject.erase("foo"), "[json.exception.type_error.307] cannot use erase() with boolean", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.erase(std::string_view("foo")), "[json.exception.type_error.307] cannot use erase() with boolean", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByKeyInNonObjectTypeTest, String)
{
    Json j_nonobject(Json::value_t::string);
    CHECK_THROWS_WITH_AS(j_nonobject.erase("foo"), "[json.exception.type_error.307] cannot use erase() with string", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.erase(std::string_view("foo")), "[json.exception.type_error.307] cannot use erase() with string", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByKeyInNonObjectTypeTest, Array)
{
    Json j_nonobject(Json::value_t::array);
    CHECK_THROWS_WITH_AS(j_nonobject.erase("foo"), "[json.exception.type_error.307] cannot use erase() with array", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.erase(std::string_view("foo")), "[json.exception.type_error.307] cannot use erase() with array", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByKeyInNonObjectTypeTest, NumberInteger)
{
    Json j_nonobject(Json::value_t::number_integer);
    CHECK_THROWS_WITH_AS(j_nonobject.erase("foo"), "[json.exception.type_error.307] cannot use erase() with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.erase(std::string_view("foo")), "[json.exception.type_error.307] cannot use erase() with number", typename Json::type_error&);
#endif
}

TEST_F(ElementAccess2ObjectRemoveSpecifiedElementRemoveElementByKeyInNonObjectTypeTest, NumberFloatingPoint)
{
    Json j_nonobject(Json::value_t::number_float);
    CHECK_THROWS_WITH_AS(j_nonobject.erase("foo"), "[json.exception.type_error.307] cannot use erase() with number", typename Json::type_error&);

#ifdef JSON_HAS_CPP_17
    CHECK_THROWS_WITH_AS(j_nonobject.erase(std::string_view("foo")), "[json.exception.type_error.307] cannot use erase() with number", typename Json::type_error&);
#endif
}

class ElementAccess2ObjectFindAnElementInAnObjectTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectFindAnElementInAnObjectTest, ExistingElement)
{
    for (const auto* key :
            {"integer", "unsigned", "floating", "null", "string", "boolean", "object", "array"
            })
    {
        CHECK(j.find(key) != j.end());
        CHECK(*j.find(key) == j.at(key));
        CHECK(j_const.find(key) != j_const.end());
        CHECK(*j_const.find(key) == j_const.at(key));
    }
#ifdef JSON_HAS_CPP_17
    for (const std::string_view key :
            {"integer", "unsigned", "floating", "null", "string", "boolean", "object", "array"
            })
    {
        CHECK(j.find(key) != j.end());
        CHECK(*j.find(key) == j.at(key));
        CHECK(j_const.find(key) != j_const.end());
        CHECK(*j_const.find(key) == j_const.at(key));
    }
#endif
}

TEST_F(ElementAccess2ObjectFindAnElementInAnObjectTest, NonexistingElement)
{
    CHECK(j.find("foo") == j.end());
    CHECK(j_const.find("foo") == j_const.end());

#ifdef JSON_HAS_CPP_17
    CHECK(j.find(std::string_view("foo")) == j.end());
    CHECK(j_const.find(std::string_view("foo")) == j_const.end());
#endif
}

class ElementAccess2ObjectFindAnElementInAnObjectAllTypesTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectFindAnElementInAnObjectAllTypesTest, Null)
{
    Json j_nonarray(Json::value_t::null);
    const Json j_nonarray_const(j_nonarray);

    CHECK(j_nonarray.find("foo") == j_nonarray.end());
    CHECK(j_nonarray_const.find("foo") == j_nonarray_const.end());

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonarray.find(std::string_view("foo")) == j_nonarray.end());
    CHECK(j_nonarray_const.find(std::string_view("foo")) == j_nonarray_const.end());
#endif
}

TEST_F(ElementAccess2ObjectFindAnElementInAnObjectAllTypesTest, String)
{
    Json j_nonarray(Json::value_t::string);
    const Json j_nonarray_const(j_nonarray);

    CHECK(j_nonarray.find("foo") == j_nonarray.end());
    CHECK(j_nonarray_const.find("foo") == j_nonarray_const.end());

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonarray.find(std::string_view("foo")) == j_nonarray.end());
    CHECK(j_nonarray_const.find(std::string_view("foo")) == j_nonarray_const.end());
#endif
}

TEST_F(ElementAccess2ObjectFindAnElementInAnObjectAllTypesTest, Object)
{
    Json j_nonarray(Json::value_t::object);
    const Json j_nonarray_const(j_nonarray);

    CHECK(j_nonarray.find("foo") == j_nonarray.end());
    CHECK(j_nonarray_const.find("foo") == j_nonarray_const.end());

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonarray.find(std::string_view("foo")) == j_nonarray.end());
    CHECK(j_nonarray_const.find(std::string_view("foo")) == j_nonarray_const.end());
#endif
}

TEST_F(ElementAccess2ObjectFindAnElementInAnObjectAllTypesTest, Array)
{
    Json j_nonarray(Json::value_t::array);
    const Json j_nonarray_const(j_nonarray);

    CHECK(j_nonarray.find("foo") == j_nonarray.end());
    CHECK(j_nonarray_const.find("foo") == j_nonarray_const.end());

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonarray.find(std::string_view("foo")) == j_nonarray.end());
    CHECK(j_nonarray_const.find(std::string_view("foo")) == j_nonarray_const.end());
#endif
}

TEST_F(ElementAccess2ObjectFindAnElementInAnObjectAllTypesTest, Boolean)
{
    Json j_nonarray(Json::value_t::boolean);
    const Json j_nonarray_const(j_nonarray);

    CHECK(j_nonarray.find("foo") == j_nonarray.end());
    CHECK(j_nonarray_const.find("foo") == j_nonarray_const.end());

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonarray.find(std::string_view("foo")) == j_nonarray.end());
    CHECK(j_nonarray_const.find(std::string_view("foo")) == j_nonarray_const.end());
#endif
}

TEST_F(ElementAccess2ObjectFindAnElementInAnObjectAllTypesTest, NumberInteger)
{
    Json j_nonarray(Json::value_t::number_integer);
    const Json j_nonarray_const(j_nonarray);

    CHECK(j_nonarray.find("foo") == j_nonarray.end());
    CHECK(j_nonarray_const.find("foo") == j_nonarray_const.end());

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonarray.find(std::string_view("foo")) == j_nonarray.end());
    CHECK(j_nonarray_const.find(std::string_view("foo")) == j_nonarray_const.end());
#endif
}

TEST_F(ElementAccess2ObjectFindAnElementInAnObjectAllTypesTest, NumberUnsigned)
{
    Json j_nonarray(Json::value_t::number_unsigned);
    const Json j_nonarray_const(j_nonarray);

    CHECK(j_nonarray.find("foo") == j_nonarray.end());
    CHECK(j_nonarray_const.find("foo") == j_nonarray_const.end());

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonarray.find(std::string_view("foo")) == j_nonarray.end());
    CHECK(j_nonarray_const.find(std::string_view("foo")) == j_nonarray_const.end());
#endif
}

TEST_F(ElementAccess2ObjectFindAnElementInAnObjectAllTypesTest, NumberFloatingPoint)
{
    Json j_nonarray(Json::value_t::number_float);
    const Json j_nonarray_const(j_nonarray);

    CHECK(j_nonarray.find("foo") == j_nonarray.end());
    CHECK(j_nonarray_const.find("foo") == j_nonarray_const.end());

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonarray.find(std::string_view("foo")) == j_nonarray.end());
    CHECK(j_nonarray_const.find(std::string_view("foo")) == j_nonarray_const.end());
#endif
}

class ElementAccess2ObjectCountKeysInAnObjectTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectCountKeysInAnObjectTest, ExistingElement)
{
    for (const auto* key :
            {"integer", "unsigned", "floating", "null", "string", "boolean", "object", "array"
            })
    {
        CHECK(j.count(key) == 1);
        CHECK(j_const.count(key) == 1);
    }
#ifdef JSON_HAS_CPP_17
    for (const std::string_view key :
            {"integer", "unsigned", "floating", "null", "string", "boolean", "object", "array"
            })
    {
        CHECK(j.count(key) == 1);
        CHECK(j_const.count(key) == 1);
    }
#endif
}

TEST_F(ElementAccess2ObjectCountKeysInAnObjectTest, NonexistingElement)
{
    CHECK(j.count("foo") == 0);
    CHECK(j_const.count("foo") == 0);

#ifdef JSON_HAS_CPP_17
    CHECK(j.count(std::string_view("foo")) == 0);
    CHECK(j_const.count(std::string_view("foo")) == 0);
#endif
}

class ElementAccess2ObjectCountKeysInAnObjectAllTypesTest : public ElementAccess2ObjectTest{};


TEST(ElementAccess2ObjectCountKeysInAnObjectAllTypesTest, Null)
{
    Json j_nonobject(Json::value_t::null);
    const Json j_nonobject_const(Json::value_t::null);

    CHECK(j_nonobject.count("foo") == 0);
    CHECK(j_nonobject_const.count("foo") == 0);

#ifdef JSON_HAS_CPP_17
    CHECK(j.count(std::string_view("foo")) == 0);
    CHECK(j_const.count(std::string_view("foo")) == 0);
#endif
}

TEST(ElementAccess2ObjectCountKeysInAnObjectAllTypesTest, String)
{
    Json j_nonobject(Json::value_t::string);
    const Json j_nonobject_const(Json::value_t::string);

    CHECK(j_nonobject.count("foo") == 0);
    CHECK(j_nonobject_const.count("foo") == 0);

#ifdef JSON_HAS_CPP_17
    CHECK(j.count(std::string_view("foo")) == 0);
    CHECK(j_const.count(std::string_view("foo")) == 0);
#endif
}

TEST(ElementAccess2ObjectCountKeysInAnObjectAllTypesTest, Object)
{
    Json j_nonobject(Json::value_t::object);
    const Json j_nonobject_const(Json::value_t::object);

    CHECK(j_nonobject.count("foo") == 0);
    CHECK(j_nonobject_const.count("foo") == 0);

#ifdef JSON_HAS_CPP_17
    CHECK(j.count(std::string_view("foo")) == 0);
    CHECK(j_const.count(std::string_view("foo")) == 0);
#endif
}

TEST(ElementAccess2ObjectCountKeysInAnObjectAllTypesTest, Array)
{
    Json j_nonobject(Json::value_t::array);
    const Json j_nonobject_const(Json::value_t::array);

    CHECK(j_nonobject.count("foo") == 0);
    CHECK(j_nonobject_const.count("foo") == 0);

#ifdef JSON_HAS_CPP_17
    CHECK(j.count(std::string_view("foo")) == 0);
    CHECK(j_const.count(std::string_view("foo")) == 0);
#endif
}

TEST(ElementAccess2ObjectCountKeysInAnObjectAllTypesTest, Boolean)
{
    Json j_nonobject(Json::value_t::boolean);
    const Json j_nonobject_const(Json::value_t::boolean);

    CHECK(j_nonobject.count("foo") == 0);
    CHECK(j_nonobject_const.count("foo") == 0);

#ifdef JSON_HAS_CPP_17
    CHECK(j.count(std::string_view("foo")) == 0);
    CHECK(j_const.count(std::string_view("foo")) == 0);
#endif
}

TEST(ElementAccess2ObjectCountKeysInAnObjectAllTypesTest, NumberInteger)
{
    Json j_nonobject(Json::value_t::number_integer);
    const Json j_nonobject_const(Json::value_t::number_integer);

    CHECK(j_nonobject.count("foo") == 0);
    CHECK(j_nonobject_const.count("foo") == 0);

#ifdef JSON_HAS_CPP_17
    CHECK(j.count(std::string_view("foo")) == 0);
    CHECK(j_const.count(std::string_view("foo")) == 0);
#endif
}

TEST(ElementAccess2ObjectCountKeysInAnObjectAllTypesTest, NumberUnsigned)
{
    Json j_nonobject(Json::value_t::number_unsigned);
    const Json j_nonobject_const(Json::value_t::number_unsigned);

    CHECK(j_nonobject.count("foo") == 0);
    CHECK(j_nonobject_const.count("foo") == 0);

#ifdef JSON_HAS_CPP_17
    CHECK(j.count(std::string_view("foo")) == 0);
    CHECK(j_const.count(std::string_view("foo")) == 0);
#endif
}

TEST(ElementAccess2ObjectCountKeysInAnObjectAllTypesTest, NumberFloatingPoint)
{
    Json j_nonobject(Json::value_t::number_float);
    const Json j_nonobject_const(Json::value_t::number_float);

    CHECK(j_nonobject.count("foo") == 0);
    CHECK(j_nonobject_const.count("foo") == 0);

#ifdef JSON_HAS_CPP_17
    CHECK(j.count(std::string_view("foo")) == 0);
    CHECK(j_const.count(std::string_view("foo")) == 0);
#endif
}

class ElementAccess2ObjectCheckExistenceOfKeyInAnObjectTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectCheckExistenceOfKeyInAnObjectTest, ExistingElement)
{
    for (const auto* key :
            {"integer", "unsigned", "floating", "null", "string", "boolean", "object", "array"
            })
    {
        CHECK(j.contains(key) == true);
        CHECK(j_const.contains(key) == true);
    }

#ifdef JSON_HAS_CPP_17
    for (const std::string_view key :
            {"integer", "unsigned", "floating", "null", "string", "boolean", "object", "array"
            })
    {
        CHECK(j.contains(key) == true);
        CHECK(j_const.contains(key) == true);
    }
#endif
}

TEST_F(ElementAccess2ObjectCheckExistenceOfKeyInAnObjectTest, NonexistingElement)
{
    CHECK(j.contains("foo") == false);
    CHECK(j_const.contains("foo") == false);

#ifdef JSON_HAS_CPP_17
    CHECK(j.contains(std::string_view("foo")) == false);
    CHECK(j_const.contains(std::string_view("foo")) == false);
#endif
}

class ElementAccess2ObjectCheckExistenceOfKeyInAnObjectAllTypesTest : public ElementAccess2ObjectTest{};


TEST_F(ElementAccess2ObjectCheckExistenceOfKeyInAnObjectAllTypesTest, Null)
{
    Json j_nonobject(Json::value_t::null);
    const Json j_nonobject_const(Json::value_t::null);

    CHECK(j_nonobject.contains("foo") == false);
    CHECK(j_nonobject_const.contains("foo") == false);

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonobject.contains(std::string_view("foo")) == false);
    CHECK(j_nonobject_const.contains(std::string_view("foo")) == false);
#endif
}

TEST_F(ElementAccess2ObjectCheckExistenceOfKeyInAnObjectAllTypesTest, String)
{
    Json j_nonobject(Json::value_t::string);
    const Json j_nonobject_const(Json::value_t::string);

    CHECK(j_nonobject.contains("foo") == false);
    CHECK(j_nonobject_const.contains("foo") == false);

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonobject.contains(std::string_view("foo")) == false);
    CHECK(j_nonobject_const.contains(std::string_view("foo")) == false);
#endif
}

TEST_F(ElementAccess2ObjectCheckExistenceOfKeyInAnObjectAllTypesTest, Object)
{
    Json j_nonobject(Json::value_t::object);
    const Json j_nonobject_const(Json::value_t::object);

    CHECK(j_nonobject.contains("foo") == false);
    CHECK(j_nonobject_const.contains("foo") == false);

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonobject.contains(std::string_view("foo")) == false);
    CHECK(j_nonobject_const.contains(std::string_view("foo")) == false);
#endif
}

TEST_F(ElementAccess2ObjectCheckExistenceOfKeyInAnObjectAllTypesTest, Array)
{
    Json j_nonobject(Json::value_t::array);
    const Json j_nonobject_const(Json::value_t::array);

    CHECK(j_nonobject.contains("foo") == false);
    CHECK(j_nonobject_const.contains("foo") == false);

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonobject.contains(std::string_view("foo")) == false);
    CHECK(j_nonobject_const.contains(std::string_view("foo")) == false);
#endif
}

TEST_F(ElementAccess2ObjectCheckExistenceOfKeyInAnObjectAllTypesTest, Boolean)
{
    Json j_nonobject(Json::value_t::boolean);
    const Json j_nonobject_const(Json::value_t::boolean);

    CHECK(j_nonobject.contains("foo") == false);
    CHECK(j_nonobject_const.contains("foo") == false);

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonobject.contains(std::string_view("foo")) == false);
    CHECK(j_nonobject_const.contains(std::string_view("foo")) == false);
#endif
}

TEST_F(ElementAccess2ObjectCheckExistenceOfKeyInAnObjectAllTypesTest, NumberInteger)
{
    Json j_nonobject(Json::value_t::number_integer);
    const Json j_nonobject_const(Json::value_t::number_integer);

    CHECK(j_nonobject.contains("foo") == false);
    CHECK(j_nonobject_const.contains("foo") == false);

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonobject.contains(std::string_view("foo")) == false);
    CHECK(j_nonobject_const.contains(std::string_view("foo")) == false);
#endif
}

TEST_F(ElementAccess2ObjectCheckExistenceOfKeyInAnObjectAllTypesTest, NumberUnsigned)
{
    Json j_nonobject(Json::value_t::number_unsigned);
    const Json j_nonobject_const(Json::value_t::number_unsigned);

    CHECK(j_nonobject.contains("foo") == false);
    CHECK(j_nonobject_const.contains("foo") == false);

#ifdef JSON_HAS_CPP_17
    CHECK(j_nonobject.contains(std::string_view("foo")) == false);
    CHECK(j_nonobject_const.contains(std::string_view("foo")) == false);
#endif
}

TEST_F(ElementAccess2ObjectCheckExistenceOfKeyInAnObjectAllTypesTest, NumberFloatingPoint)
{
    Json j_nonobject(Json::value_t::number_float);
    const Json j_nonobject_const(Json::value_t::number_float);
    CHECK(j_nonobject.contains("foo") == false);
    CHECK(j_nonobject_const.contains("foo") == false);
#ifdef JSON_HAS_CPP_17
    CHECK(j_nonobject.contains(std::string_view("foo")) == false);
    CHECK(j_nonobject_const.contains(std::string_view("foo")) == false);
#endif
}

#if !defined(JSON_NOEXCEPTION)



class ElementAccess2ThrowingTestsObjectTest : public ::testing::Test {
 protected:
    Json j = {{"integer", 1}, {"unsigned", 1u}, {"floating", 42.23}, {"null", nullptr}, {"string", "hello world"}, {"boolean", true}, {"object", Json::object()}, {"array", {1, 2, 3}}};
    const Json j_const = {{"integer", 1}, {"unsigned", 1u}, {"floating", 42.23}, {"null", nullptr}, {"string", "hello world"}, {"boolean", true}, {"object", Json::object()}, {"array", {1, 2, 3}}};
};


class ElementAccess2ThrowingTestsObjectAccessSpecifiedElementWithDefaultValueTest : public ElementAccess2ThrowingTestsObjectTest{};


class ElementAccess2ThrowingTestsObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerTest : public ElementAccess2ThrowingTestsObjectTest{};


TEST_F(ElementAccess2ThrowingTestsObjectAccessSpecifiedElementWithDefaultValueGivenAJSONPointerTest, AccessNonExistingValue)
{
    CHECK(j.value("/not/existing"_json_pointer, 2) == 2);
    CHECK(j.value("/not/existing"_json_pointer, 2u) == 2u);
    CHECK(j.value("/not/existing"_json_pointer, false) == false);
    CHECK(j.value("/not/existing"_json_pointer, "bar") == "bar");
    EXPECT_NEAR(j.value("/not/existing"_json_pointer, 12.34), 12.34, 0.001);
    CHECK(j.value("/not/existing"_json_pointer, Json({{"foo", "bar"}})) == Json({{"foo", "bar"}}));
    CHECK(j.value("/not/existing"_json_pointer, Json({10, 100})) == Json({10, 100}));

    CHECK(j_const.value("/not/existing"_json_pointer, 2) == 2);
    CHECK(j_const.value("/not/existing"_json_pointer, 2u) == 2u);
    CHECK(j_const.value("/not/existing"_json_pointer, false) == false);
    CHECK(j_const.value("/not/existing"_json_pointer, "bar") == "bar");
    EXPECT_NEAR(j_const.value("/not/existing"_json_pointer, 12.34), 12.34, 0.001);
    CHECK(j_const.value("/not/existing"_json_pointer, Json({{"foo", "bar"}})) == Json({{"foo", "bar"}}));
    CHECK(j_const.value("/not/existing"_json_pointer, Json({10, 100})) == Json({10, 100}));
}
#endif

// TODO(falbrechtskirchinger) merge with the other test case; clean up
class ElementAccess2AdditionalValueTestsTest : public ::testing::Test {
 protected:
    using string_t = typename Json::string_t;
    using number_integer_t = typename Json::number_integer_t;

    // test assumes string_t and object_t::key_type are the same
    // REQUIRE(std::is_same<string_t, typename Json::object_t::key_type>::value);

    Json j
    {
        {"foo", "bar"},
        {"baz", 42}
    };

    const char* cpstr = "default";
    // const char castr[] = "default"; // NOLINT(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    string_t str = "default";

    number_integer_t integer = 69;
    std::size_t size = 69;
};


class ElementAccess2AdditionalValueTestsDeducedValueTypeTest : public ElementAccess2AdditionalValueTestsTest{};


TEST_F(ElementAccess2AdditionalValueTestsDeducedValueTypeTest, LiteralKey)
{
    CHECK(j.value("foo", "default") == "bar");
    CHECK(j.value("foo", cpstr) == "bar");
    // CHECK(j.value("foo", castr) == "bar");
    CHECK(j.value("foo", str) == "bar");
    // this test is in fact different than the one below,
    // because of 0 considering const char * overloads
    // where as any other number does not
    CHECK(j.value("baz", 0) == 42);
    CHECK(j.value("baz", 47) == 42);
    CHECK(j.value("baz", integer) == 42);
    CHECK(j.value("baz", size) == 42);

    CHECK(j.value("bar", "default") == "default");
    CHECK(j.value("bar", 0) == 0);
    CHECK(j.value("bar", 47) == 47);
    CHECK(j.value("bar", integer) == integer);
    CHECK(j.value("bar", size) == size);

    CHECK_THROWS_WITH_AS(Json().value("foo", "default"), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(Json().value("foo", str), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

TEST_F(ElementAccess2AdditionalValueTestsDeducedValueTypeTest, ConstCharPointerKey)
{
    const char* key = "foo";
    const char* key2 = "baz";
    const char* key_notfound = "bar";

    CHECK(j.value(key, "default") == "bar");
    CHECK(j.value(key, cpstr) == "bar");
    // CHECK(j.value(key, castr) == "bar");
    CHECK(j.value(key, str) == "bar");
    CHECK(j.value(key2, 0) == 42);
    CHECK(j.value(key2, 47) == 42);
    CHECK(j.value(key2, integer) == 42);
    CHECK(j.value(key2, size) == 42);

    CHECK(j.value(key_notfound, "default") == "default");
    CHECK(j.value(key_notfound, 0) == 0);
    CHECK(j.value(key_notfound, 47) == 47);
    CHECK(j.value(key_notfound, integer) == integer);
    CHECK(j.value(key_notfound, size) == size);

    CHECK_THROWS_WITH_AS(Json().value(key, "default"), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(Json().value(key, str), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

TEST_F(ElementAccess2AdditionalValueTestsDeducedValueTypeTest, ConstCharKey)
{
    const char key[] = "foo"; // NOLINT(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    const char key2[] = "baz"; // NOLINT(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    const char key_notfound[] = "bar"; // NOLINT(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)

    CHECK(j.value(key, "default") == "bar");
    CHECK(j.value(key, cpstr) == "bar");
    // CHECK(j.value(key, castr) == "bar");
    CHECK(j.value(key, str) == "bar");
    CHECK(j.value(key2, 0) == 42);
    CHECK(j.value(key2, 47) == 42);
    CHECK(j.value(key2, integer) == 42);
    CHECK(j.value(key2, size) == 42);

    CHECK(j.value(key_notfound, "default") == "default");
    CHECK(j.value(key_notfound, 0) == 0);
    CHECK(j.value(key_notfound, 47) == 47);
    CHECK(j.value(key_notfound, integer) == integer);
    CHECK(j.value(key_notfound, size) == size);

    CHECK_THROWS_WITH_AS(Json().value(key, "default"), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(Json().value(key, str), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

TEST_F(ElementAccess2AdditionalValueTestsDeducedValueTypeTest, StringTObjectTKeyTypeKey)
{
    string_t key = "foo";
    string_t key2 = "baz";
    string_t key_notfound = "bar";

    CHECK(j.value(key, "default") == "bar");
    CHECK(j.value(key, cpstr) == "bar");
    // CHECK(j.value(key, castr) == "bar");
    CHECK(j.value(key, str) == "bar");
    CHECK(j.value(key2, 0) == 42);
    CHECK(j.value(key2, 47) == 42);
    CHECK(j.value(key2, integer) == 42);
    CHECK(j.value(key2, size) == 42);

    CHECK(j.value(key_notfound, "default") == "default");
    CHECK(j.value(key_notfound, 0) == 0);
    CHECK(j.value(key_notfound, 47) == 47);
    CHECK(j.value(key_notfound, integer) == integer);
    CHECK(j.value(key_notfound, size) == size);

    CHECK_THROWS_WITH_AS(Json().value(key, "default"), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(Json().value(key, str), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

#ifdef JSON_HAS_CPP_17

TEST_F(ElementAccess2AdditionalValueTestsDeducedValueTypeTest, StdStringViewKey)
{
    std::string_view key = "foo";
    std::string_view key2 = "baz";
    std::string_view key_notfound = "bar";

    CHECK(j.value(key, "default") == "bar");
    CHECK(j.value(key, cpstr) == "bar");
    // CHECK(j.value(key, castr) == "bar");
    CHECK(j.value(key, str) == "bar");
    CHECK(j.value(key2, 0) == 42);
    CHECK(j.value(key2, 47) == 42);
    CHECK(j.value(key2, integer) == 42);
    CHECK(j.value(key2, size) == 42);

    CHECK(j.value(key_notfound, "default") == "default");
    CHECK(j.value(key_notfound, 0) == 0);
    CHECK(j.value(key_notfound, 47) == 47);
    CHECK(j.value(key_notfound, integer) == integer);
    CHECK(j.value(key_notfound, size) == size);

    CHECK_THROWS_WITH_AS(Json().value(key, "default"), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(Json().value(key, str), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

#endif

class ElementAccess2AdditionalValueTestsExplicitValueTypeTest : public ElementAccess2AdditionalValueTestsTest{};


TEST_F(ElementAccess2AdditionalValueTestsExplicitValueTypeTest, LiteralKey)
{
    CHECK(j.template value<string_t>("foo", "default") == "bar");
    CHECK(j.template value<string_t>("foo", cpstr) == "bar");
    // CHECK(j.template value<string_t>("foo", castr) == "bar");
    CHECK(j.template value<string_t>("foo", str) == "bar");
    CHECK(j.template value<number_integer_t>("baz", 0) == 42);
    CHECK(j.template value<number_integer_t>("baz", 47) == 42);
    CHECK(j.template value<number_integer_t>("baz", integer) == 42);
    CHECK(j.template value<std::size_t>("baz", 0) == 42);
    CHECK(j.template value<std::size_t>("baz", 47) == 42);
    CHECK(j.template value<std::size_t>("baz", size) == 42);

    CHECK(j.template value<string_t>("bar", "default") == "default");
    CHECK(j.template value<number_integer_t>("bar", 0) == 0);
    CHECK(j.template value<number_integer_t>("bar", 47) == 47);
    CHECK(j.template value<number_integer_t>("bar", integer) == integer);
    CHECK(j.template value<std::size_t>("bar", 0) == 0);
    CHECK(j.template value<std::size_t>("bar", 47) == 47);
    CHECK(j.template value<std::size_t>("bar", size) == size);

    CHECK_THROWS_WITH_AS(Json().template value<string_t>("foo", "default"), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(Json().template value<string_t>("foo", str), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

TEST_F(ElementAccess2AdditionalValueTestsExplicitValueTypeTest, ConstCharPointerKey)
{
    const char* key = "foo";
    const char* key2 = "baz";
    const char* key_notfound = "bar";

    CHECK(j.template value<string_t>(key, "default") == "bar");
    CHECK(j.template value<string_t>(key, cpstr) == "bar");
    // CHECK(j.template value<string_t>(key, castr) == "bar");
    CHECK(j.template value<string_t>(key, str) == "bar");
    CHECK(j.template value<number_integer_t>(key2, 0) == 42);
    CHECK(j.template value<number_integer_t>(key2, 47) == 42);
    CHECK(j.template value<number_integer_t>(key2, integer) == 42);
    CHECK(j.template value<std::size_t>(key2, 0) == 42);
    CHECK(j.template value<std::size_t>(key2, 47) == 42);
    CHECK(j.template value<std::size_t>(key2, size) == 42);

    CHECK(j.template value<string_t>(key_notfound, "default") == "default");
    CHECK(j.template value<number_integer_t>(key_notfound, 0) == 0);
    CHECK(j.template value<number_integer_t>(key_notfound, 47) == 47);
    CHECK(j.template value<number_integer_t>(key_notfound, integer) == integer);
    CHECK(j.template value<std::size_t>(key_notfound, 0) == 0);
    CHECK(j.template value<std::size_t>(key_notfound, 47) == 47);
    CHECK(j.template value<std::size_t>(key_notfound, size) == size);

    CHECK_THROWS_WITH_AS(Json().template value<string_t>(key, "default"), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(Json().template value<string_t>(key, str), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

TEST_F(ElementAccess2AdditionalValueTestsExplicitValueTypeTest, ConstCharKey)
{
    const char key[] = "foo"; // NOLINT(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    const char key2[] = "baz"; // NOLINT(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    const char key_notfound[] = "bar"; // NOLINT(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)

    CHECK(j.template value<string_t>(key, "default") == "bar");
    CHECK(j.template value<string_t>(key, cpstr) == "bar");
    // CHECK(j.template value<string_t>(key, castr) == "bar");
    CHECK(j.template value<string_t>(key, str) == "bar");
    CHECK(j.template value<number_integer_t>(key2, 0) == 42);
    CHECK(j.template value<number_integer_t>(key2, 47) == 42);
    CHECK(j.template value<number_integer_t>(key2, integer) == 42);
    CHECK(j.template value<std::size_t>(key2, 0) == 42);
    CHECK(j.template value<std::size_t>(key2, 47) == 42);
    CHECK(j.template value<std::size_t>(key2, size) == 42);

    CHECK(j.template value<string_t>(key_notfound, "default") == "default");
    CHECK(j.template value<number_integer_t>(key_notfound, 0) == 0);
    CHECK(j.template value<number_integer_t>(key_notfound, 47) == 47);
    CHECK(j.template value<number_integer_t>(key_notfound, integer) == integer);
    CHECK(j.template value<std::size_t>(key_notfound, 0) == 0);
    CHECK(j.template value<std::size_t>(key_notfound, 47) == 47);
    CHECK(j.template value<std::size_t>(key_notfound, size) == size);

    CHECK_THROWS_WITH_AS(Json().template value<string_t>(key, "default"), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(Json().template value<string_t>(key, str), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

TEST_F(ElementAccess2AdditionalValueTestsExplicitValueTypeTest, StringTObjectTKeyTypeKey)
{
    string_t key = "foo";
    string_t key2 = "baz";
    string_t key_notfound = "bar";

    CHECK(j.template value<string_t>(key, "default") == "bar");
    CHECK(j.template value<string_t>(key, cpstr) == "bar");
    // CHECK(j.template value<string_t>(key, castr) == "bar");
    CHECK(j.template value<string_t>(key, str) == "bar");
    CHECK(j.template value<number_integer_t>(key2, 0) == 42);
    CHECK(j.template value<number_integer_t>(key2, 47) == 42);
    CHECK(j.template value<std::size_t>(key2, 0) == 42);
    CHECK(j.template value<std::size_t>(key2, 47) == 42);

    CHECK(j.template value<string_t>(key_notfound, "default") == "default");
    CHECK(j.template value<number_integer_t>(key_notfound, 0) == 0);
    CHECK(j.template value<number_integer_t>(key_notfound, 47) == 47);
    CHECK(j.template value<std::size_t>(key_notfound, 0) == 0);
    CHECK(j.template value<std::size_t>(key_notfound, 47) == 47);

    CHECK_THROWS_WITH_AS(Json().template value<string_t>(key, "default"), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(Json().template value<string_t>(key, str), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

#ifdef JSON_HAS_CPP_17

TEST_F(ElementAccess2AdditionalValueTestsExplicitValueTypeTest, StdStringViewKey)
{
    std::string_view key = "foo";
    std::string_view key2 = "baz";
    std::string_view key_notfound = "bar";

    CHECK(j.template value<string_t>(key, "default") == "bar");
    CHECK(j.template value<string_t>(key, cpstr) == "bar");
    // CHECK(j.template value<string_t>(key, castr) == "bar");
    CHECK(j.template value<string_t>(key, str) == "bar");
    CHECK(j.template value<number_integer_t>(key2, 0) == 42);
    CHECK(j.template value<number_integer_t>(key2, 47) == 42);
    CHECK(j.template value<number_integer_t>(key2, integer) == 42);
    CHECK(j.template value<std::size_t>(key2, 0) == 42);
    CHECK(j.template value<std::size_t>(key2, 47) == 42);
    CHECK(j.template value<std::size_t>(key2, size) == 42);

    CHECK(j.template value<string_t>(key_notfound, "default") == "default");
    CHECK(j.template value<number_integer_t>(key_notfound, 0) == 0);
    CHECK(j.template value<number_integer_t>(key_notfound, 47) == 47);
    CHECK(j.template value<number_integer_t>(key_notfound, integer) == integer);
    CHECK(j.template value<std::size_t>(key_notfound, 0) == 0);
    CHECK(j.template value<std::size_t>(key_notfound, 47) == 47);
    CHECK(j.template value<std::size_t>(key_notfound, size) == size);

    CHECK(j.template value<std::string_view>(key, "default") == "bar");
    CHECK(j.template value<std::string_view>(key, cpstr) == "bar");
    // CHECK(j.template value<std::string_view>(key, castr) == "bar");
    CHECK(j.template value<std::string_view>(key, str) == "bar");

    CHECK(j.template value<std::string_view>(key_notfound, "default") == "default");

    CHECK_THROWS_WITH_AS(Json().template value<string_t>(key, "default"), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
    CHECK_THROWS_WITH_AS(Json().template value<string_t>(key, str), "[json.exception.type_error.306] cannot use value() with null", typename Json::type_error&);
}

#endif

