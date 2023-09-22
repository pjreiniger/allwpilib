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

#include <fstream>







TEST(JSONPatchExamplesFromRFC6902Test, 4Operations)
{
    // the ordering of members in JSON objects is not significant:
    json op1 = R"({ "op": "add", "path": "/a/b/c", "value": "foo" })"_json;
    json op2 = R"({ "path": "/a/b/c", "op": "add", "value": "foo" })"_json;
    json op3 = R"({ "value": "foo", "path": "/a/b/c", "op": "add" })"_json;

    // check if the operation objects are equivalent
    CHECK(op1 == op2);
    CHECK(op1 == op3);
}

TEST(JSONPatchExamplesFromRFC6902Test, 41Add)
{
    json patch1 = R"([{ "op": "add", "path": "/a/b", "value": [ "foo", "bar" ] }])"_json;

    // However, the object itself or an array containing it does need
    // to exist, and it remains an error for that not to be the case.
    // For example, an "add" with a target location of "/a/b" starting
    // with this document
    json doc1 = R"({ "a": { "foo": 1 } })"_json;

    // is not an error, because "a" exists, and "b" will be added to
    // its value.
    CHECK_NOTHROW(doc1.patch(patch1));
    auto doc1_ans = R"(
        {
            "a": {
                "foo": 1,
                "b": [ "foo", "bar" ]
            }
        }
    )"_json;
    CHECK(doc1.patch(patch1) == doc1_ans);

    // It is an error in this document:
    json doc2 = R"({ "q": { "bar": 2 } })"_json;

    // because "a" does not exist.
    CHECK_THROWS_WITH_AS(doc2.patch(patch1), "[json.exception.out_of_range.403] key 'a' not found", json::out_of_range&);

    json doc3 = R"({ "a": {} })"_json;
    json patch2 = R"([{ "op": "add", "path": "/a/b/c", "value": 1 }])"_json;

    // should cause an error because "b" does not exist in doc3
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(doc3.patch(patch2), "[json.exception.out_of_range.403] (/a) key 'b' not found", json::out_of_range&);
#else
    CHECK_THROWS_WITH_AS(doc3.patch(patch2), "[json.exception.out_of_range.403] key 'b' not found", json::out_of_range&);
#endif
}

TEST(JSONPatchExamplesFromRFC6902Test, 42Remove)
{
    // If removing an element from an array, any elements above the
    // specified index are shifted one position to the left.
    json doc = {1, 2, 3, 4};
    json patch = {{{"op", "remove"}, {"path", "/1"}}};
    CHECK(doc.patch(patch) == json({1, 3, 4}));
}

