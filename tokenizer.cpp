#include "tokenizer.h"
#include <iostream>
#include <cassert>

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

static auto is_punctuator(const char c) -> bool {
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

static auto is_potential_operator(const Token& token) -> bool {
	// TODO: Implement it
	return true;
}

static auto is_skip_token(const Token& token) -> bool {
	switch (token.type) {
		case Token::TYPE::SKW_WHITESPACE:
		case Token::TYPE::SKW_NEWLINE:
		case Token::TYPE::SKW_COMMENT: return true;
		default: return false;
	}
}

static auto is_left_bound(const CharCursor& cur) -> bool {
	bool result;
	//if (tok_begin == buffer_begin) return false;

	switch (cur.peek(-1)) {
		case ' ': case '\r': case '\n': case '\t':
		case '(': case '[': case '{':
		case ',': case ';': case ':':
		case '\0':
		{
			result = false;
		} break;
		default: result = true;
	}
	return result;
}

static auto can_be_multichar_punctuator(const char c) -> bool {
	bool result;
	switch (c) {
		case ';':
		case '[': case ']': case '(': case ')': case '{': case '}':
		{
			result = false;
		}break;
		default: result = true; break;
	}
	return result;
}

static auto is_right_bound(const CharCursor& cur) -> bool {
	bool result;
	switch (cur.peek(+1)) {
		case ' ': case '\r': case '\n': case '\t':
		case ')': case ']': case '}':
		case ',': case ';': case ':':
		case '\0':
		{
			result = false;
		} break;
		default: result = true;
	}
	return result;
}
enum class BASE {
	BINARY,
	OCTAL,
	DECIMAL,
	HEXADECIMAL,
};

Tokenizer::Tokenizer() {

	m_keyword_map["func"] = Token::TYPE::KW_FUNC;
	m_keyword_map["return"] = Token::TYPE::KW_RETURN;
	m_keyword_map["var"] = Token::TYPE::KW_VAR;

	m_keyword_map["true"] = Token::TYPE::L_TRUE;
	m_keyword_map["false"] = Token::TYPE::L_FALSE;

	m_keyword_map["print"] = Token::TYPE::SKW_PRINT;


}

auto Tokenizer::scan_tokens(const std::string& source) const -> const std::vector<Token> {

	std::vector<Token> tokens;
	CharCursor start_cur = { source }; // start cursor
	CharCursor cur_cur = start_cur; // current cursor
	const char* source_begin = source.c_str();

	while (cur_cur.peek(0) != '\0') {
		start_cur = cur_cur;
		Token token = scan_token(cur_cur.peek(0), start_cur, cur_cur);

		// Handle whitespace attribute
		{
			if (start_cur.peek(-1) == ' ') {
				token.whitespace.left = true;
			}
			if (cur_cur.peek(+1) == ' ') {
				token.whitespace.right = true;
			}

		}

		{
			if (token.type == Token::TYPE::S_PUNCTUATOR) {
				const bool left_bound = is_left_bound(start_cur);
				const bool right_bound = is_right_bound(cur_cur);

				if (left_bound == right_bound) {
					token.operator_type = Token::OPERATOR_TYPE::INFIX;
				} else if (left_bound == true) {
					token.operator_type = Token::OPERATOR_TYPE::POSTFIX;
				} else if (right_bound == true) {
					token.operator_type = Token::OPERATOR_TYPE::PREFIX;
				}
			}
		}


		if (is_skip_token(token) == false) {
			tokens.push_back(token);
		}

		if (token.type == Token::TYPE::SKW_NEWLINE) {
			cur_cur.new_line();
			cur_cur.advance();
		} else {
			cur_cur.advance();
		}
	}
	start_cur.advance();
	tokens.push_back(create_token_from_type(Token::TYPE::SKW_EOF, start_cur, cur_cur));
	return tokens;
}

auto Tokenizer::scan_token(const char c, CharCursor& start_cursor, CharCursor& current_cursor) const -> const Token {
	const Token::TYPE token_type = determine_token_type(c, current_cursor);
	const Token token = create_token_from_type(token_type, start_cursor, current_cursor);
	return token;
}


auto Tokenizer::determine_token_type(const char c, CharCursor& current_cursor) const -> const Token::TYPE {
	Token::TYPE token_type;
	switch (c) {
		case '/':
		{
			if (current_cursor.peek(1) == '/') {
				current_cursor.advance();
				while (current_cursor.peek(1) != '\n' && current_cursor.peek(1) != '\0') {
					current_cursor.advance();
				}
				token_type = Token::TYPE::SKW_COMMENT;
				break;
			} else if (current_cursor.peek(1) == '*') {
				current_cursor.advance();
				while (current_cursor.peek(1) != '*' && current_cursor.peek(2) != '/') {
					current_cursor.advance();
				}
				current_cursor.advance();
				current_cursor.advance();
				token_type = Token::TYPE::SKW_COMMENT;
				break;
			}
		};

		case ' ':
		case '\r':
		case '\t': token_type = Token::TYPE::SKW_WHITESPACE; break;
		case '\n': token_type = Token::TYPE::SKW_NEWLINE; break;

		default:
		{
			if (is_punctuator(c)) {
				token_type = lex_punctuator(current_cursor);
			} else if (is_digit(c)) {
				token_type = lex_number(current_cursor);
			} else if (is_alpha(c)) {
				token_type = lex_identifier(current_cursor);
			} else {
				token_type = Token::TYPE::SKW_UNKNOWN;
			}

		}break;
	}
	return token_type;

}
auto Tokenizer::lex_punctuator(CharCursor& current_cursor) const -> const Token::TYPE {
	const CharCursor prev_cursor = current_cursor;
	while (is_punctuator(current_cursor.peek(1)) && can_be_multichar_punctuator(current_cursor.peek(1))) {
		current_cursor.advance();
	}
	std::string lexeme(prev_cursor.get(), current_cursor.get() + 1);
	return Token::TYPE::S_PUNCTUATOR;
}
auto Tokenizer::lex_number(CharCursor& current_cursor) const -> const Token::TYPE {
	//TODO: Add floating point support
	auto (*func)(char) = is_digit;

	if (current_cursor.peek(0) == '0') {
		switch (current_cursor.peek(1)) {
			case 'b': current_cursor.advance(); func = is_binary_digit; break;
			case 'o': current_cursor.advance(); func = is_octal_digit; break;
			case 'x': current_cursor.advance(); func = is_hexadecimal_digit; break;
			case 'd': current_cursor.advance(); func = is_digit; break;
			default: break;
		}
	}

	while (func(current_cursor.peek(1))) {
		current_cursor.advance();
	}
	return Token::TYPE::L_NUMBER;
}
auto Tokenizer::lex_identifier(CharCursor& current_cursor) const -> const Token::TYPE {
	const CharCursor prev_cursor = current_cursor;
	while (is_alpha_numeric(current_cursor.peek(1))) {
		current_cursor.advance();
	}
	std::string lexeme(prev_cursor.get(), current_cursor.get() + 1);

	//determine whether identifier is a keyword
	Token::TYPE token_type;
	if (m_keyword_map.find(lexeme) == m_keyword_map.end()) {
		token_type = Token::TYPE::S_IDENTIFIER;
	} else {
		token_type = m_keyword_map.at(lexeme);
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
	} else if (literal.rfind("0d", 0) == 0) {
		number = literal.substr(2);
		base = 10;
	} else {
		number = literal;
		base = 10;
	}
	return { number, base };
}
static auto is_lone_escape_string(const std::string& str) -> bool {
	bool result = false;
	if (str.size() == 1) {
		char c = str[0];
		switch (c) {
			case '\n':
			case '\t':
			case '\r': result = true; break;
			default: result = false; break;
		}
	} else {
		result = false;
	}
	return result;
}
static auto escape_if_escape_string(const std::string& str) -> std::string {
	std::string result;
	bool is_escape = is_lone_escape_string(str);
	if (is_escape == true) {
		if (str == "\n") {
			result = "\\n";
		} else if (str == "\t") {
			result = "\\t";
		} else if (str == "\r") {
			result = "\\r";
		} else {

			assert(("not yet implemented", false));
		}
	} else {
		result = str;
	}
	return result;
}



auto Tokenizer::create_token_from_type(const Token::TYPE type, CharCursor& start_cursor, CharCursor& current_cursor) const -> const Token {
	Token token;
	std::string lexeme(start_cursor.get(), current_cursor.get() + 1);


	lexeme = escape_if_escape_string(lexeme);

	if (type == Token::TYPE::L_NUMBER) {
		auto [number, base] = process_number(lexeme);
		int num = std::stoi(number, 0, base);
	}
	const Span span = {
		.start = start_cursor.m_loc,
		.end = current_cursor.m_loc,
	};
	token = Token{ type, lexeme, span };
	return token;
}


auto CharCursor::get() const -> const char* {
	return m_it;
}
auto CharCursor::advance() const -> void {
	m_it++;
	m_loc.column++;
}
auto CharCursor::new_line() const -> void {
	//m_it++;
	m_loc.line++;
	m_loc.column = 0;
}
auto CharCursor::peek(int n) const -> const char {
	return *(m_it + n);
}