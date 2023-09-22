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







class CapacityEmptyBooleanTest : public ::testing::Test {
 protected:
    CapacityEmptyBooleanTest() : j(true), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacityEmptyBooleanTest, ResultOfEmpty)
{
    CHECK(j.empty() == false);
    CHECK(j_const.empty() == false);
}

TEST_F(CapacityEmptyBooleanTest, DefinitionOfEmpty)
{
    CHECK(j.empty() == (j.begin() == j.end()));
    CHECK(j_const.empty() == (j_const.begin() == j_const.end()));
}

class CapacityEmptyStringTest : public ::testing::Test {
 protected:
    CapacityEmptyStringTest() : j("hello world"), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacityEmptyStringTest, ResultOfEmpty)
{
    CHECK(j.empty() == false);
    CHECK(j_const.empty() == false);
}

TEST_F(CapacityEmptyStringTest, DefinitionOfEmpty)
{
    CHECK(j.empty() == (j.begin() == j.end()));
    CHECK(j_const.empty() == (j_const.begin() == j_const.end()));
}




class CapacityEmptyArrayEmptyArrayTest : public ::testing::Test {
 protected:
    CapacityEmptyArrayEmptyArrayTest() : j(json::array()), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacityEmptyArrayEmptyArrayTest, ResultOfEmpty)
{
    CHECK(j.empty() == true);
    CHECK(j_const.empty() == true);
}

TEST_F(CapacityEmptyArrayEmptyArrayTest, DefinitionOfEmpty)
{
    CHECK(j.empty() == (j.begin() == j.end()));
    CHECK(j_const.empty() == (j_const.begin() == j_const.end()));
}

class CapacityEmptyArrayFilledArrayTest : public ::testing::Test {
 protected:
    CapacityEmptyArrayFilledArrayTest() : j({1, 2, 3}), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacityEmptyArrayFilledArrayTest, ResultOfEmpty)
{
    CHECK(j.empty() == false);
    CHECK(j_const.empty() == false);
}

TEST_F(CapacityEmptyArrayFilledArrayTest, DefinitionOfEmpty)
{
    CHECK(j.empty() == (j.begin() == j.end()));
    CHECK(j_const.empty() == (j_const.begin() == j_const.end()));
}




class CapacityEmptyObjectEmptyObjectTest : public ::testing::Test {
 protected:
    CapacityEmptyObjectEmptyObjectTest() : j(json::object()), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacityEmptyObjectEmptyObjectTest, ResultOfEmpty)
{
    CHECK(j.empty() == true);
    CHECK(j_const.empty() == true);
}

TEST_F(CapacityEmptyObjectEmptyObjectTest, DefinitionOfEmpty)
{
    CHECK(j.empty() == (j.begin() == j.end()));
    CHECK(j_const.empty() == (j_const.begin() == j_const.end()));
}

class CapacityEmptyObjectFilledObjectTest : public ::testing::Test {
 protected:
    CapacityEmptyObjectFilledObjectTest() : j({{"one", 1}, {"two", 2}, {"three", 3}}), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacityEmptyObjectFilledObjectTest, ResultOfEmpty)
{
    CHECK(j.empty() == false);
    CHECK(j_const.empty() == false);
}

TEST_F(CapacityEmptyObjectFilledObjectTest, DefinitionOfEmpty)
{
    CHECK(j.empty() == (j.begin() == j.end()));
    CHECK(j_const.empty() == (j_const.begin() == j_const.end()));
}

class CapacityEmptyNumberIntegerTest : public ::testing::Test {
 protected:
    CapacityEmptyNumberIntegerTest() : j(-23), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacityEmptyNumberIntegerTest, ResultOfEmpty)
{
    CHECK(j.empty() == false);
    CHECK(j_const.empty() == false);
}

TEST_F(CapacityEmptyNumberIntegerTest, DefinitionOfEmpty)
{
    CHECK(j.empty() == (j.begin() == j.end()));
    CHECK(j_const.empty() == (j_const.begin() == j_const.end()));
}

