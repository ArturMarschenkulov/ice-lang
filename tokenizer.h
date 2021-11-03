#pragma once
#include <string>
#include <vector>
#include "Token.h"
#include <map>

class CharCursor {
public:
	CharCursor() {
	}
	CharCursor(std::string& str) {
		it = &str[0];
	}
	auto operator=(std::string& str) -> CharCursor& {
		it = &str[0];
		return *this;
	}
	auto operator=(const std::string& str) -> CharCursor& {
		it = &str[0];
		return *this;
	}
	auto get() const -> const char*;	
	auto advance() const -> void;
	auto peek(int n) const -> const char;
private:
	mutable const char* it;
};

class Tokenizer {
public:
	Tokenizer();
	auto scan_tokens(const std::string source) const -> const std::vector<Token>;
	auto scan_token(const char c) const -> const Token;

	auto determine_token_type(const char c) const -> const TOKEN_TYPE;
	auto create_token_from_type(const TOKEN_TYPE type) const -> const Token;

	auto lex_number() const -> const TOKEN_TYPE;
	auto lex_identifier() const -> const TOKEN_TYPE;

private:
	int m_line = 1;

	mutable CharCursor start_cursor;
	mutable CharCursor current_cursor;

	std::map<std::string, TOKEN_TYPE> keyword_map;
	
};