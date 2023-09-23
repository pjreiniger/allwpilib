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
#ifdef JSON_TEST_NO_GLOBAL_UDLS
    using namespace wpi::literals; // NOLINT(google-build-using-namespace)
#endif

#include <iostream>
#include <iterator>
#include <sstream>
#include <valarray>

namespace
{
struct SaxEventLogger : public wpi::json_sax<json>
{
    bool null() override
    {
        events.emplace_back("null()");
        return true;
    }

    bool boolean(bool val) override
    {
        events.emplace_back(val ? "boolean(true)" : "boolean(false)");
        return true;
    }

    bool number_integer(json::number_integer_t val) override
    {
        events.push_back("number_integer(" + std::to_string(val) + ")");
        return true;
    }

    bool number_unsigned(json::number_unsigned_t val) override
    {
        events.push_back("number_unsigned(" + std::to_string(val) + ")");
        return true;
    }

    bool number_float(json::number_float_t /*val*/, const std::string& s) override
    {
        events.push_back("number_float(" + s + ")");
        return true;
    }

    bool string(std::string& val) override
    {
        events.push_back("string(" + val + ")");
        return true;
    }

    bool binary(json::binary_t& val) override
    {
        std::string binary_contents = "binary(";
        std::string comma_space;
        for (auto b : val)
        {
            binary_contents.append(comma_space);
            binary_contents.append(std::to_string(static_cast<int>(b)));
            comma_space = ", ";
        }
        binary_contents.append(")");
        events.push_back(binary_contents);
        return true;
    }

    bool start_object(std::size_t elements) override
    {
        if (elements == static_cast<std::size_t>(-1))
        {
            events.emplace_back("start_object()");
        }
        else
        {
            events.push_back("start_object(" + std::to_string(elements) + ")");
        }
        return true;
    }

    bool key(std::string& val) override
    {
        events.push_back("key(" + val + ")");
        return true;
    }

    bool end_object() override
    {
        events.emplace_back("end_object()");
        return true;
    }

    bool start_array(std::size_t elements) override
    {
        if (elements == static_cast<std::size_t>(-1))
        {
            events.emplace_back("start_array()");
        }
        else
        {
            events.push_back("start_array(" + std::to_string(elements) + ")");
        }
        return true;
    }

    bool end_array() override
    {
        events.emplace_back("end_array()");
        return true;
    }

    bool parse_error(std::size_t position, const std::string& /*last_token*/, const json::exception& /*ex*/) override
    {
        events.push_back("parse_error(" + std::to_string(position) + ")");
        return false;
    }

    std::vector<std::string> events {};
};

struct SaxEventLoggerExitAfterStartObject : public SaxEventLogger
{
    bool start_object(std::size_t elements) override
    {
        if (elements == static_cast<std::size_t>(-1))
        {
            events.emplace_back("start_object()");
        }
        else
        {
            events.push_back("start_object(" + std::to_string(elements) + ")");
        }
        return false;
    }
};

struct SaxEventLoggerExitAfterKey : public SaxEventLogger
{
    bool key(std::string& val) override
    {
        events.push_back("key(" + val + ")");
        return false;
    }
};

struct SaxEventLoggerExitAfterStartArray : public SaxEventLogger
{
    bool start_array(std::size_t elements) override
    {
        if (elements == static_cast<std::size_t>(-1))
        {
            events.emplace_back("start_array()");
        }
        else
        {
            events.push_back("start_array(" + std::to_string(elements) + ")");
        }
        return false;
    }
};

template <typename T>
class proxy_iterator
{
  public:
    using iterator = typename T::iterator;
    using value_type = typename std::iterator_traits<iterator>::value_type;
    using reference = typename std::iterator_traits<iterator>::reference;
    using pointer = typename std::iterator_traits<iterator>::pointer;
    using difference_type =
        typename std::iterator_traits<iterator>::difference_type;
    using iterator_category = std::input_iterator_tag;

    proxy_iterator() = default;
    explicit proxy_iterator(iterator& it) : m_it(std::addressof(it)) {}

