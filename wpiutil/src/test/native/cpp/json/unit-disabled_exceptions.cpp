//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// Copyright (c) 2013-2022 Niels Lohmann <http://nlohmann.me>.
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

// disable -Wnoexcept as exceptions are switched off for this test suite

#include "unit-json.h"
using json = wpi::json;

/////////////////////////////////////////////////////////////////////
// for #2824
/////////////////////////////////////////////////////////////////////
namespace {

class sax_no_exception : public wpi::detail::json_sax_dom_parser<json>
{
  public:
    explicit sax_no_exception(json& j) : wpi::detail::json_sax_dom_parser<json>(j, false) {}

    static bool parse_error(std::size_t /*position*/, const std::string& /*last_token*/, const json::exception& ex)
    {
        error_string = new std::string(ex.what()); // NOLINT(cppcoreguidelines-owning-memory)
        return false;
    }

    static std::string* error_string;
};

std::string* sax_no_exception::error_string = nullptr;

}



TEST(TestsWithDisabledExceptionsTest, Issue2824EncodingOfJsonExceptionWhat)
{
    json j;
    sax_no_exception sax(j);

    CHECK (!json::sax_parse("xyz", &sax));
    CHECK(*sax_no_exception::error_string == "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - invalid literal; last read: 'x'");
    delete sax_no_exception::error_string; // NOLINT(cppcoreguidelines-owning-memory)
}


