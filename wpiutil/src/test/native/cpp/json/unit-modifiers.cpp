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







TEST(ModifiersClearTest, Boolean)
{
    json j = true;
    json k = j;

    j.clear();
    CHECK(j == json(json::value_t::boolean));
    CHECK(j == json(k.type()));
}

TEST(ModifiersClearTest, String)
{
    json j = "hello world";
    json k = j;

    j.clear();
    CHECK(j == json(json::value_t::string));
    CHECK(j == json(k.type()));
}




TEST(ModifiersClearArrayTest, EmptyArray)
{
    json j = json::array();
    json k = j;

    j.clear();
    CHECK(j.empty());
    CHECK(j == json(json::value_t::array));
    CHECK(j == json(k.type()));
}

TEST(ModifiersClearArrayTest, FilledArray)
{
    json j = {1, 2, 3};
    json k = j;

    j.clear();
    CHECK(j.empty());
    CHECK(j == json(json::value_t::array));
    CHECK(j == json(k.type()));
}




TEST(ModifiersClearObjectTest, EmptyObject)
{
    json j = json::object();
    json k = j;

    j.clear();
    CHECK(j.empty());
    CHECK(j == json(json::value_t::object));
    CHECK(j == json(k.type()));
}

TEST(ModifiersClearObjectTest, FilledObject)
{
    json j = {{"one", 1}, {"two", 2}, {"three", 3}};
    json k = j;

    j.clear();
    CHECK(j.empty());
    CHECK(j == json(json::value_t::object));
    CHECK(j == json(k.type()));
}




TEST(ModifiersClearBinaryTest, EmptyBinary)
{
    json j = json::binary({});
    json k = j;

    j.clear();
    CHECK(!j.empty());
    CHECK(j == json(json::value_t::binary));
    CHECK(j == json(k.type()));
}

TEST(ModifiersClearBinaryTest, FilledBinary)
{
    json j = json::binary({1, 2, 3, 4, 5});
    json k = j;

    j.clear();
    CHECK(!j.empty());
    CHECK(j == json(json::value_t::binary));
    CHECK(j == json(k.type()));
}

TEST(ModifiersClearTest, NumberInteger)
{
    json j = 23;
    json k = j;

    j.clear();
    CHECK(j == json(json::value_t::number_integer));
    CHECK(j == json(k.type()));
}

TEST(ModifiersClearTest, NumberUnsigned)
{
    json j = 23u;
    json k = j;

    j.clear();
    CHECK(j == json(json::value_t::number_integer));
    CHECK(j == json(k.type()));
}

TEST(ModifiersClearTest, NumberFloat)
{
    json j = 23.42;
    json k = j;

    j.clear();
    CHECK(j == json(json::value_t::number_float));
    CHECK(j == json(k.type()));
}

TEST(ModifiersClearTest, Null)
{
    json j = nullptr;
    json k = j;

    j.clear();
    CHECK(j == json(json::value_t::null));
    CHECK(j == json(k.type()));
}










TEST(ModifiersPushBackToArrayJsonTest, Null)
{
    json j;
    j.push_back(1);
    j.push_back(2);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 2}));
}

TEST(ModifiersPushBackToArrayJsonTest, Array)
{
    json j = {1, 2, 3};
    j.push_back("Hello");
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 2, 3, "Hello"}));
}

TEST(ModifiersPushBackToArrayJsonTest, OtherType)
{
    json j = 1;
    CHECK_THROWS_WITH_AS(j.push_back("Hello"), "[json.exception.type_error.308] cannot use push_back() with number", json::type_error&);
}




TEST(ModifiersPushBackToArrayConstJsonTest, Null)
{
    json j;
    json k(1);
    j.push_back(k);
    j.push_back(k);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 1}));
}

TEST(ModifiersPushBackToArrayConstJsonTest, Array)
{
    json j = {1, 2, 3};
    json k("Hello");
    j.push_back(k);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 2, 3, "Hello"}));
}

TEST(ModifiersPushBackToArrayConstJsonTest, OtherType)
{
    json j = 1;
    json k("Hello");
    CHECK_THROWS_WITH_AS(j.push_back(k), "[json.exception.type_error.308] cannot use push_back() with number", json::type_error&);
}




