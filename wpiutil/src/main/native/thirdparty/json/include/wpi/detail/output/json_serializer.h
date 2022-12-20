#pragma once

#include <algorithm> // reverse, remove, fill, find, none_of
#include <array> // array
#include <cassert> // assert
#include <cmath> // labs, isfinite, isnan, signbit
#include <cstddef> // size_t, ptrdiff_t
#include <cstdint> // uint8_t
#include <string> // string
#include <type_traits> // is_same

#include "wpi/detail/json_macro_scope.h"
#include "wpi/detail/json_meta.h"
#include "wpi/json.h"

#include "wpi/raw_ostream.h"

namespace wpi
{
///////////////////
// serialization //
///////////////////

class json::serializer
{
    static constexpr uint8_t UTF8_ACCEPT = 0;
    static constexpr uint8_t UTF8_REJECT = 1;

  public:
    /*!
    @param[in] s  output stream to serialize to
    @param[in] ichar  indentation character to use
    @param[in] indent_init_len  initial length of indentation string buffer
    */
    serializer(raw_ostream& s, const char ichar, size_t indent_init_len = 512)
        : o(s), indent_char(ichar),
          indent_string(indent_init_len, indent_char)
    {}

    // delete because of pointer members
    serializer(const serializer&) = delete;
    serializer& operator=(const serializer&) = delete;

    /*!
    @brief internal implementation of the serialization function

    This function is called by the public member function dump and organizes
    the serialization internally. The indentation level is propagated as
    additional parameter. In case of arrays and objects, the function is
    called recursively.

    - strings and object keys are escaped using `escape_string()`
    - integer numbers are converted implicitly via `operator<<`
    - floating-point numbers are converted to a string using `"%g"` format

    @param[in] val             value to serialize
    @param[in] pretty_print    whether the output shall be pretty-printed
    @param[in] ensure_ascii    whether the output shall only use ASCII chars
    @param[in] indent_step     the indent level
    @param[in] current_indent  the current indent level (only used internally)
    */
    void dump(const json& val, const bool pretty_print,
              const bool ensure_ascii,
              const unsigned int indent_step,
              const unsigned int current_indent = 0);

    /*!
    @brief dump escaped string

    Escape a string by replacing certain special characters by a sequence of an
    escape character (backslash) and another character and other control
    characters by a sequence of "\u" followed by a four-digit hex
    representation. The escaped string is written to output stream @a o.

    @param[in] s  the string to escape
    @param[in] ensure_ascii  whether to escape non-ASCII characters with
                             "\uXXXX" sequences

    Complexity: Linear in the length of string @a s.
    */
    void dump_escaped(std::string_view s, const bool ensure_ascii);

    template <typename NumberType,
              detail::enable_if_t<std::is_same_v<NumberType, uint64_t>, int> = 0>
    bool is_negative_integer(NumberType x) {
      return false;
    }

    template <typename NumberType,
              detail::enable_if_t<std::is_same_v<NumberType, int64_t>, int> = 0>
    bool is_negative_integer(NumberType x) {
      return x < 0;
    }

    /*!
    @brief dump an integer

    Dump a given integer to output stream @a o. Works internally with
    @a number_buffer.

    @param[in] x  integer number (signed or unsigned) to dump
    @tparam NumberType either @a int64_t or @a uint64_t
    */
    template<typename NumberType, detail::enable_if_t<
                 std::is_same<NumberType, uint64_t>::value or
                 std::is_same<NumberType, int64_t>::value,
                 int> = 0>
    void dump_integer(NumberType x)
    {
        // special case for "0"
        if (x == 0)
        {
            o << '0';
            return;
        }

        const bool is_negative = is_negative_integer(x);  // see issue #755
        std::size_t i = 0;

        while (x != 0)
        {
            // spare 1 byte for '\0'
            assert(i < number_buffer.size() - 1);

            const auto digit = std::labs(static_cast<long>(x % 10));
            number_buffer[i++] = static_cast<char>('0' + digit);
            x /= 10;
        }

        if (is_negative)
        {
            // make sure there is capacity for the '-'
            assert(i < number_buffer.size() - 2);
            number_buffer[i++] = '-';
        }

        std::reverse(number_buffer.begin(), number_buffer.begin() + i);
        o.write(number_buffer.data(), i);
    }

    /*!
    @brief dump a floating-point number

    Dump a given floating-point number to output stream @a o. Works internally
    with @a number_buffer.

    @param[in] x  floating-point number to dump
    */
    void dump_float(double x);

    /*!
    @brief check whether a string is UTF-8 encoded

    The function checks each byte of a string whether it is UTF-8 encoded. The
    result of the check is stored in the @a state parameter. The function must
    be called initially with state 0 (accept). State 1 means the string must
    be rejected, because the current byte is not allowed. If the string is
    completely processed, but the state is non-zero, the string ended
    prematurely; that is, the last byte indicated more bytes should have
    followed.

    @param[in,out] state  the state of the decoding
    @param[in,out] codep  codepoint (valid only if resulting state is UTF8_ACCEPT)
    @param[in] byte       next byte to decode
    @return               new state

    @note The function has been edited: a std::array is used.

    @copyright Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
    @sa http://bjoern.hoehrmann.de/utf-8/decoder/dfa/
    */
    static uint8_t decode(uint8_t& state, uint32_t& codep, const uint8_t byte) noexcept;

  private:
    /// the output of the serializer
    raw_ostream& o;

    /// a (hopefully) large enough character buffer
    std::array<char, 64> number_buffer{{}};

    /// the indentation character
    const char indent_char;
    /// the indentation string
    std::string indent_string;
};
}
