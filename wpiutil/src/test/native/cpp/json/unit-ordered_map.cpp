//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

#include "unit-json.h"
using wpi::ordered_map;








TEST(OrderedMapConstructorTest, ConstructorFromIteratorRange)
{
    std::map<std::string, std::string> m {{"eins", "one"}, {"zwei", "two"}, {"drei", "three"}};
    ordered_map<std::string, std::string> om(m.begin(), m.end());
    CHECK(om.size() == 3);
}

TEST(OrderedMapConstructorTest, CopyAssignment)
{
    std::map<std::string, std::string> m {{"eins", "one"}, {"zwei", "two"}, {"drei", "three"}};
    ordered_map<std::string, std::string> om(m.begin(), m.end());
    const auto com = om;
    om.clear(); // silence a warning by forbidding having "const auto& com = om;"
    CHECK(com.size() == 3);
}

class OrderedMapAtTest : public ::testing::Test {
 protected:
    OrderedMapAtTest() : om(m.begin(), m.end()), com(om) {}
    std::map<std::string, std::string> m {{"eins", "one"}, {"zwei", "two"}, {"drei", "three"}};
    ordered_map<std::string, std::string> om;
    const ordered_map<std::string, std::string> com;
};


TEST_F(OrderedMapAtTest, WithKey)
{
    CHECK(om.at(std::string("eins")) == std::string("one"));
    CHECK(com.at(std::string("eins")) == std::string("one"));
    CHECK_THROWS_AS(om.at(std::string("vier")), std::out_of_range);
    CHECK_THROWS_AS(com.at(std::string("vier")), std::out_of_range);
}

TEST_F(OrderedMapAtTest, WithConstKey)
{
    const std::string eins = "eins";
    const std::string vier = "vier";
    CHECK(om.at(eins) == std::string("one"));
    CHECK(com.at(eins) == std::string("one"));
    CHECK_THROWS_AS(om.at(vier), std::out_of_range);
    CHECK_THROWS_AS(com.at(vier), std::out_of_range);
}

TEST_F(OrderedMapAtTest, WithStringLiteral)
{
    CHECK(om.at("eins") == std::string("one"));
    CHECK(com.at("eins") == std::string("one"));
    CHECK_THROWS_AS(om.at("vier"), std::out_of_range);
    CHECK_THROWS_AS(com.at("vier"), std::out_of_range);
}

class OrderedMapOperatorTest : public ::testing::Test {
 protected:
    OrderedMapOperatorTest() : om(m.begin(), m.end()), com(om) {}
    std::map<std::string, std::string> m {{"eins", "one"}, {"zwei", "two"}, {"drei", "three"}};
    ordered_map<std::string, std::string> om;
    const ordered_map<std::string, std::string> com;
};


TEST_F(OrderedMapOperatorTest, WithKey)
{
    CHECK(om[std::string("eins")] == std::string("one"));
    CHECK(com[std::string("eins")] == std::string("one"));

    CHECK(om[std::string("vier")] == std::string(""));
    CHECK(om.size() == 4);
}

TEST_F(OrderedMapOperatorTest, WithConstKey)
{
    const std::string eins = "eins";
    const std::string vier = "vier";

    CHECK(om[eins] == std::string("one"));
    CHECK(com[eins] == std::string("one"));

    CHECK(om[vier] == std::string(""));
    CHECK(om.size() == 4);
}

TEST_F(OrderedMapOperatorTest, WithStringLiteral)
{
    CHECK(om["eins"] == std::string("one"));
    CHECK(com["eins"] == std::string("one"));

    CHECK(om["vier"] == std::string(""));
    CHECK(om.size() == 4);
}

class OrderedMapEraseTest : public ::testing::Test {
 protected:
    ordered_map<std::string, std::string> om;

    OrderedMapEraseTest() {
        om["eins"] = "one";
        om["zwei"] = "two";
        om["drei"] = "three";
    }
};


TEST_F(OrderedMapEraseTest, BasicTest)
{
    auto it = om.begin();
    CHECK(it->first == "eins");
    ++it;
    CHECK(it->first == "zwei");
    ++it;
    CHECK(it->first == "drei");
    ++it;
    CHECK(it == om.end());
}

TEST_F(OrderedMapEraseTest, WithKey)
{
    CHECK(om.size() == 3);
    CHECK(om.erase(std::string("eins")) == 1);
    CHECK(om.size() == 2);
    CHECK(om.erase(std::string("vier")) == 0);
    CHECK(om.size() == 2);

    auto it = om.begin();
    CHECK(it->first == "zwei");
    ++it;
    CHECK(it->first == "drei");
    ++it;
    CHECK(it == om.end());
}

TEST_F(OrderedMapEraseTest, WithConstKey)
{
    const std::string eins = "eins";
    const std::string vier = "vier";
    CHECK(om.size() == 3);
    CHECK(om.erase(eins) == 1);
    CHECK(om.size() == 2);
    CHECK(om.erase(vier) == 0);
    CHECK(om.size() == 2);

    auto it = om.begin();
    CHECK(it->first == "zwei");
    ++it;
    CHECK(it->first == "drei");
    ++it;
    CHECK(it == om.end());
}