TEST(ModifiersPushBackToObjectTest, Null)
{
    json j;
    j.push_back(json::object_t::value_type({"one", 1}));
    j.push_back(json::object_t::value_type({"two", 2}));
    CHECK(j.type() == json::value_t::object);
    CHECK(j.size() == 2);
    CHECK(j["one"] == json(1));
    CHECK(j["two"] == json(2));
}

TEST(ModifiersPushBackToObjectTest, Object)
{
    json j(json::value_t::object);
    j.push_back(json::object_t::value_type({"one", 1}));
    j.push_back(json::object_t::value_type({"two", 2}));
    CHECK(j.size() == 2);
    CHECK(j["one"] == json(1));
    CHECK(j["two"] == json(2));
}

TEST(ModifiersPushBackToObjectTest, OtherType)
{
    json j = 1;
    json k("Hello");
    CHECK_THROWS_WITH_AS(j.push_back(json::object_t::value_type({"one", 1})), "[json.exception.type_error.308] cannot use push_back() with number", json::type_error&);
}




TEST(ModifiersPushBackWithInitializerListTest, Null)
{
    json j;
    j.push_back({"foo", "bar"});
    CHECK(j == json::array({{"foo", "bar"}}));

    json k;
    k.push_back({1, 2, 3});
    CHECK(k == json::array({{1, 2, 3}}));
}

TEST(ModifiersPushBackWithInitializerListTest, Array)
{
    json j = {1, 2, 3};
    j.push_back({"foo", "bar"});
    CHECK(j == json({1, 2, 3, {"foo", "bar"}}));

    json k = {1, 2, 3};
    k.push_back({1, 2, 3});
    CHECK(k == json({1, 2, 3, {1, 2, 3}}));
}

TEST(ModifiersPushBackWithInitializerListTest, Object)
{
    json j = {{"key1", 1}};
    j.push_back({"key2", "bar"});
    CHECK(j == json({{"key1", 1}, {"key2", "bar"}}));

    // invalid values (no string/val pair)
    CHECK_THROWS_WITH_AS(j.push_back({1}), "[json.exception.type_error.308] cannot use push_back() with object", json::type_error&);
    CHECK_THROWS_WITH_AS(j.push_back({1, 2}), "[json.exception.type_error.308] cannot use push_back() with object", json::type_error&);
    CHECK_THROWS_WITH_AS(j.push_back({1, 2, 3, 4}), "[json.exception.type_error.308] cannot use push_back() with object", json::type_error&);
}







TEST(ModifiersEmplaceBackToArrayTest, Null)
{
    json j;
    auto& x1 = j.emplace_back(1);
    CHECK(x1 == 1);
    auto& x2 = j.emplace_back(2);
    CHECK(x2 == 2);
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 2}));
}

TEST(ModifiersEmplaceBackToArrayTest, Array)
{
    json j = {1, 2, 3};
    auto& x = j.emplace_back("Hello");
    CHECK(x == "Hello");
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 2, 3, "Hello"}));
}

TEST(ModifiersEmplaceBackToArrayTest, MultipleValues)
{
    json j;
    auto& x = j.emplace_back(3, "foo");
    CHECK(x == json({"foo", "foo", "foo"}));
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({{"foo", "foo", "foo"}}));
}

TEST(ModifiersEmplaceBackTest, OtherType)
{
    json j = 1;
    CHECK_THROWS_WITH_AS(j.emplace_back("Hello"), "[json.exception.type_error.311] cannot use emplace_back() with number", json::type_error&);
}







TEST(ModifiersEmplaceToObjectTest, Null)
{
    // start with a null value
    json j;

    // add a new key
    auto res1 = j.emplace("foo", "bar");
    CHECK(res1.second == true);
    CHECK(*res1.first == "bar");

    // the null value is changed to an object
    CHECK(j.type() == json::value_t::object);

    // add a new key
    auto res2 = j.emplace("baz", "bam");
    CHECK(res2.second == true);
    CHECK(*res2.first == "bam");

    // we try to insert at given key - no change
    auto res3 = j.emplace("baz", "bad");
    CHECK(res3.second == false);
    CHECK(*res3.first == "bam");

    // the final object
    CHECK(j == json({{"baz", "bam"}, {"foo", "bar"}}));
}

