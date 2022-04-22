#pragma once

#include "token.h"
#include "ast.h"
#include "misc/option.h"

#include <vector>
#include <memory>
#include <iostream>
#include <string>
#include <cassert>
// #include <format>



static auto get_unary_operator_precedence(const Token& token) -> int {
    int precedence = 0;
    if (token.type == Token::TYPE::S_PUNCTUATOR) {
        auto& op = token.lexeme;
        if (op == "+" || op == "-" || op == "!") { precedence = 6; }
    } else {
        precedence = 0;
    }
    return precedence;
}
static auto get_binary_operator_precedence(const Token& token) -> int {
    int precedence = 0;

    if (token.type == Token::TYPE::S_PUNCTUATOR) {
        auto& op = token.lexeme;
        if (op == "*" || op == "/") { precedence = 5; }
        if (op == "+" || op == "-") { precedence = 4; }
        if (op == "<" || op == ">") { precedence = 3; }
        if (op == "&") { precedence = 2; }
        if (op == "|") { precedence = 1; }
    } else {
        precedence = 0;
    }
    // switch (token.type) {
    // case Token::TYPE::P_STAR:
    // case Token::TYPE::P_SLASH: precedence = 5; break;

    // case Token::TYPE::P_PLUS:
    // case Token::TYPE::P_MINUS: precedence = 4; break;

    // case Token::TYPE::P_LESS:
    ////case TOKEN_TYPE::P_LESS_EQUAL:
    // case Token::TYPE::P_GREATER:
    ////case TOKEN_TYPE::P_GREATER_EQUAL:
    ////case TOKEN_TYPE::P_EQUAL_EQUAL:
    ////case TOKEN_TYPE::P_BANG_EQUAL:
    //       precedence = 3; break;

    // case Token::TYPE::P_AMPERSAND:
    ////case TOKEN_TYPE::P_AMPERSAND_AMPERSAND:
    //       precedence = 2; break;

    // case Token::TYPE::P_PIPE:
    ////case TOKEN_TYPE::P_PIPE_PIPE:
    //       precedence = 1; break;

    // case Token::TYPE::SKW_UNKNOWN: __debugbreak(); precedence = -1; break;
    // default: precedence = 0;
    // }
    return precedence;
}

struct TokenCursor {
    TokenCursor()
        : m_it(nullptr) /*, m_loc({ 1, 1 })*/ {}

    TokenCursor(const TokenCursor& o)
        : m_it(o.m_it) /*, m_loc(o.m_loc)*/ {}
    TokenCursor(const std::vector<Token>& tokens)
        : m_it(&tokens[0]) /*, m_loc({ 1, 1 })*/ {}
    auto operator=(const std::vector<Token>& tokens) -> TokenCursor& {
        m_it = &tokens[0];
        return *this;
    }
    auto get() const -> const Token* { return m_it; }
    // retuns the current token before the move
    auto advance() -> const Token& {
        const Token& tok = this->peek(0);
        m_it++;
        return tok;
    }
    auto peek_(int n) const -> Option<const Token&> {
        // if (m_it + n >= m_tokens.end()) { return Option<const Token&>(); }
        return {*(m_it + n)};
    }
    auto peek(int n) const -> const Token& { return *(m_it + n); }
    auto is_at_end() const -> bool { return this->peek(0).type == Token::TYPE::SKW_EOF; }

    auto expect(const std::string& expected_lexeme /*, const std::string& error_string*/) const -> void {
        if (expected_lexeme != this->peek(0).lexeme) {
            std::cout << "Expected \"" << expected_lexeme << "\", but got \"" << this->peek(0).lexeme << "\""
                      << std::endl;
        }
    }
    auto match(std::initializer_list<Token::TYPE> types) const -> bool {
        for (const Token::TYPE& type : types) {
            if (this->is_at_end()) return false;

            if (this->peek(0).type == type) { return true; }
        }
        return false;
    }
    auto skip_if(const Token::TYPE& s) -> bool {
        bool result = false;
        if (s == this->peek(0).type) {
            result = true;
            this->advance();
        }
        return result;
    }
    auto skip_if(const std::string& s) -> bool {
        bool result = false;
        if (s == this->peek(0).lexeme) {
            result = true;
            this->advance();
        }
        return result;
    }

    // auto consume_2(const std::string& s) -> const Option<const Token&> {
    //     const Token token = this->peek(0);
    //     const bool  skipped = this->skip_if(s);
    //     if (skipped) {
    //         return Option<const Token&>(token);
    //     } else {
    //         const Token& tok = this->peek(0);
    //         auto&        ss = token.span.start;
    //         std::string  msg = "error: expected \"" + s + "\", but instead found \"" + token.lexeme +
    //                           "\"\n--><source>:" + std::to_string(ss.line) + ":" + std::to_string(ss.column);
    //         std::cout << msg << std::endl;
    //         return Option<const Token&>();
    //     }
    // }

    // auto c(const std::string& s) -> const Option<Token> {
    //     const Token& res = this->peek(0);
    //     const bool   skipped = this->skip_if(s);
    //     if (false == skipped) {
    //         const Token& token = this->peek(0);
    //         auto&        ss = token.span.start;
    //         std::string  msg = "error: expected \"" + s + "\", but instead found \"" + token.lexeme +
    //                           "\"\n--><source>:" + std::to_string(ss.line) + ":" + std::to_string(ss.column);
    //         std::cout << msg << std::endl;
    //         // result;
    //     }
    //     return Option<Token>();
    // }


    // Consumes the token, meaning it moves the cursor forward if it is the token provided.
    // That consumed token is returned as an lvalue ref Option.
    // If the token is not the one provided, then the cursor is not moved and the Option is empty.
    auto consume(const Token& token) -> Option<const Token&> {
        const Token& result = this->peek(0);
        const bool   skipped = this->skip_if(token.lexeme);

        bool to_print_error = false;

        // print error
        // TODO: Try to somehow extract it out of this function. Maybe Result type?
        if (false == skipped) {
            const Token& tok = this->peek(0);
            auto&        ss = tok.span.start;

            std::string msg = "error: expected \"" + tok.lexeme + "\", but instead found \"" + tok.lexeme +
                              "\"\n--><source>:" + std::to_string(ss.line) + ":" + std::to_string(ss.column);
            std::cout << msg << std::endl;
            return Option<const Token&>();
        }
        return result;
    }
    // Returns a pointer to the consumed Token otherwise nullptr
    auto consume__(const std::string& s) -> const Token* {
        const Token* result = &this->peek(0);

        const bool skipped = this->skip_if(s);
        if (false == skipped) {
            const Token& token = this->peek(0);
            auto&        ss = token.span.start;
            // auto         msg = std::format(
            //             "error: expected \"{}\", but instead found \"{}\"\n--><source>:{}:{}", s, token.lexeme,
            //             ss.line, ss.column);

            std::string msg = "error: expected \"" + s + "\", but instead found \"" + token.lexeme +
                              "\"\n--><source>:" + std::to_string(ss.line) + ":" + std::to_string(ss.column);
            std::cout << msg << std::endl;
            result = nullptr;
        }

        return result;
    }

private:
    mutable const Token*     m_it;
    int                      token_index = 0;
    const std::vector<Token> m_tokens;
    // Location m_loc;
};

class Parser {
public:
    auto parse_tokens(const std::vector<Token>& tokens) -> std::vector<std::unique_ptr<Stmt>>;

    TokenCursor m_tc;
};