//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

#include "gtest/gtest.h"

#define JSON_TESTS_PRIVATE
#include "unit-json.h"
using wpi::json;

#include <algorithm>
#include <climits>
#include <limits>
#include <iostream>
#include <fstream>
#include <set>

namespace
{
class SaxCountdown
{
  public:
    explicit SaxCountdown(const int count) : events_left(count)
    {}

    bool null()
    {
        return events_left-- > 0;
    }

    bool boolean(bool /*unused*/)
    {
        return events_left-- > 0;
    }

    bool number_integer(json::number_integer_t /*unused*/)
    {
        return events_left-- > 0;
    }

    bool number_unsigned(json::number_unsigned_t /*unused*/)
    {
        return events_left-- > 0;
    }

    bool number_float(json::number_float_t /*unused*/, const std::string& /*unused*/)
    {
        return events_left-- > 0;
    }

    bool string(std::string& /*unused*/)
    {
        return events_left-- > 0;
    }

    bool binary(std::vector<std::uint8_t>& /*unused*/)
    {
        return events_left-- > 0;
    }

    bool start_object(std::size_t /*unused*/)
    {
        return events_left-- > 0;
    }

    bool key(std::string& /*unused*/)
    {
        return events_left-- > 0;
    }

    bool end_object()
    {
        return events_left-- > 0;
    }

    bool start_array(std::size_t /*unused*/)
    {
        return events_left-- > 0;
    }

    bool end_array()
    {
        return events_left-- > 0;
    }

    bool parse_error(std::size_t /*unused*/, const std::string& /*unused*/, const json::exception& /*unused*/) // NOLINT(readability-convert-member-functions-to-static)
    {
        return false;
    }

  private:
    int events_left = 0;
};
} // namespace

// at some point in the future, a unit test dedicated to type traits might be a good idea
template <typename OfType, typename T, bool MinInRange, bool MaxInRange>
struct trait_test_arg
{
    using of_type = OfType;
    using type = T;
    static constexpr bool min_in_range = MinInRange;
    static constexpr bool max_in_range = MaxInRange;
};

template <typename T>
class ValueInRangeOfTraitFixture : public testing::Test {
};


using ValueInRangeOfTraitTypes = ::testing::Types<
                          trait_test_arg<std::int32_t, std::int32_t, true, true>, \
                          trait_test_arg<std::int32_t, std::uint32_t, true, false>, \
                          trait_test_arg<std::uint32_t, std::int32_t, false, true>, \
                          trait_test_arg<std::uint32_t, std::uint32_t, true, true>, \
                          trait_test_arg<std::int32_t, std::int64_t, false, false>, \
                          trait_test_arg<std::int32_t, std::uint64_t, true, false>, \
                          trait_test_arg<std::uint32_t, std::int64_t, false, false>, \
                          trait_test_arg<std::uint32_t, std::uint64_t, true, false>, \
                          trait_test_arg<std::int64_t, std::int32_t, true, true>, \
                          trait_test_arg<std::int64_t, std::uint32_t, true, true>, \
                          trait_test_arg<std::uint64_t, std::int32_t, false, true>, \
                          trait_test_arg<std::uint64_t, std::uint32_t, true, true>, \
                          trait_test_arg<std::int64_t, std::int64_t, true, true>, \
                          trait_test_arg<std::int64_t, std::uint64_t, true, false>, \
                          trait_test_arg<std::uint64_t, std::int64_t, false, true>, \
                          trait_test_arg<std::uint64_t, std::uint64_t, true, true> \
                          >;
TYPED_TEST_SUITE(ValueInRangeOfTraitFixture, ValueInRangeOfTraitTypes);


TYPED_TEST(ValueInRangeOfTraitFixture, Example) {
    using T = TypeParam;
    using wpi::detail::value_in_range_of;

    using of_type = typename T::of_type;
    using type = typename T::type;
    constexpr bool min_in_range = T::min_in_range;
    constexpr bool max_in_range = T::max_in_range;

    type val_min = std::numeric_limits<type>::min();
    type val_min2 = val_min + 1;
    type val_max = std::numeric_limits<type>::max();
    type val_max2 = val_max - 1;

    REQUIRE(CHAR_BIT == 8);

    std::string of_type_str;
    if (std::is_unsigned<of_type>::value)
    {
        of_type_str += "u";
    }
    of_type_str += "int";
    of_type_str += std::to_string(sizeof(of_type) * 8);

    INFO("of_type := ", of_type_str);

    std::string type_str;
    if (std::is_unsigned<type>::value)
    {
        type_str += "u";
    }
    type_str += "int";
    type_str += std::to_string(sizeof(type) * 8);

    INFO("type := ", type_str);

    CAPTURE(val_min);
    CAPTURE(min_in_range);
    CAPTURE(val_max);
    CAPTURE(max_in_range);

    if (min_in_range)
    {
        CHECK(value_in_range_of<of_type>(val_min));
        CHECK(value_in_range_of<of_type>(val_min2));
    }
    else
    {
        CHECK_FALSE(value_in_range_of<of_type>(val_min));
        CHECK_FALSE(value_in_range_of<of_type>(val_min2));
    }

    if (max_in_range)
    {
        CHECK(value_in_range_of<of_type>(val_max));
        CHECK(value_in_range_of<of_type>(val_max2));
    }
    else
    {
        CHECK_FALSE(value_in_range_of<of_type>(val_max));
        CHECK_FALSE(value_in_range_of<of_type>(val_max2));
    }
}


TEST(BJDataTest, BinaryReaderBJDataLUTArraysAreSorted)
{
    std::vector<std::uint8_t> data;
    auto ia = wpi::detail::input_adapter(data);
    // NOLINTNEXTLINE(hicpp-move-const-arg,performance-move-const-arg)
    wpi::detail::binary_reader<json, decltype(ia)> br{std::move(ia), json::input_format_t::bjdata};

    CHECK(std::is_sorted(br.bjd_optimized_type_markers.begin(), br.bjd_optimized_type_markers.end()));
    CHECK(std::is_sorted(br.bjd_types_map.begin(), br.bjd_types_map.end()));
}




TEST(BJDataIndividualValuesTest, Discarded)
{
    // discarded values are not serialized
    json j = json::value_t::discarded;
    const auto result = json::to_bjdata(j);
    CHECK(result.empty());
}

TEST(BJDataIndividualValuesTest, Null)
{
    json j = nullptr;
    std::vector<uint8_t> expected = {'Z'};
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}




TEST(BJDataIndividualValuesBooleanTest, True)
{
    json j = true;
    std::vector<uint8_t> expected = {'T'};
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesBooleanTest, False)
{
    json j = false;
    std::vector<uint8_t> expected = {'F'};
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}







TEST(BJDataIndividualValuesNumberSignedTest, Neg0)
{
    std::vector<int64_t> numbers;
    numbers.push_back((std::numeric_limits<int64_t>::min)());
    numbers.push_back(-1000000000000000000LL);
    numbers.push_back(-100000000000000000LL);
    numbers.push_back(-10000000000000000LL);
    numbers.push_back(-1000000000000000LL);
    numbers.push_back(-100000000000000LL);
    numbers.push_back(-10000000000000LL);
    numbers.push_back(-1000000000000LL);
    numbers.push_back(-100000000000LL);
    numbers.push_back(-10000000000LL);
    numbers.push_back(-2147483649LL);
    for (auto i : numbers)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(static_cast<uint8_t>('L'));
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 16) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 24) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 32) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 40) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 48) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 56) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 9);

        // check individual bytes
        CHECK(result[0] == 'L');
        int64_t restored = (static_cast<int64_t>(result[8]) << 070) +
                           (static_cast<int64_t>(result[7]) << 060) +
                           (static_cast<int64_t>(result[6]) << 050) +
                           (static_cast<int64_t>(result[5]) << 040) +
                           (static_cast<int64_t>(result[4]) << 030) +
                           (static_cast<int64_t>(result[3]) << 020) +
                           (static_cast<int64_t>(result[2]) << 010) +
                           static_cast<int64_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, Neg1)
{
    std::vector<int32_t> numbers;
    numbers.push_back(-32769);
    numbers.push_back(-100000);
    numbers.push_back(-1000000);
    numbers.push_back(-10000000);
    numbers.push_back(-100000000);
    numbers.push_back(-1000000000);
    numbers.push_back(-2147483647 - 1); // https://stackoverflow.com/a/29356002/266378
    for (auto i : numbers)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(static_cast<uint8_t>('l'));
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 16) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 24) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 5);

        // check individual bytes
        CHECK(result[0] == 'l');
        int32_t restored = (static_cast<int32_t>(result[4]) << 030) +
                           (static_cast<int32_t>(result[3]) << 020) +
                           (static_cast<int32_t>(result[2]) << 010) +
                           static_cast<int32_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, Neg2)
{
    for (int32_t i = -32768; i <= -129; ++i)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(static_cast<uint8_t>('I'));
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 3);

        // check individual bytes
        CHECK(result[0] == 'I');
        auto restored = static_cast<int16_t>(((result[2] << 8) + result[1]));
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, Neg3)
{
    json j = -9263;
    std::vector<uint8_t> expected = {'I', 0xd1, 0xdb};

    // compare result + size
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);
    CHECK(result.size() == 3);

    // check individual bytes
    CHECK(result[0] == 'I');
    auto restored = static_cast<int16_t>(((result[2] << 8) + result[1]));
    CHECK(restored == -9263);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesNumberSignedTest, Neg4)
{
    for (auto i = -128; i <= -1; ++i)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('i');
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 2);

        // check individual bytes
        CHECK(result[0] == 'i');
        CHECK(static_cast<int8_t>(result[1]) == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, Pos0)
{
    for (size_t i = 0; i <= 127; ++i)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = -1;
        j.get_ref<json::number_integer_t&>() = static_cast<json::number_integer_t>(i);

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(static_cast<uint8_t>('i'));
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 2);

        // check individual bytes
        CHECK(result[0] == 'i');
        CHECK(result[1] == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, Pos1)
{
    for (size_t i = 128; i <= 255; ++i)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = -1;
        j.get_ref<json::number_integer_t&>() = static_cast<json::number_integer_t>(i);

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(static_cast<uint8_t>('U'));
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 2);

        // check individual bytes
        CHECK(result[0] == 'U');
        CHECK(result[1] == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, Pos2)
{
    for (size_t i = 256; i <= 32767; ++i)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = -1;
        j.get_ref<json::number_integer_t&>() = static_cast<json::number_integer_t>(i);

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(static_cast<uint8_t>('I'));
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 3);

        // check individual bytes
        CHECK(result[0] == 'I');
        auto restored = static_cast<uint16_t>(static_cast<uint8_t>(result[2]) * 256 + static_cast<uint8_t>(result[1]));
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, 3276865535Uint16)
{
    for (uint32_t i :
            {
                32768u, 55555u, 65535u
            })
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = -1;
        j.get_ref<json::number_integer_t&>() = static_cast<json::number_integer_t>(i);

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(static_cast<uint8_t>('u'));
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 3);

        // check individual bytes
        CHECK(result[0] == 'u');
        auto restored = static_cast<uint16_t>(static_cast<uint8_t>(result[2]) * 256 + static_cast<uint8_t>(result[1]));
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, Pos3)
{
    for (uint32_t i :
            {
                65536u, 77777u, 2147483647u
            })
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = -1;
        j.get_ref<json::number_integer_t&>() = static_cast<json::number_integer_t>(i);

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('l');
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 16) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 24) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 5);

        // check individual bytes
        CHECK(result[0] == 'l');
        uint32_t restored = (static_cast<uint32_t>(result[4]) << 030) +
                            (static_cast<uint32_t>(result[3]) << 020) +
                            (static_cast<uint32_t>(result[2]) << 010) +
                            static_cast<uint32_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, 21474836484294967295Uint32)
{
    for (uint32_t i :
            {
                2147483648u, 3333333333u, 4294967295u
            })
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = -1;
        j.get_ref<json::number_integer_t&>() = static_cast<json::number_integer_t>(i);

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('m');
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 16) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 24) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 5);

        // check individual bytes
        CHECK(result[0] == 'm');
        uint32_t restored = (static_cast<uint32_t>(result[4]) << 030) +
                            (static_cast<uint32_t>(result[3]) << 020) +
                            (static_cast<uint32_t>(result[2]) << 010) +
                            static_cast<uint32_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, 42949672969223372036854775807Int64)
{
    std::vector<uint64_t> v = {4294967296LU, 9223372036854775807LU};
    for (uint64_t i : v)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = -1;
        j.get_ref<json::number_integer_t&>() = static_cast<json::number_integer_t>(i);

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('L');
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 010) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 020) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 030) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 040) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 050) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 060) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 070) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 9);

        // check individual bytes
        CHECK(result[0] == 'L');
        uint64_t restored = (static_cast<uint64_t>(result[8]) << 070) +
                            (static_cast<uint64_t>(result[7]) << 060) +
                            (static_cast<uint64_t>(result[6]) << 050) +
                            (static_cast<uint64_t>(result[5]) << 040) +
                            (static_cast<uint64_t>(result[4]) << 030) +
                            (static_cast<uint64_t>(result[3]) << 020) +
                            (static_cast<uint64_t>(result[2]) << 010) +
                            static_cast<uint64_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberSignedTest, 922337203685477580818446744073709551615Uint64)
{
    std::vector<uint64_t> v = {9223372036854775808ull, 18446744073709551615ull};
    for (uint64_t i : v)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('M');
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 010) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 020) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 030) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 040) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 050) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 060) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 070) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 9);

        // check individual bytes
        CHECK(result[0] == 'M');
        uint64_t restored = (static_cast<uint64_t>(result[8]) << 070) +
                            (static_cast<uint64_t>(result[7]) << 060) +
                            (static_cast<uint64_t>(result[6]) << 050) +
                            (static_cast<uint64_t>(result[5]) << 040) +
                            (static_cast<uint64_t>(result[4]) << 030) +
                            (static_cast<uint64_t>(result[3]) << 020) +
                            (static_cast<uint64_t>(result[2]) << 010) +
                            static_cast<uint64_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}




