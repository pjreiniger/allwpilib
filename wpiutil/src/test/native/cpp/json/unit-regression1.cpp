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

#define JSON_TESTS_PRIVATE
#include "unit-json.h"
using wpi::json;
#ifdef JSON_TEST_NO_GLOBAL_UDLS
    using namespace wpi::literals; // NOLINT(google-build-using-namespace)
#endif

#include <fstream>
#include <sstream>
#include <list>
#include <cstdio>

#ifdef JSON_HAS_CPP_17
    #include <variant>
#endif

#include "fifo_map.hpp"

/////////////////////////////////////////////////////////////////////
// for #972
/////////////////////////////////////////////////////////////////////

template<class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = wpi::fifo_map<K, V, wpi::fifo_map_compare<K>, A>;
using my_json = wpi::basic_json<my_workaround_fifo_map>;

/////////////////////////////////////////////////////////////////////
// for #977
/////////////////////////////////////////////////////////////////////

namespace ns
{
struct foo
{
    int x;
};

template <typename, typename SFINAE = void>
struct foo_serializer;

template<typename T>
struct foo_serializer<T, typename std::enable_if<std::is_same<foo, T>::value>::type>
{
    template <typename BasicJsonType>
    static void to_json(BasicJsonType& j, const T& value)
    {
        j = BasicJsonType{{"x", value.x}};
    }
    template <typename BasicJsonType>
    static void from_json(const BasicJsonType& j, T& value)     // !!!
    {
        wpi::from_json(j.at("x"), value.x);
    }
};

template<typename T>
struct foo_serializer < T, typename std::enable_if < !std::is_same<foo, T>::value >::type >
{
    template <typename BasicJsonType>
    static void to_json(BasicJsonType& j, const T& value) noexcept // NOLINT(bugprone-exception-escape)
    {
        ::wpi::to_json(j, value);
    }
    template <typename BasicJsonType>
    static void from_json(const BasicJsonType& j, T& value)   //!!!
    {
        ::wpi::from_json(j, value);
    }
};
} // namespace ns

using foo_json = wpi::basic_json<std::map, std::vector, std::string, bool, std::int64_t,
      std::uint64_t, double, std::allocator, ns::foo_serializer, std::vector<std::uint8_t>>;

/////////////////////////////////////////////////////////////////////
// for #805
/////////////////////////////////////////////////////////////////////

namespace
{
struct nocopy // NOLINT(cppcoreguidelines-special-member-functions,hicpp-special-member-functions)
{
    nocopy() = default;
    nocopy(const nocopy&) = delete;
    nocopy(nocopy&&) = delete;
    nocopy& operator=(const nocopy&) = delete;
    nocopy& operator=(nocopy&&) = delete;

    int val = 0;

    friend void to_json(json& j, const nocopy& n)
    {
        j = {{"val", n.val}};
    }
};
} // namespace







TEST(RegressionTests1Issue60DoubleQuotationMarkIsNotParsedCorrectlyTest, EscapeDoublequote)
{
    const auto* s = R"(["\"foo\""])";
    json j = json::parse(s);
    auto expected = R"(["\"foo\""])"_json;
    CHECK(j == expected);
}




TEST(RegressionTests1Issue70HandleInfinityAndNaNCasesTest, NANValue)
{
    CHECK(json(NAN).dump() == "null");
    CHECK(json(json::number_float_t(NAN)).dump() == "null");
}

TEST(RegressionTests1Issue70HandleInfinityAndNaNCasesTest, Infinity)
{
    CHECK(json(INFINITY).dump() == "null");
    CHECK(json(json::number_float_t(INFINITY)).dump() == "null");
}

TEST(RegressionTests1Issue70HandleInfinityAndNaNCasesTest, NANValue2)
{
    json j1 = NAN;
    CHECK(j1.is_number_float());
    json::number_float_t f1{j1};
    CHECK(std::isnan(f1));

    json j2 = static_cast<json::number_float_t>(NAN);
    CHECK(j2.is_number_float());
    json::number_float_t f2{j2};
    CHECK(std::isnan(f2));
}

TEST(RegressionTests1Issue70HandleInfinityAndNaNCasesTest, Infinity2)
{
    json j1 = INFINITY;
    CHECK(j1.is_number_float());
    json::number_float_t f1{j1};
    CHECK(!std::isfinite(f1));

    json j2 = static_cast<json::number_float_t>(INFINITY);
    CHECK(j2.is_number_float());
    json::number_float_t f2{j2};
    CHECK(!std::isfinite(f2));
}

TEST(RegressionTests1Test, PullRequest71HandleEnumType)
{
    enum { t = 0, u = 102};
    json j = json::array();
    j.push_back(t);

    // maybe this is not the place to test this?
    json j2 = u;

    auto anon_enum_value = j2.get<decltype(u)>();
    CHECK(u == anon_enum_value);

    // check if the actual value was stored
    CHECK(j2 == 102);

    static_assert(std::is_same<decltype(anon_enum_value), decltype(u)>::value, "types must be the same");

    j.push_back(json::object(
    {
        {"game_type", t}
    }));
}

TEST(RegressionTests1Test, Issue76DumpParseNotIdempotent)
{
    // create JSON object
    json fields;
    fields["one"] = std::string("one");
    fields["two"] = std::string("two three");
    fields["three"] = std::string("three \"four\"");

    // create another JSON object by deserializing the serialization
    std::string payload = fields.dump();
    json parsed_fields = json::parse(payload);

    // check individual fields to match both objects
    CHECK(parsed_fields["one"] == fields["one"]);
    CHECK(parsed_fields["two"] == fields["two"]);
    CHECK(parsed_fields["three"] == fields["three"]);

    // check individual fields to match original input
    CHECK(parsed_fields["one"] == std::string("one"));
    CHECK(parsed_fields["two"] == std::string("two three"));
    CHECK(parsed_fields["three"] == std::string("three \"four\""));

    // check equality of the objects
    CHECK(parsed_fields == fields);

    // check equality of the serialized objects
    CHECK(fields.dump() == parsed_fields.dump());

    // check everything in one line
    CHECK(fields == json::parse(fields.dump()));
}

