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










TEST(ConstIteratorClassConstructionConstructorTest, Null)
{
    json j(json::value_t::null);
    json::const_iterator it(&j);
}

TEST(ConstIteratorClassConstructionConstructorTest, Object)
{
    json j(json::value_t::object);
    json::const_iterator it(&j);
}

TEST(ConstIteratorClassConstructionConstructorTest, Array)
{
    json j(json::value_t::array);
    json::const_iterator it(&j);
}

TEST(ConstIteratorClassConstructionTest, CopyAssignment)
{
    json j(json::value_t::null);
    json::const_iterator it(&j);
    json::const_iterator it2(&j);
    it2 = it;
}




TEST(ConstIteratorClassConstructionCopyConstructorFromNonConstIteratorTest, CreateFromUninitializedIterator)
{
    const json::iterator it {};
    json::const_iterator cit(it);
}

TEST(ConstIteratorClassConstructionCopyConstructorFromNonConstIteratorTest, CreateFromInitializedIterator)
{
    json j;
    const json::iterator it = j.begin();
    json::const_iterator cit(it);
}







TEST(ConstIteratorClassInitializationSetBeginTest, Null)
{
    json j(json::value_t::null);
    json::const_iterator it(&j);
    it.set_begin();
    CHECK((it == j.cbegin()));
}

TEST(ConstIteratorClassInitializationSetBeginTest, Object)
{
    json j(json::value_t::object);
    json::const_iterator it(&j);
    it.set_begin();
    CHECK((it == j.cbegin()));
}

TEST(ConstIteratorClassInitializationSetBeginTest, Array)
{
    json j(json::value_t::array);
    json::const_iterator it(&j);
    it.set_begin();
    CHECK((it == j.cbegin()));
}




TEST(ConstIteratorClassInitializationSetEndTest, Null)
{
    json j(json::value_t::null);
    json::const_iterator it(&j);
    it.set_end();
    CHECK((it == j.cend()));
}

TEST(ConstIteratorClassInitializationSetEndTest, Object)
{
    json j(json::value_t::object);
    json::const_iterator it(&j);
    it.set_end();
    CHECK((it == j.cend()));
}

TEST(ConstIteratorClassInitializationSetEndTest, Array)
{
    json j(json::value_t::array);
    json::const_iterator it(&j);
    it.set_end();
    CHECK((it == j.cend()));
}







