//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// Copyright (c) 2013-2022 Niels Lohmann <http://nlohmann.me>.
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

// cmake/test.cmake selects the C++ standard versions with which to build a
// unit test based on the presence of JSON_HAS_CPP_<VERSION> macros.
// When using macros that are only defined for particular versions of the standard
// (e.g., JSON_HAS_FILESYSTEM for C++17 and up), please mention the corresponding
// version macro in a comment close by, like this:
// JSON_HAS_CPP_<VERSION> (do not remove; see note at top of file)

#include "gtest/gtest.h"

#define JSON_TESTS_PRIVATE
#include "unit-json.h"
using wpi::json;

#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <valarray>

// WPI_JSON_SERIALIZE_ENUM uses a static std::pair




class ValueConversionGetAnObjectExplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetAnObjectExplicitTest() : j(o_reference) {}
    json::object_t o_reference = {{"object", json::object()},
        {"array", {1, 2, 3, 4}},
        {"number", 42},
        {"boolean", false},
        {"null", nullptr},
        {"string", "Hello world"}
    };
    json j;
};


TEST_F(ValueConversionGetAnObjectExplicitTest, JsonObjectT)
{
    json::object_t o = j.get<json::object_t>();
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectExplicitTest, StdMapStringJson)
{
    std::map<json::string_t, json> o =
        j.get<std::map<json::string_t, json>>();
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectExplicitTest, StdMultimapStringJson)
{
    std::multimap<json::string_t, json> o =
        j.get<std::multimap<json::string_t, json>>();
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectExplicitTest, StdUnorderedMapStringJson)
{
    std::unordered_map<json::string_t, json> o =
        j.get<std::unordered_map<json::string_t, json>>();
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectExplicitTest, StdUnorderedMultimapStringJson)
{
    std::unordered_multimap<json::string_t, json> o =
        j.get<std::unordered_multimap<json::string_t, json>>();
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectExplicitTest, ExceptionInCaseOfANonObjectType)
{
    CHECK_THROWS_WITH_AS(
        json(json::value_t::null).get<json::object_t>(),
        "[json.exception.type_error.302] type must be object, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::array).get<json::object_t>(),
        "[json.exception.type_error.302] type must be object, but is array", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::string).get<json::object_t>(),
        "[json.exception.type_error.302] type must be object, but is string", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::boolean).get<json::object_t>(),
                         "[json.exception.type_error.302] type must be object, "
                         "but is boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_integer).get<json::object_t>(),
        "[json.exception.type_error.302] type must be object, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_unsigned).get<json::object_t>(),
        "[json.exception.type_error.302] type must be object, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_float).get<json::object_t>(),
        "[json.exception.type_error.302] type must be object, but is number", json::type_error&);
}

class ValueConversionGetAnObjectExplicitGetToTest : public ::testing::Test {
 protected:
    ValueConversionGetAnObjectExplicitGetToTest() : j(o_reference) {}
    json::object_t o_reference = {{"object", json::object()},
        {"array", {1, 2, 3, 4}},
        {"number", 42},
        {"boolean", false},
        {"null", nullptr},
        {"string", "Hello world"}
    };
    json j;
};


TEST_F(ValueConversionGetAnObjectExplicitGetToTest, JsonObjectT)
{
    json::object_t o = {{"previous", "value"}};
    j.get_to(o);
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectExplicitGetToTest, StdMapStringJson)
{
    std::map<json::string_t, json> o{{"previous", "value"}};
    j.get_to(o);
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectExplicitGetToTest, StdMultimapStringJson)
{
    std::multimap<json::string_t, json> o{{"previous", "value"}};
    j.get_to(o);
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectExplicitGetToTest, StdUnorderedMapStringJson)
{
    std::unordered_map<json::string_t, json> o{{"previous", "value"}};
    j.get_to(o);
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectExplicitGetToTest, StdUnorderedMultimapStringJson)
{
    std::unordered_multimap<json::string_t, json> o{{"previous", "value"}};
    j.get_to(o);
    CHECK(json(o) == j);
}

#if JSON_USE_IMPLICIT_CONVERSIONS

class ValueConversionGetAnObjectImplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetAnObjectImplicitTest() : j(o_reference) {}
    json::object_t o_reference = {{"object", json::object()},
        {"array", {1, 2, 3, 4}},
        {"number", 42},
        {"boolean", false},
        {"null", nullptr},
        {"string", "Hello world"}
    };
    json j;
};


TEST_F(ValueConversionGetAnObjectImplicitTest, JsonObjectT)
{
    json::object_t o = j;
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectImplicitTest, StdMapStringJson)
{
    std::map<json::string_t, json> o = j;
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectImplicitTest, StdMultimapStringJson)
{
    std::multimap<json::string_t, json> o = j;
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectImplicitTest, StdUnorderedMapStringJson)
{
    std::unordered_map<json::string_t, json> o = j;
    CHECK(json(o) == j);
}

TEST_F(ValueConversionGetAnObjectImplicitTest, StdUnorderedMultimapStringJson)
{
    std::unordered_multimap<json::string_t, json> o = j;
    CHECK(json(o) == j);
}

#endif

class ValueConversionGetAnArrayExplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetAnArrayExplicitTest() : j(a_reference) {}
    json::array_t a_reference{json(1),     json(1u),       json(2.2),
                              json(false), json("string"), json()};
    json j;
};


TEST_F(ValueConversionGetAnArrayExplicitTest, JsonArray)
{
    json::array_t a = j.get<json::array_t>();
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayExplicitTest, StdListJson)
{
    std::list<json> a = j.get<std::list<json>>();
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayExplicitTest, StdForwardListJson)
{
    std::forward_list<json> a = j.get<std::forward_list<json>>();
    CHECK(json(a) == j);

    CHECK_THROWS_WITH_AS(
        json(json::value_t::null).get<std::forward_list<json>>(),
        "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
}

class ValueConversionGetAnArrayExplicitStdVectorJsonTest : public ValueConversionGetAnArrayExplicitTest{};


TEST_F(ValueConversionGetAnArrayExplicitStdVectorJsonTest, BasicTest)
{
    std::vector<json> a = j.get<std::vector<json>>();
    CHECK(json(a) == j);

    CHECK_THROWS_WITH_AS(
        json(json::value_t::null).get<std::vector<json>>(),
        "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
}

#if !defined(JSON_NOEXCEPTION)

TEST_F(ValueConversionGetAnArrayExplicitStdVectorJsonTest, ReserveIsCalledOnContainersThatSupportsIt)
{
    // make sure all values are properly copied
    json j2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    auto v2 = j2.get<std::vector<int>>();
    CHECK(v2.size() == 10);
}

#endif

TEST_F(ValueConversionGetAnArrayExplicitTest, BuiltInArrays)
{
    const char str[] = "a string"; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    const int nbs[] = {0, 1, 2}; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)

    json j2 = nbs;
    json j3 = str;

    auto v = j2.get<std::vector<int>>();
    auto s = j3.get<std::string>();
    CHECK(std::equal(v.begin(), v.end(), std::begin(nbs)));
    CHECK(s == str);
}

TEST_F(ValueConversionGetAnArrayExplicitTest, StdDequeJson)
{
    std::deque<json> a = j.get<std::deque<json>>();
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayExplicitTest, ExceptionInCaseOfANonArrayType)
{
    CHECK_THROWS_WITH_AS(
        json(json::value_t::object).get<std::vector<int>>(),
        "[json.exception.type_error.302] type must be array, but is object", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::null).get<json::array_t>(),
        "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::object).get<json::array_t>(),
        "[json.exception.type_error.302] type must be array, but is object", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::string).get<json::array_t>(),
        "[json.exception.type_error.302] type must be array, but is string", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::boolean).get<json::array_t>(),
        "[json.exception.type_error.302] type must be array, but is boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_integer).get<json::array_t>(),
        "[json.exception.type_error.302] type must be array, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_unsigned).get<json::array_t>(),
        "[json.exception.type_error.302] type must be array, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_float).get<json::array_t>(),
        "[json.exception.type_error.302] type must be array, but is number", json::type_error&);
}

class ValueConversionGetAnArrayExplicitGetToTest : public ::testing::Test {
 protected:
    ValueConversionGetAnArrayExplicitGetToTest() : j(a_reference) {}
    json::array_t a_reference{json(1),     json(1u),       json(2.2),
                              json(false), json("string"), json()};
    json j;
};


TEST_F(ValueConversionGetAnArrayExplicitGetToTest, JsonArray)
{
    json::array_t a{"previous", "value"};
    j.get_to(a);
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayExplicitGetToTest, StdValarrayJson)
{
    std::valarray<json> a{"previous", "value"};
    j.get_to(a);
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayExplicitGetToTest, StdListJson)
{
    std::list<json> a{"previous", "value"};
    j.get_to(a);
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayExplicitGetToTest, StdForwardListJson)
{
    std::forward_list<json> a{"previous", "value"};
    j.get_to(a);
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayExplicitGetToTest, StdVectorJson)
{
    std::vector<json> a{"previous", "value"};
    j.get_to(a);
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayExplicitGetToTest, BuiltInArrays)
{
    const int nbs[] = {0, 1, 2}; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    int nbs2[] = {0, 0, 0}; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)

    json j2 = nbs;
    j2.get_to(nbs2);
    CHECK(std::equal(std::begin(nbs), std::end(nbs), std::begin(nbs2)));
}

TEST_F(ValueConversionGetAnArrayExplicitGetToTest, StdDequeJson)
{
    std::deque<json> a{"previous", "value"};
    j.get_to(a);
    CHECK(json(a) == j);
}

#if JSON_USE_IMPLICIT_CONVERSIONS

class ValueConversionGetAnArrayImplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetAnArrayImplicitTest() : j(a_reference) {}
    json::array_t a_reference{json(1),     json(1u),       json(2.2),
                              json(false), json("string"), json()};
    json j;
};


TEST_F(ValueConversionGetAnArrayImplicitTest, JsonArray)
{
    json::array_t a = j;
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayImplicitTest, StdListJson)
{
    std::list<json> a = j;
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayImplicitTest, StdForwardListJson)
{
    std::forward_list<json> a = j;
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayImplicitTest, StdVectorJson)
{
    std::vector<json> a = j;
    CHECK(json(a) == j);
}

