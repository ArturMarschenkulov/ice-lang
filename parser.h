#pragma once

#include "token.h"
#include <vector>


static auto get_unary_operator_precedence(const Token& token) -> int {
	int precedence;
	switch (token.type) {
	case TOKEN_TYPE::P_PLUS:
	case TOKEN_TYPE::P_MINUS:
	case TOKEN_TYPE::P_BANG: precedence = 6; break;
	case TOKEN_TYPE::SKW_UNKNOWN: __debugbreak(); precedence = -1; break;
	default: precedence = 0; break;
	}
	return precedence;
}
static auto get_binary_operator_precedence(const Token& token) -> int {
	int precedence;
	switch (token.type) {
	case TOKEN_TYPE::P_STAR:
	case TOKEN_TYPE::P_SLASH: precedence = 5; break;

	case TOKEN_TYPE::P_PLUS:
	case TOKEN_TYPE::P_MINUS: precedence = 4; break;

	case TOKEN_TYPE::P_LESS:
	//case TOKEN_TYPE::P_LESS_EQUAL:
	case TOKEN_TYPE::P_GREATER:
	//case TOKEN_TYPE::P_GREATER_EQUAL:
	//case TOKEN_TYPE::P_EQUAL_EQUAL:
	//case TOKEN_TYPE::P_BANG_EQUAL: 
        precedence = 3; break;

	case TOKEN_TYPE::P_AMPERSAND:
	//case TOKEN_TYPE::P_AMPERSAND_AMPERSAND: 
        precedence = 2; break;

	case TOKEN_TYPE::P_PIPE:
	//case TOKEN_TYPE::P_PIPE_PIPE: 
        precedence = 1; break;

	case TOKEN_TYPE::SKW_UNKNOWN: __debugbreak(); precedence = -1; break;
	default: precedence = 0;
	}
	return precedence;
}

class Parser {
public:
    auto parse_tokens(std::vector<Token> tokens) -> void;
};