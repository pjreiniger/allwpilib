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
#include <sstream>
#include <iomanip>
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







TEST(MessagePackIndividualValuesTest, Discarded)
{
    // discarded values are not serialized
    json j = json::value_t::discarded;
    const auto result = json::to_msgpack(j);
    CHECK(result.empty());
}

TEST(MessagePackIndividualValuesTest, Null)
{
    json j = nullptr;
    std::vector<uint8_t> expected = {0xc0};
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}




TEST(MessagePackIndividualValuesBooleanTest, True)
{
    json j = true;
    std::vector<uint8_t> expected = {0xc3};
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesBooleanTest, False)
{
    json j = false;
    std::vector<uint8_t> expected = {0xc2};
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}







TEST(MessagePackIndividualValuesNumberSignedTest, 321NegativeFixnum)
{
    for (auto i = -32; i <= -1; ++i)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 1);

        // check individual bytes
        CHECK(static_cast<int8_t>(result[0]) == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberSignedTest, 0127PositiveFixnum)
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
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 1);

        // check individual bytes
        CHECK(result[0] == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberSignedTest, 128255Int8)
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
        expected.push_back(0xcc);
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 2);

        // check individual bytes
        CHECK(result[0] == 0xcc);
        auto restored = static_cast<uint8_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberSignedTest, 25665535Int16)
{
    for (size_t i = 256; i <= 65535; ++i)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = -1;
        j.get_ref<json::number_integer_t&>() = static_cast<json::number_integer_t>(i);

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(0xcd);
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>(i & 0xff));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 3);

        // check individual bytes
        CHECK(result[0] == 0xcd);
        auto restored = static_cast<uint16_t>(static_cast<uint8_t>(result[1]) * 256 + static_cast<uint8_t>(result[2]));
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberSignedTest, 655364294967295Int32)
{
    for (uint32_t i :
            {
                65536u, 77777u, 1048576u, 4294967295u
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
        expected.push_back(0xce);
        expected.push_back(static_cast<uint8_t>((i >> 24) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 16) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>(i & 0xff));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 5);

        // check individual bytes
        CHECK(result[0] == 0xce);
        uint32_t restored = (static_cast<uint32_t>(result[1]) << 030) +
                            (static_cast<uint32_t>(result[2]) << 020) +
                            (static_cast<uint32_t>(result[3]) << 010) +
                            static_cast<uint32_t>(result[4]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberSignedTest, 42949672969223372036854775807Int64)
{
    for (uint64_t i :
            {
                4294967296LU, 9223372036854775807LU
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
        expected.push_back(0xcf);
        expected.push_back(static_cast<uint8_t>((i >> 070) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 060) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 050) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 040) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 030) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 020) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 010) & 0xff));
        expected.push_back(static_cast<uint8_t>(i & 0xff));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 9);

        // check individual bytes
        CHECK(result[0] == 0xcf);
        uint64_t restored = (static_cast<uint64_t>(result[1]) << 070) +
                            (static_cast<uint64_t>(result[2]) << 060) +
                            (static_cast<uint64_t>(result[3]) << 050) +
                            (static_cast<uint64_t>(result[4]) << 040) +
                            (static_cast<uint64_t>(result[5]) << 030) +
                            (static_cast<uint64_t>(result[6]) << 020) +
                            (static_cast<uint64_t>(result[7]) << 010) +
                            static_cast<uint64_t>(result[8]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberSignedTest, 12833Int8)
{
    for (auto i = -128; i <= -33; ++i)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(0xd0);
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 2);

        // check individual bytes
        CHECK(result[0] == 0xd0);
        CHECK(static_cast<int8_t>(result[1]) == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberSignedTest, 9263Int16)
{
    json j = -9263;
    std::vector<uint8_t> expected = {0xd1, 0xdb, 0xd1};

    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    auto restored = static_cast<int16_t>((result[1] << 8) + result[2]);
    CHECK(restored == -9263);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesNumberSignedTest, 32768129Int16)
{
    for (int16_t i = -32768; i <= static_cast<std::int16_t>(-129); ++i)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(0xd1);
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>(i & 0xff));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 3);

        // check individual bytes
        CHECK(result[0] == 0xd1);
        auto restored = static_cast<int16_t>((result[1] << 8) + result[2]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberSignedTest, 327692147483648)
{
    std::vector<int32_t> numbers;
    numbers.push_back(-32769);
    numbers.push_back(-65536);
    numbers.push_back(-77777);
    numbers.push_back(-1048576);
    numbers.push_back(-2147483648LL);
    for (auto i : numbers)
    {
        CAPTURE(i)

        // create JSON value with integer number
        json j = i;

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(0xd2);
        expected.push_back(static_cast<uint8_t>((i >> 24) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 16) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>(i & 0xff));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 5);

        // check individual bytes
        CHECK(result[0] == 0xd2);
        uint32_t restored = (static_cast<uint32_t>(result[1]) << 030) +
                            (static_cast<uint32_t>(result[2]) << 020) +
                            (static_cast<uint32_t>(result[3]) << 010) +
                            static_cast<uint32_t>(result[4]);
        CHECK(static_cast<std::int32_t>(restored) == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberSignedTest, 92233720368547758082147483649Int64)
{
    std::vector<int64_t> numbers;
    numbers.push_back(INT64_MIN);
    numbers.push_back(-2147483649LL);
    for (auto i : numbers)
    {
        CAPTURE(i)

        // create JSON value with unsigned integer number
        json j = i;

        // check type
        CHECK(j.is_number_integer());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(0xd3);
        expected.push_back(static_cast<uint8_t>((i >> 070) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 060) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 050) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 040) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 030) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 020) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 010) & 0xff));
        expected.push_back(static_cast<uint8_t>(i & 0xff));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 9);

        // check individual bytes
        CHECK(result[0] == 0xd3);
        int64_t restored = (static_cast<int64_t>(result[1]) << 070) +
                           (static_cast<int64_t>(result[2]) << 060) +
                           (static_cast<int64_t>(result[3]) << 050) +
                           (static_cast<int64_t>(result[4]) << 040) +
                           (static_cast<int64_t>(result[5]) << 030) +
                           (static_cast<int64_t>(result[6]) << 020) +
                           (static_cast<int64_t>(result[7]) << 010) +
                           static_cast<int64_t>(result[8]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}