    proxy_iterator& operator++()
    {
        ++*m_it;
        return *this;
    }

    proxy_iterator& operator--()
    {
        --*m_it;
        return *this;
    }

    bool operator==(const proxy_iterator& rhs) const
    {
        return (m_it && rhs.m_it) ? (*m_it == *rhs.m_it) : (m_it == rhs.m_it);
    }

    bool operator!=(const proxy_iterator& rhs) const
    {
        return !(*this == rhs);
    }

    reference operator*() const
    {
        return **m_it;
    }

  private:
    iterator* m_it = nullptr;
};
} // namespace







TEST(DeserializationSuccessfulDeserializationTest, Stream)
{
    std::stringstream ss1;
    std::stringstream ss2;
    std::stringstream ss3;
    ss1 << R"(["foo",1,2,3,false,{"one":1}])";
    ss2 << R"(["foo",1,2,3,false,{"one":1}])";
    ss3 << R"(["foo",1,2,3,false,{"one":1}])";
    json j = json::parse(ss1);
    CHECK(json::accept(ss2));
    CHECK(j == json({"foo", 1, 2, 3, false, {{"one", 1}}}));

    SaxEventLogger l;
    CHECK(json::sax_parse(ss3, &l));
    CHECK(l.events.size() == 11);
    CHECK(l.events == std::vector<std::string>(
    {
        "start_array()", "string(foo)", "number_unsigned(1)",
        "number_unsigned(2)", "number_unsigned(3)", "boolean(false)",
        "start_object()", "key(one)", "number_unsigned(1)",
        "end_object()", "end_array()"
    }));
}

TEST(DeserializationSuccessfulDeserializationTest, StringLiteral)
{
    const auto* s = R"(["foo",1,2,3,false,{"one":1}])";
    json j = json::parse(s);
    CHECK(json::accept(s));
    CHECK(j == json({"foo", 1, 2, 3, false, {{"one", 1}}}));

    SaxEventLogger l;
    CHECK(json::sax_parse(s, &l));
    CHECK(l.events.size() == 11);
    CHECK(l.events == std::vector<std::string>(
    {
        "start_array()", "string(foo)", "number_unsigned(1)",
        "number_unsigned(2)", "number_unsigned(3)", "boolean(false)",
        "start_object()", "key(one)", "number_unsigned(1)",
        "end_object()", "end_array()"
    }));
}

TEST(DeserializationSuccessfulDeserializationTest, StringT)
{
    json::string_t s = R"(["foo",1,2,3,false,{"one":1}])";
    json j = json::parse(s);
    CHECK(json::accept(s));
    CHECK(j == json({"foo", 1, 2, 3, false, {{"one", 1}}}));

    SaxEventLogger l;
    CHECK(json::sax_parse(s, &l));
    CHECK(l.events.size() == 11);
    CHECK(l.events == std::vector<std::string>(
    {
        "start_array()", "string(foo)", "number_unsigned(1)",
        "number_unsigned(2)", "number_unsigned(3)", "boolean(false)",
        "start_object()", "key(one)", "number_unsigned(1)",
        "end_object()", "end_array()"
    }));
}

TEST(DeserializationSuccessfulDeserializationTest, InOperator)
{
    std::stringstream ss;
    ss << R"(["foo",1,2,3,false,{"one":1}])";
    json j;
    ss >> j;
    CHECK(j == json({"foo", 1, 2, 3, false, {{"one", 1}}}));
}

TEST(DeserializationSuccessfulDeserializationTest, UserDefinedStringLiteral)
{
    CHECK("[\"foo\",1,2,3,false,{\"one\":1}]"_json == json({"foo", 1, 2, 3, false, {{"one", 1}}}));
}