TEST(RegressionTests1Test, Issue82LexerGetNumberReturnNAN)
{
    const auto* const content = R"(
    {
        "Test":"Test1",
        "Number":100,
        "Foo":42.42
    })";

    std::stringstream ss;
    ss << content;
    json j;
    ss >> j;

    auto test = j["Test"].get<std::string>();
    CHECK(test == "Test1");
    int number{j["Number"]};
    CHECK(number == 100);
    float foo{j["Foo"]};
    EXPECT_NEAR(static_cast<double>(foo), 42.42, 0.001);
}

TEST(RegressionTests1Test, Issue89NonstandardIntegerType)
{
    // create JSON class with nonstandard integer number type
    using custom_json =
        wpi::basic_json<std::map, std::vector, std::string, bool, int32_t, uint32_t, float>;
    custom_json j;
    j["int_1"] = 1;
    CHECK(j["int_1"] == 1);

    // tests for correct handling of non-standard integers that overflow the type selected by the user

    // unsigned integer object creation - expected to wrap and still be stored as an integer
    j = 4294967296U; // 2^32
    CHECK(static_cast<int>(j.type()) == static_cast<int>(custom_json::value_t::number_unsigned));
    CHECK(j.get<uint32_t>() == 0);  // Wrap

    // unsigned integer parsing - expected to overflow and be stored as a float
    j = custom_json::parse("4294967296"); // 2^32
    CHECK(static_cast<int>(j.type()) == static_cast<int>(custom_json::value_t::number_float));
    CHECK(j.get<float>() == 4294967296.0f);

    // integer object creation - expected to wrap and still be stored as an integer
    j = -2147483649LL; // -2^31-1
    CHECK(static_cast<int>(j.type()) == static_cast<int>(custom_json::value_t::number_integer));
    CHECK(j.get<int32_t>() == 2147483647);  // Wrap

    // integer parsing - expected to overflow and be stored as a float with rounding
    j = custom_json::parse("-2147483649"); // -2^31
    CHECK(static_cast<int>(j.type()) == static_cast<int>(custom_json::value_t::number_float));
    CHECK(j.get<float>() == -2147483650.0f);
}

TEST(RegressionTests1Test, Issue93ReverseIteratorOperatorInheritanceProblem)
{
    {
        json a = {1, 2, 3};
        json::reverse_iterator rit = a.rbegin();
        ++rit;
        CHECK(*rit == json(2));
        CHECK(rit.value() == json(2));
    }
    {
        json a = {1, 2, 3};
        json::reverse_iterator rit = ++a.rbegin();
        CHECK(*rit == json(2));
        CHECK(rit.value() == json(2));
    }
    {
        json a = {1, 2, 3};
        json::reverse_iterator rit = a.rbegin();
        ++rit;
        json b = {0, 0, 0};
        std::transform(rit, a.rend(), b.rbegin(), [](json el)
        {
            return el;
        });
        CHECK(b == json({0, 1, 2}));
    }
    {
        json a = {1, 2, 3};
        json b = {0, 0, 0};
        std::transform(++a.rbegin(), a.rend(), b.rbegin(), [](json el)
        {
            return el;
        });
        CHECK(b == json({0, 1, 2}));
    }
}

TEST(RegressionTests1Test, Issue100FailedToIteratorJsonObjectWithReverseIterator)
{
    json config =
    {
        { "111", 111 },
        { "112", 112 },
        { "113", 113 }
    };

    std::stringstream ss;

    for (auto it = config.begin(); it != config.end(); ++it)
    {
        ss << it.key() << ": " << it.value() << '\n';
    }

    for (auto it = config.rbegin(); it != config.rend(); ++it)
    {
        ss << it.key() << ": " << it.value() << '\n';
    }

    CHECK(ss.str() == "111: 111\n112: 112\n113: 113\n113: 113\n112: 112\n111: 111\n");
}

TEST(RegressionTests1Test, Issue101BinaryStringCausesNumbersToBeDumpedAsHex)
{
    int64_t number = 10;
    std::string bytes{"\x00" "asdf\n", 6};
    json j;
    j["int64"] = number;
    j["binary string"] = bytes;
    // make sure the number is really printed as decimal "10" and not as
    // hexadecimal "a"
    CHECK(j.dump() == "{\"binary string\":\"\\u0000asdf\\n\",\"int64\":10}");
}

TEST(RegressionTests1Test, Issue111SubsequentUnicodeChars)
{
    std::string bytes{0x7, 0x7};
    json j;
    j["string"] = bytes;
    CHECK(j["string"] == "\u0007\u0007");
}

#if JSON_USE_IMPLICIT_CONVERSIONS

TEST(RegressionTests1Test, Issue144ImplicitAssignmentToStdStringFails)
{
    json o = {{"name", "value"}};

    std::string s1 = o["name"];
    CHECK(s1 == "value");

    std::string s2;
    s2 = o["name"];

    CHECK(s2 == "value");

    // improve coverage
    o["int"] = 1;
#if JSON_DIAGNOSTICS
    CHECK_THROWS_WITH_AS(s2 = o["int"], "[json.exception.type_error.302] (/int) type must be string, but is number", json::type_error);
#else
    CHECK_THROWS_WITH_AS(s2 = o["int"], "[json.exception.type_error.302] type must be string, but is number", json::type_error);
#endif
}

#endif

TEST(RegressionTests1Test, Issue146CharacterFollowingASurrogatePairIsSkipped)
{
    CHECK(json::parse("\"\\ud80c\\udc60abc\"").get<json::string_t>() == "\xf0\x93\x81\xa0\x61\x62\x63");
}

TEST(RegressionTests1Test, Issue171CannotIndexByKeyOfTypeStaticConstexprConstCharPointer)
{
    json j;

    // Non-const access with key as "char []"
    char array_key[] = "Key1"; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    CHECK_NOTHROW(j[array_key] = 1);
    CHECK(j[array_key] == json(1));

    // Non-const access with key as "const char[]"
    const char const_array_key[] = "Key2"; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    CHECK_NOTHROW(j[const_array_key] = 2);
    CHECK(j[const_array_key] == json(2));

    // Non-const access with key as "char *"
    char _ptr_key[] = "Key3"; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    char* ptr_key = &_ptr_key[0]; // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    CHECK_NOTHROW(j[ptr_key] = 3);
    CHECK(j[ptr_key] == json(3));

    // Non-const access with key as "const char *"
    const char* const_ptr_key = "Key4";
    CHECK_NOTHROW(j[const_ptr_key] = 4);
    CHECK(j[const_ptr_key] == json(4));

    // Non-const access with key as "static constexpr const char *"
    static constexpr const char* constexpr_ptr_key = "Key5";
    CHECK_NOTHROW(j[constexpr_ptr_key] = 5);
    CHECK(j[constexpr_ptr_key] == json(5));

    const json j_const = j;

    // Const access with key as "char []"
    CHECK(j_const[array_key] == json(1));

    // Const access with key as "const char[]"
    CHECK(j_const[const_array_key] == json(2));

    // Const access with key as "char *"
    CHECK(j_const[ptr_key] == json(3));

    // Const access with key as "const char *"
    CHECK(j_const[const_ptr_key] == json(4));

    // Const access with key as "static constexpr const char *"
    CHECK(j_const[constexpr_ptr_key] == json(5));
}

