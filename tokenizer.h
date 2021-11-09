#pragma once

#include "token.h"

#include <map>
#include <string>
#include <vector>

class CharCursor {
public:
	CharCursor() {
	}
	CharCursor(const CharCursor& o) {
		m_it = o.m_it;
		m_loc = o.m_loc;
	}
	CharCursor(const std::string& str) {
		m_it = &str[0];
	}
	auto operator=(const std::string& str) -> CharCursor& {
		m_it = &str[0];
		return *this;
	}
	auto get() const -> const char*;
	auto advance() const -> void;
	auto new_line() const -> void;

	auto peek(int n) const -> const char;
public:
	mutable const char* m_it;
	mutable Location m_loc = { 1, 1 }; // starts at location 1, 1
};

class Tokenizer {
public:
	Tokenizer();
	auto scan_tokens(const std::string& source) const -> const std::vector<Token>;
	auto scan_token(const char c, CharCursor& start_cursor, CharCursor& m_current_cursor) const -> const Token;

	auto determine_token_type(const char c, CharCursor& current_cursor) const -> const Token::TYPE;
	auto create_token_from_type(const Token::TYPE type, CharCursor& start_cursor, CharCursor& current_cursor) const -> const Token;

	auto lex_punctuator(CharCursor& current_cursor) const -> const Token::TYPE;
	auto lex_number(CharCursor& current_cursor) const -> const Token::TYPE;
	auto lex_identifier(CharCursor& current_cursor) const -> const Token::TYPE;

private:

	std::map<std::string, Token::TYPE> m_keyword_map;

};