TEST_F(ValueConversionGetAnArrayImplicitTest, StdDequeJson)
{
    std::deque<json> a = j;
    CHECK(json(a) == j);
}

#endif

class ValueConversionGetAStringExplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetAStringExplicitTest() : j(s_reference) {}
    json::string_t s_reference{"Hello world"};
    json j;
};


TEST_F(ValueConversionGetAStringExplicitTest, StringT)
{
    json::string_t s = j.get<json::string_t>();
    CHECK(json(s) == j);
}

TEST_F(ValueConversionGetAStringExplicitTest, StdString)
{
    std::string s = j.get<std::string>();
    CHECK(json(s) == j);
}

#if defined(JSON_HAS_CPP_17)

TEST_F(ValueConversionGetAStringExplicitTest, StdStringView)
{
    std::string_view s = j.get<std::string_view>();
    CHECK(json(s) == j);
}

#endif

TEST_F(ValueConversionGetAStringExplicitTest, ExceptionInCaseOfANonStringType)
{
    CHECK_THROWS_WITH_AS(
        json(json::value_t::null).get<json::string_t>(),
        "[json.exception.type_error.302] type must be string, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::object).get<json::string_t>(),
        "[json.exception.type_error.302] type must be string, but is object", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::array).get<json::string_t>(),
        "[json.exception.type_error.302] type must be string, but is array", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::boolean).get<json::string_t>(),
                         "[json.exception.type_error.302] type must be string, "
                         "but is boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_integer).get<json::string_t>(),
        "[json.exception.type_error.302] type must be string, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_unsigned).get<json::string_t>(),
        "[json.exception.type_error.302] type must be string, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_float).get<json::string_t>(),
        "[json.exception.type_error.302] type must be string, but is number", json::type_error&);
}

#if defined(JSON_HAS_CPP_17)

TEST_F(ValueConversionGetAStringExplicitTest, ExceptionInCaseOfANonStringTypeUsingStringView)
{
    CHECK_THROWS_WITH_AS(json(json::value_t::null).get<std::string_view>(),
                         "[json.exception.type_error.302] type must be string, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::object).get<std::string_view>(),
                         "[json.exception.type_error.302] type must be string, but is object", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::array).get<std::string_view>(),
                         "[json.exception.type_error.302] type must be string, but is array", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::boolean).get<std::string_view>(),
                         "[json.exception.type_error.302] type must be string, but is boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::number_integer).get<std::string_view>(),
                         "[json.exception.type_error.302] type must be string, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::number_unsigned).get<std::string_view>(),
                         "[json.exception.type_error.302] type must be string, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::number_float).get<std::string_view>(),
                         "[json.exception.type_error.302] type must be string, but is number", json::type_error&);
}

#endif

class ValueConversionGetAStringExplicitGetToTest : public ::testing::Test {
 protected:
    ValueConversionGetAStringExplicitGetToTest() : j(s_reference) {}
    json::string_t s_reference{"Hello world"};
    json j;
};


TEST_F(ValueConversionGetAStringExplicitGetToTest, StringT)
{
    json::string_t s = "previous value";
    j.get_to(s);
    CHECK(json(s) == j);
}

TEST_F(ValueConversionGetAStringExplicitGetToTest, StdString)
{
    std::string s = "previous value";
    j.get_to(s);
    CHECK(json(s) == j);
}

#if defined(JSON_HAS_CPP_17)

TEST_F(ValueConversionGetAStringExplicitGetToTest, StdStringView)
{
    std::string s = "previous value";
    std::string_view sv = s;
    j.get_to(sv);
    CHECK(json(sv) == j);
}

#endif

TEST(ValueConversionTest, GetNullExplicit)
{
    std::nullptr_t n = nullptr;
    json j(n);

    auto n2 = j.get<std::nullptr_t>();
    CHECK(n2 == n);

    CHECK_THROWS_WITH_AS(json(json::value_t::string).get<std::nullptr_t>(),
                         "[json.exception.type_error.302] type must be null, but is string", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::object).get<std::nullptr_t>(),
                         "[json.exception.type_error.302] type must be null, but is object", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::array).get<std::nullptr_t>(),
                         "[json.exception.type_error.302] type must be null, but is array", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::boolean).get<std::nullptr_t>(),
                         "[json.exception.type_error.302] type must be null, but is boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::number_integer).get<std::nullptr_t>(),
                         "[json.exception.type_error.302] type must be null, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::number_unsigned).get<std::nullptr_t>(),
                         "[json.exception.type_error.302] type must be null, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::number_float).get<std::nullptr_t>(),
                         "[json.exception.type_error.302] type must be null, but is number", json::type_error&);
}

#if JSON_USE_IMPLICIT_CONVERSIONS

class ValueConversionGetAStringImplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetAStringImplicitTest() : j(s_reference) {}
    json::string_t s_reference{"Hello world"};
    json j;
};


TEST_F(ValueConversionGetAStringImplicitTest, StringT)
{
    json::string_t s = j;
    CHECK(json(s) == j);
}

#if defined(JSON_HAS_CPP_17)

TEST_F(ValueConversionGetAStringImplicitTest, StdStringView)
{
    std::string_view s = j.get<std::string_view>();
    CHECK(json(s) == j);
}

#endif

