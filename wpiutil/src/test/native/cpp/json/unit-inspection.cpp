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

#include <fstream>
#include <sstream>







TEST(ObjectInspectionConvenienceTypeCheckerTest, Object)
{
    json j {{"foo", 1}, {"bar", false}};
    CHECK(!j.is_null());
    CHECK(!j.is_boolean());
    CHECK(!j.is_number());
    CHECK(!j.is_number_integer());
    CHECK(!j.is_number_unsigned());
    CHECK(!j.is_number_float());
    CHECK(!j.is_binary());
    CHECK(j.is_object());
    CHECK(!j.is_array());
    CHECK(!j.is_string());
    CHECK(!j.is_discarded());
    CHECK(!j.is_primitive());
    CHECK(j.is_structured());
}

TEST(ObjectInspectionConvenienceTypeCheckerTest, Array)
{
    json j {"foo", 1, 1u, 42.23, false};
    CHECK(!j.is_null());
    CHECK(!j.is_boolean());
    CHECK(!j.is_number());
    CHECK(!j.is_number_integer());
    CHECK(!j.is_number_unsigned());
    CHECK(!j.is_number_float());
    CHECK(!j.is_binary());
    CHECK(!j.is_object());
    CHECK(j.is_array());
    CHECK(!j.is_string());
    CHECK(!j.is_discarded());
    CHECK(!j.is_primitive());
    CHECK(j.is_structured());
}

TEST(ObjectInspectionConvenienceTypeCheckerTest, Null)
{
    json j(nullptr);
    CHECK(j.is_null());
    CHECK(!j.is_boolean());
    CHECK(!j.is_number());
    CHECK(!j.is_number_integer());
    CHECK(!j.is_number_unsigned());
    CHECK(!j.is_number_float());
    CHECK(!j.is_binary());
    CHECK(!j.is_object());
    CHECK(!j.is_array());
    CHECK(!j.is_string());
    CHECK(!j.is_discarded());
    CHECK(j.is_primitive());
    CHECK(!j.is_structured());
}

TEST(ObjectInspectionConvenienceTypeCheckerTest, Boolean)
{
    json j(true);
    CHECK(!j.is_null());
    CHECK(j.is_boolean());
    CHECK(!j.is_number());
    CHECK(!j.is_number_integer());
    CHECK(!j.is_number_unsigned());
    CHECK(!j.is_number_float());
    CHECK(!j.is_binary());
    CHECK(!j.is_object());
    CHECK(!j.is_array());
    CHECK(!j.is_string());
    CHECK(!j.is_discarded());
    CHECK(j.is_primitive());
    CHECK(!j.is_structured());
}

TEST(ObjectInspectionConvenienceTypeCheckerTest, String)
{
    json j("Hello world");
    CHECK(!j.is_null());
    CHECK(!j.is_boolean());
    CHECK(!j.is_number());
    CHECK(!j.is_number_integer());
    CHECK(!j.is_number_unsigned());
    CHECK(!j.is_number_float());
    CHECK(!j.is_binary());
    CHECK(!j.is_object());
    CHECK(!j.is_array());
    CHECK(j.is_string());
    CHECK(!j.is_discarded());
    CHECK(j.is_primitive());
    CHECK(!j.is_structured());
}

TEST(ObjectInspectionConvenienceTypeCheckerTest, NumberInteger)
{
    json j(42);
    CHECK(!j.is_null());
    CHECK(!j.is_boolean());
    CHECK(j.is_number());
    CHECK(j.is_number_integer());
    CHECK(!j.is_number_unsigned());
    CHECK(!j.is_number_float());
    CHECK(!j.is_binary());
    CHECK(!j.is_object());
    CHECK(!j.is_array());
    CHECK(!j.is_string());
    CHECK(!j.is_discarded());
    CHECK(j.is_primitive());
    CHECK(!j.is_structured());
}

TEST(ObjectInspectionConvenienceTypeCheckerTest, NumberUnsigned)
{
    json j(42u);
    CHECK(!j.is_null());
    CHECK(!j.is_boolean());
    CHECK(j.is_number());
    CHECK(j.is_number_integer());
    CHECK(j.is_number_unsigned());
    CHECK(!j.is_number_float());
    CHECK(!j.is_binary());
    CHECK(!j.is_object());
    CHECK(!j.is_array());
    CHECK(!j.is_string());
    CHECK(!j.is_discarded());
    CHECK(j.is_primitive());
    CHECK(!j.is_structured());
}

