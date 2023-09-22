//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// Copyright (c) 2013-2022 Niels Lohmann <http://nlohmann.me>.
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

#ifdef JSON_DIAGNOSTICS
    #undef JSON_DIAGNOSTICS
#endif

#define JSON_DIAGNOSTICS 1

#include "unit-json.h"
using wpi::json;




TEST(BetterDiagnosticsTest, EmptyJSONPointer)
{
    json j = 1;
    std::string s;
    CHECK_THROWS_WITH_AS(s = j.get<std::string>(), "[json.exception.type_error.302] type must be string, but is number", json::type_error);
}

TEST(BetterDiagnosticsTest, InvalidType)
{
    json j;
    j["a"]["b"]["c"] = 1;
    std::string s;
    CHECK_THROWS_WITH_AS(s = j["a"]["b"]["c"].get<std::string>(), "[json.exception.type_error.302] (/a/b/c) type must be string, but is number", json::type_error);
}

TEST(BetterDiagnosticsTest, MissingKey)
{
    json j;
    j["object"]["object"] = true;
    CHECK_THROWS_WITH_AS(j["object"].at("not_found"), "[json.exception.out_of_range.403] (/object) key 'not_found' not found", json::out_of_range);
}

TEST(BetterDiagnosticsTest, ArrayIndexOutOfRange)
{
    json j;
    j["array"][4] = true;
    CHECK_THROWS_WITH_AS(j["array"].at(5), "[json.exception.out_of_range.401] (/array) array index 5 is out of range", json::out_of_range);
}

TEST(BetterDiagnosticsTest, ArrayIndexAtWrongType)
{
    json j;
    j["array"][4] = true;
    CHECK_THROWS_WITH_AS(j["array"][4][5], "[json.exception.type_error.305] (/array/4) cannot use operator[] with a numeric argument with boolean", json::type_error);
}

TEST(BetterDiagnosticsTest, WrongIterator)
{
    json j;
    j["array"] = json::array();
    CHECK_THROWS_WITH_AS(j["array"].erase(j.begin()), "[json.exception.invalid_iterator.202] (/array) iterator does not fit current value", json::invalid_iterator);
}

TEST(BetterDiagnosticsTest, JSONPointerEscaping)
{
    json j;
    j["a/b"]["m~n"] = 1;
    std::string s;
    CHECK_THROWS_WITH_AS(s = j["a/b"]["m~n"].get<std::string>(), "[json.exception.type_error.302] (/a~1b/m~0n) type must be string, but is number", json::type_error);
}

TEST(BetterDiagnosticsTest, ParseError)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::parse(""), "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error);
}

TEST(BetterDiagnosticsTest, WrongTypeInUpdate)
{
    json j = {{"foo", "bar"}};
    json k = {{"bla", 1}};

    CHECK_THROWS_WITH_AS(j.update(k["bla"].begin(), k["bla"].end()), "[json.exception.type_error.312] (/bla) cannot use update() with number", json::type_error);
    CHECK_THROWS_WITH_AS(j.update(k["bla"]), "[json.exception.type_error.312] (/bla) cannot use update() with number", json::type_error);
}




TEST(RegressionTestsForExtendedDiagnosticsTest, RegressionTestForHttpsGithubComNlohmannJsonPull2562Pullrequestreview574858448)
{
    CHECK_THROWS_WITH_AS(json({"0", "0"})[1].get<int>(), "[json.exception.type_error.302] (/1) type must be number, but is string", json::type_error);
    CHECK_THROWS_WITH_AS(json({"0", "1"})[1].get<int>(), "[json.exception.type_error.302] (/1) type must be number, but is string", json::type_error);
}

TEST(RegressionTestsForExtendedDiagnosticsTest, RegressionTestForHttpsGithubComNlohmannJsonPull2562Files380a613f2b5d32425021129cd1f371ddcfd54ddfR563259793)
{
    json j;
    j["/foo"] = {1, 2, 3};
    CHECK_THROWS_WITH_AS(j.unflatten(), "[json.exception.type_error.315] (/~1foo) values in object must be primitive", json::type_error);
}

