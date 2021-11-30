#pragma once

#include "token.h"
#include "ast.h"

#include <vector>
#include <memory>
#include <iostream>
#include <string>



static auto get_unary_operator_precedence(const Token& token) -> int {
	int precedence = 0;
	if (token.type == Token::TYPE::S_PUNCTUATOR) {
		auto& op = token.lexeme;
		if (op == "+"
			|| op == "-"
			|| op == "!"
			) {
			precedence = 6;
		}
	} else {
		precedence = 0;
	}
	return precedence;
}
static auto get_binary_operator_precedence(const Token& token) -> int {
	int precedence = 0;

	if (token.type == Token::TYPE::S_PUNCTUATOR) {
		auto& op = token.lexeme;
		if (op == "*"
			|| op == "/"
			) {
			precedence = 5;
		}
		if (op == "+"
			|| op == "-"
			) {
			precedence = 4;
		}
		if (op == "<"
			|| op == ">"
			) {
			precedence = 3;
		}
		if (op == "&"
			) {
			precedence = 2;
		}
		if (op == "|"
			) {
			precedence = 1;
		}
	} else {
		precedence = 0;
	}
	//switch (token.type) {
	//case Token::TYPE::P_STAR:
	//case Token::TYPE::P_SLASH: precedence = 5; break;

	//case Token::TYPE::P_PLUS:
	//case Token::TYPE::P_MINUS: precedence = 4; break;

	//case Token::TYPE::P_LESS:
	////case TOKEN_TYPE::P_LESS_EQUAL:
	//case Token::TYPE::P_GREATER:
	////case TOKEN_TYPE::P_GREATER_EQUAL:
	////case TOKEN_TYPE::P_EQUAL_EQUAL:
	////case TOKEN_TYPE::P_BANG_EQUAL: 
 //       precedence = 3; break;

	//case Token::TYPE::P_AMPERSAND:
	////case TOKEN_TYPE::P_AMPERSAND_AMPERSAND: 
 //       precedence = 2; break;

	//case Token::TYPE::P_PIPE:
	////case TOKEN_TYPE::P_PIPE_PIPE: 
 //       precedence = 1; break;

	//case Token::TYPE::SKW_UNKNOWN: __debugbreak(); precedence = -1; break;
	//default: precedence = 0;
	//}
	return precedence;
}

struct TokenCursor {
	TokenCursor() {
	}

	TokenCursor(const TokenCursor& o) {
		m_it = o.m_it;
	}
	TokenCursor(const std::vector<Token>& tokens) {
		m_it = &tokens[0];
	}
	auto operator=(const std::vector<Token>& tokens) -> TokenCursor& {
		m_it = &tokens[0];
		return *this;
	}
	auto get() const -> const Token* {
		return m_it;
	}
	auto advance() -> void {
		m_it++;
	}
	auto peek(int n) const -> const Token& {
		return *(m_it + n);
	}
	auto is_at_end() const -> bool {
		return this->peek(0).type == Token::TYPE::SKW_EOF;
	}

	auto expect(const std::string& expected_lexeme/*, const std::string& error_string*/) const -> void {
		if(expected_lexeme != this->peek(0).lexeme) {
			std::cout << "Expected " << expected_lexeme << ", but got" << this->peek(0).lexeme << std::endl;
		}
	}
	//auto match(Token::TYPE type) const -> bool {
	//	return this->peek(0).type == type;
	//}
	auto match(std::initializer_list<Token::TYPE> types) -> bool {
		for (const Token::TYPE& type : types) {
			if (this->peek(0).type == type && !this->is_at_end()) {
				return true;
			}
		}
		return false;
	}
private:
	mutable const Token* m_it;
};

class Parser {
public:
	auto parse_tokens(const std::vector<Token>& tokens) ->std::vector<std::unique_ptr<Stmt>>;

	TokenCursor m_tc;
};