TEST(JSONPatchExamplesFromRFC6902Test, A1AddingAnObjectMember)
{
    // An example target JSON document:
    json doc = R"(
            { "foo": "bar"}
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "add", "path": "/baz", "value": "qux" }
            ]
        )"_json;

    // The resulting JSON document:
    json expected = R"(
            {
                "baz": "qux",
                "foo": "bar"
            }
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchExamplesFromRFC6902Test, A2AddingAnArrayElement)
{
    // An example target JSON document:
    json doc = R"(
            { "foo": [ "bar", "baz" ] }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "add", "path": "/foo/1", "value": "qux" }
            ]
        )"_json;

    // The resulting JSON document:
    json expected = R"(
            { "foo": [ "bar", "qux", "baz" ] }
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchExamplesFromRFC6902Test, A3RemovingAnObjectMember)
{
    // An example target JSON document:
    json doc = R"(
            {
                "baz": "qux",
                "foo": "bar"
            }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "remove", "path": "/baz" }
            ]
        )"_json;

    // The resulting JSON document:
    json expected = R"(
            { "foo": "bar" }
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchExamplesFromRFC6902Test, A4RemovingAnArrayElement)
{
    // An example target JSON document:
    json doc = R"(
            { "foo": [ "bar", "qux", "baz" ] }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "remove", "path": "/foo/1" }
            ]
        )"_json;

    // The resulting JSON document:
    json expected = R"(
            { "foo": [ "bar", "baz" ] }
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchExamplesFromRFC6902Test, A5ReplacingAValue)
{
    // An example target JSON document:
    json doc = R"(
            {
                "baz": "qux",
                "foo": "bar"
            }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "replace", "path": "/baz", "value": "boo" }
            ]
        )"_json;

    json expected = R"(
            {
                "baz": "boo",
                "foo": "bar"
            }
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchExamplesFromRFC6902Test, A6MovingAValue)
{
    // An example target JSON document:
    json doc = R"(
            {
                "foo": {
                   "bar": "baz",
                    "waldo": "fred"
                },
                "qux": {
                    "corge": "grault"
                }
            }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "move", "from": "/foo/waldo", "path": "/qux/thud" }
            ]
        )"_json;

    // The resulting JSON document:
    json expected = R"(
            {
                "foo": {
                   "bar": "baz"
                },
                "qux": {
                    "corge": "grault",
                    "thud": "fred"
                }
            }
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchExamplesFromRFC6902Test, A7MovingAValue)
{
    // An example target JSON document:
    json doc = R"(
            { "foo": [ "all", "grass", "cows", "eat" ] }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "move", "from": "/foo/1", "path": "/foo/3" }
            ]
        )"_json;

    // The resulting JSON document:
    json expected = R"(
            { "foo": [ "all", "cows", "eat", "grass" ] }
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchExamplesFromRFC6902Test, A8TestingAValueSuccess)
{
    // An example target JSON document:
    json doc = R"(
            {
                 "baz": "qux",
                 "foo": [ "a", 2, "c" ]
            }
        )"_json;

    // A JSON Patch document that will result in successful evaluation:
    json patch = R"(
            [
                { "op": "test", "path": "/baz", "value": "qux" },
                { "op": "test", "path": "/foo/1", "value": 2 }
            ]
        )"_json;

    // check if evaluation does not throw
    CHECK_NOTHROW(doc.patch(patch));
    // check if patched document is unchanged
    CHECK(doc.patch(patch) == doc);
}

TEST(JSONPatchExamplesFromRFC6902Test, A9TestingAValueError)
{
    // An example target JSON document:
    json doc = R"(
            { "baz": "qux" }
        )"_json;

    // A JSON Patch document that will result in an error condition:
    json patch = R"(
            [
                { "op": "test", "path": "/baz", "value": "bar" }
            ]
        )"_json;

    // check that evaluation throws
    CHECK_THROWS_AS(doc.patch(patch), json::other_error&);
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_STD_STR(doc.patch(patch), "[json.exception.other_error.501] (/0) unsuccessful: " + patch[0].dump());
#else
    CHECK_THROWS_WITH_STD_STR(doc.patch(patch), "[json.exception.other_error.501] unsuccessful: " + patch[0].dump());
#endif
}

TEST(JSONPatchExamplesFromRFC6902Test, A10AddingANestedMemberObject)
{
    // An example target JSON document:
    json doc = R"(
            { "foo": "bar" }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "add", "path": "/child", "value": { "grandchild": { } } }
            ]
        )"_json;

    // The resulting JSON document:
    json expected = R"(
        {
            "foo": "bar",
            "child": {
                "grandchild": {
                }
            }
        }
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchExamplesFromRFC6902Test, A11IgnoringUnrecognizedElements)
{
    // An example target JSON document:
    json doc = R"(
            { "foo": "bar" }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "add", "path": "/baz", "value": "qux", "xyz": 123 }
            ]
        )"_json;

    json expected = R"(
            {
                "foo": "bar",
                "baz": "qux"
            } 
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchExamplesFromRFC6902Test, A12AddingToANonexistentTarget)
{
    // An example target JSON document:
    json doc = R"(
            { "foo": "bar" }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "add", "path": "/baz/bat", "value": "qux" }
            ]
        )"_json;

    // This JSON Patch document, applied to the target JSON document
    // above, would result in an error (therefore, it would not be
    // applied), because the "add" operation's target location that
    // references neither the root of the document, nor a member of
    // an existing object, nor a member of an existing array.

    CHECK_THROWS_WITH_AS(doc.patch(patch), "[json.exception.out_of_range.403] key 'baz' not found", json::out_of_range&);
}

