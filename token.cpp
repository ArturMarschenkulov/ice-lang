#include "Token.h"
#include <iostream>

static auto to_string(Token::TYPE token_type) -> std::string {
	switch (token_type) {

	case Token::TYPE::P_COMMA: return "P_COMMA"; break;
	case Token::TYPE::P_MINUS: return "P_MINUS"; break;
	case Token::TYPE::P_PLUS: return "P_PLUS"; break;
	case Token::TYPE::P_SLASH: return "P_SLASH"; break;
	case Token::TYPE::P_STAR: return "P_STAR"; break;
	case Token::TYPE::P_COLON: return "P_COLON"; break;
	case Token::TYPE::P_SEMICOLON: return "P_SEMICOLON"; break;

	case Token::TYPE::P_LEFT_PAREN: return "P_LEFT_PAREN"; break;
	case Token::TYPE::P_RIGHT_PAREN: return "P_RIGHT_PAREN"; break;
	case Token::TYPE::P_LEFT_BRACKET: return "P_LEFT_BRACKET"; break;
	case Token::TYPE::P_RIGHT_BRACKET: return "P_RIGHT_BRACKET"; break;
	case Token::TYPE::P_LEFT_BRACE: return "P_LEFT_BRACE"; break;
	case Token::TYPE::P_RIGHT_BRACE: return "P_RIGHT_BRACE"; break;

	// case TOKEN_TYPE::P_MINUS_GREATER: return "P_MINUS_GREATER"; break;
	// case TOKEN_TYPE::P_EQUAL_GREATER: return "P_EQUAL_GREATER"; break;
	// case TOKEN_TYPE::P_EQUAL_EQUAL: return "P_EQUAL_EQUAL"; break;
	// case TOKEN_TYPE::P_BANG_EQUAL: return "P_BANG_EQUAL"; break;
	// case TOKEN_TYPE::P_PIPE_PIPE: return "P_PIPE_PIPE"; break;

	case Token::TYPE::P_EQUAL: return "P_EQUAL"; break;

	case Token::TYPE::P_BANG: return "P_BANG"; break;
	case Token::TYPE::P_PIPE: return "P_PIPE"; break;



	case Token::TYPE::KW_FUNC: return "KW_FUNC"; break;
	case Token::TYPE::KW_VAR: return "KW_VAR"; break;
	case Token::TYPE::KW_RETURN: return "KW_RETURN"; break;


	case Token::TYPE::L_NUMBER: return "L_NUMBER"; break;
	case Token::TYPE::L_TRUE: return "L_TRUE"; break;
	case Token::TYPE::L_FALSE: return "L_FALSE"; break;
	case Token::TYPE::L_IDENTIFIER: return "L_IDENTIFIER"; break;

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
	ost << to_string(token.type) 
	<< " [ " << token.lexeme << " ]" << " "
	<< "(" << token.span.start.line << ":" << token.span.start.column 
	<< "-" << token.span.end.line << ":" << token.span.end.column <<  ")";
	return ost;
}