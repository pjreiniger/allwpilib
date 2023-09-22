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
using wpi::json;




class ElementAccess1ArrayTest : public ::testing::Test {
 protected:
    json j = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
    const json j_const = j;
};


class ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingTest : public ElementAccess1ArrayTest{};


TEST_F(ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingTest, AccessWithinBounds)
{
    CHECK(j.at(0) == json(1));
    CHECK(j.at(1) == json(1u));
    CHECK(j.at(2) == json(true));
    CHECK(j.at(3) == json(nullptr));
    CHECK(j.at(4) == json("string"));
    CHECK(j.at(5) == json(42.23));
    CHECK(j.at(6) == json::object());
    CHECK(j.at(7) == json({1, 2, 3}));

    CHECK(j_const.at(0) == json(1));
    CHECK(j_const.at(1) == json(1u));
    CHECK(j_const.at(2) == json(true));
    CHECK(j_const.at(3) == json(nullptr));
    CHECK(j_const.at(4) == json("string"));
    CHECK(j_const.at(5) == json(42.23));
    CHECK(j_const.at(6) == json::object());
    CHECK(j_const.at(7) == json({1, 2, 3}));
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingTest, AccessOutsideBounds)
{
    CHECK_THROWS_WITH_AS(j.at(8),
                         "[json.exception.out_of_range.401] array index 8 is out of range", json::out_of_range&);
    CHECK_THROWS_WITH_AS(j_const.at(8),
                         "[json.exception.out_of_range.401] array index 8 is out of range", json::out_of_range&);
}

class ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingAccessOnNonArrayTypeTest : public ElementAccess1ArrayTest{};


