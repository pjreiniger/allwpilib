//     __ _____ _____ _____
//  __|  |   __|     |   | |  JSON for Modern C++ (supporting code)
// |  |  |__   |  |  | | | |  version 3.11.2
// |_____|_____|_____|_|___|  https://github.com/nlohmann/json
//
// SPDX-FileCopyrightText: 2013-2022 Niels Lohmann <https://nlohmann.me>
// SPDX-License-Identifier: MIT

// cmake/test.cmake selects the C++ standard versions with which to build a
// unit test based on the presence of JSON_HAS_CPP_<VERSION> macros.
// When using macros that are only defined for particular versions of the standard
// (e.g., JSON_HAS_FILESYSTEM for C++17 and up), please mention the corresponding
// version macro in a comment close by, like this:
// JSON_HAS_CPP_<VERSION> (do not remove; see note at top of file)

#include "gtest/gtest.h"

// for some reason including this after the json header leads to linker errors with VS 2017...
#include <locale>

#define JSON_TESTS_PRIVATE
#include "unit-json.h"
using json = wpi::json;
using ordered_json = wpi::ordered_json;
#ifdef JSON_TEST_NO_GLOBAL_UDLS
    using namespace wpi::literals; // NOLINT(google-build-using-namespace)
#endif

#include <cstdio>
#include <list>
#include <type_traits>
#include <utility>

#ifdef JSON_HAS_CPP_17
    #include <any>
    #include <variant>
#endif

#ifdef JSON_HAS_CPP_20
    #include <span>
#endif

// WPI_JSON_SERIALIZE_ENUM uses a static std::pair

/////////////////////////////////////////////////////////////////////
// for #1021
/////////////////////////////////////////////////////////////////////

using float_json = wpi::basic_json<std::map, std::vector, std::string, bool, std::int64_t, std::uint64_t, float>;

/////////////////////////////////////////////////////////////////////
// for #1647
/////////////////////////////////////////////////////////////////////
namespace
{
struct NonDefaultFromJsonStruct
{};

inline bool operator==(NonDefaultFromJsonStruct const& /*unused*/, NonDefaultFromJsonStruct const& /*unused*/)
{
    return true;
}

enum class for_1647
{
    one,
    two
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays): this is a false positive
WPI_JSON_SERIALIZE_ENUM(for_1647,
{
    {for_1647::one, "one"},
    {for_1647::two, "two"},
})
}  // namespace

/////////////////////////////////////////////////////////////////////
// for #1299
/////////////////////////////////////////////////////////////////////

struct Data
{
    Data() = default;
    Data(std::string a_, std::string b_)
        : a(std::move(a_))
        , b(std::move(b_))
    {}
    std::string a{};
    std::string b{};
};

void from_json(const json& j, Data& data);
void from_json(const json& j, Data& data)
{
    j["a"].get_to(data.a);
    j["b"].get_to(data.b);
}

bool operator==(Data const& lhs, Data const& rhs);
bool operator==(Data const& lhs, Data const& rhs)
{
    return lhs.a == rhs.a && lhs.b == rhs.b;
}

//bool operator!=(Data const& lhs, Data const& rhs)
//{
//    return !(lhs == rhs);
//}

namespace wpi
{
template<>
struct adl_serializer<NonDefaultFromJsonStruct>
{
    static NonDefaultFromJsonStruct from_json(json const& /*unused*/) noexcept
    {
        return {};
    }
};
}  // namespace wpi

/////////////////////////////////////////////////////////////////////
// for #1805
/////////////////////////////////////////////////////////////////////

struct NotSerializableData
{
    int mydata;
    float myfloat;
};

/////////////////////////////////////////////////////////////////////
// for #2574
/////////////////////////////////////////////////////////////////////

struct NonDefaultConstructible
{
    explicit NonDefaultConstructible(int a)
        : x(a)
    {}
    int x;
};

namespace wpi
{
template<>
struct adl_serializer<NonDefaultConstructible>
{
    static NonDefaultConstructible from_json(json const& j)
    {
        return NonDefaultConstructible(j.get<int>());
    }
};
}  // namespace wpi

/////////////////////////////////////////////////////////////////////
// for #2824
/////////////////////////////////////////////////////////////////////