TEST(RegressionTests1Test, Issue186MiloyipNativejsonBenchmarkFloatingPointParsing)
{
    json j;

    j = json::parse("-0.0");
    CHECK(j.get<double>() == -0.0);

    j = json::parse("2.22507385850720113605740979670913197593481954635164564e-308");
    CHECK(j.get<double>() == 2.2250738585072009e-308);

    j = json::parse("0.999999999999999944488848768742172978818416595458984374");
    CHECK(j.get<double>() == 0.99999999999999989);

    j = json::parse("1.00000000000000011102230246251565404236316680908203126");
    CHECK(j.get<double>() == 1.00000000000000022);

    j = json::parse("7205759403792793199999e-5");
    CHECK(j.get<double>() == 72057594037927928.0);

    j = json::parse("922337203685477529599999e-5");
    CHECK(j.get<double>() == 9223372036854774784.0);

    j = json::parse("1014120480182583464902367222169599999e-5");
    CHECK(j.get<double>() == 10141204801825834086073718800384.0);

    j = json::parse("5708990770823839207320493820740630171355185151999e-3");
    CHECK(j.get<double>() == 5708990770823838890407843763683279797179383808.0);

    // create JSON class with nonstandard float number type

    // float
    wpi::basic_json<std::map, std::vector, std::string, bool, int32_t, uint32_t, float> j_float =
        1.23e25f;
    CHECK(j_float.get<float>() == 1.23e25f);

    // double
    wpi::basic_json<std::map, std::vector, std::string, bool, int64_t, uint64_t, double> j_double =
        1.23e35;
    CHECK(j_double.get<double>() == 1.23e35);

    // long double
    wpi::basic_json<std::map, std::vector, std::string, bool, int64_t, uint64_t, long double>
    j_long_double = 1.23e45L;
    CHECK(j_long_double.get<long double>() == 1.23e45L);
}

TEST(RegressionTests1Test, Issue228DoubleValuesAreSerializedWithCommasAsDecimalPoints)
{
    json j1a = 2312.42;
    json j1b = json::parse("2312.42");

    json j2a = 2342e-2;
    //issue #230
    //json j2b = json::parse("2342e-2");

    json j3a = 10E3;
    json j3b = json::parse("10E3");
    json j3c = json::parse("10e3");

    // class to create a locale that would use a comma for decimals
    class CommaDecimalSeparator : public std::numpunct<char>
    {
      protected:
        char do_decimal_point() const override
        {
            return ',';
        }

        char do_thousands_sep() const override
        {
            return '.';
        }

        std::string do_grouping() const override
        {
            return "\03";
        }
    };

    // change locale to mess with decimal points
    auto orig_locale = std::locale::global(std::locale(std::locale(), new CommaDecimalSeparator));

    CHECK(j1a.dump() == "2312.42");
    CHECK(j1b.dump() == "2312.42");

    // check if locale is properly reset
    std::stringstream ss;
    ss.imbue(std::locale(std::locale(), new CommaDecimalSeparator));
    ss << 4712.11;
    CHECK(ss.str() == "4.712,11");
    ss << j1a;
    CHECK(ss.str() == "4.712,112312.42");
    ss << 47.11;
    CHECK(ss.str() == "4.712,112312.4247,11");

    CHECK(j2a.dump() == "23.42");
    //issue #230
    //CHECK(j2b.dump() == "23.42");

    CHECK(j3a.dump() == "10000.0");
    CHECK(j3b.dump() == "10000.0");
    CHECK(j3c.dump() == "10000.0");
    //CHECK(j3b.dump() == "1E04"); // roundtrip error
    //CHECK(j3c.dump() == "1e04"); // roundtrip error

    std::locale::global(orig_locale);
}

TEST(RegressionTests1Test, Issue378LocaleIndependentNumToStr)
{
    static_cast<void>(setlocale(LC_NUMERIC, "de_DE.UTF-8"));

    // verify that dumped correctly with '.' and no grouping
    const json j1 = 12345.67;
    CHECK(json(12345.67).dump() == "12345.67");
    static_cast<void>(setlocale(LC_NUMERIC, "C"));
}

TEST(RegressionTests1Test, Issue379LocaleIndependentStrToNum)
{
    static_cast<void>(setlocale(LC_NUMERIC, "de_DE.UTF-8"));

    // verify that parsed correctly despite using strtod internally
    CHECK(json::parse("3.14").get<double>() == 3.14);

    // check a different code path
    CHECK(json::parse("1.000000000000000000000000000000000000000000000000000000000000000000000000").get<double>() == 1.0);
}

TEST(RegressionTests1Test, Issue233CantUseBasicJsonIteratorAsABaseIteratorForStdMoveIterator)
{
    json source = {"a", "b", "c"};
    json expected = {"a", "b"};
    json dest;

    std::copy_n(std::make_move_iterator(source.begin()), 2, std::back_inserter(dest));

    CHECK(dest == expected);
}

TEST(RegressionTests1Test, Issue235AmbiguousOverloadForPushBackAndOperator)
{
    json data = {{"key", "value"}};
    data.push_back({"key2", "value2"});
    data += {"key3", "value3"};

    CHECK(data == json({{"key", "value"}, {"key2", "value2"}, {"key3", "value3"}}));
}

TEST(RegressionTests1Test, Issue269DiffGeneratesIncorrectPatchWhenRemovingMultipleArrayElements)
{
    json doc = R"( { "arr1": [1, 2, 3, 4] } )"_json;
    json expected = R"( { "arr1": [1, 2] } )"_json;

    // check roundtrip
    CHECK(doc.patch(json::diff(doc, expected)) == expected);
}

