#pragma once

#include <string>
#include <any>
#include <variant>
#include <cassert>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>


class Token;

constexpr auto is_digit(const char c) -> bool;
constexpr auto is_binary_digit(const char c) -> bool;
constexpr auto is_octal_digit(const char c) -> bool;
constexpr auto is_hexadecimal_digit(const char c) -> bool;
constexpr auto is_alpha(const char c) -> bool;
constexpr auto is_alpha_numeric(const char c) -> bool;
constexpr auto is_punctuator(const char c) -> bool;
constexpr auto is_keyword(const char* str) -> bool;
constexpr auto is_potential_operator(const Token& token) -> bool;
constexpr auto is_skip_token(const Token& token) -> bool;
constexpr auto can_be_multichar_punctuator(const char c) -> bool;

constexpr auto str_len(const char* str) -> uint64_t;
constexpr auto str_cmp(const char* str1, const char* str2) -> bool;

struct Location {
    int line = 1;
    int column = 1;
};
struct Span {
    Location start;
    Location end;
};
// using IceObject = std::variant<int, bool, std::monostate>;
class Token {
public:
    enum class TYPE2 {
        KEYWORD,
        LITERAL,
        IDENTIFIER,
        PUNCTUATOR,

        SKW_EOF,
        SKW_WHITESPACE,
        SKW_TAB,
        SKW_NEWLINE,
        SKW_COMMENT,
        SKW_UNKNOWN,
        SKW_PRINT,
    };
    enum class TYPE {
        // KEYWORDS
        KW_VAR,
        KW_FN,
        KW_RETURN,
        KW_I8,
        KW_I16,
        KW_I32,
        KW_I64,
        KW_I128,
        KW_U8,
        KW_U16,
        KW_U32,
        KW_U64,
        KW_U128,
        KW_F8,
        KW_F16,
        KW_F32,
        KW_F64,
        KW_F128,

        // LITERALS
        L_TRUE,
        L_FALSE,
        // L_NUMBER,
        L_INTEGER,
        L_FLOAT,
        L_CHAR,
        L_STRING,

        // SYMBOLS
        S_IDENTIFIER,
        S_PUNCTUATOR,


        // EXTRA
        SKW_EOF,
        SKW_WHITESPACE,
        SKW_TAB,
        SKW_NEWLINE,
        SKW_COMMENT,
        SKW_UNKNOWN,
        SKW_PRINT,
    };
    constexpr static auto to_TYPE(const char* str) -> TYPE {
        TYPE result;

        if (is_keyword(str)) {
            if (str == "var") result = TYPE::KW_VAR;
            else if (str == "fn") result = TYPE::KW_FN;
            else if (str == "return") result = TYPE::KW_RETURN;
            else if (str == "i8") result = TYPE::KW_I8;
            else if (str == "i16") result = TYPE::KW_I16;
            else if (str == "i32") result = TYPE::KW_I32;
            else if (str == "i64") result = TYPE::KW_I64;
            else if (str == "i128") result = TYPE::KW_I128;
            else if (str == "u8") result = TYPE::KW_U8;
            else if (str == "u16") result = TYPE::KW_U16;
            else if (str == "u32") result = TYPE::KW_U32;
            else if (str == "u64") result = TYPE::KW_U64;
            else if (str == "u128") result = TYPE::KW_U128;
            else if (str == "f8") result = TYPE::KW_F8;
            else if (str == "f16") result = TYPE::KW_F16;
            else if (str == "f32") result = TYPE::KW_F32;
            else if (str == "f64") result = TYPE::KW_F64;
            else if (str == "f128") result = TYPE::KW_F128;

            else if (str == "print") result = TYPE::SKW_PRINT;
            else if (str == "true") result = TYPE::L_TRUE;
            else if (str == "false") result = TYPE::L_FALSE;
        }

        if (str_len(str) == 1) {
            char c = str[0];
            switch (c) {
                case '+':
                case '-':
                case '*':
                case '/':
                case '(':
                case ')':
                case '[':
                case ']':
                case '{':
                case '}':
                case ';': result = TYPE::S_PUNCTUATOR; break;
                default: std::cout << str[0] << std::endl; assert(false && "Invalid punctuator: ");
            }
        }

        result = TYPE::S_IDENTIFIER;
        return result;
    }
    struct Whitespace {
        bool left = false;
        bool right = false;
    };

    // NOTE: This is only relevant for TYPE::S_PUNCTUATOR. Maybe in the future also TYPE::S_IDENTIFIER.
    // TODO: Maybe refactor how it is handled.
    enum class OPERATOR_TYPE {
        NONE,
        PREFIX,
        POSTFIX,
        INFIX,
    };

public:
    constexpr Token(TYPE type, const std::string& lexeme, Span span)
        : type(type)
        , lexeme(lexeme)
        , span(span) {}

    // Main used to create temporary tokens.
    explicit constexpr Token(const char* lexeme)
        : lexeme(lexeme) {
        span.start = {-1, -1};
        span.end = {-1, -1};

        type = to_TYPE(lexeme);
    }