TEST(MessagePackIndividualValuesNumberUnsignedTest, 0127PositiveFixnum)
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
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 1);

        // check individual bytes
        CHECK(result[0] == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberUnsignedTest, 128255Uint8)
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
        expected.push_back(0xcc);
        expected.push_back(static_cast<uint8_t>(i));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 2);

        // check individual bytes
        CHECK(result[0] == 0xcc);
        auto restored = static_cast<uint8_t>(result[1]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberUnsignedTest, 25665535Uint16)
{
    for (size_t i = 256; i <= 65535; ++i)
    {
        CAPTURE(i)

        // create JSON value with unsigned integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(0xcd);
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>(i & 0xff));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 3);

        // check individual bytes
        CHECK(result[0] == 0xcd);
        auto restored = static_cast<uint16_t>(static_cast<uint8_t>(result[1]) * 256 + static_cast<uint8_t>(result[2]));
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberUnsignedTest, 655364294967295Uint32)
{
    for (uint32_t i :
            {
                65536u, 77777u, 1048576u, 4294967295u
            })
    {
        CAPTURE(i)

        // create JSON value with unsigned integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(0xce);
        expected.push_back(static_cast<uint8_t>((i >> 24) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 16) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 8) & 0xff));
        expected.push_back(static_cast<uint8_t>(i & 0xff));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 5);

        // check individual bytes
        CHECK(result[0] == 0xce);
        uint32_t restored = (static_cast<uint32_t>(result[1]) << 030) +
                            (static_cast<uint32_t>(result[2]) << 020) +
                            (static_cast<uint32_t>(result[3]) << 010) +
                            static_cast<uint32_t>(result[4]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesNumberUnsignedTest, 429496729618446744073709551615Uint64)
{
    for (uint64_t i :
            {
                4294967296LU, 18446744073709551615LU
            })
    {
        CAPTURE(i)

        // create JSON value with unsigned integer number
        json j = i;

        // check type
        CHECK(j.is_number_unsigned());

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(0xcf);
        expected.push_back(static_cast<uint8_t>((i >> 070) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 060) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 050) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 040) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 030) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 020) & 0xff));
        expected.push_back(static_cast<uint8_t>((i >> 010) & 0xff));
        expected.push_back(static_cast<uint8_t>(i & 0xff));

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == 9);

        // check individual bytes
        CHECK(result[0] == 0xcf);
        uint64_t restored = (static_cast<uint64_t>(result[1]) << 070) +
                            (static_cast<uint64_t>(result[2]) << 060) +
                            (static_cast<uint64_t>(result[3]) << 050) +
                            (static_cast<uint64_t>(result[4]) << 040) +
                            (static_cast<uint64_t>(result[5]) << 030) +
                            (static_cast<uint64_t>(result[6]) << 020) +
                            (static_cast<uint64_t>(result[7]) << 010) +
                            static_cast<uint64_t>(result[8]);
        CHECK(restored == i);

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}




