//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

#define JSON_TESTS_PRIVATE
#include "unit-json.h"
using wpi::json;

#include <deque>
#include <forward_list>
#include <fstream>
#include <list>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <valarray>







TEST(ConstructorsCreateAnEmptyValueWithAGivenTypeTest, Null)
{
    auto t = json::value_t::null;
    json j(t);
    CHECK(j.type() == t);
}

TEST(ConstructorsCreateAnEmptyValueWithAGivenTypeTest, Discarded)
{
    auto t = json::value_t::discarded;
    json j(t);
    CHECK(j.type() == t);
}

TEST(ConstructorsCreateAnEmptyValueWithAGivenTypeTest, Object)
{
    auto t = json::value_t::object;
    json j(t);
    CHECK(j.type() == t);
}

TEST(ConstructorsCreateAnEmptyValueWithAGivenTypeTest, Array)
{
    auto t = json::value_t::array;
    json j(t);
    CHECK(j.type() == t);
}

TEST(ConstructorsCreateAnEmptyValueWithAGivenTypeTest, Boolean)
{
    auto t = json::value_t::boolean;
    json j(t);
    CHECK(j.type() == t);
    CHECK(j == false);
}

TEST(ConstructorsCreateAnEmptyValueWithAGivenTypeTest, String)
{
    auto t = json::value_t::string;
    json j(t);
    CHECK(j.type() == t);
    CHECK(j == "");
}

TEST(ConstructorsCreateAnEmptyValueWithAGivenTypeTest, NumberInteger)
{
    auto t = json::value_t::number_integer;
    json j(t);
    CHECK(j.type() == t);
    CHECK(j == 0);
}

TEST(ConstructorsCreateAnEmptyValueWithAGivenTypeTest, NumberUnsigned)
{
    auto t = json::value_t::number_unsigned;
    json j(t);
    CHECK(j.type() == t);
    CHECK(j == 0);
}

TEST(ConstructorsCreateAnEmptyValueWithAGivenTypeTest, NumberFloat)
{
    auto t = json::value_t::number_float;
    json j(t);
    CHECK(j.type() == t);
    CHECK(j == 0.0);
}

TEST(ConstructorsCreateAnEmptyValueWithAGivenTypeTest, Binary)
{
    auto t = json::value_t::binary;
    json j(t);
    CHECK(j.type() == t);
    CHECK(j == json::binary({}));
}




TEST(ConstructorsCreateANullObjectImplicitlyTest, NoParameter)
{
    json j{};
    CHECK(j.type() == json::value_t::null);
}




TEST(ConstructorsCreateANullObjectExplicitlyTest, Parameter)
{
    json j(nullptr);
    CHECK(j.type() == json::value_t::null);
}




TEST(ConstructorsCreateAnObjectExplicitTest, EmptyObject)
{
    json::object_t o;
    json j(o);
    CHECK(j.type() == json::value_t::object);
}

TEST(ConstructorsCreateAnObjectExplicitTest, FilledObject)
{
    json::object_t o {{"a", json(1)}, {"b", json(1u)}, {"c", json(2.2)}, {"d", json(false)}, {"e", json("string")}, {"f", json()}};
    json j(o);
    CHECK(j.type() == json::value_t::object);
}

class ConstructorsCreateAnObjectImplicitTest : public ::testing::Test {
 protected:
    ConstructorsCreateAnObjectImplicitTest() : o_reference({{"a", json(1)}, {"b", json(1u)}, {"c", json(2.2)}, {"d", json(false)}, {"e", json("string")}, {"f", json()}}), j_reference(o_reference) {}

    // reference object
    json::object_t o_reference;
    json j_reference;
};


TEST_F(ConstructorsCreateAnObjectImplicitTest, StdMapStringJson)
{
    std::map<json::string_t, json> o {{"a", json(1)}, {"b", json(1u)}, {"c", json(2.2)}, {"d", json(false)}, {"e", json("string")}, {"f", json()}};
    json j(o);
    CHECK(j.type() == json::value_t::object);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnObjectImplicitTest, StdMapStdStringStdString600)
{
    std::map<std::string, std::string> m;
    m["a"] = "b";
    m["c"] = "d";
    m["e"] = "f";

    json j(m);
    CHECK((j.get<decltype(m)>() == m));
}

