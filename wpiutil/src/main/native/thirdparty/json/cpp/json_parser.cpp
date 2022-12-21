#define WPI_JSON_IMPLEMENTATION
#include "wpi/detail/input/json_parser.h"

#include "wpi/json.h"
#include "fmt/format.h"

namespace wpi
{

void json::parser::parse(const bool strict, json& result)
{
    if (callback)
    {
        detail::json_sax_dom_callback_parser sdp(result, callback, allow_exceptions);
        sax_parse_internal(&sdp);
        result.assert_invariant();

        // in strict mode, input must be completely read
        if (strict and (get_token() != token_type::end_of_input))
        {
            sdp.parse_error(m_lexer.get_position(),
                            m_lexer.get_token_string(),
                            parse_error::create(101, m_lexer.get_position(), exception_message(token_type::end_of_input)));
        }

        // in case of an error, return discarded value
        if (sdp.is_errored())
        {
            result = value_t::discarded;
            return;
        }

        // set top-level value to null if it was discarded by the callback
        // function
        if (result.is_discarded())
        {
            result = nullptr;
        }
    }
    else
    {
        detail::json_sax_dom_parser sdp(result, allow_exceptions);
        sax_parse_internal(&sdp);
        result.assert_invariant();

        // in strict mode, input must be completely read
        if (strict and (get_token() != token_type::end_of_input))
        {
            sdp.parse_error(m_lexer.get_position(),
                            m_lexer.get_token_string(),
                            parse_error::create(101, m_lexer.get_position(), exception_message(token_type::end_of_input)));
        }

        // in case of an error, return discarded value
        if (sdp.is_errored())
        {
            result = value_t::discarded;
            return;
        }
    }
}

bool json::parser::accept(const bool strict)
{
    detail::json_sax_acceptor sax_acceptor;
    return sax_parse(&sax_acceptor, strict);
}

template <typename SAX>
bool json::parser::sax_parse_internal(SAX* sax)
{
    // stack to remember the hieararchy of structured values we are parsing
    // true = array; false = object
    std::vector<bool> states;
    // value to avoid a goto (see comment where set to true)
    bool skip_to_state_evaluation = false;

    while (true)
    {
        if (not skip_to_state_evaluation)
        {
            // invariant: get_token() was called before each iteration
            switch (last_token)
            {
                case token_type::begin_object:
                {
                    if (JSON_UNLIKELY(not sax->start_object(std::size_t(-1))))
                    {
                        return false;
                    }

                    // closing } -> we are done
                    if (get_token() == token_type::end_object)
                    {
                        if (JSON_UNLIKELY(not sax->end_object()))
                        {
                            return false;
                        }
                        break;
                    }

                    // parse key
                    if (JSON_UNLIKELY(last_token != token_type::value_string))
                    {
                        return sax->parse_error(m_lexer.get_position(),
                                                m_lexer.get_token_string(),
                                                parse_error::create(101, m_lexer.get_position(), exception_message(token_type::value_string)));
                    }
                    else
                    {
                        if (JSON_UNLIKELY(not sax->key(m_lexer.get_string())))
                        {
                            return false;
                        }
                    }

                    // parse separator (:)
                    if (JSON_UNLIKELY(get_token() != token_type::name_separator))
                    {
                        return sax->parse_error(m_lexer.get_position(),
                                                m_lexer.get_token_string(),
                                                parse_error::create(101, m_lexer.get_position(), exception_message(token_type::name_separator)));
                    }

                    // remember we are now inside an object
                    states.push_back(false);

                    // parse values
                    get_token();
                    continue;
                }

                case token_type::begin_array:
                {
                    if (JSON_UNLIKELY(not sax->start_array(std::size_t(-1))))
                    {
                        return false;
                    }

                    // closing ] -> we are done
                    if (get_token() == token_type::end_array)
                    {
                        if (JSON_UNLIKELY(not sax->end_array()))
                        {
                            return false;
                        }
                        break;
                    }

                    // remember we are now inside an array
                    states.push_back(true);

                    // parse values (no need to call get_token)
                    continue;
                }

                case token_type::value_float:
                {
                    const auto res = m_lexer.get_number_float();

                    if (JSON_UNLIKELY(not std::isfinite(res)))
                    {
                        return sax->parse_error(m_lexer.get_position(),
                                                m_lexer.get_token_string(),
                                                out_of_range::create(406, "number overflow parsing '" + m_lexer.get_token_string() + "'"));
                    }
                    else
                    {
                        if (JSON_UNLIKELY(not sax->number_float(res, m_lexer.get_string())))
                        {
                            return false;
                        }
                        break;
                    }
                }

                case token_type::literal_false:
                {
                    if (JSON_UNLIKELY(not sax->boolean(false)))
                    {
                        return false;
                    }
                    break;
                }

                case token_type::literal_null:
                {
                    if (JSON_UNLIKELY(not sax->null()))
                    {
                        return false;
                    }
                    break;
                }

                case token_type::literal_true:
                {
                    if (JSON_UNLIKELY(not sax->boolean(true)))
                    {
                        return false;
                    }
                    break;
                }

                case token_type::value_integer:
                {
                    if (JSON_UNLIKELY(not sax->number_integer(m_lexer.get_number_integer())))
                    {
                        return false;
                    }
                    break;
                }

                case token_type::value_string:
                {
                    if (JSON_UNLIKELY(not sax->string(m_lexer.get_string())))
                    {
                        return false;
                    }
                    break;
                }

                case token_type::value_unsigned:
                {
                    if (JSON_UNLIKELY(not sax->number_unsigned(m_lexer.get_number_unsigned())))
                    {
                        return false;
                    }
                    break;
                }

                case token_type::parse_error:
                {
                    // using "uninitialized" to avoid "expected" message
                    return sax->parse_error(m_lexer.get_position(),
                                            m_lexer.get_token_string(),
                                            parse_error::create(101, m_lexer.get_position(), exception_message(token_type::uninitialized)));
                }

                default: // the last token was unexpected
                {
                    return sax->parse_error(m_lexer.get_position(),
                                            m_lexer.get_token_string(),
                                            parse_error::create(101, m_lexer.get_position(), exception_message(token_type::literal_or_value)));
                }
            }
        }
        else
        {
            skip_to_state_evaluation = false;
        }

        // we reached this line after we successfully parsed a value
        if (states.empty())
        {
            // empty stack: we reached the end of the hieararchy: done
            return true;
        }
        else
        {
            if (states.back())  // array
            {
                // comma -> next value
                if (get_token() == token_type::value_separator)
                {
                    // parse a new value
                    get_token();
                    continue;
                }

                // closing ]
                if (JSON_LIKELY(last_token == token_type::end_array))
                {
                    if (JSON_UNLIKELY(not sax->end_array()))
                    {
                        return false;
                    }

                    // We are done with this array. Before we can parse a
                    // new value, we need to evaluate the new state first.
                    // By setting skip_to_state_evaluation to false, we
                    // are effectively jumping to the beginning of this if.
                    assert(not states.empty());
                    states.pop_back();
                    skip_to_state_evaluation = true;
                    continue;
                }
                else
                {
                    return sax->parse_error(m_lexer.get_position(),
                                            m_lexer.get_token_string(),
                                            parse_error::create(101, m_lexer.get_position(), exception_message(token_type::end_array)));
                }
            }
            else  // object
            {
                // comma -> next value
                if (get_token() == token_type::value_separator)
                {
                    // parse key
                    if (JSON_UNLIKELY(get_token() != token_type::value_string))
                    {
                        return sax->parse_error(m_lexer.get_position(),
                                                m_lexer.get_token_string(),
                                                parse_error::create(101, m_lexer.get_position(), exception_message(token_type::value_string)));
                    }
                    else
                    {
                        if (JSON_UNLIKELY(not sax->key(m_lexer.get_string())))
                        {
                            return false;
                        }
                    }

                    // parse separator (:)
                    if (JSON_UNLIKELY(get_token() != token_type::name_separator))
                    {
                        return sax->parse_error(m_lexer.get_position(),
                                                m_lexer.get_token_string(),
                                                parse_error::create(101, m_lexer.get_position(), exception_message(token_type::name_separator)));
                    }

                    // parse values
                    get_token();
                    continue;
                }

                // closing }
                if (JSON_LIKELY(last_token == token_type::end_object))
                {
                    if (JSON_UNLIKELY(not sax->end_object()))
                    {
                        return false;
                    }

                    // We are done with this object. Before we can parse a
                    // new value, we need to evaluate the new state first.
                    // By setting skip_to_state_evaluation to false, we
                    // are effectively jumping to the beginning of this if.
                    assert(not states.empty());
                    states.pop_back();
                    skip_to_state_evaluation = true;
                    continue;
                }
                else
                {
                    return sax->parse_error(m_lexer.get_position(),
                                            m_lexer.get_token_string(),
                                            parse_error::create(101, m_lexer.get_position(), exception_message(token_type::end_object)));
                }
            }
        }
    }
}

std::string json::parser::exception_message(const token_type expected)
{
    std::string error_msg = "syntax error - ";
    if (last_token == token_type::parse_error)
    {
        error_msg += std::string(m_lexer.get_error_message()) + "; last read: '" +
                     m_lexer.get_token_string() + "'";
    }
    else
    {
        error_msg += "unexpected " + std::string(lexer_t::token_type_name(last_token));
    }

    if (expected != token_type::uninitialized)
    {
        error_msg += "; expected " + std::string(lexer_t::token_type_name(expected));
    }

    return error_msg;
}
}  // namespace wpi
#undef WPI_JSON_IMPLEMENTATION