TEST(RegressionTests1Test, Issue283ValueDoesNotWorkWithJsonPointerTypes)
{
    json j =
    {
        {"object", {{"key1", 1}, {"key2", 2}}},
    };

    int at_integer{j.at("/object/key2"_json_pointer)};
    int val_integer = j.value("/object/key2"_json_pointer, 0);

    CHECK(at_integer == val_integer);
}

TEST(RegressionTests1Test, Issue304UnusedVariableWarning)
{
    // code triggered a "warning: unused variable" warning and is left
    // here to avoid the warning in the future
    json object;
    json patch = json::array();
    object = object.patch(patch);
}

TEST(RegressionTests1Test, Issue323AddNestedObjectCapabilitiesToPointers)
{
    json j;
    j["/this/that/2"_json_pointer] = 27;
    CHECK(j == json({{"this", {{"that", {nullptr, nullptr, 27}}}}}));
}

TEST(RegressionTests1Test, Issue329SerializedValueNotAlwaysCanBeParsed)
{
    json _;
    CHECK_THROWS_WITH_AS(_ = json::parse("22e2222"), "[json.exception.out_of_range.406] number overflow parsing '22e2222'", json::out_of_range&);
}

TEST(RegressionTests1Test, Issue360LossOfPrecisionWhenSerializingDouble)
{
    auto check_roundtrip = [](double number)
    {
        CAPTURE(number)

        json j = number;
        CHECK(j.is_number_float());

        std::stringstream ss;
        ss << j;

        CHECK_NOTHROW(ss >> j);
        CHECK(j.is_number_float());
        CHECK(j.get<json::number_float_t>() == number);
    };

    check_roundtrip(100000000000.1236);
    check_roundtrip((std::numeric_limits<json::number_float_t>::max)());

    // Some more numbers which fail to roundtrip when serialized with digits10 significand digits (instead of max_digits10)
    check_roundtrip(1.541888611948064e-17);
    check_roundtrip(5.418771028591015e-16);
    check_roundtrip(9.398685592608595e-15);
    check_roundtrip(8.826843952762347e-14);
    check_roundtrip(8.143291313475335e-13);
    check_roundtrip(4.851328172762508e-12);
    check_roundtrip(6.677850998084358e-11);
    check_roundtrip(3.995398518174525e-10);
    check_roundtrip(1.960452605645124e-9);
    check_roundtrip(3.551812586302883e-8);
    check_roundtrip(2.947988411689261e-7);
    check_roundtrip(8.210166748056192e-6);
    check_roundtrip(6.104889704266753e-5);
    check_roundtrip(0.0008629954631330876);
    check_roundtrip(0.004936993881051611);
    check_roundtrip(0.08309725102608073);
    check_roundtrip(0.5210494268499783);
    check_roundtrip(6.382927930939767);
    check_roundtrip(59.94947245358671);
    check_roundtrip(361.0838651266122);
    check_roundtrip(4678.354596181877);
    check_roundtrip(61412.17658956043);
    check_roundtrip(725696.0799057782);
    check_roundtrip(2811732.583399828);
    check_roundtrip(30178351.07533605);
    check_roundtrip(689684880.3235844);
    check_roundtrip(5714887673.555147);
    check_roundtrip(84652038821.18808);
    check_roundtrip(156510583431.7721);
    check_roundtrip(5938450569021.732);
    check_roundtrip(83623297654460.33);
    check_roundtrip(701466573254773.6);
    check_roundtrip(1369013370304513);
    check_roundtrip(96963648023094720); // NOLINT(bugprone-narrowing-conversions,cppcoreguidelines-narrowing-conversions)
    check_roundtrip(3.478237409280108e+17);
}