TEST(ModifiersEmplaceToObjectTest, Object)
{
    // start with an object
    json j = {{"foo", "bar"}};

    // add a new key
    auto res1 = j.emplace("baz", "bam");
    CHECK(res1.second == true);
    CHECK(*res1.first == "bam");

    // add an existing key
    auto res2 = j.emplace("foo", "bad");
    CHECK(res2.second == false);
    CHECK(*res2.first == "bar");

    // check final object
    CHECK(j == json({{"baz", "bam"}, {"foo", "bar"}}));
}

TEST(ModifiersEmplaceTest, OtherType)
{
    json j = 1;
    CHECK_THROWS_WITH_AS(j.emplace("foo", "bar"), "[json.exception.type_error.311] cannot use emplace() with number", json::type_error&);
}










TEST(ModifiersOperatorToArrayJsonTest, Null)
{
    json j;
    j += 1;
    j += 2;
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 2}));
}

TEST(ModifiersOperatorToArrayJsonTest, Array)
{
    json j = {1, 2, 3};
    j += "Hello";
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 2, 3, "Hello"}));
}

TEST(ModifiersOperatorToArrayJsonTest, OtherType)
{
    json j = 1;
    CHECK_THROWS_WITH_AS(j += "Hello", "[json.exception.type_error.308] cannot use push_back() with number", json::type_error&);
}




TEST(ModifiersOperatorToArrayConstJsonTest, Null)
{
    json j;
    json k(1);
    j += k;
    j += k;
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 1}));
}

TEST(ModifiersOperatorToArrayConstJsonTest, Array)
{
    json j = {1, 2, 3};
    json k("Hello");
    j += k;
    CHECK(j.type() == json::value_t::array);
    CHECK(j == json({1, 2, 3, "Hello"}));
}

TEST(ModifiersOperatorToArrayConstJsonTest, OtherType)
{
    json j = 1;
    json k("Hello");
    CHECK_THROWS_WITH_AS(j += k, "[json.exception.type_error.308] cannot use push_back() with number", json::type_error&);
}




TEST(ModifiersOperatorToObjectTest, Null)
{
    json j;
    j += json::object_t::value_type({"one", 1});
    j += json::object_t::value_type({"two", 2});
    CHECK(j.type() == json::value_t::object);
    CHECK(j.size() == 2);
    CHECK(j["one"] == json(1));
    CHECK(j["two"] == json(2));
}

TEST(ModifiersOperatorToObjectTest, Object)
{
    json j(json::value_t::object);
    j += json::object_t::value_type({"one", 1});
    j += json::object_t::value_type({"two", 2});
    CHECK(j.size() == 2);
    CHECK(j["one"] == json(1));
    CHECK(j["two"] == json(2));
}

TEST(ModifiersOperatorToObjectTest, OtherType)
{
    json j = 1;
    json k("Hello");
    CHECK_THROWS_WITH_AS(j += json::object_t::value_type({"one", 1}), "[json.exception.type_error.308] cannot use push_back() with number", json::type_error&);
}




TEST(ModifiersOperatorWithInitializerListTest, Null)
{
    json j;
    j += {"foo", "bar"};
    CHECK(j == json::array({{"foo", "bar"}}));

    json k;
    k += {1, 2, 3};
    CHECK(k == json::array({{1, 2, 3}}));
}

TEST(ModifiersOperatorWithInitializerListTest, Array)
{
    json j = {1, 2, 3};
    j += {"foo", "bar"};
    CHECK(j == json({1, 2, 3, {"foo", "bar"}}));

    json k = {1, 2, 3};
    k += {1, 2, 3};
    CHECK(k == json({1, 2, 3, {1, 2, 3}}));
}

TEST(ModifiersOperatorWithInitializerListTest, Object)
{
    json j = {{"key1", 1}};
    j += {"key2", "bar"};
    CHECK(j == json({{"key1", 1}, {"key2", "bar"}}));

    json k = {{"key1", 1}};
    CHECK_THROWS_WITH_AS((k += {1, 2, 3, 4}), "[json.exception.type_error.308] cannot use push_back() with object", json::type_error&);
}

class ModifiersInsertTest : public ::testing::Test {
 protected:
    json j_array = {1, 2, 3, 4};
    json j_value = 5;
};


class ModifiersInsertValueAtPositionTest : public ModifiersInsertTest{};