TEST(ObjectInspectionConvenienceTypeCheckerTest, NumberFloatingPoint)
{
    json j(42.23);
    CHECK(!j.is_null());
    CHECK(!j.is_boolean());
    CHECK(j.is_number());
    CHECK(!j.is_number_integer());
    CHECK(!j.is_number_unsigned());
    CHECK(j.is_number_float());
    CHECK(!j.is_binary());
    CHECK(!j.is_object());
    CHECK(!j.is_array());
    CHECK(!j.is_string());
    CHECK(!j.is_discarded());
    CHECK(j.is_primitive());
    CHECK(!j.is_structured());
}

TEST(ObjectInspectionConvenienceTypeCheckerTest, Binary)
{
    json j(json::value_t::binary);
    CHECK(!j.is_null());
    CHECK(!j.is_boolean());
    CHECK(!j.is_number());
    CHECK(!j.is_number_integer());
    CHECK(!j.is_number_unsigned());
    CHECK(!j.is_number_float());
    CHECK(j.is_binary());
    CHECK(!j.is_object());
    CHECK(!j.is_array());
    CHECK(!j.is_string());
    CHECK(!j.is_discarded());
    CHECK(j.is_primitive());
    CHECK(!j.is_structured());
}

TEST(ObjectInspectionConvenienceTypeCheckerTest, Discarded)
{
    json j(json::value_t::discarded);
    CHECK(!j.is_null());
    CHECK(!j.is_boolean());
    CHECK(!j.is_number());
    CHECK(!j.is_number_integer());
    CHECK(!j.is_number_unsigned());
    CHECK(!j.is_number_float());
    CHECK(!j.is_binary());
    CHECK(!j.is_object());
    CHECK(!j.is_array());
    CHECK(!j.is_string());
    CHECK(j.is_discarded());
    CHECK(!j.is_primitive());
    CHECK(!j.is_structured());
}

class ObjectInspectionSerializationTest : public ::testing::Test {
 protected:
    json j {{"object", json::object()}, {"array", {1, 2, 3, 4}}, {"number", 42}, {"boolean", false}, {"null", nullptr}, {"string", "Hello world"} };
};


TEST_F(ObjectInspectionSerializationTest, NoIndentIndent1)
{
    CHECK(j.dump() ==
          "{\"array\":[1,2,3,4],\"boolean\":false,\"null\":null,\"number\":42,\"object\":{},\"string\":\"Hello world\"}");

    CHECK(j.dump() == j.dump(-1));
}

TEST_F(ObjectInspectionSerializationTest, Indent0)
{
    CHECK(j.dump(0) ==
          "{\n\"array\": [\n1,\n2,\n3,\n4\n],\n\"boolean\": false,\n\"null\": null,\n\"number\": 42,\n\"object\": {},\n\"string\": \"Hello world\"\n}");
}

TEST_F(ObjectInspectionSerializationTest, Indent1SpaceTab)
{
    CHECK(j.dump(1, '\t') ==
          "{\n\t\"array\": [\n\t\t1,\n\t\t2,\n\t\t3,\n\t\t4\n\t],\n\t\"boolean\": false,\n\t\"null\": null,\n\t\"number\": 42,\n\t\"object\": {},\n\t\"string\": \"Hello world\"\n}");
}

TEST_F(ObjectInspectionSerializationTest, Indent4)
{
    CHECK(j.dump(4) ==
          "{\n    \"array\": [\n        1,\n        2,\n        3,\n        4\n    ],\n    \"boolean\": false,\n    \"null\": null,\n    \"number\": 42,\n    \"object\": {},\n    \"string\": \"Hello world\"\n}");
}

TEST_F(ObjectInspectionSerializationTest, IndentX)
{
    CHECK(j.dump().size() == 94);
    CHECK(j.dump(1).size() == 127);
    CHECK(j.dump(2).size() == 142);
    CHECK(j.dump(512).size() == 7792);

    // important test, because it yields a resize of the indent_string
    // inside the dump() function
    CHECK(j.dump(1024).size() == 15472);

    const auto binary = json::binary({1, 2, 3}, 128);
    CHECK(binary.dump(1024).size() == 2086);
}

TEST_F(ObjectInspectionSerializationTest, DumpAndFloatingPointNumbers)
{
    auto s = json(42.23).dump();
    CHECK(s.find("42.23") != std::string::npos);
}

