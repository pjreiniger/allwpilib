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







TEST(OtherConstructorsAndDestructorCopyConstructorTest, Object)
{
    json j {{"foo", 1}, {"bar", false}};
    json k(j); // NOLINT(performance-unnecessary-copy-initialization)
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyConstructorTest, Array)
{
    json j {"foo", 1, 42.23, false};
    json k(j); // NOLINT(performance-unnecessary-copy-initialization)
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyConstructorTest, Null)
{
    json j(nullptr);
    json k(j); // NOLINT(performance-unnecessary-copy-initialization)
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyConstructorTest, Boolean)
{
    json j(true);
    json k(j); // NOLINT(performance-unnecessary-copy-initialization)
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyConstructorTest, String)
{
    json j("Hello world");
    json k(j); // NOLINT(performance-unnecessary-copy-initialization)
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyConstructorTest, NumberInteger)
{
    json j(42);
    json k(j); // NOLINT(performance-unnecessary-copy-initialization)
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyConstructorTest, NumberUnsigned)
{
    json j(42u);
    json k(j); // NOLINT(performance-unnecessary-copy-initialization)
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyConstructorTest, NumberFloatingPoint)
{
    json j(42.23);
    json k(j); // NOLINT(performance-unnecessary-copy-initialization)
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyConstructorTest, Binary)
{
    json j = json::binary({1, 2, 3});
    json k(j); // NOLINT(performance-unnecessary-copy-initialization)
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorTest, MoveConstructor)
{
    json j {{"foo", "bar"}, {"baz", {1, 2, 3, 4}}, {"a", 42u}, {"b", 42.23}, {"c", nullptr}};
    CHECK(j.type() == json::value_t::object);
    json k(std::move(j));
    CHECK(k.type() == json::value_t::object);
    CHECK(j.type() == json::value_t::null); // NOLINT: access after move is OK here
}




TEST(OtherConstructorsAndDestructorCopyAssignmentTest, Object)
{
    json j {{"foo", 1}, {"bar", false}};
    json k;
    k = j;
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyAssignmentTest, Array)
{
    json j {"foo", 1, 42.23, false};
    json k;
    k = j;
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyAssignmentTest, Null)
{
    json j(nullptr);
    json k;
    k = j;
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyAssignmentTest, Boolean)
{
    json j(true);
    json k;
    k = j;
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyAssignmentTest, String)
{
    json j("Hello world");
    json k;
    k = j;
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyAssignmentTest, NumberInteger)
{
    json j(42);
    json k;
    k = j;
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyAssignmentTest, NumberUnsigned)
{
    json j(42u);
    json k;
    k = j;
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyAssignmentTest, NumberFloatingPoint)
{
    json j(42.23);
    json k;
    k = j;
    CHECK(j == k);
}

TEST(OtherConstructorsAndDestructorCopyAssignmentTest, Binary)
{
    json j = json::binary({1, 2, 3});
    json k;
    k = j;
    CHECK(j == k);
}




TEST(OtherConstructorsAndDestructorDestructorTest, Object)
{
    auto* j = new json {{"foo", 1}, {"bar", false}}; // NOLINT(cppcoreguidelines-owning-memory)
    delete j; // NOLINT(cppcoreguidelines-owning-memory)
}

TEST(OtherConstructorsAndDestructorDestructorTest, Array)
{
    auto* j = new json {"foo", 1, 1u, false, 23.42}; // NOLINT(cppcoreguidelines-owning-memory)
    delete j; // NOLINT(cppcoreguidelines-owning-memory)
}

TEST(OtherConstructorsAndDestructorDestructorTest, String)
{
    auto* j = new json("Hello world"); // NOLINT(cppcoreguidelines-owning-memory)
    delete j; // NOLINT(cppcoreguidelines-owning-memory)
}