TEST(MessagePackIndividualValuesNumberFloatTest, Pi)
{
    double v = 3.1415925;
    json j = v;
    std::vector<uint8_t> expected =
    {
        0xcb, 0x40, 0x09, 0x21, 0xfb, 0x3f, 0xa6, 0xde, 0xfc
    };
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result) == v);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesNumberFloatTest, 10)
{
    double v = 1.0;
    json j = v;
    std::vector<uint8_t> expected =
    {
        0xca, 0x3f, 0x80, 0x00, 0x00
    };
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result) == v);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesNumberFloatTest, 128128)
{
    double v = 128.1280059814453125;
    json j = v;
    std::vector<uint8_t> expected =
    {
        0xca, 0x43, 0x00, 0x20, 0xc5
    };
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result) == v);
    CHECK(json::from_msgpack(result, true, false) == j);
}




TEST(MessagePackIndividualValuesStringTest, N031)
{
    // explicitly enumerate the first byte for all 32 strings
    const std::vector<uint8_t> first_bytes =
    {
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
        0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1,
        0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba,
        0xbb, 0xbc, 0xbd, 0xbe, 0xbf
    };

    for (size_t N = 0; N < first_bytes.size(); ++N)
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::string(N, 'x');
        json j = s;

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(first_bytes[N]);
        for (size_t i = 0; i < N; ++i)
        {
            expected.push_back('x');
        }

        // check first byte
        CHECK((first_bytes[N] & 0x1f) == N);

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 1);
        // check that no null byte is appended
        if (N > 0)
        {
            CHECK(result.back() != '\x00');
        }

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesStringTest, N32255)
{
    for (size_t N = 32; N <= 255; ++N)
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::string(N, 'x');
        json j = s;

        // create expected byte vector
        std::vector<uint8_t> expected;
        expected.push_back(0xd9);
        expected.push_back(static_cast<uint8_t>(N));
        for (size_t i = 0; i < N; ++i)
        {
            expected.push_back('x');
        }

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 2);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesStringTest, N25665535)
{
    for (size_t N :
            {
                256u, 999u, 1025u, 3333u, 2048u, 65535u
            })
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::string(N, 'x');
        json j = s;

        // create expected byte vector (hack: create string first)
        std::vector<uint8_t> expected(N, 'x');
        // reverse order of commands, because we insert at begin()
        expected.insert(expected.begin(), static_cast<uint8_t>(N & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 8) & 0xff));
        expected.insert(expected.begin(), 0xda);

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 3);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesStringTest, N655364294967295)
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
        expected.insert(expected.begin(), static_cast<uint8_t>(N & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 8) & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 16) & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 24) & 0xff));
        expected.insert(expected.begin(), 0xdb);

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 5);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}




