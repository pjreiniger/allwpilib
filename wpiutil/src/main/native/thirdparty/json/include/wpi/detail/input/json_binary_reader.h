#pragma once

#include <cstddef> // size_t
#include <string> // char_traits, string

#include "wpi/json.h"
#include "wpi/detail/input/json_sax.h"
#include "wpi/detail/json_macro_scope.h"
#include "wpi/detail/meta/json_is_sax.h"
#include "wpi/detail/json_value_t.h"

namespace wpi
{
class json;
class raw_istream;
}

namespace wpi
{
///////////////////
// binary reader //
///////////////////

/*!
@brief deserialization of CBOR, MessagePack, and UBJSON values
*/
class json::binary_reader
{
    using SAX = detail::json_sax_dom_parser;
    using json_sax_t = SAX;

  public:
    /*!
    @brief create a binary reader

    @param[in] adapter  input adapter to read from
    */
    explicit binary_reader(raw_istream& s) : is(s)
    {
    }

    /*!
    @param[in] format  the binary format to parse
    @param[in] sax_    a SAX event processor
    @param[in] strict  whether to expect the input to be consumed completed

    @return
    */
    bool sax_parse(const input_format_t format,
                   json_sax_t* sax_,
                   const bool strict = true);

    /*!
    @brief determine system byte order

    @return true if and only if system's byte order is little endian

    @note from http://stackoverflow.com/a/1001328/266378
    */
    static bool little_endianess(int num = 1) noexcept
    {
        return (*reinterpret_cast<char*>(&num) == 1);
    }

  private:
    /*!
    @param[in] get_char  whether a new character should be retrieved from the
                         input (true, default) or whether the last read
                         character should be considered instead

    @return whether a valid CBOR value was passed to the SAX parser
    */
    bool parse_cbor_internal(const bool get_char = true);

    /*!
    @return whether a valid MessagePack value was passed to the SAX parser
    */
    bool parse_msgpack_internal();

    /*!
    @param[in] get_char  whether a new character should be retrieved from the
                         input (true, default) or whether the last read
                         character should be considered instead

    @return whether a valid UBJSON value was passed to the SAX parser
    */
    bool parse_ubjson_internal(const bool get_char = true)
    {
        return get_ubjson_value(get_char ? get_ignore_noop() : current);
    }

    /*!
    @brief get next character from the input

    This function provides the interface to the used input adapter. It does
    not throw in case the input reached EOF, but returns a -'ve valued
    `std::char_traits<char>::eof()` in that case.

    @return character read from the input
    */
    int get();

    /*!
    @return character read from the input after ignoring all 'N' entries
    */
    int get_ignore_noop();

    /*
    @brief read a number from the input

    @tparam NumberType the type of the number
    @param[out] result  number of type @a NumberType

    @return whether conversion completed

    @note This function needs to respect the system's endianess, because
          bytes in CBOR, MessagePack, and UBJSON are stored in network order
          (big endian) and therefore need reordering on little endian systems.
    */
    template<typename NumberType>
    bool get_number(NumberType& result);

    /*!
    @brief create a string by reading characters from the input

    @tparam NumberType the type of the number
    @param[in] len number of characters to read
    @param[out] string created by reading @a len bytes

    @return whether string creation completed

    @note We can not reserve @a len bytes for the result, because @a len
          may be too large. Usually, @ref unexpect_eof() detects the end of
          the input before we run out of string memory.
    */
    template<typename NumberType>
    bool get_string(const NumberType len, std::string& result);

    /*!
    @brief reads a CBOR string

    This function first reads starting bytes to determine the expected
    string length and then copies this number of bytes into a string.
    Additionally, CBOR's strings with indefinite lengths are supported.

    @param[out] result  created string

    @return whether string creation completed
    */
    bool get_cbor_string(std::string& result);

    /*!
    @param[in] len  the length of the array or std::size_t(-1) for an
                    array of indefinite size
    @return whether array creation completed
    */
    bool get_cbor_array(const std::size_t len);

    /*!
    @param[in] len  the length of the object or std::size_t(-1) for an
                    object of indefinite size
    @return whether object creation completed
    */
    bool get_cbor_object(const std::size_t len);

    /*!
    @brief reads a MessagePack string

    This function first reads starting bytes to determine the expected
    string length and then copies this number of bytes into a string.

    @param[out] result  created string

    @return whether string creation completed
    */
    bool get_msgpack_string(std::string& result);

    /*!
    @param[in] len  the length of the array
    @return whether array creation completed
    */
    bool get_msgpack_array(const std::size_t len);

    /*!
    @param[in] len  the length of the object
    @return whether object creation completed
    */
    bool get_msgpack_object(const std::size_t len);

    /*!
    @brief reads a UBJSON string

    This function is either called after reading the 'S' byte explicitly
    indicating a string, or in case of an object key where the 'S' byte can be
    left out.

    @param[out] result   created string
    @param[in] get_char  whether a new character should be retrieved from the
                         input (true, default) or whether the last read
                         character should be considered instead

    @return whether string creation completed
    */
    bool get_ubjson_string(std::string& result, const bool get_char = true);

    /*!
    @param[out] result  determined size
    @return whether size determination completed
    */
    bool get_ubjson_size_value(std::size_t& result);

    /*!
    @brief determine the type and size for a container

    In the optimized UBJSON format, a type and a size can be provided to allow
    for a more compact representation.

    @param[out] result  pair of the size and the type

    @return whether pair creation completed
    */
    bool get_ubjson_size_type(std::pair<std::size_t, int>& result);

    /*!
    @param prefix  the previously read or set type prefix
    @return whether value creation completed
    */
    bool get_ubjson_value(const int prefix);

    /*!
    @return whether array creation completed
    */
    bool get_ubjson_array();

    /*!
    @return whether object creation completed
    */
    bool get_ubjson_object();

    /*!
    @return whether the last read character is not EOF
    */
    bool unexpect_eof() const;

    /*!
    @return a string representation of the last read byte
    */
    std::string get_token_string() const;

  private:
    /// input adapter
    raw_istream& is;

    /// the current character
    int current = std::char_traits<char>::eof();

    /// the number of characters read
    std::size_t chars_read = 0;

    /// whether we can assume little endianess
    const bool is_little_endian = little_endianess();

    /// the SAX parser
    json_sax_t* sax = nullptr;
};
}