TEST_F(ConstructorsCreateAnObjectImplicitTest, StdMapConstCharPointerJson)
{
    std::map<const char*, json> o {{"a", json(1)}, {"b", json(1u)}, {"c", json(2.2)}, {"d", json(false)}, {"e", json("string")}, {"f", json()}};
    json j(o);
    CHECK(j.type() == json::value_t::object);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnObjectImplicitTest, StdMultimapStringJson)
{
    std::multimap<json::string_t, json> o {{"a", json(1)}, {"b", json(1u)}, {"c", json(2.2)}, {"d", json(false)}, {"e", json("string")}, {"f", json()}};
    json j(o);
    CHECK(j.type() == json::value_t::object);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnObjectImplicitTest, StdUnorderedMapStringJson)
{
    std::unordered_map<json::string_t, json> o {{"a", json(1)}, {"b", json(1u)}, {"c", json(2.2)}, {"d", json(false)}, {"e", json("string")}, {"f", json()}};
    json j(o);
    CHECK(j.type() == json::value_t::object);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnObjectImplicitTest, StdUnorderedMultimapStringJson)
{
    std::unordered_multimap<json::string_t, json> o {{"a", json(1)}, {"b", json(1u)}, {"c", json(2.2)}, {"d", json(false)}, {"e", json("string")}, {"f", json()}};
    json j(o);
    CHECK(j.type() == json::value_t::object);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnObjectImplicitTest, AssociativeContainerLiteral)
{
    json j({{"a", json(1)}, {"b", json(1u)}, {"c", json(2.2)}, {"d", json(false)}, {"e", json("string")}, {"f", json()}});
    CHECK(j.type() == json::value_t::object);
    CHECK(j == j_reference);
}




TEST(ConstructorsCreateAnArrayExplicitTest, EmptyArray)
{
    json::array_t a;
    json j(a);
    CHECK(j.type() == json::value_t::array);
}

TEST(ConstructorsCreateAnArrayExplicitTest, FilledArray)
{
    json::array_t a {json(1), json(1u), json(2.2), json(false), json("string"), json()};
    json j(a);
    CHECK(j.type() == json::value_t::array);
}

class ConstructorsCreateAnArrayImplicitTest : public ::testing::Test {
 protected:
    ConstructorsCreateAnArrayImplicitTest() : a_reference({json(1), json(1u), json(2.2), json(false), json("string"), json()}), j_reference(a_reference) {}

    // reference array
    json::array_t a_reference;
    json j_reference;
};


TEST_F(ConstructorsCreateAnArrayImplicitTest, StdListJson)
{
    std::list<json> a {json(1), json(1u), json(2.2), json(false), json("string"), json()};
    json j(a);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdPair)
{
    std::pair<float, std::string> p{1.0f, "string"};
    json j(p);

    CHECK(j.type() == json::value_t::array);
    CHECK(j.get<decltype(p)>() == p);
    REQUIRE(j.size() == 2);
    CHECK(j[0] == std::get<0>(p));
    CHECK(j[1] == std::get<1>(p));
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdPairWithDiscardedValues)
{
    json j{1, 2.0, "string"};

    const auto p = j.get<std::pair<int, float>>();
    CHECK(p.first == j[0]);
    CHECK(p.second == j[1]);
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdTuple)
{
    const auto t = std::make_tuple(1.0, std::string{"string"}, 42, std::vector<int> {0, 1});
    json j(t);

    CHECK(j.type() == json::value_t::array);
    REQUIRE(j.size() == 4);
    CHECK(j.get<decltype(t)>() == t);
    CHECK(j[0] == std::get<0>(t));
    CHECK(j[1] == std::get<1>(t));
    CHECK(j[2] == std::get<2>(t));
    CHECK(j[3][0] == 0);
    CHECK(j[3][1] == 1);
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdTupleWithDiscardedValues)
{
    json j{1, 2.0, "string", 42};

    const auto t = j.get<std::tuple<int, float, std::string>>();
    CHECK(std::get<0>(t) == j[0]);
    CHECK(std::get<1>(t) == j[1]);
    CHECK(std::get<2>(t) == j[2]);
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdPairTupleArrayFailures)
{
    json j{1};

    CHECK_THROWS_WITH_AS((j.get<std::pair<int, int>>()), "[json.exception.out_of_range.401] array index 1 is out of range", json::out_of_range&);
    CHECK_THROWS_WITH_AS((j.get<std::tuple<int, int>>()), "[json.exception.out_of_range.401] array index 1 is out of range", json::out_of_range&);
    CHECK_THROWS_WITH_AS((j.get<std::array<int, 3>>()), "[json.exception.out_of_range.401] array index 1 is out of range", json::out_of_range&);
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdForwardListJson)
{
    std::forward_list<json> a {json(1), json(1u), json(2.2), json(false), json("string"), json()};
    json j(a);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdArrayJson6)
{
    std::array<json, 6> a {{json(1), json(1u), json(2.2), json(false), json("string"), json()}};
    json j(a);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == j_reference);

    const auto a2 = j.get<std::array<json, 6>>();
    CHECK(a2 == a);
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdValarrayInt)
{
    std::valarray<int> va = {1, 2, 3, 4, 5};
    json j(va);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 2, 3, 4, 5}));

    auto jva = j.get<std::valarray<int>>();
    CHECK(jva.size() == va.size());
    for (size_t i = 0; i < jva.size(); ++i)
    {
        CHECK(va[i] == jva[i]);
    }
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdValarrayDouble)
{
    std::valarray<double> va = {1.2, 2.3, 3.4, 4.5, 5.6};
    json j(va);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1.2, 2.3, 3.4, 4.5, 5.6}));

    auto jva = j.get<std::valarray<double>>();
    CHECK(jva.size() == va.size());
    for (size_t i = 0; i < jva.size(); ++i)
    {
        CHECK(va[i] == jva[i]);
    }
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdVectorJson)
{
    std::vector<json> a {json(1), json(1u), json(2.2), json(false), json("string"), json()};
    json j(a);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdDequeJson)
{
    std::deque<json> a {json(1), json(1u), json(2.2), json(false), json("string"), json()};
    json j(a);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdSetJson)
{
    std::set<json> a {json(1), json(1u), json(2.2), json(false), json("string"), json()};
    json j(a);
    CHECK(j.type() == json::value_t::array);
    // we cannot really check for equality here
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, StdUnorderedSetJson)
{
    std::unordered_set<json> a {json(1), json(1u), json(2.2), json(false), json("string"), json()};
    json j(a);
    CHECK(j.type() == json::value_t::array);
    // we cannot really check for equality here
}