namespace {
class sax_no_exception : public wpi::detail::json_sax_dom_parser<json>
{
  public:
    explicit sax_no_exception(json& j)
        : wpi::detail::json_sax_dom_parser<json>(j, false)
    {}

    static bool parse_error(std::size_t /*position*/, const std::string& /*last_token*/, const json::exception& ex)
    {
        error_string = new std::string(ex.what());  // NOLINT(cppcoreguidelines-owning-memory)
        return false;
    }

    static std::string* error_string;
};

std::string* sax_no_exception::error_string = nullptr;
}

/////////////////////////////////////////////////////////////////////
// for #2982
/////////////////////////////////////////////////////////////////////

template<class T>
class my_allocator : public std::allocator<T>
{
  public:
    using std::allocator<T>::allocator;
};

/////////////////////////////////////////////////////////////////////
// for #3077
/////////////////////////////////////////////////////////////////////

class FooAlloc
{};

class Foo
{
  public:
    explicit Foo(const FooAlloc& /* unused */ = FooAlloc()) {}

    bool value = false;
};

class FooBar
{
  public:
    Foo foo{};
};

inline void from_json(const wpi::json& j, FooBar& fb)
{
    j.at("value").get_to(fb.foo.value);
}

/////////////////////////////////////////////////////////////////////
// for #3171
/////////////////////////////////////////////////////////////////////

struct for_3171_base // NOLINT(cppcoreguidelines-special-member-functions)
{
    for_3171_base(const std::string& /*unused*/ = {}) {}
    virtual ~for_3171_base() = default;

    virtual void _from_json(const json& j)
    {
        j.at("str").get_to(str);
    }

    std::string str{};
};

struct for_3171_derived : public for_3171_base
{
    for_3171_derived() = default;
    explicit for_3171_derived(const std::string& /*unused*/) { }
};

inline void from_json(const json& j, for_3171_base& tb)
{
    tb._from_json(j);
}

/////////////////////////////////////////////////////////////////////
// for #3312
/////////////////////////////////////////////////////////////////////

#ifdef JSON_HAS_CPP_20
struct for_3312
{
    std::string name;
};

inline void from_json(const json& j, for_3312& obj)
{
    j.at("name").get_to(obj.name);
}
#endif

/////////////////////////////////////////////////////////////////////
// for #3204
/////////////////////////////////////////////////////////////////////

struct for_3204_foo
{
    for_3204_foo() = default;
    explicit for_3204_foo(std::string /*unused*/) {} // NOLINT(performance-unnecessary-value-param)
};

struct for_3204_bar
{
    enum constructed_from_t
    {
        constructed_from_none = 0,
        constructed_from_foo = 1,
        constructed_from_json = 2
    };

    explicit for_3204_bar(std::function<void(for_3204_foo)> /*unused*/) noexcept // NOLINT(performance-unnecessary-value-param)
        : constructed_from(constructed_from_foo) {}
    explicit for_3204_bar(std::function<void(json)> /*unused*/) noexcept // NOLINT(performance-unnecessary-value-param)
        : constructed_from(constructed_from_json) {}

    constructed_from_t constructed_from = constructed_from_none;
};

/////////////////////////////////////////////////////////////////////
// for #3333
/////////////////////////////////////////////////////////////////////

struct for_3333 final
{
    for_3333(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}

    template <class T>
    for_3333(const T& /*unused*/)
    {
        CHECK(false);
    }

    int x = 0;
    int y = 0;
};

template <>
inline for_3333::for_3333(const json& j)
    : for_3333(j.value("x", 0), j.value("y", 0))
{}