TEST_F(OrderedMapEraseTest, WithStringLiteral)
{
    CHECK(om.size() == 3);
    CHECK(om.erase("eins") == 1);
    CHECK(om.size() == 2);
    CHECK(om.erase("vier") == 0);
    CHECK(om.size() == 2);

    auto it = om.begin();
    CHECK(it->first == "zwei");
    ++it;
    CHECK(it->first == "drei");
    ++it;
    CHECK(it == om.end());
}

TEST_F(OrderedMapEraseTest, WithIterator)
{
    CHECK(om.size() == 3);
    CHECK(om.begin()->first == "eins");
    CHECK(std::next(om.begin(), 1)->first == "zwei");
    CHECK(std::next(om.begin(), 2)->first == "drei");

    auto it = om.erase(om.begin());
    CHECK(it->first == "zwei");
    CHECK(om.size() == 2);

    auto it2 = om.begin();
    CHECK(it2->first == "zwei");
    ++it2;
    CHECK(it2->first == "drei");
    ++it2;
    CHECK(it2 == om.end());
}

class OrderedMapEraseWithIteratorPairTest : public OrderedMapEraseTest{};


TEST_F(OrderedMapEraseWithIteratorPairTest, RangeInTheMiddle)
{
    // need more elements
    om["vier"] = "four";
    om["fünf"] = "five";

    // delete "zwei" and "drei"
    auto it = om.erase(om.begin() + 1, om.begin() + 3);
    CHECK(it->first == "vier");
    CHECK(om.size() == 3);
}

TEST_F(OrderedMapEraseWithIteratorPairTest, RangeAtTheBeginning)
{
    // need more elements
    om["vier"] = "four";
    om["fünf"] = "five";

    // delete "eins" and "zwei"
    auto it = om.erase(om.begin(), om.begin() + 2);
    CHECK(it->first == "drei");
    CHECK(om.size() == 3);
}

TEST_F(OrderedMapEraseWithIteratorPairTest, RangeAtTheEnd)
{
    // need more elements
    om["vier"] = "four";
    om["fünf"] = "five";

    // delete "vier" and "fünf"
    auto it = om.erase(om.begin() + 3, om.end());
    CHECK(it == om.end());
    CHECK(om.size() == 3);
}

TEST(OrderedMapTest, Count)
{
    ordered_map<std::string, std::string> om;
    om["eins"] = "one";
    om["zwei"] = "two";
    om["drei"] = "three";

    const std::string eins("eins");
    const std::string vier("vier");
    CHECK(om.count("eins") == 1);
    CHECK(om.count(std::string("eins")) == 1);
    CHECK(om.count(eins) == 1);
    CHECK(om.count("vier") == 0);
    CHECK(om.count(std::string("vier")) == 0);
    CHECK(om.count(vier) == 0);
}

TEST(OrderedMapTest, Find)
{
    ordered_map<std::string, std::string> om;
    om["eins"] = "one";
    om["zwei"] = "two";
    om["drei"] = "three";
    const auto com = om;

    const std::string eins("eins");
    const std::string vier("vier");
    CHECK(om.find("eins") == om.begin());
    CHECK(om.find(std::string("eins")) == om.begin());
    CHECK(om.find(eins) == om.begin());
    CHECK(om.find("vier") == om.end());
    CHECK(om.find(std::string("vier")) == om.end());
    CHECK(om.find(vier) == om.end());

    CHECK(com.find("eins") == com.begin());
    CHECK(com.find(std::string("eins")) == com.begin());
    CHECK(com.find(eins) == com.begin());
    CHECK(com.find("vier") == com.end());
    CHECK(com.find(std::string("vier")) == com.end());
    CHECK(com.find(vier) == com.end());
}

class OrderedMapInsertTest : public ::testing::Test {
 protected:
    OrderedMapInsertTest() {
        om["eins"] = "one";
        om["zwei"] = "two";
        om["drei"] = "three";
    }

    ordered_map<std::string, std::string> om;
};


TEST_F(OrderedMapInsertTest, ConstValueType)
{
    ordered_map<std::string, std::string>::value_type vt1 {"eins", "1"};
    ordered_map<std::string, std::string>::value_type vt4 {"vier", "four"};

    auto res1 = om.insert(vt1);
    CHECK(res1.first == om.begin());
    CHECK(res1.second == false);
    CHECK(om.size() == 3);

    auto res4 = om.insert(vt4);
    CHECK(res4.first == om.begin() + 3);
    CHECK(res4.second == true);
    CHECK(om.size() == 4);
}

TEST_F(OrderedMapInsertTest, ValueType)
{
    auto res1 = om.insert({"eins", "1"});
    CHECK(res1.first == om.begin());
    CHECK(res1.second == false);
    CHECK(om.size() == 3);

    auto res4 = om.insert({"vier", "four"});
    CHECK(res4.first == om.begin() + 3);
    CHECK(res4.second == true);
    CHECK(om.size() == 4);
}