TEST(MessagePackIndividualValuesArrayTest, Empty)
{
    json j = json::array();
    std::vector<uint8_t> expected = {0x90};
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesArrayTest, Null)
{
    json j = {nullptr};
    std::vector<uint8_t> expected = {0x91, 0xc0};
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesArrayTest, Simple)
{
    json j = json::parse("[1,2,3,4,5]");
    std::vector<uint8_t> expected = {0x95, 0x01, 0x02, 0x03, 0x04, 0x05};
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesArrayTest, NestedEmpty)
{
    json j = json::parse("[[[[]]]]");
    std::vector<uint8_t> expected = {0x91, 0x91, 0x91, 0x90};
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesArrayTest, Array16)
{
    json j(16, nullptr);
    std::vector<uint8_t> expected(j.size() + 3, 0xc0); // all null
    expected[0] = 0xdc; // array 16
    expected[1] = 0x00; // size (0x0010), byte 0
    expected[2] = 0x10; // size (0x0010), byte 1
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesArrayTest, Array32)
{
    json j(65536, nullptr);
    std::vector<uint8_t> expected(j.size() + 5, 0xc0); // all null
    expected[0] = 0xdd; // array 32
    expected[1] = 0x00; // size (0x00100000), byte 0
    expected[2] = 0x01; // size (0x00100000), byte 1
    expected[3] = 0x00; // size (0x00100000), byte 2
    expected[4] = 0x00; // size (0x00100000), byte 3
    const auto result = json::to_msgpack(j);
    //CHECK(result == expected);

    CHECK(result.size() == expected.size());
    for (size_t i = 0; i < expected.size(); ++i)
    {
        CAPTURE(i)
        CHECK(result[i] == expected[i]);
    }

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}




TEST(MessagePackIndividualValuesObjectTest, Empty)
{
    json j = json::object();
    std::vector<uint8_t> expected = {0x80};
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesObjectTest, EmptyKey)
{
    json j = {{"", nullptr}};
    std::vector<uint8_t> expected = {0x81, 0xa0, 0xc0};
    const auto result = json::to_msgpack(j);
    CHECK(result == expected);

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesObjectTest, NestedEmpty)
{
               json j = json::parse(R"({"a": {"b": {"c": {}}}})");
               std::vector<uint8_t> expected =
               {
                   0x81, 0xa1, 0x61, 0x81, 0xa1, 0x62, 0x81, 0xa1, 0x63, 0x80
               };
               const auto result = json::to_msgpack(j);
               CHECK(result == expected);

               // roundtrip
               CHECK(json::from_msgpack(result) == j);
               CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesObjectTest, Map16)
{
    json j = R"({"00": null, "01": null, "02": null, "03": null,
                 "04": null, "05": null, "06": null, "07": null,
                 "08": null, "09": null, "10": null, "11": null,
                 "12": null, "13": null, "14": null, "15": null})"_json;

    const auto result = json::to_msgpack(j);

    // Checking against an expected vector byte by byte is
    // difficult, because no assumption on the order of key/value
    // pairs are made. We therefore only check the prefix (type and
    // size and the overall size. The rest is then handled in the
    // roundtrip check.
    CHECK(result.size() == 67); // 1 type, 2 size, 16*4 content
    CHECK(result[0] == 0xde); // map 16
    CHECK(result[1] == 0x00); // byte 0 of size (0x0010)
    CHECK(result[2] == 0x10); // byte 1 of size (0x0010)

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}