TEST(RegressionTests2Test, Issue1001FixMemoryLeakDuringParserCallback)
{
    const auto* geojsonExample = R"(
      { "type": "FeatureCollection",
        "features": [
          { "type": "Feature",
            "geometry": {"type": "Point", "coordinates": [102.0, 0.5]},
            "properties": {"prop0": "value0"}
            },
          { "type": "Feature",
            "geometry": {
              "type": "LineString",
              "coordinates": [
                [102.0, 0.0], [103.0, 1.0], [104.0, 0.0], [105.0, 1.0]
                ]
              },
            "properties": {
              "prop0": "value0",
              "prop1": 0.0
              }
            },
          { "type": "Feature",
             "geometry": {
               "type": "Polygon",
               "coordinates": [
                 [ [100.0, 0.0], [101.0, 0.0], [101.0, 1.0],
                   [100.0, 1.0], [100.0, 0.0] ]
                 ]
             },
             "properties": {
               "prop0": "value0",
               "prop1": {"this": "that"}
               }
             }
           ]
         })";

    json::parser_callback_t cb = [&](int /*level*/, json::parse_event_t event, json & parsed) noexcept
    {
        // skip uninteresting events
        if (event == json::parse_event_t::value && !parsed.is_primitive())
        {
            return false;
        }

        switch (event)
        {
            case json::parse_event_t::key:
            {
                return true;
            }
            case json::parse_event_t::value:
            {
                return false;
            }
            case json::parse_event_t::object_start:
            {
                return true;
            }
            case json::parse_event_t::object_end:
            {
                return false;
            }
            case json::parse_event_t::array_start:
            {
                return true;
            }
            case json::parse_event_t::array_end:
            {
                return false;
            }

            default:
            {
                return true;
            }
        }
    };

    auto j = json::parse(geojsonExample, cb, true);
    CHECK(j == json());
}

TEST(RegressionTests2Test, Issue1021ToFromMsgpackOnlyWorksWithStandardTypization)
{
    float_json j = 1000.0;
    CHECK(float_json::from_cbor(float_json::to_cbor(j)) == j);
    CHECK(float_json::from_msgpack(float_json::to_msgpack(j)) == j);
    CHECK(float_json::from_ubjson(float_json::to_ubjson(j)) == j);

    float_json j2 = {1000.0, 2000.0, 3000.0};
    CHECK(float_json::from_ubjson(float_json::to_ubjson(j2, true, true)) == j2);
}

TEST(RegressionTests2Test, Issue1045UsingSTLAlgorithmsWithJSONContainersWithExpectedResults)
{
    json diffs = wpi::json::array();
    json m1{{"key1", 42}};
    json m2{{"key2", 42}};
    auto p1 = m1.items();
    auto p2 = m2.items();

    using it_type = decltype(p1.begin());

    std::set_difference(
        p1.begin(),
        p1.end(),
        p2.begin(),
        p2.end(),
        std::inserter(diffs, diffs.end()),
        [&](const it_type & e1, const it_type & e2) -> bool
    {
        using comper_pair = std::pair<std::string, decltype(e1.value())>;              // Trying to avoid unneeded copy
        return comper_pair(e1.key(), e1.value()) < comper_pair(e2.key(), e2.value());  // Using pair comper
    });

    CHECK(diffs.size() == 1);  // Note the change here, was 2
}

#ifdef JSON_HAS_CPP_17

TEST(RegressionTests2Test, Issue1292SerializingStdVariantCausesStackOverflow)
{
    static_assert(!std::is_constructible<json, std::variant<int, float>>::value, "unexpected value");
}

#endif

TEST(RegressionTests2Test, Issue1299CompileErrorInFromJsonConvertingToContainerWithStdPair)
{
    json j =
    {
        {"1", {{"a", "testa_1"}, {"b", "testb_1"}}},
        {"2", {{"a", "testa_2"}, {"b", "testb_2"}}},
        {"3", {{"a", "testa_3"}, {"b", "testb_3"}}},
    };

    std::map<std::string, Data> expected
    {
        {"1", {"testa_1", "testb_1"}},
        {"2", {"testa_2", "testb_2"}},
        {"3", {"testa_3", "testb_3"}},
    };
    const auto data = j.get<decltype(expected)>();
    CHECK(expected == data);
}




TEST(RegressionTests2Issue1445BufferOverflowInDumpingInvalidUtf8StringsTest, ABunchOf1EnsureAsciiTrue)
{
    const auto length = 300;

    json dump_test;
    dump_test["1"] = std::string(length, -1);

    std::string expected = R"({"1":")";
    for (int i = 0; i < length; ++i)
    {
        expected += "\\ufffd";
    }
    expected += "\"}";

    auto s = dump_test.dump(-1, ' ', true, wpi::json::error_handler_t::replace);
    CHECK(s == expected);
}

