#pragma once

#include <ciso646> // or, and, not
#include <iterator> // begin, end
#include <tuple> // tuple, get
#include <type_traits> // is_same, is_constructible, is_floating_point, is_enum, underlying_type
#include <utility> // move, forward, declval, pair
#include <valarray> // valarray
#include <vector> // vector

#include "wpi/detail/meta/json_cpp_future.h"
#include "wpi/detail/meta/json_type_traits.h"
#include "wpi/detail/json_value_t.h"
#include "wpi/detail/iterators/json_iteration_proxy.h"

namespace wpi
{
namespace detail
{
//////////////////
// constructors //
//////////////////

template<value_t> struct external_constructor;

template<>
struct external_constructor<value_t::boolean>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, bool b) noexcept
    {
        j.m_type = value_t::boolean;
        j.m_value = b;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::string>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, std::string_view s)
    {
        j.m_type = value_t::string;
        j.m_value = s;
        j.assert_invariant();
    }

    template<typename BasicJsonType, typename T,
             enable_if_t<std::is_same<std::string, T>::value, int> = 0>
    static void construct(BasicJsonType& j, T&& s)
    {
        j.m_type = value_t::string;
        j.m_value = std::move(s);
        j.assert_invariant();
    }

    template<typename BasicJsonType, typename CompatibleStringType,
             enable_if_t<not std::is_same<CompatibleStringType, std::string>::value,
                         int> = 0>
    static void construct(BasicJsonType& j, const CompatibleStringType& str)
    {
        j.m_type = value_t::string;
        j.m_value.string = j.template create<std::string>(str);
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::number_float>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, double val) noexcept
    {
        j.m_type = value_t::number_float;
        j.m_value = val;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::number_unsigned>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, uint64_t val) noexcept
    {
        j.m_type = value_t::number_unsigned;
        j.m_value = val;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::number_integer>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, int64_t val) noexcept
    {
        j.m_type = value_t::number_integer;
        j.m_value = val;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::array>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const typename BasicJsonType::array_t& arr)
    {
        j.m_type = value_t::array;
        j.m_value = arr;
        j.assert_invariant();
    }

    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::array_t&& arr)
    {
        j.m_type = value_t::array;
        j.m_value = std::move(arr);
        j.assert_invariant();
    }

    template<typename BasicJsonType, typename T>
    static void construct(BasicJsonType& j, std::span<T> arr)
    {
        using std::begin;
        using std::end;
        j.m_type = value_t::array;
        j.m_value.array = j.template create<typename BasicJsonType::array_t>(begin(arr), end(arr));
        j.assert_invariant();
    }

    template<typename BasicJsonType, typename CompatibleArrayType,
             enable_if_t<not std::is_same<CompatibleArrayType, typename BasicJsonType::array_t>::value,
                         int> = 0>
    static void construct(BasicJsonType& j, const CompatibleArrayType& arr)
    {
        using std::begin;
        using std::end;
        j.m_type = value_t::array;
        j.m_value.array = j.template create<typename BasicJsonType::array_t>(begin(arr), end(arr));
        j.assert_invariant();
    }

    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const std::vector<bool>& arr)
    {
        j.m_type = value_t::array;
        j.m_value = value_t::array;
        j.m_value.array->reserve(arr.size());
        for (const bool x : arr)
        {
            j.m_value.array->push_back(x);
        }
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::object>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const typename BasicJsonType::object_t& obj)
    {
        j.m_type = value_t::object;
        j.m_value = obj;
        j.assert_invariant();
    }

    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::object_t&& obj)
    {
        j.m_type = value_t::object;
        j.m_value = std::move(obj);
        j.assert_invariant();
    }

    template<typename BasicJsonType, typename CompatibleObjectType,
             enable_if_t<not std::is_same<CompatibleObjectType, typename BasicJsonType::object_t>::value, int> = 0>
    static void construct(BasicJsonType& j, const CompatibleObjectType& obj)
    {
        j.m_type = value_t::object;
        j.m_value = value_t::object;
        for (const auto& x : obj)
        {
            j.m_value.object->try_emplace(x.first, x.second);
        }
        j.assert_invariant();
    }
};

/////////////
// to_json //
/////////////

template<typename BasicJsonType, typename T,
         enable_if_t<std::is_same<T, bool>::value, int> = 0>
void to_json(BasicJsonType& j, T b) noexcept
{
    external_constructor<value_t::boolean>::construct(j, b);
}

template<typename BasicJsonType, typename CompatibleString,
         enable_if_t<std::is_constructible<std::string_view, CompatibleString>::value, int> = 0>
void to_json(BasicJsonType& j, const CompatibleString& s)
{
    external_constructor<value_t::string>::construct(j, s);
}

template<typename BasicJsonType, typename T,
         enable_if_t<std::is_same<std::string, T>::value, int> = 0>