TEST_F(ConstructorsCreateAnArrayImplicitTest, SequenceContainerLiteral)
{
    json j({json(1), json(1u), json(2.2), json(false), json("string"), json()});
    CHECK(j.type() == json::value_t::array);
    CHECK(j == j_reference);
}




TEST(ConstructorsCreateAStringExplicitTest, EmptyString)
{
    json::string_t s;
    json j(s);
    CHECK(j.type() == json::value_t::string);
}

TEST(ConstructorsCreateAStringExplicitTest, FilledString)
{
    json::string_t s {"Hello world"};
    json j(s);
    CHECK(j.type() == json::value_t::string);
}

class ConstructorsCreateAStringImplicitTest : public ::testing::Test {
 protected:
    ConstructorsCreateAStringImplicitTest() : s_reference({"Hello world"}), j_reference(s_reference) {}

    // reference string
    json::string_t s_reference;
    json j_reference;
};


TEST_F(ConstructorsCreateAStringImplicitTest, StdString)
{
    std::string s {"Hello world"};
    json j(s);
    CHECK(j.type() == json::value_t::string);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAStringImplicitTest, Char)
{
    char s[] {"Hello world"}; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    json j(s);
    CHECK(j.type() == json::value_t::string);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAStringImplicitTest, ConstCharPointer)
{
    const char* s {"Hello world"};
    json j(s);
    CHECK(j.type() == json::value_t::string);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAStringImplicitTest, StringLiteral)
{
    json j("Hello world");
    CHECK(j.type() == json::value_t::string);
    CHECK(j == j_reference);
}




TEST(ConstructorsCreateABooleanExplicitTest, EmptyBoolean)
{
    json::boolean_t b{};
    json j(b);
    CHECK(j.type() == json::value_t::boolean);
}

TEST(ConstructorsCreateABooleanExplicitTest, FilledBooleanTrue)
{
    json j(true);
    CHECK(j.type() == json::value_t::boolean);
}

TEST(ConstructorsCreateABooleanExplicitTest, FilledBooleanFalse)
{
    json j(false);
    CHECK(j.type() == json::value_t::boolean);
}

TEST(ConstructorsCreateABooleanExplicitTest, FromStdVectorBoolReference)
{
    std::vector<bool> v{true};
    json j(v[0]);
    CHECK(std::is_same<decltype(v[0]), std::vector<bool>::reference>::value);
    CHECK(j.type() == json::value_t::boolean);
}

TEST(ConstructorsCreateABooleanExplicitTest, FromStdVectorBoolConstReference)
{
    const std::vector<bool> v{true};
    json j(v[0]);
    CHECK(std::is_same<decltype(v[0]), std::vector<bool>::const_reference>::value);
    CHECK(j.type() == json::value_t::boolean);
}




TEST(ConstructorsCreateABinaryExplicitTest, EmptyBinary)
{
    json::binary_t b{};
    json j(b);
    CHECK(j.type() == json::value_t::binary);
}

TEST(ConstructorsCreateABinaryExplicitTest, FilledBinary)
{
    json::binary_t b({1, 2, 3});
    json j(b);
    CHECK(j.type() == json::value_t::binary);
}




TEST(ConstructorsCreateAnIntegerNumberExplicitTest, UninitializedValue)
{
    json::number_integer_t n{};
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
}

TEST(ConstructorsCreateAnIntegerNumberExplicitTest, InitializedValue)
{
    json::number_integer_t n(42);
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
}