TEST_F(ValueConversionGetAStringImplicitTest, StdString)
{
    std::string s = j;
    CHECK(json(s) == j);
}

#endif

class ValueConversionGetABooleanExplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetABooleanExplicitTest() : j(b_reference) {}
    json::boolean_t b_reference{true};
    json j;
};


TEST_F(ValueConversionGetABooleanExplicitTest, BooleanT)
{
    auto b = j.get<json::boolean_t>();
    CHECK(json(b) == j);
}

TEST_F(ValueConversionGetABooleanExplicitTest, Uint8)
{
    auto n = j.get<uint8_t>();
    CHECK(n == 1);
}

TEST_F(ValueConversionGetABooleanExplicitTest, Bool)
{
    bool b = j.get<bool>();
    CHECK(json(b) == j);
}

TEST_F(ValueConversionGetABooleanExplicitTest, ExceptionInCaseOfANonNumberType)
{
    CHECK_THROWS_AS(json(json::value_t::string).get<uint8_t>(),
                    json::type_error&);

    CHECK_THROWS_WITH_AS(
        json(json::value_t::null).get<json::boolean_t>(),
        "[json.exception.type_error.302] type must be boolean, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::object).get<json::boolean_t>(),
                         "[json.exception.type_error.302] type must be boolean, "
                         "but is object", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::array).get<json::boolean_t>(),
        "[json.exception.type_error.302] type must be boolean, but is array", json::type_error&);
    CHECK_THROWS_WITH_AS(json(json::value_t::string).get<json::boolean_t>(),
                         "[json.exception.type_error.302] type must be boolean, "
                         "but is string", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_integer).get<json::boolean_t>(),
        "[json.exception.type_error.302] type must be boolean, but is "
        "number", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_unsigned).get<json::boolean_t>(),
        "[json.exception.type_error.302] type must be boolean, but is "
        "number", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::number_float).get<json::boolean_t>(),
        "[json.exception.type_error.302] type must be boolean, but is "
        "number", json::type_error&);
}

#if JSON_USE_IMPLICIT_CONVERSIONS

class ValueConversionGetABooleanImplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetABooleanImplicitTest() : j(b_reference) {}
    json::boolean_t b_reference{true};
    json j;
};


TEST_F(ValueConversionGetABooleanImplicitTest, BooleanT)
{
    json::boolean_t b = j;
    CHECK(json(b) == j);
}

TEST_F(ValueConversionGetABooleanImplicitTest, Bool)
{
    bool b = j;
    CHECK(json(b) == j);
}

#endif

class ValueConversionGetAnIntegerNumberExplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetAnIntegerNumberExplicitTest() : j(n_reference), j_unsigned(n_unsigned_reference) {}
    json::number_integer_t n_reference{42};
    json j;
    json::number_unsigned_t n_unsigned_reference{42u};
    json j_unsigned;
};


TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, NumberIntegerT)
{
    auto n = j.get<json::number_integer_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, NumberUnsignedT)
{
    auto n = j_unsigned.get<json::number_unsigned_t>();
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Short)
{
    auto n = j.get<short>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, UnsignedShort)
{
    auto n = j.get<unsigned short>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int)
{
    int n = j.get<int>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, UnsignedInt)
{
    auto n = j.get<unsigned int>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Long)
{
    long n = j.get<long>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, UnsignedLong)
{
    auto n = j.get<unsigned long>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, LongLong)
{
    auto n = j.get<long long>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, UnsignedLongLong)
{
    auto n = j.get<unsigned long long>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int8)
{
    auto n = j.get<int8_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int16)
{
    auto n = j.get<int16_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int32)
{
    auto n = j.get<int32_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int64)
{
    auto n = j.get<int64_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int8FastT)
{
    auto n = j.get<int_fast8_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int16FastT)
{
    auto n = j.get<int_fast16_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int32FastT)
{
    auto n = j.get<int_fast32_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int64FastT)
{
    auto n = j.get<int_fast64_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int8LeastT)
{
    auto n = j.get<int_least8_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int16LeastT)
{
    auto n = j.get<int_least16_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int32LeastT)
{
    auto n = j.get<int_least32_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Int64LeastT)
{
    auto n = j.get<int_least64_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint8)
{
    auto n = j.get<uint8_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint16)
{
    auto n = j.get<uint16_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint32)
{
    auto n = j.get<uint32_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint64)
{
    auto n = j.get<uint64_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint8FastT)
{
    auto n = j.get<uint_fast8_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint16FastT)
{
    auto n = j.get<uint_fast16_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint32FastT)
{
    auto n = j.get<uint_fast32_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint64FastT)
{
    auto n = j.get<uint_fast64_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint8LeastT)
{
    auto n = j.get<uint_least8_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint16LeastT)
{
    auto n = j.get<uint_least16_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint32LeastT)
{
    auto n = j.get<uint_least32_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, Uint64LeastT)
{
    auto n = j.get<uint_least64_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberExplicitTest, ExceptionInCaseOfANonNumberType)
{
    CHECK_THROWS_WITH_AS(
        json(json::value_t::null).get<json::number_integer_t>(),
        "[json.exception.type_error.302] type must be number, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::object).get<json::number_integer_t>(),
        "[json.exception.type_error.302] type must be number, but is object", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::array).get<json::number_integer_t>(),
        "[json.exception.type_error.302] type must be number, but is array", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::string).get<json::number_integer_t>(),
        "[json.exception.type_error.302] type must be number, but is string", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::boolean).get<json::number_integer_t>(),
        "[json.exception.type_error.302] type must be number, but is "
        "boolean", json::type_error&);

    CHECK_NOTHROW(
        json(json::value_t::number_float).get<json::number_integer_t>());
    CHECK_NOTHROW(
        json(json::value_t::number_float).get<json::number_unsigned_t>());
}