class CapacityEmptyNumberUnsignedTest : public ::testing::Test {
 protected:
    CapacityEmptyNumberUnsignedTest() : j(23u), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacityEmptyNumberUnsignedTest, ResultOfEmpty)
{
    CHECK(j.empty() == false);
    CHECK(j_const.empty() == false);
}

TEST_F(CapacityEmptyNumberUnsignedTest, DefinitionOfEmpty)
{
    CHECK(j.empty() == (j.begin() == j.end()));
    CHECK(j_const.empty() == (j_const.begin() == j_const.end()));
}

class CapacityEmptyNumberFloatTest : public ::testing::Test {
 protected:
    CapacityEmptyNumberFloatTest() : j(23.42), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacityEmptyNumberFloatTest, ResultOfEmpty)
{
    CHECK(j.empty() == false);
    CHECK(j_const.empty() == false);
}

TEST_F(CapacityEmptyNumberFloatTest, DefinitionOfEmpty)
{
    CHECK(j.empty() == (j.begin() == j.end()));
    CHECK(j_const.empty() == (j_const.begin() == j_const.end()));
}

class CapacityEmptyNullTest : public ::testing::Test {
 protected:
    CapacityEmptyNullTest() : j(nullptr), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacityEmptyNullTest, ResultOfEmpty)
{
    CHECK(j.empty() == true);
    CHECK(j_const.empty() == true);
}

TEST_F(CapacityEmptyNullTest, DefinitionOfEmpty)
{
    CHECK(j.empty() == (j.begin() == j.end()));
    CHECK(j_const.empty() == (j_const.begin() == j_const.end()));
}




class CapacitySizeBooleanTest : public ::testing::Test {
 protected:
    CapacitySizeBooleanTest() : j(true), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacitySizeBooleanTest, ResultOfSize)
{
    CHECK(j.size() == 1);
    CHECK(j_const.size() == 1);
}

TEST_F(CapacitySizeBooleanTest, DefinitionOfSize)
{
    CHECK(std::distance(j.begin(), j.end()) == j.size());
    CHECK(std::distance(j_const.begin(), j_const.end()) == j_const.size());
    CHECK(std::distance(j.rbegin(), j.rend()) == j.size());
    CHECK(std::distance(j_const.crbegin(), j_const.crend()) == j_const.size());
}

class CapacitySizeStringTest : public ::testing::Test {
 protected:
    CapacitySizeStringTest() : j("hello world"), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacitySizeStringTest, ResultOfSize)
{
    CHECK(j.size() == 1);
    CHECK(j_const.size() == 1);
}

TEST_F(CapacitySizeStringTest, DefinitionOfSize)
{
    CHECK(std::distance(j.begin(), j.end()) == j.size());
    CHECK(std::distance(j_const.begin(), j_const.end()) == j_const.size());
    CHECK(std::distance(j.rbegin(), j.rend()) == j.size());
    CHECK(std::distance(j_const.crbegin(), j_const.crend()) == j_const.size());
}




class CapacitySizeArrayEmptyArrayTest : public ::testing::Test {
 protected:
    CapacitySizeArrayEmptyArrayTest() : j(json::array()), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacitySizeArrayEmptyArrayTest, ResultOfSize)
{
    CHECK(j.size() == 0);
    CHECK(j_const.size() == 0);
}

TEST_F(CapacitySizeArrayEmptyArrayTest, DefinitionOfSize)
{
    CHECK(std::distance(j.begin(), j.end()) == j.size());
    CHECK(std::distance(j_const.begin(), j_const.end()) == j_const.size());
    CHECK(std::distance(j.rbegin(), j.rend()) == j.size());
    CHECK(std::distance(j_const.crbegin(), j_const.crend()) == j_const.size());
}

class CapacitySizeArrayFilledArrayTest : public ::testing::Test {
 protected:
    CapacitySizeArrayFilledArrayTest() : j({1, 2, 3}), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacitySizeArrayFilledArrayTest, ResultOfSize)
{
    CHECK(j.size() == 3);
    CHECK(j_const.size() == 3);
}

TEST_F(CapacitySizeArrayFilledArrayTest, DefinitionOfSize)
{
    CHECK(std::distance(j.begin(), j.end()) == j.size());
    CHECK(std::distance(j_const.begin(), j_const.end()) == j_const.size());
    CHECK(std::distance(j.rbegin(), j.rend()) == j.size());
    CHECK(std::distance(j_const.crbegin(), j_const.crend()) == j_const.size());
}