TEST_F(ModifiersInsertValueAtPositionTest, InsertBeforeBegin)
{
    auto it = j_array.insert(j_array.begin(), j_value);
    CHECK(j_array.size() == 5);
    CHECK(*it == j_value);
    CHECK(j_array.begin() == it);
    CHECK(j_array == json({5, 1, 2, 3, 4}));
}

TEST_F(ModifiersInsertValueAtPositionTest, InsertInTheMiddle)
{
    auto it = j_array.insert(j_array.begin() + 2, j_value);
    CHECK(j_array.size() == 5);
    CHECK(*it == j_value);
    CHECK((it - j_array.begin()) == 2);
    CHECK(j_array == json({1, 2, 5, 3, 4}));
}

TEST_F(ModifiersInsertValueAtPositionTest, InsertBeforeEnd)
{
    auto it = j_array.insert(j_array.end(), j_value);
    CHECK(j_array.size() == 5);
    CHECK(*it == j_value);
    CHECK((j_array.end() - it) == 1);
    CHECK(j_array == json({1, 2, 3, 4, 5}));
}

class ModifiersInsertRvalueAtPositionTest : public ModifiersInsertTest{};


TEST_F(ModifiersInsertRvalueAtPositionTest, InsertBeforeBegin)
{
    auto it = j_array.insert(j_array.begin(), 5);
    CHECK(j_array.size() == 5);
    CHECK(*it == j_value);
    CHECK(j_array.begin() == it);
    CHECK(j_array == json({5, 1, 2, 3, 4}));
}

TEST_F(ModifiersInsertRvalueAtPositionTest, InsertInTheMiddle)
{
    auto it = j_array.insert(j_array.begin() + 2, 5);
    CHECK(j_array.size() == 5);
    CHECK(*it == j_value);
    CHECK((it - j_array.begin()) == 2);
    CHECK(j_array == json({1, 2, 5, 3, 4}));
}

TEST_F(ModifiersInsertRvalueAtPositionTest, InsertBeforeEnd)
{
    auto it = j_array.insert(j_array.end(), 5);
    CHECK(j_array.size() == 5);
    CHECK(*it == j_value);
    CHECK((j_array.end() - it) == 1);
    CHECK(j_array == json({1, 2, 3, 4, 5}));
}

class ModifiersInsertCopiesAtPositionTest : public ModifiersInsertTest{};


TEST_F(ModifiersInsertCopiesAtPositionTest, InsertBeforeBegin)
{
    auto it = j_array.insert(j_array.begin(), 3, 5);
    CHECK(j_array.size() == 7);
    CHECK(*it == j_value);
    CHECK(j_array.begin() == it);
    CHECK(j_array == json({5, 5, 5, 1, 2, 3, 4}));
}

TEST_F(ModifiersInsertCopiesAtPositionTest, InsertInTheMiddle)
{
    auto it = j_array.insert(j_array.begin() + 2, 3, 5);
    CHECK(j_array.size() == 7);
    CHECK(*it == j_value);
    CHECK((it - j_array.begin()) == 2);
    CHECK(j_array == json({1, 2, 5, 5, 5, 3, 4}));
}

TEST_F(ModifiersInsertCopiesAtPositionTest, InsertBeforeEnd)
{
    auto it = j_array.insert(j_array.end(), 3, 5);
    CHECK(j_array.size() == 7);
    CHECK(*it == j_value);
    CHECK((j_array.end() - it) == 3);
    CHECK(j_array == json({1, 2, 3, 4, 5, 5, 5}));
}

TEST_F(ModifiersInsertCopiesAtPositionTest, InsertNothingCount0)
{
    auto it = j_array.insert(j_array.end(), 0, 5);
    CHECK(j_array.size() == 4);
    // the returned iterator points to the first inserted element;
    // there were 4 elements, so it should point to the 5th
    CHECK(it == j_array.begin() + 4);
    CHECK(j_array == json({1, 2, 3, 4}));
}

class ModifiersInsertRangeForArrayTest : public ModifiersInsertTest {
 protected:
    json j_other_array = {"first", "second"};
};


TEST_F(ModifiersInsertRangeForArrayTest, ProperUsage)
{
    auto it = j_array.insert(j_array.end(), j_other_array.begin(), j_other_array.end());
    CHECK(j_array.size() == 6);
    CHECK(*it == *j_other_array.begin());
    CHECK((j_array.end() - it) == 2);
    CHECK(j_array == json({1, 2, 3, 4, "first", "second"}));
}