TEST(RegressionTests2Issue1445BufferOverflowInDumpingInvalidUtf8StringsTest, ABunchOf2EnsureAsciiFalse)
{
    const auto length = 500;

    json dump_test;
    dump_test["1"] = std::string(length, -2);

    std::string expected = R"({"1":")";
    for (int i = 0; i < length; ++i)
    {
        expected += "\xEF\xBF\xBD";
    }
    expected += "\"}";

    auto s = dump_test.dump(-1, ' ', false, wpi::json::error_handler_t::replace);
    CHECK(s == expected);
}

TEST(RegressionTests2Issue1445BufferOverflowInDumpingInvalidUtf8StringsTest, TestCaseInIssue1445)
{
    wpi::json dump_test;
    const std::array<int, 108> data =
    {
        {109, 108, 103, 125, -122, -53, 115, 18, 3, 0, 102, 19, 1, 15, -110, 13, -3, -1, -81, 32, 2, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -80, 2, 0, 0, 96, -118, 46, -116, 46, 109, -84, -87, 108, 14, 109, -24, -83, 13, -18, -51, -83, -52, -115, 14, 6, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 3, 0, 0, 0, 35, -74, -73, 55, 57, -128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33, 0, 0, 0, -96, -54, -28, -26}
    };
    std::string s;
    for (int i : data)
    {
        s += static_cast<char>(i);
    }
    dump_test["1"] = s;
    dump_test.dump(-1, ' ', true, wpi::json::error_handler_t::replace);
}

TEST(RegressionTests2Test, Issue1447IntegerOverflowOSSFuzz12506)
{
    json j = json::parse("[-9223372036854775808]");
    CHECK(j.dump() == "[-9223372036854775808]");
}

TEST(RegressionTests2Test, Issue1708MinimumValueOfInt64CanBeOutputted)
{
    constexpr auto smallest = (std::numeric_limits<int64_t>::min)();
    json j = smallest;
    CHECK(j.dump() == std::to_string(smallest));
}

TEST(RegressionTests2Test, Issue1727ContainsWithNonConstLvalueJsonPointerPicksTheWrongOverload)
{
    json j = {{"root", {{"settings", {{"logging", true}}}}}};

    auto jptr1 = "/root/settings/logging"_json_pointer;
    auto jptr2 = json::json_pointer{"/root/settings/logging"};

    CHECK(j.contains(jptr1));
    CHECK(j.contains(jptr2));
}

TEST(RegressionTests2Test, Issue1647CompileErrorWhenDeserializingEnumIfBothNonDefaultFromJsonAndNonMemberOperatorEqualsExistsForOtherType)
{
    // does not compile on ICPC when targeting C++20
#if !(defined(__INTEL_COMPILER) && __cplusplus >= 202000)
    {
        json j;
        NonDefaultFromJsonStruct x(j);
        NonDefaultFromJsonStruct y;
        CHECK(x == y);
    }
#endif

    auto val = wpi::json("one").get<for_1647>();
    CHECK(val == for_1647::one);
    json j = val;
}




TEST(RegressionTests2Issue1715JsonFromCborDoesNotRespectAllowExceptionsFalseWhenInputIsStringLiteralTest, StringLiteral)
{
    json cbor = json::from_cbor("B", true, false);
    CHECK(cbor.is_discarded());
}

TEST(RegressionTests2Issue1715JsonFromCborDoesNotRespectAllowExceptionsFalseWhenInputIsStringLiteralTest, StringArray)
{
    const std::array<char, 2> input = {{'B', 0x00}};
    json cbor = json::from_cbor(input, true, false);
    CHECK(cbor.is_discarded());
}

TEST(RegressionTests2Issue1715JsonFromCborDoesNotRespectAllowExceptionsFalseWhenInputIsStringLiteralTest, StdString)
{
    json cbor = json::from_cbor(std::string("B"), true, false);
    CHECK(cbor.is_discarded());
}

TEST(RegressionTests2Test, Issue1805APairT1T2IsJsonConstructibleOnlyIfT1AndT2AreJsonConstructible)
{
    static_assert(!std::is_constructible<json, std::pair<std::string, NotSerializableData>>::value, "unexpected result");
    static_assert(!std::is_constructible<json, std::pair<NotSerializableData, std::string>>::value, "unexpected result");
    static_assert(std::is_constructible<json, std::pair<int, std::string>>::value, "unexpected result");
}

