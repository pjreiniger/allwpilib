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

template<typename Iter>
using can_post_increment_temporary = decltype((std::declval<Iter>()++)++);

template<typename Iter>
using can_post_decrement_temporary = decltype((std::declval<Iter>()--)--);

using wpi::detail::is_detected;










TEST(IteratorClassConstructionConstructorTest, Null)
{
    json j(json::value_t::null);
    json::iterator it(&j);
}

TEST(IteratorClassConstructionConstructorTest, Object)
{
    json j(json::value_t::object);
    json::iterator it(&j);
}

TEST(IteratorClassConstructionConstructorTest, Array)
{
    json j(json::value_t::array);
    json::iterator it(&j);
}

TEST(IteratorClassConstructionTest, CopyAssignment)
{
    json j(json::value_t::null);
    json::iterator it(&j);
    json::iterator it2(&j);
    it2 = it;
}







TEST(IteratorClassInitializationSetBeginTest, Null)
{
    json j(json::value_t::null);
    json::iterator it(&j);
    it.set_begin();
    CHECK((it == j.begin()));
}

TEST(IteratorClassInitializationSetBeginTest, Object)
{
    json j(json::value_t::object);
    json::iterator it(&j);
    it.set_begin();
    CHECK((it == j.begin()));
}

TEST(IteratorClassInitializationSetBeginTest, Array)
{
    json j(json::value_t::array);
    json::iterator it(&j);
    it.set_begin();
    CHECK((it == j.begin()));
}




TEST(IteratorClassInitializationSetEndTest, Null)
{
    json j(json::value_t::null);
    json::iterator it(&j);
    it.set_end();
    CHECK((it == j.end()));
}

TEST(IteratorClassInitializationSetEndTest, Object)
{
    json j(json::value_t::object);
    json::iterator it(&j);
    it.set_end();
    CHECK((it == j.end()));
}

TEST(IteratorClassInitializationSetEndTest, Array)
{
    json j(json::value_t::array);
    json::iterator it(&j);
    it.set_end();
    CHECK((it == j.end()));
}







TEST(IteratorClassElementAccessOperatorPointerTest, Null)
{
    json j(json::value_t::null);
    json::iterator it = j.begin();
    CHECK_THROWS_WITH_AS(*it, "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

TEST(IteratorClassElementAccessOperatorPointerTest, Number)
{
    json j(17);
    json::iterator it = j.begin();
    CHECK(*it == json(17));
    it = j.end();
    CHECK_THROWS_WITH_AS(*it, "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

TEST(IteratorClassElementAccessOperatorPointerTest, Object)
{
    json j({{"foo", "bar"}});
    json::iterator it = j.begin();
    CHECK(*it == json("bar"));
}

TEST(IteratorClassElementAccessOperatorPointerTest, Array)
{
    json j({1, 2, 3, 4});
    json::iterator it = j.begin();
    CHECK(*it == json(1));
}




TEST(IteratorClassElementAccessOperatorTest, Null)
{
    json j(json::value_t::null);
    json::iterator it = j.begin();
    CHECK_THROWS_WITH_AS(std::string(it->type_name()), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

TEST(IteratorClassElementAccessOperatorTest, Number)
{
    json j(17);
    json::iterator it = j.begin();
    CHECK(std::string(it->type_name()) == "number");
    it = j.end();
    CHECK_THROWS_WITH_AS(std::string(it->type_name()), "[json.exception.invalid_iterator.214] cannot get value", json::invalid_iterator&);
}

TEST(IteratorClassElementAccessOperatorTest, Object)
{
    json j({{"foo", "bar"}});
    json::iterator it = j.begin();
    CHECK(std::string(it->type_name()) == "string");
}

TEST(IteratorClassElementAccessOperatorTest, Array)
{
    json j({1, 2, 3, 4});
    json::iterator it = j.begin();
    CHECK(std::string(it->type_name()) == "number");
}







TEST(IteratorClassIncrementDecrementPostIncrementTest, Null)
{
    json j(json::value_t::null);
    json::iterator it = j.begin();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    it++;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(IteratorClassIncrementDecrementPostIncrementTest, Number)
{
    json j(17);
    json::iterator it = j.begin();
    CHECK((it.m_it.primitive_iterator.m_it == 0));
    it++;
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    it++;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(IteratorClassIncrementDecrementPostIncrementTest, Object)
{
    json j({{"foo", "bar"}});
    json::iterator it = j.begin();
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->begin()));
    it++;
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->end()));
}

TEST(IteratorClassIncrementDecrementPostIncrementTest, Array)
{
    json j({1, 2, 3, 4});
    json::iterator it = j.begin();
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




TEST(IteratorClassIncrementDecrementPreIncrementTest, Null)
{
    json j(json::value_t::null);
    json::iterator it = j.begin();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    ++it;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(IteratorClassIncrementDecrementPreIncrementTest, Number)
{
    json j(17);
    json::iterator it = j.begin();
    CHECK((it.m_it.primitive_iterator.m_it == 0));
    ++it;
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    ++it;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(IteratorClassIncrementDecrementPreIncrementTest, Object)
{
    json j({{"foo", "bar"}});
    json::iterator it = j.begin();
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->begin()));
    ++it;
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->end()));
}

TEST(IteratorClassIncrementDecrementPreIncrementTest, Array)
{
    json j({1, 2, 3, 4});
    json::iterator it = j.begin();
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




TEST(IteratorClassIncrementDecrementPostDecrementTest, Null)
{
    json j(json::value_t::null);
    json::iterator it = j.end();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
}

TEST(IteratorClassIncrementDecrementPostDecrementTest, Number)
{
    json j(17);
    json::iterator it = j.end();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    it--;
    CHECK((it.m_it.primitive_iterator.m_it == 0));
    it--;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(IteratorClassIncrementDecrementPostDecrementTest, Object)
{
    json j({{"foo", "bar"}});
    json::iterator it = j.end();
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->end()));
    it--;
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->begin()));
}

TEST(IteratorClassIncrementDecrementPostDecrementTest, Array)
{
    json j({1, 2, 3, 4});
    json::iterator it = j.end();
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




TEST(IteratorClassIncrementDecrementPreDecrementTest, Null)
{
    json j(json::value_t::null);
    json::iterator it = j.end();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
}

TEST(IteratorClassIncrementDecrementPreDecrementTest, Number)
{
    json j(17);
    json::iterator it = j.end();
    CHECK((it.m_it.primitive_iterator.m_it == 1));
    --it;
    CHECK((it.m_it.primitive_iterator.m_it == 0));
    --it;
    CHECK((it.m_it.primitive_iterator.m_it != 0 && it.m_it.primitive_iterator.m_it != 1));
}

TEST(IteratorClassIncrementDecrementPreDecrementTest, Object)
{
    json j({{"foo", "bar"}});
    json::iterator it = j.end();
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->end()));
    --it;
    CHECK((it.m_it.object_iterator == it.m_object->m_value.object->begin()));
}