TEST(RegressionTests1Test, Issue366JsonParseOnFailedStreamGetsStuck)
{
    std::ifstream f("file_not_found.json");
    json _;
    CHECK_THROWS_WITH_AS(_ = json::parse(f), "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);
}

TEST(RegressionTests1Test, Issue367CallingStreamAtEOF)
{
    std::stringstream ss;
    json j;
    ss << "123";
    CHECK_NOTHROW(ss >> j);

    // see https://github.com/nlohmann/json/issues/367#issuecomment-262841893:
    // ss is not at EOF; this yielded an error before the fix
    // (threw basic_string::append). No, it should just throw
    // a parse error because of the EOF.
    CHECK_THROWS_WITH_AS(ss >> j, "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);
}




TEST(RegressionTests1Issue367BehaviorOfInOperatorShouldMoreCloselyResembleThatOfBuiltInOverloadsTest, Empty)
{
    std::stringstream ss;
    json j;
    CHECK_THROWS_WITH_AS(ss >> j, "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);
}

TEST(RegressionTests1Issue367BehaviorOfInOperatorShouldMoreCloselyResembleThatOfBuiltInOverloadsTest, Whitespace)
{
    std::stringstream ss;
    ss << "   ";
    json j;
    CHECK_THROWS_WITH_AS(ss >> j,
                         "[json.exception.parse_error.101] parse error at line 1, column 4: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);
}

TEST(RegressionTests1Issue367BehaviorOfInOperatorShouldMoreCloselyResembleThatOfBuiltInOverloadsTest, OneValue)
{
    std::stringstream ss;
    ss << "111";
    json j;
    CHECK_NOTHROW(ss >> j);
    CHECK(j == 111);

    CHECK_THROWS_WITH_AS(ss >> j, "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);
}

TEST(RegressionTests1Issue367BehaviorOfInOperatorShouldMoreCloselyResembleThatOfBuiltInOverloadsTest, OneValueWhitespace)
{
    std::stringstream ss;
    ss << "222 \t\n";
    json j;
    CHECK_NOTHROW(ss >> j);
    CHECK(j == 222);

    CHECK_THROWS_WITH_AS(ss >> j,
                         "[json.exception.parse_error.101] parse error at line 2, column 1: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);
}

TEST(RegressionTests1Issue367BehaviorOfInOperatorShouldMoreCloselyResembleThatOfBuiltInOverloadsTest, WhitespaceOneValue)
{
    std::stringstream ss;
    ss << "\n\t 333";
    json j;
    CHECK_NOTHROW(ss >> j);
    CHECK(j == 333);

    CHECK_THROWS_WITH_AS(ss >> j, "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);
}

TEST(RegressionTests1Issue367BehaviorOfInOperatorShouldMoreCloselyResembleThatOfBuiltInOverloadsTest, ThreeValues)
{
    std::stringstream ss;
    ss << " 111 \n222\n \n  333";
    json j;
    CHECK_NOTHROW(ss >> j);
    CHECK(j == 111);
    CHECK_NOTHROW(ss >> j);
    CHECK(j == 222);
    CHECK_NOTHROW(ss >> j);
    CHECK(j == 333);

    CHECK_THROWS_WITH_AS(ss >> j, "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);
}

TEST(RegressionTests1Issue367BehaviorOfInOperatorShouldMoreCloselyResembleThatOfBuiltInOverloadsTest, LiteralsWithoutWhitespace)
{
    std::stringstream ss;
    ss << "truefalsenull\"\"";
    json j;
    CHECK_NOTHROW(ss >> j);
    CHECK(j == true);
    CHECK_NOTHROW(ss >> j);
    CHECK(j == false);
    CHECK_NOTHROW(ss >> j);
    CHECK(j == nullptr);
    CHECK_NOTHROW(ss >> j);
    CHECK(j == "");

    CHECK_THROWS_WITH_AS(ss >> j, "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);
}

TEST(RegressionTests1Issue367BehaviorOfInOperatorShouldMoreCloselyResembleThatOfBuiltInOverloadsTest, ExampleFrom529)
{
    std::stringstream ss;
    ss << "{\n    \"one\"   : 1,\n    \"two\"   : 2\n}\n{\n    \"three\" : 3\n}";
    json j;
    CHECK_NOTHROW(ss >> j);
    CHECK(j == json({{"one", 1}, {"two", 2}}));
    CHECK_NOTHROW(ss >> j);
    CHECK(j == json({{"three", 3}}));

    CHECK_THROWS_WITH_AS(ss >> j, "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - unexpected end of input; expected '[', '{', or a literal", json::parse_error&);
}

TEST(RegressionTests1Issue367BehaviorOfInOperatorShouldMoreCloselyResembleThatOfBuiltInOverloadsTest, SecondExampleFrom529)
{
    std::string str = "{\n\"one\"   : 1,\n\"two\"   : 2\n}\n{\n\"three\" : 3\n}";

    {
        std::ofstream file("test.json");
        file << str;
    }

    std::ifstream stream("test.json", std::ifstream::in);
    json val;

    size_t i = 0;
    while (stream.peek() != EOF)
    {
        CAPTURE(i)
        CHECK_NOTHROW(stream >> val);

        CHECK(i < 2);

        if (i == 0)
        {
            CHECK(val == json({{"one", 1}, {"two", 2}}));
        }

        if (i == 1)
        {
            CHECK(val == json({{"three", 3}}));
        }

        ++i;
    }

    static_cast<void>(std::remove("test.json"));
}

TEST(RegressionTests1Test, Issue389IntegerOverflowOSSFuzzIssue267)
{
    // original test case
    json j1 = json::parse("-9223372036854775808");
    CHECK(j1.is_number_integer());
    CHECK(j1.get<json::number_integer_t>() == INT64_MIN);

    // edge case (+1; still an integer)
    json j2 = json::parse("-9223372036854775807");
    CHECK(j2.is_number_integer());
    CHECK(j2.get<json::number_integer_t>() == INT64_MIN + 1);

    // edge case (-1; overflow -> floats)
    json j3 = json::parse("-9223372036854775809");
    CHECK(j3.is_number_float());
}

TEST(RegressionTests1Test, Issue380BugInOverflowDetectionWhenParsingIntegers)
{
    json j = json::parse("166020696663385964490");
    CHECK(j.is_number_float());
    CHECK(j.get<json::number_float_t>() == 166020696663385964490.0);
}

TEST(RegressionTests1Test, Issue405HeapBufferOverflowOSSFuzzIssue342)
{
    // original test case
    std::vector<uint8_t> vec {0x65, 0xf5, 0x0a, 0x48, 0x21};
    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec), "[json.exception.parse_error.110] parse error at byte 6: syntax error while parsing CBOR string: unexpected end of input", json::parse_error&);
}

TEST(RegressionTests1Test, Issue407HeapBufferOverflowOSSFuzzIssue343)
{
    json _;

    // original test case: incomplete float64
    std::vector<uint8_t> vec1 {0xcb, 0x8f, 0x0a};
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(vec1), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);

    // related test case: incomplete float32
    std::vector<uint8_t> vec2 {0xca, 0x8f, 0x0a};
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(vec2), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing MessagePack number: unexpected end of input", json::parse_error&);

    // related test case: incomplete Half-Precision Float (CBOR)
    std::vector<uint8_t> vec3 {0xf9, 0x8f};
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec3), "[json.exception.parse_error.110] parse error at byte 3: syntax error while parsing CBOR number: unexpected end of input", json::parse_error&);

    // related test case: incomplete Single-Precision Float (CBOR)
    std::vector<uint8_t> vec4 {0xfa, 0x8f, 0x0a};
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec4), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing CBOR number: unexpected end of input", json::parse_error&);

    // related test case: incomplete Double-Precision Float (CBOR)
    std::vector<uint8_t> vec5 {0xfb, 0x8f, 0x0a};
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec5), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing CBOR number: unexpected end of input", json::parse_error&);
}

TEST(RegressionTests1Test, Issue408HeapBufferOverflowOSSFuzzIssue344)
{
    json _;

    // original test case
    std::vector<uint8_t> vec1 {0x87};
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(vec1), "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing MessagePack string: unexpected end of input", json::parse_error&);

    // more test cases for MessagePack
    for (auto b :
            {
                0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, // fixmap
                0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, // fixarray
                0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, // fixstr
                0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf
            })
    {
        std::vector<uint8_t> vec(1, static_cast<uint8_t>(b));
        CHECK_THROWS_AS(_ = json::from_msgpack(vec), json::parse_error&);
    }

    // more test cases for CBOR
    for (auto b :
            {
                0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
                0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, // UTF-8 string
                0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
                0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, // array
                0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
                0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7 // map
            })
    {
        std::vector<uint8_t> vec(1, static_cast<uint8_t>(b));
        CHECK_THROWS_AS(_ = json::from_cbor(vec), json::parse_error&);
    }

    // special case: empty input
    std::vector<uint8_t> vec2;
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec2), "[json.exception.parse_error.110] parse error at byte 1: syntax error while parsing CBOR value: unexpected end of input", json::parse_error&);
    CHECK_THROWS_WITH_AS(_ = json::from_msgpack(vec2), "[json.exception.parse_error.110] parse error at byte 1: syntax error while parsing MessagePack value: unexpected end of input", json::parse_error&);
}

