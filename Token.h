#pragma once

#include <string>
#include <any>
#include <variant>



enum class TOKEN_TYPE {
	//PUNCTUATORS ONE
	P_SEMICOLON, // ;
	P_COLON,// :
	P_EQUAL,// =
	P_BANG, // !
	P_GREATER, // >
	P_LESS, // <
	P_COMMA,// ,
	P_PLUS,	// +
	P_MINUS,// -
	P_STAR,	// *
	P_SLASH,// /
	P_AMPERSAND, // &
	P_PIPE, // |
	P_HASH, // #
	P_QUESTION, // ?
	P_AT, // @
	P_DOLLAR, // $
	P_TILDE, // ~

	P_LEFT_PAREN,    // (
	P_RIGHT_PAREN,	 // )
	P_LEFT_BRACKET,	 // [
	P_RIGHT_BRACKET, // ]
	P_LEFT_BRACE,	 // {
	P_RIGHT_BRACE,	 // }

	//PUNCTUATORS TWO
	P_COLON_COLON, // ::
	P_EQUAL_EQUAL, // ==
	P_BANG_EQUAL, // !=
	P_GREATER_EQUAL, // >=
	P_LESS_EQUAL, // <=
	P_AMPERSAND_AMPERSAND, // &&
	P_PIPE_PIPE, // ||
	P_MINUS_GREATER, // ->
	P_EQUAL_GREATER, // =>

	//KEYWORDS
	KW_VAR,
	KW_FUNC,
	KW_RETURN,
	KW_I8, KW_I16, KW_I32, KW_I64, KW_I128,
	KW_U8, KW_U16, KW_U32, KW_U64, KW_U128,
	KW_F8, KW_F16, KW_F32, KW_F64, KW_F128,

	//LITERAL
	L_NUMBER,
	L_TRUE,
	L_FALSE,
	L_IDENTIFIER,


	//EXTRA
	SKW_EOF,
	SKW_WHITESPACE,
	SKW_TAB,
	SKW_NEWLINE,
	SKW_COMMENT,
	SKW_UNKNOWN,
	SKW_PRINT,
};

using IceObject = std::variant<int, bool, std::monostate>;
class Token {
public:
	TOKEN_TYPE type = TOKEN_TYPE::SKW_UNKNOWN;
	std::string lexeme = "DEBUG: THE LEXEME OF A TOKEN WAS UNTOUCHED. THIS SHOULDN'T BE!";
	IceObject literal = std::monostate();
	int line = 1;
};
auto to_string(TOKEN_TYPE token_type) -> std::string;
auto operator<<(std::ostream& ost, const Token& token) -> std::ostream&;