#if JSON_USE_IMPLICIT_CONVERSIONS

class ValueConversionGetAnIntegerNumberImplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetAnIntegerNumberImplicitTest() : j(n_reference), j_unsigned(n_unsigned_reference) {}
    json::number_integer_t n_reference{42};
    json j;
    json::number_unsigned_t n_unsigned_reference{42u};
    json j_unsigned;
};


TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, NumberIntegerT)
{
    auto n = j.get<json::number_integer_t>();
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, NumberUnsignedT)
{
    auto n = j_unsigned.get<json::number_unsigned_t>();
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Short)
{
    short n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, UnsignedShort)
{
    unsigned short n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int)
{
    int n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, UnsignedInt)
{
    unsigned int n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Long)
{
    long n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, UnsignedLong)
{
    unsigned long n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, LongLong)
{
    long long n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, UnsignedLongLong)
{
    unsigned long long n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int8)
{
    int8_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int16)
{
    int16_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int32)
{
    int32_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int64)
{
    int64_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int8FastT)
{
    int_fast8_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int16FastT)
{
    int_fast16_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int32FastT)
{
    int_fast32_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int64FastT)
{
    int_fast64_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int8LeastT)
{
    int_least8_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int16LeastT)
{
    int_least16_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int32LeastT)
{
    int_least32_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Int64LeastT)
{
    int_least64_t n = j;
    CHECK(json(n) == j);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint8)
{
    uint8_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint16)
{
    uint16_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint32)
{
    uint32_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint64)
{
    uint64_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint8FastT)
{
    uint_fast8_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint16FastT)
{
    uint_fast16_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint32FastT)
{
    uint_fast32_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint64FastT)
{
    uint_fast64_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint8LeastT)
{
    uint_least8_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint16LeastT)
{
    uint_least16_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint32LeastT)
{
    uint_least32_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

TEST_F(ValueConversionGetAnIntegerNumberImplicitTest, Uint64LeastT)
{
    uint_least64_t n = j_unsigned;
    CHECK(json(n) == j_unsigned);
}

#endif

class ValueConversionGetAFloatingPointNumberExplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetAFloatingPointNumberExplicitTest() : j(n_reference) {}
    json::number_float_t n_reference{42.23};
    json j;
};


TEST_F(ValueConversionGetAFloatingPointNumberExplicitTest, NumberFloatT)
{
    auto n = j.get<json::number_float_t>();
    EXPECT_NEAR(json(n).m_value.number_float, j.m_value.number_float, 0.001);
}

TEST_F(ValueConversionGetAFloatingPointNumberExplicitTest, Float)
{
    auto n = j.get<float>();
    EXPECT_NEAR(json(n).m_value.number_float, j.m_value.number_float, 0.001);
}

TEST_F(ValueConversionGetAFloatingPointNumberExplicitTest, Double)
{
    auto n = j.get<double>();
    EXPECT_NEAR(json(n).m_value.number_float, j.m_value.number_float, 0.001);
}

TEST_F(ValueConversionGetAFloatingPointNumberExplicitTest, ExceptionInCaseOfANonStringType)
{
    CHECK_THROWS_WITH_AS(
        json(json::value_t::null).get<json::number_float_t>(),
        "[json.exception.type_error.302] type must be number, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::object).get<json::number_float_t>(),
        "[json.exception.type_error.302] type must be number, but is object", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::array).get<json::number_float_t>(),
        "[json.exception.type_error.302] type must be number, but is array", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::string).get<json::number_float_t>(),
        "[json.exception.type_error.302] type must be number, but is string", json::type_error&);
    CHECK_THROWS_WITH_AS(
        json(json::value_t::boolean).get<json::number_float_t>(),
        "[json.exception.type_error.302] type must be number, but is "
        "boolean", json::type_error&);

    CHECK_NOTHROW(
        json(json::value_t::number_integer).get<json::number_float_t>());
    CHECK_NOTHROW(
        json(json::value_t::number_unsigned).get<json::number_float_t>());
}

#if JSON_USE_IMPLICIT_CONVERSIONS

class ValueConversionGetAFloatingPointNumberImplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetAFloatingPointNumberImplicitTest() : j(n_reference) {}
    json::number_float_t n_reference{42.23};
    json j;
};


TEST_F(ValueConversionGetAFloatingPointNumberImplicitTest, NumberFloatT)
{
    json::number_float_t n = j;
    EXPECT_NEAR(json(n).m_value.number_float, j.m_value.number_float, 0.001);
}

TEST_F(ValueConversionGetAFloatingPointNumberImplicitTest, Float)
{
    float n = j;
    EXPECT_NEAR(json(n).m_value.number_float, j.m_value.number_float, 0.001);
}

