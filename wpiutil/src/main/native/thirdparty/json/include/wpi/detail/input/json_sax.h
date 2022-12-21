#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "wpi/detail/input/json_parser.h"
#include "wpi/detail/json_exceptions.h"
#include "wpi/json.h"

namespace wpi
{

/*!
@brief SAX interface

This class describes the SAX interface used by @ref wpi::json::sax_parse.
Each function is called in different situations while the input is parsed. The
boolean return value informs the parser whether to continue processing the
input.
*/
struct json_sax
{
    /// type for (signed) integers
    /// type for unsigned integers
    /// type for floating-point numbers
    /// type for strings

    /*!
    @brief a null value was read
    @return whether parsing should proceed
    */
    virtual bool null() = 0;

    /*!
    @brief a boolean value was read
    @param[in] val  boolean value
    @return whether parsing should proceed
    */
    virtual bool boolean(bool val) = 0;

    /*!
    @brief an integer number was read
    @param[in] val  integer value
    @return whether parsing should proceed
    */
    virtual bool number_integer(int64_t val) = 0;

    /*!
    @brief an unsigned integer number was read
    @param[in] val  unsigned integer value
    @return whether parsing should proceed
    */
    virtual bool number_unsigned(uint64_t val) = 0;

    /*!
    @brief an floating-point number was read
    @param[in] val  floating-point value
    @param[in] s    raw token value
    @return whether parsing should proceed
    */
    virtual bool number_float(double val, std::string_view s) = 0;

    /*!
    @brief a string was read
    @param[in] val  string value
    @return whether parsing should proceed
    @note It is safe to move the passed string.
    */
    virtual bool string(std::string_view val) = 0;

    /*!
    @brief the beginning of an object was read
    @param[in] elements  number of object elements or -1 if unknown
    @return whether parsing should proceed
    @note binary formats may report the number of elements
    */
    virtual bool start_object(std::size_t elements) = 0;

    /*!
    @brief an object key was read
    @param[in] val  object key
    @return whether parsing should proceed
    @note It is safe to move the passed string.
    */
    virtual bool key(std::string_view val) = 0;

    /*!
    @brief the end of an object was read
    @return whether parsing should proceed
    */
    virtual bool end_object() = 0;

    /*!
    @brief the beginning of an array was read
    @param[in] elements  number of array elements or -1 if unknown
    @return whether parsing should proceed
    @note binary formats may report the number of elements
    */
    virtual bool start_array(std::size_t elements) = 0;

    /*!
    @brief the end of an array was read
    @return whether parsing should proceed
    */
    virtual bool end_array() = 0;

    /*!
    @brief a parse error occurred
    @param[in] position    the position in the input where the error occurs
    @param[in] last_token  the last read token
    @param[in] error_msg   a detailed error message
    @return whether parsing should proceed (must return false)
    */
    virtual bool parse_error(std::size_t position,
                             const std::string& last_token,
                             const detail::exception& ex) = 0;

    virtual ~json_sax() = default;
};


namespace detail
{
/*!
@brief SAX implementation to create a JSON value from SAX events

This class implements the @ref json_sax interface and processes the SAX events
to create a JSON value which makes it basically a DOM parser. The structure or
hierarchy of the JSON value is managed by the stack `ref_stack` which contains
a pointer to the respective array or object for each recursion depth.

After successful parsing, the value that is passed by reference to the
constructor contains the parsed value.

@tparam json  the JSON type
*/
class json_sax_dom_parser
{
  public:

    /*!
    @param[in, out] r  reference to a JSON value that is manipulated while
                       parsing
    @param[in] allow_exceptions_  whether parse errors yield exceptions
    */
    explicit json_sax_dom_parser(json& r, const bool allow_exceptions_ = true)
        : root(r), allow_exceptions(allow_exceptions_)
    {}

    bool null()
    {
        handle_value(nullptr);
        return true;
    }

    bool boolean(bool val)
    {
        handle_value(val);
        return true;
    }

    bool number_integer(int64_t val)
    {
        handle_value(val);
        return true;
    }

    bool number_unsigned(uint64_t val)
    {
        handle_value(val);
        return true;
    }

    bool number_float(double val, std::string_view)
    {
        handle_value(val);
        return true;
    }

    bool string(std::string_view val)
    {
        handle_value(val);
        return true;
    }

    bool start_object(std::size_t len);

    bool key(std::string_view val)
    {
        // add null at given key and store the reference for later
        object_element = &(ref_stack.back()->m_value.object->operator[](val));
        return true;
    }

    bool end_object()
    {
        ref_stack.pop_back();
        return true;
    }

    bool start_array(std::size_t len);

    bool end_array()
    {
        ref_stack.pop_back();
        return true;
    }

    bool parse_error(std::size_t, const std::string&,
                     const detail::exception& ex);

    constexpr bool is_errored() const
    {
        return errored;
    }

  private:
    /*!
    @invariant If the ref stack is empty, then the passed value will be the new
               root.
    @invariant If the ref stack contains a value, then it is an array or an
               object to which we can add elements
    */
    template<typename Value>
    json* handle_value(Value&& v)
    {
        if (ref_stack.empty())
        {
            root = json(std::forward<Value>(v));
            return &root;
        }
        else
        {
            assert(ref_stack.back()->is_array() or ref_stack.back()->is_object());
            if (ref_stack.back()->is_array())
            {
                ref_stack.back()->m_value.array->emplace_back(std::forward<Value>(v));
                return &(ref_stack.back()->m_value.array->back());
            }
            else
            {
                assert(object_element);
                *object_element = json(std::forward<Value>(v));
                return object_element;
            }
        }
    }