TEST_F(ModifiersInsertRangeForArrayTest, EmptyRange)
{
    auto it = j_array.insert(j_array.end(), j_other_array.begin(), j_other_array.begin());
    CHECK(j_array.size() == 4);
    CHECK(it == j_array.end());
    CHECK(j_array == json({1, 2, 3, 4}));
}

TEST_F(ModifiersInsertRangeForArrayTest, InvalidIterators)
{
    json j_other_array2 = {"first", "second"};

    CHECK_THROWS_WITH_AS(j_array.insert(j_array.end(), j_array.begin(), j_array.end()), "[json.exception.invalid_iterator.211] passed iterators may not belong to container",
                         json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(j_array.insert(j_array.end(), j_other_array.begin(), j_other_array2.end()), "[json.exception.invalid_iterator.210] iterators do not fit",
                         json::invalid_iterator&);
}

class ModifiersInsertRangeForObjectTest : public ModifiersInsertTest {
 protected:
    json j_object1 = {{"one", "eins"}, {"two", "zwei"}};
    json j_object2 = {{"eleven", "elf"}, {"seventeen", "siebzehn"}};
};


TEST_F(ModifiersInsertRangeForObjectTest, ProperUsage)
{
    j_object1.insert(j_object2.begin(), j_object2.end());
    CHECK(j_object1.size() == 4);
}

TEST_F(ModifiersInsertRangeForObjectTest, EmptyRange)
{
    j_object1.insert(j_object2.begin(), j_object2.begin());
    CHECK(j_object1.size() == 2);
}

TEST_F(ModifiersInsertRangeForObjectTest, InvalidIterators)
{
    json j_other_array2 = {"first", "second"};

    CHECK_THROWS_WITH_AS(j_array.insert(j_object2.begin(), j_object2.end()), "[json.exception.type_error.309] cannot use insert() with array", json::type_error&);
    CHECK_THROWS_WITH_AS(j_object1.insert(j_object1.begin(), j_object2.end()), "[json.exception.invalid_iterator.210] iterators do not fit", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(j_object1.insert(j_array.begin(), j_array.end()), "[json.exception.invalid_iterator.202] iterators first and last must point to objects", json::invalid_iterator&);
}

class ModifiersInsertInitializerListAtPositionTest : public ModifiersInsertTest{};


TEST_F(ModifiersInsertInitializerListAtPositionTest, InsertBeforeBegin)
{
    auto it = j_array.insert(j_array.begin(), {7, 8, 9});
    CHECK(j_array.size() == 7);
    CHECK(*it == json(7));
    CHECK(j_array.begin() == it);
    CHECK(j_array == json({7, 8, 9, 1, 2, 3, 4}));
}

TEST_F(ModifiersInsertInitializerListAtPositionTest, InsertInTheMiddle)
{
    auto it = j_array.insert(j_array.begin() + 2, {7, 8, 9});
    CHECK(j_array.size() == 7);
    CHECK(*it == json(7));
    CHECK((it - j_array.begin()) == 2);
    CHECK(j_array == json({1, 2, 7, 8, 9, 3, 4}));
}

TEST_F(ModifiersInsertInitializerListAtPositionTest, InsertBeforeEnd)
{
    auto it = j_array.insert(j_array.end(), {7, 8, 9});
    CHECK(j_array.size() == 7);
    CHECK(*it == json(7));
    CHECK((j_array.end() - it) == 3);
    CHECK(j_array == json({1, 2, 3, 4, 7, 8, 9}));
}

TEST_F(ModifiersInsertTest, InvalidIterator)
{
    // pass iterator to a different array
    json j_another_array = {1, 2};
    json j_yet_another_array = {"first", "second"};
    CHECK_THROWS_WITH_AS(j_array.insert(j_another_array.end(), 10), "[json.exception.invalid_iterator.202] iterator does not fit current value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(j_array.insert(j_another_array.end(), j_value), "[json.exception.invalid_iterator.202] iterator does not fit current value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(j_array.insert(j_another_array.end(), 10, 11), "[json.exception.invalid_iterator.202] iterator does not fit current value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(j_array.insert(j_another_array.end(), j_yet_another_array.begin(), j_yet_another_array.end()), "[json.exception.invalid_iterator.202] iterator does not fit current value", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(j_array.insert(j_another_array.end(), {1, 2, 3, 4}), "[json.exception.invalid_iterator.202] iterator does not fit current value", json::invalid_iterator&);
}

TEST_F(ModifiersInsertTest, NonArrayType)
{
    // call insert on a non-array type
    json j_nonarray = 3;
    json j_yet_another_array = {"first", "second"};
    CHECK_THROWS_WITH_AS(j_nonarray.insert(j_nonarray.end(), 10), "[json.exception.type_error.309] cannot use insert() with number", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray.insert(j_nonarray.end(), j_value), "[json.exception.type_error.309] cannot use insert() with number", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray.insert(j_nonarray.end(), 10, 11), "[json.exception.type_error.309] cannot use insert() with number", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray.insert(j_nonarray.end(), j_yet_another_array.begin(), j_yet_another_array.end()), "[json.exception.type_error.309] cannot use insert() with number", json::type_error&);
    CHECK_THROWS_WITH_AS(j_nonarray.insert(j_nonarray.end(), {1, 2, 3, 4}), "[json.exception.type_error.309] cannot use insert() with number", json::type_error&);
}




class ModifiersUpdateNonRecursiveDefaultTest : public ::testing::Test {
 protected:
    json j_object1 = {{"one", "eins"}, {"two", "zwei"}};
    json j_object2 = {{"three", "drei"}, {"two", "zwo"}};
    json j_array = {1, 2, 3, 4};
};


class ModifiersUpdateNonRecursiveDefaultConstReferenceTest : public ModifiersUpdateNonRecursiveDefaultTest{};


TEST_F(ModifiersUpdateNonRecursiveDefaultConstReferenceTest, ProperUsage)
{
    j_object1.update(j_object2);
    CHECK(j_object1 == json({{"one", "eins"}, {"two", "zwo"}, {"three", "drei"}}));

    json j_null;
    j_null.update(j_object2);
    CHECK(j_null == j_object2);
}

TEST_F(ModifiersUpdateNonRecursiveDefaultConstReferenceTest, WrongTypes)
{
    CHECK_THROWS_WITH_AS(j_array.update(j_object1), "[json.exception.type_error.312] cannot use update() with array", json::type_error&);

    CHECK_THROWS_WITH_AS(j_object1.update(j_array), "[json.exception.type_error.312] cannot use update() with array", json::type_error&);
}

class ModifiersUpdateNonRecursiveDefaultIteratorRangeTest : public ModifiersUpdateNonRecursiveDefaultTest{};


TEST_F(ModifiersUpdateNonRecursiveDefaultIteratorRangeTest, ProperUsage)
{
    j_object1.update(j_object2.begin(), j_object2.end());
    CHECK(j_object1 == json({{"one", "eins"}, {"two", "zwo"}, {"three", "drei"}}));

    json j_null;
    j_null.update(j_object2.begin(), j_object2.end());
    CHECK(j_null == j_object2);
}

TEST_F(ModifiersUpdateNonRecursiveDefaultIteratorRangeTest, EmptyRange)
{
    j_object1.update(j_object2.begin(), j_object2.begin());
    CHECK(j_object1 == json({{"one", "eins"}, {"two", "zwei"}}));
}

TEST_F(ModifiersUpdateNonRecursiveDefaultIteratorRangeTest, InvalidIterators)
{
    json j_other_array2 = {"first", "second"};

    CHECK_THROWS_WITH_AS(j_array.update(j_object2.begin(), j_object2.end()), "[json.exception.type_error.312] cannot use update() with array", json::type_error&);
    CHECK_THROWS_WITH_AS(j_object1.update(j_object1.begin(), j_object2.end()), "[json.exception.invalid_iterator.210] iterators do not fit", json::invalid_iterator&);
    CHECK_THROWS_WITH_AS(j_object1.update(j_array.begin(), j_array.end()), "[json.exception.type_error.312] cannot use update() with array", json::type_error&);
}







TEST(ModifiersUpdateRecursiveConstReferenceTest, ExtendObject)
{
    json j1 = {{"string", "s"}, {"numbers", {{"one", 1}}}};
    json j2 = {{"string", "t"}, {"numbers", {{"two", 2}}}};
    j1.update(j2, true);
    CHECK(j1 == json({{"string", "t"}, {"numbers", {{"one", 1}, {"two", 2}}}}));
}

TEST(ModifiersUpdateRecursiveConstReferenceTest, ReplaceObject)
{
    json j1 = {{"string", "s"}, {"numbers", {{"one", 1}}}};
    json j2 = {{"string", "t"}, {"numbers", 1}};
    j1.update(j2, true);
    CHECK(j1 == json({{"string", "t"}, {"numbers", 1}}));
}







TEST(ModifiersSwapJsonTest, MemberSwap)
{
    json j("hello world");
    json k(42.23);

    j.swap(k);

    CHECK(j == json(42.23));
    CHECK(k == json("hello world"));
}

TEST(ModifiersSwapJsonTest, NonmemberSwap)
{
    json j("hello world");
    json k(42.23);

    using std::swap;
    swap(j, k);

    CHECK(j == json(42.23));
    CHECK(k == json("hello world"));
}




TEST(ModifiersSwapArrayTest, ArrayType)
{
    json j = {1, 2, 3, 4};
    json::array_t a = {"foo", "bar", "baz"};

    j.swap(a);

    CHECK(j == json({"foo", "bar", "baz"}));

    j.swap(a);

    CHECK(j == json({1, 2, 3, 4}));
}

TEST(ModifiersSwapArrayTest, NonArrayType)
{
    json j = 17;
    json::array_t a = {"foo", "bar", "baz"};

    CHECK_THROWS_WITH_AS(j.swap(a), "[json.exception.type_error.310] cannot use swap(array_t&) with number", json::type_error&);
}




TEST(ModifiersSwapObjectTTest, ObjectTType)
{
    json j = {{"one", 1}, {"two", 2}};
    json::object_t o = {{"cow", "Kuh"}, {"chicken", "Huhn"}};

    j.swap(o);

    CHECK(j == json({{"cow", "Kuh"}, {"chicken", "Huhn"}}));

    j.swap(o);

    CHECK(j == json({{"one", 1}, {"two", 2}}));
}

TEST(ModifiersSwapObjectTTest, NonObjectTType)
{
    json j = 17;
    json::object_t o = {{"cow", "Kuh"}, {"chicken", "Huhn"}};

    CHECK_THROWS_WITH_AS(j.swap(o), "[json.exception.type_error.310] cannot use swap(object_t&) with number", json::type_error&);
}




TEST(ModifiersSwapStringTTest, StringTType)
{
    json j = "Hello world";
    json::string_t s = "Hallo Welt";

    j.swap(s);

    CHECK(j == json("Hallo Welt"));

    j.swap(s);

    CHECK(j == json("Hello world"));
}

TEST(ModifiersSwapStringTTest, NonStringTType)
{
    json j = 17;
    json::string_t s = "Hallo Welt";

    CHECK_THROWS_WITH_AS(j.swap(s), "[json.exception.type_error.310] cannot use swap(string_t&) with number", json::type_error&);
}




TEST(ModifiersSwapBinaryTTest, BinaryTType)
{
    json j = json::binary({1, 2, 3, 4});
    json::binary_t s = {{5, 6, 7, 8}};

    j.swap(s);

    CHECK(j == json::binary({5, 6, 7, 8}));

    j.swap(s);

    CHECK(j == json::binary({1, 2, 3, 4}));
}

TEST(ModifiersSwapBinaryTTest, BinaryTContainerTypeType)
{
    json j = json::binary({1, 2, 3, 4});
    std::vector<std::uint8_t> s = {{5, 6, 7, 8}};

    j.swap(s);

    CHECK(j == json::binary({5, 6, 7, 8}));

    j.swap(s);

    CHECK(j == json::binary({1, 2, 3, 4}));
}

TEST(ModifiersSwapBinaryTTest, NonBinaryTType)
{
    json j = 17;
    json::binary_t s1 = {{1, 2, 3, 4}};
    std::vector<std::uint8_t> s2 = {{5, 6, 7, 8}};

    CHECK_THROWS_WITH_AS(j.swap(s1), "[json.exception.type_error.310] cannot use swap(binary_t&) with number", json::type_error);
    CHECK_THROWS_WITH_AS(j.swap(s2), "[json.exception.type_error.310] cannot use swap(binary_t::container_type&) with number", json::type_error);
}