TEST_F(ObjectInspectionSerializationTest, DumpAndSmallFloatingPointNumbers)
{
    auto s = json(1.23456e-78).dump();
    CHECK(s.find("1.23456e-78") != std::string::npos);
}

TEST_F(ObjectInspectionSerializationTest, DumpAndNonASCIICharacters)
{
    CHECK(json("ä").dump() == "\"ä\"");
    CHECK(json("Ö").dump() == "\"Ö\"");
    CHECK(json("❤️").dump() == "\"❤️\"");
}

TEST_F(ObjectInspectionSerializationTest, DumpWithEnsureAsciiAndNonASCIICharacters)
{
    CHECK(json("ä").dump(-1, ' ', true) == "\"\\u00e4\"");
    CHECK(json("Ö").dump(-1, ' ', true) == "\"\\u00d6\"");
    CHECK(json("❤️").dump(-1, ' ', true) == "\"\\u2764\\ufe0f\"");
}

TEST_F(ObjectInspectionSerializationTest, SerializationOfDiscardedElement)
{
    json j_discarded(json::value_t::discarded);
    CHECK(j_discarded.dump() == "<discarded>");
}

TEST_F(ObjectInspectionSerializationTest, CheckThatPrecisionIsResetAfterSerialization)
{
    // create stringstream and set precision
    std::stringstream ss;
    ss.precision(3);
    ss << 3.141592653589793 << std::fixed;
    CHECK(ss.str() == "3.14");

    // reset stringstream
    ss.str(std::string());

    // use stringstream for JSON serialization
    json j_number = 3.14159265358979;
    ss << j_number;

    // check that precision has been overridden during serialization
    CHECK(ss.str() == "3.14159265358979");

    // check that precision has been restored
    CHECK(ss.precision() == 3);
}

TEST(ObjectInspectionTest, RoundTrips)
{
    for (const auto& s :
            {"3.141592653589793", "1000000000000000010E5"
            })
    {
        json j1 = json::parse(s);
        std::string s1 = j1.dump();
        json j2 = json::parse(s1);
        std::string s2 = j2.dump();
        CHECK(s1 == s2);
    }
}




TEST(ObjectInspectionReturnTheTypeOfTheObjectExplicitTest, Null)
{
    json j = nullptr;
    CHECK(j.type() == json::value_t::null);
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectExplicitTest, Object)
{
    json j = {{"foo", "bar"}};
    CHECK(j.type() == json::value_t::object);
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectExplicitTest, Array)
{
    json j = {1, 2, 3, 4};
    CHECK(j.type() == json::value_t::array);
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectExplicitTest, Boolean)
{
    json j = true;
    CHECK(j.type() == json::value_t::boolean);
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectExplicitTest, String)
{
    json j = "Hello world";
    CHECK(j.type() == json::value_t::string);
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectExplicitTest, NumberInteger)
{
    json j = 23;
    CHECK(j.type() == json::value_t::number_integer);
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectExplicitTest, NumberUnsigned)
{
    json j = 23u;
    CHECK(j.type() == json::value_t::number_unsigned);
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectExplicitTest, NumberFloatingPoint)
{
    json j = 42.23;
    CHECK(j.type() == json::value_t::number_float);
}




TEST(ObjectInspectionReturnTheTypeOfTheObjectImplicitTest, Null)
{
    json j = nullptr;
    json::value_t t = j;
    CHECK(t == j.type());
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectImplicitTest, Object)
{
    json j = {{"foo", "bar"}};
    json::value_t t = j;
    CHECK(t == j.type());
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectImplicitTest, Array)
{
    json j = {1, 2, 3, 4};
    json::value_t t = j;
    CHECK(t == j.type());
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectImplicitTest, Boolean)
{
    json j = true;
    json::value_t t = j;
    CHECK(t == j.type());
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectImplicitTest, String)
{
    json j = "Hello world";
    json::value_t t = j;
    CHECK(t == j.type());
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectImplicitTest, NumberInteger)
{
    json j = 23;
    json::value_t t = j;
    CHECK(t == j.type());
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectImplicitTest, NumberUnsigned)
{
    json j = 23u;
    json::value_t t = j;
    CHECK(t == j.type());
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectImplicitTest, NumberFloatingPoint)
{
    json j = 42.23;
    json::value_t t = j;
    CHECK(t == j.type());
}

TEST(ObjectInspectionReturnTheTypeOfTheObjectImplicitTest, Binary)
{
    json j = json::binary({});
    json::value_t t = j;
    CHECK(t == j.type());
}