TEST_F(ValueConversionGetAFloatingPointNumberImplicitTest, Double)
{
    double n = j;
    EXPECT_NEAR(json(n).m_value.number_float, j.m_value.number_float, 0.001);
}

#endif

class ValueConversionGetABinaryValueExplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetABinaryValueExplicitTest() : j(n_reference) {}
    json::binary_t n_reference{{1, 2, 3}};
    json j;
};


TEST_F(ValueConversionGetABinaryValueExplicitTest, BinaryT)
{
    json::binary_t b = j.get<json::binary_t>();
    CHECK(*json(b).m_value.binary == *j.m_value.binary);
}

class ValueConversionGetABinaryValueExplicitGetBinaryTest : public ValueConversionGetABinaryValueExplicitTest{};


TEST_F(ValueConversionGetABinaryValueExplicitGetBinaryTest, NonConst)
{
    auto& b = j.get_binary();
    CHECK(*json(b).m_value.binary == *j.m_value.binary);
}

TEST_F(ValueConversionGetABinaryValueExplicitGetBinaryTest, NonConst2)
{
    const json j_const = j;
    const auto& b = j_const.get_binary();
    CHECK(*json(b).m_value.binary == *j.m_value.binary);
}

TEST_F(ValueConversionGetABinaryValueExplicitTest, ExceptionInCaseOfANonStringType)
{
    json j_null(json::value_t::null);
    json j_object(json::value_t::object);
    json j_array(json::value_t::array);
    json j_string(json::value_t::string);
    json j_boolean(json::value_t::boolean);
    const json j_null_const(json::value_t::null);
    const json j_object_const(json::value_t::object);
    const json j_array_const(json::value_t::array);
    const json j_string_const(json::value_t::string);
    const json j_boolean_const(json::value_t::boolean);

    CHECK_THROWS_WITH_AS(j_null.get<json::binary_t>(),
                         "[json.exception.type_error.302] type must be binary, but is null",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_object.get<json::binary_t>(),
                         "[json.exception.type_error.302] type must be binary, but is object",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_array.get<json::binary_t>(),
                         "[json.exception.type_error.302] type must be binary, but is array",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_string.get<json::binary_t>(),
                         "[json.exception.type_error.302] type must be binary, but is string",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_boolean.get<json::binary_t>(),
                         "[json.exception.type_error.302] type must be binary, but is boolean",
                         json::type_error&);

    CHECK_THROWS_WITH_AS(j_null_const.get<json::binary_t>(),
                         "[json.exception.type_error.302] type must be binary, but is null",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_object_const.get<json::binary_t>(),
                         "[json.exception.type_error.302] type must be binary, but is object",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_array_const.get<json::binary_t>(),
                         "[json.exception.type_error.302] type must be binary, but is array",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_string_const.get<json::binary_t>(),
                         "[json.exception.type_error.302] type must be binary, but is string",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_boolean_const.get<json::binary_t>(),
                         "[json.exception.type_error.302] type must be binary, but is boolean",
                         json::type_error&);

    CHECK_THROWS_WITH_AS(j_null.get_binary(),
                         "[json.exception.type_error.302] type must be binary, but is null",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_object.get_binary(),
                         "[json.exception.type_error.302] type must be binary, but is object",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_array.get_binary(),
                         "[json.exception.type_error.302] type must be binary, but is array",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_string.get_binary(),
                         "[json.exception.type_error.302] type must be binary, but is string",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_boolean.get_binary(),
                         "[json.exception.type_error.302] type must be binary, but is boolean",
                         json::type_error&);

    CHECK_THROWS_WITH_AS(j_null_const.get_binary(),
                         "[json.exception.type_error.302] type must be binary, but is null",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_object_const.get_binary(),
                         "[json.exception.type_error.302] type must be binary, but is object",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_array_const.get_binary(),
                         "[json.exception.type_error.302] type must be binary, but is array",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_string_const.get_binary(),
                         "[json.exception.type_error.302] type must be binary, but is string",
                         json::type_error&);
    CHECK_THROWS_WITH_AS(j_boolean_const.get_binary(),
                         "[json.exception.type_error.302] type must be binary, but is boolean",
                         json::type_error&);
}

#if JSON_USE_IMPLICIT_CONVERSIONS

class ValueConversionGetABinaryValueImplicitTest : public ::testing::Test {
 protected:
    ValueConversionGetABinaryValueImplicitTest() : j(n_reference) {}
    json::binary_t n_reference{{1, 2, 3}};
    json j;
};


TEST_F(ValueConversionGetABinaryValueImplicitTest, BinaryT)
{
    json::binary_t b = j;
    CHECK(*json(b).m_value.binary == *j.m_value.binary);
}

#endif

TEST(ValueConversionTest, GetAnEnum)
{
    enum c_enum { value_1, value_2 };
    enum class cpp_enum { value_1, value_2 };

    CHECK(json(value_1).get<c_enum>() == value_1);
    CHECK(json(cpp_enum::value_1).get<cpp_enum>() == cpp_enum::value_1);
}




