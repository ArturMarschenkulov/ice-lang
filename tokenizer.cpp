#include "tokenizer.h"

static auto is_digit(const char c) -> bool {
	return c >= '0' && c <= '9';
}
static auto is_binary_digit(const char c) -> bool {
	return c >= '0' && c <= '1';
}
static auto is_octal_digit(const char c) -> bool {
	return c >= '0' && c <= '7';
}
static auto is_hexadecimal_digit(const char c) -> bool {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}
static auto is_alpha(const char c) -> bool {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
static auto is_alpha_numeric(const char c) -> bool {
	return is_alpha(c) || is_digit(c);
}

static auto is_skip_token(const Token& token) -> bool {
	switch (token.type) {
		case TOKEN_TYPE::SKW_WHITESPACE:
		case TOKEN_TYPE::SKW_NEWLINE:
		case TOKEN_TYPE::SKW_COMMENT: return true;
		default: return false;
	}
}
enum class BASE {
	BINARY,
	OCTAL,
	DECIMAL,
	HEXADECIMAL,
};

Tokenizer::Tokenizer() {

	keyword_map["func"] = TOKEN_TYPE::KW_FUNC;
	keyword_map["return"] = TOKEN_TYPE::KW_RETURN;
	keyword_map["var"] = TOKEN_TYPE::KW_VAR;

	keyword_map["true"] = TOKEN_TYPE::L_TRUE;
	keyword_map["false"] = TOKEN_TYPE::L_FALSE;

	keyword_map["print"] = TOKEN_TYPE::SKW_PRINT;


}

auto Tokenizer::scan_tokens(const std::string source) const -> const std::vector<Token> {
	start_cursor = source;
	current_cursor = source;
	std::vector<Token> tokens;

	while (current_cursor.peek(0) != '\0') {
		start_cursor = current_cursor;
		Token token = scan_token(current_cursor.peek(0));
		if (is_skip_token(token) == false) {
			tokens.push_back(token);
		}
		current_cursor.advance();
	}
	start_cursor.advance();
	tokens.push_back(create_token_from_type(TOKEN_TYPE::SKW_EOF));
	return tokens;
}

auto Tokenizer::scan_token(const char c) const -> const Token {
	const TOKEN_TYPE token_type = determine_token_type(c);
	const Token token = create_token_from_type(token_type);
	return token;
}


auto Tokenizer::determine_token_type(const char c) const -> const TOKEN_TYPE {
	TOKEN_TYPE token_type;
	switch (c) {
		case '+': token_type = TOKEN_TYPE::P_PLUS; break;
		case '-': token_type = TOKEN_TYPE::P_MINUS; break;
		case '*': token_type = TOKEN_TYPE::P_STAR; break;
		case '/':
		{
			if (current_cursor.peek(1) == '/') {
				current_cursor.advance();
				while (current_cursor.peek(1) != '\n' && current_cursor.peek(1) != '\0') {
					current_cursor.advance();
				}
				token_type = TOKEN_TYPE::SKW_COMMENT;
			} else if (current_cursor.peek(1) == '*') {
				current_cursor.advance();
				while (current_cursor.peek(1) != '*' && current_cursor.peek(2) != '/') {
					current_cursor.advance();
				}
				current_cursor.advance();
				current_cursor.advance();
				token_type = TOKEN_TYPE::SKW_COMMENT;
			} else {
				token_type = TOKEN_TYPE::P_SLASH;
			}
		}break;
		case '&': token_type = TOKEN_TYPE::P_AMPERSAND; break;
		case '!': token_type = TOKEN_TYPE::P_BANG; break;
		case '|': token_type = TOKEN_TYPE::P_PIPE; break;
		case '=': token_type = TOKEN_TYPE::P_EQUAL; break;
		case '<': token_type = TOKEN_TYPE::P_LESS; break;
		case '>': token_type = TOKEN_TYPE::P_GREATER; break;
		case ',': token_type = TOKEN_TYPE::P_COMMA; break;
		case ':': token_type = TOKEN_TYPE::P_COLON; break;
		case ';': token_type = TOKEN_TYPE::P_SEMICOLON; break;

		case '#': token_type = TOKEN_TYPE::P_HASH; break;
		case '?': token_type = TOKEN_TYPE::P_QUESTION; break;
		case '@': token_type = TOKEN_TYPE::P_AT; break;
		case '$': token_type = TOKEN_TYPE::P_DOLLAR; break;
		case '~': token_type = TOKEN_TYPE::P_TILDE; break;

		case '(': token_type = TOKEN_TYPE::P_LEFT_PAREN; break;
		case ')': token_type = TOKEN_TYPE::P_RIGHT_PAREN; break;
		case '[': token_type = TOKEN_TYPE::P_LEFT_BRACKET; break;
		case ']': token_type = TOKEN_TYPE::P_RIGHT_BRACKET; break;
		case '{': token_type = TOKEN_TYPE::P_LEFT_BRACE; break;
		case '}': token_type = TOKEN_TYPE::P_RIGHT_BRACE; break;

		case ' ':
		case '\r':
		case '\t': token_type = TOKEN_TYPE::SKW_WHITESPACE;  break;
		case '\n': token_type = TOKEN_TYPE::SKW_NEWLINE;  /*line++*/; break;

		default:
		{
			if (is_digit(c)) {
				token_type = lex_number();
			} else if (is_alpha(c)) {
				token_type = lex_identifier();
			} else {
				token_type = TOKEN_TYPE::SKW_UNKNOWN;
			}

		}break;
	}
	return token_type;

}
auto Tokenizer::lex_number() const -> const TOKEN_TYPE {
	auto (*func)(char) = is_digit;

	if (current_cursor.peek(0) == '0') {
		switch (current_cursor.peek(1)) {
			case 'b': current_cursor.advance(); func = is_binary_digit; break;
			case 'o': current_cursor.advance(); func = is_octal_digit; break;
			case 'x': current_cursor.advance(); func = is_hexadecimal_digit; break;
			default: break;
		}
	}

	while (func(current_cursor.peek(1))) {
		current_cursor.advance();
	}
	return TOKEN_TYPE::L_NUMBER;
}
auto Tokenizer::lex_identifier() const -> const TOKEN_TYPE {

	//while (is_alpha_numeric(current_cursor.peek(1))) {
	//	current_cursor.advance();
	//}
	//std::string text(start_cursor.get(), current_cursor.get() + 1);


	auto prev_cursor = current_cursor;
	while (is_alpha_numeric(current_cursor.peek(1))) {
		current_cursor.advance();
	}
	std::string text(prev_cursor.get(), current_cursor.get() + 1);

	//determine whether identifier is a keyword
	TOKEN_TYPE token_type;
	if (keyword_map.find(text) == keyword_map.end()) {
		token_type = TOKEN_TYPE::L_IDENTIFIER;
	} else {
		token_type = keyword_map.at(text);
	}
	return token_type;
}
static auto process_number(const std::string& literal) -> std::pair<std::string, int> {
	int base = -1;
	std::string number;
	if (literal.rfind("0b", 0) == 0) {
		number = literal.substr(2);
		base = 2;
	} else if (literal.rfind("0o", 0) == 0) {
		number = literal.substr(2);
		base = 8;
	} else if (literal.rfind("0x", 0) == 0) {
		number = literal.substr(2);
		base = 16;
	} else {
		number = literal;
		base = 10;
	}
	return {number, base};
}
auto Tokenizer::create_token_from_type(const TOKEN_TYPE type) const -> const Token {
	Token token;
	std::string lexeme(start_cursor.get(), current_cursor.get() + 1);

	if (type == TOKEN_TYPE::L_NUMBER) {
		auto [number, base] = process_number(lexeme);
		int num = std::stoi(number, 0, base);
		token = Token{ type, lexeme, num, m_line };

	} else {
		token = Token{ type, lexeme, m_line };
	}

	return token;
}


auto CharCursor::get() const -> const char* {
	return it;
}
auto CharCursor::advance() const -> void {
	it++;
}
auto CharCursor::peek(int n) const -> const char {
	return *(it + n);
}