class CapacitySizeObjectEmptyObjectTest : public ::testing::Test {
 protected:
    CapacitySizeObjectEmptyObjectTest() : j(json::object()), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacitySizeObjectEmptyObjectTest, ResultOfSize)
{
    CHECK(j.size() == 0);
    CHECK(j_const.size() == 0);
}

TEST_F(CapacitySizeObjectEmptyObjectTest, DefinitionOfSize)
{
    CHECK(std::distance(j.begin(), j.end()) == j.size());
    CHECK(std::distance(j_const.begin(), j_const.end()) == j_const.size());
    CHECK(std::distance(j.rbegin(), j.rend()) == j.size());
    CHECK(std::distance(j_const.crbegin(), j_const.crend()) == j_const.size());
}

class CapacitySizeObjectFilledObjectTest : public ::testing::Test {
 protected:
    CapacitySizeObjectFilledObjectTest() : j({{"one", 1}, {"two", 2}, {"three", 3}}), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacitySizeObjectFilledObjectTest, ResultOfSize)
{
    CHECK(j.size() == 3);
    CHECK(j_const.size() == 3);
}

TEST_F(CapacitySizeObjectFilledObjectTest, DefinitionOfSize)
{
    CHECK(std::distance(j.begin(), j.end()) == j.size());
    CHECK(std::distance(j_const.begin(), j_const.end()) == j_const.size());
    CHECK(std::distance(j.rbegin(), j.rend()) == j.size());
    CHECK(std::distance(j_const.crbegin(), j_const.crend()) == j_const.size());
}

class CapacitySizeNumberIntegerTest : public ::testing::Test {
 protected:
    CapacitySizeNumberIntegerTest() : j(-23), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacitySizeNumberIntegerTest, ResultOfSize)
{
    CHECK(j.size() == 1);
    CHECK(j_const.size() == 1);
}

TEST_F(CapacitySizeNumberIntegerTest, DefinitionOfSize)
{
    CHECK(std::distance(j.begin(), j.end()) == j.size());
    CHECK(std::distance(j_const.begin(), j_const.end()) == j_const.size());
    CHECK(std::distance(j.rbegin(), j.rend()) == j.size());
    CHECK(std::distance(j_const.crbegin(), j_const.crend()) == j_const.size());
}

class CapacitySizeNumberUnsignedTest : public ::testing::Test {
 protected:
    CapacitySizeNumberUnsignedTest() : j(23u), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacitySizeNumberUnsignedTest, ResultOfSize)
{
    CHECK(j.size() == 1);
    CHECK(j_const.size() == 1);
}

TEST_F(CapacitySizeNumberUnsignedTest, DefinitionOfSize)
{
    CHECK(std::distance(j.begin(), j.end()) == j.size());
    CHECK(std::distance(j_const.begin(), j_const.end()) == j_const.size());
    CHECK(std::distance(j.rbegin(), j.rend()) == j.size());
    CHECK(std::distance(j_const.crbegin(), j_const.crend()) == j_const.size());
}

class CapacitySizeNumberFloatTest : public ::testing::Test {
 protected:
    CapacitySizeNumberFloatTest() : j(23.42), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacitySizeNumberFloatTest, ResultOfSize)
{
    CHECK(j.size() == 1);
    CHECK(j_const.size() == 1);
}

TEST_F(CapacitySizeNumberFloatTest, DefinitionOfSize)
{
    CHECK(std::distance(j.begin(), j.end()) == j.size());
    CHECK(std::distance(j_const.begin(), j_const.end()) == j_const.size());
    CHECK(std::distance(j.rbegin(), j.rend()) == j.size());
    CHECK(std::distance(j_const.crbegin(), j_const.crend()) == j_const.size());
}

class CapacitySizeNullTest : public ::testing::Test {
 protected:
    CapacitySizeNullTest() : j(nullptr), j_const(j) {}

    json j;
    const json j_const;
};


