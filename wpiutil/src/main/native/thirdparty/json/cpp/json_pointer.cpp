#define WPI_JSON_IMPLEMENTATION
#include "wpi/detail/json_pointer.h"

#include "wpi/json.h"

#include <numeric> // accumulate

#include "fmt/format.h"
#include "wpi/SmallString.h"
#include "wpi/StringExtras.h"

namespace wpi
{

std::string json_pointer::to_string() const noexcept
{
    return std::accumulate(reference_tokens.begin(), reference_tokens.end(),
                           std::string{},
                           [](const std::string & a, const std::string & b)
    {
        return a + "/" + escape(b);
    });
}

int json_pointer::array_index(std::string_view s)
{
    SmallString<128> str{s};
    std::size_t processed_chars = 0;
    const int res = std::stoi(str.c_str(), &processed_chars);

    // check if the string was completely read
    if (JSON_UNLIKELY(processed_chars != str.size()))
    {
        JSON_THROW(detail::out_of_range::create(404, fmt::format("unresolved reference token '{}'", s)));
    }

    return res;
}

std::string json_pointer::pop_back()
{
    if (JSON_UNLIKELY(is_root()))
    {
        JSON_THROW(detail::out_of_range::create(405, "JSON pointer has no parent"));
    }

    auto last = reference_tokens.back();
    reference_tokens.pop_back();
    return last;
}

json_pointer json_pointer::top() const
{
    if (JSON_UNLIKELY(is_root()))
    {
        JSON_THROW(detail::out_of_range::create(405, "JSON pointer has no parent"));
    }

    json_pointer result = *this;
    result.reference_tokens = {reference_tokens[0]};
    return result;
}

json& json_pointer::get_and_create(json& j) const
{
    using size_type = typename json::size_type;
    auto result = &j;

    // in case no reference tokens exist, return a reference to the JSON value
    // j which will be overwritten by a primitive value
    for (const auto& reference_token : reference_tokens)
    {
        switch (result->m_type)
        {
            case detail::value_t::null:
            {
                if (reference_token == "0")
                {
                    // start a new array if reference token is 0
                    result = &result->operator[](0);
                }
                else
                {
                    // start a new object otherwise
                    result = &result->operator[](reference_token);
                }
                break;
            }

            case detail::value_t::object:
            {
                // create an entry in the object
                result = &result->operator[](reference_token);
                break;
            }

            case detail::value_t::array:
            {
                // create an entry in the array
                JSON_TRY
                {
                    result = &result->operator[](static_cast<size_type>(array_index(reference_token)));
                }
                JSON_CATCH(std::invalid_argument&)
                {
                    JSON_THROW(detail::parse_error::create(109, 0, fmt::format("array index '{}' is not a number", reference_token)));
                }
                break;
            }

            /*
            The following code is only reached if there exists a reference
            token _and_ the current value is primitive. In this case, we have
            an error situation, because primitive values may only occur as
            single value; that is, with an empty list of reference tokens.
            */
            default:
                JSON_THROW(detail::type_error::create(313, "invalid value to unflatten"));
        }
    }

    return *result;
}

json& json_pointer::get_unchecked(json* ptr) const
{
    using size_type = typename json::size_type;
    for (const auto& reference_token : reference_tokens)
    {
        // convert null values to arrays or objects before continuing
        if (ptr->m_type == detail::value_t::null)
        {
            // check if reference token is a number
            const bool nums =
                std::all_of(reference_token.begin(), reference_token.end(),
                            [](const char x)
            {
                return (x >= '0' and x <= '9');
            });

            // change value to array for numbers or "-" or to object otherwise
            *ptr = (nums or reference_token == "-")
                   ? detail::value_t::array
                   : detail::value_t::object;
        }

        switch (ptr->m_type)
        {
            case detail::value_t::object:
            {
                // use unchecked object access
                ptr = &ptr->operator[](reference_token);
                break;
            }

            case detail::value_t::array:
            {
                // error condition (cf. RFC 6901, Sect. 4)
                if (JSON_UNLIKELY(reference_token.size() > 1 and reference_token[0] == '0'))
                {
                    JSON_THROW(detail::parse_error::create(106, 0,
                                            fmt::format("array index '{}' must not begin with '0'", reference_token)));
                }

                if (reference_token == "-")
                {
                    // explicitly treat "-" as index beyond the end
                    ptr = &ptr->operator[](ptr->m_value.array->size());
                }
                else
                {
                    // convert array index to number; unchecked access
                    JSON_TRY
                    {
                        ptr = &ptr->operator[](
                            static_cast<size_type>(array_index(reference_token)));
                    }
                    JSON_CATCH(std::invalid_argument&)
                    {
                        JSON_THROW(detail::parse_error::create(109, 0,
                                                    fmt::format("array index '{}' is not a number", reference_token)));
                    }
                }
                break;
            }

            default:
                JSON_THROW(detail::out_of_range::create(404,
                                    fmt::format("unresolved reference token '{}'", reference_token)));
        }
    }

    return *ptr;
}

json& json_pointer::get_checked(json* ptr) const
{
    using size_type = typename json::size_type;
    for (const auto& reference_token : reference_tokens)
    {
        switch (ptr->m_type)
        {
            case detail::value_t::object:
            {
                // note: at performs range check
                ptr = &ptr->at(reference_token);
                break;
            }

            case detail::value_t::array:
            {
                if (JSON_UNLIKELY(reference_token == "-"))
                {
                    // "-" always fails the range check
                    JSON_THROW(detail::out_of_range::create(402,
                        fmt::format("array index '-' ({}) is out of range", ptr->m_value.array->size())));
                }

                // error condition (cf. RFC 6901, Sect. 4)
                if (JSON_UNLIKELY(reference_token.size() > 1 and reference_token[0] == '0'))
                {
                    JSON_THROW(detail::parse_error::create(106, 0,
                                            fmt::format("array index '{}' must not begin with '0'", reference_token)));
                }

                // note: at performs range check
                JSON_TRY
                {
                    ptr = &ptr->at(static_cast<size_type>(array_index(reference_token)));
                }
                JSON_CATCH(std::invalid_argument&)
                {
                    JSON_THROW(detail::parse_error::create(109, 0,
                                            fmt::format("array index '{}' is not a number", reference_token)));
                }
                break;
            }

            default:
                JSON_THROW(detail::out_of_range::create(404,
                                    fmt::format("unresolved reference token '{}'", reference_token)));
        }
    }

    return *ptr;
}

const json& json_pointer::get_unchecked(const json* ptr) const
{
    using size_type = typename json::size_type;
    for (const auto& reference_token : reference_tokens)
    {
        switch (ptr->m_type)
        {
            case detail::value_t::object:
            {
                // use unchecked object access
                ptr = &ptr->operator[](reference_token);
                break;
            }

            case detail::value_t::array:
            {
                if (JSON_UNLIKELY(reference_token == "-"))
                {
                    // "-" cannot be used for const access
                    JSON_THROW(detail::out_of_range::create(402,
                        fmt::format("array index '-' ({}) is out of range", ptr->m_value.array->size())));
                }

                // error condition (cf. RFC 6901, Sect. 4)
                if (JSON_UNLIKELY(reference_token.size() > 1 and reference_token[0] == '0'))
                {
                    JSON_THROW(detail::parse_error::create(106, 0,
                        fmt::format("array index '{}' must not begin with '0'", reference_token)));
                }

                // use unchecked array access
                JSON_TRY
                {
                    ptr = &ptr->operator[](
                        static_cast<size_type>(array_index(reference_token)));
                }
                JSON_CATCH(std::invalid_argument&)
                {
                    JSON_THROW(detail::parse_error::create(109, 0,
                        fmt::format("array index '{}' is not a number", reference_token)));
                }
                break;
            }

            default:
                JSON_THROW(detail::out_of_range::create(404,
                                    fmt::format("unresolved reference token '{}'", reference_token)));
        }
    }

    return *ptr;
}

const json& json_pointer::get_checked(const json* ptr) const
{
    using size_type = typename json::size_type;
    for (const auto& reference_token : reference_tokens)
    {
        switch (ptr->m_type)
        {
            case detail::value_t::object:
            {
                // note: at performs range check
                ptr = &ptr->at(reference_token);
                break;
            }

            case detail::value_t::array:
            {
                if (JSON_UNLIKELY(reference_token == "-"))
                {
                    // "-" always fails the range check
                    JSON_THROW(detail::out_of_range::create(402,
                                            fmt::format("array index '-' ({}) is out of range", ptr->m_value.array->size())));
                }

                // error condition (cf. RFC 6901, Sect. 4)
                if (JSON_UNLIKELY(reference_token.size() > 1 and reference_token[0] == '0'))
                {
                    JSON_THROW(detail::parse_error::create(106, 0,
                                            fmt::format("array index '{}' must not begin with '0'", reference_token)));
                }

                // note: at performs range check
                JSON_TRY
                {
                    ptr = &ptr->at(static_cast<size_type>(array_index(reference_token)));
                }
                JSON_CATCH(std::invalid_argument&)
                {
                    JSON_THROW(detail::parse_error::create(109, 0,
                                            fmt::format("array index '{}' is not a number", reference_token)));
                }
                break;
            }

            default:
                JSON_THROW(detail::out_of_range::create(404,
                                    fmt::format("unresolved reference token '{}'", reference_token)));
        }
    }

    return *ptr;
}

std::vector<std::string> json_pointer::split(std::string_view reference_string)
{
    std::vector<std::string> result;

    // special case: empty reference string -> no reference tokens
    if (reference_string.empty())
    {
        return result;
    }

    // check if nonempty reference string begins with slash
    if (JSON_UNLIKELY(reference_string[0] != '/'))
    {
        JSON_THROW(detail::parse_error::create(107, 1,
            fmt::format("JSON pointer must be empty or begin with '/' - was: '{}'", reference_string)));
    }

    // extract the reference tokens:
    // - slash: position of the last read slash (or end of string)
    // - start: position after the previous slash
    for (
        // search for the first slash after the first character
        std::size_t slash = reference_string.find_first_of('/', 1),
        // set the beginning of the first reference token
        start = 1;
        // we can stop if start == string::npos+1 = 0
        start != 0;
        // set the beginning of the next reference token
        // (will eventually be 0 if slash == std::string::npos)
        start = slash + 1,
        // find next slash
        slash = reference_string.find_first_of('/', start))
    {
        // use the text between the beginning of the reference token
        // (start) and the last slash (slash).
        auto reference_token = slice(reference_string, start, slash);

        // check reference tokens are properly escaped
        for (std::size_t pos = reference_token.find_first_of('~');
                pos != std::string_view::npos;
                pos = reference_token.find_first_of('~', pos + 1))
        {
            assert(reference_token[pos] == '~');

            // ~ must be followed by 0 or 1
            if (JSON_UNLIKELY(pos == reference_token.size() - 1 or
                              (reference_token[pos + 1] != '0' and
                               reference_token[pos + 1] != '1')))
            {
                JSON_THROW(detail::parse_error::create(108, 0, "escape character '~' must be followed with '0' or '1'"));
            }
        }

        // finally, store the reference token
        std::string ref_tok{reference_token};
        unescape(ref_tok);
        result.emplace_back(std::move(ref_tok));
    }

    return result;
}

void json_pointer::replace_substring(std::string& s, const std::string& f,
                                  const std::string& t)
{
    assert(not f.empty());
    for (auto pos = s.find(f);                // find first occurrence of f
            pos != std::string::npos;         // make sure f was found
            s.replace(pos, f.size(), t),      // replace with t, and
            pos = s.find(f, pos + t.size()))  // find next occurrence of f
    {}
}

std::string json_pointer::escape(std::string s)
{
    replace_substring(s, "~", "~0");
    replace_substring(s, "/", "~1");
    return s;
}

void json_pointer::unescape(std::string& s)
{
    replace_substring(s, "~1", "/");
    replace_substring(s, "~0", "~");
}

void json_pointer::flatten(std::string_view reference_string,
                        const json& value,
                        json& result)
{
    switch (value.m_type)
    {
        case detail::value_t::array:
        {
            if (value.m_value.array->empty())
            {
                // flatten empty array as null
                result[reference_string] = nullptr;
            }
            else
            {
                // iterate array and use index as reference string
                for (std::size_t i = 0; i < value.m_value.array->size(); ++i)
                {
                    flatten(fmt::format("{}/{}", reference_string, i),
                            value.m_value.array->operator[](i), result);
                }
            }
            break;
        }

        case detail::value_t::object:
        {
            if (value.m_value.object->empty())
            {
                // flatten empty object as null
                result[reference_string] = nullptr;
            }
            else
            {
                // iterate object and use keys as reference string
                for (const auto& element : *value.m_value.object)
                {
                    flatten(fmt::format("{}/{}", reference_string, escape(std::string{element.first()})), element.second, result);
                }
            }
            break;
        }

        default:
        {
            // add primitive value with its reference string
            result[reference_string] = value;
            break;
        }
    }
}

json json_pointer::unflatten(const json& value)
{
    if (JSON_UNLIKELY(not value.is_object()))
    {
        JSON_THROW(detail::type_error::create(314, "only objects can be unflattened"));
    }

    // we need to iterate over the object values in sorted key order
    SmallVector<StringMapConstIterator<json>, 64> sorted;
    for (auto i = value.m_value.object->begin(),
         end = value.m_value.object->end(); i != end; ++i)
    {
        if (!i->second.is_primitive())
        {
            JSON_THROW(detail::type_error::create(315, "values in object must be primitive"));
        }
        sorted.push_back(i);
    }
    std::sort(sorted.begin(), sorted.end(),
              [](const StringMapConstIterator<json>& a,
                 const StringMapConstIterator<json>& b) {
                return a->getKey() < b->getKey();
              });

    json result;

    // iterate the sorted JSON object values
    for (const auto& element : sorted)
    {

        // assign value to reference pointed to by JSON pointer; Note
        // that if the JSON pointer is "" (i.e., points to the whole
        // value), function get_and_create returns a reference to
        // result itself. An assignment will then create a primitive
        // value.
        json_pointer(element->first()).get_and_create(result) = element->second;
    }

    return result;
}
}  // namespace wpi
#undef WPI_JSON_IMPLEMENTATION