TEST(RegressionTests2Test, Issue1825ATupleArgsIsJsonConstructibleOnlyIfAllTInArgsAreJsonConstructible)
{
    static_assert(!std::is_constructible<json, std::tuple<std::string, NotSerializableData>>::value, "unexpected result");
    static_assert(!std::is_constructible<json, std::tuple<NotSerializableData, std::string>>::value, "unexpected result");
    static_assert(std::is_constructible<json, std::tuple<int, std::string>>::value, "unexpected result");
}

TEST(RegressionTests2Test, Issue1983JSONPatchDiffForOpAddFormationIsNotAsPerStandardRFC6902)
{
    const auto source = R"({ "foo": [ "1", "2" ] })"_json;
    const auto target = R"({"foo": [ "1", "2", "3" ]})"_json;
    const auto result = json::diff(source, target);
    CHECK(result.dump() == R"([{"op":"add","path":"/foo/-","value":"3"}])");
}

TEST(RegressionTests2Test, Issue2067CannotSerializeBinaryDataToTextJSON)
{
    const std::array<unsigned char, 23> data = {{0x81, 0xA4, 0x64, 0x61, 0x74, 0x61, 0xC4, 0x0F, 0x33, 0x30, 0x30, 0x32, 0x33, 0x34, 0x30, 0x31, 0x30, 0x37, 0x30, 0x35, 0x30, 0x31, 0x30}};
    json j = json::from_msgpack(data.data(), data.size());
    CHECK_NOTHROW(
        j.dump(4,                             // Indent
               ' ',                           // Indent char
               false,                         // Ensure ascii
               json::error_handler_t::strict  // Error
              ));
}

TEST(RegressionTests2Test, PR2181RegressionBugWithLvalue)
{
    // see https://github.com/nlohmann/json/pull/2181#issuecomment-653326060
    json j{{"x", "test"}};
    std::string defval = "default value";
    auto val = j.value("x", defval);
    auto val2 = j.value("y", defval);
}

TEST(RegressionTests2Test, Issue2293EofDoesntCauseParsingToStop)
{
    std::vector<uint8_t> data =
    {
        0x7B,
        0x6F,
        0x62,
        0x6A,
        0x65,
        0x63,
        0x74,
        0x20,
        0x4F,
        0x42
    };
    json result = json::from_cbor(data, true, false);
    CHECK(result.is_discarded());
}

TEST(RegressionTests2Test, Issue2315JsonUpdateAndVectorPairDoesNotWorkWithOrderedJson)
{
    wpi::ordered_json jsonAnimals = {{"animal", "dog"}};
    wpi::ordered_json jsonCat = {{"animal", "cat"}};
    jsonAnimals.update(jsonCat);
    CHECK(jsonAnimals["animal"] == "cat");

    auto jsonAnimals_parsed = wpi::ordered_json::parse(jsonAnimals.dump());
    CHECK(jsonAnimals == jsonAnimals_parsed);

    std::vector<std::pair<std::string, int64_t>> intData = {std::make_pair("aaaa", 11),
                                                            std::make_pair("bbb", 222)
                                                           };
    wpi::ordered_json jsonObj;
    for (const auto& data : intData)
    {
        jsonObj[data.first] = data.second;
    }
    CHECK(jsonObj["aaaa"] == 11);
    CHECK(jsonObj["bbb"] == 222);
}

TEST(RegressionTests2Test, Issue2330IgnoreCommentTrueFailsOnMultipleConsecutiveLinesStartingWithComments)
{
    std::string ss = "//\n//\n{\n}\n";
    json j = json::parse(ss, nullptr, true, true);
    CHECK(j.dump() == "{}");
}

#ifdef JSON_HAS_CPP_20

TEST(RegressionTests2Test, Issue2546ParsingContainersOfStdByte)
{
    const char DATA[] = R"("Hello, world!")"; // NOLINT(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    const auto s = std::as_bytes(std::span(DATA));
    json j = json::parse(s);
    CHECK(j.dump() == "\"Hello, world!\"");
}

#endif




TEST(RegressionTests2Issue2574DeserializationToStdArrayStdPairAndStdTupleWithNonDefaultConstructableTypesFailsTest, StdArray)
{
    {
        json j = {7, 4};
        auto arr = j.get<std::array<NonDefaultConstructible, 2>>();
        CHECK(arr[0].x == 7);
        CHECK(arr[1].x == 4);
    }

    {
        json j = 7;
        CHECK_THROWS_AS((j.get<std::array<NonDefaultConstructible, 1>>()), json::type_error);
    }
}

