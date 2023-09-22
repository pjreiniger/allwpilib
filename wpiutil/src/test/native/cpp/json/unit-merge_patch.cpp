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
#ifdef JSON_TEST_NO_GLOBAL_UDLS
    using namespace wpi::literals; // NOLINT(google-build-using-namespace)
#endif







TEST(JSONMergePatchExamplesFromRFC7396Test, Section1)
{
    json document = R"({
        "a": "b",
        "c": {
            "d": "e",
            "f": "g"
        }
    })"_json;

    json patch = R"({
        "a": "z",
        "c": {
            "f": null
        }
    })"_json;

    json expected = R"({
        "a": "z",
        "c": {
            "d": "e"
        }
    })"_json;

    document.merge_patch(patch);
    CHECK(document == expected);
}

TEST(JSONMergePatchExamplesFromRFC7396Test, Section3)
{
    json document = R"({
        "title": "Goodbye!",
        "author": {
            "givenName": "John",
            "familyName": "Doe"
        },
        "tags": [
            "example",
            "sample"
        ],
        "content": "This will be unchanged"
    })"_json;

    json patch = R"({
        "title": "Hello!",
        "phoneNumber": "+01-123-456-7890",
        "author": {
            "familyName": null
        },
        "tags": [
            "example"
        ]
    })"_json;

    json expected = R"({
        "title": "Hello!",
        "author": {
            "givenName": "John"
        },
        "tags": [
            "example"
        ],
        "content": "This will be unchanged",
        "phoneNumber": "+01-123-456-7890"
    })"_json;

    document.merge_patch(patch);
    CHECK(document == expected);
}




TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example1)
{
    json original = R"({"a":"b"})"_json;
    json patch = R"({"a":"c"})"_json;
    json result = R"({"a":"c"})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example2)
{
    json original = R"({"a":"b"})"_json;
    json patch = R"({"b":"c"})"_json;
    json result = R"({"a":"b", "b":"c"})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example3)
{
    json original = R"({"a":"b"})"_json;
    json patch = R"({"a":null})"_json;
    json result = R"({})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example4)
{
    json original = R"({"a":"b","b":"c"})"_json;
    json patch = R"({"a":null})"_json;
    json result = R"({"b":"c"})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example5)
{
    json original = R"({"a":["b"]})"_json;
    json patch = R"({"a":"c"})"_json;
    json result = R"({"a":"c"})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example6)
{
    json original = R"({"a":"c"})"_json;
    json patch = R"({"a":["b"]})"_json;
    json result = R"({"a":["b"]})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example7)
{
    json original = R"({"a":{"b": "c"}})"_json;
    json patch = R"({"a":{"b":"d","c":null}})"_json;
    json result = R"({"a": {"b": "d"}})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example8)
{
    json original = R"({"a":[{"b":"c"}]})"_json;
    json patch = R"({"a":[1]})"_json;
    json result = R"({"a":[1]})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example9)
{
    json original = R"(["a","b"])"_json;
    json patch = R"(["c","d"])"_json;
    json result = R"(["c","d"])"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example10)
{
    json original = R"({"a":"b"})"_json;
    json patch = R"(["c"])"_json;
    json result = R"(["c"])"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example11)
{
    json original = R"({"a":"foo"})"_json;
    json patch = R"(null)"_json;
    json result = R"(null)"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example12)
{
    json original = R"({"a":"foo"})"_json;
    json patch = R"("bar")"_json;
    json result = R"("bar")"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example13)
{
    json original = R"({"e":null})"_json;
    json patch = R"({"a":1})"_json;
    json result = R"({"e":null,"a":1})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example14)
{
    json original = R"([1,2])"_json;
    json patch = R"({"a":"b","c":null})"_json;
    json result = R"({"a":"b"})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

TEST(JSONMergePatchExamplesFromRFC7396AppendixATest, Example15)
{
    json original = R"({})"_json;
    json patch = R"({"a":{"bb":{"ccc":null}}})"_json;
    json result = R"({"a":{"bb":{}}})"_json;

    original.merge_patch(patch);
    CHECK(original == result);
}

