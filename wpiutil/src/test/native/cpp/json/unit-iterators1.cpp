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







TEST(Iterators1BasicBehaviorTest, Uninitialized)
{
    json::iterator it;
    CHECK(it.m_object == nullptr);

    json::const_iterator cit;
    CHECK(cit.m_object == nullptr);
}

class Iterators1BasicBehaviorBooleanTest : public ::testing::Test {
 protected:
    Iterators1BasicBehaviorBooleanTest() : j(true), j_const(j) {}

    json j;
    json j_const;
};


TEST_F(Iterators1BasicBehaviorBooleanTest, JsonBeginEnd)
{
    json::iterator it = j.begin();
    CHECK(it != j.end());
    CHECK(*it == j);

    it++;
    CHECK(it != j.begin());
    CHECK(it == j.end());

    it--;
    CHECK(it == j.begin());
    CHECK(it != j.end());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.begin());
    CHECK(it == j.end());

    --it;
    CHECK(it == j.begin());
    CHECK(it != j.end());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorBooleanTest, ConstJsonBeginEnd)
{
    json::const_iterator it = j_const.begin();
    CHECK(it != j_const.end());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.begin());
    CHECK(it == j_const.end());

    it--;
    CHECK(it == j_const.begin());
    CHECK(it != j_const.end());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.begin());
    CHECK(it == j_const.end());

    --it;
    CHECK(it == j_const.begin());
    CHECK(it != j_const.end());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorBooleanTest, JsonCbeginCend)
{
    json::const_iterator it = j.cbegin();
    CHECK(it != j.cend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.cbegin());
    CHECK(it == j.cend());

    it--;
    CHECK(it == j.cbegin());
    CHECK(it != j.cend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.cbegin());
    CHECK(it == j.cend());

    --it;
    CHECK(it == j.cbegin());
    CHECK(it != j.cend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorBooleanTest, ConstJsonCbeginCend)
{
    json::const_iterator it = j_const.cbegin();
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.cbegin());
    CHECK(it == j_const.cend());

    it--;
    CHECK(it == j_const.cbegin());
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.cbegin());
    CHECK(it == j_const.cend());

    --it;
    CHECK(it == j_const.cbegin());
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorBooleanTest, JsonRbeginRend)
{
    json::reverse_iterator it = j.rbegin();
    CHECK(it != j.rend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.rbegin());
    CHECK(it == j.rend());

    it--;
    CHECK(it == j.rbegin());
    CHECK(it != j.rend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.rbegin());
    CHECK(it == j.rend());

    --it;
    CHECK(it == j.rbegin());
    CHECK(it != j.rend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorBooleanTest, JsonCrbeginCrend)
{
    json::const_reverse_iterator it = j.crbegin();
    CHECK(it != j.crend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.crbegin());
    CHECK(it == j.crend());

    it--;
    CHECK(it == j.crbegin());
    CHECK(it != j.crend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.crbegin());
    CHECK(it == j.crend());

    --it;
    CHECK(it == j.crbegin());
    CHECK(it != j.crend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorBooleanTest, ConstJsonCrbeginCrend)
{
    json::const_reverse_iterator it = j_const.crbegin();
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.crbegin());
    CHECK(it == j_const.crend());

    it--;
    CHECK(it == j_const.crbegin());
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.crbegin());
    CHECK(it == j_const.crend());

    --it;
    CHECK(it == j_const.crbegin());
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);
}

class Iterators1BasicBehaviorBooleanAdditionalTestsTest : public Iterators1BasicBehaviorBooleanTest{};


TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, NotBeginNotBegin)
{
    CHECK(!(j.begin() != j.begin()));
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, NotEndNotEnd)
{
    CHECK(!(j.end() != j.end()));
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, BeginEnd)
{
    CHECK(j.begin() < j.end());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, BeginEnd2)
{
    CHECK(j.begin() <= j.end());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, EndBegin)
{
    CHECK(j.end() > j.begin());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, EndBegin2)
{
    CHECK(j.end() >= j.begin());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, EndEqualsEnd)
{
    CHECK(j.end() == j.end());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, EndEnd)
{
    CHECK(j.end() <= j.end());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, BeginEqualsBegin)
{
    CHECK(j.begin() == j.begin());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, BeginBegin)
{
    CHECK(j.begin() <= j.begin());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, BeginBegin2)
{
    CHECK(j.begin() >= j.begin());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, NotBeginEqualsEnd)
{
    CHECK(!(j.begin() == j.end()));
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, BeginNotEnd)
{
    CHECK(j.begin() != j.end());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, Begin1EqualsEnd)
{
    CHECK(j.begin() + 1 == j.end());
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, BeginEqualsEnd1)
{
    CHECK(j.begin() == j.end() - 1);
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, BeginNotEnd1)
{
    CHECK(j.begin() != j.end() + 1);
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, EndNotEnd1)
{
    CHECK(j.end() != j.end() + 1);
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, Begin1NotBegin2)
{
    CHECK(j.begin() + 1 != j.begin() + 2);
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, Begin1Begin2)
{
    CHECK(j.begin() + 1 < j.begin() + 2);
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, Begin1Begin22)
{
    CHECK(j.begin() + 1 <= j.begin() + 2);
}

TEST_F(Iterators1BasicBehaviorBooleanAdditionalTestsTest, End1NotEnd2)
{
    CHECK(j.end() + 1 != j.end() + 2);
}