class ValueConversionMoreInvolvedConversionsObjectLikeSTLContainersTest : public ::testing::Test {
 protected:
    json j1 = {{"one", 1}, {"two", 2}, {"three", 3}};
    json j2 = {{"one", 1u}, {"two", 2u}, {"three", 3u}};
    json j3 = {{"one", 1.1}, {"two", 2.2}, {"three", 3.3}};
    json j4 = {{"one", true}, {"two", false}, {"three", true}};
    json j5 = {{"one", "eins"}, {"two", "zwei"}, {"three", "drei"}};
};


TEST_F(ValueConversionMoreInvolvedConversionsObjectLikeSTLContainersTest, StdMap)
{
    j1.get<std::map<std::string, int>>();
    j2.get<std::map<std::string, unsigned int>>();
    j3.get<std::map<std::string, double>>();
    j4.get<std::map<std::string, bool>>();
    j5.get<std::map<std::string, std::string>>();
}

TEST_F(ValueConversionMoreInvolvedConversionsObjectLikeSTLContainersTest, StdUnorderedMap)
{
    j1.get<std::unordered_map<std::string, int>>();
    j2.get<std::unordered_map<std::string, unsigned int>>();
    j3.get<std::unordered_map<std::string, double>>();
    j4.get<std::unordered_map<std::string, bool>>();
    j5.get<std::unordered_map<std::string, std::string>>();
    // CHECK(m5["one"] == "eins");
}

TEST_F(ValueConversionMoreInvolvedConversionsObjectLikeSTLContainersTest, StdMultimap)
{
    j1.get<std::multimap<std::string, int>>();
    j2.get<std::multimap<std::string, unsigned int>>();
    j3.get<std::multimap<std::string, double>>();
    j4.get<std::multimap<std::string, bool>>();
    j5.get<std::multimap<std::string, std::string>>();
    // CHECK(m5["one"] == "eins");
}

TEST_F(ValueConversionMoreInvolvedConversionsObjectLikeSTLContainersTest, StdUnorderedMultimap)
{
    j1.get<std::unordered_multimap<std::string, int>>();
    j2.get<std::unordered_multimap<std::string, unsigned int>>();
    j3.get<std::unordered_multimap<std::string, double>>();
    j4.get<std::unordered_multimap<std::string, bool>>();
    j5.get<std::unordered_multimap<std::string, std::string>>();
    // CHECK(m5["one"] == "eins");
}

TEST_F(ValueConversionMoreInvolvedConversionsObjectLikeSTLContainersTest, ExceptionInCaseOfANonObjectType)
{
    CHECK_THROWS_WITH_AS(
        (json().get<std::map<std::string, int>>()),
        "[json.exception.type_error.302] type must be object, but is null", json::type_error&);
}

class ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest : public ::testing::Test {
 protected:
    json j1 = {1, 2, 3, 4};
    json j2 = {1u, 2u, 3u, 4u};
    json j3 = {1.2, 2.3, 3.4, 4.5};
    json j4 = {true, false, true};
    json j5 = {"one", "two", "three"};
};


TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest, StdList)
{
    j1.get<std::list<int>>();
    j2.get<std::list<unsigned int>>();
    j3.get<std::list<double>>();
    j4.get<std::list<bool>>();
    j5.get<std::list<std::string>>();
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest, StdForwardList)
{
    j1.get<std::forward_list<int>>();
    j2.get<std::forward_list<unsigned int>>();
    j3.get<std::forward_list<double>>();
    j4.get<std::forward_list<bool>>();
    j5.get<std::forward_list<std::string>>();
}

class ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersStdArrayTest : public ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest{};


TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersStdArrayTest, BasicTest)
{
    j1.get<std::array<int, 4>>();
    j2.get<std::array<unsigned int, 3>>();
    j3.get<std::array<double, 4>>();
    j4.get<std::array<bool, 3>>();
    j5.get<std::array<std::string, 3>>();
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersStdArrayTest, StdArrayIsLargerThanJSON)
{
    std::array<int, 6> arr6 = {{1, 2, 3, 4, 5, 6}};
    CHECK_THROWS_WITH_AS(j1.get_to(arr6), "[json.exception.out_of_range.401] "
                         "array index 4 is out of range", json::out_of_range&);
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersStdArrayTest, StdArrayIsSmallerThanJSON)
{
    std::array<int, 2> arr2 = {{8, 9}};
    j1.get_to(arr2);
    CHECK(arr2[0] == 1);
    CHECK(arr2[1] == 2);
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest, StdValarray)
{
    j1.get<std::valarray<int>>();
    j2.get<std::valarray<unsigned int>>();
    j3.get<std::valarray<double>>();
    j4.get<std::valarray<bool>>();
    j5.get<std::valarray<std::string>>();
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest, StdVector)
{
    j1.get<std::vector<int>>();
    j2.get<std::vector<unsigned int>>();
    j3.get<std::vector<double>>();
    j4.get<std::vector<bool>>();
    j5.get<std::vector<std::string>>();
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest, StdDeque)
{
    j1.get<std::deque<int>>();
    j2.get<std::deque<unsigned int>>();
    j2.get<std::deque<double>>();
    j4.get<std::deque<bool>>();
    j5.get<std::deque<std::string>>();
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest, StdSet)
{
    j1.get<std::set<int>>();
    j2.get<std::set<unsigned int>>();
    j3.get<std::set<double>>();
    j4.get<std::set<bool>>();
    j5.get<std::set<std::string>>();
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest, StdUnorderedSet)
{
    j1.get<std::unordered_set<int>>();
    j2.get<std::unordered_set<unsigned int>>();
    j3.get<std::unordered_set<double>>();
    j4.get<std::unordered_set<bool>>();
    j5.get<std::unordered_set<std::string>>();
}

class ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersStdMapArrayOfPairsTest : public ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest {
 protected:
    std::map<int, int> m{{0, 1}, {1, 2}, {2, 3}};
};


TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersStdMapArrayOfPairsTest, BasicTest)
{
    json j6 = m;

    auto m2 = j6.get<std::map<int, int>>();
    CHECK(m == m2);

    json j7 = {0, 1, 2, 3};
    json j8 = 2;
    CHECK_THROWS_WITH_AS((j7.get<std::map<int, int>>()),
                         "[json.exception.type_error.302] type must be array, "
                         "but is number", json::type_error&);
    CHECK_THROWS_WITH_AS((j8.get<std::map<int, int>>()),
                         "[json.exception.type_error.302] type must be array, "
                         "but is number", json::type_error&);
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersStdMapArrayOfPairsTest, SuperfluousEntries)
{
    json j9 = {{0, 1, 2}, {1, 2, 3}, {2, 3, 4}};
    auto m2 = j9.get<std::map<int, int>>();
    CHECK(m == m2);
}

class ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersStdUnorderedMapArrayOfPairsTest : public ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest {
 protected:
    std::unordered_map<int, int> m{{0, 1}, {1, 2}, {2, 3}};
};


TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersStdUnorderedMapArrayOfPairsTest, BasicTest)
{
    json j6 = m;

    auto m2 = j6.get<std::unordered_map<int, int>>();
    CHECK(m == m2);

    json j7 = {0, 1, 2, 3};
    json j8 = 2;
    CHECK_THROWS_WITH_AS((j7.get<std::unordered_map<int, int>>()),
                         "[json.exception.type_error.302] type must be array, "
                         "but is number", json::type_error&);
    CHECK_THROWS_WITH_AS((j8.get<std::unordered_map<int, int>>()),
                         "[json.exception.type_error.302] type must be array, "
                         "but is number", json::type_error&);
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersStdUnorderedMapArrayOfPairsTest, SuperfluousEntries)
{
    json j9{{0, 1, 2}, {1, 2, 3}, {2, 3, 4}};
    auto m2 = j9.get<std::unordered_map<int, int>>();
    CHECK(m == m2);
}

TEST_F(ValueConversionMoreInvolvedConversionsArrayLikeSTLContainersTest, ExceptionInCaseOfANonObjectType)
{
    // does type really must be an array? or it rather must not be null?
    // that's what I thought when other test like this one broke
    CHECK_THROWS_WITH_AS(
        (json().get<std::list<int>>()),
        "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(
        (json().get<std::vector<int>>()),
        "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(
        (json().get<std::vector<json>>()),
        "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(
        (json().get<std::list<json>>()),
        "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(
        (json().get<std::valarray<int>>()),
        "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
    CHECK_THROWS_WITH_AS(
        (json().get<std::map<int, int>>()),
        "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
}

enum class cards {kreuz, pik, herz, karo};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays) - false positive
WPI_JSON_SERIALIZE_ENUM(cards,
{
    {cards::kreuz, "kreuz"},
    {cards::pik, "pik"},
    {cards::pik, "puk"},  // second entry for cards::puk; will not be used
    {cards::herz, "herz"},
    {cards::karo, "karo"}
})

enum TaskState
{
    TS_STOPPED,
    TS_RUNNING,
    TS_COMPLETED,
    TS_INVALID = -1,
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays) - false positive
WPI_JSON_SERIALIZE_ENUM(TaskState,
{
    {TS_INVALID, nullptr},
    {TS_STOPPED, "stopped"},
    {TS_RUNNING, "running"},
    {TS_COMPLETED, "completed"},
})




TEST(JSONToEnumMappingTest, EnumClass)
{
    // enum -> json
    CHECK(json(cards::kreuz) == "kreuz");
    CHECK(json(cards::pik) == "pik");
    CHECK(json(cards::herz) == "herz");
    CHECK(json(cards::karo) == "karo");

    // json -> enum
    CHECK(cards::kreuz == json("kreuz"));
    CHECK(cards::pik == json("pik"));
    CHECK(cards::herz == json("herz"));
    CHECK(cards::karo == json("karo"));

    // invalid json -> first enum
    CHECK(cards::kreuz == json("what?").get<cards>());
}

TEST(JSONToEnumMappingTest, TraditionalEnum)
{
    // enum -> json
    CHECK(json(TS_STOPPED) == "stopped");
    CHECK(json(TS_RUNNING) == "running");
    CHECK(json(TS_COMPLETED) == "completed");
    CHECK(json(TS_INVALID) == json());

    // json -> enum
    CHECK(TS_STOPPED == json("stopped"));
    CHECK(TS_RUNNING == json("running"));
    CHECK(TS_COMPLETED == json("completed"));
    CHECK(TS_INVALID == json());

    // invalid json -> first enum
    CHECK(TS_INVALID == json("what?").get<TaskState>());
}