class ConstructorsCreateAnIntegerNumberImplicitTest : public ::testing::Test {
 protected:
    ConstructorsCreateAnIntegerNumberImplicitTest() : j_reference(n_reference), j_unsigned_reference(n_unsigned_reference) {}
    // reference objects
    json::number_integer_t n_reference = 42;
    json j_reference;
    json::number_unsigned_t n_unsigned_reference = 42;
    json j_unsigned_reference;
};


TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Short)
{
    short n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UnsignedShort)
{
    unsigned short n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Int)
{
    int n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UnsignedInt)
{
    unsigned int n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Long)
{
    long n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UnsignedLong)
{
    unsigned long n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, LongLong)
{
    long long n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UnsignedLongLong)
{
    unsigned long long n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Int8)
{
    int8_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Int16)
{
    int16_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Int32)
{
    int32_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Int64)
{
    int64_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntFast8T)
{
    int_fast8_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntFast16T)
{
    int_fast16_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntFast32T)
{
    int_fast32_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntFast64T)
{
    int_fast64_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntLeast8T)
{
    int_least8_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntLeast16T)
{
    int_least16_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntLeast32T)
{
    int_least32_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntLeast64T)
{
    int_least64_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Uint8)
{
    uint8_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Uint16)
{
    uint16_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Uint32)
{
    uint32_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, Uint64)
{
    uint64_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UintFast8T)
{
    uint_fast8_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UintFast16T)
{
    uint_fast16_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UintFast32T)
{
    uint_fast32_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UintFast64T)
{
    uint_fast64_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UintLeast8T)
{
    uint_least8_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UintLeast16T)
{
    uint_least16_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UintLeast32T)
{
    uint_least32_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, UintLeast64T)
{
    uint_least64_t n = 42;
    json j(n);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntegerLiteralWithoutSuffix)
{
    json j(42);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntegerLiteralWithUSuffix)
{
    json j(42u);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntegerLiteralWithLSuffix)
{
    json j(42L);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntegerLiteralWithUlSuffix)
{
    json j(42ul);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntegerLiteralWithLlSuffix)
{
    json j(42LL);
    CHECK(j.type() == json::value_t::number_integer);
    CHECK(j == j_reference);
}

TEST_F(ConstructorsCreateAnIntegerNumberImplicitTest, IntegerLiteralWithUllSuffix)
{
    json j(42ull);
    CHECK(j.type() == json::value_t::number_unsigned);
    CHECK(j == j_unsigned_reference);
}




TEST(ConstructorsCreateAFloatingPointNumberExplicitTest, UninitializedValue)
{
    json::number_float_t n{};
    json j(n);
    CHECK(j.type() == json::value_t::number_float);
}

TEST(ConstructorsCreateAFloatingPointNumberExplicitTest, InitializedValue)
{
    json::number_float_t n(42.23);
    json j(n);
    CHECK(j.type() == json::value_t::number_float);
}

TEST(ConstructorsCreateAFloatingPointNumberExplicitTest, NaN)
{
    // NaN is stored properly, but serialized to null
    json::number_float_t n(std::numeric_limits<json::number_float_t>::quiet_NaN());
    json j(n);
    CHECK(j.type() == json::value_t::number_float);

    // check round trip of NaN
    json::number_float_t d{j};
    CHECK((std::isnan(d) && std::isnan(n)) == true);

    // check that NaN is serialized to null
    CHECK(j.dump() == "null");
}

TEST(ConstructorsCreateAFloatingPointNumberExplicitTest, Infinity)
{
    // infinity is stored properly, but serialized to null
    json::number_float_t n(std::numeric_limits<json::number_float_t>::infinity());
    json j(n);
    CHECK(j.type() == json::value_t::number_float);

    // check round trip of infinity
    json::number_float_t d{j};
    CHECK(d == n);

    // check that inf is serialized to null
    CHECK(j.dump() == "null");
}

class ConstructorsCreateAFloatingPointNumberImplicitTest : public ::testing::Test {
 protected:
    ConstructorsCreateAFloatingPointNumberImplicitTest() : n_reference(42.23), j_reference(n_reference) {}

    // reference object
    json::number_float_t n_reference;
    json j_reference;
};


TEST_F(ConstructorsCreateAFloatingPointNumberImplicitTest, Float)
{
    float n = 42.23f;
    json j(n);
    CHECK(j.type() == json::value_t::number_float);
    EXPECT_NEAR(j.m_value.number_float, j_reference.m_value.number_float, 0.001);
}

TEST_F(ConstructorsCreateAFloatingPointNumberImplicitTest, Double)
{
    double n = 42.23;
    json j(n);
    CHECK(j.type() == json::value_t::number_float);
    EXPECT_NEAR(j.m_value.number_float, j_reference.m_value.number_float, 0.001);
}