    /// the parsed JSON value
    json& root;
    /// stack to model hierarchy of values
    std::vector<json*> ref_stack;
    /// helper to hold the reference for the next object element
    json* object_element = nullptr;
    /// whether a syntax error occurred
    bool errored = false;
    /// whether to throw exceptions in case of errors
    const bool allow_exceptions = true;
};

class json_sax_dom_callback_parser
{
  public:
    using parser_callback_t = typename json::parser_callback_t;
    using parse_event_t = typename json::parse_event_t;

    json_sax_dom_callback_parser(json& r,
                                 const parser_callback_t cb,
                                 const bool allow_exceptions_ = true)
        : root(r), callback(cb), allow_exceptions(allow_exceptions_)
    {
        keep_stack.push_back(true);
    }

    bool null()
    {
        handle_value(nullptr);
        return true;
    }

    bool boolean(bool val)
    {
        handle_value(val);
        return true;
    }

    bool number_integer(int64_t val)
    {
        handle_value(val);
        return true;
    }

    bool number_unsigned(uint64_t val)
    {
        handle_value(val);
        return true;
    }

    bool number_float(double val, std::string_view)
    {
        handle_value(val);
        return true;
    }

    bool string(std::string_view val)
    {
        handle_value(val);
        return true;
    }

    bool start_object(std::size_t len);

    bool key(std::string_view val);

    bool end_object();

    bool start_array(std::size_t len);

    bool end_array();

    bool parse_error(std::size_t, const std::string&,
                     const detail::exception& ex);

    constexpr bool is_errored() const
    {
        return errored;
    }

  private:
    /*!
    @param[in] v  value to add to the JSON value we build during parsing
    @param[in] skip_callback  whether we should skip calling the callback
               function; this is required after start_array() and
               start_object() SAX events, because otherwise we would call the
               callback function with an empty array or object, respectively.

    @invariant If the ref stack is empty, then the passed value will be the new
               root.
    @invariant If the ref stack contains a value, then it is an array or an
               object to which we can add elements

    @return pair of boolean (whether value should be kept) and pointer (to the
            passed value in the ref_stack hierarchy; nullptr if not kept)
    */
    template<typename Value>
    std::pair<bool, json*> handle_value(Value&& v, const bool skip_callback = false)
    {
        assert(not keep_stack.empty());

        // do not handle this value if we know it would be added to a discarded
        // container
        if (not keep_stack.back())
        {
            return {false, nullptr};
        }

        // create value
        auto value = json(std::forward<Value>(v));

        // check callback
        const bool keep = skip_callback or callback(static_cast<int>(ref_stack.size()), parse_event_t::value, value);

        // do not handle this value if we just learnt it shall be discarded
        if (not keep)
        {
            return {false, nullptr};
        }

        if (ref_stack.empty())
        {
            root = std::move(value);
            return {true, &root};
        }
        else
        {
            // skip this value if we already decided to skip the parent
            // (https://github.com/nlohmann/json/issues/971#issuecomment-413678360)
            if (not ref_stack.back())
            {
                return {false, nullptr};
            }

            assert(ref_stack.back()->is_array() or ref_stack.back()->is_object());
            if (ref_stack.back()->is_array())
            {
                ref_stack.back()->m_value.array->push_back(std::move(value));
                return {true, &(ref_stack.back()->m_value.array->back())};
            }
            else
            {
                // check if we should store an element for the current key
                assert(not key_keep_stack.empty());
                const bool store_element = key_keep_stack.back();
                key_keep_stack.pop_back();

                if (not store_element)
                {
                    return {false, nullptr};
                }

                assert(object_element);
                *object_element = std::move(value);
                return {true, object_element};
            }
        }
    }

    /// the parsed JSON value
    json& root;
    /// stack to model hierarchy of values
    std::vector<json*> ref_stack;
    /// stack to manage which values to keep
    std::vector<bool> keep_stack;
    /// stack to manage which object keys to keep
    std::vector<bool> key_keep_stack;
    /// helper to hold the reference for the next object element
    json* object_element = nullptr;
    /// whether a syntax error occurred
    bool errored = false;
    /// callback function
    const parser_callback_t callback = nullptr;
    /// whether to throw exceptions in case of errors
    const bool allow_exceptions = true;
    /// a discarded value for the callback
    json discarded = json::value_t::discarded;
};

class json_sax_acceptor
{
  public:

    bool null()
    {
        return true;
    }

    bool boolean(bool)
    {
        return true;
    }

    bool number_integer(int64_t)
    {
        return true;
    }

    bool number_unsigned(uint64_t)
    {
        return true;
    }

    bool number_float(double, std::string_view)
    {
        return true;
    }

    bool string(std::string_view)
    {
        return true;
    }

    bool start_object(std::size_t = std::size_t(-1))
    {
        return true;
    }

    bool key(std::string_view)
    {
        return true;
    }

    bool end_object()
    {
        return true;
    }

    bool start_array(std::size_t = std::size_t(-1))
    {
        return true;
    }

    bool end_array()
    {
        return true;
    }

    bool parse_error(std::size_t, const std::string&, const detail::exception&)
    {
        return false;
    }
};
}

}