TEST(RegressionTests2Issue2574DeserializationToStdArrayStdPairAndStdTupleWithNonDefaultConstructableTypesFailsTest, StdPair)
{
    {
        json j = {3, 8};
        auto p = j.get<std::pair<NonDefaultConstructible, NonDefaultConstructible>>();
        CHECK(p.first.x == 3);
        CHECK(p.second.x == 8);
    }

    {
        json j = {4, 1};
        auto p = j.get<std::pair<int, NonDefaultConstructible>>();
        CHECK(p.first == 4);
        CHECK(p.second.x == 1);
    }

    {
        json j = {6, 7};
        auto p = j.get<std::pair<NonDefaultConstructible, int>>();
        CHECK(p.first.x == 6);
        CHECK(p.second == 7);
    }

    {
        json j = 7;
        CHECK_THROWS_AS((j.get<std::pair<NonDefaultConstructible, int>>()), json::type_error);
    }
}

TEST(RegressionTests2Issue2574DeserializationToStdArrayStdPairAndStdTupleWithNonDefaultConstructableTypesFailsTest, StdTuple)
{
    {
        json j = {9};
        auto t = j.get<std::tuple<NonDefaultConstructible>>();
        CHECK(std::get<0>(t).x == 9);
    }

    {
        json j = {9, 8, 7};
        auto t = j.get<std::tuple<NonDefaultConstructible, int, NonDefaultConstructible>>();
        CHECK(std::get<0>(t).x == 9);
        CHECK(std::get<1>(t) == 8);
        CHECK(std::get<2>(t).x == 7);
    }

    {
        json j = 7;
        CHECK_THROWS_AS((j.get<std::tuple<NonDefaultConstructible>>()), json::type_error);
    }
}

TEST(RegressionTests2Test, Issue2865ASANDetectsMemoryLeaks)
{
    // the code below is expected to not leak memory
    {
        wpi::json o;
        std::string s = "bar";

        wpi::to_json(o["foo"], s);

        wpi::json p = o;

        // call to_json with a non-null JSON value
        wpi::to_json(p["foo"], s);
    }

    {
        wpi::json o;
        std::string s = "bar";

        wpi::to_json(o["foo"], s);

        // call to_json with a non-null JSON value
        wpi::to_json(o["foo"], s);
    }
}

TEST(RegressionTests2Test, Issue2824EncodingOfJsonExceptionWhat)
{
    json j;
    sax_no_exception sax(j);

    CHECK(!json::sax_parse("xyz", &sax));
    CHECK(*sax_no_exception::error_string == "[json.exception.parse_error.101] parse error at line 1, column 1: syntax error while parsing value - invalid literal; last read: 'x'");
    delete sax_no_exception::error_string;  // NOLINT(cppcoreguidelines-owning-memory)
}

TEST(RegressionTests2Test, Issue2825ProperlyConstrainTheBasicJsonConversionOperator)
{
    static_assert(std::is_copy_assignable<wpi::ordered_json>::value, "ordered_json must be copy assignable");
}

TEST(RegressionTests2Test, Issue2958InsertingInUnorderedJsonUsingAPointerRetainsTheLeadingSlash)
{
    std::string p = "/root";

    json test1;
    test1[json::json_pointer(p)] = json::object();
    CHECK(test1.dump() == "{\"root\":{}}");

    ordered_json test2;
    test2[ordered_json::json_pointer(p)] = json::object();
    CHECK(test2.dump() == "{\"root\":{}}");

    // json::json_pointer and ordered_json::json_pointer are the same type; behave as above
    ordered_json test3;
    test3[json::json_pointer(p)] = json::object();
    CHECK(std::is_same<json::json_pointer::string_t, ordered_json::json_pointer::string_t>::value);
    CHECK(test3.dump() == "{\"root\":{}}");
}

TEST(RegressionTests2Test, Issue2982ToBinaryFormatDoesNotProvideAMechanismForSpecifyingACustomAllocatorForTheReturnedType)
{
    std::vector<std::uint8_t, my_allocator<std::uint8_t>> my_vector;
    json j = {1, 2, 3, 4};
    json::to_cbor(j, my_vector);
    json k = json::from_cbor(my_vector);
    CHECK(j == k);
}