TEST(RegressionTestsForExtendedDiagnosticsTest, RegressionTestForIssue2838AssertionFailureWhenInsertingIntoArraysWithJSONDIAGNOSTICSSet)
{
    // void push_back(basic_json&& val)
    {
        json j_arr = json::array();
        j_arr.push_back(json::object());
        j_arr.push_back(json::object());
        j_arr.push_back(json::object());
        j_arr.push_back(json::object());
        json j_obj = json::object();
        j_obj["key"] = j_arr;
    }

    // void push_back(const basic_json& val)
    {
        json j_arr = json::array();
        auto object = json::object();
        j_arr.push_back(object);
        j_arr.push_back(object);
        j_arr.push_back(object);
        j_arr.push_back(object);
        json j_obj = json::object();
        j_obj["key"] = j_arr;
    }

    // reference emplace_back(Args&& ... args)
    {
        json j_arr = json::array();
        j_arr.emplace_back(json::object());
        j_arr.emplace_back(json::object());
        j_arr.emplace_back(json::object());
        j_arr.emplace_back(json::object());
        json j_obj = json::object();
        j_obj["key"] = j_arr;
    }

    // iterator insert(const_iterator pos, const basic_json& val)
    {
        json j_arr = json::array();
        j_arr.insert(j_arr.begin(), json::object());
        j_arr.insert(j_arr.begin(), json::object());
        j_arr.insert(j_arr.begin(), json::object());
        j_arr.insert(j_arr.begin(), json::object());
        json j_obj = json::object();
        j_obj["key"] = j_arr;
    }

    // iterator insert(const_iterator pos, size_type cnt, const basic_json& val)
    {
        json j_arr = json::array();
        j_arr.insert(j_arr.begin(), 2, json::object());
        json j_obj = json::object();
        j_obj["key"] = j_arr;
    }

    // iterator insert(const_iterator pos, const_iterator first, const_iterator last)
    {
        json j_arr = json::array();
        json j_objects = {json::object(), json::object()};
        j_arr.insert(j_arr.begin(), j_objects.begin(), j_objects.end());
        json j_obj = json::object();
        j_obj["key"] = j_arr;
    }
}

TEST(RegressionTestsForExtendedDiagnosticsTest, RegressionTestForIssue2962JSONDIAGNOSTICSAssertionForOrderedJson)
{
    wpi::ordered_json j;
    wpi::ordered_json j2;
    const std::string value;
    j["first"] = value;
    j["second"] = value;
    j2["something"] = j;
}

TEST(RegressionTestsForExtendedDiagnosticsTest, RegressionTestForIssue3007ParentPointersProperlySetWhenUsingUpdate)
{
    // void update(const_reference j)
    {
        json j = json::object();

        {
            json j2 = json::object();
            j2["one"] = 1;

            j.update(j2);
        }

        // Must call operator[] on const element, otherwise m_parent gets updated.
        auto const& constJ = j;
        CHECK_THROWS_WITH_AS(constJ["one"].at(0), "[json.exception.type_error.304] (/one) cannot use at() with number", json::type_error);
    }

    // void update(const_iterator first, const_iterator last)
    {
        json j = json::object();

        {
            json j2 = json::object();
            j2["one"] = 1;

            j.update(j2.begin(), j2.end());
        }

        // Must call operator[] on const element, otherwise m_parent gets updated.
        auto const& constJ = j;
        CHECK_THROWS_WITH_AS(constJ["one"].at(0), "[json.exception.type_error.304] (/one) cannot use at() with number", json::type_error);
    }

    // Code from #3007 triggering unwanted assertion without fix to update().
    {
        json root = json::array();
        json lower = json::object();

        {
            json lowest = json::object();
            lowest["one"] = 1;

            lower.update(lowest);
        }

        root.push_back(lower);
    }
}

TEST(RegressionTestsForExtendedDiagnosticsTest, RegressionTestForIssue3032YetAnotherAssertionFailureWhenInsertingIntoArraysWithJSONDIAGNOSTICSSet)
{
    // reference operator[](size_type idx)
    {
        json j_arr = json::array();
        j_arr[0] = 0;
        j_arr[1] = 1;
        j_arr[2] = 2;
        j_arr[3] = 3;
        j_arr[4] = 4;
        j_arr[5] = 5;
        j_arr[6] = 6;
        j_arr[7] = 7;
        json j_arr_copy = j_arr;
    }
}