TEST(BJDataIndividualValuesNumberUnsignedTest, Pos0)
{
    for (size_t i = 0; i <= 127; ++i)
    {
        CAPTURE(i)

        // create JSON value with unsigned integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('i');
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 2);

        // check individual bytes
        CHECK(result[0] == 'i');
        auto restored = static_cast<uint8_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberUnsignedTest, Pos1)
{
    for (size_t i = 128; i <= 255; ++i)
    {
        CAPTURE(i)

        // create JSON value with unsigned integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('U');
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 2);

        // check individual bytes
        CHECK(result[0] == 'U');
        auto restored = static_cast<uint8_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberUnsignedTest, Pos2)
{
    for (size_t i = 256; i <= 32767; ++i)
    {
        CAPTURE(i)

        // create JSON value with unsigned integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('I');
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 3);

        // check individual bytes
        CHECK(result[0] == 'I');
        auto restored = static_cast<uint16_t>(static_cast<uint8_t>(result[2]) * 256 + static_cast<uint8_t>(result[1]));
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberUnsignedTest, 3276865535Uint16)
{
    for (uint32_t i :
            {
                32768u, 55555u, 65535u
            })
    {
        CAPTURE(i)

        // create JSON value with unsigned integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('u');
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 3);

        // check individual bytes
        CHECK(result[0] == 'u');
        auto restored = static_cast<uint16_t>(static_cast<uint8_t>(result[2]) * 256 + static_cast<uint8_t>(result[1]));
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberUnsignedTest, Pos3)
{
    for (uint32_t i :
            {
                65536u, 77777u, 2147483647u
            })
    {
        CAPTURE(i)

        // create JSON value with unsigned integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('l');
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 16) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 24) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 5);

        // check individual bytes
        CHECK(result[0] == 'l');
        uint32_t restored = (static_cast<uint32_t>(result[4]) << 030) +
                            (static_cast<uint32_t>(result[3]) << 020) +
                            (static_cast<uint32_t>(result[2]) << 010) +
                            static_cast<uint32_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberUnsignedTest, 21474836484294967295Uint32)
{
    for (uint32_t i :
            {
                2147483648u, 3333333333u, 4294967295u
            })
    {
        CAPTURE(i)

        // create JSON value with unsigned integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('m');
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 16) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 24) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 5);

        // check individual bytes
        CHECK(result[0] == 'm');
        uint32_t restored = (static_cast<uint32_t>(result[4]) << 030) +
                            (static_cast<uint32_t>(result[3]) << 020) +
                            (static_cast<uint32_t>(result[2]) << 010) +
                            static_cast<uint32_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberUnsignedTest, 42949672969223372036854775807Int64)
{
    std::vector<uint64_t> v = {4294967296ul, 9223372036854775807ul};
    for (uint64_t i : v)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('L');
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 010) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 020) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 030) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 040) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 050) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 060) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 070) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 9);

        // check individual bytes
        CHECK(result[0] == 'L');
        uint64_t restored = (static_cast<uint64_t>(result[8]) << 070) +
                            (static_cast<uint64_t>(result[7]) << 060) +
                            (static_cast<uint64_t>(result[6]) << 050) +
                            (static_cast<uint64_t>(result[5]) << 040) +
                            (static_cast<uint64_t>(result[4]) << 030) +
                            (static_cast<uint64_t>(result[3]) << 020) +
                            (static_cast<uint64_t>(result[2]) << 010) +
                            static_cast<uint64_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesNumberUnsignedTest, 922337203685477580818446744073709551615Uint64)
{
    std::vector<uint64_t> v = {9223372036854775808ull, 18446744073709551615ull};
    for (uint64_t i : v)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('M');
        expected.push_back(static_cast<uint8_t>(i & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 010) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 020) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 030) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 040) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 050) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 060) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 070) & 0xff));

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == 9);

        // check individual bytes
        CHECK(result[0] == 'M');
        uint64_t restored = (static_cast<uint64_t>(result[8]) << 070) +
                            (static_cast<uint64_t>(result[7]) << 060) +
                            (static_cast<uint64_t>(result[6]) << 050) +
                            (static_cast<uint64_t>(result[5]) << 040) +
                            (static_cast<uint64_t>(result[4]) << 030) +
                            (static_cast<uint64_t>(result[3]) << 020) +
                            (static_cast<uint64_t>(result[2]) << 010) +
                            static_cast<uint64_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}




TEST(BJDataIndividualValuesNumberFloat64Test, Pi)
{
    double v = 3.1415925;
    json j = v;
    std::vector<uint8_t> expected =
    {
        'D', 0xfc, 0xde, 0xa6, 0x3f, 0xfb, 0x21, 0x09, 0x40
    };
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result) == v);
    CHECK(json::from_bjdata(result, true, false) == j);
}




TEST(BJDataIndividualValuesNumberHalfPrecisionFloatTest, SimpleHalfFloats)
{
    CHECK(json::parse("0.0") == json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0x00})));
    CHECK(json::parse("-0.0") == json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0x80})));
    CHECK(json::parse("1.0") == json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0x3c})));
    CHECK(json::parse("1.5") == json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0x3e})));
    CHECK(json::parse("65504.0") == json::from_bjdata(std::vector<uint8_t>({'h', 0xff, 0x7b})));
}




TEST(BJDataIndividualValuesNumberHalfPrecisionFloatErrorsTest, NoByteFollows)
{
    json _;
    std::vector<uint8_t> vec0 = {'h'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vec0), "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vec0, true, false).is_discarded());
}

TEST(BJDataIndividualValuesNumberHalfPrecisionFloatErrorsTest, OnlyOneByteFollows)
{
    json _;
    std::vector<uint8_t> vec1 = {'h', 0x00};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vec1), "[json.exception.parse_error.110] parse error at byte 3: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vec1, true, false).is_discarded());
}







TEST(BJDataIndividualValuesNumberHalfPrecisionFloatEdgeCasesExp0b00000Test, 00000000000000000)
{
    json j = json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0x00}));
    json::number_float_t d{j};
    CHECK(d == 0.0);
}

TEST(BJDataIndividualValuesNumberHalfPrecisionFloatEdgeCasesExp0b00000Test, 01000000000000000)
{
    json j = json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0x80}));
    json::number_float_t d{j};
    CHECK(d == -0.0);
}

TEST(BJDataIndividualValuesNumberHalfPrecisionFloatEdgeCasesExp0b00000Test, 2PointerPointer240000000000000001)
{
    json j = json::from_bjdata(std::vector<uint8_t>({'h', 0x01, 0x00}));
    json::number_float_t d{j};
    CHECK(d == std::pow(2.0, -24.0));
}




TEST(BJDataIndividualValuesNumberHalfPrecisionFloatEdgeCasesExp0b11111Test, Infinity0111110000000000)
{
    json j = json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0x7c}));
    json::number_float_t d{j};
    CHECK(d == std::numeric_limits<json::number_float_t>::infinity());
    CHECK(j.dump() == "null");
}

TEST(BJDataIndividualValuesNumberHalfPrecisionFloatEdgeCasesExp0b11111Test, Infinity1111110000000000)
{
    json j = json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0xfc}));
    json::number_float_t d{j};
    CHECK(d == -std::numeric_limits<json::number_float_t>::infinity());
    CHECK(j.dump() == "null");
}




TEST(BJDataIndividualValuesNumberHalfPrecisionFloatEdgeCasesOtherValuesFromHttpsEnWikipediaOrgWikiHalfPrecisionFloatingPointFormatTest, 10011110000000000)
{
    json j = json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0x3c}));
    json::number_float_t d{j};
    CHECK(d == 1);
}

TEST(BJDataIndividualValuesNumberHalfPrecisionFloatEdgeCasesOtherValuesFromHttpsEnWikipediaOrgWikiHalfPrecisionFloatingPointFormatTest, 21100000000000000)
{
    json j = json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0xc0}));
    json::number_float_t d{j};
    CHECK(d == -2);
}

TEST(BJDataIndividualValuesNumberHalfPrecisionFloatEdgeCasesOtherValuesFromHttpsEnWikipediaOrgWikiHalfPrecisionFloatingPointFormatTest, 655040111101111111111)
{
    json j = json::from_bjdata(std::vector<uint8_t>({'h', 0xff, 0x7b}));
    json::number_float_t d{j};
    CHECK(d == 65504);
}

TEST(BJDataIndividualValuesNumberHalfPrecisionFloatEdgeCasesTest, Infinity)
{
    json j = json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0x7c}));
    json::number_float_t d{j};
    CHECK_FALSE(std::isfinite(d));
    CHECK(j.dump() == "null");
}

TEST(BJDataIndividualValuesNumberHalfPrecisionFloatEdgeCasesTest, NaN)
{
    json j = json::from_bjdata(std::vector<uint8_t>({'h', 0x00, 0x7e }));
    json::number_float_t d{j};
    CHECK(std::isnan(d));
    CHECK(j.dump() == "null");
}




TEST(BJDataIndividualValuesNumberHighPrecisionNumberTest, UnsignedIntegerNumber)
{
    std::vector<uint8_t> vec = {'H', 'i', 0x14, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    const auto j = json::from_bjdata(vec);
    CHECK(j.is_number_unsigned());
    CHECK(j.dump() == "12345678901234567890");
}

TEST(BJDataIndividualValuesNumberHighPrecisionNumberTest, SignedIntegerNumber)
{
    std::vector<uint8_t> vec = {'H', 'i', 0x13, '-', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8'};
    const auto j = json::from_bjdata(vec);
    CHECK(j.is_number_integer());
    CHECK(j.dump() == "-123456789012345678");
}

TEST(BJDataIndividualValuesNumberHighPrecisionNumberTest, FloatingPointNumber)
{
    std::vector<uint8_t> vec = {'H', 'i', 0x16, '3', '.', '1', '4', '1', '5', '9',  '2', '6', '5', '3', '5', '8', '9',  '7', '9', '3', '2', '3', '8', '4',  '6'};
    const auto j = json::from_bjdata(vec);
    CHECK(j.is_number_float());
    CHECK(j.dump() == "3.141592653589793");
}

TEST(BJDataIndividualValuesNumberHighPrecisionNumberTest, Errors)
{
    // error while parsing length
    std::vector<uint8_t> vec0 = {'H', 'i'};
    CHECK(json::from_bjdata(vec0, true, false).is_discarded());
    // error while parsing string
    std::vector<uint8_t> vec1 = {'H', 'i', '1'};
    CHECK(json::from_bjdata(vec1, true, false).is_discarded());

    json _;
    std::vector<uint8_t> vec2 = {'H', 'i', 2, '1', 'A', '3'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vec2), "[json.exception.parse_error.115] parse error at byte 5: syntax error while parsing BJData high-precision number: invalid number text: 1A", json::parse_error);
    std::vector<uint8_t> vec3 = {'H', 'i', 2, '1', '.'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vec3), "[json.exception.parse_error.115] parse error at byte 5: syntax error while parsing BJData high-precision number: invalid number text: 1.", json::parse_error);
    std::vector<uint8_t> vec4 = {'H', 2, '1', '0'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vec4), "[json.exception.parse_error.113] parse error at byte 2: syntax error while parsing BJData size: expected length type specification (U, i, u, I, m, l, M, L) after '#'; last byte: 0x02", json::parse_error);
}




TEST(BJDataIndividualValuesStringTest, String1)
{
    for (size_t N = 0; N <= 127; ++N)
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::string(N, 'x');
        json j = s;

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('S');
        expected.push_back('i');
        expected.push_back(static_cast<uint8_t>(N));
        for (size_t i = 0; i < N; ++i)
        {
            expected.push_back('x');
        }

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 3);
        // check that no null byte is appended
        if (N > 0)
        {
            CHECK(result.back() != '\x00');
        }

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesStringTest, String2)
{
    for (size_t N = 128; N <= 255; ++N)
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::string(N, 'x');
        json j = s;

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back('S');
        expected.push_back('U');
        expected.push_back(static_cast<uint8_t>(N));
        for (size_t i = 0; i < N; ++i)
        {
            expected.push_back('x');
        }

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 3);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesStringTest, String3)
{
    for (size_t N :
            {
                256u, 999u, 1025u, 3333u, 2048u, 32767u
            })
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::string(N, 'x');
        json j = s;

        // create expected byte vector (hack: create string first)
        std::vector<uint8_t> expected(N, 'x');
        // reverse order of commands, because we insert at begin()
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 8) & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>(N & 0xff));
        expected.insert(expected.begin(), 'I');
        expected.insert(expected.begin(), 'S');

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 4);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesStringTest, N3276865535)
{
    for (size_t N :
            {
                32768u, 55555u, 65535u
            })
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::string(N, 'x');
        json j = s;

        // create expected byte vector (hack: create string first)
        std::vector<uint8_t> expected(N, 'x');
        // reverse order of commands, because we insert at begin()
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 8) & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>(N & 0xff));
        expected.insert(expected.begin(), 'u');
        expected.insert(expected.begin(), 'S');

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 4);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}

TEST(BJDataIndividualValuesStringTest, String4)
{
    for (size_t N :
            {
                65536u, 77777u, 1048576u
            })
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::string(N, 'x');
        json j = s;

        // create expected byte vector (hack: create string first)
        std::vector<uint8_t> expected(N, 'x');
        // reverse order of commands, because we insert at begin()
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 24) & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 16) & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 8) & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>(N & 0xff));
        expected.insert(expected.begin(), 'l');
        expected.insert(expected.begin(), 'S');

        // compare result + size
        const auto result = json::to_bjdata(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 6);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_bjdata(result) == j);
        CHECK(json::from_bjdata(result, true, false) == j);
    }
}




TEST(BJDataIndividualValuesBinaryTest, String1)
{
    for (std::size_t N = 0; N <= 127; ++N)
    {
        CAPTURE(N)

        // create JSON value with byte array containing of N * 'x'
        const auto s = std::vector<std::uint8_t>(N, 'x');
        json j = json::binary(s);

        // create expected byte vector
        std::vector<std::uint8_t> expected;
        expected.push_back(static_cast<std::uint8_t>('['));
        if (N != 0)
        {
            expected.push_back(static_cast<std::uint8_t>('$'));
            expected.push_back(static_cast<std::uint8_t>('U'));
        }
        expected.push_back(static_cast<std::uint8_t>('#'));
        expected.push_back(static_cast<std::uint8_t>('i'));
        expected.push_back(static_cast<std::uint8_t>(N));
        for (size_t i = 0; i < N; ++i)
        {
            expected.push_back(0x78);
        }

        // compare result + size
        const auto result = json::to_bjdata(j, true, true);
        CHECK(result == expected);
        if (N == 0)
        {
            CHECK(result.size() == N + 4);
        }
        else
        {
            CHECK(result.size() == N + 6);
        }

        // check that no null byte is appended
        if (N > 0)
        {
            CHECK(result.back() != '\x00');
        }

        // roundtrip only works to an array of numbers
        json j_out = s;
        CHECK(json::from_bjdata(result) == j_out);
        CHECK(json::from_bjdata(result, true, false) == j_out);
    }
}

TEST(BJDataIndividualValuesBinaryTest, String2)
{
    for (std::size_t N = 128; N <= 255; ++N)
    {
        CAPTURE(N)

        // create JSON value with byte array containing of N * 'x'
        const auto s = std::vector<std::uint8_t>(N, 'x');
        json j = json::binary(s);

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(static_cast<std::uint8_t>('['));
        expected.push_back(static_cast<std::uint8_t>('$'));
        expected.push_back(static_cast<std::uint8_t>('U'));
        expected.push_back(static_cast<std::uint8_t>('#'));
        expected.push_back(static_cast<std::uint8_t>('U'));
        expected.push_back(static_cast<std::uint8_t>(N));
        for (size_t i = 0; i < N; ++i)
        {
            expected.push_back(0x78);
        }

        // compare result + size
        const auto result = json::to_bjdata(j, true, true);
        CHECK(result == expected);
        CHECK(result.size() == N + 6);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip only works to an array of numbers
        json j_out = s;
        CHECK(json::from_bjdata(result) == j_out);
        CHECK(json::from_bjdata(result, true, false) == j_out);
    }
}

