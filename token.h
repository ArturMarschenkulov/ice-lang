#pragma once

#include <string>
#include <any>
#include <variant>





struct Location {
	int line;
	int column;
};
struct Span {
	Location start;
	Location end;
};
//using IceObject = std::variant<int, bool, std::monostate>;
class Token {
public:
	enum class TYPE {
		//KEYWORDS
		KW_VAR,
		KW_FUNC,
		KW_RETURN,
		KW_I8, KW_I16, KW_I32, KW_I64, KW_I128,
		KW_U8, KW_U16, KW_U32, KW_U64, KW_U128,
		KW_F8, KW_F16, KW_F32, KW_F64, KW_F128,

		// LITERALS
		L_TRUE,
		L_FALSE,
		L_NUMBER,
		L_CHAR,
		L_STRING,

		// SYMBOLS
		S_IDENTIFIER,
		S_PUNCTUATOR,


		//EXTRA
		SKW_EOF,
		SKW_WHITESPACE,
		SKW_TAB,
		SKW_NEWLINE,
		SKW_COMMENT,
		SKW_UNKNOWN,
		SKW_PRINT,
	};
	struct Whitespace {
		bool left = false;
		bool right = false;
	};
public:
	TYPE type = TYPE::SKW_UNKNOWN;
	std::string lexeme = "DEBUG: THE LEXEME OF A TOKEN WAS UNTOUCHED. THIS SHOULDN'T BE!";
	Span span;
	Whitespace whitespace;
};
//auto to_string(TOKEN_TYPE token_type) -> std::string;
auto operator<<(std::ostream& ost, const Token& token)->std::ostream&;