TEST(RegressionTests1Test, Issue411HeapBufferOverflowOSSFuzzIssue366)
{
    json _;

    // original test case: empty UTF-8 string (indefinite length)
    std::vector<uint8_t> vec1 {0x7f};
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec1), "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing CBOR string: unexpected end of input", json::parse_error&);

    // related test case: empty array (indefinite length)
    std::vector<uint8_t> vec2 {0x9f};
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec2), "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing CBOR value: unexpected end of input", json::parse_error&);

    // related test case: empty map (indefinite length)
    std::vector<uint8_t> vec3 {0xbf};
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec3), "[json.exception.parse_error.110] parse error at byte 2: syntax error while parsing CBOR string: unexpected end of input", json::parse_error&);
}

TEST(RegressionTests1Test, Issue412HeapBufferOverflowOSSFuzzIssue367)
{
    // original test case
    std::vector<uint8_t> vec
    {
        0xab, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98,
        0x98, 0x98, 0x98, 0x98, 0x98, 0x00, 0x00, 0x00,
        0x60, 0xab, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98,
        0x98, 0x98, 0x98, 0x98, 0x98, 0x00, 0x00, 0x00,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0xa0, 0x9f,
        0x9f, 0x97, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
        0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60
    };

    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec), "[json.exception.parse_error.113] parse error at byte 2: syntax error while parsing CBOR string: expected length specification (0x60-0x7B) or indefinite string type (0x7F); last byte: 0x98", json::parse_error&);

    // related test case: nonempty UTF-8 string (indefinite length)
    std::vector<uint8_t> vec1 {0x7f, 0x61, 0x61};
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec1), "[json.exception.parse_error.110] parse error at byte 4: syntax error while parsing CBOR string: unexpected end of input", json::parse_error&);

    // related test case: nonempty array (indefinite length)
    std::vector<uint8_t> vec2 {0x9f, 0x01};
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec2), "[json.exception.parse_error.110] parse error at byte 3: syntax error while parsing CBOR value: unexpected end of input", json::parse_error&);

    // related test case: nonempty map (indefinite length)
    std::vector<uint8_t> vec3 {0xbf, 0x61, 0x61, 0x01};
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec3), "[json.exception.parse_error.110] parse error at byte 5: syntax error while parsing CBOR string: unexpected end of input", json::parse_error&);
}

TEST(RegressionTests1Test, Issue414CompareWithLiteral0)
{
#define CHECK_TYPE(v) \
CHECK((json(v) == (v)));\
CHECK(((v) == json(v)));\
CHECK_FALSE((json(v) != (v)));\
CHECK_FALSE(((v) != json(v)));

    CHECK_TYPE(nullptr)
    CHECK_TYPE(0)
    CHECK_TYPE(0u)
    CHECK_TYPE(0L)
    CHECK_TYPE(0.0)
    CHECK_TYPE("") // NOLINT(readability-container-size-empty)

#undef CHECK_TYPE
}

TEST(RegressionTests1Test, Issue416UseOfUninitializedValueOSSFuzzIssue377)
{
    // original test case
    std::vector<uint8_t> vec1
    {
        0x94, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa,
        0x3a, 0x96, 0x96, 0xb4, 0xb4, 0xb4, 0xb4, 0xb4,
        0xb4, 0xb4, 0xb4, 0xb4, 0xb4, 0xb4, 0xb4, 0x71,
        0xb4, 0xb4, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0x3a,
        0x96, 0x96, 0xb4, 0xb4, 0xfa, 0x94, 0x94, 0x61,
        0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0xfa
    };

    json _;
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec1), "[json.exception.parse_error.113] parse error at byte 13: syntax error while parsing CBOR string: expected length specification (0x60-0x7B) or indefinite string type (0x7F); last byte: 0xB4", json::parse_error&);

    // related test case: double-precision
    std::vector<uint8_t> vec2
    {
        0x94, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa,
        0x3a, 0x96, 0x96, 0xb4, 0xb4, 0xb4, 0xb4, 0xb4,
        0xb4, 0xb4, 0xb4, 0xb4, 0xb4, 0xb4, 0xb4, 0x71,
        0xb4, 0xb4, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0x3a,
        0x96, 0x96, 0xb4, 0xb4, 0xfa, 0x94, 0x94, 0x61,
        0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0xfb
    };
    CHECK_THROWS_WITH_AS(_ = json::from_cbor(vec2), "[json.exception.parse_error.113] parse error at byte 13: syntax error while parsing CBOR string: expected length specification (0x60-0x7B) or indefinite string type (0x7F); last byte: 0xB4", json::parse_error&);
}

TEST(RegressionTests1Test, Issue452HeapBufferOverflowOSSFuzzIssue585)
{
    std::vector<uint8_t> vec = {'-', '0', '1', '2', '2', '7', '4'};
    json _;
    CHECK_THROWS_AS(_ = json::parse(vec), json::parse_error&);
}

TEST(RegressionTests1Test, Issue454DoublesArePrintedAsIntegers)
{
    json j = R"({"bool_value":true,"double_value":2.0,"int_value":10,"level1":{"list_value":[3,"hi",false],"tmp":5.0},"string_value":"hello"})"_json;
    CHECK(j["double_value"].is_number_float());
}

#if JSON_USE_IMPLICIT_CONVERSIONS

TEST(RegressionTests1Test, Issue464VS2017ImplicitToStdStringConversionFix)
{
    json v = "test";
    std::string test;
    test = v;
    CHECK(v == "test");
}

#endif

TEST(RegressionTests1Test, Issue465RoundtripErrorWhileParsing1000000000000000010E5)
{
    json j1 = json::parse("1000000000000000010E5");
    std::string s1 = j1.dump();
    json j2 = json::parse(s1);
    std::string s2 = j2.dump();
    CHECK(s1 == s2);
}

#if JSON_USE_IMPLICIT_CONVERSIONS

