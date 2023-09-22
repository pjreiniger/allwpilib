//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

// for some reason including this after the json header leads to linker errors with VS 2017...
#include <locale>

#include "unit-json.h"
using wpi::json;

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

// this test suite uses static variables with non-trivial destructors

namespace
{
extern size_t calls;
size_t calls = 0;

void check_utf8dump(bool success_expected, int byte1, int byte2, int byte3, int byte4);

void check_utf8dump(bool success_expected, int byte1, int byte2 = -1, int byte3 = -1, int byte4 = -1)
{
    static std::string json_string;
    json_string.clear();

    CAPTURE(byte1)
    CAPTURE(byte2)
    CAPTURE(byte3)
    CAPTURE(byte4)

    json_string += std::string(1, static_cast<char>(byte1));

    if (byte2 != -1)
    {
        json_string += std::string(1, static_cast<char>(byte2));
    }

    if (byte3 != -1)
    {
        json_string += std::string(1, static_cast<char>(byte3));
    }

    if (byte4 != -1)
    {
        json_string += std::string(1, static_cast<char>(byte4));
    }

    CAPTURE(json_string)

    // store the string in a JSON value
    static json j;
    static json j2;
    j = json_string;
    j2 = "abc" + json_string + "xyz";

    static std::string s_ignored;
    static std::string s_ignored2;
    static std::string s_ignored_ascii;
    static std::string s_ignored2_ascii;
    static std::string s_replaced;
    static std::string s_replaced2;
    static std::string s_replaced_ascii;
    static std::string s_replaced2_ascii;

    // dumping with ignore/replace must not throw in any case
    s_ignored = j.dump(-1, ' ', false, json::error_handler_t::ignore);
    s_ignored2 = j2.dump(-1, ' ', false, json::error_handler_t::ignore);
    s_ignored_ascii = j.dump(-1, ' ', true, json::error_handler_t::ignore);
    s_ignored2_ascii = j2.dump(-1, ' ', true, json::error_handler_t::ignore);
    s_replaced = j.dump(-1, ' ', false, json::error_handler_t::replace);
    s_replaced2 = j2.dump(-1, ' ', false, json::error_handler_t::replace);
    s_replaced_ascii = j.dump(-1, ' ', true, json::error_handler_t::replace);
    s_replaced2_ascii = j2.dump(-1, ' ', true, json::error_handler_t::replace);

    if (success_expected)
    {
        static std::string s_strict;
        // strict mode must not throw if success is expected
        s_strict = j.dump();
        // all dumps should agree on the string
        CHECK(s_strict == s_ignored);
        CHECK(s_strict == s_replaced);
    }
    else
    {
        // strict mode must throw if success is not expected
        CHECK_THROWS_AS(j.dump(), json::type_error&);
        // ignore and replace must create different dumps
        CHECK(s_ignored != s_replaced);

        // check that replace string contains a replacement character
        CHECK(s_replaced.find("\xEF\xBF\xBD") != std::string::npos);
    }

    // check that prefix and suffix are preserved
    CHECK(s_ignored2.substr(1, 3) == "abc");
    CHECK(s_ignored2.substr(s_ignored2.size() - 4, 3) == "xyz");
    CHECK(s_ignored2_ascii.substr(1, 3) == "abc");
    CHECK(s_ignored2_ascii.substr(s_ignored2_ascii.size() - 4, 3) == "xyz");
    CHECK(s_replaced2.substr(1, 3) == "abc");
    CHECK(s_replaced2.substr(s_replaced2.size() - 4, 3) == "xyz");
    CHECK(s_replaced2_ascii.substr(1, 3) == "abc");
    CHECK(s_replaced2_ascii.substr(s_replaced2_ascii.size() - 4, 3) == "xyz");
}

void check_utf8string(bool success_expected, int byte1, int byte2, int byte3, int byte4);

// create and check a JSON string with up to four UTF-8 bytes
void check_utf8string(bool success_expected, int byte1, int byte2 = -1, int byte3 = -1, int byte4 = -1)
{
    if (++calls % 100000 == 0)
    {
        std::cout << calls << " of 1641521 UTF-8 strings checked" << std::endl;
    }

    static std::string json_string;
    json_string = "\"";

    CAPTURE(byte1)
    json_string += std::string(1, static_cast<char>(byte1));

    if (byte2 != -1)
    {
        CAPTURE(byte2)
        json_string += std::string(1, static_cast<char>(byte2));
    }

    if (byte3 != -1)
    {
        CAPTURE(byte3)
        json_string += std::string(1, static_cast<char>(byte3));
    }

    if (byte4 != -1)
    {
        CAPTURE(byte4)
        json_string += std::string(1, static_cast<char>(byte4));
    }

    json_string += "\"";

    CAPTURE(json_string)

    json _;
    if (success_expected)
    {
        CHECK_NOTHROW(_ = json::parse(json_string));
    }
    else
    {
        CHECK_THROWS_AS(_ = json::parse(json_string), json::parse_error&);
    }
}
} // namespace