TEST(IteratorClassIncrementDecrementPreDecrementTest, Array)
{
    json j({1, 2, 3, 4});
    json::iterator it = j.end();
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







TEST(IteratorClassEqualityPreservingPostIncrementTest, PrimitiveIteratorT)
{
    using Iter = wpi::detail::primitive_iterator_t;
    CHECK(std::is_same < decltype(std::declval<Iter&>()++), Iter >::value);
}

TEST(IteratorClassEqualityPreservingPostIncrementTest, IterImpl)
{
    using Iter = wpi::detail::iter_impl<json>;
    CHECK(std::is_same < decltype(std::declval<Iter&>()++), Iter >::value);
}

TEST(IteratorClassEqualityPreservingPostIncrementTest, JsonReverseIterator)
{
    using Base = wpi::detail::iter_impl<json>;
    using Iter = wpi::detail::json_reverse_iterator<Base>;
    CHECK(std::is_same < decltype(std::declval<Iter&>()++), Iter >::value);
}




TEST(IteratorClassEqualityPreservingPostDecrementTest, PrimitiveIteratorT)
{
    using Iter = wpi::detail::primitive_iterator_t;
    CHECK(std::is_same < decltype(std::declval<Iter&>()--), Iter >::value);
}

TEST(IteratorClassEqualityPreservingPostDecrementTest, IterImpl)
{
    using Iter = wpi::detail::iter_impl<json>;
    CHECK(std::is_same < decltype(std::declval<Iter&>()--), Iter >::value );
}

TEST(IteratorClassEqualityPreservingPostDecrementTest, JsonReverseIterator)
{
    using Base = wpi::detail::iter_impl<json>;
    using Iter = wpi::detail::json_reverse_iterator<Base>;
    CHECK(std::is_same < decltype(std::declval<Iter&>()--), Iter >::value );
}







TEST(IteratorClassCertDcl21CppPostIncrementTest, PrimitiveIteratorT)
{
    using Iter = wpi::detail::primitive_iterator_t;
    CHECK_FALSE(is_detected<can_post_increment_temporary, Iter&>::value);
}

TEST(IteratorClassCertDcl21CppPostIncrementTest, IterImpl)
{
    using Iter = wpi::detail::iter_impl<json>;
    CHECK_FALSE(is_detected<can_post_increment_temporary, Iter&>::value);
}

TEST(IteratorClassCertDcl21CppPostIncrementTest, JsonReverseIterator)
{
    using Base = wpi::detail::iter_impl<json>;
    using Iter = wpi::detail::json_reverse_iterator<Base>;
    CHECK_FALSE(is_detected<can_post_increment_temporary, Iter&>::value);
}




TEST(IteratorClassCertDcl21CppPostDecrementTest, PrimitiveIteratorT)
{
    using Iter = wpi::detail::primitive_iterator_t;
    CHECK_FALSE(is_detected<can_post_decrement_temporary, Iter&>::value);
}

TEST(IteratorClassCertDcl21CppPostDecrementTest, IterImpl)
{
    using Iter = wpi::detail::iter_impl<json>;
    CHECK_FALSE(is_detected<can_post_decrement_temporary, Iter&>::value);
}

TEST(IteratorClassCertDcl21CppPostDecrementTest, JsonReverseIterator)
{
    using Base = wpi::detail::iter_impl<json>;
    using Iter = wpi::detail::json_reverse_iterator<Base>;
    CHECK_FALSE(is_detected<can_post_decrement_temporary, Iter&>::value);
}