TEST(MessagePackIndividualValuesObjectTest, Map32)
{
    json j;
    for (auto i = 0; i < 65536; ++i)
    {
        // format i to a fixed width of 5
        // each entry will need 7 bytes: 6 for fixstr, 1 for null
        std::stringstream ss;
        ss << std::setw(5) << std::setfill('0') << i;
        j.emplace(ss.str(), nullptr);
    }

    const auto result = json::to_msgpack(j);

    // Checking against an expected vector byte by byte is
    // difficult, because no assumption on the order of key/value
    // pairs are made. We therefore only check the prefix (type and
    // size and the overall size. The rest is then handled in the
    // roundtrip check.
    CHECK(result.size() == 458757); // 1 type, 4 size, 65536*7 content
    CHECK(result[0] == 0xdf); // map 32
    CHECK(result[1] == 0x00); // byte 0 of size (0x00010000)
    CHECK(result[2] == 0x01); // byte 1 of size (0x00010000)
    CHECK(result[3] == 0x00); // byte 2 of size (0x00010000)
    CHECK(result[4] == 0x00); // byte 3 of size (0x00010000)

    // roundtrip
    CHECK(json::from_msgpack(result) == j);
    CHECK(json::from_msgpack(result, true, false) == j);
}




TEST(MessagePackIndividualValuesExtensionTest, N0255)
{
    for (size_t N = 0; N <= 0xFF; ++N)
    {
        CAPTURE(N)

        // create JSON value with byte array containing of N * 'x'
        const auto s = std::vector<uint8_t>(N, 'x');
        json j = json::binary(s);
        std::uint8_t subtype = 42;
        j.get_binary().set_subtype(subtype);

        // create expected byte vector
        std::vector<uint8_t> expected;
        switch (N)
        {
            case 1:
                expected.push_back(static_cast<std::uint8_t>(0xD4));
                break;
            case 2:
                expected.push_back(static_cast<std::uint8_t>(0xD5));
                break;
            case 4:
                expected.push_back(static_cast<std::uint8_t>(0xD6));
                break;
            case 8:
                expected.push_back(static_cast<std::uint8_t>(0xD7));
                break;
            case 16:
                expected.push_back(static_cast<std::uint8_t>(0xD8));
                break;
            default:
                expected.push_back(static_cast<std::uint8_t>(0xC7));
                expected.push_back(static_cast<std::uint8_t>(N));
                break;
        }
        expected.push_back(subtype);

        for (size_t i = 0; i < N; ++i)
        {
            expected.push_back(0x78);
        }

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        switch (N)
        {
            case 1:
            case 2:
            case 4:
            case 8:
            case 16:
                CHECK(result.size() == N + 2);
                break;
            default:
                CHECK(result.size() == N + 3);
                break;
        }

        // check that no null byte is appended
        if (N > 0)
        {
            CHECK(result.back() != '\x00');
        }

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesExtensionTest, N25665535)
{
    for (std::size_t N :
            {
                256u, 999u, 1025u, 3333u, 2048u, 65535u
            })
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::vector<uint8_t>(N, 'x');
        json j = json::binary(s);
        std::uint8_t subtype = 42;
        j.get_binary().set_subtype(subtype);

        // create expected byte vector (hack: create string first)
        std::vector<uint8_t> expected(N, 'x');
        // reverse order of commands, because we insert at begin()
        expected.insert(expected.begin(), subtype);
        expected.insert(expected.begin(), static_cast<uint8_t>(N & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 8) & 0xff));
        expected.insert(expected.begin(), 0xC8);

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 4);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesExtensionTest, N655364294967295)
{
    for (std::size_t N :
            {
                65536u, 77777u, 1048576u
            })
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::vector<uint8_t>(N, 'x');
        json j = json::binary(s);
        std::uint8_t subtype = 42;
        j.get_binary().set_subtype(subtype);

        // create expected byte vector (hack: create string first)
        std::vector<uint8_t> expected(N, 'x');
        // reverse order of commands, because we insert at begin()
        expected.insert(expected.begin(), subtype);
        expected.insert(expected.begin(), static_cast<uint8_t>(N & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 8) & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 16) & 0xff));
        expected.insert(expected.begin(), static_cast<uint8_t>((N >> 24) & 0xff));
        expected.insert(expected.begin(), 0xC9);

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 6);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}




