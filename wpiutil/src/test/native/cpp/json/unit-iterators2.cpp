//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

// cmake/test.cmake selects the C++ standard versions with which to build a
// unit test based on the presence of JSON_HAS_CPP_<VERSION> macros.
// When using macros that are only defined for particular versions of the standard
// (e.g., JSON_HAS_FILESYSTEM for C++17 and up), please mention the corresponding
// version macro in a comment close by, like this:
// JSON_HAS_CPP_<VERSION> (do not remove; see note at top of file)

#include "gtest/gtest.h"

#include "unit-json.h"
using wpi::json;

#if JSON_HAS_RANGES
    #include <algorithm>
    #include <ranges>
#endif




TEST(Iterators2Test, IteratorComparisons)
{
    json j_values = {nullptr, true, 42, 42u, 23.23, {{"one", 1}, {"two", 2}}, {1, 2, 3, 4, 5}, "Hello, world"};

    for (json& j : j_values)
    {
        auto it1 = j.begin();
        auto it2 = j.begin();
        auto it3 = j.begin();
        ++it2;
        ++it3;
        ++it3;
        auto it1_c = j.cbegin();
        auto it2_c = j.cbegin();
        auto it3_c = j.cbegin();
        ++it2_c;
        ++it3_c;
        ++it3_c;

        // comparison: equal
        {
            CHECK(it1 == it1);
            CHECK(!(it1 == it2));
            CHECK(!(it1 == it3));
            CHECK(!(it2 == it3));
            CHECK(it1_c == it1_c);
            CHECK(!(it1_c == it2_c));
            CHECK(!(it1_c == it3_c));
            CHECK(!(it2_c == it3_c));
        }

        // comparison: not equal
        {
            // check definition
            CHECK( (it1 != it1) == !(it1 == it1) );
            CHECK( (it1 != it2) == !(it1 == it2) );
            CHECK( (it1 != it3) == !(it1 == it3) );
            CHECK( (it2 != it3) == !(it2 == it3) );
            CHECK( (it1_c != it1_c) == !(it1_c == it1_c) );
            CHECK( (it1_c != it2_c) == !(it1_c == it2_c) );
            CHECK( (it1_c != it3_c) == !(it1_c == it3_c) );
            CHECK( (it2_c != it3_c) == !(it2_c == it3_c) );
        }

        // comparison: smaller
        {
            if (j.type() == json::value_t::object)
            {
#if JSON_DIAGNOSTICS
                CHECK_THROWS_WITH_AS(it1 < it1, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 < it2, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 < it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 < it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it1_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it2_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c < it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
#else
                CHECK_THROWS_WITH_AS(it1 < it1, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 < it2, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 < it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 < it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it1_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it2_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c < it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
#endif
            }
            else
            {
                CHECK(!(it1 < it1));
                CHECK(it1 < it2);
                CHECK(it1 < it3);
                CHECK(it2 < it3);
                CHECK(!(it1_c < it1_c));
                CHECK(it1_c < it2_c);
                CHECK(it1_c < it3_c);
                CHECK(it2_c < it3_c);
            }
        }

        // comparison: less than or equal
        {
            if (j.type() == json::value_t::object)
            {
#if JSON_DIAGNOSTICS
                CHECK_THROWS_WITH_AS(it1 <= it1, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 <= it2, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 <= it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 <= it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it1_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it2_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c <= it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
#else
                CHECK_THROWS_WITH_AS(it1 <= it1, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 <= it2, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 <= it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 <= it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it1_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it2_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c <= it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
#endif
            }
            else
            {
                // check definition
                CHECK( (it1 <= it1) == !(it1 < it1) );
                CHECK( (it1 <= it2) == !(it2 < it1) );
                CHECK( (it1 <= it3) == !(it3 < it1) );
                CHECK( (it2 <= it3) == !(it3 < it2) );
                CHECK( (it1_c <= it1_c) == !(it1_c < it1_c) );
                CHECK( (it1_c <= it2_c) == !(it2_c < it1_c) );
                CHECK( (it1_c <= it3_c) == !(it3_c < it1_c) );
                CHECK( (it2_c <= it3_c) == !(it3_c < it2_c) );
            }
        }

        // comparison: greater than
        {
            if (j.type() == json::value_t::object)
            {
#if JSON_DIAGNOSTICS
                CHECK_THROWS_WITH_AS(it1 > it1, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 > it2, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 > it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 > it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it1_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it2_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c > it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
#else
                CHECK_THROWS_WITH_AS(it1 > it1, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 > it2, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 > it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 > it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it1_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it2_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c > it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
#endif
            }
            else
            {
                // check definition
                CHECK( (it1 > it1) == (it1 < it1) );
                CHECK( (it1 > it2) == (it2 < it1) );
                CHECK( (it1 > it3) == (it3 < it1) );
                CHECK( (it2 > it3) == (it3 < it2) );
                CHECK( (it1_c > it1_c) == (it1_c < it1_c) );
                CHECK( (it1_c > it2_c) == (it2_c < it1_c) );
                CHECK( (it1_c > it3_c) == (it3_c < it1_c) );
                CHECK( (it2_c > it3_c) == (it3_c < it2_c) );
            }
        }

        // comparison: greater than or equal
        {
            if (j.type() == json::value_t::object)
            {
#if JSON_DIAGNOSTICS
                CHECK_THROWS_WITH_AS(it1 >= it1, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 >= it2, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 >= it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 >= it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it1_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it2_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c >= it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
#else
                CHECK_THROWS_WITH_AS(it1 >= it1, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 >= it2, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 >= it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 >= it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it1_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it2_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c >= it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
#endif
            }
            else
            {
                // check definition
                CHECK( (it1 >= it1) == !(it1 < it1) );
                CHECK( (it1 >= it2) == !(it1 < it2) );
                CHECK( (it1 >= it3) == !(it1 < it3) );
                CHECK( (it2 >= it3) == !(it2 < it3) );
                CHECK( (it1_c >= it1_c) == !(it1_c < it1_c) );
                CHECK( (it1_c >= it2_c) == !(it1_c < it2_c) );
                CHECK( (it1_c >= it3_c) == !(it1_c < it3_c) );
                CHECK( (it2_c >= it3_c) == !(it2_c < it3_c) );
            }
        }
    }

    // check exceptions if different objects are compared
    for (auto j : j_values)
    {
        for (auto k : j_values)
        {
            if (j != k)
            {
#if JSON_DIAGNOSTICS
                // the output differs in each loop, so we cannot fix a string for the expected exception
#else
                CHECK_THROWS_WITH_AS(j.begin() == k.begin(), "[json.exception.invalid_iterator.212] cannot compare iterators of different containers", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(j.cbegin() == k.cbegin(), "[json.exception.invalid_iterator.212] cannot compare iterators of different containers", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(j.begin() < k.begin(), "[json.exception.invalid_iterator.212] cannot compare iterators of different containers", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(j.cbegin() < k.cbegin(), "[json.exception.invalid_iterator.212] cannot compare iterators of different containers", json::invalid_iterator&);
#endif
            }
        }
    }
}

class Iterators2IteratorArithmeticTest : public ::testing::Test {
 protected:
    json j_object = {{"one", 1}, {"two", 2}, {"three", 3}};
    json j_array = {1, 2, 3, 4, 5, 6};
    json j_null = nullptr;
    json j_value = 42;
};


class Iterators2IteratorArithmeticAdditionAndSubtractionTest : public Iterators2IteratorArithmeticTest{};


TEST_F(Iterators2IteratorArithmeticAdditionAndSubtractionTest, Object)
{
    {
        auto it = j_object.begin();
        CHECK_THROWS_WITH_AS(it += 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.cbegin();
        CHECK_THROWS_WITH_AS(it += 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.begin();
        CHECK_THROWS_WITH_AS(it + 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.cbegin();
        CHECK_THROWS_WITH_AS(it + 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.begin();
        CHECK_THROWS_WITH_AS(1 + it, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.cbegin();
        CHECK_THROWS_WITH_AS(1 + it, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.begin();
        CHECK_THROWS_WITH_AS(it -= 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.cbegin();
        CHECK_THROWS_WITH_AS(it -= 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.begin();
        CHECK_THROWS_WITH_AS(it - 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.cbegin();
        CHECK_THROWS_WITH_AS(it - 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.begin();
        CHECK_THROWS_WITH_AS(it - it, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.cbegin();
        CHECK_THROWS_WITH_AS(it - it, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
}

TEST_F(Iterators2IteratorArithmeticAdditionAndSubtractionTest, Array)
{
    {
        auto it = j_array.begin();
        it += 3;
        CHECK((j_array.begin() + 3) == it);
        CHECK((3 + j_array.begin()) == it);
        CHECK((it - 3) == j_array.begin());
        CHECK((it - j_array.begin()) == 3);
        CHECK(*it == json(4));
        it -= 2;
        CHECK(*it == json(2));
    }
    {
        auto it = j_array.cbegin();
        it += 3;
        CHECK((j_array.cbegin() + 3) == it);
        CHECK((3 + j_array.cbegin()) == it);
        CHECK((it - 3) == j_array.cbegin());
        CHECK((it - j_array.cbegin()) == 3);
        CHECK(*it == json(4));
        it -= 2;
        CHECK(*it == json(2));
    }
}

TEST_F(Iterators2IteratorArithmeticAdditionAndSubtractionTest, Null)
{
    {
        auto it = j_null.begin();
        it += 3;
        CHECK((j_null.begin() + 3) == it);
        CHECK((3 + j_null.begin()) == it);
        CHECK((it - 3) == j_null.begin());
        CHECK((it - j_null.begin()) == 3);
        CHECK(it != j_null.end());
        it -= 3;
        CHECK(it == j_null.end());
    }
    {
        auto it = j_null.cbegin();
        it += 3;
        CHECK((j_null.cbegin() + 3) == it);
        CHECK((3 + j_null.cbegin()) == it);
        CHECK((it - 3) == j_null.cbegin());
        CHECK((it - j_null.cbegin()) == 3);
        CHECK(it != j_null.cend());
        it -= 3;
        CHECK(it == j_null.cend());
    }
}

TEST_F(Iterators2IteratorArithmeticAdditionAndSubtractionTest, Value)
{
    {
        auto it = j_value.begin();
        it += 3;
        CHECK((j_value.begin() + 3) == it);
        CHECK((3 + j_value.begin()) == it);
        CHECK((it - 3) == j_value.begin());
        CHECK((it - j_value.begin()) == 3);
        CHECK(it != j_value.end());
        it -= 3;
        CHECK(*it == json(42));
    }
    {
        auto it = j_value.cbegin();
        it += 3;
        CHECK((j_value.cbegin() + 3) == it);
        CHECK((3 + j_value.cbegin()) == it);
        CHECK((it - 3) == j_value.cbegin());
        CHECK((it - j_value.cbegin()) == 3);
        CHECK(it != j_value.cend());
        it -= 3;
        CHECK(*it == json(42));
    }
}

class Iterators2IteratorArithmeticSubscriptOperatorTest : public Iterators2IteratorArithmeticTest{};


TEST_F(Iterators2IteratorArithmeticSubscriptOperatorTest, Object)
{
    {
        auto it = j_object.begin();
        CHECK_THROWS_WITH_AS(it[0], "[json.exception.invalid_iterator.208] cannot use operator[] for object iterators", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.208] cannot use operator[] for object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.cbegin();
        CHECK_THROWS_WITH_AS(it[0], "[json.exception.invalid_iterator.208] cannot use operator[] for object iterators", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.208] cannot use operator[] for object iterators", json::invalid_iterator&);
    }
}

TEST_F(Iterators2IteratorArithmeticSubscriptOperatorTest, Array)
{
    {
        auto it = j_array.begin();
        CHECK(it[0] == json(1));
        CHECK(it[1] == json(2));
        CHECK(it[2] == json(3));
        CHECK(it[3] == json(4));
        CHECK(it[4] == json(5));
        CHECK(it[5] == json(6));
    }
    {
        auto it = j_array.cbegin();
        CHECK(it[0] == json(1));
        CHECK(it[1] == json(2));
        CHECK(it[2] == json(3));
        CHECK(it[3] == json(4));
        CHECK(it[4] == json(5));
        CHECK(it[5] == json(6));
    }
}

TEST_F(Iterators2IteratorArithmeticSubscriptOperatorTest, Null)
{
    {
        auto it = j_null.begin();
        CHECK_THROWS_WITH_AS(it[0], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    }
    {
        auto it = j_null.cbegin();
        CHECK_THROWS_WITH_AS(it[0], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    }
}

TEST_F(Iterators2IteratorArithmeticSubscriptOperatorTest, Value)
{
    {
        auto it = j_value.begin();
        CHECK(it[0] == json(42));
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    }
    {
        auto it = j_value.cbegin();
        CHECK(it[0] == json(42));
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    }
}

TEST(Iterators2Test, ReverseIteratorComparisons)
{
    json j_values = {nullptr, true, 42, 42u, 23.23, {{"one", 1}, {"two", 2}}, {1, 2, 3, 4, 5}, "Hello, world"};

    for (json& j : j_values)
    {
        auto it1 = j.rbegin();
        auto it2 = j.rbegin();
        auto it3 = j.rbegin();
        ++it2;
        ++it3;
        ++it3;
        auto it1_c = j.crbegin();
        auto it2_c = j.crbegin();
        auto it3_c = j.crbegin();
        ++it2_c;
        ++it3_c;
        ++it3_c;

        // comparison: equal
        {
            CHECK(it1 == it1);
            CHECK(!(it1 == it2));
            CHECK(!(it1 == it3));
            CHECK(!(it2 == it3));
            CHECK(it1_c == it1_c);
            CHECK(!(it1_c == it2_c));
            CHECK(!(it1_c == it3_c));
            CHECK(!(it2_c == it3_c));
        }

        // comparison: not equal
        {
            // check definition
            CHECK( (it1 != it1) == !(it1 == it1) );
            CHECK( (it1 != it2) == !(it1 == it2) );
            CHECK( (it1 != it3) == !(it1 == it3) );
            CHECK( (it2 != it3) == !(it2 == it3) );
            CHECK( (it1_c != it1_c) == !(it1_c == it1_c) );
            CHECK( (it1_c != it2_c) == !(it1_c == it2_c) );
            CHECK( (it1_c != it3_c) == !(it1_c == it3_c) );
            CHECK( (it2_c != it3_c) == !(it2_c == it3_c) );
        }

        // comparison: smaller
        {
            if (j.type() == json::value_t::object)
            {
#if JSON_DIAGNOSTICS
                CHECK_THROWS_WITH_AS(it1 < it1, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 < it2, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 < it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 < it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it1_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it2_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c < it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
#else
                CHECK_THROWS_WITH_AS(it1 < it1, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 < it2, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 < it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 < it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it1_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it2_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c < it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c < it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
#endif
            }
            else
            {
                CHECK(!(it1 < it1));
                CHECK(it1 < it2);
                CHECK(it1 < it3);
                CHECK(it2 < it3);
                CHECK(!(it1_c < it1_c));
                CHECK(it1_c < it2_c);
                CHECK(it1_c < it3_c);
                CHECK(it2_c < it3_c);
            }
        }

        // comparison: less than or equal
        {
            if (j.type() == json::value_t::object)
            {
#if JSON_DIAGNOSTICS
                CHECK_THROWS_WITH_AS(it1 <= it1, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 <= it2, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 <= it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 <= it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it1_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it2_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c <= it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
#else
                CHECK_THROWS_WITH_AS(it1 <= it1, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 <= it2, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 <= it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 <= it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it1_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it2_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c <= it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c <= it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
#endif
            }
            else
            {
                // check definition
                CHECK( (it1 <= it1) == !(it1 < it1) );
                CHECK( (it1 <= it2) == !(it2 < it1) );
                CHECK( (it1 <= it3) == !(it3 < it1) );
                CHECK( (it2 <= it3) == !(it3 < it2) );
                CHECK( (it1_c <= it1_c) == !(it1_c < it1_c) );
                CHECK( (it1_c <= it2_c) == !(it2_c < it1_c) );
                CHECK( (it1_c <= it3_c) == !(it3_c < it1_c) );
                CHECK( (it2_c <= it3_c) == !(it3_c < it2_c) );
            }
        }

        // comparison: greater than
        {
            if (j.type() == json::value_t::object)
            {
#if JSON_DIAGNOSTICS
                CHECK_THROWS_WITH_AS(it1 > it1, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 > it2, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 > it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 > it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it1_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it2_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c > it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
#else
                CHECK_THROWS_WITH_AS(it1 > it1, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 > it2, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 > it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 > it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it1_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it2_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c > it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c > it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
#endif
            }
            else
            {
                // check definition
                CHECK( (it1 > it1) == (it1 < it1) );
                CHECK( (it1 > it2) == (it2 < it1) );
                CHECK( (it1 > it3) == (it3 < it1) );
                CHECK( (it2 > it3) == (it3 < it2) );
                CHECK( (it1_c > it1_c) == (it1_c < it1_c) );
                CHECK( (it1_c > it2_c) == (it2_c < it1_c) );
                CHECK( (it1_c > it3_c) == (it3_c < it1_c) );
                CHECK( (it2_c > it3_c) == (it3_c < it2_c) );
            }
        }

        // comparison: greater than or equal
        {
            if (j.type() == json::value_t::object)
            {
#if JSON_DIAGNOSTICS
                CHECK_THROWS_WITH_AS(it1 >= it1, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 >= it2, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 >= it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 >= it3, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it1_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it2_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c >= it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it3_c, "[json.exception.invalid_iterator.213] (/5) cannot compare order of object iterators", json::invalid_iterator&);
#else
                CHECK_THROWS_WITH_AS(it1 >= it1, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 >= it2, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2 >= it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1 >= it3, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it1_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it2_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it2_c >= it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(it1_c >= it3_c, "[json.exception.invalid_iterator.213] cannot compare order of object iterators", json::invalid_iterator&);
#endif
            }
            else
            {
                // check definition
                CHECK( (it1 >= it1) == !(it1 < it1) );
                CHECK( (it1 >= it2) == !(it1 < it2) );
                CHECK( (it1 >= it3) == !(it1 < it3) );
                CHECK( (it2 >= it3) == !(it2 < it3) );
                CHECK( (it1_c >= it1_c) == !(it1_c < it1_c) );
                CHECK( (it1_c >= it2_c) == !(it1_c < it2_c) );
                CHECK( (it1_c >= it3_c) == !(it1_c < it3_c) );
                CHECK( (it2_c >= it3_c) == !(it2_c < it3_c) );
            }
        }
    }

    // check exceptions if different objects are compared
    for (auto j : j_values)
    {
        for (auto k : j_values)
        {
            if (j != k)
            {
#if JSON_DIAGNOSTICS
                // the output differs in each loop, so we cannot fix a string for the expected exception
#else
                CHECK_THROWS_WITH_AS(j.rbegin() == k.rbegin(), "[json.exception.invalid_iterator.212] cannot compare iterators of different containers", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(j.crbegin() == k.crbegin(), "[json.exception.invalid_iterator.212] cannot compare iterators of different containers", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(j.rbegin() < k.rbegin(), "[json.exception.invalid_iterator.212] cannot compare iterators of different containers", json::invalid_iterator&);
                CHECK_THROWS_WITH_AS(j.crbegin() < k.crbegin(), "[json.exception.invalid_iterator.212] cannot compare iterators of different containers", json::invalid_iterator&);
#endif
            }
        }
    }
}

class Iterators2ReverseIteratorArithmeticTest : public ::testing::Test {
 protected:
    json j_object = {{"one", 1}, {"two", 2}, {"three", 3}};
    json j_array = {1, 2, 3, 4, 5, 6};
    json j_null = nullptr;
    json j_value = 42;
};


class Iterators2ReverseIteratorArithmeticAdditionAndSubtractionTest : public Iterators2ReverseIteratorArithmeticTest{};


TEST_F(Iterators2ReverseIteratorArithmeticAdditionAndSubtractionTest, Object)
{
    {
        auto it = j_object.rbegin();
        CHECK_THROWS_WITH_AS(it += 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.crbegin();
        CHECK_THROWS_WITH_AS(it += 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.rbegin();
        CHECK_THROWS_WITH_AS(it + 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.crbegin();
        CHECK_THROWS_WITH_AS(it + 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.rbegin();
        CHECK_THROWS_WITH_AS(1 + it, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.crbegin();
        CHECK_THROWS_WITH_AS(1 + it, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.rbegin();
        CHECK_THROWS_WITH_AS(it -= 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.crbegin();
        CHECK_THROWS_WITH_AS(it -= 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.rbegin();
        CHECK_THROWS_WITH_AS(it - 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.crbegin();
        CHECK_THROWS_WITH_AS(it - 1, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.rbegin();
        CHECK_THROWS_WITH_AS(it - it, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.crbegin();
        CHECK_THROWS_WITH_AS(it - it, "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
}

TEST_F(Iterators2ReverseIteratorArithmeticAdditionAndSubtractionTest, Array)
{
    {
        auto it = j_array.rbegin();
        it += 3;
        CHECK((j_array.rbegin() + 3) == it);
        CHECK(json::reverse_iterator(3 + j_array.rbegin()) == it);
        CHECK((it - 3) == j_array.rbegin());
        CHECK((it - j_array.rbegin()) == 3);
        CHECK(*it == json(3));
        it -= 2;
        CHECK(*it == json(5));
    }
    {
        auto it = j_array.crbegin();
        it += 3;
        CHECK((j_array.crbegin() + 3) == it);
        CHECK(json::const_reverse_iterator(3 + j_array.crbegin()) == it);
        CHECK((it - 3) == j_array.crbegin());
        CHECK((it - j_array.crbegin()) == 3);
        CHECK(*it == json(3));
        it -= 2;
        CHECK(*it == json(5));
    }
}

TEST_F(Iterators2ReverseIteratorArithmeticAdditionAndSubtractionTest, Null)
{
    {
        auto it = j_null.rbegin();
        it += 3;
        CHECK((j_null.rbegin() + 3) == it);
        CHECK(json::reverse_iterator(3 + j_null.rbegin()) == it);
        CHECK((it - 3) == j_null.rbegin());
        CHECK((it - j_null.rbegin()) == 3);
        CHECK(it != j_null.rend());
        it -= 3;
        CHECK(it == j_null.rend());
    }
    {
        auto it = j_null.crbegin();
        it += 3;
        CHECK((j_null.crbegin() + 3) == it);
        CHECK(json::const_reverse_iterator(3 + j_null.crbegin()) == it);
        CHECK((it - 3) == j_null.crbegin());
        CHECK((it - j_null.crbegin()) == 3);
        CHECK(it != j_null.crend());
        it -= 3;
        CHECK(it == j_null.crend());
    }
}

TEST_F(Iterators2ReverseIteratorArithmeticAdditionAndSubtractionTest, Value)
{
    {
        auto it = j_value.rbegin();
        it += 3;
        CHECK((j_value.rbegin() + 3) == it);
        CHECK(json::reverse_iterator(3 + j_value.rbegin()) == it);
        CHECK((it - 3) == j_value.rbegin());
        CHECK((it - j_value.rbegin()) == 3);
        CHECK(it != j_value.rend());
        it -= 3;
        CHECK(*it == json(42));
    }
    {
        auto it = j_value.crbegin();
        it += 3;
        CHECK((j_value.crbegin() + 3) == it);
        CHECK(json::const_reverse_iterator(3 + j_value.crbegin()) == it);
        CHECK((it - 3) == j_value.crbegin());
        CHECK((it - j_value.crbegin()) == 3);
        CHECK(it != j_value.crend());
        it -= 3;
        CHECK(*it == json(42));
    }
}

class Iterators2ReverseIteratorArithmeticSubscriptOperatorTest : public Iterators2ReverseIteratorArithmeticTest{};


TEST_F(Iterators2ReverseIteratorArithmeticSubscriptOperatorTest, Object)
{
    {
        auto it = j_object.rbegin();
        CHECK_THROWS_WITH_AS(it[0], "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
    {
        auto it = j_object.crbegin();
        CHECK_THROWS_WITH_AS(it[0], "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.209] cannot use offsets with object iterators", json::invalid_iterator&);
    }
}

TEST_F(Iterators2ReverseIteratorArithmeticSubscriptOperatorTest, Array)
{
    {
        auto it = j_array.rbegin();
        CHECK(it[0] == json(6));
        CHECK(it[1] == json(5));
        CHECK(it[2] == json(4));
        CHECK(it[3] == json(3));
        CHECK(it[4] == json(2));
        CHECK(it[5] == json(1));
    }
    {
        auto it = j_array.crbegin();
        CHECK(it[0] == json(6));
        CHECK(it[1] == json(5));
        CHECK(it[2] == json(4));
        CHECK(it[3] == json(3));
        CHECK(it[4] == json(2));
        CHECK(it[5] == json(1));
    }
}

TEST_F(Iterators2ReverseIteratorArithmeticSubscriptOperatorTest, Null)
{
    {
        auto it = j_null.rbegin();
        CHECK_THROWS_WITH_AS(it[0], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    }
    {
        auto it = j_null.crbegin();
        CHECK_THROWS_WITH_AS(it[0], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    }
}

TEST_F(Iterators2ReverseIteratorArithmeticSubscriptOperatorTest, Value)
{
    {
        auto it = j_value.rbegin();
        CHECK(it[0] == json(42));
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    }
    {
        auto it = j_value.crbegin();
        CHECK(it[0] == json(42));
        CHECK_THROWS_WITH_AS(it[1], "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    }
}

#if JSON_HAS_RANGES

class Iterators2RangesTest : public ::testing::Test {
 protected:
    

    // libstdc++ algorithms don't work with Clang 15 (04/2022)

    


    // libstdc++ views don't work with Clang 15 (04/2022)
    // libc++ hides limited ranges implementation behind guard macro
};


TEST(Iterators2RangesTest, Concepts)
{
    using wpi::detail::iteration_proxy_value;
    CHECK(std::bidirectional_iterator<json::iterator>);
    CHECK(std::input_iterator<iteration_proxy_value<json::iterator>>);

    CHECK(std::is_same<json::iterator, std::ranges::iterator_t<json>>::value);
    CHECK(std::ranges::bidirectional_range<json>);

    using wpi::detail::iteration_proxy;
    using items_type = decltype(std::declval<json&>().items());
    CHECK(std::is_same<items_type, iteration_proxy<json::iterator>>::value);
    CHECK(std::is_same<iteration_proxy_value<json::iterator>, std::ranges::iterator_t<items_type>>::value);
    CHECK(std::ranges::input_range<items_type>);
}

#if !DOCTEST_CLANG || (DOCTEST_CLANG && defined(__GLIBCXX__))

class Iterators2RangesAlgorithmsTest : public Iterators2RangesTest{};


TEST_F(Iterators2RangesAlgorithmsTest, Copy)
{
    json j{"foo", "bar"};
    auto j_copied = json::array();

    std::ranges::copy(j, std::back_inserter(j_copied));

    CHECK(j == j_copied);
}

TEST_F(Iterators2RangesAlgorithmsTest, FindIf)
{
    json j{1, 3, 2, 4};
    auto j_even = json::array();

#if JSON_USE_IMPLICIT_CONVERSIONS
    auto it = std::ranges::find_if(j, [](int v) noexcept
    {
        return (v % 2) == 0;
    });
#else
    auto it = std::ranges::find_if(j, [](const json & j) noexcept
    {
        int v;
        j.get_to(v);
        return (v % 2) == 0;
    });
#endif

    CHECK(*it == 2);
}

#endif

#if !(DOCTEST_CLANG && (defined(__GLIBCXX__) || defined(_LIBCPP_HAS_NO_INCOMPLETE_RANGES)))

class Iterators2RangesViewsTest : public Iterators2RangesTest{};


TEST_F(Iterators2RangesViewsTest, Reverse)
{
    json j{1, 2, 3, 4, 5};
    json j_expected{5, 4, 3, 2, 1};

    auto reversed = j | std::views::reverse;
    CHECK(reversed == j_expected);
}

TEST_F(Iterators2RangesViewsTest, Transform)
{
    json j
    {
        { "a_key", "a_value"},
        { "b_key", "b_value"},
        { "c_key", "c_value"},
    };
    json j_expected{"a_key", "b_key", "c_key"};

    auto transformed = j.items() | std::views::transform([](const auto & item)
    {
        return item.key();
    });
    auto j_transformed = json::array();
    std::ranges::copy(transformed, std::back_inserter(j_transformed));

    CHECK(j_transformed == j_expected);
}

#endif

#endif