TEST_F(Iterators1BasicBehaviorBooleanTest, KeyValue)
{
    auto it = j.begin();
    auto cit = j_const.cbegin();
    CHECK_THROWS_WITH_AS(it.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(it.value() == json(true));
    CHECK_THROWS_WITH_AS(cit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(cit.value() == json(true));

    auto rit = j.rend();
    auto crit = j.crend();
    CHECK_THROWS_WITH_AS(rit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(rit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

class Iterators1BasicBehaviorStringTest : public ::testing::Test {
 protected:
    Iterators1BasicBehaviorStringTest() : j("hello world"), j_const(j) {}

    json j;
    json j_const;
};


TEST_F(Iterators1BasicBehaviorStringTest, JsonBeginEnd)
{
    json::iterator it = j.begin();
    CHECK(it != j.end());
    CHECK(*it == j);

    it++;
    CHECK(it != j.begin());
    CHECK(it == j.end());

    it--;
    CHECK(it == j.begin());
    CHECK(it != j.end());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.begin());
    CHECK(it == j.end());

    --it;
    CHECK(it == j.begin());
    CHECK(it != j.end());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorStringTest, ConstJsonBeginEnd)
{
    json::const_iterator it = j_const.begin();
    CHECK(it != j_const.end());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.begin());
    CHECK(it == j_const.end());

    it--;
    CHECK(it == j_const.begin());
    CHECK(it != j_const.end());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.begin());
    CHECK(it == j_const.end());

    --it;
    CHECK(it == j_const.begin());
    CHECK(it != j_const.end());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorStringTest, JsonCbeginCend)
{
    json::const_iterator it = j.cbegin();
    CHECK(it != j.cend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.cbegin());
    CHECK(it == j.cend());

    it--;
    CHECK(it == j.cbegin());
    CHECK(it != j.cend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.cbegin());
    CHECK(it == j.cend());

    --it;
    CHECK(it == j.cbegin());
    CHECK(it != j.cend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorStringTest, ConstJsonCbeginCend)
{
    json::const_iterator it = j_const.cbegin();
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.cbegin());
    CHECK(it == j_const.cend());

    it--;
    CHECK(it == j_const.cbegin());
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.cbegin());
    CHECK(it == j_const.cend());

    --it;
    CHECK(it == j_const.cbegin());
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorStringTest, JsonRbeginRend)
{
    json::reverse_iterator it = j.rbegin();
    CHECK(it != j.rend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.rbegin());
    CHECK(it == j.rend());

    it--;
    CHECK(it == j.rbegin());
    CHECK(it != j.rend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.rbegin());
    CHECK(it == j.rend());

    --it;
    CHECK(it == j.rbegin());
    CHECK(it != j.rend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorStringTest, JsonCrbeginCrend)
{
    json::const_reverse_iterator it = j.crbegin();
    CHECK(it != j.crend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.crbegin());
    CHECK(it == j.crend());

    it--;
    CHECK(it == j.crbegin());
    CHECK(it != j.crend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.crbegin());
    CHECK(it == j.crend());

    --it;
    CHECK(it == j.crbegin());
    CHECK(it != j.crend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorStringTest, ConstJsonCrbeginCrend)
{
    json::const_reverse_iterator it = j_const.crbegin();
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.crbegin());
    CHECK(it == j_const.crend());

    it--;
    CHECK(it == j_const.crbegin());
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.crbegin());
    CHECK(it == j_const.crend());

    --it;
    CHECK(it == j_const.crbegin());
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorStringTest, KeyValue)
{
    auto it = j.begin();
    auto cit = j_const.cbegin();
    CHECK_THROWS_WITH_AS(it.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(it.value() == json("hello world"));
    CHECK_THROWS_WITH_AS(cit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(cit.value() == json("hello world"));

    auto rit = j.rend();
    auto crit = j.crend();
    CHECK_THROWS_WITH_AS(rit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(rit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

class Iterators1BasicBehaviorArrayTest : public ::testing::Test {
 protected:
    Iterators1BasicBehaviorArrayTest() : j({1, 2, 3}), j_const(j) {}

    json j;
    json j_const;
};


TEST_F(Iterators1BasicBehaviorArrayTest, JsonBeginEnd)
{
    json::iterator it_begin = j.begin();
    json::iterator it_end = j.end();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j[0]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[1]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[2]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorArrayTest, ConstJsonBeginEnd)
{
    json::const_iterator it_begin = j_const.begin();
    json::const_iterator it_end = j_const.end();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j_const[0]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j_const[1]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j_const[2]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorArrayTest, JsonCbeginCend)
{
    json::const_iterator it_begin = j.cbegin();
    json::const_iterator it_end = j.cend();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j[0]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[1]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[2]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorArrayTest, ConstJsonCbeginCend)
{
    json::const_iterator it_begin = j_const.cbegin();
    json::const_iterator it_end = j_const.cend();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j[0]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[1]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[2]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorArrayTest, JsonRbeginRend)
{
    json::reverse_iterator it_begin = j.rbegin();
    json::reverse_iterator it_end = j.rend();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j[2]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[1]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[0]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorArrayTest, JsonCrbeginCrend)
{
    json::const_reverse_iterator it_begin = j.crbegin();
    json::const_reverse_iterator it_end = j.crend();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j[2]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[1]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[0]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorArrayTest, ConstJsonCrbeginCrend)
{
    json::const_reverse_iterator it_begin = j_const.crbegin();
    json::const_reverse_iterator it_end = j_const.crend();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j[2]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[1]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j[0]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorArrayTest, KeyValue)
{
    auto it = j.begin();
    auto cit = j_const.cbegin();
    CHECK_THROWS_WITH_AS(it.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(it.value() == json(1));
    CHECK_THROWS_WITH_AS(cit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(cit.value() == json(1));
}

class Iterators1BasicBehaviorObjectTest : public ::testing::Test {
 protected:
    Iterators1BasicBehaviorObjectTest() : j({{"A", 1}, {"B", 2}, {"C", 3}}), j_const(j) {}

    json j;
    json j_const;
};


TEST_F(Iterators1BasicBehaviorObjectTest, JsonBeginEnd)
{
    json::iterator it_begin = j.begin();
    json::iterator it_end = j.end();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j["A"]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j["B"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j["C"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorObjectTest, ConstJsonBeginEnd)
{
    json::const_iterator it_begin = j_const.begin();
    json::const_iterator it_end = j_const.end();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j_const["A"]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j_const["B"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j_const["C"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorObjectTest, JsonCbeginCend)
{
    json::const_iterator it_begin = j.cbegin();
    json::const_iterator it_end = j.cend();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j["A"]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j["B"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j["C"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorObjectTest, ConstJsonCbeginCend)
{
    json::const_iterator it_begin = j_const.cbegin();
    json::const_iterator it_end = j_const.cend();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j_const["A"]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j_const["B"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j_const["C"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorObjectTest, JsonRbeginRend)
{
    json::reverse_iterator it_begin = j.rbegin();
    json::reverse_iterator it_end = j.rend();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j["C"]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j["B"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j["A"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorObjectTest, JsonCrbeginCrend)
{
    json::const_reverse_iterator it_begin = j.crbegin();
    json::const_reverse_iterator it_end = j.crend();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j["C"]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j["B"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j["A"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorObjectTest, ConstJsonCrbeginCrend)
{
    json::const_reverse_iterator it_begin = j_const.crbegin();
    json::const_reverse_iterator it_end = j_const.crend();

    auto it = it_begin;
    CHECK(it != it_end);
    CHECK(*it == j["C"]);

    it++;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j["B"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it != it_end);
    CHECK(*it == j["A"]);

    ++it;
    CHECK(it != it_begin);
    CHECK(it == it_end);
}

TEST_F(Iterators1BasicBehaviorObjectTest, KeyValue)
{
    auto it = j.begin();
    auto cit = j_const.cbegin();
    CHECK(it.key() == "A");
    CHECK(it.value() == json(1));
    CHECK(cit.key() == "A");
    CHECK(cit.value() == json(1));
}

class Iterators1BasicBehaviorNumberIntegerTest : public ::testing::Test {
 protected:
    Iterators1BasicBehaviorNumberIntegerTest() : j(23), j_const(j) {}

    json j;
    json j_const;
};


TEST_F(Iterators1BasicBehaviorNumberIntegerTest, JsonBeginEnd)
{
    json::iterator it = j.begin();
    CHECK(it != j.end());
    CHECK(*it == j);

    it++;
    CHECK(it != j.begin());
    CHECK(it == j.end());

    it--;
    CHECK(it == j.begin());
    CHECK(it != j.end());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.begin());
    CHECK(it == j.end());

    --it;
    CHECK(it == j.begin());
    CHECK(it != j.end());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberIntegerTest, ConstJsonBeginEnd)
{
    json::const_iterator it = j_const.begin();
    CHECK(it != j_const.end());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.begin());
    CHECK(it == j_const.end());

    it--;
    CHECK(it == j_const.begin());
    CHECK(it != j_const.end());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.begin());
    CHECK(it == j_const.end());

    --it;
    CHECK(it == j_const.begin());
    CHECK(it != j_const.end());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorNumberIntegerTest, JsonCbeginCend)
{
    json::const_iterator it = j.cbegin();
    CHECK(it != j.cend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.cbegin());
    CHECK(it == j.cend());

    it--;
    CHECK(it == j.cbegin());
    CHECK(it != j.cend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.cbegin());
    CHECK(it == j.cend());

    --it;
    CHECK(it == j.cbegin());
    CHECK(it != j.cend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberIntegerTest, ConstJsonCbeginCend)
{
    json::const_iterator it = j_const.cbegin();
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.cbegin());
    CHECK(it == j_const.cend());

    it--;
    CHECK(it == j_const.cbegin());
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.cbegin());
    CHECK(it == j_const.cend());

    --it;
    CHECK(it == j_const.cbegin());
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorNumberIntegerTest, JsonRbeginRend)
{
    json::reverse_iterator it = j.rbegin();
    CHECK(it != j.rend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.rbegin());
    CHECK(it == j.rend());

    it--;
    CHECK(it == j.rbegin());
    CHECK(it != j.rend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.rbegin());
    CHECK(it == j.rend());

    --it;
    CHECK(it == j.rbegin());
    CHECK(it != j.rend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberIntegerTest, JsonCrbeginCrend)
{
    json::const_reverse_iterator it = j.crbegin();
    CHECK(it != j.crend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.crbegin());
    CHECK(it == j.crend());

    it--;
    CHECK(it == j.crbegin());
    CHECK(it != j.crend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.crbegin());
    CHECK(it == j.crend());

    --it;
    CHECK(it == j.crbegin());
    CHECK(it != j.crend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberIntegerTest, ConstJsonCrbeginCrend)
{
    json::const_reverse_iterator it = j_const.crbegin();
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.crbegin());
    CHECK(it == j_const.crend());

    it--;
    CHECK(it == j_const.crbegin());
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.crbegin());
    CHECK(it == j_const.crend());

    --it;
    CHECK(it == j_const.crbegin());
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorNumberIntegerTest, KeyValue)
{
    auto it = j.begin();
    auto cit = j_const.cbegin();
    CHECK_THROWS_WITH_AS(it.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(it.value() == json(23));
    CHECK_THROWS_WITH_AS(cit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(cit.value() == json(23));

    auto rit = j.rend();
    auto crit = j.crend();
    CHECK_THROWS_WITH_AS(rit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(rit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

class Iterators1BasicBehaviorNumberUnsignedTest : public ::testing::Test {
 protected:
    Iterators1BasicBehaviorNumberUnsignedTest() : j(23u), j_const(j) {}

    json j;
    json j_const;
};


TEST_F(Iterators1BasicBehaviorNumberUnsignedTest, JsonBeginEnd)
{
    json::iterator it = j.begin();
    CHECK(it != j.end());
    CHECK(*it == j);

    it++;
    CHECK(it != j.begin());
    CHECK(it == j.end());

    it--;
    CHECK(it == j.begin());
    CHECK(it != j.end());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.begin());
    CHECK(it == j.end());

    --it;
    CHECK(it == j.begin());
    CHECK(it != j.end());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberUnsignedTest, ConstJsonBeginEnd)
{
    json::const_iterator it = j_const.begin();
    CHECK(it != j_const.end());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.begin());
    CHECK(it == j_const.end());

    it--;
    CHECK(it == j_const.begin());
    CHECK(it != j_const.end());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.begin());
    CHECK(it == j_const.end());

    --it;
    CHECK(it == j_const.begin());
    CHECK(it != j_const.end());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorNumberUnsignedTest, JsonCbeginCend)
{
    json::const_iterator it = j.cbegin();
    CHECK(it != j.cend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.cbegin());
    CHECK(it == j.cend());

    it--;
    CHECK(it == j.cbegin());
    CHECK(it != j.cend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.cbegin());
    CHECK(it == j.cend());

    --it;
    CHECK(it == j.cbegin());
    CHECK(it != j.cend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberUnsignedTest, ConstJsonCbeginCend)
{
    json::const_iterator it = j_const.cbegin();
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.cbegin());
    CHECK(it == j_const.cend());

    it--;
    CHECK(it == j_const.cbegin());
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.cbegin());
    CHECK(it == j_const.cend());

    --it;
    CHECK(it == j_const.cbegin());
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorNumberUnsignedTest, JsonRbeginRend)
{
    json::reverse_iterator it = j.rbegin();
    CHECK(it != j.rend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.rbegin());
    CHECK(it == j.rend());

    it--;
    CHECK(it == j.rbegin());
    CHECK(it != j.rend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.rbegin());
    CHECK(it == j.rend());

    --it;
    CHECK(it == j.rbegin());
    CHECK(it != j.rend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberUnsignedTest, JsonCrbeginCrend)
{
    json::const_reverse_iterator it = j.crbegin();
    CHECK(it != j.crend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.crbegin());
    CHECK(it == j.crend());

    it--;
    CHECK(it == j.crbegin());
    CHECK(it != j.crend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.crbegin());
    CHECK(it == j.crend());

    --it;
    CHECK(it == j.crbegin());
    CHECK(it != j.crend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberUnsignedTest, ConstJsonCrbeginCrend)
{
    json::const_reverse_iterator it = j_const.crbegin();
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.crbegin());
    CHECK(it == j_const.crend());

    it--;
    CHECK(it == j_const.crbegin());
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.crbegin());
    CHECK(it == j_const.crend());

    --it;
    CHECK(it == j_const.crbegin());
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorNumberUnsignedTest, KeyValue)
{
    auto it = j.begin();
    auto cit = j_const.cbegin();
    CHECK_THROWS_WITH_AS(it.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(it.value() == json(23));
    CHECK_THROWS_WITH_AS(cit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(cit.value() == json(23));

    auto rit = j.rend();
    auto crit = j.crend();
    CHECK_THROWS_WITH_AS(rit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(rit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

class Iterators1BasicBehaviorNumberFloatTest : public ::testing::Test {
 protected:
    Iterators1BasicBehaviorNumberFloatTest() : j(23.42), j_const(j) {}

    json j;
    json j_const;
};


TEST_F(Iterators1BasicBehaviorNumberFloatTest, JsonBeginEnd)
{
    json::iterator it = j.begin();
    CHECK(it != j.end());
    CHECK(*it == j);

    it++;
    CHECK(it != j.begin());
    CHECK(it == j.end());

    it--;
    CHECK(it == j.begin());
    CHECK(it != j.end());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.begin());
    CHECK(it == j.end());

    --it;
    CHECK(it == j.begin());
    CHECK(it != j.end());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberFloatTest, ConstJsonBeginEnd)
{
    json::const_iterator it = j_const.begin();
    CHECK(it != j_const.end());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.begin());
    CHECK(it == j_const.end());

    it--;
    CHECK(it == j_const.begin());
    CHECK(it != j_const.end());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.begin());
    CHECK(it == j_const.end());

    --it;
    CHECK(it == j_const.begin());
    CHECK(it != j_const.end());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorNumberFloatTest, JsonCbeginCend)
{
    json::const_iterator it = j.cbegin();
    CHECK(it != j.cend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.cbegin());
    CHECK(it == j.cend());

    it--;
    CHECK(it == j.cbegin());
    CHECK(it != j.cend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.cbegin());
    CHECK(it == j.cend());

    --it;
    CHECK(it == j.cbegin());
    CHECK(it != j.cend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberFloatTest, ConstJsonCbeginCend)
{
    json::const_iterator it = j_const.cbegin();
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.cbegin());
    CHECK(it == j_const.cend());

    it--;
    CHECK(it == j_const.cbegin());
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.cbegin());
    CHECK(it == j_const.cend());

    --it;
    CHECK(it == j_const.cbegin());
    CHECK(it != j_const.cend());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorNumberFloatTest, JsonRbeginRend)
{
    json::reverse_iterator it = j.rbegin();
    CHECK(it != j.rend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.rbegin());
    CHECK(it == j.rend());

    it--;
    CHECK(it == j.rbegin());
    CHECK(it != j.rend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.rbegin());
    CHECK(it == j.rend());

    --it;
    CHECK(it == j.rbegin());
    CHECK(it != j.rend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberFloatTest, JsonCrbeginCrend)
{
    json::const_reverse_iterator it = j.crbegin();
    CHECK(it != j.crend());
    CHECK(*it == j);

    it++;
    CHECK(it != j.crbegin());
    CHECK(it == j.crend());

    it--;
    CHECK(it == j.crbegin());
    CHECK(it != j.crend());
    CHECK(*it == j);

    ++it;
    CHECK(it != j.crbegin());
    CHECK(it == j.crend());

    --it;
    CHECK(it == j.crbegin());
    CHECK(it != j.crend());
    CHECK(*it == j);
}

TEST_F(Iterators1BasicBehaviorNumberFloatTest, ConstJsonCrbeginCrend)
{
    json::const_reverse_iterator it = j_const.crbegin();
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);

    it++;
    CHECK(it != j_const.crbegin());
    CHECK(it == j_const.crend());

    it--;
    CHECK(it == j_const.crbegin());
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);

    ++it;
    CHECK(it != j_const.crbegin());
    CHECK(it == j_const.crend());

    --it;
    CHECK(it == j_const.crbegin());
    CHECK(it != j_const.crend());
    CHECK(*it == j_const);
}

TEST_F(Iterators1BasicBehaviorNumberFloatTest, KeyValue)
{
    auto it = j.begin();
    auto cit = j_const.cbegin();
    CHECK_THROWS_WITH_AS(it.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(it.value() == json(23.42));
    CHECK_THROWS_WITH_AS(cit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK(cit.value() == json(23.42));

    auto rit = j.rend();
    auto crit = j.crend();
    CHECK_THROWS_WITH_AS(rit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(rit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

class Iterators1BasicBehaviorNullTest : public ::testing::Test {
 protected:
    Iterators1BasicBehaviorNullTest() : j(nullptr), j_const(j) {}

    json j;
    json j_const;
};


TEST_F(Iterators1BasicBehaviorNullTest, JsonBeginEnd)
{
    json::iterator it = j.begin();
    CHECK(it == j.end());
}

TEST_F(Iterators1BasicBehaviorNullTest, ConstJsonBeginEnd)
{
    json::const_iterator it_begin = j_const.begin();
    json::const_iterator it_end = j_const.end();
    CHECK(it_begin == it_end);
}

TEST_F(Iterators1BasicBehaviorNullTest, JsonCbeginCend)
{
    json::const_iterator it_begin = j.cbegin();
    json::const_iterator it_end = j.cend();
    CHECK(it_begin == it_end);
}

TEST_F(Iterators1BasicBehaviorNullTest, ConstJsonCbeginCend)
{
    json::const_iterator it_begin = j_const.cbegin();
    json::const_iterator it_end = j_const.cend();
    CHECK(it_begin == it_end);
}

TEST_F(Iterators1BasicBehaviorNullTest, JsonRbeginRend)
{
    json::reverse_iterator it = j.rbegin();
    CHECK(it == j.rend());
}

TEST_F(Iterators1BasicBehaviorNullTest, JsonCrbeginCrend)
{
    json::const_reverse_iterator it = j.crbegin();
    CHECK(it == j.crend());
}

TEST_F(Iterators1BasicBehaviorNullTest, ConstJsonCrbeginCrend)
{
    json::const_reverse_iterator it = j_const.crbegin();
    CHECK(it == j_const.crend());
}

TEST_F(Iterators1BasicBehaviorNullTest, KeyValue)
{
    auto it = j.begin();
    auto cit = j_const.cbegin();
    CHECK_THROWS_WITH_AS(it.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(it.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(cit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(cit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);

    auto rit = j.rend();
    auto crit = j.crend();
    CHECK_THROWS_WITH_AS(rit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(rit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.key(), "[json.exception.invalid_iterator.207] cannot use key() for non-object iterators", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(crit.value(), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}




TEST(Iterators1ConversionFromIteratorToConstIteratorTest, Boolean)
{
    json j = true;
    json::const_iterator it = j.begin();
    CHECK(it == j.cbegin());
    it = j.begin();
    CHECK(it == j.cbegin());
}

TEST(Iterators1ConversionFromIteratorToConstIteratorTest, String)
{
    json j = "hello world";
    json::const_iterator it = j.begin();
    CHECK(it == j.cbegin());
    it = j.begin();
    CHECK(it == j.cbegin());
}

TEST(Iterators1ConversionFromIteratorToConstIteratorTest, Array)
{
    json j = {1, 2, 3};
    json::const_iterator it = j.begin();
    CHECK(it == j.cbegin());
    it = j.begin();
    CHECK(it == j.cbegin());
}

TEST(Iterators1ConversionFromIteratorToConstIteratorTest, Object)
{
    json j = {{"A", 1}, {"B", 2}, {"C", 3}};
    json::const_iterator it = j.begin();
    CHECK(it == j.cbegin());
    it = j.begin();
    CHECK(it == j.cbegin());
}

TEST(Iterators1ConversionFromIteratorToConstIteratorTest, NumberInteger)
{
    json j = 23;
    json::const_iterator it = j.begin();
    CHECK(it == j.cbegin());
    it = j.begin();
    CHECK(it == j.cbegin());
}

TEST(Iterators1ConversionFromIteratorToConstIteratorTest, NumberUnsigned)
{
    json j = 23u;
    json::const_iterator it = j.begin();
    CHECK(it == j.cbegin());
    it = j.begin();
    CHECK(it == j.cbegin());
}

TEST(Iterators1ConversionFromIteratorToConstIteratorTest, NumberFloat)
{
    json j = 23.42;
    json::const_iterator it = j.begin();
    CHECK(it == j.cbegin());
    it = j.begin();
    CHECK(it == j.cbegin());
}

TEST(Iterators1ConversionFromIteratorToConstIteratorTest, Null)
{
    json j = nullptr;
    json::const_iterator it = j.begin();
    CHECK(it == j.cbegin());
    it = j.begin();
    CHECK(it == j.cbegin());
}