class RegressionTests1Issue473InconsistentBehaviorInConversionToArrayTypeTest : public ::testing::Test {
 protected:
    json j_array = {1, 2, 3, 4};
    json j_number = 42;
    json j_null = nullptr;
};


TEST_F(RegressionTests1Issue473InconsistentBehaviorInConversionToArrayTypeTest, StdVector)
{
    auto create = [](const json & j)
    {
        std::vector<int> v = j;
    };

    CHECK_NOTHROW(create(j_array));
    CHECK_THROWS_WITH_AS(create(j_number), "[json.exception.type_error.302] type must be array, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(create(j_null), "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
}

TEST_F(RegressionTests1Issue473InconsistentBehaviorInConversionToArrayTypeTest, StdList)
{
    auto create = [](const json & j)
    {
        std::list<int> v = j;
    };

    CHECK_NOTHROW(create(j_array));
    CHECK_THROWS_WITH_AS(create(j_number), "[json.exception.type_error.302] type must be array, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(create(j_null), "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
}

TEST_F(RegressionTests1Issue473InconsistentBehaviorInConversionToArrayTypeTest, StdForwardList)
{
    auto create = [](const json & j)
    {
        std::forward_list<int> v = j;
    };

    CHECK_NOTHROW(create(j_array));
    CHECK_THROWS_WITH_AS(create(j_number), "[json.exception.type_error.302] type must be array, but is number", json::type_error&);
    CHECK_THROWS_WITH_AS(create(j_null), "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
}

#endif

TEST(RegressionTests1Test, Issue486JsonValueTCantBeAMapsKeyTypeInVC2015)
{
    // the code below must compile with MSVC
    std::map<json::value_t, std::string> jsonTypes ;
    jsonTypes[json::value_t::array] = "array";
}

TEST(RegressionTests1Test, Issue494ConversionFromVectorBoolToJsonFailsToBuild)
{
    std::vector<bool> boolVector = {false, true, false, false};
    json j;
    j["bool_vector"] = boolVector;

    CHECK(j["bool_vector"].dump() == "[false,true,false,false]");
}

TEST(RegressionTests1Test, Issue504AssertionErrorOSSFuzz856)
{
    std::vector<uint8_t> vec1 = {0xf9, 0xff, 0xff, 0x4a, 0x3a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x37, 0x02, 0x38};
    json j1 = json::from_cbor(vec1, false);

    // step 2: round trip
    std::vector<uint8_t> vec2 = json::to_cbor(j1);

    // parse serialization
    json j2 = json::from_cbor(vec2);

    // NaN is dumped to "null"
    CHECK(j2.is_number_float());
    CHECK(std::isnan(j2.get<json::number_float_t>()));
    CHECK(j2.dump() == "null");

    // check if serializations match
    CHECK(json::to_cbor(j2) == vec2);
}

TEST(RegressionTests1Test, Issue512UseOfOverloadedOperatorIsAmbiguous)
{
    json j;
    j["a"] = 5;

    // json op scalar
    CHECK(j["a"] == 5);
    CHECK(j["a"] != 4);

    CHECK(j["a"] <= 7);
    CHECK(j["a"] <  7);
    CHECK(j["a"] >= 3);
    CHECK(j["a"] >  3);


    CHECK(!(j["a"] <= 4));
    CHECK(!(j["a"] <  4));
    CHECK(!(j["a"] >= 6));
    CHECK(!(j["a"] >  6));

    // scalar op json
    CHECK(5 == j["a"]);
    CHECK(4 != j["a"]);

    CHECK(7 >= j["a"]);
    CHECK(7 >  j["a"]);
    CHECK(3 <= j["a"]);
    CHECK(3 <  j["a"]);

    CHECK(!(4 >= j["a"]));
    CHECK(!(4 >  j["a"]));
    CHECK(!(6 <= j["a"]));
    CHECK(!(6 <  j["a"]));
}

TEST(RegressionTests1Test, Issue575HeapBufferOverflowOSSFuzz1400)
{
    json _;
    std::vector<uint8_t> vec = {'"', '\\', '"', 'X', '"', '"'};
    CHECK_THROWS_AS(_ = json::parse(vec), json::parse_error&);
}

#if JSON_USE_IMPLICIT_CONVERSIONS




TEST(RegressionTests1Issue600HowDoesOneConvertAMapInJsonBackToStdMapTest, Example1)
{
    // create a map
    std::map<std::string, int> m1 {{"key", 1}};

    // create and print a JSON from the map
    json j = m1;

    // get the map out of JSON
    std::map<std::string, int> m2 = j;

    // make sure the roundtrip succeeds
    CHECK(m1 == m2);
}

TEST(RegressionTests1Issue600HowDoesOneConvertAMapInJsonBackToStdMapTest, Example2)
{
    // create a map
    std::map<std::string, std::string> m1 {{"key", "val"}};

    // create and print a JSON from the map
    json j = m1;

    // get the map out of JSON
    std::map<std::string, std::string> m2 = j;

    // make sure the roundtrip succeeds
    CHECK(m1 == m2);
}

#endif

TEST(RegressionTests1Test, Issue602BOMNotSkippedWhenUsingJsonParseIterator)
{
    std::string i = "\xef\xbb\xbf{\n   \"foo\": true\n}";
    json _;
    CHECK_NOTHROW(_ = json::parse(i.begin(), i.end()));
}

#if JSON_USE_IMPLICIT_CONVERSIONS




TEST(RegressionTests1Issue702ConversionFromValarrayDoubleToJsonFailsToBuildTest, OriginalExample)
{
    std::valarray<double> v;
    wpi::json j;
    j["test"] = v;
}

TEST(RegressionTests1Issue702ConversionFromValarrayDoubleToJsonFailsToBuildTest, FullExample)
{
    std::valarray<double> v = {1.2, 2.3, 3.4, 4.5};
    json j = v;
    std::valarray<double> vj = j;

    CHECK(j == json(vj));
    CHECK(v.size() == vj.size());
    for (size_t i = 0; i < v.size(); ++i)
    {
        CHECK(v[i] == vj[i]);
        CHECK(v[i] == j[i]);
    }

    CHECK_THROWS_WITH_AS(json().get<std::valarray<double>>(), "[json.exception.type_error.302] type must be array, but is null", json::type_error&);
}

#endif




TEST(RegressionTests1Issue367BehaviorOfInOperatorShouldMoreCloselyResembleThatOfBuiltInOverloadsTest, Example1)
{
    std::istringstream i1_2_3( R"({"first": "one" }{"second": "two"}3)" );
    json j1;
    json j2;
    json j3;
    i1_2_3 >> j1;
    i1_2_3 >> j2;
    i1_2_3 >> j3;

    auto m1 = j1.get<std::map<std::string, std::string>>();
    auto m2 = j2.get<std::map<std::string, std::string>>();
    int i3{j3};

    CHECK( m1 == ( std::map<std::string, std::string> {{ "first",  "one" }} ));
    CHECK( m2 == ( std::map<std::string, std::string> {{ "second", "two" }} ));
    CHECK( i3 == 3 );
}

TEST(RegressionTests1Test, Issue805CopyConstructorIsUsedWithStdInitializerListConstructor)
{
    nocopy n;
    json j;
    j = {{"nocopy", n}};
    CHECK(j["nocopy"]["val"] == 0);
}

TEST(RegressionTests1Test, Issue838IncorrectParseErrorWithBinaryDataInKeys)
{
    std::array<uint8_t, 28> key1 = {{ 103, 92, 117, 48, 48, 48, 55, 92, 114, 215, 126, 214, 95, 92, 34, 174, 40, 71, 38, 174, 40, 71, 38, 223, 134, 247, 127, 0 }};
    std::string key1_str(reinterpret_cast<char*>(key1.data()));
    json j = key1_str;
    CHECK_THROWS_WITH_AS(j.dump(), "[json.exception.type_error.316] invalid UTF-8 byte at index 10: 0x7E", json::type_error&);
}

#if JSON_USE_IMPLICIT_CONVERSIONS

TEST(RegressionTests1Test, Issue843ConvertingToArrayNotWorking)
{
    json j;
    std::array<int, 4> ar = {{1, 1, 1, 1}};
    j = ar;
    ar = j;
}

#endif

TEST(RegressionTests1Test, Issue894InvalidRFC6902CopyOperationSucceeds)
{
    auto model = R"({
        "one": {
            "two": {
                "three": "hello",
                "four": 42
            }
        }
    })"_json;

    auto p1 = R"([{"op": "move",
                   "from": "/one/two/three",
                   "path": "/a/b/c"}])"_json;
    CHECK_THROWS_WITH_AS(model.patch(p1),
                         "[json.exception.out_of_range.403] key 'a' not found", json::out_of_range&);

    auto p2 = R"([{"op": "copy",
                   "from": "/one/two/three",
                   "path": "/a/b/c"}])"_json;
    CHECK_THROWS_WITH_AS(model.patch(p2),
                         "[json.exception.out_of_range.403] key 'a' not found", json::out_of_range&);
}