    auto operator==(const Token& other) const -> bool { return type == other.type && lexeme == other.lexeme; }

public:
    TYPE          type = TYPE::SKW_UNKNOWN;
    std::string   lexeme = "DEBUG: THE LEXEME OF A TOKEN WAS UNTOUCHED. THIS SHOULDN'T BE!";
    Span          span;
    Whitespace    whitespace;
    OPERATOR_TYPE operator_type = OPERATOR_TYPE::NONE;
};
// auto to_string(TOKEN_TYPE token_type) -> std::string;
auto operator<<(std::ostream& ost, const Token& token) -> std::ostream&;


constexpr auto is_digit(const char c) -> bool { return c >= '0' && c <= '9'; }
constexpr auto is_binary_digit(const char c) -> bool { return c >= '0' && c <= '1'; }
constexpr auto is_octal_digit(const char c) -> bool { return c >= '0' && c <= '7'; }
constexpr auto is_hexadecimal_digit(const char c) -> bool {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}
constexpr auto is_alpha(const char c) -> bool { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }
constexpr auto is_alpha_numeric(const char c) -> bool { return is_alpha(c) || is_digit(c); }

constexpr auto is_punctuator(const char c) -> bool {

    switch (c) {
        case '+': return true;
    }
    if (c == '+') {}
    bool result = false;
    switch (c) {
        case '+':
        case '-':
        case '*':
        case '/':

        case '&':
        case '!':
        case '|':
        case '=':
        case '<':
        case '>':
        case '.':
        case ',':
        case ':':
        case ';':

        case '#':
        case '?':
        case '@':
        case '$':
        case '~':

        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}': result = true; break;
        default: result = false; break;
    }
    return result;
}
constexpr auto str_len(const char* str) -> uint64_t { return *str ? 1 + str_len(str + 1) : 0; }
constexpr auto str_cmp(const char* str1, const char* str2) -> bool {
    return *str1 ? (*str1 == *str2 ? str_cmp(str1 + 1, str2 + 1) : false) : (*str2 ? false : true);
}
constexpr auto is_keyword(const char* str) -> bool {
    if (str_len(str) == 1) { return false; }

    if (str_cmp(str, "var") == 0) { return true; }
    if (str_cmp(str, "fn") == 0) { return true; }
    if (str_cmp(str, "return") == 0) { return true; }

    if (str_cmp(str, "i8") == 0) { return true; }
    if (str_cmp(str, "i16") == 0) { return true; }
    if (str_cmp(str, "i32") == 0) { return true; }
    if (str_cmp(str, "i64") == 0) { return true; }

    if (str_cmp(str, "u8") == 0) { return true; }
    if (str_cmp(str, "u16") == 0) { return true; }
    if (str_cmp(str, "u32") == 0) { return true; }
    if (str_cmp(str, "u64") == 0) { return true; }

    if (str_cmp(str, "f32") == 0) { return true; }
    if (str_cmp(str, "f64") == 0) { return true; }

    if (str_cmp(str, "bool") == 0) { return true; }
    if (str_cmp(str, "true") == 0) { return true; }
    if (str_cmp(str, "false") == 0) { return true; }

    if (str_cmp(str, "if") == 0) { return true; }
    if (str_cmp(str, "else") == 0) { return true; }
    if (str_cmp(str, "while") == 0) { return true; }
    if (str_cmp(str, "for") == 0) { return true; }
    if (str_cmp(str, "break") == 0) { return true; }
    if (str_cmp(str, "continue") == 0) { return true; }

    assert(false && "Unknown keyword");
    return false;
}


constexpr auto is_potential_operator(const Token& token) -> bool {
    /*
            NOTE: Swift does not allow those following tokens to be in custom operators or overloadable
            "(", ")", "{", "}", "[", "]", ".", ",", ":", ";", "=", "@", "#", "&" (as a prefix operator), "->", "`", "?",
       and "!" (as a postfix operator).
    */
    if (token.type == Token::TYPE::S_PUNCTUATOR) {
        std::vector<std::string> not_potential_operator = {"(", ")", "{", "}", "[", "]", ",", ".", ";"};
        bool                     result = !std::any_of(
                                not_potential_operator.cbegin(), not_potential_operator.cend(),
                                [&token](const std::string& s) { return token.lexeme == s; });

        return result;
    }
    return false;
}

constexpr auto is_skip_token(const Token& token) -> bool {
    switch (token.type) {
        case Token::TYPE::SKW_WHITESPACE:
        case Token::TYPE::SKW_NEWLINE:
        case Token::TYPE::SKW_COMMENT: return true;
        default: return false;
    }
}
constexpr auto can_be_multichar_punctuator(const char c) -> bool {
    bool result;
    switch (c) {
        case ';':
        case '[':
        case ']':
        case '(':
        case ')':
        case '{':
        case '}': {
            result = false;
        } break;
        default: result = true; break;
    }
    return result;
}