TEST(MessagePackIndividualValuesBinaryTest, N0255)
{
    for (std::size_t N = 0; N <= 0xFF; ++N)
    {
        CAPTURE(N)

        // create JSON value with byte array containing of N * 'x'
        const auto s = std::vector<uint8_t>(N, 'x');
        json j = json::binary(s);

        // create expected byte vector
        std::vector<std::uint8_t> expected;
        expected.push_back(static_cast<std::uint8_t>(0xC4));
        expected.push_back(static_cast<std::uint8_t>(N));
        for (size_t i = 0; i < N; ++i)
        {
            expected.push_back(0x78);
        }

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 2);
        // check that no null byte is appended
        if (N > 0)
        {
            CHECK(result.back() != '\x00');
        }

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesBinaryTest, N25665535)
{
    for (std::size_t N :
            {
                256u, 999u, 1025u, 3333u, 2048u, 65535u
            })
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::vector<std::uint8_t>(N, 'x');
        json j = json::binary(s);

        // create expected byte vector (hack: create string first)
        std::vector<std::uint8_t> expected(N, 'x');
        // reverse order of commands, because we insert at begin()
        expected.insert(expected.begin(), static_cast<std::uint8_t>(N & 0xff));
        expected.insert(expected.begin(), static_cast<std::uint8_t>((N >> 8) & 0xff));
        expected.insert(expected.begin(), 0xC5);

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 3);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackIndividualValuesBinaryTest, N655364294967295)
{
    for (std::size_t N :
            {
                65536u, 77777u, 1048576u
            })
    {
        CAPTURE(N)

        // create JSON value with string containing of N * 'x'
        const auto s = std::vector<std::uint8_t>(N, 'x');
        json j = json::binary(s);

        // create expected byte vector (hack: create string first)
        std::vector<uint8_t> expected(N, 'x');
        // reverse order of commands, because we insert at begin()
        expected.insert(expected.begin(), static_cast<std::uint8_t>(N & 0xff));
        expected.insert(expected.begin(), static_cast<std::uint8_t>((N >> 8) & 0xff));
        expected.insert(expected.begin(), static_cast<std::uint8_t>((N >> 16) & 0xff));
        expected.insert(expected.begin(), static_cast<std::uint8_t>((N >> 24) & 0xff));
        expected.insert(expected.begin(), 0xC6);

        // compare result + size
        const auto result = json::to_msgpack(j);
        CHECK(result == expected);
        CHECK(result.size() == N + 5);
        // check that no null byte is appended
        CHECK(result.back() != '\x00');

        // roundtrip
        CHECK(json::from_msgpack(result) == j);
        CHECK(json::from_msgpack(result, true, false) == j);
    }
}

TEST(MessagePackTest, FromFloat32)
{
    auto given = std::vector<uint8_t>({0xca, 0x41, 0xc8, 0x00, 0x01});
    json j = json::from_msgpack(given);
    EXPECT_NEAR(j.get<double>(), 25.0000019073486, 0.001);
}




TEST(MessagePackErrorsTest, EmptyByteVector)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>()), "[json.exception.parse_error.110] parse error at byte 1: syntax error while parsing MessagePack value: unexpected end of input", json::parse_error&);
    CHECK(json::from_msgpack(std::vector<uint8_t>(), true, false).is_discarded());
}