TEST(JSONPatchExamplesFromRFC6902Test, A14EscapeOrdering)
{
    // An example target JSON document:
    json doc = R"(
            {
                "/": 9,
                "~1": 10
            }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                {"op": "test", "path": "/~01", "value": 10}
            ]
        )"_json;

    json expected = R"(
            {
                "/": 9,
                "~1": 10
            } 
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchExamplesFromRFC6902Test, A15ComparingStringsAndNumbers)
{
    // An example target JSON document:
    json doc = R"(
            {
                "/": 9,
                "~1": 10
            } 
        )"_json;

    // A JSON Patch document that will result in an error condition:
    json patch = R"(
            [
                {"op": "test", "path": "/~01", "value": "10"}
            ]
        )"_json;

    // check that evaluation throws
    CHECK_THROWS_AS(doc.patch(patch), json::other_error&);
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_STD_STR(doc.patch(patch), "[json.exception.other_error.501] (/0) unsuccessful: " + patch[0].dump());
#else
    CHECK_THROWS_WITH_STD_STR(doc.patch(patch), "[json.exception.other_error.501] unsuccessful: " + patch[0].dump());
#endif
}

TEST(JSONPatchExamplesFromRFC6902Test, A16AddingAnArrayValue)
{
    // An example target JSON document:
    json doc = R"(
            { "foo": ["bar"] }
        )"_json;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "add", "path": "/foo/-", "value": ["abc", "def"] }
            ]
        )"_json;

    // The resulting JSON document:
    json expected = R"(
            { "foo": ["bar", ["abc", "def"]] }
        )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}







TEST(JSONPatchOwnExamplesAddTest, AddToTheRootElement)
{
    // If the path is the root of the target document - the
    // specified value becomes the entire content of the target
    // document.

    // An example target JSON document:
    json doc = 17;

    // A JSON Patch document:
    json patch = R"(
            [
                { "op": "add", "path": "", "value": [1,2,3] }
            ]
        )"_json;

    // The resulting JSON document:
    json expected = {1, 2, 3};

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchOwnExamplesAddTest, AddToEndOfTheArray)
{
    // The specified index MUST NOT be greater than the number of
    // elements in the array. The example below uses and index of
    // exactly the number of elements in the array which is legal.

    // An example target JSON document:
    json doc = {0, 1, 2};

    // A JSON Patch document:
    json patch = R"(
        [
            { "op": "add", "path": "/3", "value": 3 }
        ]
    )"_json;

    // The resulting JSON document:
    json expected = {0, 1, 2, 3};

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchOwnExamplesTest, Copy)
{
    // An example target JSON document:
    json doc = R"(
        {
            "foo": {
                "bar": "baz",
                "waldo": "fred"
            },
            "qux": {
               "corge": "grault"
            }
        }
    )"_json;

    // A JSON Patch document:
    json patch = R"(
        [
            { "op": "copy", "from": "/foo/waldo", "path": "/qux/thud" }
        ]
    )"_json;

    // The resulting JSON document:
    json expected = R"(
        {
            "foo": {
                "bar": "baz",
                "waldo": "fred"
            },
            "qux": {
               "corge": "grault",
               "thud": "fred"
            }
        }
    )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == expected);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(JSONPatchOwnExamplesTest, Replace)
{
    json j = "string";
    json patch = {{{"op", "replace"}, {"path", ""}, {"value", 1}}};
    CHECK(j.patch(patch) == json(1));
}

