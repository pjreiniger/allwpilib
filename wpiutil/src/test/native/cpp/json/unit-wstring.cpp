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

// ICPC errors out on multibyte character sequences in source files
#ifndef __INTEL_COMPILER
namespace
{
bool wstring_is_utf16();
bool wstring_is_utf16()
{
    return (std::wstring(L"💩") == std::wstring(L"\U0001F4A9"));
}

bool u16string_is_utf16();
bool u16string_is_utf16()
{
    return (std::u16string(u"💩") == std::u16string(u"\U0001F4A9"));
}

bool u32string_is_utf32();
bool u32string_is_utf32()
{
    return (std::u32string(U"💩") == std::u32string(U"\U0001F4A9"));
}
} // namespace




TEST(WideStringsTest, StdWstring)
{
    if (wstring_is_utf16())
    {
        std::wstring w = L"[12.2,\"Ⴥaäö💤🧢\"]";
        json j = json::parse(w);
        CHECK(j.dump() == "[12.2,\"Ⴥaäö💤🧢\"]");
    }
}

TEST(WideStringsTest, InvalidStdWstring)
{
    if (wstring_is_utf16())
    {
        std::wstring w = L"\"\xDBFF";
        json _;
        CHECK_THROWS_AS(_ = json::parse(w), json::parse_error&);
    }
}

TEST(WideStringsTest, StdU16string)
{
    if (u16string_is_utf16())
    {
        std::u16string w = u"[12.2,\"Ⴥaäö💤🧢\"]";
        json j = json::parse(w);
        CHECK(j.dump() == "[12.2,\"Ⴥaäö💤🧢\"]");
    }
}

TEST(WideStringsTest, InvalidStdU16string)
{
    if (wstring_is_utf16())
    {
        std::u16string w = u"\"\xDBFF";
        json _;
        CHECK_THROWS_AS(_ = json::parse(w), json::parse_error&);
    }
}

TEST(WideStringsTest, StdU32string)
{
    if (u32string_is_utf32())
    {
        std::u32string w = U"[12.2,\"Ⴥaäö💤🧢\"]";
        json j = json::parse(w);
        CHECK(j.dump() == "[12.2,\"Ⴥaäö💤🧢\"]");
    }
}

TEST(WideStringsTest, InvalidStdU32string)
{
    if (u32string_is_utf32())
    {
        std::u32string w = U"\"\x110000";
        json _;
        CHECK_THROWS_AS(_ = json::parse(w), json::parse_error&);
    }
}
#endif