TEST_F(CapacitySizeNullTest, ResultOfSize)
{
    CHECK(j.size() == 0);
    CHECK(j_const.size() == 0);
}

TEST_F(CapacitySizeNullTest, DefinitionOfSize)
{
    CHECK(std::distance(j.begin(), j.end()) == j.size());
    CHECK(std::distance(j_const.begin(), j_const.end()) == j_const.size());
    CHECK(std::distance(j.rbegin(), j.rend()) == j.size());
    CHECK(std::distance(j_const.crbegin(), j_const.crend()) == j_const.size());
}




class CapacityMaxSizeBooleanTest : public ::testing::Test {
 protected:
    json j = true;
    const json j_const = true;
};


TEST_F(CapacityMaxSizeBooleanTest, ResultOfMaxSize)
{
    CHECK(j.max_size() == 1);
    CHECK(j_const.max_size() == 1);
}

class CapacityMaxSizeStringTest : public ::testing::Test {
 protected:
    json j = "hello world";
    const json j_const = "hello world";
};


TEST_F(CapacityMaxSizeStringTest, ResultOfMaxSize)
{
    CHECK(j.max_size() == 1);
    CHECK(j_const.max_size() == 1);
}




class CapacityMaxSizeArrayEmptyArrayTest : public ::testing::Test {
 protected:
    json j = json::array();
    const json j_const = json::array();
};


TEST_F(CapacityMaxSizeArrayEmptyArrayTest, ResultOfMaxSize)
{
    CHECK(j.max_size() >= j.size());
    CHECK(j_const.max_size() >= j_const.size());
}

class CapacityMaxSizeArrayFilledArrayTest : public ::testing::Test {
 protected:
    json j = {1, 2, 3};
    const json j_const = {1, 2, 3};
};


TEST_F(CapacityMaxSizeArrayFilledArrayTest, ResultOfMaxSize)
{
    CHECK(j.max_size() >= j.size());
    CHECK(j_const.max_size() >= j_const.size());
}




class CapacityMaxSizeObjectEmptyObjectTest : public ::testing::Test {
 protected:
    json j = json::object();
    const json j_const = json::object();
};


TEST_F(CapacityMaxSizeObjectEmptyObjectTest, ResultOfMaxSize)
{
    CHECK(j.max_size() >= j.size());
    CHECK(j_const.max_size() >= j_const.size());
}

class CapacityMaxSizeObjectFilledObjectTest : public ::testing::Test {
 protected:
    json j = {{"one", 1}, {"two", 2}, {"three", 3}};
    const json j_const = {{"one", 1}, {"two", 2}, {"three", 3}};
};


TEST_F(CapacityMaxSizeObjectFilledObjectTest, ResultOfMaxSize)
{
    CHECK(j.max_size() >= j.size());
    CHECK(j_const.max_size() >= j_const.size());
}

class CapacityMaxSizeNumberIntegerTest : public ::testing::Test {
 protected:
    json j = -23;
    const json j_const = -23;
};


TEST_F(CapacityMaxSizeNumberIntegerTest, ResultOfMaxSize)
{
    CHECK(j.max_size() == 1);
    CHECK(j_const.max_size() == 1);
}

class CapacityMaxSizeNumberUnsignedTest : public ::testing::Test {
 protected:
    json j = 23u;
    const json j_const = 23u;
};


TEST_F(CapacityMaxSizeNumberUnsignedTest, ResultOfMaxSize)
{
    CHECK(j.max_size() == 1);
    CHECK(j_const.max_size() == 1);
}

class CapacityMaxSizeNumberFloatTest : public ::testing::Test {
 protected:
    json j = 23.42;
    const json j_const = 23.42;
};


TEST_F(CapacityMaxSizeNumberFloatTest, ResultOfMaxSize)
{
    CHECK(j.max_size() == 1);
    CHECK(j_const.max_size() == 1);
}

class CapacityMaxSizeNullTest : public ::testing::Test {
 protected:
    json j = nullptr;
    const json j_const = nullptr;
};


TEST_F(CapacityMaxSizeNullTest, ResultOfMaxSize)
{
    CHECK(j.max_size() == 0);
    CHECK(j_const.max_size() == 0);
}

