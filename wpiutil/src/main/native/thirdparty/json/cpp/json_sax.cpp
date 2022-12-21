#define WPI_JSON_IMPLEMENTATION
#include "wpi/detail/input/json_sax.h"

#include "fmt/format.h"

namespace wpi
{
namespace detail
{

bool json_sax_dom_parser::start_object(std::size_t len)
{
    ref_stack.push_back(handle_value(json::value_t::object));

    if (JSON_UNLIKELY(len != std::size_t(-1) and len > ref_stack.back()->max_size()))
    {
        JSON_THROW(out_of_range::create(408,
                                        fmt::format("excessive object size: {}", len)));
    }

    return true;
}

bool json_sax_dom_parser::start_array(std::size_t len)
{
    ref_stack.push_back(handle_value(json::value_t::array));

    if (JSON_UNLIKELY(len != std::size_t(-1) and len > ref_stack.back()->max_size()))
    {
        JSON_THROW(out_of_range::create(408,
                                        fmt::format("excessive array size: {}", len)));
    }

    return true;
}

bool json_sax_dom_parser::parse_error(std::size_t, const std::string&,
                     const detail::exception& ex)
{
    errored = true;
    if (allow_exceptions)
    {
        // determine the proper exception type from the id
        switch ((ex.id / 100) % 100)
        {
            case 1:
                JSON_THROW(*reinterpret_cast<const detail::parse_error*>(&ex));
            case 4:
                JSON_THROW(*reinterpret_cast<const detail::out_of_range*>(&ex));
            // LCOV_EXCL_START
            case 2:
                JSON_THROW(*reinterpret_cast<const detail::invalid_iterator*>(&ex));
            case 3:
                JSON_THROW(*reinterpret_cast<const detail::type_error*>(&ex));
            case 5:
                JSON_THROW(*reinterpret_cast<const detail::other_error*>(&ex));
            default:
                assert(false);
                // LCOV_EXCL_STOP
        }
    }
    return false;
}

bool json_sax_dom_callback_parser::start_object(std::size_t len)
{
    // check callback for object start
    const bool keep = callback(static_cast<int>(ref_stack.size()), parse_event_t::object_start, discarded);
    keep_stack.push_back(keep);

    auto val = handle_value(json::value_t::object, true);
    ref_stack.push_back(val.second);

    // check object limit
    if (ref_stack.back())
    {
        if (JSON_UNLIKELY(len != std::size_t(-1) and len > ref_stack.back()->max_size()))
        {
            JSON_THROW(out_of_range::create(408,
                                            fmt::format("excessive object size: {}", len)));
        }
    }

    return true;
}

bool json_sax_dom_callback_parser::key(std::string_view val)
{
    json k = json(val);

    // check callback for key
    const bool keep = callback(static_cast<int>(ref_stack.size()), parse_event_t::key, k);
    key_keep_stack.push_back(keep);

    // add discarded value at given key and store the reference for later
    if (keep and ref_stack.back())
    {
        object_element = &(ref_stack.back()->m_value.object->operator[](val) = discarded);
    }

    return true;
}

bool json_sax_dom_callback_parser::end_object()
{
    if (ref_stack.back())
    {
        if (not callback(static_cast<int>(ref_stack.size()) - 1, parse_event_t::object_end, *ref_stack.back()))
        {
            // discard object
            *ref_stack.back() = discarded;
        }
    }

    assert(not ref_stack.empty());
    assert(not keep_stack.empty());
    ref_stack.pop_back();
    keep_stack.pop_back();

    if (not ref_stack.empty() and ref_stack.back())
    {
        // remove discarded value
        if (ref_stack.back()->is_object())
        {
            for (auto it = ref_stack.back()->begin(); it != ref_stack.back()->end(); ++it)
            {
                if (it->is_discarded())
                {
                    ref_stack.back()->erase(it);
                    break;
                }
            }
        }
    }

    return true;
}

bool json_sax_dom_callback_parser::start_array(std::size_t len)
{
    const bool keep = callback(static_cast<int>(ref_stack.size()), parse_event_t::array_start, discarded);
    keep_stack.push_back(keep);

    auto val = handle_value(json::value_t::array, true);
    ref_stack.push_back(val.second);

    // check array limit
    if (ref_stack.back())
    {
        if (JSON_UNLIKELY(len != std::size_t(-1) and len > ref_stack.back()->max_size()))
        {
            JSON_THROW(out_of_range::create(408,
                                            fmt::format("excessive array size: {}", len)));
        }
    }

    return true;
}

bool json_sax_dom_callback_parser::end_array()
{
    bool keep = true;

    if (ref_stack.back())
    {
        keep = callback(static_cast<int>(ref_stack.size()) - 1, parse_event_t::array_end, *ref_stack.back());
        if (not keep)
        {
            // discard array
            *ref_stack.back() = discarded;
        }
    }

    assert(not ref_stack.empty());
    assert(not keep_stack.empty());
    ref_stack.pop_back();
    keep_stack.pop_back();

    // remove discarded value
    if (not keep and not ref_stack.empty())
    {
        if (ref_stack.back()->is_array())
        {
            ref_stack.back()->m_value.array->pop_back();
        }
    }

    return true;
}

bool json_sax_dom_callback_parser::parse_error(std::size_t, const std::string&,
                     const detail::exception& ex)
{
    errored = true;
    if (allow_exceptions)
    {
        // determine the proper exception type from the id
        switch ((ex.id / 100) % 100)
        {
            case 1:
                JSON_THROW(*reinterpret_cast<const detail::parse_error*>(&ex));
            case 4:
                JSON_THROW(*reinterpret_cast<const detail::out_of_range*>(&ex));
            // LCOV_EXCL_START
            case 2:
                JSON_THROW(*reinterpret_cast<const detail::invalid_iterator*>(&ex));
            case 3:
                JSON_THROW(*reinterpret_cast<const detail::type_error*>(&ex));
            case 5:
                JSON_THROW(*reinterpret_cast<const detail::other_error*>(&ex));
            default:
                assert(false);
                // LCOV_EXCL_STOP
        }
    }
    return false;
}
}  // namespace detail
}  // namespace wpi
#undef WPI_JSON_IMPLEMENTATION