TEST(RegressionTests1Test, Issue961IncorrectParsingOfIndefiniteLengthCBORStrings)
{
    std::vector<uint8_t> v_cbor =
    {
        0x7F,
        0x64,
        'a', 'b', 'c', 'd',
        0x63,
        '1', '2', '3',
        0xFF
    };
    json j = json::from_cbor(v_cbor);
    CHECK(j == "abcd123");
}

TEST(RegressionTests1Test, Issue962TimeoutOSSFuzz6034)
{
    json _;
    std::vector<uint8_t> v_ubjson = {'[', '$', 'Z', '#', 'L', 0x78, 0x28, 0x00, 0x68, 0x28, 0x69, 0x69, 0x17};
    CHECK_THROWS_AS(_ = json::from_ubjson(v_ubjson), json::out_of_range&);
    //CHECK_THROWS_WITH(json::from_ubjson(v_ubjson),
    //                  "[json.exception.out_of_range.408] excessive array size: 8658170730974374167");

    v_ubjson[0] = '{';
    CHECK_THROWS_AS(_ = json::from_ubjson(v_ubjson), json::out_of_range&);
    //CHECK_THROWS_WITH(json::from_ubjson(v_ubjson),
    //                  "[json.exception.out_of_range.408] excessive object size: 8658170730974374167");
}

TEST(RegressionTests1Test, Issue971AddASAXParserLateBug)
{
    // a JSON text
    const auto* text = R"(
{
    "Image": {
        "Width":  800,
        "Height": 600,
        "Title":  "View from 15th Floor",
        "Thumbnail": {
            "Url":    "http://www.example.com/image/481989943",
            "Height": 125,
            "Width":  100
        },
        "Animated" : false,
        "IDs": [116, 943, 234, 38793]
    }
}
)";

    // define parser callback
    json::parser_callback_t cb = [](int /*depth*/, json::parse_event_t event, json & parsed)
    {
        // skip object elements with key "Thumbnail"
        return !(event == json::parse_event_t::key && parsed == json("Thumbnail"));
    };

    // parse (with callback) and serialize JSON
    json j_filtered = json::parse(text, cb);

    CHECK(j_filtered == R"({"Image":{"Animated":false,"Height":600,"IDs":[116,943,234,38793], "Title":"View from 15th Floor","Width":800}})"_json);
}

TEST(RegressionTests1Test, Issue972SegmentationFaultOnGWhenTryingToAssignJsonStringLiteralToCustomJsonType)
{
    my_json foo = R"([1, 2, 3])"_json;
}

TEST(RegressionTests1Test, Issue977AssigningBetweenDifferentJsonTypes)
{
    foo_json lj = ns::foo{3};
    ns::foo ff(lj);
    CHECK(lj.is_object());
    CHECK(lj.size() == 1);
    CHECK(lj["x"] == 3);
    CHECK(ff.x == 3);
    wpi::json nj = lj;                // This line works as expected
}

#if !defined(JSON_NOEXCEPTION)



TEST(RegressionTestsExceptionsDependentTest, Issue1340EofNotSetOnExhaustedInputStream)
{
    std::stringstream s("{}{}");
    json j;
    s >> j;
    s >> j;
    CHECK_THROWS_AS(s >> j, json::parse_error const&);
    CHECK(s.eof());
}
#endif

/////////////////////////////////////////////////////////////////////
// for #1642
/////////////////////////////////////////////////////////////////////

// the code below fails with Clang on Windows, so we need to exclude it there
#if DOCTEST_CLANG && (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__))
#else
template <typename T> class array {};
template <typename T> class object {};
template <typename T> class string {};
template <typename T> class number_integer {};
template <typename T> class number_unsigned {};
template <typename T> class number_float {};
#endif