TEST(JSONPatchOwnExamplesTest, DocumentationGIF)
{
    {
        // a JSON patch
        json p1 = R"(
             [{"op": "add", "path": "/GB", "value": "London"}]
            )"_json;

        // a JSON value
        json source = R"(
              {"D": "Berlin", "F": "Paris"}
            )"_json;

        // apply the patch
        json target = source.patch(p1);
        // target = { "D": "Berlin", "F": "Paris", "GB": "London" }
        CHECK(target == R"({ "D": "Berlin", "F": "Paris", "GB": "London" })"_json);

        // create a diff from two JSONs
        json p2 = json::diff(target, source); // NOLINT(readability-suspicious-call-argument)
        // p2 = [{"op": "delete", "path": "/GB"}]
        CHECK(p2 == R"([{"op":"remove","path":"/GB"}])"_json);
    }
    {
        // a JSON value
        json j = {"good", "bad", "ugly"};

        // a JSON pointer
        auto ptr = json::json_pointer("/2");

        // use to access elements
        j[ptr] = {{"it", "cattivo"}};
        CHECK(j == R"(["good","bad",{"it":"cattivo"}])"_json);

        // use user-defined string literal
        j["/2/en"_json_pointer] = "ugly";
        CHECK(j == R"(["good","bad",{"en":"ugly","it":"cattivo"}])"_json);

        json flat = j.flatten();
        CHECK(flat == R"({"/0":"good","/1":"bad","/2/en":"ugly","/2/it":"cattivo"})"_json);
    }
}







TEST(JSONPatchErrorsUnknownOperationTest, NotAnArray)
{
    json j;
    json patch = {{"op", "add"}, {"path", ""}, {"value", 1}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.104] parse error: JSON patch must be an array of objects", json::parse_error&);
}