TEST(BJDataIndividualValuesBinaryTest, String3)
{
    for (std::size_t N :
            {
                256u, 999u, 1025u, 3333u, 2048u, 32767u
            })
    {
        CAPTURE(N)

        // create JSON value with byte array containing of N * 'x'
        const auto s = std::vector<std::uint8_t>(N, 'x');
        json j = json::binary(s);

        // create expected byte vector
        std::vector<std::uint8_t> expected(N + 7, 'x');
        expected[0] = '[';
        expected[1] = '$';
        expected[2] = 'U';
        expected[3] = '#';
        expected[4] = 'I';
        expected[5] = static_cast<std::uint8_t>(N & 0xFF);
        expected[6] = static_cast<std::uint8_t>((N >> 8) & 0xFF);

        // compare result + size
        const auto result = json::to_bjdata(j, true, true);
        CHECK(result == expected);
        CHECK(result.size() == N + 7);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip only works to an array of numbers
        json j_out = s;
        CHECK(json::from_bjdata(result) == j_out);
        CHECK(json::from_bjdata(result, true, false) == j_out);
    }
}

TEST(BJDataIndividualValuesBinaryTest, N3276865535)
{
    for (std::size_t N :
            {
                32768u, 55555u, 65535u
            })
    {
        CAPTURE(N)

        // create JSON value with byte array containing of N * 'x'
        const auto s = std::vector<std::uint8_t>(N, 'x');
        json j = json::binary(s);

        // create expected byte vector
        std::vector<std::uint8_t> expected(N + 7, 'x');
        expected[0] = '[';
        expected[1] = '$';
        expected[2] = 'U';
        expected[3] = '#';
        expected[4] = 'u';
        expected[5] = static_cast<std::uint8_t>(N & 0xFF);
        expected[6] = static_cast<std::uint8_t>((N >> 8) & 0xFF);

        // compare result + size
        const auto result = json::to_bjdata(j, true, true);
        CHECK(result == expected);
        CHECK(result.size() == N + 7);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip only works to an array of numbers
        json j_out = s;
        CHECK(json::from_bjdata(result) == j_out);
        CHECK(json::from_bjdata(result, true, false) == j_out);
    }
}

TEST(BJDataIndividualValuesBinaryTest, String4)
{
    for (std::size_t N :
            {
                65536u, 77777u, 1048576u
            })
    {
        CAPTURE(N)

        // create JSON value with byte array containing of N * 'x'
        const auto s = std::vector<std::uint8_t>(N, 'x');
        json j = json::binary(s);

        // create expected byte vector
        std::vector<std::uint8_t> expected(N + 9, 'x');
        expected[0] = '[';
        expected[1] = '$';
        expected[2] = 'U';
        expected[3] = '#';
        expected[4] = 'l';
        expected[5] = static_cast<std::uint8_t>(N & 0xFF);
        expected[6] = static_cast<std::uint8_t>((N >> 8) & 0xFF);
        expected[7] = static_cast<std::uint8_t>((N >> 16) & 0xFF);
        expected[8] = static_cast<std::uint8_t>((N >> 24) & 0xFF);

        // compare result + size
        const auto result = json::to_bjdata(j, true, true);
        CHECK(result == expected);
        CHECK(result.size() == N + 9);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip only works to an array of numbers
        json j_out = s;
        CHECK(json::from_bjdata(result) == j_out);
        CHECK(json::from_bjdata(result, true, false) == j_out);
    }
}

class BJDataIndividualValuesBinaryOtherSerializationsTest : public ::testing::Test {
 protected:
    BJDataIndividualValuesBinaryOtherSerializationsTest() : s(N, 'x') {
        j = json::binary(s);
    }
    static constexpr std::size_t N = 10;
    const std::vector<std::uint8_t> s;
    json j;
};


TEST_F(BJDataIndividualValuesBinaryOtherSerializationsTest, NoCountNoType)
{
    std::vector<uint8_t> expected;
    expected.push_back(static_cast<std::uint8_t>('['));
    for (std::size_t i = 0; i < N; ++i)
    {
        expected.push_back(static_cast<std::uint8_t>('U'));
        expected.push_back(static_cast<std::uint8_t>(0x78));
    }
    expected.push_back(static_cast<std::uint8_t>(']'));

    // compare result + size
    const auto result = json::to_bjdata(j, false, false);
    CHECK(result == expected);
    CHECK(result.size() == N + 12);
    // check that no null byte is appended
    CHECK(result.back() != '\x00');

    // roundtrip only works to an array of numbers
    json j_out = s;
    CHECK(json::from_bjdata(result) == j_out);
    CHECK(json::from_bjdata(result, true, false) == j_out);
}

TEST_F(BJDataIndividualValuesBinaryOtherSerializationsTest, YesCountNoType)
{
    std::vector<std::uint8_t> expected;
    expected.push_back(static_cast<std::uint8_t>('['));
    expected.push_back(static_cast<std::uint8_t>('#'));
    expected.push_back(static_cast<std::uint8_t>('i'));
    expected.push_back(static_cast<std::uint8_t>(N));

    for (size_t i = 0; i < N; ++i)
    {
        expected.push_back(static_cast<std::uint8_t>('U'));
        expected.push_back(static_cast<std::uint8_t>(0x78));
    }

    // compare result + size
    const auto result = json::to_bjdata(j, true, false);
    CHECK(result == expected);
    CHECK(result.size() == N + 14);
    // check that no null byte is appended
    CHECK(result.back() != '\x00');

    // roundtrip only works to an array of numbers
    json j_out = s;
    CHECK(json::from_bjdata(result) == j_out);
    CHECK(json::from_bjdata(result, true, false) == j_out);
}