TEST(DeserializationUnsuccessfulDeserializationTest, Stream)
{
    std::stringstream ss1;
    std::stringstream ss3;
    std::stringstream ss4;
    std::stringstream ss5;
    ss1 << R"(["foo",1,2,3,false,{"one":1})";
    ss3 << R"(["foo",1,2,3,false,{"one":1})";
    ss4 << R"(["foo",1,2,3,false,{"one":1})";
    ss5 << R"(["foo",1,2,3,false,{"one":1})";

    json _;
    CHECK_THROWS_WITH_AS(_ = json::parse(ss1), "[json.exception.parse_error.101] parse error at line 1, column 29: syntax error while parsing array - unexpected end of input; expected ']'", json::parse_error&);
    CHECK(!json::accept(ss3));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(ss4, nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(ss5, &l));
    CHECK(l.events.size() == 11);
    CHECK(l.events == std::vector<std::string>(
    {
        "start_array()", "string(foo)", "number_unsigned(1)",
        "number_unsigned(2)", "number_unsigned(3)", "boolean(false)",
        "start_object()", "key(one)", "number_unsigned(1)",
        "end_object()", "parse_error(29)"
    }));
}

TEST(DeserializationUnsuccessfulDeserializationTest, String)
{
    json::string_t s = R"(["foo",1,2,3,false,{"one":1})";
    json _;
    CHECK_THROWS_WITH_AS(_ = json::parse(s), "[json.exception.parse_error.101] parse error at line 1, column 29: syntax error while parsing array - unexpected end of input; expected ']'", json::parse_error&);
    CHECK(!json::accept(s));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(s, nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(s, &l));
    CHECK(l.events.size() == 11);
    CHECK(l.events == std::vector<std::string>(
    {
        "start_array()", "string(foo)", "number_unsigned(1)",
        "number_unsigned(2)", "number_unsigned(3)", "boolean(false)",
        "start_object()", "key(one)", "number_unsigned(1)",
        "end_object()", "parse_error(29)"
    }));
}

TEST(DeserializationUnsuccessfulDeserializationTest, InOperator)
{
    std::stringstream ss;
    ss << R"(["foo",1,2,3,false,{"one":1})";
    json j;
    CHECK_THROWS_WITH_AS(ss >> j, "[json.exception.parse_error.101] parse error at line 1, column 29: syntax error while parsing array - unexpected end of input; expected ']'", json::parse_error&);
}

TEST(DeserializationUnsuccessfulDeserializationTest, UserDefinedStringLiteral)
{
    CHECK_THROWS_WITH_AS("[\"foo\",1,2,3,false,{\"one\":1}"_json, "[json.exception.parse_error.101] parse error at line 1, column 29: syntax error while parsing array - unexpected end of input; expected ']'", json::parse_error&);
}

class DeserializationContiguousContainersTest : public ::testing::Test {
 protected:
    

    

    // these cases are required for 100% line coverage
};





