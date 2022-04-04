#include "Token.h"
#include <iostream>

static auto to_string(Token::TYPE token_type) -> std::string {
    switch (token_type) {

        case Token::TYPE::KW_FN: return "KW_FN"; break;
        case Token::TYPE::KW_VAR: return "KW_VAR"; break;
        case Token::TYPE::KW_RETURN: return "KW_RETURN"; break;


        // case Token::TYPE::L_NUMBER: return "L_NUMBER"; break;
        case Token::TYPE::L_INTEGER: return "L_INTEGER"; break;
        case Token::TYPE::L_FLOAT: return "L_FLOAT"; break;
        case Token::TYPE::L_TRUE: return "L_TRUE"; break;
        case Token::TYPE::L_FALSE: return "L_FALSE"; break;

        case Token::TYPE::S_IDENTIFIER: return "S_IDENTIFIER"; break;
        case Token::TYPE::S_PUNCTUATOR: return "S_PUNCTUATOR"; break;

        case Token::TYPE::SKW_WHITESPACE: return "SKW_WHITESPACE"; break;
        case Token::TYPE::SKW_NEWLINE: return "SKW_NEWLINE"; break;
        case Token::TYPE::SKW_TAB: return "SKW_TAB"; break;
        case Token::TYPE::SKW_COMMENT: return "SKW_COMMENT"; break;
        case Token::TYPE::SKW_EOF: return "SKW_EOF"; break;

        case Token::TYPE::SKW_UNKNOWN: return "SKW_UNKNOWN"; break;

        default: /*__debugbreak();*/ return "INVALID TOKEN"; break;
    }
}

auto operator<<(std::ostream& ost, const Token& token) -> std::ostream& {
    ost << to_string(token.type) << " [ " << token.lexeme << " ]"
        << " "
        << "(" << token.span.start.line << ":" << token.span.start.column << "-" << token.span.end.line << ":"
        << token.span.end.column << ")";
    return ost;
}