TEST_F(ConstructorsCreateAFloatingPointNumberImplicitTest, LongDouble)
{
    long double n = 42.23L;
    json j(n);
    CHECK(j.type() == json::value_t::number_float);
    EXPECT_NEAR(j.m_value.number_float, j_reference.m_value.number_float, 0.001);
}

TEST_F(ConstructorsCreateAFloatingPointNumberImplicitTest, FloatingPointLiteralWithoutSuffix)
{
    json j(42.23);
    CHECK(j.type() == json::value_t::number_float);
    EXPECT_NEAR(j.m_value.number_float, j_reference.m_value.number_float, 0.001);
}

TEST_F(ConstructorsCreateAFloatingPointNumberImplicitTest, IntegerLiteralWithFSuffix)
{
    json j(42.23f);
    CHECK(j.type() == json::value_t::number_float);
    EXPECT_NEAR(j.m_value.number_float, j_reference.m_value.number_float, 0.001);
}

TEST_F(ConstructorsCreateAFloatingPointNumberImplicitTest, IntegerLiteralWithLSuffix)
{
    json j(42.23L);
    CHECK(j.type() == json::value_t::number_float);
    EXPECT_NEAR(j.m_value.number_float, j_reference.m_value.number_float, 0.001);
}







TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListEmptyInitializerListTest, Explicit)
{
    json j(json::initializer_list_t {});
    CHECK(j.type() == json::value_t::object);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListEmptyInitializerListTest, Implicit)
{
    json j {};
    CHECK(j.type() == json::value_t::null);
}







TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementArrayTest, Explicit)
{
    json j(json::initializer_list_t {json(json::array_t())});
    CHECK(j.type() == json::value_t::array);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementArrayTest, Implicit)
{
    json j {json::array_t()};
    CHECK(j.type() == json::value_t::array);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementObjectTest, Explicit)
{
    json j(json::initializer_list_t {json(json::object_t())});
    CHECK(j.type() == json::value_t::array);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementObjectTest, Implicit)
{
    json j {json::object_t()};
    CHECK(j.type() == json::value_t::array);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementStringTest, Explicit)
{
    json j(json::initializer_list_t {json("Hello world")});
    CHECK(j.type() == json::value_t::array);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementStringTest, Implicit)
{
    json j {"Hello world"};
    CHECK(j.type() == json::value_t::array);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementBooleanTest, Explicit)
{
    json j(json::initializer_list_t {json(true)});
    CHECK(j.type() == json::value_t::array);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementBooleanTest, Implicit)
{
    json j {true};
    CHECK(j.type() == json::value_t::array);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementNumberIntegerTest, Explicit)
{
    json j(json::initializer_list_t {json(1)});
    CHECK(j.type() == json::value_t::array);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementNumberIntegerTest, Implicit)
{
    json j {1};
    CHECK(j.type() == json::value_t::array);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementNumberUnsignedTest, Explicit)
{
    json j(json::initializer_list_t {json(1u)});
    CHECK(j.type() == json::value_t::array);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementNumberUnsignedTest, Implicit)
{
    json j {1u};
    CHECK(j.type() == json::value_t::array);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementNumberFloatingPointTest, Explicit)
{
    json j(json::initializer_list_t {json(42.23)});
    CHECK(j.type() == json::value_t::array);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListOneElementNumberFloatingPointTest, Implicit)
{
    json j {42.23};
    CHECK(j.type() == json::value_t::array);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoreElementsTest, Explicit)
{
    json j(json::initializer_list_t {1, 1u, 42.23, true, nullptr, json::object_t(), json::array_t()});
    CHECK(j.type() == json::value_t::array);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoreElementsTest, Implicit)
{
    json j {1, 1u, 42.23, true, nullptr, json::object_t(), json::array_t()};
    CHECK(j.type() == json::value_t::array);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListImplicitTypeDeductionTest, Object)
{
    json j { {"one", 1}, {"two", 1u}, {"three", 2.2}, {"four", false} };
    CHECK(j.type() == json::value_t::object);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListImplicitTypeDeductionTest, Array)
{
    json j { {"one", 1}, {"two", 1u}, {"three", 2.2}, {"four", false}, 13 };
    CHECK(j.type() == json::value_t::array);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListExplicitTypeDeductionTest, EmptyObject)
{
    json j = json::object();
    CHECK(j.type() == json::value_t::object);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListExplicitTypeDeductionTest, Object)
{
    json j = json::object({ {"one", 1}, {"two", 1u}, {"three", 2.2}, {"four", false} });
    CHECK(j.type() == json::value_t::object);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListExplicitTypeDeductionTest, ObjectWithError)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::object({ {"one", 1}, {"two", 1u}, {"three", 2.2}, {"four", false}, 13 }), "[json.exception.type_error.301] cannot create object from initializer list", json::type_error&);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListExplicitTypeDeductionTest, EmptyArray)
{
    json j = json::array();
    CHECK(j.type() == json::value_t::array);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListExplicitTypeDeductionTest, Array)
{
    json j = json::array({ {"one", 1}, {"two", 1u}, {"three", 2.2}, {"four", false} });
    CHECK(j.type() == json::value_t::array);
}







TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListStringTest, ConstructorWithImplicitTypesArray)
{
    // This should break through any short string optimization in std::string
    std::string source(1024, '!');
    const auto* source_addr = source.data();
    json j = {std::move(source)};
    const auto* target_addr = j[0].get_ref<std::string const&>().data();
    const bool success = (target_addr == source_addr);
    CHECK(success);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListStringTest, ConstructorWithImplicitTypesObject)
{
    // This should break through any short string optimization in std::string
    std::string source(1024, '!');
    const auto* source_addr = source.data();
    json j = {{"key", std::move(source)}};
    const auto* target_addr = j["key"].get_ref<std::string const&>().data();
    const bool success = (target_addr == source_addr);
    CHECK(success);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListStringTest, ConstructorWithImplicitTypesObjectKey)
{
    // This should break through any short string optimization in std::string
    std::string source(1024, '!');
    const auto* source_addr = source.data();
    json j = {{std::move(source), 42}};
    const auto* target_addr = j.get_ref<json::object_t&>().begin()->first.data();
    const bool success = (target_addr == source_addr);
    CHECK(success);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListArrayTest, ConstructorWithImplicitTypesArray)
{
    json::array_t source = {1, 2, 3};
    const auto* source_addr = source.data();
    json j {std::move(source)};
    const auto* target_addr = j[0].get_ref<json::array_t const&>().data();
    const bool success = (target_addr == source_addr);
    CHECK(success);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListArrayTest, ConstructorWithImplicitTypesObject)
{
    json::array_t source = {1, 2, 3};
    const auto* source_addr = source.data();
    json j {{"key", std::move(source)}};
    const auto* target_addr = j["key"].get_ref<json::array_t const&>().data();
    const bool success = (target_addr == source_addr);
    CHECK(success);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListArrayTest, AssignmentWithImplicitTypesArray)
{
    json::array_t source = {1, 2, 3};
    const auto* source_addr = source.data();
    json j = {std::move(source)};
    const auto* target_addr = j[0].get_ref<json::array_t const&>().data();
    const bool success = (target_addr == source_addr);
    CHECK(success);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListArrayTest, AssignmentWithImplicitTypesObject)
{
    json::array_t source = {1, 2, 3};
    const auto* source_addr = source.data();
    json j = {{"key", std::move(source)}};
    const auto* target_addr = j["key"].get_ref<json::array_t const&>().data();
    const bool success = (target_addr == source_addr);
    CHECK(success);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListObjectTest, ConstructorWithImplicitTypesArray)
{
    json::object_t source = {{"hello", "world"}};
    const json* source_addr = &source.at("hello");
    json j {std::move(source)};
    CHECK(&(j[0].get_ref<json::object_t const&>().at("hello")) == source_addr);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListObjectTest, ConstructorWithImplicitTypesObject)
{
    json::object_t source = {{"hello", "world"}};
    const json* source_addr = &source.at("hello");
    json j {{"key", std::move(source)}};
    CHECK(&(j["key"].get_ref<json::object_t const&>().at("hello")) == source_addr);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListObjectTest, AssignmentWithImplicitTypesArray)
{
    json::object_t source = {{"hello", "world"}};
    const json* source_addr = &source.at("hello");
    json j = {std::move(source)};
    CHECK(&(j[0].get_ref<json::object_t const&>().at("hello")) == source_addr);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListObjectTest, AssignmentWithImplicitTypesObject)
{
    json::object_t source = {{"hello", "world"}};
    const json* source_addr = &source.at("hello");
    json j = {{"key", std::move(source)}};
    CHECK(&(j["key"].get_ref<json::object_t const&>().at("hello")) == source_addr);
}




TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListJsonTest, ConstructorWithImplicitTypesArray)
{
    json source {1, 2, 3};
    const json* source_addr = &source[0];
    json j {std::move(source), {}};
    CHECK(&j[0][0] == source_addr);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListJsonTest, ConstructorWithImplicitTypesObject)
{
    json source {1, 2, 3};
    const json* source_addr = &source[0];
    json j {{"key", std::move(source)}};
    CHECK(&j["key"][0] == source_addr);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListJsonTest, AssignmentWithImplicitTypesArray)
{
    json source {1, 2, 3};
    const json* source_addr = &source[0];
    json j = {std::move(source), {}};
    CHECK(&j[0][0] == source_addr);
}