TEST(ConstIteratorClassElementAccessOperatorPointerTest, Null)
{
    json j(json::value_t::null);
    json::const_iterator it = j.cbegin();
    CHECK_THROWS_WITH_AS(*it, "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

TEST(ConstIteratorClassElementAccessOperatorPointerTest, Number)
{
    json j(17);
    json::const_iterator it = j.cbegin();
    CHECK(*it == json(17));
    it = j.cend();
    CHECK_THROWS_WITH_AS(*it, "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

TEST(ConstIteratorClassElementAccessOperatorPointerTest, Object)
{
    json j({{"foo", "bar"}});
    json::const_iterator it = j.cbegin();
    CHECK(*it == json("bar"));
}

TEST(ConstIteratorClassElementAccessOperatorPointerTest, Array)
{
    json j({1, 2, 3, 4});
    json::const_iterator it = j.cbegin();
    CHECK(*it == json(1));
}




TEST(ConstIteratorClassElementAccessOperatorTest, Null)
{
    json j(json::value_t::null);
    json::const_iterator it = j.cbegin();
    CHECK_THROWS_WITH_AS(std::string(it->type_name()), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

TEST(ConstIteratorClassElementAccessOperatorTest, Number)
{
    json j(17);
    json::const_iterator it = j.cbegin();
    CHECK(std::string(it->type_name()) == "number");
    it = j.cend();
    CHECK_THROWS_WITH_AS(std::string(it->type_name()), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

TEST(ConstIteratorClassElementAccessOperatorTest, Object)
{
    json j({{"foo", "bar"}});
    json::const_iterator it = j.cbegin();
    CHECK(std::string(it->type_name()) == "string");
}

TEST(ConstIteratorClassElementAccessOperatorTest, Array)
{
    json j({1, 2, 3, 4});
    json::const_iterator it = j.cbegin();
    CHECK(std::string(it->type_name()) == "number");
}







TEST(ConstIteratorClassIncrementDecrementPostIncrementTest, Null)
{
    json j(json::value_t::null);
    json::const_iterator it = j.cbegin();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    it++;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(ConstIteratorClassIncrementDecrementPostIncrementTest, Number)
{
    json j(17);
    json::const_iterator it = j.cbegin();
    CHECK((it.m_it.primitive_iterator.m_it == 0));
    it++;
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    it++;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(ConstIteratorClassIncrementDecrementPostIncrementTest, Object)
{
    json j({{"foo", "bar"}});
    json::const_iterator it = j.cbegin();
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->begin()));
    it++;
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->end()));
}

TEST(ConstIteratorClassIncrementDecrementPostIncrementTest, Array)
{
    json j({1, 2, 3, 4});
    json::const_iterator it = j.cbegin();
    CHECK((it.m_it.array_iterator == it.m_object->m_value.array->begin()));
    it++;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    it++;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    it++;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    it++;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator == it.m_object->m_value.array->end()));
}




TEST(ConstIteratorClassIncrementDecrementPreIncrementTest, Null)
{
    json j(json::value_t::null);
    json::const_iterator it = j.cbegin();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    ++it;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(ConstIteratorClassIncrementDecrementPreIncrementTest, Number)
{
    json j(17);
    json::const_iterator it = j.cbegin();
    CHECK((it.m_it.primitive_iterator.m_it == 0));
    ++it;
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    ++it;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(ConstIteratorClassIncrementDecrementPreIncrementTest, Object)
{
    json j({{"foo", "bar"}});
    json::const_iterator it = j.cbegin();
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->begin()));
    ++it;
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->end()));
}

TEST(ConstIteratorClassIncrementDecrementPreIncrementTest, Array)
{
    json j({1, 2, 3, 4});
    json::const_iterator it = j.cbegin();
    CHECK((it.m_it.array_iterator == it.m_object->m_value.array->begin()));
    ++it;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    ++it;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    ++it;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    ++it;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator == it.m_object->m_value.array->end()));
}




TEST(ConstIteratorClassIncrementDecrementPostDecrementTest, Null)
{
    json j(json::value_t::null);
    json::const_iterator it = j.cend();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
}

TEST(ConstIteratorClassIncrementDecrementPostDecrementTest, Number)
{
    json j(17);
    json::const_iterator it = j.cend();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    it--;
    CHECK((it.m_it.primitive_iterator.m_it == 0));
    it--;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(ConstIteratorClassIncrementDecrementPostDecrementTest, Object)
{
    json j({{"foo", "bar"}});
    json::const_iterator it = j.cend();
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->end()));
    it--;
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->begin()));
}

TEST(ConstIteratorClassIncrementDecrementPostDecrementTest, Array)
{
    json j({1, 2, 3, 4});
    json::const_iterator it = j.cend();
    CHECK((it.m_it.array_iterator == it.m_object->m_value.array->end()));
    it--;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    it--;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    it--;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    it--;
    CHECK((it.m_it.array_iterator == it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
}




TEST(ConstIteratorClassIncrementDecrementPreDecrementTest, Null)
{
    json j(json::value_t::null);
    json::const_iterator it = j.cend();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
}

TEST(ConstIteratorClassIncrementDecrementPreDecrementTest, Number)
{
    json j(17);
    json::const_iterator it = j.cend();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    --it;
    CHECK((it.m_it.primitive_iterator.m_it == 0));
    --it;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(ConstIteratorClassIncrementDecrementPreDecrementTest, Object)
{
    json j({{"foo", "bar"}});
    json::const_iterator it = j.cend();
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->end()));
    --it;
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->begin()));
}

TEST(ConstIteratorClassIncrementDecrementPreDecrementTest, Array)
{
    json j({1, 2, 3, 4});
    json::const_iterator it = j.cend();
    CHECK((it.m_it.array_iterator == it.m_object->m_value.array->end()));
    --it;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    --it;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    --it;
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
    --it;
    CHECK((it.m_it.array_iterator == it.m_object->m_value.array->begin()));
    CHECK((it.m_it.array_iterator != it.m_object->m_value.array->end()));
}