TEST(BJDataIndividualValuesArrayEmptyTest, SizeFalseTypeFalse)
{
    json j = json::array();
    std::vector<uint8_t> expected = {'[', ']'};
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArrayEmptyTest, SizeTrueTypeFalse)
{
    json j = json::array();
    std::vector<uint8_t> expected = {'[', '#', 'i', 0};
    const auto result = json::to_bjdata(j, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArrayEmptyTest, SizeTrueTypeTrue)
{
    json j = json::array();
    std::vector<uint8_t> expected = {'[', '#', 'i', 0};
    const auto result = json::to_bjdata(j, true, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}




TEST(BJDataIndividualValuesArrayNullTest, SizeFalseTypeFalse)
{
    json j = {nullptr};
    std::vector<uint8_t> expected = {'[', 'Z', ']'};
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArrayNullTest, SizeTrueTypeFalse)
{
    json j = {nullptr};
    std::vector<uint8_t> expected = {'[', '#', 'i', 1, 'Z'};
    const auto result = json::to_bjdata(j, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArrayNullTest, SizeTrueTypeTrue)
{
    json j = {nullptr};
    std::vector<uint8_t> expected = {'[', '#', 'i', 1, 'Z'};
    const auto result = json::to_bjdata(j, true, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}




TEST(BJDataIndividualValuesArraySimpleTest, SizeFalseTypeFalse)
{
    json j = json::parse("[1,2,3,4,5]");
    std::vector<uint8_t> expected = {'[', 'i', 1, 'i', 2, 'i', 3, 'i', 4, 'i', 5, ']'};
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArraySimpleTest, SizeTrueTypeFalse)
{
    json j = json::parse("[1,2,3,4,5]");
    std::vector<uint8_t> expected = {'[', '#', 'i', 5, 'i', 1, 'i', 2, 'i', 3, 'i', 4, 'i', 5};
    const auto result = json::to_bjdata(j, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArraySimpleTest, SizeTrueTypeTrue)
{
    json j = json::parse("[1,2,3,4,5]");
    std::vector<uint8_t> expected = {'[', '$', 'i', '#', 'i', 5, 1, 2, 3, 4, 5};
    const auto result = json::to_bjdata(j, true, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}




TEST(BJDataIndividualValuesArrayNestedEmptyTest, SizeFalseTypeFalse)
{
    json j = json::parse("[[[[]]]]");
    std::vector<uint8_t> expected = {'[', '[', '[', '[', ']', ']', ']', ']'};
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArrayNestedEmptyTest, SizeTrueTypeFalse)
{
    json j = json::parse("[[[[]]]]");
    std::vector<uint8_t> expected = {'[', '#', 'i', 1, '[', '#', 'i', 1, '[', '#', 'i', 1, '[', '#', 'i', 0};
    const auto result = json::to_bjdata(j, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArrayNestedEmptyTest, SizeTrueTypeTrue)
{
    json j = json::parse("[[[[]]]]");
    std::vector<uint8_t> expected = {'[', '#', 'i', 1, '[', '#', 'i', 1, '[', '#', 'i', 1, '[', '#', 'i', 0};
    const auto result = json::to_bjdata(j, true, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}




TEST(BJDataIndividualValuesArrayArrayWithInt16ElementsTest, SizeFalseTypeFalse)
{
    json j(257, nullptr);
    std::vector<uint8_t> expected(j.size() + 2, 'Z'); // all null
    expected[0] = '['; // opening array
    expected[258] = ']'; // closing array
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArrayArrayWithInt16ElementsTest, SizeTrueTypeFalse)
{
    json j(257, nullptr);
    std::vector<uint8_t> expected(j.size() + 5, 'Z'); // all null
    expected[0] = '['; // opening array
    expected[1] = '#'; // array size
    expected[2] = 'I'; // int16
    expected[3] = 0x01; // 0x0101, first byte
    expected[4] = 0x01; // 0x0101, second byte
    const auto result = json::to_bjdata(j, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}




TEST(BJDataIndividualValuesArrayArrayWithUint16ElementsTest, SizeFalseTypeFalse)
{
    json j(32768, nullptr);
    std::vector<uint8_t> expected(j.size() + 2, 'Z'); // all null
    expected[0] = '['; // opening array
    expected[32769] = ']'; // closing array
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArrayArrayWithUint16ElementsTest, SizeTrueTypeFalse)
{
    json j(32768, nullptr);
    std::vector<uint8_t> expected(j.size() + 5, 'Z'); // all null
    expected[0] = '['; // opening array
    expected[1] = '#'; // array size
    expected[2] = 'u'; // int16
    expected[3] = 0x00; // 0x0101, first byte
    expected[4] = 0x80; // 0x0101, second byte
    const auto result = json::to_bjdata(j, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}




TEST(BJDataIndividualValuesArrayArrayWithInt32ElementsTest, SizeFalseTypeFalse)
{
    json j(65793, nullptr);
    std::vector<uint8_t> expected(j.size() + 2, 'Z'); // all null
    expected[0] = '['; // opening array
    expected[65794] = ']'; // closing array
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesArrayArrayWithInt32ElementsTest, SizeTrueTypeFalse)
{
    json j(65793, nullptr);
    std::vector<uint8_t> expected(j.size() + 7, 'Z'); // all null
    expected[0] = '['; // opening array
    expected[1] = '#'; // array size
    expected[2] = 'l'; // int32
    expected[3] = 0x01; // 0x00010101, fourth byte
    expected[4] = 0x01; // 0x00010101, third byte
    expected[5] = 0x01; // 0x00010101, second byte
    expected[6] = 0x00; // 0x00010101, first byte
    const auto result = json::to_bjdata(j, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}







TEST(BJDataIndividualValuesObjectEmptyTest, SizeFalseTypeFalse)
{
    json j = json::object();
    std::vector<uint8_t> expected = {'{', '}'};
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesObjectEmptyTest, SizeTrueTypeFalse)
{
    json j = json::object();
    std::vector<uint8_t> expected = {'{', '#', 'i', 0};
    const auto result = json::to_bjdata(j, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesObjectEmptyTest, SizeTrueTypeTrue)
{
    json j = json::object();
    std::vector<uint8_t> expected = {'{', '#', 'i', 0};
    const auto result = json::to_bjdata(j, true, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}




TEST(BJDataIndividualValuesObjectEmptyKeyTest, SizeFalseTypeFalse)
{
    json j = {{"", nullptr}};
    std::vector<uint8_t> expected = {'{', 'i', 0, 'Z', '}'};
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesObjectEmptyKeyTest, SizeTrueTypeFalse)
{
    json j = {{"", nullptr}};
    std::vector<uint8_t> expected = {'{', '#', 'i', 1, 'i', 0, 'Z'};
    const auto result = json::to_bjdata(j, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}




TEST(BJDataIndividualValuesObjectNestedEmptyTest, SizeFalseTypeFalse)
{
    json j = json::parse(R"({"a": {"b": {"c": {}}}})");
    std::vector<uint8_t> expected =
    {
        '{', 'i', 1, 'a', '{', 'i', 1, 'b', '{', 'i', 1, 'c', '{', '}', '}', '}', '}'
    };
    const auto result = json::to_bjdata(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesObjectNestedEmptyTest, SizeTrueTypeFalse)
{
    json j = json::parse(R"({"a": {"b": {"c": {}}}})");
    std::vector<uint8_t> expected =
    {
        '{', '#', 'i', 1, 'i', 1, 'a', '{', '#', 'i', 1, 'i', 1, 'b', '{', '#', 'i', 1, 'i', 1, 'c', '{', '#', 'i', 0
    };
    const auto result = json::to_bjdata(j, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}

TEST(BJDataIndividualValuesObjectNestedEmptyTest, SizeTrueTypeTrueIgnoreObjectTypeMarker)
{
    json j = json::parse(R"({"a": {"b": {"c": {}}}})");
    std::vector<uint8_t> expected =
    {
        '{', '#', 'i', 1, 'i', 1, 'a', '{', '#', 'i', 1, 'i', 1, 'b', '{', '#', 'i', 1, 'i', 1, 'c', '{', '#', 'i', 0
    };
    const auto result = json::to_bjdata(j, true, true);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_bjdata(result) == j);
    CHECK(json::from_bjdata(result, true, false) == j);
}




class BJDataErrorsStrictModeTest : public ::testing::Test {
 protected:
    std::vector<uint8_t> vec = {'Z', 'Z'};
};


TEST_F(BJDataErrorsStrictModeTest, NonStrictMode)
{
    const auto result = json::from_bjdata(vec, false);
    CHECK(result == json());
}

TEST_F(BJDataErrorsStrictModeTest, StrictMode)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vec),
                         "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing BJData value: expected end of input; last byte: 0x5A", json::parse_error&);
}




TEST(BJDataSAXAbortsTest, StartArray)
{
    std::vector<uint8_t> v = {'[', 'T', 'F', ']'};
    SaxCountdown scp(0);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, StartObject)
{
    std::vector<uint8_t> v = {'{', 'i', 3, 'f', 'o', 'o', 'F', '}'};
    SaxCountdown scp(0);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, KeyInObject)
{
    std::vector<uint8_t> v = {'{', 'i', 3, 'f', 'o', 'o', 'F', '}'};
    SaxCountdown scp(1);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, StartArrayLen)
{
    std::vector<uint8_t> v = {'[', '#', 'i', '2', 'T', 'F'};
    SaxCountdown scp(0);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, StartObjectLen)
{
    std::vector<uint8_t> v = {'{', '#', 'i', '1', 3, 'f', 'o', 'o', 'F'};
    SaxCountdown scp(0);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, KeyInObjectWithLength)
{
    std::vector<uint8_t> v = {'{', 'i', 3, 'f', 'o', 'o', 'F', '}'};
    SaxCountdown scp(1);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, StartArrayInNdarrayArraySize)
{
    std::vector<uint8_t> v = {'[', '$', 'i', '#', '[', '$', 'i', '#', 'i', 2, 2, 1, 1, 2};
    SaxCountdown scp(2);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, NumberIntegerInNdarrayArraySize)
{
    std::vector<uint8_t> v = {'[', '$', 'U', '#', '[', '$', 'i', '#', 'i', 2, 2, 1, 1, 2};
    SaxCountdown scp(3);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, KeyInNdarrayArrayType)
{
    std::vector<uint8_t> v = {'[', '$', 'U', '#', '[', '$', 'U', '#', 'i', 2, 2, 2, 1, 2, 3, 4};
    SaxCountdown scp(6);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, StringInNdarrayArrayType)
{
    std::vector<uint8_t> v = {'[', '$', 'U', '#', '[', '$', 'U', '#', 'i', 2, 2, 2, 1, 2, 3, 4};
    SaxCountdown scp(7);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, KeyInNdarrayArrayData)
{
    std::vector<uint8_t> v = {'[', '$', 'U', '#', '[', '$', 'U', '#', 'i', 2, 2, 2, 1, 2, 3, 4};
    SaxCountdown scp(8);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, StringInNdarrayArrayData)
{
    std::vector<uint8_t> v = {'[', '$', 'U', '#', '[', '$', 'U', '#', 'i', 2, 2, 2, 1, 2, 3, 4};
    SaxCountdown scp(9);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, StringInNdarrayArrayType2)
{
    std::vector<uint8_t> v = {'[', '$', 'U', '#', '[', '$', 'i', '#', 'i', 2, 3, 2, 6, 5, 4, 3, 2, 1};
    SaxCountdown scp(11);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}

TEST(BJDataSAXAbortsTest, StartArrayInNdarrayArrayData)
{
    std::vector<uint8_t> v = {'[', '$', 'U', '#', '[', 'i', 2, 'i', 3, ']', 6, 5, 4, 3, 2, 1};
    SaxCountdown scp(13);
    CHECK_FALSE(json::sax_parse(v, &scp, json::input_format_t::bjdata));
}




TEST(BJDataParsingValuesTest, Strings)
{
    // create a single-character string for all number types
    std::vector<uint8_t> s_i = {'S', 'i', 1, 'a'};
    std::vector<uint8_t> s_U = {'S', 'U', 1, 'a'};
    std::vector<uint8_t> s_I = {'S', 'I', 1, 0, 'a'};
    std::vector<uint8_t> s_u = {'S', 'u', 1, 0, 'a'};
    std::vector<uint8_t> s_l = {'S', 'l', 1, 0, 0, 0, 'a'};
    std::vector<uint8_t> s_m = {'S', 'm', 1, 0, 0, 0, 'a'};
    std::vector<uint8_t> s_L = {'S', 'L', 1, 0, 0, 0, 0, 0, 0, 0, 'a'};
    std::vector<uint8_t> s_M = {'S', 'M', 1, 0, 0, 0, 0, 0, 0, 0, 'a'};

    // check if string is parsed correctly to "a"
    CHECK(json::from_bjdata(s_i) == "a");
    CHECK(json::from_bjdata(s_U) == "a");
    CHECK(json::from_bjdata(s_I) == "a");
    CHECK(json::from_bjdata(s_u) == "a");
    CHECK(json::from_bjdata(s_l) == "a");
    CHECK(json::from_bjdata(s_m) == "a");
    CHECK(json::from_bjdata(s_L) == "a");
    CHECK(json::from_bjdata(s_M) == "a");

    // roundtrip: output should be optimized
    CHECK(json::to_bjdata(json::from_bjdata(s_i)) == s_i);
    CHECK(json::to_bjdata(json::from_bjdata(s_U)) == s_i);
    CHECK(json::to_bjdata(json::from_bjdata(s_I)) == s_i);
    CHECK(json::to_bjdata(json::from_bjdata(s_u)) == s_i);
    CHECK(json::to_bjdata(json::from_bjdata(s_l)) == s_i);
    CHECK(json::to_bjdata(json::from_bjdata(s_m)) == s_i);
    CHECK(json::to_bjdata(json::from_bjdata(s_L)) == s_i);
    CHECK(json::to_bjdata(json::from_bjdata(s_M)) == s_i);
}




TEST(BJDataParsingValuesNumberTest, Float)
{
    // float32
    std::vector<uint8_t> v_d = {'d', 0xd0, 0x0f, 0x49, 0x40};
    CHECK(json::from_bjdata(v_d) == 3.14159f);

    // float64
    std::vector<uint8_t> v_D = {'D', 0x6e, 0x86, 0x1b, 0xf0, 0xf9, 0x21, 0x09, 0x40};
    CHECK(json::from_bjdata(v_D) == 3.14159);

    // float32 is serialized as float64 as the library does not support float32
    CHECK(json::to_bjdata(json::from_bjdata(v_d)) == json::to_bjdata(3.14159f));
}




TEST(BJDataParsingValuesArrayTest, OptimizedVersionLengthOnly)
{
    // create vector with two elements of the same type
    std::vector<uint8_t> v_TU = {'[', '#', 'U', 2, 'T', 'T'};
    std::vector<uint8_t> v_T = {'[', '#', 'i', 2, 'T', 'T'};
    std::vector<uint8_t> v_F = {'[', '#', 'i', 2, 'F', 'F'};
    std::vector<uint8_t> v_Z = {'[', '#', 'i', 2, 'Z', 'Z'};
    std::vector<uint8_t> v_i = {'[', '#', 'i', 2, 'i', 0x7F, 'i', 0x7F};
    std::vector<uint8_t> v_U = {'[', '#', 'i', 2, 'U', 0xFF, 'U', 0xFF};
    std::vector<uint8_t> v_I = {'[', '#', 'i', 2, 'I', 0xFF, 0x7F, 'I', 0xFF, 0x7F};
    std::vector<uint8_t> v_u = {'[', '#', 'i', 2, 'u', 0x0F, 0xA7, 'u', 0x0F, 0xA7};
    std::vector<uint8_t> v_l = {'[', '#', 'i', 2, 'l', 0xFF, 0xFF, 0xFF, 0x7F, 'l', 0xFF, 0xFF, 0xFF, 0x7F};
    std::vector<uint8_t> v_m = {'[', '#', 'i', 2, 'm', 0xFF, 0xC9, 0x9A, 0xBB, 'm', 0xFF, 0xC9, 0x9A, 0xBB};
    std::vector<uint8_t> v_L = {'[', '#', 'i', 2, 'L', 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 'L', 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F};
    std::vector<uint8_t> v_M = {'[', '#', 'i', 2, 'M', 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D, 'M', 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D};
    std::vector<uint8_t> v_D = {'[', '#', 'i', 2, 'D', 0x4a, 0xd8, 0x12, 0x4d, 0xfb, 0x21, 0x09, 0x40, 'D', 0x4a, 0xd8, 0x12, 0x4d, 0xfb, 0x21, 0x09, 0x40};
    std::vector<uint8_t> v_S = {'[', '#', 'i', 2, 'S', 'i', 1, 'a', 'S', 'i', 1, 'a'};
    std::vector<uint8_t> v_C = {'[', '#', 'i', 2, 'C', 'a', 'C', 'a'};

    // check if vector is parsed correctly
    CHECK(json::from_bjdata(v_TU) == json({true, true}));
    CHECK(json::from_bjdata(v_T) == json({true, true}));
    CHECK(json::from_bjdata(v_F) == json({false, false}));
    CHECK(json::from_bjdata(v_Z) == json({nullptr, nullptr}));
    CHECK(json::from_bjdata(v_i) == json({127, 127}));
    CHECK(json::from_bjdata(v_U) == json({255, 255}));
    CHECK(json::from_bjdata(v_I) == json({32767, 32767}));
    CHECK(json::from_bjdata(v_u) == json({42767, 42767}));
    CHECK(json::from_bjdata(v_l) == json({2147483647, 2147483647}));
    CHECK(json::from_bjdata(v_m) == json({3147483647, 3147483647}));
    CHECK(json::from_bjdata(v_L) == json({9223372036854775807, 9223372036854775807}));
    CHECK(json::from_bjdata(v_M) == json({10223372036854775807ull, 10223372036854775807ull}));
    CHECK(json::from_bjdata(v_D) == json({3.1415926, 3.1415926}));
    CHECK(json::from_bjdata(v_S) == json({"a", "a"}));
    CHECK(json::from_bjdata(v_C) == json({"a", "a"}));

    // roundtrip: output should be optimized
    CHECK(json::to_bjdata(json::from_bjdata(v_T), true) == v_T);
    CHECK(json::to_bjdata(json::from_bjdata(v_F), true) == v_F);
    CHECK(json::to_bjdata(json::from_bjdata(v_Z), true) == v_Z);
    CHECK(json::to_bjdata(json::from_bjdata(v_i), true) == v_i);
    CHECK(json::to_bjdata(json::from_bjdata(v_U), true) == v_U);
    CHECK(json::to_bjdata(json::from_bjdata(v_I), true) == v_I);
    CHECK(json::to_bjdata(json::from_bjdata(v_u), true) == v_u);
    CHECK(json::to_bjdata(json::from_bjdata(v_l), true) == v_l);
    CHECK(json::to_bjdata(json::from_bjdata(v_m), true) == v_m);
    CHECK(json::to_bjdata(json::from_bjdata(v_L), true) == v_L);
    CHECK(json::to_bjdata(json::from_bjdata(v_M), true) == v_M);
    CHECK(json::to_bjdata(json::from_bjdata(v_D), true) == v_D);
    CHECK(json::to_bjdata(json::from_bjdata(v_S), true) == v_S);
    CHECK(json::to_bjdata(json::from_bjdata(v_C), true) == v_S); // char is serialized to string
}

TEST(BJDataParsingValuesArrayTest, OptimizedVersionTypeAndLength)
{
    // create vector with two elements of the same type
    std::vector<uint8_t> v_i = {'[', '$', 'i', '#', 'i', 2, 0x7F, 0x7F};
    std::vector<uint8_t> v_U = {'[', '$', 'U', '#', 'i', 2, 0xFF, 0xFF};
    std::vector<uint8_t> v_I = {'[', '$', 'I', '#', 'i', 2, 0xFF, 0x7F, 0xFF, 0x7F};
    std::vector<uint8_t> v_u = {'[', '$', 'u', '#', 'i', 2, 0x0F, 0xA7, 0x0F, 0xA7};
    std::vector<uint8_t> v_l = {'[', '$', 'l', '#', 'i', 2, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x7F};
    std::vector<uint8_t> v_m = {'[', '$', 'm', '#', 'i', 2, 0xFF, 0xC9, 0x9A, 0xBB, 0xFF, 0xC9, 0x9A, 0xBB};
    std::vector<uint8_t> v_L = {'[', '$', 'L', '#', 'i', 2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F};
    std::vector<uint8_t> v_M = {'[', '$', 'M', '#', 'i', 2, 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D, 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D};
    std::vector<uint8_t> v_D = {'[', '$', 'D', '#', 'i', 2, 0x4a, 0xd8, 0x12, 0x4d, 0xfb, 0x21, 0x09, 0x40, 0x4a, 0xd8, 0x12, 0x4d, 0xfb, 0x21, 0x09, 0x40};
    std::vector<uint8_t> v_S = {'[', '#', 'i', 2, 'S', 'i', 1, 'a', 'S', 'i', 1, 'a'};
    std::vector<uint8_t> v_C = {'[', '$', 'C', '#', 'i', 2, 'a', 'a'};

    // check if vector is parsed correctly
    CHECK(json::from_bjdata(v_i) == json({127, 127}));
    CHECK(json::from_bjdata(v_U) == json({255, 255}));
    CHECK(json::from_bjdata(v_I) == json({32767, 32767}));
    CHECK(json::from_bjdata(v_u) == json({42767, 42767}));
    CHECK(json::from_bjdata(v_l) == json({2147483647, 2147483647}));
    CHECK(json::from_bjdata(v_m) == json({3147483647, 3147483647}));
    CHECK(json::from_bjdata(v_L) == json({9223372036854775807, 9223372036854775807}));
    CHECK(json::from_bjdata(v_M) == json({10223372036854775807ull, 10223372036854775807ull}));
    CHECK(json::from_bjdata(v_D) == json({3.1415926, 3.1415926}));
    CHECK(json::from_bjdata(v_S) == json({"a", "a"}));
    CHECK(json::from_bjdata(v_C) == json({"a", "a"}));

    // roundtrip: output should be optimized
    std::vector<uint8_t> v_empty = {'[', '#', 'i', 0};
    CHECK(json::to_bjdata(json::from_bjdata(v_i), true, true) == v_i);
    CHECK(json::to_bjdata(json::from_bjdata(v_U), true, true) == v_U);
    CHECK(json::to_bjdata(json::from_bjdata(v_I), true, true) == v_I);
    CHECK(json::to_bjdata(json::from_bjdata(v_u), true, true) == v_u);
    CHECK(json::to_bjdata(json::from_bjdata(v_l), true, true) == v_l);
    CHECK(json::to_bjdata(json::from_bjdata(v_m), true, true) == v_m);
    CHECK(json::to_bjdata(json::from_bjdata(v_L), true, true) == v_L);
    CHECK(json::to_bjdata(json::from_bjdata(v_M), true, true) == v_M);
    CHECK(json::to_bjdata(json::from_bjdata(v_D), true, true) == v_D);
    CHECK(json::to_bjdata(json::from_bjdata(v_S), true, true) == v_S);
    CHECK(json::to_bjdata(json::from_bjdata(v_C), true, true) == v_S); // char is serialized to string
}

TEST(BJDataParsingValuesArrayTest, OptimizedNdarrayTypeAndVectorSizeAsOptimized1DArray)
{
    // create vector with two elements of the same type
    std::vector<uint8_t> v_0 = {'[', '$', 'i', '#', '[', '$', 'i', '#', 'i', 1, 0};
    std::vector<uint8_t> v_1 = {'[', '$', 'i', '#', '[', '$', 'i', '#', 'i', 1, 2, 0x7F, 0x7F};
    std::vector<uint8_t> v_i = {'[', '$', 'i', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 0x7F, 0x7F};
    std::vector<uint8_t> v_U = {'[', '$', 'U', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 0xFF, 0xFF};
    std::vector<uint8_t> v_I = {'[', '$', 'I', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 0xFF, 0x7F, 0xFF, 0x7F};
    std::vector<uint8_t> v_u = {'[', '$', 'u', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 0x0F, 0xA7, 0x0F, 0xA7};
    std::vector<uint8_t> v_l = {'[', '$', 'l', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x7F};
    std::vector<uint8_t> v_m = {'[', '$', 'm', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 0xFF, 0xC9, 0x9A, 0xBB, 0xFF, 0xC9, 0x9A, 0xBB};
    std::vector<uint8_t> v_L = {'[', '$', 'L', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F};
    std::vector<uint8_t> v_M = {'[', '$', 'M', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D, 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D};
    std::vector<uint8_t> v_D = {'[', '$', 'D', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 0x4a, 0xd8, 0x12, 0x4d, 0xfb, 0x21, 0x09, 0x40, 0x4a, 0xd8, 0x12, 0x4d, 0xfb, 0x21, 0x09, 0x40};
    std::vector<uint8_t> v_S = {'[', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 'S', 'i', 1, 'a', 'S', 'i', 1, 'a'};
    std::vector<uint8_t> v_C = {'[', '$', 'C', '#', '[', '$', 'i', '#', 'i', 2, 1, 2, 'a', 'a'};

    // check if vector is parsed correctly
    CHECK(json::from_bjdata(v_0) == json::array());
    CHECK(json::from_bjdata(v_1) == json({127, 127}));
    CHECK(json::from_bjdata(v_i) == json({127, 127}));
    CHECK(json::from_bjdata(v_U) == json({255, 255}));
    CHECK(json::from_bjdata(v_I) == json({32767, 32767}));
    CHECK(json::from_bjdata(v_u) == json({42767, 42767}));
    CHECK(json::from_bjdata(v_l) == json({2147483647, 2147483647}));
    CHECK(json::from_bjdata(v_m) == json({3147483647, 3147483647}));
    CHECK(json::from_bjdata(v_L) == json({9223372036854775807, 9223372036854775807}));
    CHECK(json::from_bjdata(v_M) == json({10223372036854775807ull, 10223372036854775807ull}));
    CHECK(json::from_bjdata(v_D) == json({3.1415926, 3.1415926}));
    CHECK(json::from_bjdata(v_S) == json({"a", "a"}));
    CHECK(json::from_bjdata(v_C) == json({"a", "a"}));
}

TEST(BJDataParsingValuesArrayTest, OptimizedNdarrayTypeAndVectorSizeNdarrayWithJDataAnnotations)
{
    // create vector with 0, 1, 2 elements of the same type
    std::vector<uint8_t> v_e = {'[', '$', 'U', '#', '[', '$', 'i', '#', 'i', 2, 2, 1, 0xFE, 0xFF};
    std::vector<uint8_t> v_U = {'[', '$', 'U', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    std::vector<uint8_t> v_i = {'[', '$', 'i', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    std::vector<uint8_t> v_u = {'[', '$', 'u', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00};
    std::vector<uint8_t> v_I = {'[', '$', 'I', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 0x01, 0x00, 0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00};
    std::vector<uint8_t> v_m = {'[', '$', 'm', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00};
    std::vector<uint8_t> v_l = {'[', '$', 'l', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00};
    std::vector<uint8_t> v_M = {'[', '$', 'M', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    std::vector<uint8_t> v_L = {'[', '$', 'L', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    std::vector<uint8_t> v_d = {'[', '$', 'd', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0xA0, 0x40, 0x00, 0x00, 0xC0, 0x40};
    std::vector<uint8_t> v_D = {'[', '$', 'D', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x40};
    std::vector<uint8_t> v_C = {'[', '$', 'C', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 'a', 'b', 'c', 'd', 'e', 'f'};

    // check if vector is parsed correctly
    CHECK(json::from_bjdata(v_e) == json({{"_ArrayData_", {254, 255}}, {"_ArraySize_", {2, 1}}, {"_ArrayType_", "uint8"}}));
    CHECK(json::from_bjdata(v_U) == json({{"_ArrayData_", {1, 2, 3, 4, 5, 6}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "uint8"}}));
    CHECK(json::from_bjdata(v_i) == json({{"_ArrayData_", {1, 2, 3, 4, 5, 6}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "int8"}}));
    CHECK(json::from_bjdata(v_i) == json({{"_ArrayData_", {1, 2, 3, 4, 5, 6}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "int8"}}));
    CHECK(json::from_bjdata(v_u) == json({{"_ArrayData_", {1, 2, 3, 4, 5, 6}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "uint16"}}));
    CHECK(json::from_bjdata(v_I) == json({{"_ArrayData_", {1, 2, 3, 4, 5, 6}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "int16"}}));
    CHECK(json::from_bjdata(v_m) == json({{"_ArrayData_", {1, 2, 3, 4, 5, 6}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "uint32"}}));
    CHECK(json::from_bjdata(v_l) == json({{"_ArrayData_", {1, 2, 3, 4, 5, 6}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "int32"}}));
    CHECK(json::from_bjdata(v_M) == json({{"_ArrayData_", {1, 2, 3, 4, 5, 6}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "uint64"}}));
    CHECK(json::from_bjdata(v_L) == json({{"_ArrayData_", {1, 2, 3, 4, 5, 6}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "int64"}}));
    CHECK(json::from_bjdata(v_d) == json({{"_ArrayData_", {1.f, 2.f, 3.f, 4.f, 5.f, 6.f}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "single"}}));
    CHECK(json::from_bjdata(v_D) == json({{"_ArrayData_", {1., 2., 3., 4., 5., 6.}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "double"}}));
    CHECK(json::from_bjdata(v_C) == json({{"_ArrayData_", {'a', 'b', 'c', 'd', 'e', 'f'}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "char"}}));

    // roundtrip: output should be optimized
    CHECK(json::to_bjdata(json::from_bjdata(v_e), true, true) == v_e);
    CHECK(json::to_bjdata(json::from_bjdata(v_U), true, true) == v_U);
    CHECK(json::to_bjdata(json::from_bjdata(v_i), true, true) == v_i);
    CHECK(json::to_bjdata(json::from_bjdata(v_u), true, true) == v_u);
    CHECK(json::to_bjdata(json::from_bjdata(v_I), true, true) == v_I);
    CHECK(json::to_bjdata(json::from_bjdata(v_m), true, true) == v_m);
    CHECK(json::to_bjdata(json::from_bjdata(v_l), true, true) == v_l);
    CHECK(json::to_bjdata(json::from_bjdata(v_M), true, true) == v_M);
    CHECK(json::to_bjdata(json::from_bjdata(v_L), true, true) == v_L);
    CHECK(json::to_bjdata(json::from_bjdata(v_d), true, true) == v_d);
    CHECK(json::to_bjdata(json::from_bjdata(v_D), true, true) == v_D);
    CHECK(json::to_bjdata(json::from_bjdata(v_C), true, true) == v_C);
}

TEST(BJDataParsingValuesArrayTest, OptimizedNdarrayTypeAndVectorSizeAs1DArray)
{
    // create vector with two elements of the same type
    std::vector<uint8_t> v_0 = {'[', '$', 'i', '#', '[', ']'};
    std::vector<uint8_t> v_E = {'[', '$', 'i', '#', '[', 'i', 2, 'i', 0, ']'};
    std::vector<uint8_t> v_i = {'[', '$', 'i', '#', '[', 'i', 1, 'i', 2, ']', 0x7F, 0x7F};
    std::vector<uint8_t> v_U = {'[', '$', 'U', '#', '[', 'i', 1, 'i', 2, ']', 0xFF, 0xFF};
    std::vector<uint8_t> v_I = {'[', '$', 'I', '#', '[', 'i', 1, 'i', 2, ']', 0xFF, 0x7F, 0xFF, 0x7F};
    std::vector<uint8_t> v_u = {'[', '$', 'u', '#', '[', 'i', 1, 'i', 2, ']', 0x0F, 0xA7, 0x0F, 0xA7};
    std::vector<uint8_t> v_l = {'[', '$', 'l', '#', '[', 'i', 1, 'i', 2, ']', 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x7F};
    std::vector<uint8_t> v_m = {'[', '$', 'm', '#', '[', 'i', 1, 'i', 2, ']', 0xFF, 0xC9, 0x9A, 0xBB, 0xFF, 0xC9, 0x9A, 0xBB};
    std::vector<uint8_t> v_L = {'[', '$', 'L', '#', '[', 'i', 1, 'i', 2, ']', 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F};
    std::vector<uint8_t> v_M = {'[', '$', 'M', '#', '[', 'i', 1, 'i', 2, ']', 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D, 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D};
    std::vector<uint8_t> v_D = {'[', '$', 'D', '#', '[', 'i', 1, 'i', 2, ']', 0x4a, 0xd8, 0x12, 0x4d, 0xfb, 0x21, 0x09, 0x40, 0x4a, 0xd8, 0x12, 0x4d, 0xfb, 0x21, 0x09, 0x40};
    std::vector<uint8_t> v_S = {'[', '#', '[', 'i', 1, 'i', 2, ']', 'S', 'i', 1, 'a', 'S', 'i', 1, 'a'};
    std::vector<uint8_t> v_C = {'[', '$', 'C', '#', '[', 'i', 1, 'i', 2, ']', 'a', 'a'};
    std::vector<uint8_t> v_R = {'[', '#', '[', 'i', 2, ']', 'i', 6, 'U', 7};

    // check if vector is parsed correctly
    CHECK(json::from_bjdata(v_0) == json::array());
    CHECK(json::from_bjdata(v_E) == json::array());
    CHECK(json::from_bjdata(v_i) == json({127, 127}));
    CHECK(json::from_bjdata(v_U) == json({255, 255}));
    CHECK(json::from_bjdata(v_I) == json({32767, 32767}));
    CHECK(json::from_bjdata(v_u) == json({42767, 42767}));
    CHECK(json::from_bjdata(v_l) == json({2147483647, 2147483647}));
    CHECK(json::from_bjdata(v_m) == json({3147483647, 3147483647}));
    CHECK(json::from_bjdata(v_L) == json({9223372036854775807, 9223372036854775807}));
    CHECK(json::from_bjdata(v_M) == json({10223372036854775807ull, 10223372036854775807ull}));
    CHECK(json::from_bjdata(v_D) == json({3.1415926, 3.1415926}));
    CHECK(json::from_bjdata(v_S) == json({"a", "a"}));
    CHECK(json::from_bjdata(v_C) == json({"a", "a"}));
    CHECK(json::from_bjdata(v_R) == json({6, 7}));
}

TEST(BJDataParsingValuesArrayTest, OptimizedNdarrayTypeAndVectorSizeAsSizeOptimizedArray)
{
    // create vector with two elements of the same type
    std::vector<uint8_t> v_i = {'[', '$', 'i', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 0x7F, 0x7F};
    std::vector<uint8_t> v_U = {'[', '$', 'U', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 0xFF, 0xFF};
    std::vector<uint8_t> v_I = {'[', '$', 'I', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 0xFF, 0x7F, 0xFF, 0x7F};
    std::vector<uint8_t> v_u = {'[', '$', 'u', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 0x0F, 0xA7, 0x0F, 0xA7};
    std::vector<uint8_t> v_l = {'[', '$', 'l', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x7F};
    std::vector<uint8_t> v_m = {'[', '$', 'm', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 0xFF, 0xC9, 0x9A, 0xBB, 0xFF, 0xC9, 0x9A, 0xBB};
    std::vector<uint8_t> v_L = {'[', '$', 'L', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F};
    std::vector<uint8_t> v_M = {'[', '$', 'M', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D, 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D};
    std::vector<uint8_t> v_D = {'[', '$', 'D', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 0x4a, 0xd8, 0x12, 0x4d, 0xfb, 0x21, 0x09, 0x40, 0x4a, 0xd8, 0x12, 0x4d, 0xfb, 0x21, 0x09, 0x40};
    std::vector<uint8_t> v_S = {'[', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 'S', 'i', 1, 'a', 'S', 'i', 1, 'a'};
    std::vector<uint8_t> v_C = {'[', '$', 'C', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2, 'a', 'a'};

    // check if vector is parsed correctly
    CHECK(json::from_bjdata(v_i) == json({127, 127}));
    CHECK(json::from_bjdata(v_U) == json({255, 255}));
    CHECK(json::from_bjdata(v_I) == json({32767, 32767}));
    CHECK(json::from_bjdata(v_u) == json({42767, 42767}));
    CHECK(json::from_bjdata(v_l) == json({2147483647, 2147483647}));
    CHECK(json::from_bjdata(v_m) == json({3147483647, 3147483647}));
    CHECK(json::from_bjdata(v_L) == json({9223372036854775807, 9223372036854775807}));
    CHECK(json::from_bjdata(v_M) == json({10223372036854775807ull, 10223372036854775807ull}));
    CHECK(json::from_bjdata(v_D) == json({3.1415926, 3.1415926}));
    CHECK(json::from_bjdata(v_S) == json({"a", "a"}));
    CHECK(json::from_bjdata(v_C) == json({"a", "a"}));
}

TEST(BJDataParsingValuesArrayTest, InvalidNdarrayAnnotationsRemainsAsObject)
{
    // check if invalid ND array annotations stay as object
    json j_type = json({{"_ArrayData_", {1, 2, 3, 4, 5, 6}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "invalidtype"}});
    json j_size = json({{"_ArrayData_", {1, 2, 3, 4, 5}}, {"_ArraySize_", {2, 3}}, {"_ArrayType_", "uint8"}});

    // roundtrip: output should stay as object
    CHECK(json::from_bjdata(json::to_bjdata(j_type), true, true) == j_type);
    CHECK(json::from_bjdata(json::to_bjdata(j_size), true, true) == j_size);
}




TEST(BJDataParseErrorsTest, EmptyByteVector)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(std::vector<uint8_t>()),
                         "[json.exception.parse_error.110] parse error at byte 1: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
}




TEST(BJDataParseErrorsCharTest, EofAfterCByte)
{
    std::vector<uint8_t> v = {'C'};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v), "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing BJData char: unexpected end of input", json::parse_error&);
}

TEST(BJDataParseErrorsCharTest, ByteOutOfRange)
{
    std::vector<uint8_t> v = {'C', 130};
    json _;
    CHECK_THROWS_WITH(_ = json::from_bjdata(v), "[json.exception.parse_error.113] parse error at byte 2: syntax error while parsing BJData char: byte after 'C' must be in range 0x00..0x7F; last byte: 0x82");
}




TEST(BJDataParseErrorsStringsTest, EofAfterSByte)
{
    std::vector<uint8_t> v = {'S'};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v), "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
}

TEST(BJDataParseErrorsStringsTest, InvalidByte)
{
    std::vector<uint8_t> v = {'S', '1', 'a'};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v), "[json.exception.parse_error.113] parse error at byte 2: syntax error while parsing BJData string: expected length type specification (U, i, u, I, m, l, M, L); last byte: 0x31", json::parse_error&);
}

TEST(BJDataParseErrorsStringsTest, ParseBjdataMarkersInUbjson)
{
    // create a single-character string for all number types
    std::vector<uint8_t> s_u = {'S', 'u', 1, 0, 'a'};
    std::vector<uint8_t> s_m = {'S', 'm', 1, 0, 0, 0, 'a'};
    std::vector<uint8_t> s_M = {'S', 'M', 1, 0, 0, 0, 0, 0, 0, 0, 'a'};

    json _;
    // check if string is parsed correctly to "a"
    CHECK_THROWS_WITH_AS(_ = json::from_ubjson(s_u), "[json.exception.parse_error.113] parse error at byte 2: syntax error while parsing UBJSON string: expected length type specification (U, i, I, l, L); last byte: 0x75", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_ubjson(s_m), "[json.exception.parse_error.113] parse error at byte 2: syntax error while parsing UBJSON string: expected length type specification (U, i, I, l, L); last byte: 0x6D", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_ubjson(s_M), "[json.exception.parse_error.113] parse error at byte 2: syntax error while parsing UBJSON string: expected length type specification (U, i, I, l, L); last byte: 0x4D", json::parse_error&);
}




TEST(BJDataParseErrorsArrayTest, OptimizedArrayNoSizeFollowingType)
{
    std::vector<uint8_t> v = {'[', '$', 'i', 2};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v), "[json.exception.parse_error.112] parse error at byte 4: syntax error while parsing BJData size: expected '#' after type information; last byte: 0x02", json::parse_error&);
}

TEST(BJDataParseErrorsArrayTest, OptimizedArrayNegativeSize)
{
    std::vector<uint8_t> v1 = {'[', '#', 'i', 0xF1};
    std::vector<uint8_t> v2 = {'[', '$', 'I', '#', 'i', 0xF2};
    std::vector<uint8_t> v3 = {'[', '$', 'I', '#', '[', 'i', 0xF4, 'i', 0x02, ']'};
    std::vector<uint8_t> v4 = {'[', '$', 0xF6, '#', 'i', 0xF7};
    std::vector<uint8_t> v5 = {'[', '$', 'I', '#', '[', 'i', 0xF5, 'i', 0xF1, ']'};
    std::vector<uint8_t> v6 = {'[', '#', '[', 'i', 0xF3, 'i', 0x02, ']'};

    std::vector<uint8_t> vI = {'[', '#', 'I', 0x00, 0xF1};
    std::vector<uint8_t> vl = {'[', '#', 'l', 0x00, 0x00, 0x00, 0xF2};
    std::vector<uint8_t> vL = {'[', '#', 'L', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3};
    std::vector<uint8_t> vM = {'[', '$', 'M', '#', '[', 'I', 0x00, 0x20, 'M', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xFF, ']'};
    std::vector<uint8_t> vMX = {'[', '$', 'U', '#', '[', 'M', 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 'U', 0x01, ']'};

    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v1), "[json.exception.parse_error.113] parse error at byte 4: syntax error while parsing BJData size: count in an optimized container must be positive", json::parse_error&);
    CHECK(json::from_bjdata(v1, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v2), "[json.exception.parse_error.113] parse error at byte 6: syntax error while parsing BJData size: count in an optimized container must be positive", json::parse_error&);
    CHECK(json::from_bjdata(v2, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v3), "[json.exception.parse_error.113] parse error at byte 7: syntax error while parsing BJData size: count in an optimized container must be positive", json::parse_error&);
    CHECK(json::from_bjdata(v3, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v4), "[json.exception.parse_error.113] parse error at byte 6: syntax error while parsing BJData size: count in an optimized container must be positive", json::parse_error&);
    CHECK(json::from_bjdata(v4, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v5), "[json.exception.parse_error.113] parse error at byte 7: syntax error while parsing BJData size: count in an optimized container must be positive", json::parse_error&);
    CHECK(json::from_bjdata(v5, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v6), "[json.exception.parse_error.113] parse error at byte 5: syntax error while parsing BJData size: count in an optimized container must be positive", json::parse_error&);
    CHECK(json::from_bjdata(v6, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vI), "[json.exception.parse_error.113] parse error at byte 5: syntax error while parsing BJData size: count in an optimized container must be positive", json::parse_error&);
    CHECK(json::from_bjdata(vI, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vl), "[json.exception.parse_error.113] parse error at byte 7: syntax error while parsing BJData size: count in an optimized container must be positive", json::parse_error&);
    CHECK(json::from_bjdata(vl, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vL), "[json.exception.parse_error.113] parse error at byte 11: syntax error while parsing BJData size: count in an optimized container must be positive", json::parse_error&);
    CHECK(json::from_bjdata(vL, true, false).is_discarded());

#if SIZE_MAX != 0xffffffff
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vM), "[json.exception.out_of_range.408] syntax error while parsing BJData size: excessive ndarray size caused overflow", json::out_of_range&);
#else
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vM), "[json.exception.out_of_range.408] syntax error while parsing BJData size: integer value overflow", json::out_of_range&);
#endif
    CHECK(json::from_bjdata(vM, true, false).is_discarded());

#if SIZE_MAX != 0xffffffff
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vMX), "[json.exception.out_of_range.408] syntax error while parsing BJData size: excessive ndarray size caused overflow", json::out_of_range&);
#else
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vMX), "[json.exception.out_of_range.408] syntax error while parsing BJData size: integer value overflow", json::out_of_range&);
#endif
    CHECK(json::from_bjdata(vMX, true, false).is_discarded());
}