TEST(ConstructorsCreateAContainerArrayOrObjectFromAnInitializerListMoveFromInitializerListJsonTest, AssignmentWithImplicitTypesObject)
{
    json source {1, 2, 3};
    const json* source_addr = &source[0];
    json j = {{"key", std::move(source)}};
    CHECK(&j["key"][0] == source_addr);
}




TEST(ConstructorsCreateAnArrayOfNCopiesOfAGivenValueTest, Cnt0)
{
    json v = {1, "foo", 34.23, {1, 2, 3}, {{"A", 1}, {"B", 2u}}};
    json arr(0, v);
    CHECK(arr.size() == 0);
}

TEST(ConstructorsCreateAnArrayOfNCopiesOfAGivenValueTest, Cnt1)
{
    json v = {1, "foo", 34.23, {1, 2, 3}, {{"A", 1}, {"B", 2u}}};
    json arr(1, v);
    CHECK(arr.size() == 1);
    for (auto& x : arr)
    {
        CHECK(x == v);
    }
}

TEST(ConstructorsCreateAnArrayOfNCopiesOfAGivenValueTest, Cnt3)
{
    json v = {1, "foo", 34.23, {1, 2, 3}, {{"A", 1}, {"B", 2u}}};
    json arr(3, v);
    CHECK(arr.size() == 3);
    for (auto& x : arr)
    {
        CHECK(x == v);
    }
}







TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeObjectTest, JsonBeginEnd)
{
    {
        json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        json j_new(jobject.begin(), jobject.end());
        CHECK(j_new == jobject);
    }
    {
        json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        json j_new(jobject.cbegin(), jobject.cend());
        CHECK(j_new == jobject);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeObjectTest, JsonBeginBegin)
{
    {
        json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        json j_new(jobject.begin(), jobject.begin());
        CHECK(j_new == json::object());
    }
    {
        json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        json j_new(jobject.cbegin(), jobject.cbegin());
        CHECK(j_new == json::object());
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeObjectTest, ConstructFromSubrange)
{
    json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}, {"d", false}, {"e", true}};
    json j_new(jobject.find("b"), jobject.find("e"));
    CHECK(j_new == json({{"b", 1}, {"c", 17u}, {"d", false}}));
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeObjectTest, IncompatibleIterators)
{
    {
        json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}, {"d", false}, {"e", true}};
        json jobject2 = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        CHECK_THROWS_WITH_AS(json(jobject.begin(), jobject2.end()), "[json.exception.invalid_iterator.201] iterators are not compatible", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(jobject2.begin(), jobject.end()), "[json.exception.invalid_iterator.201] iterators are not compatible", json::invalid_iterator&);
    }
    {
        json jobject = {{"a", "a"}, {"b", 1}, {"c", 17u}, {"d", false}, {"e", true}};
        json jobject2 = {{"a", "a"}, {"b", 1}, {"c", 17u}};
        CHECK_THROWS_WITH_AS(json(jobject.cbegin(), jobject2.cend()), "[json.exception.invalid_iterator.201] iterators are not compatible", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(jobject2.cbegin(), jobject.cend()), "[json.exception.invalid_iterator.201] iterators are not compatible", json::invalid_iterator&);
    }
}




TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeArrayTest, JsonBeginEnd)
{
    {
        json jarray = {1, 2, 3, 4, 5};
        json j_new(jarray.begin(), jarray.end());
        CHECK(j_new == jarray);
    }
    {
        json jarray = {1, 2, 3, 4, 5};
        json j_new(jarray.cbegin(), jarray.cend());
        CHECK(j_new == jarray);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeArrayTest, JsonBeginBegin)
{
    {
        json jarray = {1, 2, 3, 4, 5};
        json j_new(jarray.begin(), jarray.begin());
        CHECK(j_new == json::array());
    }
    {
        json jarray = {1, 2, 3, 4, 5};
        json j_new(jarray.cbegin(), jarray.cbegin());
        CHECK(j_new == json::array());
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeArrayTest, ConstructFromSubrange)
{
    {
        json jarray = {1, 2, 3, 4, 5};
        json j_new(jarray.begin() + 1, jarray.begin() + 3);
        CHECK(j_new == json({2, 3}));
    }
    {
        json jarray = {1, 2, 3, 4, 5};
        json j_new(jarray.cbegin() + 1, jarray.cbegin() + 3);
        CHECK(j_new == json({2, 3}));
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeArrayTest, IncompatibleIterators)
{
    {
        json jarray = {1, 2, 3, 4};
        json jarray2 = {2, 3, 4, 5};
        CHECK_THROWS_WITH_AS(json(jarray.begin(), jarray2.end()), "[json.exception.invalid_iterator.201] iterators are not compatible", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(jarray2.begin(), jarray.end()), "[json.exception.invalid_iterator.201] iterators are not compatible", json::invalid_iterator&);
    }
    {
        json jarray = {1, 2, 3, 4};
        json jarray2 = {2, 3, 4, 5};
        CHECK_THROWS_WITH_AS(json(jarray.cbegin(), jarray2.cend()), "[json.exception.invalid_iterator.201] iterators are not compatible", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(jarray2.cbegin(), jarray.cend()), "[json.exception.invalid_iterator.201] iterators are not compatible", json::invalid_iterator&);
    }
}







TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoValidIteratorsTest, Null)
{
    {
        json j;
        CHECK_THROWS_WITH_AS(json(j.begin(), j.end()), "[json.exception.invalid_iterator.206] cannot construct with iterators from null", json::invalid_iterator&);
    }
    {
        json j;
        CHECK_THROWS_WITH_AS(json(j.cbegin(), j.cend()), "[json.exception.invalid_iterator.206] cannot construct with iterators from null", json::invalid_iterator&);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoValidIteratorsTest, String)
{
    {
        json j = "foo";
        json j_new(j.begin(), j.end());
        CHECK(j == j_new);
    }
    {
        json j = "bar";
        json j_new(j.cbegin(), j.cend());
        CHECK(j == j_new);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoValidIteratorsTest, NumberBoolean)
{
    {
        json j = false;
        json j_new(j.begin(), j.end());
        CHECK(j == j_new);
    }
    {
        json j = true;
        json j_new(j.cbegin(), j.cend());
        CHECK(j == j_new);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoValidIteratorsTest, NumberInteger)
{
    {
        json j = 17;
        json j_new(j.begin(), j.end());
        CHECK(j == j_new);
    }
    {
        json j = 17;
        json j_new(j.cbegin(), j.cend());
        CHECK(j == j_new);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoValidIteratorsTest, NumberUnsigned)
{
    {
        json j = 17u;
        json j_new(j.begin(), j.end());
        CHECK(j == j_new);
    }
    {
        json j = 17u;
        json j_new(j.cbegin(), j.cend());
        CHECK(j == j_new);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoValidIteratorsTest, NumberFloatingPoint)
{
    {
        json j = 23.42;
        json j_new(j.begin(), j.end());
        CHECK(j == j_new);
    }
    {
        json j = 23.42;
        json j_new(j.cbegin(), j.cend());
        CHECK(j == j_new);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoValidIteratorsTest, Binary)
{
    {
        json j = json::binary({1, 2, 3});
        json j_new(j.begin(), j.end());
        CHECK((j == j_new));
    }
    {
        json j = json::binary({1, 2, 3});
        json j_new(j.cbegin(), j.cend());
        CHECK((j == j_new));
    }
}




TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoInvalidIteratorsTest, String)
{
    {
        json j = "foo";
        CHECK_THROWS_WITH_AS(json(j.end(), j.end()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(j.begin(), j.begin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
    {
        json j = "bar";
        CHECK_THROWS_WITH_AS(json(j.cend(), j.cend()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(j.cbegin(), j.cbegin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoInvalidIteratorsTest, NumberBoolean)
{
    {
        json j = false;
        CHECK_THROWS_WITH_AS(json(j.end(), j.end()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(j.begin(), j.begin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
    {
        json j = true;
        CHECK_THROWS_WITH_AS(json(j.cend(), j.cend()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(j.cbegin(), j.cbegin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoInvalidIteratorsTest, NumberInteger)
{
    {
        json j = 17;
        CHECK_THROWS_WITH_AS(json(j.end(), j.end()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(j.begin(), j.begin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
    {
        json j = 17;
        CHECK_THROWS_WITH_AS(json(j.cend(), j.cend()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(j.cbegin(), j.cbegin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoInvalidIteratorsTest, NumberInteger2)
{
    {
        json j = 17u;
        CHECK_THROWS_WITH_AS(json(j.end(), j.end()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(j.begin(), j.begin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
    {
        json j = 17u;
        CHECK_THROWS_WITH_AS(json(j.cend(), j.cend()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(j.cbegin(), j.cbegin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
}

TEST(ConstructorsCreateAJSONContainerFromAnIteratorRangeOtherValuesConstructWithTwoInvalidIteratorsTest, NumberFloatingPoint)
{
    {
        json j = 23.42;
        CHECK_THROWS_WITH_AS(json(j.end(), j.end()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(j.begin(), j.begin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
    {
        json j = 23.42;
        CHECK_THROWS_WITH_AS(json(j.cend(), j.cend()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(json(j.cbegin(), j.cbegin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
}