TEST(JSONPatchErrorsUnknownOperationTest, NotAnArrayOfObjects)
{
    json j;
    json patch = {"op", "add", "path", "", "value", 1};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.104] parse error: (/0) JSON patch must be an array of objects", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.104] parse error: JSON patch must be an array of objects", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsUnknownOperationTest, MissingOp)
{
    json j;
    json patch = {{{"foo", "bar"}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation must have member 'op'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation must have member 'op'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsUnknownOperationTest, NonStringOp)
{
    json j;
    json patch = {{{"op", 1}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation must have string member 'op'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation must have string member 'op'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsUnknownOperationTest, InvalidOperation)
{
    json j;
    json patch = {{{"op", "foo"}, {"path", ""}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation value 'foo' is invalid", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation value 'foo' is invalid", json::parse_error&);
#endif
}




TEST(JSONPatchErrorsAddTest, MissingPath)
{
    json j;
    json patch = {{{"op", "add"}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'add' must have member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'add' must have member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsAddTest, NonStringPath)
{
    json j;
    json patch = {{{"op", "add"}, {"path", 1}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'add' must have string member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'add' must have string member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsAddTest, MissingValue)
{
    json j;
    json patch = {{{"op", "add"}, {"path", ""}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'add' must have member 'value'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'add' must have member 'value'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsAddTest, InvalidArrayIndex)
{
    json j = {1, 2};
    json patch = {{{"op", "add"}, {"path", "/4"}, {"value", 4}}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.out_of_range.401] array index 4 is out of range", json::out_of_range&);
}




TEST(JSONPatchErrorsRemoveTest, MissingPath)
{
    json j;
    json patch = {{{"op", "remove"}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'remove' must have member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'remove' must have member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsRemoveTest, NonStringPath)
{
    json j;
    json patch = {{{"op", "remove"}, {"path", 1}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'remove' must have string member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'remove' must have string member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsRemoveTest, NonexistingTargetLocationArray)
{
    json j = {1, 2, 3};
    json patch = {{{"op", "remove"}, {"path", "/17"}}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.out_of_range.401] array index 17 is out of range", json::out_of_range&);
}

TEST(JSONPatchErrorsRemoveTest, NonexistingTargetLocationObject)
{
    json j = {{"foo", 1}, {"bar", 2}};
    json patch = {{{"op", "remove"}, {"path", "/baz"}}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.out_of_range.403] key 'baz' not found", json::out_of_range&);
}

TEST(JSONPatchErrorsRemoveTest, RootElementAsTargetLocation)
{
    json j = "string";
    json patch = {{{"op", "remove"}, {"path", ""}}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.out_of_range.405] JSON pointer has no parent", json::out_of_range&);
}




TEST(JSONPatchErrorsReplaceTest, MissingPath)
{
    json j;
    json patch = {{{"op", "replace"}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'replace' must have member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'replace' must have member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsReplaceTest, NonStringPath)
{
    json j;
    json patch = {{{"op", "replace"}, {"path", 1}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'replace' must have string member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'replace' must have string member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsReplaceTest, MissingValue)
{
    json j;
    json patch = {{{"op", "replace"}, {"path", ""}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'replace' must have member 'value'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'replace' must have member 'value'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsReplaceTest, NonexistingTargetLocationArray)
{
    json j = {1, 2, 3};
    json patch = {{{"op", "replace"}, {"path", "/17"}, {"value", 19}}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.out_of_range.401] array index 17 is out of range", json::out_of_range&);
}

TEST(JSONPatchErrorsReplaceTest, NonexistingTargetLocationObject)
{
    json j = {{"foo", 1}, {"bar", 2}};
    json patch = {{{"op", "replace"}, {"path", "/baz"}, {"value", 3}}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.out_of_range.403] key 'baz' not found", json::out_of_range&);
}




TEST(JSONPatchErrorsMoveTest, MissingPath)
{
    json j;
    json patch = {{{"op", "move"}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'move' must have member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'move' must have member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsMoveTest, NonStringPath)
{
    json j;
    json patch = {{{"op", "move"}, {"path", 1}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'move' must have string member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'move' must have string member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsMoveTest, MissingFrom)
{
    json j;
    json patch = {{{"op", "move"}, {"path", ""}}};
    CHECK_THROWS_AS(j.patch(patch), json::parse_error&);
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'move' must have member 'from'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'move' must have member 'from'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsMoveTest, NonStringFrom)
{
    json j;
    json patch = {{{"op", "move"}, {"path", ""}, {"from", 1}}};
    CHECK_THROWS_AS(j.patch(patch), json::parse_error&);
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'move' must have string member 'from'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'move' must have string member 'from'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsMoveTest, NonexistingFromLocationArray)
{
    json j = {1, 2, 3};
    json patch = {{{"op", "move"}, {"path", "/0"}, {"from", "/5"}}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.out_of_range.401] array index 5 is out of range", json::out_of_range&);
}

TEST(JSONPatchErrorsMoveTest, NonexistingFromLocationObject)
{
    json j = {{"foo", 1}, {"bar", 2}};
    json patch = {{{"op", "move"}, {"path", "/baz"}, {"from", "/baz"}}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.out_of_range.403] key 'baz' not found", json::out_of_range&);
}




TEST(JSONPatchErrorsCopyTest, MissingPath)
{
    json j;
    json patch = {{{"op", "copy"}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'copy' must have member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'copy' must have member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsCopyTest, NonStringPath)
{
    json j;
    json patch = {{{"op", "copy"}, {"path", 1}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'copy' must have string member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'copy' must have string member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsCopyTest, MissingFrom)
{
    json j;
    json patch = {{{"op", "copy"}, {"path", ""}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'copy' must have member 'from'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'copy' must have member 'from'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsCopyTest, NonStringFrom)
{
    json j;
    json patch = {{{"op", "copy"}, {"path", ""}, {"from", 1}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'copy' must have string member 'from'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'copy' must have string member 'from'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsCopyTest, NonexistingFromLocationArray)
{
    json j = {1, 2, 3};
    json patch = {{{"op", "copy"}, {"path", "/0"}, {"from", "/5"}}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.out_of_range.401] array index 5 is out of range", json::out_of_range&);
}

TEST(JSONPatchErrorsCopyTest, NonexistingFromLocationObject)
{
    json j = {{"foo", 1}, {"bar", 2}};
    json patch = {{{"op", "copy"}, {"path", "/fob"}, {"from", "/baz"}}};
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.out_of_range.403] key 'baz' not found", json::out_of_range&);
}




TEST(JSONPatchErrorsTestTest, MissingPath)
{
    json j;
    json patch = {{{"op", "test"}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'test' must have member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'test' must have member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsTestTest, NonStringPath)
{
    json j;
    json patch = {{{"op", "test"}, {"path", 1}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'test' must have string member 'path'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'test' must have string member 'path'", json::parse_error&);
#endif
}

TEST(JSONPatchErrorsTestTest, MissingValue)
{
    json j;
    json patch = {{{"op", "test"}, {"path", ""}}};
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: (/0) operation 'test' must have member 'value'", json::parse_error&);
#else
    CHECK_THROWS_WITH_AS(j.patch(patch), "[json.exception.parse_error.105] parse error: operation 'test' must have member 'value'", json::parse_error&);
#endif
}




TEST(JSONPatchExamplesFromJsonpatchComTest, SimpleExample)
{
    // The original document
    json doc = R"(
        {
          "baz": "qux",
          "foo": "bar"
        }
    )"_json;

    // The patch
    json patch = R"(
        [
          { "op": "replace", "path": "/baz", "value": "boo" },
          { "op": "add", "path": "/hello", "value": ["world"] },
          { "op": "remove", "path": "/foo"}
        ]
    )"_json;

    // The result
    json result = R"(
        {
           "baz": "boo",
           "hello": ["world"]
        }
    )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == result);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, result)) == result);
}

class JSONPatchExamplesFromJsonpatchComOperationsTest : public ::testing::Test {
 protected:
    // The original document
    json doc = R"(
        {
          "biscuits": [
            {"name":"Digestive"},
            {"name": "Choco Liebniz"}
          ]
        }
    )"_json;
};


TEST_F(JSONPatchExamplesFromJsonpatchComOperationsTest, Add)
{
    // The patch
    json patch = R"(
        [
            {"op": "add", "path": "/biscuits/1", "value": {"name": "Ginger Nut"}}
        ]
    )"_json;

    // The result
    json result = R"(
        {
          "biscuits": [
            {"name": "Digestive"},
            {"name": "Ginger Nut"},
            {"name": "Choco Liebniz"}
          ]
        }
    )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == result);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, result)) == result);
}

TEST_F(JSONPatchExamplesFromJsonpatchComOperationsTest, Remove)
{
    // The patch
    json patch = R"(
        [
            {"op": "remove", "path": "/biscuits"}
        ]
    )"_json;

    // The result
    json result = R"(
        {}
    )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == result);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, result)) == result);
}

TEST_F(JSONPatchExamplesFromJsonpatchComOperationsTest, Replace)
{
    // The patch
    json patch = R"(
        [
            {"op": "replace", "path": "/biscuits/0/name", "value": "Chocolate Digestive"}
        ]
    )"_json;

    // The result
    json result = R"(
        {
          "biscuits": [
            {"name": "Chocolate Digestive"},
            {"name": "Choco Liebniz"}
          ]
        }
    )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == result);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, result)) == result);
}

TEST_F(JSONPatchExamplesFromJsonpatchComOperationsTest, Copy)
{
    // The patch
    json patch = R"(
        [
            {"op": "copy", "from": "/biscuits/0", "path": "/best_biscuit"}
        ]
    )"_json;

    // The result
    json result = R"(
        {
          "biscuits": [
            {"name": "Digestive"},
            {"name": "Choco Liebniz"}
          ],
          "best_biscuit": {
            "name": "Digestive"
          }
        }
    )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == result);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, result)) == result);
}

TEST_F(JSONPatchExamplesFromJsonpatchComOperationsTest, Move)
{
    // The patch
    json patch = R"(
        [
            {"op": "move", "from": "/biscuits", "path": "/cookies"}
        ]
    )"_json;

    // The result
    json result = R"(
        {
          "cookies": [
            {"name": "Digestive"},
            {"name": "Choco Liebniz"}
          ]
        }
    )"_json;

    // check if patched value is as expected
    CHECK(doc.patch(patch) == result);

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, result)) == result);
}