TEST(BJDataParseErrorsArrayTest, OptimizedArrayIntegerValueOverflow)
{
    std::vector<uint8_t> vL = {'[', '#', 'L', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F};
    std::vector<uint8_t> vM = {'[', '$', 'M', '#', '[', 'I', 0x00, 0x20, 'M', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xFF, ']'};

    json _;
#if SIZE_MAX == 0xffffffff
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vL), "[json.exception.out_of_range.408] syntax error while parsing BJData size: integer value overflow", json::out_of_range&);
    CHECK(json::from_bjdata(vL, true, false).is_discarded());
#endif

#if SIZE_MAX == 0xffffffff
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vM), "[json.exception.out_of_range.408] syntax error while parsing BJData size: integer value overflow", json::out_of_range&);
    CHECK(json::from_bjdata(vM, true, false).is_discarded());
#endif
}

TEST(BJDataParseErrorsArrayTest, DoNotAcceptNTFZMarkersInNdarrayOptimizedTypeWithCount)
{
    json _;
    std::vector<uint8_t> v_N = {'[', '$', 'N', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2};
    std::vector<uint8_t> v_T = {'[', '$', 'T', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2};
    std::vector<uint8_t> v_F = {'[', '$', 'F', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2};
    std::vector<uint8_t> v_Z = {'[', '$', 'Z', '#', '[', '#', 'i', 2, 'i', 1, 'i', 2};

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v_N), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x4E is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v_N, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v_T), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x54 is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v_T, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v_F), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x46 is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v_F, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v_Z), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x5A is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v_Z, true, false).is_discarded());
}