TEST(DeserializationContiguousContainersDirectlyTest, FromStdVector)
{
    std::vector<uint8_t> v = {'t', 'r', 'u', 'e'};
    CHECK(json::parse(v) == json(true));
    CHECK(json::accept(v));

    SaxEventLogger l;
    CHECK(json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersDirectlyTest, FromStdArray)
{
    std::array<uint8_t, 5> v { {'t', 'r', 'u', 'e'} };
    CHECK(json::parse(v) == json(true));
    CHECK(json::accept(v));

    SaxEventLogger l;
    CHECK(json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersDirectlyTest, FromArray)
{
    uint8_t v[] = {'t', 'r', 'u', 'e'}; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    CHECK(json::parse(v) == json(true));
    CHECK(json::accept(v));

    SaxEventLogger l;
    CHECK(json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersDirectlyTest, FromChars)
{
    auto* v = new uint8_t[5]; // NOLINT(cppcoreguidelines-owning-memory)
    v[0] = 't';
    v[1] = 'r';
    v[2] = 'u';
    v[3] = 'e';
    v[4] = '\0';
    CHECK(json::parse(v) == json(true));
    CHECK(json::accept(v));

    SaxEventLogger l;
    CHECK(json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));

    delete[] v; // NOLINT(cppcoreguidelines-owning-memory)
}

TEST(DeserializationContiguousContainersDirectlyTest, FromStdString)
{
    std::string v = {'t', 'r', 'u', 'e'};
    CHECK(json::parse(v) == json(true));
    CHECK(json::accept(v));

    SaxEventLogger l;
    CHECK(json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersDirectlyTest, FromStdInitializerList)
{
    std::initializer_list<uint8_t> v = {'t', 'r', 'u', 'e'};
    CHECK(json::parse(v) == json(true));
    CHECK(json::accept(v));

    SaxEventLogger l;
    CHECK(json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersDirectlyTest, EmptyContainer)
{
    std::vector<uint8_t> v;
    json _;
    CHECK_THROWS_AS(_ = json::parse(v), json::parse_error&);
    CHECK(!json::accept(v));

    SaxEventLogger l;
    CHECK(!json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(1)"}));
}




TEST(DeserializationContiguousContainersViaIteratorRangeTest, FromStdVector)
{
    std::vector<uint8_t> v = {'t', 'r', 'u', 'e'};
    CHECK(json::parse(std::begin(v), std::end(v)) == json(true));
    CHECK(json::accept(std::begin(v), std::end(v)));

    SaxEventLogger l;
    CHECK(json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersViaIteratorRangeTest, FromStdArray)
{
    std::array<uint8_t, 5> v { {'t', 'r', 'u', 'e'} };
    CHECK(json::parse(std::begin(v), std::end(v)) == json(true));
    CHECK(json::accept(std::begin(v), std::end(v)));

    SaxEventLogger l;
    CHECK(json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersViaIteratorRangeTest, FromArray)
{
    uint8_t v[] = {'t', 'r', 'u', 'e'}; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    CHECK(json::parse(std::begin(v), std::end(v)) == json(true));
    CHECK(json::accept(std::begin(v), std::end(v)));

    SaxEventLogger l;
    CHECK(json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersViaIteratorRangeTest, FromStdString)
{
    std::string v = {'t', 'r', 'u', 'e'};
    CHECK(json::parse(std::begin(v), std::end(v)) == json(true));
    CHECK(json::accept(std::begin(v), std::end(v)));

    SaxEventLogger l;
    CHECK(json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersViaIteratorRangeTest, FromStdInitializerList)
{
    std::initializer_list<uint8_t> v = {'t', 'r', 'u', 'e'};
    CHECK(json::parse(std::begin(v), std::end(v)) == json(true));
    CHECK(json::accept(std::begin(v), std::end(v)));

    SaxEventLogger l;
    CHECK(json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersViaIteratorRangeTest, FromStdValarray)
{
    std::valarray<uint8_t> v = {'t', 'r', 'u', 'e'};
    CHECK(json::parse(std::begin(v), std::end(v)) == json(true));
    CHECK(json::accept(std::begin(v), std::end(v)));

    SaxEventLogger l;
    CHECK(json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}

TEST(DeserializationContiguousContainersViaIteratorRangeTest, WithEmptyRange)
{
    std::vector<uint8_t> v;
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(1)"}));
}

TEST(DeserializationContiguousContainersViaIteratorRangeTest, IteratorInputAdapterAdvancesIteratorsCorrectly)
{
    using wpi::json;
    using wpi::detail::input_format_t;
    using wpi::detail::json_sax_dom_parser;
    using proxy = proxy_iterator<std::string>;

    std::string str1 = "[1]";
    std::string str2 = "[2]";
    std::string str = str1 + str2;

    auto first = str.begin();
    auto last = str.end();
    json j;
    json_sax_dom_parser<json> sax(j, true);

    CHECK(json::sax_parse(proxy(first), proxy(last), &sax,
                          input_format_t::json, false));
    CHECK(j.dump() == str1);
    CHECK(std::string(first, last) == str2);
}

class DeserializationContiguousContainersErrorCasesTest : public DeserializationContiguousContainersTest{};


TEST_F(DeserializationContiguousContainersErrorCasesTest, Case1)
{
    std::array<std::uint8_t, 9> v = {{'\"', 'a', 'a', 'a', 'a', 'a', 'a', '\\', 'u'}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(10)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case2)
{
    std::array<std::uint8_t, 10> v = {{'\"', 'a', 'a', 'a', 'a', 'a', 'a', '\\', 'u', '1'}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(11)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case3)
{
    std::array<std::uint8_t, 17> v = {{'\"', 'a', 'a', 'a', 'a', 'a', 'a', '\\', 'u', '1', '1', '1', '1', '1', '1', '1', '1'}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(18)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case4)
{
    std::array<std::uint8_t, 17> v = {{'\"', 'a', 'a', 'a', 'a', 'a', 'a', 'u', '1', '1', '1', '1', '1', '1', '1', '1', '\\'}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(18)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case5)
{
    std::array<std::uint8_t, 3> v = {{'\"', 0x7F, 0xC1}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(3)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case6)
{
    std::array<std::uint8_t, 4> v = {{'\"', 0x7F, 0xDF, 0x7F}};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::parse(std::begin(v), std::end(v)), "[json.exception.parse_error.101] parse error at line 1, column 4: syntax error while parsing value - invalid string: ill-formed UTF-8 byte; last read: '\"\x7f\xdf\x7f'", json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(4)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case7)
{
    std::array<std::uint8_t, 4> v = {{'\"', 0x7F, 0xDF, 0xC0}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(4)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case8)
{
    std::array<std::uint8_t, 4> v = {{'\"', 0x7F, 0xE0, 0x9F}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(4)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case9)
{
    std::array<std::uint8_t, 4> v = {{'\"', 0x7F, 0xEF, 0xC0}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(4)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case10)
{
    std::array<std::uint8_t, 4> v = {{'\"', 0x7F, 0xED, 0x7F}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(4)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case11)
{
    std::array<std::uint8_t, 4> v = {{'\"', 0x7F, 0xF0, 0x8F}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(4)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case12)
{
    std::array<std::uint8_t, 4> v = {{'\"', 0x7F, 0xF0, 0xC0}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(4)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case13)
{
    std::array<std::uint8_t, 4> v = {{'\"', 0x7F, 0xF3, 0x7F}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(4)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case14)
{
    std::array<std::uint8_t, 4> v = {{'\"', 0x7F, 0xF3, 0xC0}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(4)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case15)
{
    std::array<std::uint8_t, 4> v = {{'\"', 0x7F, 0xF4, 0x7F}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"parse_error(4)"}));
}

TEST_F(DeserializationContiguousContainersErrorCasesTest, Case16)
{
    std::array<std::uint8_t, 6> v = {{'{', '\"', '\"', ':', '1', '1'}};
    json _;
    CHECK_THROWS_AS(_ = json::parse(std::begin(v), std::end(v)), json::parse_error&);
    CHECK(!json::accept(std::begin(v), std::end(v)));

    json j_error;
    CHECK_NOTHROW(j_error = json::parse(std::begin(v), std::end(v), nullptr, false));
    CHECK(j_error.is_discarded());

    SaxEventLogger l;
    CHECK(!json::sax_parse(std::begin(v), std::end(v), &l));
    CHECK(l.events.size() == 4);
    CHECK(l.events == std::vector<std::string>(
    {
        "start_object()", "key()", "number_unsigned(11)",
        "parse_error(7)"
    }));
}

class DeserializationIgnoringByteOrderMarksTest : public ::testing::Test {
 protected:
    std::string bom = "\xEF\xBB\xBF";
};


TEST_F(DeserializationIgnoringByteOrderMarksTest, BOMOnly)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::parse(bom), "[json.exception.parse_error.101] parse error at line 1, column 4: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);

    CHECK_THROWS_WITH_AS(_ = json::parse(std::istringstream(bom)), "[json.exception.parse_error.101] parse error at line 1, column 4: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);

    SaxEventLogger l;
    CHECK(!json::sax_parse(bom, &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>(
    {
        "parse_error(4)"
    }));
}

TEST_F(DeserializationIgnoringByteOrderMarksTest, BOMAndContent)
{
    CHECK(json::parse(bom + "1") == 1);
    CHECK(json::parse(std::istringstream(bom + "1")) == 1);

    SaxEventLogger l1;
    SaxEventLogger l2;
    CHECK(json::sax_parse(std::istringstream(bom + "1"), &l1));
    CHECK(json::sax_parse(bom + "1", &l2));
    CHECK(l1.events.size() == 1);
    CHECK(l1.events == std::vector<std::string>(
    {
        "number_unsigned(1)"
    }));
    CHECK(l2.events.size() == 1);
    CHECK(l2.events == std::vector<std::string>(
    {
        "number_unsigned(1)"
    }));
}

TEST_F(DeserializationIgnoringByteOrderMarksTest, 2ByteOfBOM)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::parse(bom.substr(0, 2)), "[json.exception.parse_error.101] parse error at line 1, column 3: syntax error while parsing value - invalid BOM; must be 0xEF 0xBB 0xBF if given; last read: '\xEF\xBB'", json::parse_error&);

    CHECK_THROWS_WITH_AS(_ = json::parse(std::istringstream(bom.substr(0, 2))), "[json.exception.parse_error.101] parse error at line 1, column 3: syntax error while parsing value - invalid BOM; must be 0xEF 0xBB 0xBF if given; last read: '\xEF\xBB'", json::parse_error&);

    SaxEventLogger l1;
    SaxEventLogger l2;
    CHECK(!json::sax_parse(std::istringstream(bom.substr(0, 2)), &l1));
    CHECK(!json::sax_parse(bom.substr(0, 2), &l2));
    CHECK(l1.events.size() == 1);
    CHECK(l1.events == std::vector<std::string>(
    {
        "parse_error(3)"
    }));
    CHECK(l2.events.size() == 1);
    CHECK(l2.events == std::vector<std::string>(
    {
        "parse_error(3)"
    }));
}

TEST_F(DeserializationIgnoringByteOrderMarksTest, 1ByteOfBOM)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::parse(bom.substr(0, 1)), "[json.exception.parse_error.101] parse error at line 1, column 2: syntax error while parsing value - invalid BOM; must be 0xEF 0xBB 0xBF if given; last read: '\xEF'", json::parse_error&);

    CHECK_THROWS_WITH_AS(_ = json::parse(std::istringstream(bom.substr(0, 1))), "[json.exception.parse_error.101] parse error at line 1, column 2: syntax error while parsing value - invalid BOM; must be 0xEF 0xBB 0xBF if given; last read: '\xEF'", json::parse_error&);

    SaxEventLogger l1;
    SaxEventLogger l2;
    CHECK(!json::sax_parse(std::istringstream(bom.substr(0, 1)), &l1));
    CHECK(!json::sax_parse(bom.substr(0, 1), &l2));
    CHECK(l1.events.size() == 1);
    CHECK(l1.events == std::vector<std::string>(
    {
        "parse_error(2)"
    }));
    CHECK(l2.events.size() == 1);
    CHECK(l2.events == std::vector<std::string>(
    {
        "parse_error(2)"
    }));
}

TEST_F(DeserializationIgnoringByteOrderMarksTest, Variations)
{
    // calculate variations of each byte of the BOM to make sure
    // that the BOM and only the BOM is skipped
    for (int i0 = -1; i0 < 2; ++i0)
    {
        for (int i1 = -1; i1 < 2; ++i1)
        {
            for (int i2 = -1; i2 < 2; ++i2)
            {
                // debug output for the variations
                CAPTURE(i0)
                CAPTURE(i1)
                CAPTURE(i2)

                std::string s;
                s.push_back(static_cast<char>(bom[0] + i0));
                s.push_back(static_cast<char>(bom[1] + i1));
                s.push_back(static_cast<char>(bom[2] + i2));

                if (i0 == 0 && i1 == 0 && i2 == 0)
                {
                    // without any variation, we skip the BOM
                    CHECK(json::parse(s + "null") == json());
                    CHECK(json::parse(std::istringstream(s + "null")) == json());

                    SaxEventLogger l;
                    CHECK(json::sax_parse(s + "null", &l));
                    CHECK(l.events.size() == 1);
                    CHECK(l.events == std::vector<std::string>(
                    {
                        "null()"
                    }));
                }
                else
                {
                    // any variation is an error
                    json _;
                    CHECK_THROWS_AS(_ = json::parse(s + "null"), json::parse_error&);
                    CHECK_THROWS_AS(_ = json::parse(std::istringstream(s + "null")), json::parse_error&);

                    SaxEventLogger l;
                    CHECK(!json::sax_parse(s + "null", &l));
                    CHECK(l.events.size() == 1);

                    if (i0 != 0)
                    {
                        CHECK(l.events == std::vector<std::string>(
                        {
                            "parse_error(1)"
                        }));
                    }
                    else if (i1 != 0)
                    {
                        CHECK(l.events == std::vector<std::string>(
                        {
                            "parse_error(2)"
                        }));
                    }
                    else
                    {
                        CHECK(l.events == std::vector<std::string>(
                        {
                            "parse_error(3)"
                        }));
                    }
                }
            }
        }
    }
}

TEST(DeserializationTest, SAXAndEarlyAbort)
{
    std::string s = R"([1, ["string", 43.12], null, {"key1": true, "key2": false}])";

    SaxEventLogger default_logger;
    SaxEventLoggerExitAfterStartObject exit_after_start_object;
    SaxEventLoggerExitAfterKey exit_after_key;
    SaxEventLoggerExitAfterStartArray exit_after_start_array;

    json::sax_parse(s, &default_logger);
    CHECK(default_logger.events.size() == 14);
    CHECK(default_logger.events == std::vector<std::string>(
    {
        "start_array()", "number_unsigned(1)", "start_array()",
        "string(string)", "number_float(43.12)", "end_array()", "null()",
        "start_object()", "key(key1)", "boolean(true)", "key(key2)",
        "boolean(false)", "end_object()", "end_array()"
    }));

    json::sax_parse(s, &exit_after_start_object);
    CHECK(exit_after_start_object.events.size() == 8);
    CHECK(exit_after_start_object.events == std::vector<std::string>(
    {
        "start_array()", "number_unsigned(1)", "start_array()",
        "string(string)", "number_float(43.12)", "end_array()", "null()",
        "start_object()"
    }));

    json::sax_parse(s, &exit_after_key);
    CHECK(exit_after_key.events.size() == 9);
    CHECK(exit_after_key.events == std::vector<std::string>(
    {
        "start_array()", "number_unsigned(1)", "start_array()",
        "string(string)", "number_float(43.12)", "end_array()", "null()",
        "start_object()", "key(key1)"
    }));

    json::sax_parse(s, &exit_after_start_array);
    CHECK(exit_after_start_array.events.size() == 1);
    CHECK(exit_after_start_array.events == std::vector<std::string>(
    {
        "start_array()"
    }));
}




TEST(DeserializationJSONLinesTest, ExampleFile)
{
    std::stringstream ss;
    ss << R"({"name": "Gilbert", "wins": [["straight", "7♣"], ["one pair", "10♥"]]}
            {"name": "Alexa", "wins": [["two pair", "4♠"], ["two pair", "9♠"]]}
            {"name": "May", "wins": []}
            {"name": "Deloise", "wins": [["three of a kind", "5♣"]]}
)";

    std::string line;
    int object_count = 0;
    while (std::getline(ss, line))
    {
        ++object_count;
        CHECK(json::accept(line));
    }

    CHECK(object_count == 4);
}

TEST(DeserializationJSONLinesTest, ExampleFileWithoutTrailingNewline)
{
    std::stringstream ss;
    ss << R"({"name": "Gilbert", "wins": [["straight", "7♣"], ["one pair", "10♥"]]}
            {"name": "Alexa", "wins": [["two pair", "4♠"], ["two pair", "9♠"]]}
            {"name": "May", "wins": []}
            {"name": "Deloise", "wins": [["three of a kind", "5♣"]]})";

    std::string line;
    int object_count = 0;
    while (std::getline(ss, line))
    {
        ++object_count;
        CHECK(json::accept(line));
    }

    CHECK(object_count == 4);
}


template <class T>
class DeserializationOfDifferentCharacterTypesASCII : public testing::Test {
 protected:
    using ParamType = T;
};

using DeserializationOfDifferentCharacterTypesASCIITypes = ::testing::Types <char, unsigned char, signed char, wchar_t, char16_t, char32_t, std::uint8_t, std::int8_t, std::int16_t, std::uint16_t, std::int32_t, std::uint32_t>;
TYPED_TEST_CASE(DeserializationOfDifferentCharacterTypesASCII, DeserializationOfDifferentCharacterTypesASCIITypes);

TYPED_TEST(DeserializationOfDifferentCharacterTypesASCII, Case)

{
    using T = typename TestFixture::ParamType;

    std::vector<T> v = {'t', 'r', 'u', 'e'};
    CHECK(json::parse(v) == json(true));
    CHECK(json::accept(v));

    SaxEventLogger l;
    CHECK(json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
    CHECK(l.events == std::vector<std::string>({"boolean(true)"}));
}



template <class T>
class DeserializationOfDifferentCharacterTypesUTF8 : public testing::Test {
 protected:
    using ParamType = T;
};

using DeserializationOfDifferentCharacterTypesUTF8Types = ::testing::Types <char, unsigned char, std::uint8_t>;
TYPED_TEST_CASE(DeserializationOfDifferentCharacterTypesUTF8, DeserializationOfDifferentCharacterTypesUTF8Types);

TYPED_TEST(DeserializationOfDifferentCharacterTypesUTF8, Case)

{
    using T = typename TestFixture::ParamType;

    // a star emoji
    std::vector<T> v = {'"', static_cast<T>(0xe2u), static_cast<T>(0xadu), static_cast<T>(0x90u), static_cast<T>(0xefu), static_cast<T>(0xb8u), static_cast<T>(0x8fu), '"'};
    CHECK(json::parse(v).dump(-1, ' ', true) == "\"\\u2b50\\ufe0f\"");
    CHECK(json::accept(v));

    SaxEventLogger l;
    CHECK(json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
}



template <class T>
class DeserializationOfDifferentCharacterTypesUTF16 : public testing::Test {
 protected:
    using ParamType = T;
};

using DeserializationOfDifferentCharacterTypesUTF16Types = ::testing::Types <char16_t, std::uint16_t>;
TYPED_TEST_CASE(DeserializationOfDifferentCharacterTypesUTF16, DeserializationOfDifferentCharacterTypesUTF16Types);

TYPED_TEST(DeserializationOfDifferentCharacterTypesUTF16, Case)

{
    using T = typename TestFixture::ParamType;

    // a star emoji
    std::vector<T> v = {static_cast<T>('"'), static_cast<T>(0x2b50), static_cast<T>(0xfe0f), static_cast<T>('"')};
    CHECK(json::parse(v).dump(-1, ' ', true) == "\"\\u2b50\\ufe0f\"");
    CHECK(json::accept(v));

    SaxEventLogger l;
    CHECK(json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
}



template <class T>
class DeserializationOfDifferentCharacterTypesUTF32 : public testing::Test {
 protected:
    using ParamType = T;
};

using DeserializationOfDifferentCharacterTypesUTF32Types = ::testing::Types <char32_t, std::uint32_t>;
TYPED_TEST_CASE(DeserializationOfDifferentCharacterTypesUTF32, DeserializationOfDifferentCharacterTypesUTF32Types);

TYPED_TEST(DeserializationOfDifferentCharacterTypesUTF32, Case)

{
    using T = typename TestFixture::ParamType;

    // a star emoji
    std::vector<T> v = {static_cast<T>('"'), static_cast<T>(0x2b50), static_cast<T>(0xfe0f), static_cast<T>('"')};
    CHECK(json::parse(v).dump(-1, ' ', true) == "\"\\u2b50\\ufe0f\"");
    CHECK(json::accept(v));

    SaxEventLogger l;
    CHECK(json::sax_parse(v, &l));
    CHECK(l.events.size() == 1);
}