TEST_F(JSONPatchExamplesFromJsonpatchComOperationsTest, Test)
{
    // The patch
    json patch = R"(
        [
            {"op": "test", "path": "/best_biscuit/name", "value": "Choco Liebniz"}
        ]
    )"_json;

    // the test will fail
    CHECK_THROWS_AS(doc.patch(patch), json::other_error&);
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_STD_STR(doc.patch(patch), "[json.exception.other_error.501] (/0) unsuccessful: " + patch[0].dump());
#else
    CHECK_THROWS_WITH_STD_STR(doc.patch(patch), "[json.exception.other_error.501] unsuccessful: " + patch[0].dump());
#endif
}




TEST(JSONPatchExamplesFromBruthGithubIoJsonpatchJsTest, Add)
{
    CHECK(R"( {} )"_json.patch(
              R"( [{"op": "add", "path": "/foo", "value": "bar"}] )"_json
          ) == R"( {"foo": "bar"} )"_json);

    CHECK(R"( {"foo": [1, 3]} )"_json.patch(
              R"( [{"op": "add", "path": "/foo", "value": "bar"}] )"_json
          ) == R"( {"foo": "bar"} )"_json);

    CHECK(R"( {"foo": [{}]} )"_json.patch(
              R"( [{"op": "add", "path": "/foo/0/bar", "value": "baz"}] )"_json
          ) == R"( {"foo": [{"bar": "baz"}]} )"_json);
}