#if JSON_HAS_FILESYSTEM || JSON_HAS_EXPERIMENTAL_FILESYSTEM

TEST(RegressionTests2Test, Issue3070Version3103BreaksBackwardCompatibilityWith3102)
{
    wpi::detail::std_fs::path text_path("/tmp/text.txt");
    json j(text_path);

    const auto j_path = j.get<wpi::detail::std_fs::path>();
    CHECK(j_path == text_path);

#if DOCTEST_CLANG || DOCTEST_GCC >= DOCTEST_COMPILER(8, 4, 0)
    // only known to work on Clang and GCC >=8.4
    CHECK_THROWS_WITH_AS(wpi::detail::std_fs::path(json(1)), "[json.exception.type_error.302] type must be string, but is number", json::type_error);
#endif
}

#endif

TEST(RegressionTests2Test, Issue3077ExplicitConstructorWithDefaultDoesNotCompile)
{
    json j;
    j[0]["value"] = true;
    std::vector<FooBar> foo;
    j.get_to(foo);
}

TEST(RegressionTests2Test, Issue3108OrderedJsonDoesntSupportRangeBasedErase)
{
    ordered_json j = {1, 2, 2, 4};

    auto last = std::unique(j.begin(), j.end());
    j.erase(last, j.end());

    CHECK(j.dump() == "[1,2,4]");

    j.erase(std::remove_if(j.begin(), j.end(), [](const ordered_json & val)
    {
        return val == 2;
    }), j.end());

    CHECK(j.dump() == "[1,4]");
}

TEST(RegressionTests2Test, Issue3343JsonAndOrderedJsonAreNotInterchangable)
{
    json::object_t jobj({ { "product", "one" } });
    ordered_json::object_t ojobj({{"product", "one"}});

    auto jit = jobj.begin();
    auto ojit = ojobj.begin();

    CHECK(jit->first == ojit->first);
    CHECK(jit->second.get<std::string>() == ojit->second.get<std::string>());
}

TEST(RegressionTests2Test, Issue3171IfClassIsConstructibleFromStdStringWrongFromJsonOverloadIsBeingSelectedCompilationFailed)
{
    json j{{ "str", "value"}};

    // failed with: error: no match for ‘operator=’ (operand types are ‘for_3171_derived’ and ‘const wpi::basic_json<>::string_t’
    //                                               {aka ‘const std::__cxx11::basic_string<char>’})
    //                  s = *j.template get_ptr<const typename BasicJsonType::string_t*>();
    auto td = j.get<for_3171_derived>();

    CHECK(td.str == "value");
}

#ifdef JSON_HAS_CPP_20

TEST(RegressionTests2Test, Issue3312ParseToCustomClassFromUnorderedJsonBreaksOnG1120WithC20)
{
    // see test for #3171
    ordered_json j = {{"name", "class"}};
    for_3312 obj{};

    j.get_to(obj);

    CHECK(obj.name == "class");
}

#endif

#if defined(JSON_HAS_CPP_17) && JSON_USE_IMPLICIT_CONVERSIONS

TEST(RegressionTests2Test, Issue3428ErrorOccurredWhenConvertingNlohmannJsonToStdAny)
{
    json j;
    std::any a1 = j;
    std::any&& a2 = j;

    CHECK(a1.type() == typeid(j));
    CHECK(a2.type() == typeid(j));
}

#endif

TEST(RegressionTests2Test, Issue3204AmbiguousRegression)
{
    for_3204_bar bar_from_foo([](for_3204_foo) noexcept {}); // NOLINT(performance-unnecessary-value-param)
    for_3204_bar bar_from_json([](json) noexcept {}); // NOLINT(performance-unnecessary-value-param)

    CHECK(bar_from_foo.constructed_from == for_3204_bar::constructed_from_foo);
    CHECK(bar_from_json.constructed_from == for_3204_bar::constructed_from_json);
}

TEST(RegressionTests2Test, Issue3333AmbiguousConversionFromNlohmannBasicJsonToCustomClass)
{
    const json j
    {
        {"x", 1},
        {"y", 2}
    };
    for_3333 p = j;

    CHECK(p.x == 1);
    CHECK(p.y == 2);
}