TEST(BJDataParseErrorsArrayTest, DoNotAcceptNTFZMarkersInNdarrayOptimizedTypeWithoutCount)
{
    json _;
    std::vector<uint8_t> v_N = {'[', '$', 'N', '#', '[', 'i', 1, 'i', 2, ']'};
    std::vector<uint8_t> v_T = {'[', '$', 'T', '#', '[', 'i', 1, 'i', 2, ']'};
    std::vector<uint8_t> v_F = {'[', '$', 'F', '#', '[', 'i', 1, 'i', 2, ']'};
    std::vector<uint8_t> v_Z = {'[', '$', 'Z', '#', '[', 'i', 1, 'i', 2, ']'};

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v_N), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x4E is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v_N, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v_T), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x54 is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v_T, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v_F), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x46 is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v_F, true, false).is_discarded());

    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v_Z), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x5A is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v_Z, true, false).is_discarded());
}

TEST(BJDataParseErrorsTest, Strings)
{
    std::vector<uint8_t> vS = {'S'};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vS), "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vS, true, false).is_discarded());

    std::vector<uint8_t> v = {'S', 'i', '2', 'a'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v), "[json.exception.parse_error.110] parse error at byte 5: syntax error while parsing BJData string: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(v, true, false).is_discarded());

    std::vector<uint8_t> vC = {'C'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vC), "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing BJData char: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vC, true, false).is_discarded());
}

TEST(BJDataParseErrorsTest, Sizes)
{
    std::vector<uint8_t> vU = {'[', '#', 'U'};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vU), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vU, true, false).is_discarded());

    std::vector<uint8_t> vi = {'[', '#', 'i'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vi), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vi, true, false).is_discarded());

    std::vector<uint8_t> vI = {'[', '#', 'I'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vI), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vI, true, false).is_discarded());

    std::vector<uint8_t> vu = {'[', '#', 'u'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vu), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vu, true, false).is_discarded());

    std::vector<uint8_t> vl = {'[', '#', 'l'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vl), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vl, true, false).is_discarded());

    std::vector<uint8_t> vm = {'[', '#', 'm'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vm), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vm, true, false).is_discarded());

    std::vector<uint8_t> vL = {'[', '#', 'L'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vL), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vL, true, false).is_discarded());

    std::vector<uint8_t> vM = {'[', '#', 'M'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vM), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vM, true, false).is_discarded());

    std::vector<uint8_t> v0 = {'[', '#', 'T', ']'};
    CHECK_THROWS_WITH(_ = json::from_bjdata(v0), "[json.exception.parse_error.113] parse error at byte 3: syntax error while parsing BJData size: expected length type specification (U, i, u, I, m, l, M, L) after '#'; last byte: 0x54");
    CHECK(json::from_bjdata(v0, true, false).is_discarded());
}

TEST(BJDataParseErrorsTest, ParseBjdataMarkersAsArraySizeInUbjson)
{
    json _;
    std::vector<uint8_t> vu = {'[', '#', 'u'};
    CHECK_THROWS_WITH_AS(_ = json::from_ubjson(vu), "[json.exception.parse_error.113] parse error at byte 3: syntax error while parsing UBJSON size: expected length type specification (U, i, I, l, L) after '#'; last byte: 0x75", json::parse_error&);
    CHECK(json::from_ubjson(vu, true, false).is_discarded());

    std::vector<uint8_t> vm = {'[', '#', 'm'};
    CHECK_THROWS_WITH_AS(_ = json::from_ubjson(vm), "[json.exception.parse_error.113] parse error at byte 3: syntax error while parsing UBJSON size: expected length type specification (U, i, I, l, L) after '#'; last byte: 0x6D", json::parse_error&);
    CHECK(json::from_ubjson(vm, true, false).is_discarded());

    std::vector<uint8_t> vM = {'[', '#', 'M'};
    CHECK_THROWS_WITH_AS(_ = json::from_ubjson(vM), "[json.exception.parse_error.113] parse error at byte 3: syntax error while parsing UBJSON size: expected length type specification (U, i, I, l, L) after '#'; last byte: 0x4D", json::parse_error&);
    CHECK(json::from_ubjson(vM, true, false).is_discarded());

    std::vector<uint8_t> v0 = {'[', '#', '['};
    CHECK_THROWS_WITH_AS(_ = json::from_ubjson(v0), "[json.exception.parse_error.113] parse error at byte 3: syntax error while parsing UBJSON size: expected length type specification (U, i, I, l, L) after '#'; last byte: 0x5B", json::parse_error&);
    CHECK(json::from_ubjson(v0, true, false).is_discarded());
}

TEST(BJDataParseErrorsTest, Types)
{
    std::vector<uint8_t> v0 = {'[', '$'};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v0), "[json.exception.parse_error.110] parse error at byte 3: syntax error while parsing BJData type: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(v0, true, false).is_discarded());

    std::vector<uint8_t> vi = {'[', '$', '#'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vi), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vi, true, false).is_discarded());

    std::vector<uint8_t> vU = {'[', '$', 'U'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vU), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vU, true, false).is_discarded());

    std::vector<uint8_t> v1 = {'[', '$', '['};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v1), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x5B is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v1, true, false).is_discarded());
}

TEST(BJDataParseErrorsTest, Arrays)
{
    std::vector<uint8_t> vST = {'[', '$', 'i', '#', 'i', 2, 1};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vST), "[json.exception.parse_error.110] parse error at byte 8: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vST, true, false).is_discarded());

    std::vector<uint8_t> vS = {'[', '#', 'i', 2, 'i', 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vS), "[json.exception.parse_error.110] parse error at byte 7: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vS, true, false).is_discarded());

    std::vector<uint8_t> v = {'[', 'i', 2, 'i', 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v), "[json.exception.parse_error.110] parse error at byte 6: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(v, true, false).is_discarded());
}

TEST(BJDataParseErrorsTest, Ndarrays)
{
    std::vector<uint8_t> vST = {'[', '$', 'i', '#', '[', '$', 'i', '#'};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vST), "[json.exception.parse_error.113] parse error at byte 9: syntax error while parsing BJData size: expected length type specification (U, i, u, I, m, l, M, L) after '#'; last byte: 0xFF", json::parse_error&);
    CHECK(json::from_bjdata(vST, true, false).is_discarded());

    std::vector<uint8_t> v = {'[', '$', 'i', '#', '[', '$', 'i', '#', 'i', 2, 1, 2};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v), "[json.exception.parse_error.110] parse error at byte 13: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(v, true, false).is_discarded());

    std::vector<uint8_t> vS0 = {'[', '$', 'i', '#', '[', '$', 'i', '#', 'i', 2, 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vS0), "[json.exception.parse_error.110] parse error at byte 12: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vS0, true, false).is_discarded());

    std::vector<uint8_t> vS = {'[', '$', 'i', '#', '[', '#', 'i', 2, 1, 2, 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vS), "[json.exception.parse_error.113] parse error at byte 9: syntax error while parsing BJData size: expected length type specification (U, i, u, I, m, l, M, L) after '#'; last byte: 0x01", json::parse_error&);
    CHECK(json::from_bjdata(vS, true, false).is_discarded());

    std::vector<uint8_t> vT = {'[', '$', 'i', '#', '[', 'i', 2, 'i'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vT), "[json.exception.parse_error.110] parse error at byte 9: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vT, true, false).is_discarded());

    std::vector<uint8_t> vT0 = {'[', '$', 'i', '#', '[', 'i'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vT0), "[json.exception.parse_error.110] parse error at byte 7: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vT0, true, false).is_discarded());

    std::vector<uint8_t> vu = {'[', '$', 'i', '#', '[', '$', 'i', '#', 'u', 1, 0};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vu), "[json.exception.parse_error.110] parse error at byte 12: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vu, true, false).is_discarded());

    std::vector<uint8_t> vm = {'[', '$', 'i', '#', '[', '$', 'i', '#', 'm', 1, 0, 0, 0};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vm), "[json.exception.parse_error.110] parse error at byte 14: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vm, true, false).is_discarded());

    std::vector<uint8_t> vM = {'[', '$', 'i', '#', '[', '$', 'i', '#', 'M', 1, 0, 0, 0, 0, 0, 0, 0};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vM), "[json.exception.parse_error.110] parse error at byte 18: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vM, true, false).is_discarded());

    std::vector<uint8_t> vU = {'[', '$', 'U', '#', '[', '$', 'i', '#', 'i', 2, 2, 3, 1, 2, 3, 4, 5};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vU), "[json.exception.parse_error.110] parse error at byte 18: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vU, true, false).is_discarded());

    std::vector<uint8_t> vT1 = {'[', '$', 'T', '#', '[', '$', 'i', '#', 'i', 2, 2, 3};
    CHECK(json::from_bjdata(vT1, true, false).is_discarded());

    std::vector<uint8_t> vh = {'[', '$', 'h', '#', '[', '$', 'i', '#', 'i', 2, 2, 3};
    CHECK(json::from_bjdata(vh, true, false).is_discarded());

    std::vector<uint8_t> vR = {'[', '$', 'i', '#', '[', 'i', 1, '[', ']', ']', 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vR), "[json.exception.parse_error.113] parse error at byte 8: syntax error while parsing BJData size: ndarray dimentional vector is not allowed", json::parse_error&);
    CHECK(json::from_bjdata(vR, true, false).is_discarded());

    std::vector<uint8_t> vRo = {'[', '$', 'i', '#', '[', 'i', 0, '{', '}', ']', 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vRo), "[json.exception.parse_error.113] parse error at byte 8: syntax error while parsing BJData size: expected length type specification (U, i, u, I, m, l, M, L) after '#'; last byte: 0x7B", json::parse_error&);
    CHECK(json::from_bjdata(vRo, true, false).is_discarded());

    std::vector<uint8_t> vR1 = {'[', '$', 'i', '#', '[', '[', 'i', 1, ']', ']', 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vR1), "[json.exception.parse_error.113] parse error at byte 6: syntax error while parsing BJData size: ndarray dimentional vector is not allowed", json::parse_error&);
    CHECK(json::from_bjdata(vR1, true, false).is_discarded());

    std::vector<uint8_t> vR2 = {'[', '$', 'i', '#', '[', '#', '[', 'i', 1, ']', ']', 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vR2), "[json.exception.parse_error.113] parse error at byte 11: syntax error while parsing BJData size: expected length type specification (U, i, u, I, m, l, M, L) after '#'; last byte: 0x5D", json::parse_error&);
    CHECK(json::from_bjdata(vR2, true, false).is_discarded());

    std::vector<uint8_t> vR3 = {'[', '#', '[', 'i', '2', 'i', 2, ']'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vR3), "[json.exception.parse_error.112] parse error at byte 8: syntax error while parsing BJData size: ndarray requires both type and size", json::parse_error&);
    CHECK(json::from_bjdata(vR3, true, false).is_discarded());

    std::vector<uint8_t> vR4 = {'[', '$', 'i', '#', '[', '$', 'i', '#', '[', 'i', 1, ']', 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vR4), "[json.exception.parse_error.110] parse error at byte 14: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vR4, true, false).is_discarded());

    std::vector<uint8_t> vR5 = {'[', '$', 'i', '#', '[', '[', '[', ']', ']', ']'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vR5), "[json.exception.parse_error.113] parse error at byte 6: syntax error while parsing BJData size: ndarray dimentional vector is not allowed", json::parse_error&);
    CHECK(json::from_bjdata(vR5, true, false).is_discarded());

    std::vector<uint8_t> vR6 = {'[', '$', 'i', '#', '[', '$', 'i', '#', '[', 'i', '2', 'i', 2, ']'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vR6), "[json.exception.parse_error.112] parse error at byte 14: syntax error while parsing BJData size: ndarray can not be recursive", json::parse_error&);
    CHECK(json::from_bjdata(vR6, true, false).is_discarded());

    std::vector<uint8_t> vH = {'[', 'H', '[', '#', '[', '$', 'i', '#', '[', 'i', '2', 'i', 2, ']'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vH), "[json.exception.parse_error.113] parse error at byte 3: syntax error while parsing BJData size: ndarray dimentional vector is not allowed", json::parse_error&);
    CHECK(json::from_bjdata(vH, true, false).is_discarded());
}