void to_json(BasicJsonType& j, T&& s)
{
    external_constructor<value_t::string>::construct(j, std::move(s));
}

template<typename BasicJsonType, typename FloatType,
         enable_if_t<std::is_floating_point<FloatType>::value, int> = 0>
void to_json(BasicJsonType& j, FloatType val) noexcept
{
    external_constructor<value_t::number_float>::construct(j, static_cast<double>(val));
}

template<typename BasicJsonType, typename CompatibleNumberUnsignedType,
         enable_if_t<is_compatible_integer_type<uint64_t, CompatibleNumberUnsignedType>::value, int> = 0>
void to_json(BasicJsonType& j, CompatibleNumberUnsignedType val) noexcept
{
    external_constructor<value_t::number_unsigned>::construct(j, static_cast<uint64_t>(val));
}

template<typename BasicJsonType, typename CompatibleNumberIntegerType,
         enable_if_t<is_compatible_integer_type<int64_t, CompatibleNumberIntegerType>::value, int> = 0>
void to_json(BasicJsonType& j, CompatibleNumberIntegerType val) noexcept
{
    external_constructor<value_t::number_integer>::construct(j, static_cast<int64_t>(val));
}

template<typename BasicJsonType, typename EnumType,
         enable_if_t<std::is_enum<EnumType>::value, int> = 0>
void to_json(BasicJsonType& j, EnumType e) noexcept
{
    using underlying_type = typename std::underlying_type<EnumType>::type;
    external_constructor<value_t::number_integer>::construct(j, static_cast<underlying_type>(e));
}

template<typename BasicJsonType>
void to_json(BasicJsonType& j, const std::vector<bool>& e)
{
    external_constructor<value_t::array>::construct(j, e);
}

template <typename BasicJsonType, typename CompatibleArrayType,
          enable_if_t<is_compatible_array_type<BasicJsonType,
                      CompatibleArrayType>::value and
                      not is_compatible_object_type<
                          BasicJsonType, CompatibleArrayType>::value and
                      not is_compatible_string_type<BasicJsonType, CompatibleArrayType>::value and
                      not is_json<CompatibleArrayType>::value,
                      int> = 0>
void to_json(BasicJsonType& j, const CompatibleArrayType& arr)
{
    external_constructor<value_t::array>::construct(j, arr);
}

template<typename BasicJsonType>
void to_json(BasicJsonType& j, typename BasicJsonType::array_t&& arr)
{
    external_constructor<value_t::array>::construct(j, std::move(arr));
}

template<typename BasicJsonType, typename CompatibleObjectType,
         enable_if_t<is_compatible_object_type<BasicJsonType, CompatibleObjectType>::value and not is_json<CompatibleObjectType>::value, int> = 0>
void to_json(BasicJsonType& j, const CompatibleObjectType& obj)
{
    external_constructor<value_t::object>::construct(j, obj);
}

template<typename BasicJsonType>
void to_json(BasicJsonType& j, typename BasicJsonType::object_t&& obj)
{
    external_constructor<value_t::object>::construct(j, std::move(obj));
}

template <
    typename BasicJsonType, typename T, std::size_t N,
    enable_if_t<not std::is_constructible<std::string_view,
                const T (&)[N]>::value,
                int> = 0 >
void to_json(BasicJsonType& j, const T (&arr)[N])
{
    external_constructor<value_t::array>::construct(j, arr);
}

template<typename BasicJsonType, typename... Args>
void to_json(BasicJsonType& j, const std::pair<Args...>& p)
{
    j = {p.first, p.second};
}

// for https://github.com/nlohmann/json/pull/1134
template<typename BasicJsonType, typename T,
         enable_if_t<std::is_same<T, typename iteration_proxy<typename BasicJsonType::iterator>::iteration_proxy_internal>::value, int> = 0>
void to_json(BasicJsonType& j, T b) noexcept
{
    j = {{b.key(), b.value()}};
}

template<typename BasicJsonType, typename Tuple, std::size_t... Idx>
void to_json_tuple_impl(BasicJsonType& j, const Tuple& t, std::index_sequence<Idx...>)
{
    j = {std::get<Idx>(t)...};
}

template<typename BasicJsonType, typename... Args>
void to_json(BasicJsonType& j, const std::tuple<Args...>& t)
{
    to_json_tuple_impl(j, t, std::index_sequence_for<Args...> {});
}

struct to_json_fn
{
    template<typename BasicJsonType, typename T>
    auto operator()(BasicJsonType& j, T&& val) const noexcept(noexcept(to_json(j, std::forward<T>(val))))
    -> decltype(to_json(j, std::forward<T>(val)), void())
    {
        return to_json(j, std::forward<T>(val));
    }
};
}

// namespace to hold default `to_json` function
namespace
{
constexpr const auto& to_json = detail::static_const<detail::to_json_fn>::value;
}
}