TEST(MessagePackErrorsTest, TooShortByteVector)
{
    json _;

    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0x87})),
                         "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing MessagePack string: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcc})),
                         "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcd})),
                         "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcd, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 3: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xce})),
                         "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xce, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 3: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xce, 0x00, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xce, 0x00, 0x00, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 5: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcf})),
                         "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 3: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 5: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00, 0x00, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 6: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00, 0x00, 0x00, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 7: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 8: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})),
                         "[json.exception.parse_error.110] parse error at byte 9: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xa5, 0x68, 0x65})),
                         "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing MessagePack string: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0x92, 0x01})),
                         "[json.exception.parse_error.110] parse error at byte 3: syntax error while parsing MessagePack value: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0x81, 0xa1, 0x61})),
                         "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing MessagePack value: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xc4, 0x02})),
                         "[json.exception.parse_error.110] parse error at byte 3: syntax error while parsing MessagePack binary: unexpected end of input", json::parse_error&);

    CHECK(json::from_msgpack(std::vector<uint8_t>({0x87}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcc}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcd}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcd, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xce}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xce, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xce, 0x00, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xce, 0x00, 0x00, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcf}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00, 0x00, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00, 0x00, 0x00, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xa5, 0x68, 0x65}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0x92, 0x01}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0x81, 0xA1, 0x61}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xc4, 0x02}), true, false).is_discarded());
    CHECK(json::from_msgpack(std::vector<uint8_t>({0xc4}), true, false).is_discarded());
}




TEST(MessagePackErrorsUnsupportedBytesTest, ConcreteExamples)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0xc1})), "[json.exception.parse_error.112] parse error at byte 1: syntax error while parsing MessagePack value: invalid byte: 0xC1", json::parse_error&);
}

TEST(MessagePackErrorsUnsupportedBytesTest, AllUnsupportedBytes)
{
    for (auto byte :
            {
                // never used
                0xc1
            })
    {
        json _;
        CHECK_THROWS_AS(_ = json::from_msgpack(std::vector<uint8_t>({static_cast<uint8_t>(byte)})), json::parse_error&);
        CHECK(json::from_msgpack(std::vector<uint8_t>({static_cast<uint8_t>(byte)}), true, false).is_discarded());
    }
}

TEST(MessagePackErrorsTest, InvalidStringInMap)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(std::vector<uint8_t>({0x81, 0xff, 0x01})), "[json.exception.parse_error.113] parse error at byte 2: syntax error while parsing MessagePack string: expected length specification (0xA0-0xBF, 0xD9-0xDB); last byte: 0xFF", json::parse_error&);
    CHECK(json::from_msgpack(std::vector<uint8_t>({0x81, 0xff, 0x01}), true, false).is_discarded());
}

class MessagePackErrorsStrictModeTest : public ::testing::Test {
 protected:
    std::vector<uint8_t> vec = {0xc0, 0xc0};
};


TEST_F(MessagePackErrorsStrictModeTest, NonStrictMode)
{
    const auto result = json::from_msgpack(vec, false);
    CHECK(result == json());
}

TEST_F(MessagePackErrorsStrictModeTest, StrictMode)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(vec), "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing MessagePack value: expected end of input; last byte: 0xC0", json::parse_error&);
    CHECK(json::from_msgpack(vec, true, false).is_discarded());
}




TEST(MessagePackSAXAbortsTest, StartArrayLen)
{
    std::vector<uint8_t> v = {0x93, 0x01, 0x02, 0x03};
    SaxCountdown scp(0);
    CHECK(!json::sax_parse(v, &scp, json::input_format_t::msgpack));
}

TEST(MessagePackSAXAbortsTest, StartObjectLen)
{
    std::vector<uint8_t> v = {0x81, 0xa3, 0x66, 0x6F, 0x6F, 0xc2};
    SaxCountdown scp(0);
    CHECK(!json::sax_parse(v, &scp, json::input_format_t::msgpack));
}

TEST(MessagePackSAXAbortsTest, Key)
{
    std::vector<uint8_t> v = {0x81, 0xa3, 0x66, 0x6F, 0x6F, 0xc2};
    SaxCountdown scp(1);
    CHECK(!json::sax_parse(v, &scp, json::input_format_t::msgpack));
}