TEST(BJDataParseErrorsTest, Objects)
{
    std::vector<uint8_t> vST = {'{', '$', 'i', '#', 'i', 2, 'i', 1, 'a', 1};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vST), "[json.exception.parse_error.110] parse error at byte 11: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vST, true, false).is_discarded());

    std::vector<uint8_t> vT = {'{', '$', 'i', 'i', 1, 'a', 1};
    CHECK_THROWS_WITH(_ = json::from_bjdata(vT), "[json.exception.parse_error.112] parse error at byte 4: syntax error while parsing BJData size: expected '#' after type information; last byte: 0x69");
    CHECK(json::from_bjdata(vT, true, false).is_discarded());

    std::vector<uint8_t> vS = {'{', '#', 'i', 2, 'i', 1, 'a', 'i', 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vS), "[json.exception.parse_error.110] parse error at byte 10: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vS, true, false).is_discarded());

    std::vector<uint8_t> v = {'{', 'i', 1, 'a', 'i', 1};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v), "[json.exception.parse_error.110] parse error at byte 7: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(v, true, false).is_discarded());

    std::vector<uint8_t> v2 = {'{', 'i', 1, 'a', 'i', 1, 'i'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v2), "[json.exception.parse_error.110] parse error at byte 8: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(v2, true, false).is_discarded());

    std::vector<uint8_t> v3 = {'{', 'i', 1, 'a'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v3), "[json.exception.parse_error.110] parse error at byte 5: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(v3, true, false).is_discarded());

    std::vector<uint8_t> vST1 = {'{', '$', 'd', '#', 'i', 2, 'i', 1, 'a'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vST1), "[json.exception.parse_error.110] parse error at byte 10: syntax error while parsing BJData number: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vST1, true, false).is_discarded());

    std::vector<uint8_t> vST2 = {'{', '#', 'i', 2, 'i', 1, 'a'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vST2), "[json.exception.parse_error.110] parse error at byte 8: syntax error while parsing BJData value: unexpected end of input", json::parse_error&);
    CHECK(json::from_bjdata(vST2, true, false).is_discarded());

    std::vector<uint8_t> vO = {'{', '#', '[', 'i', 2, 'i', 1, ']', 'i', 1, 'a', 'i', 1, 'i', 1, 'b', 'i', 2};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vO), "[json.exception.parse_error.112] parse error at byte 8: syntax error while parsing BJData size: ndarray requires both type and size", json::parse_error&);
    CHECK(json::from_bjdata(vO, true, false).is_discarded());

    std::vector<uint8_t> vO2 = {'{', '$', 'i', '#', '[', 'i', 2, 'i', 1, ']', 'i', 1, 'a', 1, 'i', 1, 'b', 2};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(vO2), "[json.exception.parse_error.112] parse error at byte 10: syntax error while parsing BJData object: BJData object does not support ND-array size in optimized format", json::parse_error&);
    CHECK(json::from_bjdata(vO2, true, false).is_discarded());
}







TEST(BJDataWritingOptimizedValuesIntegerTest, ArrayOfI)
{
    json j = {1, -1};
    std::vector<uint8_t> expected = {'[', '$', 'i', '#', 'i', 2, 1, 0xff};
    CHECK(json::to_bjdata(j, true, true) == expected);
}

TEST(BJDataWritingOptimizedValuesIntegerTest, ArrayOfU)
{
    json j = {200, 201};
    std::vector<uint8_t> expected = {'[', '$', 'U', '#', 'i', 2, 0xC8, 0xC9};
    CHECK(json::to_bjdata(j, true, true) == expected);
}

TEST(BJDataWritingOptimizedValuesIntegerTest, ArrayOfI2)
{
    json j = {30000, -30000};
    std::vector<uint8_t> expected = {'[', '$', 'I', '#', 'i', 2, 0x30, 0x75, 0xd0, 0x8a};
    CHECK(json::to_bjdata(j, true, true) == expected);
}

TEST(BJDataWritingOptimizedValuesIntegerTest, ArrayOfU2)
{
    json j = {50000, 50001};
    std::vector<uint8_t> expected = {'[', '$', 'u', '#', 'i', 2, 0x50, 0xC3, 0x51, 0xC3};
    CHECK(json::to_bjdata(j, true, true) == expected);
}

TEST(BJDataWritingOptimizedValuesIntegerTest, ArrayOfL)
{
    json j = {70000, -70000};
    std::vector<uint8_t> expected = {'[', '$', 'l', '#', 'i', 2, 0x70, 0x11, 0x01, 0x00, 0x90, 0xEE, 0xFE, 0xFF};
    CHECK(json::to_bjdata(j, true, true) == expected);
}

TEST(BJDataWritingOptimizedValuesIntegerTest, ArrayOfM)
{
    json j = {3147483647, 3147483648};
    std::vector<uint8_t> expected = {'[', '$', 'm', '#', 'i', 2, 0xFF, 0xC9, 0x9A, 0xBB, 0x00, 0xCA, 0x9A, 0xBB};
    CHECK(json::to_bjdata(j, true, true) == expected);
}

TEST(BJDataWritingOptimizedValuesIntegerTest, ArrayOfL2)
{
    json j = {5000000000, -5000000000};
    std::vector<uint8_t> expected = {'[', '$', 'L', '#', 'i', 2, 0x00, 0xF2, 0x05, 0x2A, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0E, 0xFA, 0xD5, 0xFE, 0xFF, 0xFF, 0xFF};
    CHECK(json::to_bjdata(j, true, true) == expected);
}




TEST(BJDataWritingOptimizedValuesUnsignedIntegerTest, ArrayOfI)
{
    json j = {1u, 2u};
    std::vector<uint8_t> expected = {'[', '$', 'i', '#', 'i', 2, 1, 2};
    std::vector<uint8_t> expected_size = {'[', '#', 'i', 2, 'i', 1, 'i', 2};
    CHECK(json::to_bjdata(j, true, true) == expected);
    CHECK(json::to_bjdata(j, true) == expected_size);
}

TEST(BJDataWritingOptimizedValuesUnsignedIntegerTest, ArrayOfU)
{
    json j = {200u, 201u};
    std::vector<uint8_t> expected = {'[', '$', 'U', '#', 'i', 2, 0xC8, 0xC9};
    std::vector<uint8_t> expected_size = {'[', '#', 'i', 2, 'U', 0xC8, 'U', 0xC9};
    CHECK(json::to_bjdata(j, true, true) == expected);
    CHECK(json::to_bjdata(j, true) == expected_size);
}

TEST(BJDataWritingOptimizedValuesUnsignedIntegerTest, ArrayOfI2)
{
    json j = {30000u, 30001u};
    std::vector<uint8_t> expected = {'[', '$', 'I', '#', 'i', 2, 0x30, 0x75, 0x31, 0x75};
    std::vector<uint8_t> expected_size = {'[', '#', 'i', 2, 'I', 0x30, 0x75, 'I', 0x31, 0x75};
    CHECK(json::to_bjdata(j, true, true) == expected);
    CHECK(json::to_bjdata(j, true) == expected_size);
}

TEST(BJDataWritingOptimizedValuesUnsignedIntegerTest, ArrayOfU2)
{
    json j = {50000u, 50001u};
    std::vector<uint8_t> expected = {'[', '$', 'u', '#', 'i', 2, 0x50, 0xC3, 0x51, 0xC3};
    std::vector<uint8_t> expected_size = {'[', '#', 'i', 2, 'u', 0x50, 0xC3, 'u', 0x51, 0xC3};
    CHECK(json::to_bjdata(j, true, true) == expected);
    CHECK(json::to_bjdata(j, true) == expected_size);
}

TEST(BJDataWritingOptimizedValuesUnsignedIntegerTest, ArrayOfL)
{
    json j = {70000u, 70001u};
    std::vector<uint8_t> expected = {'[', '$', 'l', '#', 'i', 2, 0x70, 0x11, 0x01, 0x00, 0x71, 0x11, 0x01, 0x00};
    std::vector<uint8_t> expected_size = {'[', '#', 'i', 2, 'l', 0x70, 0x11, 0x01, 0x00, 'l', 0x71, 0x11, 0x01, 0x00};
    CHECK(json::to_bjdata(j, true, true) == expected);
    CHECK(json::to_bjdata(j, true) == expected_size);
}

TEST(BJDataWritingOptimizedValuesUnsignedIntegerTest, ArrayOfM)
{
    json j = {3147483647u, 3147483648u};
    std::vector<uint8_t> expected = {'[', '$', 'm', '#', 'i', 2, 0xFF, 0xC9, 0x9A, 0xBB, 0x00, 0xCA, 0x9A, 0xBB};
    std::vector<uint8_t> expected_size = {'[', '#', 'i', 2, 'm', 0xFF, 0xC9, 0x9A, 0xBB, 'm', 0x00, 0xCA, 0x9A, 0xBB};
    CHECK(json::to_bjdata(j, true, true) == expected);
    CHECK(json::to_bjdata(j, true) == expected_size);
}

TEST(BJDataWritingOptimizedValuesUnsignedIntegerTest, ArrayOfL2)
{
    json j = {5000000000u, 5000000001u};
    std::vector<uint8_t> expected = {'[', '$', 'L', '#', 'i', 2, 0x00, 0xF2, 0x05, 0x2A, 0x01, 0x00, 0x00, 0x00, 0x01, 0xF2, 0x05, 0x2A, 0x01, 0x00, 0x00, 0x00};
    std::vector<uint8_t> expected_size = {'[', '#', 'i', 2, 'L', 0x00, 0xF2, 0x05, 0x2A, 0x01, 0x00, 0x00, 0x00, 'L', 0x01, 0xF2, 0x05, 0x2A, 0x01, 0x00, 0x00, 0x00};
    CHECK(json::to_bjdata(j, true, true) == expected);
    CHECK(json::to_bjdata(j, true) == expected_size);
}

TEST(BJDataWritingOptimizedValuesUnsignedIntegerTest, ArrayOfM2)
{
    json j = {10223372036854775807ull, 10223372036854775808ull};
    std::vector<uint8_t> expected = {'[', '$', 'M', '#', 'i', 2, 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D, 0x00, 0x00, 0x64, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D};
    std::vector<uint8_t> expected_size = {'[', '#', 'i', 2, 'M', 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D, 'M', 0x00, 0x00, 0x64, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D};
    CHECK(json::to_bjdata(j, true, true) == expected);
    CHECK(json::to_bjdata(j, true) == expected_size);
}




TEST(BJDataJSONSpecificationExamples1Test, NullValue)
{
    json j = {{"passcode", nullptr}};
    std::vector<uint8_t> v = {'{', 'i', 8, 'p', 'a', 's', 's', 'c', 'o', 'd', 'e', 'Z', '}'};
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1Test, NoOpValue)
{
    json j = {"foo", "bar", "baz"};
    std::vector<uint8_t> v = {'[', 'S', 'i', 3, 'f', 'o', 'o',
                              'S', 'i', 3, 'b', 'a', 'r',
                              'S', 'i', 3, 'b', 'a', 'z', ']'
                             };
    std::vector<uint8_t> v2 = {'[', 'S', 'i', 3, 'f', 'o', 'o', 'N',
                               'S', 'i', 3, 'b', 'a', 'r', 'N', 'N', 'N',
                               'S', 'i', 3, 'b', 'a', 'z', 'N', 'N', ']'
                              };
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
    CHECK(json::from_bjdata(v2) == j);
}

