#pragma once

#include <cstdint> // uint8_t
#include <functional> // function

#include "wpi/detail/json_macro_scope.h"
#include "wpi/detail/input/json_lexer.h"

namespace wpi
{
class json;
class raw_istream;
}

namespace wpi
{
////////////
// parser //
////////////

/*!
@brief syntax analysis

This class implements a recursive decent parser.
*/
class json::parser
{
    using lexer_t = json::lexer;
    using token_type = typename lexer_t::token_type;

  public:
    /// a parser reading from an input adapter
    explicit parser(raw_istream& s,
                    const parser_callback_t cb = nullptr,
                    const bool allow_exceptions_ = true)
        : callback(cb), m_lexer(s), allow_exceptions(allow_exceptions_)
    {}

    /*!
    @brief public parser interface

    @param[in] strict      whether to expect the last token to be EOF
    @param[in,out] result  parsed JSON value

    @throw parse_error.101 in case of an unexpected token
    @throw parse_error.102 if to_unicode fails or surrogate error
    @throw parse_error.103 if to_unicode fails
    */
    void parse(const bool strict, json& result);

    /*!
    @brief public accept interface

    @param[in] strict  whether to expect the last token to be EOF
    @return whether the input is a proper JSON text
    */
    bool accept(const bool strict = true);

  private:
    /*!
    @brief the actual parser
    @throw parse_error.101 in case of an unexpected token
    @throw parse_error.102 if to_unicode fails or surrogate error
    @throw parse_error.103 if to_unicode fails
    */
    void parse_internal(bool keep, json& result);

    /*!
    @brief the actual acceptor

    @invariant 1. The last token is not yet processed. Therefore, the caller
                  of this function must make sure a token has been read.
               2. When this function returns, the last token is processed.
                  That is, the last read character was already considered.

    This invariant makes sure that no token needs to be "unput".
    */
    bool accept_internal();

    /// get next token from lexer
    token_type get_token()
    {
        return (last_token = m_lexer.scan());
    }

    /*!
    @throw parse_error.101 if expected token did not occur
    */
    bool expect(token_type t);

    [[noreturn]] void throw_exception() const;

  private:
    /// current level of recursion
    int depth = 0;
    /// callback function
    const parser_callback_t callback = nullptr;
    /// the type of the last read token
    token_type last_token = token_type::uninitialized;
    /// the lexer
    lexer_t m_lexer;
    /// whether a syntax error occurred
    bool errored = false;
    /// possible reason for the syntax error
    token_type expected = token_type::uninitialized;
    /// whether to throw exceptions in case of errors
    const bool allow_exceptions = true;
};
}