TEST(Unicode35RFC3629UTF84XF0X90BFUTF8TailUTF8TailTest, DISABLED_WellFormed)
{
    for (int byte1 = 0xF0; byte1 <= 0xF0; ++byte1)
    {
        for (int byte2 = 0x90; byte2 <= 0xBF; ++byte2)
        {
            for (int byte3 = 0x80; byte3 <= 0xBF; ++byte3)
            {
                for (int byte4 = 0x80; byte4 <= 0xBF; ++byte4)
                {
                    check_utf8string(true, byte1, byte2, byte3, byte4);
                    check_utf8dump(true, byte1, byte2, byte3, byte4);
                }
            }
        }
    }
}

TEST(Unicode35RFC3629UTF84XF0X90BFUTF8TailUTF8TailTest, IllFormedMissingSecondByte)
{
    for (int byte1 = 0xF0; byte1 <= 0xF0; ++byte1)
    {
        check_utf8string(false, byte1);
        check_utf8dump(false, byte1);
    }
}

TEST(Unicode35RFC3629UTF84XF0X90BFUTF8TailUTF8TailTest, DISABLED_IllFormedMissingThirdByte)
{
    for (int byte1 = 0xF0; byte1 <= 0xF0; ++byte1)
    {
        for (int byte2 = 0x90; byte2 <= 0xBF; ++byte2)
        {
            check_utf8string(false, byte1, byte2);
            check_utf8dump(false, byte1, byte2);
        }
    }
}

TEST(Unicode35RFC3629UTF84XF0X90BFUTF8TailUTF8TailTest, DISABLED_IllFormedMissingFourthByte)
{
    for (int byte1 = 0xF0; byte1 <= 0xF0; ++byte1)
    {
        for (int byte2 = 0x90; byte2 <= 0xBF; ++byte2)
        {
            for (int byte3 = 0x80; byte3 <= 0xBF; ++byte3)
            {
                check_utf8string(false, byte1, byte2, byte3);
                check_utf8dump(false, byte1, byte2, byte3);
            }
        }
    }
}

TEST(Unicode35RFC3629UTF84XF0X90BFUTF8TailUTF8TailTest, DISABLED_IllFormedWrongSecondByte)
{
    for (int byte1 = 0xF0; byte1 <= 0xF0; ++byte1)
    {
        for (int byte2 = 0x00; byte2 <= 0xFF; ++byte2)
        {
            // skip correct second byte
            if (0x90 <= byte2 && byte2 <= 0xBF)
            {
                continue;
            }

            for (int byte3 = 0x80; byte3 <= 0xBF; ++byte3)
            {
                for (int byte4 = 0x80; byte4 <= 0xBF; ++byte4)
                {
                    check_utf8string(false, byte1, byte2, byte3, byte4);
                    check_utf8dump(false, byte1, byte2, byte3, byte4);
                }
            }
        }
    }
}

TEST(Unicode35RFC3629UTF84XF0X90BFUTF8TailUTF8TailTest, DISABLED_IllFormedWrongThirdByte)
{
    for (int byte1 = 0xF0; byte1 <= 0xF0; ++byte1)
    {
        for (int byte2 = 0x90; byte2 <= 0xBF; ++byte2)
        {
            for (int byte3 = 0x00; byte3 <= 0xFF; ++byte3)
            {
                // skip correct third byte
                if (0x80 <= byte3 && byte3 <= 0xBF)
                {
                    continue;
                }

                for (int byte4 = 0x80; byte4 <= 0xBF; ++byte4)
                {
                    check_utf8string(false, byte1, byte2, byte3, byte4);
                    check_utf8dump(false, byte1, byte2, byte3, byte4);
                }
            }
        }
    }
}

TEST(Unicode35RFC3629UTF84XF0X90BFUTF8TailUTF8TailTest, DISABLED_IllFormedWrongFourthByte)
{
    for (int byte1 = 0xF0; byte1 <= 0xF0; ++byte1)
    {
        for (int byte2 = 0x90; byte2 <= 0xBF; ++byte2)
        {
            for (int byte3 = 0x80; byte3 <= 0xBF; ++byte3)
            {
                for (int byte4 = 0x00; byte4 <= 0xFF; ++byte4)
                {
                    // skip fourth second byte
                    if (0x80 <= byte3 && byte3 <= 0xBF)
                    {
                        continue;
                    }

                    check_utf8string(false, byte1, byte2, byte3, byte4);
                    check_utf8dump(false, byte1, byte2, byte3, byte4);
                }
            }
        }
    }
}