TEST(JSONPatchExamplesFromBruthGithubIoJsonpatchJsTest, Remove)
{
    CHECK(R"( {"foo": "bar"} )"_json.patch(
              R"( [{"op": "remove", "path": "/foo"}] )"_json
          ) == R"( {} )"_json);

    CHECK(R"( {"foo": [1, 2, 3]} )"_json.patch(
              R"( [{"op": "remove", "path": "/foo/1"}] )"_json
          ) == R"( {"foo": [1, 3]} )"_json);

    CHECK(R"( {"foo": [{"bar": "baz"}]} )"_json.patch(
              R"( [{"op": "remove", "path": "/foo/0/bar"}] )"_json
          ) == R"( {"foo": [{}]} )"_json);
}

TEST(JSONPatchExamplesFromBruthGithubIoJsonpatchJsTest, Replace)
{
    CHECK(R"( {"foo": "bar"} )"_json.patch(
              R"( [{"op": "replace", "path": "/foo", "value": 1}] )"_json
          ) == R"( {"foo": 1} )"_json);

    CHECK(R"( {"foo": [1, 2, 3]} )"_json.patch(
              R"( [{"op": "replace", "path": "/foo/1", "value": 4}] )"_json
          ) == R"( {"foo": [1, 4, 3]} )"_json);

    CHECK(R"( {"foo": [{"bar": "baz"}]} )"_json.patch(
              R"( [{"op": "replace", "path": "/foo/0/bar", "value": 1}] )"_json
          ) == R"( {"foo": [{"bar": 1}]} )"_json);
}

TEST(JSONPatchExamplesFromBruthGithubIoJsonpatchJsTest, Move)
{
    CHECK(R"( {"foo": [1, 2, 3]} )"_json.patch(
              R"( [{"op": "move", "from": "/foo", "path": "/bar"}] )"_json
          ) == R"( {"bar": [1, 2, 3]} )"_json);
}

TEST(JSONPatchExamplesFromBruthGithubIoJsonpatchJsTest, Copy)
{
    CHECK(R"( {"foo": [1, 2, 3]} )"_json.patch(
              R"( [{"op": "copy", "from": "/foo/1", "path": "/bar"}] )"_json
          ) == R"( {"foo": [1, 2, 3], "bar": 2} )"_json);
}

TEST(JSONPatchExamplesFromBruthGithubIoJsonpatchJsTest, Copy2)
{
    CHECK_NOTHROW(R"( {"foo": "bar"} )"_json.patch(
                      R"( [{"op": "test", "path": "/foo", "value": "bar"}] )"_json));
}