TEST_F(ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingAccessOnNonArrayTypeTest, Null)
{
    json j_nonarray(json::value_t::null);
    const json j_nonarray_const(j_nonarray);

    CHECK_THROWS_WITH_AS(j_nonarray.at(0), "[json.exception.type_error.304] cannot use at() with null", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const.at(0), "[json.exception.type_error.304] cannot use at() with null", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingAccessOnNonArrayTypeTest, Boolean)
{
    json j_nonarray(json::value_t::boolean);
    const json j_nonarray_const(j_nonarray);

    CHECK_THROWS_WITH_AS(j_nonarray.at(0), "[json.exception.type_error.304] cannot use at() with boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const.at(0), "[json.exception.type_error.304] cannot use at() with boolean", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingAccessOnNonArrayTypeTest, String)
{
    json j_nonarray(json::value_t::string);
    const json j_nonarray_const(j_nonarray);

    CHECK_THROWS_WITH_AS(j_nonarray.at(0), "[json.exception.type_error.304] cannot use at() with string", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const.at(0), "[json.exception.type_error.304] cannot use at() with string", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingAccessOnNonArrayTypeTest, Object)
{
    json j_nonarray(json::value_t::object);
    const json j_nonarray_const(j_nonarray);

    CHECK_THROWS_WITH_AS(j_nonarray.at(0), "[json.exception.type_error.304] cannot use at() with object", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const.at(0), "[json.exception.type_error.304] cannot use at() with object", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingAccessOnNonArrayTypeTest, NumberInteger)
{
    json j_nonarray(json::value_t::number_integer);
    const json j_nonarray_const(j_nonarray);

    CHECK_THROWS_WITH_AS(j_nonarray.at(0), "[json.exception.type_error.304] cannot use at() with number", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const.at(0), "[json.exception.type_error.304] cannot use at() with number", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingAccessOnNonArrayTypeTest, NumberUnsigned)
{
    json j_nonarray(json::value_t::number_unsigned);
    const json j_nonarray_const(j_nonarray);

    CHECK_THROWS_WITH_AS(j_nonarray.at(0), "[json.exception.type_error.304] cannot use at() with number", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const.at(0), "[json.exception.type_error.304] cannot use at() with number", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementWithBoundsCheckingAccessOnNonArrayTypeTest, NumberFloatingPoint)
{
    json j_nonarray(json::value_t::number_float);
    const json j_nonarray_const(j_nonarray);

    CHECK_THROWS_WITH_AS(j_nonarray.at(0), "[json.exception.type_error.304] cannot use at() with number", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const.at(0), "[json.exception.type_error.304] cannot use at() with number", json::type_error&);
}

TEST_F(ElementAccess1ArrayTest, FrontAndBack)
{
    CHECK(j.front() == json(1));
    CHECK(j_const.front() == json(1));
    CHECK(j.back() == json({1, 2, 3}));
    CHECK(j_const.back() == json({1, 2, 3}));
}

class ElementAccess1ArrayAccessSpecifiedElementTest : public ElementAccess1ArrayTest{};


TEST_F(ElementAccess1ArrayAccessSpecifiedElementTest, AccessWithinBounds)
{
    CHECK(j[0] == json(1));
    CHECK(j[1] == json(1u));
    CHECK(j[2] == json(true));
    CHECK(j[3] == json(nullptr));
    CHECK(j[4] == json("string"));
    CHECK(j[5] == json(42.23));
    CHECK(j[6] == json::object());
    CHECK(j[7] == json({1, 2, 3}));

    CHECK(j_const[0] == json(1));
    CHECK(j_const[1] == json(1u));
    CHECK(j_const[2] == json(true));
    CHECK(j_const[3] == json(nullptr));
    CHECK(j_const[4] == json("string"));
    CHECK(j_const[5] == json(42.23));
    CHECK(j_const[6] == json::object());
    CHECK(j_const[7] == json({1, 2, 3}));
}

class ElementAccess1ArrayAccessSpecifiedElementAccessOnNonArrayTypeTest : public ElementAccess1ArrayTest{};


class ElementAccess1ArrayAccessSpecifiedElementAccessOnNonArrayTypeNullTest : public ElementAccess1ArrayTest{};


TEST_F(ElementAccess1ArrayAccessSpecifiedElementAccessOnNonArrayTypeNullTest, StandardTests)
{
    json j_nonarray(json::value_t::null);
    const json j_nonarray_const(j_nonarray);
    CHECK_NOTHROW(j_nonarray[0]);
    CHECK_THROWS_WITH_AS(j_nonarray_const[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with null", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementAccessOnNonArrayTypeNullTest, ImplicitTransformationToProperlyFilledArray)
{
    json j_nonarray;
    j_nonarray[3] = 42;
    CHECK(j_nonarray == json({nullptr, nullptr, nullptr, 42}));
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementAccessOnNonArrayTypeTest, Boolean)
{
    json j_nonarray(json::value_t::boolean);
    const json j_nonarray_const(j_nonarray);
    CHECK_THROWS_WITH_AS(j_nonarray[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with boolean", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with boolean", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementAccessOnNonArrayTypeTest, String)
{
    json j_nonarray(json::value_t::string);
    const json j_nonarray_const(j_nonarray);
    CHECK_THROWS_WITH_AS(j_nonarray[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with string", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with string", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementAccessOnNonArrayTypeTest, Object)
{
    json j_nonarray(json::value_t::object);
    const json j_nonarray_const(j_nonarray);
    CHECK_THROWS_WITH_AS(j_nonarray[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with object", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with object", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementAccessOnNonArrayTypeTest, NumberInteger)
{
    json j_nonarray(json::value_t::number_integer);
    const json j_nonarray_const(j_nonarray);
    CHECK_THROWS_WITH_AS(j_nonarray[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with number", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with number", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementAccessOnNonArrayTypeTest, NumberUnsigned)
{
    json j_nonarray(json::value_t::number_unsigned);
    const json j_nonarray_const(j_nonarray);
    CHECK_THROWS_WITH_AS(j_nonarray[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with number", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with number", json::type_error&);
}

TEST_F(ElementAccess1ArrayAccessSpecifiedElementAccessOnNonArrayTypeTest, NumberFloatingPoint)
{
    json j_nonarray(json::value_t::number_float);
    const json j_nonarray_const(j_nonarray);
    CHECK_THROWS_WITH_AS(j_nonarray[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with number", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray_const[0], "[json.exception.type_error.305] cannot use operator[] with a numeric argument with number", json::type_error&);
}

class ElementAccess1ArrayRemoveSpecifiedElementTest : public ElementAccess1ArrayTest{};


TEST_F(ElementAccess1ArrayRemoveSpecifiedElementTest, RemoveElementByIndex)
{
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        jarray.erase(0);
        CHECK(jarray == json({1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}}));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        jarray.erase(1);
        CHECK(jarray == json({1, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}}));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        jarray.erase(2);
        CHECK(jarray == json({1, 1u, nullptr, "string", 42.23, json::object(), {1, 2, 3}}));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        jarray.erase(3);
        CHECK(jarray == json({1, 1u, true, "string", 42.23, json::object(), {1, 2, 3}}));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        jarray.erase(4);
        CHECK(jarray == json({1, 1u, true, nullptr, 42.23, json::object(), {1, 2, 3}}));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        jarray.erase(5);
        CHECK(jarray == json({1, 1u, true, nullptr, "string", json::object(), {1, 2, 3}}));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        jarray.erase(6);
        CHECK(jarray == json({1, 1u, true, nullptr, "string", 42.23, {1, 2, 3}}));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        jarray.erase(7);
        CHECK(jarray == json({1, 1u, true, nullptr, "string", 42.23, json::object()}));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        CHECK_THROWS_WITH_AS(jarray.erase(8), "[json.exception.out_of_range.401] array index 8 is out of range", json::out_of_range&);
    }
}

class ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIteratorTest : public ElementAccess1ArrayTest{};


TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIteratorTest, EraseBegin)
{
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json::iterator it2 = jarray.erase(jarray.begin());
        CHECK(jarray == json({1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}}));
        CHECK(*it2 == json(1u));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json::const_iterator it2 = jarray.erase(jarray.cbegin());
        CHECK(jarray == json({1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}}));
        CHECK(*it2 == json(1u));
    }
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIteratorTest, EraseBeginEnd)
{
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json::iterator it2 = jarray.erase(jarray.begin(), jarray.end());
        CHECK(jarray == json::array());
        CHECK(it2 == jarray.end());
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json::const_iterator it2 = jarray.erase(jarray.cbegin(), jarray.cend());
        CHECK(jarray == json::array());
        CHECK(it2 == jarray.cend());
    }
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIteratorTest, EraseBeginBegin)
{
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json::iterator it2 = jarray.erase(jarray.begin(), jarray.begin());
        CHECK(jarray == json({1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}}));
        CHECK(*it2 == json(1));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json::const_iterator it2 = jarray.erase(jarray.cbegin(), jarray.cbegin());
        CHECK(jarray == json({1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}}));
        CHECK(*it2 == json(1));
    }
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIteratorTest, EraseAtOffset)
{
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json::iterator it = jarray.begin() + 4;
        json::iterator it2 = jarray.erase(it);
        CHECK(jarray == json({1, 1u, true, nullptr, 42.23, json::object(), {1, 2, 3}}));
        CHECK(*it2 == json(42.23));
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json::const_iterator it = jarray.cbegin() + 4;
        json::const_iterator it2 = jarray.erase(it);
        CHECK(jarray == json({1, 1u, true, nullptr, 42.23, json::object(), {1, 2, 3}}));
        CHECK(*it2 == json(42.23));
    }
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIteratorTest, EraseSubrange)
{
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json::iterator it2 = jarray.erase(jarray.begin() + 3, jarray.begin() + 6);
        CHECK(jarray == json({1, 1u, true, json::object(), {1, 2, 3}}));
        CHECK(*it2 == json::object());
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json::const_iterator it2 = jarray.erase(jarray.cbegin() + 3, jarray.cbegin() + 6);
        CHECK(jarray == json({1, 1u, true, json::object(), {1, 2, 3}}));
        CHECK(*it2 == json::object());
    }
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIteratorTest, DifferentArrays)
{
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json jarray2 = {"foo", "bar"};

        CHECK_THROWS_WITH_AS(jarray.erase(jarray2.begin()),
                             "[json.exception.invalid_iterator.202] iterator does not fit current value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jarray.erase(jarray.begin(), jarray2.end()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jarray.erase(jarray2.begin(), jarray.end()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jarray.erase(jarray2.begin(), jarray2.end()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", json::invalid_iterator&);
    }
    {
        json jarray = {1, 1u, true, nullptr, "string", 42.23, json::object(), {1, 2, 3}};
        json jarray2 = {"foo", "bar"};

        CHECK_THROWS_WITH_AS(jarray.erase(jarray2.cbegin()),
                             "[json.exception.invalid_iterator.202] iterator does not fit current value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jarray.erase(jarray.cbegin(), jarray2.cend()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jarray.erase(jarray2.cbegin(), jarray.cend()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(jarray.erase(jarray2.cbegin(), jarray2.cend()),
                             "[json.exception.invalid_iterator.203] iterators do not fit current value", json::invalid_iterator&);
    }
}

class ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIndexInNonArrayTypeTest : public ElementAccess1ArrayTest{};


TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIndexInNonArrayTypeTest, Null)
{
    json j_nonobject(json::value_t::null);
    CHECK_THROWS_WITH_AS(j_nonobject.erase(0), "[json.exception.type_error.307] cannot use erase() with null", json::type_error&);
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIndexInNonArrayTypeTest, Boolean)
{
    json j_nonobject(json::value_t::boolean);
    CHECK_THROWS_WITH_AS(j_nonobject.erase(0), "[json.exception.type_error.307] cannot use erase() with boolean", json::type_error&);
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIndexInNonArrayTypeTest, String)
{
    json j_nonobject(json::value_t::string);
    CHECK_THROWS_WITH_AS(j_nonobject.erase(0), "[json.exception.type_error.307] cannot use erase() with string", json::type_error&);
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIndexInNonArrayTypeTest, Object)
{
    json j_nonobject(json::value_t::object);
    CHECK_THROWS_WITH_AS(j_nonobject.erase(0), "[json.exception.type_error.307] cannot use erase() with object", json::type_error&);
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIndexInNonArrayTypeTest, NumberInteger)
{
    json j_nonobject(json::value_t::number_integer);
    CHECK_THROWS_WITH_AS(j_nonobject.erase(0), "[json.exception.type_error.307] cannot use erase() with number", json::type_error&);
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIndexInNonArrayTypeTest, NumberUnsigned)
{
    json j_nonobject(json::value_t::number_unsigned);
    CHECK_THROWS_WITH_AS(j_nonobject.erase(0), "[json.exception.type_error.307] cannot use erase() with number", json::type_error&);
}

TEST_F(ElementAccess1ArrayRemoveSpecifiedElementRemoveElementByIndexInNonArrayTypeTest, NumberFloatingPoint)
{
    json j_nonobject(json::value_t::number_float);
    CHECK_THROWS_WITH_AS(j_nonobject.erase(0), "[json.exception.type_error.307] cannot use erase() with number", json::type_error&);
}







TEST(ElementAccess1OtherValuesFrontAndBackTest, Null)
{
    {
        json j;
        CHECK_THROWS_WITH_AS(j.front(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.back(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    }
    {
        const json j{};
        CHECK_THROWS_WITH_AS(j.front(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.back(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    }
}

TEST(ElementAccess1OtherValuesFrontAndBackTest, String)
{
    {
        json j = "foo";
        CHECK(j.front() == j);
        CHECK(j.back() == j);
    }
    {
        const json j = "bar";
        CHECK(j.front() == j);
        CHECK(j.back() == j);
    }
}

TEST(ElementAccess1OtherValuesFrontAndBackTest, NumberBoolean)
{
    {
        json j = false;
        CHECK(j.front() == j);
        CHECK(j.back() == j);
    }
    {
        const json j = true;
        CHECK(j.front() == j);
        CHECK(j.back() == j);
    }
}

TEST(ElementAccess1OtherValuesFrontAndBackTest, NumberInteger)
{
    {
        json j = 17;
        CHECK(j.front() == j);
        CHECK(j.back() == j);
    }
    {
        const json j = 17;
        CHECK(j.front() == j);
        CHECK(j.back() == j);
    }
}

TEST(ElementAccess1OtherValuesFrontAndBackTest, NumberUnsigned)
{
    {
        json j = 17u;
        CHECK(j.front() == j);
        CHECK(j.back() == j);
    }
    {
        const json j = 17u;
        CHECK(j.front() == j);
        CHECK(j.back() == j);
    }
}

TEST(ElementAccess1OtherValuesFrontAndBackTest, NumberFloatingPoint)
{
    {
        json j = 23.42;
        CHECK(j.front() == j);
        CHECK(j.back() == j);
    }
    {
        const json j = 23.42;
        CHECK(j.front() == j);
        CHECK(j.back() == j);
    }
}




TEST(ElementAccess1OtherValuesEraseWithOneValidIteratorTest, Null)
{
    {
        json j;
        CHECK_THROWS_WITH_AS(j.erase(j.begin()), "[json.exception.type_error.307] cannot use erase() with null", json::type_error&);
    }
    {
        json j;
        CHECK_THROWS_WITH_AS(j.erase(j.begin()),
                             "[json.exception.type_error.307] cannot use erase() with null", json::type_error&);
    }
}

TEST(ElementAccess1OtherValuesEraseWithOneValidIteratorTest, String)
{
    {
        json j = "foo";
        json::iterator it = j.erase(j.begin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = "bar";
        json::const_iterator it = j.erase(j.cbegin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}

TEST(ElementAccess1OtherValuesEraseWithOneValidIteratorTest, NumberBoolean)
{
    {
        json j = false;
        json::iterator it = j.erase(j.begin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = true;
        json::const_iterator it = j.erase(j.cbegin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}

TEST(ElementAccess1OtherValuesEraseWithOneValidIteratorTest, NumberInteger)
{
    {
        json j = 17;
        json::iterator it = j.erase(j.begin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = 17;
        json::const_iterator it = j.erase(j.cbegin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}

TEST(ElementAccess1OtherValuesEraseWithOneValidIteratorTest, NumberUnsigned)
{
    {
        json j = 17u;
        json::iterator it = j.erase(j.begin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = 17u;
        json::const_iterator it = j.erase(j.cbegin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}

TEST(ElementAccess1OtherValuesEraseWithOneValidIteratorTest, NumberFloatingPoint)
{
    {
        json j = 23.42;
        json::iterator it = j.erase(j.begin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = 23.42;
        json::const_iterator it = j.erase(j.cbegin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}

TEST(ElementAccess1OtherValuesEraseWithOneValidIteratorTest, Binary)
{
    {
        json j = json::binary({1, 2, 3});
        json::iterator it = j.erase(j.begin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = json::binary({1, 2, 3});
        json::const_iterator it = j.erase(j.cbegin());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}




TEST(ElementAccess1OtherValuesEraseWithOneInvalidIteratorTest, String)
{
    {
        json j = "foo";
        CHECK_THROWS_WITH_AS(j.erase(j.end()), "[json.exception.invalid_iterator.205] iterator out of range", json::invalid_iterator&);
    }
    {
        json j = "bar";
        CHECK_THROWS_WITH_AS(j.erase(j.cend()), "[json.exception.invalid_iterator.205] iterator out of range", json::invalid_iterator&);
    }
}

TEST(ElementAccess1OtherValuesEraseWithOneInvalidIteratorTest, NumberBoolean)
{
    {
        json j = false;
        CHECK_THROWS_WITH_AS(j.erase(j.end()), "[json.exception.invalid_iterator.205] iterator out of range", json::invalid_iterator&);
    }
    {
        json j = true;
        CHECK_THROWS_WITH_AS(j.erase(j.cend()), "[json.exception.invalid_iterator.205] iterator out of range", json::invalid_iterator&);
    }
}

TEST(ElementAccess1OtherValuesEraseWithOneInvalidIteratorTest, NumberInteger)
{
    {
        json j = 17;
        CHECK_THROWS_WITH_AS(j.erase(j.end()), "[json.exception.invalid_iterator.205] iterator out of range", json::invalid_iterator&);
    }
    {
        json j = 17;
        CHECK_THROWS_WITH_AS(j.erase(j.cend()), "[json.exception.invalid_iterator.205] iterator out of range", json::invalid_iterator&);
    }
}

TEST(ElementAccess1OtherValuesEraseWithOneInvalidIteratorTest, NumberUnsigned)
{
    {
        json j = 17u;
        CHECK_THROWS_WITH_AS(j.erase(j.end()), "[json.exception.invalid_iterator.205] iterator out of range", json::invalid_iterator&);
    }
    {
        json j = 17u;
        CHECK_THROWS_WITH_AS(j.erase(j.cend()), "[json.exception.invalid_iterator.205] iterator out of range", json::invalid_iterator&);
    }
}

TEST(ElementAccess1OtherValuesEraseWithOneInvalidIteratorTest, NumberFloatingPoint)
{
    {
        json j = 23.42;
        CHECK_THROWS_WITH_AS(j.erase(j.end()), "[json.exception.invalid_iterator.205] iterator out of range", json::invalid_iterator&);
    }
    {
        json j = 23.42;
        CHECK_THROWS_WITH_AS(j.erase(j.cend()), "[json.exception.invalid_iterator.205] iterator out of range", json::invalid_iterator&);
    }
}




TEST(ElementAccess1OtherValuesEraseWithTwoValidIteratorsTest, Null)
{
    {
        json j;
        CHECK_THROWS_WITH_AS(j.erase(j.begin(), j.end()), "[json.exception.type_error.307] cannot use erase() with null", json::type_error&);
    }
    {
        json j;
        CHECK_THROWS_WITH_AS(j.erase(j.cbegin(), j.cend()), "[json.exception.type_error.307] cannot use erase() with null", json::type_error&);
    }
}

TEST(ElementAccess1OtherValuesEraseWithTwoValidIteratorsTest, String)
{
    {
        json j = "foo";
        json::iterator it = j.erase(j.begin(), j.end());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = "bar";
        json::const_iterator it = j.erase(j.cbegin(), j.cend());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}

TEST(ElementAccess1OtherValuesEraseWithTwoValidIteratorsTest, NumberBoolean)
{
    {
        json j = false;
        json::iterator it = j.erase(j.begin(), j.end());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = true;
        json::const_iterator it = j.erase(j.cbegin(), j.cend());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}

TEST(ElementAccess1OtherValuesEraseWithTwoValidIteratorsTest, NumberInteger)
{
    {
        json j = 17;
        json::iterator it = j.erase(j.begin(), j.end());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = 17;
        json::const_iterator it = j.erase(j.cbegin(), j.cend());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}

TEST(ElementAccess1OtherValuesEraseWithTwoValidIteratorsTest, NumberUnsigned)
{
    {
        json j = 17u;
        json::iterator it = j.erase(j.begin(), j.end());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = 17u;
        json::const_iterator it = j.erase(j.cbegin(), j.cend());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}

TEST(ElementAccess1OtherValuesEraseWithTwoValidIteratorsTest, NumberFloatingPoint)
{
    {
        json j = 23.42;
        json::iterator it = j.erase(j.begin(), j.end());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = 23.42;
        json::const_iterator it = j.erase(j.cbegin(), j.cend());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}

TEST(ElementAccess1OtherValuesEraseWithTwoValidIteratorsTest, Binary)
{
    {
        json j = json::binary({1, 2, 3});
        json::iterator it = j.erase(j.begin(), j.end());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
    {
        json j = json::binary({1, 2, 3});
        json::const_iterator it = j.erase(j.cbegin(), j.cend());
        CHECK(j.type() == json::value_t::null);
        CHECK(it == j.end());
    }
}




TEST(ElementAccess1OtherValuesEraseWithTwoInvalidIteratorsTest, String)
{
    {
        json j = "foo";
        CHECK_THROWS_WITH_AS(j.erase(j.end(), j.end()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.erase(j.begin(), j.begin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
    {
        json j = "bar";
        CHECK_THROWS_WITH_AS(j.erase(j.cend(), j.cend()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.erase(j.cbegin(), j.cbegin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
}

TEST(ElementAccess1OtherValuesEraseWithTwoInvalidIteratorsTest, NumberBoolean)
{
    {
        json j = false;
        CHECK_THROWS_WITH_AS(j.erase(j.end(), j.end()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.erase(j.begin(), j.begin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
    {
        json j = true;
        CHECK_THROWS_WITH_AS(j.erase(j.cend(), j.cend()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.erase(j.cbegin(), j.cbegin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
}

TEST(ElementAccess1OtherValuesEraseWithTwoInvalidIteratorsTest, NumberInteger)
{
    {
        json j = 17;
        CHECK_THROWS_WITH_AS(j.erase(j.end(), j.end()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.erase(j.begin(), j.begin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
    {
        json j = 17;
        CHECK_THROWS_WITH_AS(j.erase(j.cend(), j.cend()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.erase(j.cbegin(), j.cbegin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
}

TEST(ElementAccess1OtherValuesEraseWithTwoInvalidIteratorsTest, NumberUnsigned)
{
    {
        json j = 17u;
        CHECK_THROWS_WITH_AS(j.erase(j.end(), j.end()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.erase(j.begin(), j.begin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
    {
        json j = 17u;
        CHECK_THROWS_WITH_AS(j.erase(j.cend(), j.cend()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.erase(j.cbegin(), j.cbegin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
}

TEST(ElementAccess1OtherValuesEraseWithTwoInvalidIteratorsTest, NumberFloatingPoint)
{
    {
        json j = 23.42;
        CHECK_THROWS_WITH_AS(j.erase(j.end(), j.end()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.erase(j.begin(), j.begin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
    {
        json j = 23.42;
        CHECK_THROWS_WITH_AS(j.erase(j.cend(), j.cend()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
        CHECK_THROWS_WITH_AS(j.erase(j.cbegin(), j.cbegin()), "[json.exception.invalid_iterator.204] iterators out of range", json::invalid_iterator&);
    }
}