TEST(BJDataJSONSpecificationExamples1Test, BooleanTypes)
{
    json j = {{"authorized", true}, {"verified", false}};
    std::vector<uint8_t> v = {'{', 'i', 10, 'a', 'u', 't', 'h', 'o', 'r', 'i', 'z', 'e', 'd', 'T',
                              'i', 8, 'v', 'e', 'r', 'i', 'f', 'i', 'e', 'd', 'F', '}'
                             };
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1Test, NumericTypes)
{
    json j =
    {
        {"int8", 16},
        {"uint8", 255},
        {"int16", 32767},
        {"uint16", 42767},
        {"int32", 2147483647},
        {"uint32", 3147483647},
        {"int64", 9223372036854775807},
        {"uint64", 10223372036854775807ull},
        {"float64", 113243.7863123}
    };
    std::vector<uint8_t> v = {'{',
                              'i', 7, 'f', 'l', 'o', 'a', 't', '6', '4', 'D', 0xcf, 0x34, 0xbc, 0x94, 0xbc, 0xa5, 0xfb, 0x40,
                              'i', 5, 'i', 'n', 't', '1', '6', 'I', 0xff, 0x7f,
                              'i', 5, 'i', 'n', 't', '3', '2', 'l', 0xff, 0xff, 0xff, 0x7f,
                              'i', 5, 'i', 'n', 't', '6', '4', 'L', 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f,
                              'i', 4, 'i', 'n', 't', '8', 'i', 16,
                              'i', 6, 'u', 'i', 'n', 't', '1', '6', 'u', 0x0F, 0xA7,
                              'i', 6, 'u', 'i', 'n', 't', '3', '2', 'm', 0xFF, 0xC9, 0x9A, 0xBB,
                              'i', 6, 'u', 'i', 'n', 't', '6', '4', 'M', 0xFF, 0xFF, 0x63, 0xA7, 0xB3, 0xB6, 0xE0, 0x8D,
                              'i', 5, 'u', 'i', 'n', 't', '8', 'U', 0xff,
                              '}'
                             };
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1Test, CharType)
{
    json j = {{"rolecode", "a"}, {"delim", ";"}};
    std::vector<uint8_t> v = {'{', 'i', 5, 'd', 'e', 'l', 'i', 'm', 'C', ';', 'i', 8, 'r', 'o', 'l', 'e', 'c', 'o', 'd', 'e', 'C', 'a', '}'};
    //CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}




TEST(BJDataJSONSpecificationExamples1StringTypeTest, English)
{
    json j = "hello";
    std::vector<uint8_t> v = {'S', 'i', 5, 'h', 'e', 'l', 'l', 'o'};
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1StringTypeTest, Russian)
{
    json j = "привет";
    std::vector<uint8_t> v = {'S', 'i', 12, 0xD0, 0xBF, 0xD1, 0x80, 0xD0, 0xB8, 0xD0, 0xB2, 0xD0, 0xB5, 0xD1, 0x82};
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1StringTypeTest, Russian2)
{
    json j = "مرحبا";
    std::vector<uint8_t> v = {'S', 'i', 10, 0xD9, 0x85, 0xD8, 0xB1, 0xD8, 0xAD, 0xD8, 0xA8, 0xD8, 0xA7};
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}




TEST(BJDataJSONSpecificationExamples1ArrayTypeTest, SizeFalseTypeFalse)
{
    // note the float has been replaced by a double
    json j = {nullptr, true, false, 4782345193, 153.132, "ham"};
    std::vector<uint8_t> v = {'[', 'Z', 'T', 'F', 'L', 0xE9, 0xCB, 0x0C, 0x1D, 0x01, 0x00, 0x00, 0x00, 'D', 0x4e, 0x62, 0x10, 0x58, 0x39, 0x24, 0x63, 0x40, 'S', 'i', 3, 'h', 'a', 'm', ']'};
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1ArrayTypeTest, SizeTrueTypeFalse)
{
    // note the float has been replaced by a double
    json j = {nullptr, true, false, 4782345193, 153.132, "ham"};
    std::vector<uint8_t> v = {'[', '#', 'i', 6, 'Z', 'T', 'F', 'L', 0xE9, 0xCB, 0x0C, 0x1D, 0x01, 0x00, 0x00, 0x00, 'D', 0x4e, 0x62, 0x10, 0x58, 0x39, 0x24, 0x63, 0x40, 'S', 'i', 3, 'h', 'a', 'm'};
    CHECK(json::to_bjdata(j, true) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1ArrayTypeTest, SizeTrueTypeTrue)
{
    // note the float has been replaced by a double
    json j = {nullptr, true, false, 4782345193, 153.132, "ham"};
    std::vector<uint8_t> v = {'[', '#', 'i', 6, 'Z', 'T', 'F', 'L', 0xE9, 0xCB, 0x0C, 0x1D, 0x01, 0x00, 0x00, 0x00, 'D', 0x4e, 0x62, 0x10, 0x58, 0x39, 0x24, 0x63, 0x40, 'S', 'i', 3, 'h', 'a', 'm'};
    CHECK(json::to_bjdata(j, true, true) == v);
    CHECK(json::from_bjdata(v) == j);
}




TEST(BJDataJSONSpecificationExamples1ObjectTypeTest, SizeFalseTypeFalse)
{
    json j =
    {
        {
            "post", {
                {"id", 1137},
                {"author", "rkalla"},
                {"timestamp", 1364482090592},
                {"body", "I totally agree!"}
            }
        }
    };
    std::vector<uint8_t> v = {'{', 'i', 4, 'p', 'o', 's', 't', '{',
                              'i', 6, 'a', 'u', 't', 'h', 'o', 'r', 'S', 'i', 6, 'r', 'k', 'a', 'l', 'l', 'a',
                              'i', 4, 'b', 'o', 'd', 'y', 'S', 'i', 16, 'I', ' ', 't', 'o', 't', 'a', 'l', 'l', 'y', ' ', 'a', 'g', 'r', 'e', 'e', '!',
                              'i', 2, 'i', 'd', 'I', 0x71, 0x04,
                              'i', 9, 't', 'i', 'm', 'e', 's', 't', 'a', 'm', 'p', 'L', 0x60, 0x66, 0x78, 0xB1, 0x3D, 0x01, 0x00, 0x00,
                              '}', '}'
                             };
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1ObjectTypeTest, SizeTrueTypeFalse)
{
    json j =
    {
        {
            "post", {
                {"id", 1137},
                {"author", "rkalla"},
                {"timestamp", 1364482090592},
                {"body", "I totally agree!"}
            }
        }
    };
    std::vector<uint8_t> v = {'{', '#', 'i', 1, 'i', 4, 'p', 'o', 's', 't', '{', '#', 'i', 4,
                              'i', 6, 'a', 'u', 't', 'h', 'o', 'r', 'S', 'i', 6, 'r', 'k', 'a', 'l', 'l', 'a',
                              'i', 4, 'b', 'o', 'd', 'y', 'S', 'i', 16, 'I', ' ', 't', 'o', 't', 'a', 'l', 'l', 'y', ' ', 'a', 'g', 'r', 'e', 'e', '!',
                              'i', 2, 'i', 'd', 'I', 0x71, 0x04,
                              'i', 9, 't', 'i', 'm', 'e', 's', 't', 'a', 'm', 'p', 'L', 0x60, 0x66, 0x78, 0xB1, 0x3D, 0x01, 0x00, 0x00,
                             };
    CHECK(json::to_bjdata(j, true) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1ObjectTypeTest, SizeTrueTypeTrue)
{
    json j =
    {
        {
            "post", {
                {"id", 1137},
                {"author", "rkalla"},
                {"timestamp", 1364482090592},
                {"body", "I totally agree!"}
            }
        }
    };
    std::vector<uint8_t> v = {'{', '#', 'i', 1, 'i', 4, 'p', 'o', 's', 't', '{', '#', 'i', 4,
                              'i', 6, 'a', 'u', 't', 'h', 'o', 'r', 'S', 'i', 6, 'r', 'k', 'a', 'l', 'l', 'a',
                              'i', 4, 'b', 'o', 'd', 'y', 'S', 'i', 16, 'I', ' ', 't', 'o', 't', 'a', 'l', 'l', 'y', ' ', 'a', 'g', 'r', 'e', 'e', '!',
                              'i', 2, 'i', 'd', 'I', 0x71, 0x04,
                              'i', 9, 't', 'i', 'm', 'e', 's', 't', 'a', 'm', 'p', 'L', 0x60, 0x66, 0x78, 0xB1, 0x3D, 0x01, 0x00, 0x00,
                             };
    CHECK(json::to_bjdata(j, true, true) == v);
    CHECK(json::from_bjdata(v) == j);
}







TEST(BJDataJSONSpecificationExamples1OptimizedFormatArrayExampleTest, NoOptimization)
{
    // note the floats have been replaced by doubles
    json j = {29.97, 31.13, 67.0, 2.113, 23.888};
    std::vector<uint8_t> v = {'[',
                              'D', 0xb8, 0x1e, 0x85, 0xeb, 0x51, 0xf8, 0x3d, 0x40,
                              'D', 0xe1, 0x7a, 0x14, 0xae, 0x47, 0x21, 0x3f, 0x40,
                              'D', 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x50, 0x40,
                              'D', 0x81, 0x95, 0x43, 0x8b, 0x6c, 0xe7, 0x00, 0x40,
                              'D', 0x17, 0xd9, 0xce, 0xf7, 0x53, 0xe3, 0x37, 0x40,
                              ']'
                             };
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1OptimizedFormatArrayExampleTest, OptimizedWithCount)
{
    // note the floats have been replaced by doubles
    json j = {29.97, 31.13, 67.0, 2.113, 23.888};
    std::vector<uint8_t> v = {'[', '#', 'i', 5,
                              'D', 0xb8, 0x1e, 0x85, 0xeb, 0x51, 0xf8, 0x3d, 0x40,
                              'D', 0xe1, 0x7a, 0x14, 0xae, 0x47, 0x21, 0x3f, 0x40,
                              'D', 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x50, 0x40,
                              'D', 0x81, 0x95, 0x43, 0x8b, 0x6c, 0xe7, 0x00, 0x40,
                              'D', 0x17, 0xd9, 0xce, 0xf7, 0x53, 0xe3, 0x37, 0x40,
                             };
    CHECK(json::to_bjdata(j, true) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1OptimizedFormatArrayExampleTest, OptimizedWithTypeCount)
{
    // note the floats have been replaced by doubles
    json j = {29.97, 31.13, 67.0, 2.113, 23.888};
    std::vector<uint8_t> v = {'[', '$', 'D', '#', 'i', 5,
                              0xb8, 0x1e, 0x85, 0xeb, 0x51, 0xf8, 0x3d, 0x40,
                              0xe1, 0x7a, 0x14, 0xae, 0x47, 0x21, 0x3f, 0x40,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x50, 0x40,
                              0x81, 0x95, 0x43, 0x8b, 0x6c, 0xe7, 0x00, 0x40,
                              0x17, 0xd9, 0xce, 0xf7, 0x53, 0xe3, 0x37, 0x40,
                             };
    CHECK(json::to_bjdata(j, true, true) == v);
    CHECK(json::from_bjdata(v) == j);
}




TEST(BJDataJSONSpecificationExamples1OptimizedFormatObjectExampleTest, NoOptimization)
{
    // note the floats have been replaced by doubles
    json j = { {"lat", 29.976}, {"long", 31.131}, {"alt", 67.0} };
    std::vector<uint8_t> v = {'{',
                              'i', 3, 'a', 'l', 't', 'D',      0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x50, 0x40,
                              'i', 3, 'l', 'a', 't', 'D',      0x60, 0xe5, 0xd0, 0x22, 0xdb, 0xf9, 0x3d, 0x40,
                              'i', 4, 'l', 'o', 'n', 'g', 'D', 0xa8, 0xc6, 0x4b, 0x37, 0x89, 0x21, 0x3f, 0x40,
                              '}'
                             };
    CHECK(json::to_bjdata(j) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1OptimizedFormatObjectExampleTest, OptimizedWithCount)
{
    // note the floats have been replaced by doubles
    json j = { {"lat", 29.976}, {"long", 31.131}, {"alt", 67.0} };
    std::vector<uint8_t> v = {'{', '#', 'i', 3,
                              'i', 3, 'a', 'l', 't', 'D',      0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x50, 0x40,
                              'i', 3, 'l', 'a', 't', 'D',      0x60, 0xe5, 0xd0, 0x22, 0xdb, 0xf9, 0x3d, 0x40,
                              'i', 4, 'l', 'o', 'n', 'g', 'D', 0xa8, 0xc6, 0x4b, 0x37, 0x89, 0x21, 0x3f, 0x40,
                             };
    CHECK(json::to_bjdata(j, true) == v);
    CHECK(json::from_bjdata(v) == j);
}

TEST(BJDataJSONSpecificationExamples1OptimizedFormatObjectExampleTest, OptimizedWithTypeCount)
{
    // note the floats have been replaced by doubles
    json j = { {"lat", 29.976}, {"long", 31.131}, {"alt", 67.0} };
    std::vector<uint8_t> v = {'{', '$', 'D', '#', 'i', 3,
                              'i', 3, 'a', 'l', 't',      0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x50, 0x40,
                              'i', 3, 'l', 'a', 't',      0x60, 0xe5, 0xd0, 0x22, 0xdb, 0xf9, 0x3d, 0x40,
                              'i', 4, 'l', 'o', 'n', 'g', 0xa8, 0xc6, 0x4b, 0x37, 0x89, 0x21, 0x3f, 0x40,
                             };
    CHECK(json::to_bjdata(j, true, true) == v);
    CHECK(json::from_bjdata(v) == j);
}




TEST(BJDataJSONSpecificationExamples1OptimizedFormatSpecialCasesNullNoOpAndBooleanTest, Array)
{
    json _;
    std::vector<uint8_t> v = {'[', '$', 'N', '#', 'I', 0x00, 0x02};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x4E is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v, true, false).is_discarded());
}

TEST(BJDataJSONSpecificationExamples1OptimizedFormatSpecialCasesNullNoOpAndBooleanTest, Object)
{
    json _;
    std::vector<uint8_t> v = {'{', '$', 'Z', '#', 'i', 3, 'i', 4, 'n', 'a', 'm', 'e', 'i', 8, 'p', 'a', 's', 's', 'w', 'o', 'r', 'd', 'i', 5, 'e', 'm', 'a', 'i', 'l'};
    CHECK_THROWS_WITH_AS(_ = json::from_bjdata(v), "[json.exception.parse_error.112] parse error at byte 3: syntax error while parsing BJData type: marker 0x5A is not a permitted optimized array type", json::parse_error&);
    CHECK(json::from_bjdata(v, true, false).is_discarded());
}

#if !defined(JSON_NOEXCEPTION)
TEST(Test, AllBJDataFirstBytes)
{
    // these bytes will fail immediately with exception parse_error.112
    std::set<uint8_t> supported =
    {
        'T', 'F', 'Z', 'U', 'i', 'I', 'l', 'L', 'd', 'D', 'C', 'S', '[', '{', 'N', 'H', 'u', 'm', 'M', 'h'
    };

    for (auto i = 0; i < 256; ++i)
    {
        const auto byte = static_cast<uint8_t>(i);
        CAPTURE(byte)

        try
        {
            auto res = json::from_bjdata(std::vector<uint8_t>(1, byte));
        }
        catch (const json::parse_error& e)
        {
            // check that parse_error.112 is only thrown if the
            // first byte is not in the supported set
            INFO_WITH_TEMP(e.what());
            if (supported.find(byte) == supported.end())
            {
                CHECK(e.id == 112);
            }
            else
            {
                CHECK(e.id != 112);
            }
        }
    }
}
#endif

