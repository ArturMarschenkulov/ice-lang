#include "parser.h"
#include "token.h"
#include "ast_printer.h"
#include <iostream>
#include <ostream>
#include <cassert>


/*
Grammar notation remainder


RULE
	= 'r'+
same as
RULE
	= 'r' RULE
	| 'r'

RULE
	= 'r'*

RULE
	= 'r'
	|

same as

RULE
	= RULE+



... = something

...* = zero or more
...+ = one or more
(...) = optional
*/

/*
0
expr
	= expr_prefix (expr_binary)

list_expr
	= expr
	| expr ',' list_expr

expr_binary
	= op_binary expr_prefix
	| expr_binary (expr_binary)
	| expr_binary*

expr_prefix
	= (op_prefix) expr_postfix

expr_postfix
	= expr_primary
	| expr_postfix op_postfix

expr_primary
	= expr_literal
	| expr_group

*/


/*

basic trick to transform left recursive grammars into non-left recursive grammar


bin
	= bin expr bin

expr
	= expr a
	| b

becomes

expr
	= b expr'
expr'
	= a expr'
	| 0
*/



/*
ideal grammar:

op
	= ...


expr
	= expr_literal
	| expr_binary
	| expr


expr_binary
	= expr OP expr

expr_bin
*/

struct BindingPower {
	float left = 0.0f;
	float right = 0.0f;

	auto is_zero() -> bool {
		if(left == 0.0f && right == 0.0f) {
			return true;
		} else {
			return false;
		}
	}
};

static auto get_infix_binding_power(const Token& token) -> BindingPower {
	BindingPower binding_power;
	if (token.type != Token::TYPE::S_PUNCTUATOR) {
		return { 0, 0 };
	};

	const std::string& op = token.lexeme;
	const float eps = 0.1f;
	if (
		op == "*"|| 
		op == "/"
		) {
		float prec = 5.0f;
		binding_power = { prec, prec + eps };
	} else if (
		op == "+" || 
		op == "-"
		) {
		const float prec = 4.0f;
		binding_power = { prec, prec + eps };
	}

}

static auto parse_expr_primary(Parser* self) -> std::unique_ptr<Expr> {
	std::unique_ptr<Expr> expr;
	if (self->m_tc.match({ Token::TYPE::L_NUMBER })) {
		const Token lit = self->m_tc.peek(0); self->m_tc.advance();
		expr = std::make_unique<ExprLiteral>(lit.lexeme);
	}
	return expr;
}


static auto parse_expr_unary_postfix(Parser* self) -> std::unique_ptr<Expr> {
	std::unique_ptr<Expr> expr;
	if (self->m_tc.peek(1).type == Token::TYPE::S_PUNCTUATOR) {
		Token op = self->m_tc.peek(1);
		//check whether it qualifies as a postfix operator
		if (op.operator_type == Token::OPERATOR_TYPE::POSTFIX) {
			std::unique_ptr<Expr> inside = parse_expr_primary(self);
			self->m_tc.advance();
			expr = std::make_unique<ExprUnaryPostfix>(std::move(inside), op);
		} else {
			expr = parse_expr_primary(self);
		}

	} else {
		expr = parse_expr_primary(self);
	}
	return expr;
}


static auto parse_expr_unary_prefix(Parser* self) -> std::unique_ptr<Expr> {
	std::unique_ptr<Expr> expr;
	if (self->m_tc.peek(0).type == Token::TYPE::S_PUNCTUATOR) {
		Token op = self->m_tc.peek(0);
		//check whether it qualifies as a prefix operator
		if (op.operator_type == Token::OPERATOR_TYPE::PREFIX) {
			self->m_tc.advance();
			std::unique_ptr<Expr> inside = parse_expr_unary_postfix(self);
			expr = std::make_unique<ExprUnaryPrefix>(op, std::move(inside));
		} else {
			assert(false);
		}
	} else {
		expr = parse_expr_unary_postfix(self);
	}
	return expr;
}


//NOTE: more or less the entry expression function
static auto parse_expr_binary_2(Parser* self, int parent_prec) -> std::unique_ptr<Expr> {
	std::unique_ptr<Expr> left;

	int un_op_prec = get_unary_operator_precedence(self->m_tc.peek(0));
	if (un_op_prec != 0 && un_op_prec >= parent_prec) {
		left = parse_expr_unary_prefix(self);
	} else {
		left = parse_expr_primary(self);
	}

	while (true) {
		int bin_op_prec = get_binary_operator_precedence(self->m_tc.peek(0));
		if (bin_op_prec == 0 || bin_op_prec <= parent_prec) {
			break;
		}
		const Token op = self->m_tc.peek(0); self->m_tc.advance();
		std::unique_ptr<Expr> right = parse_expr_binary_2(self, bin_op_prec);
		left = std::make_unique<ExprBinary>(std::move(left), op, std::move(right));
	}

	return left;
}

static auto parse_expr_binary(Parser* self, float prev_bp) -> std::unique_ptr<Expr> {
	std::unique_ptr<Expr> left;

	int un_op_prec = get_unary_operator_precedence(self->m_tc.peek(0));
	if (un_op_prec != 0 && un_op_prec >= prev_bp) {
		left = parse_expr_unary_prefix(self);
	} else {
		left = parse_expr_unary_postfix(self);
	}

	while (true) {
		BindingPower bp = get_infix_binding_power(self->m_tc.peek(0));
		//int bin_op_prec = get_binary_operator_precedence(self->m_tc.peek(0));
		if (bp.is_zero() || bp.left <= prev_bp) {
			break;
		}
		const Token op = self->m_tc.peek(0); self->m_tc.advance();
		std::unique_ptr<Expr> right = parse_expr_binary(self, bp.right);
		left = std::make_unique<ExprBinary>(std::move(left), op, std::move(right));
	}

	return left;
}

static auto parse_expr(Parser* self) -> std::unique_ptr<Expr> {
	std::unique_ptr<Expr> expr = parse_expr_binary(self, 0);
	return expr;
}




auto Parser::parse_tokens(const std::vector<Token>& tokens) -> void {
	//TokenCursor tc = { tokens };
	m_tc = { tokens };

	std::unique_ptr<Expr> expr = parse_expr(this);

	ASTPrinter printer;
	printer.print(expr);
	std::cout << printer.stream.str() << std::endl;

}
auto Parser::parse_expr_unary_prefix(int parent_prec) -> std::unique_ptr<Expr> {

	Token op = m_tc.peek(0); m_tc.advance();
	if (op.whitespace.right == true) {
		std::cout << "error: unary operator cannot be separated from its operand" << std::endl;
		assert(false && "error: unary operator cannot be separated from its operand\n");
	}
	if (m_tc.peek(1).type == Token::TYPE::S_PUNCTUATOR) {

	}
	std::unique_ptr<Expr> right = this->parse_expr_binary(parent_prec);
	auto expr = std::make_unique<ExprUnaryPrefix>(op, std::move(right));
	return expr;
}
auto Parser::parse_expr_unary_postfix(int parent_prec) -> std::unique_ptr<Expr> {

	auto op = m_tc.peek(1);
	std::unique_ptr<Expr> left = this->parse_expr_binary(parent_prec);
	auto expr = std::make_unique<ExprUnaryPostfix>(std::move(left), op);
	return expr;
}

auto Parser::parse_expr_binary(int parent_prec) -> std::unique_ptr<Expr> {

	//auto e = parse_expr_unary_prefix_(this);
	std::unique_ptr<Expr> left;
	int un_op_prec = get_unary_operator_precedence(m_tc.peek(0));
	if (un_op_prec != 0 && un_op_prec >= parent_prec) {
		left = this->parse_expr_unary_prefix(un_op_prec);
	} else {
		left = this->parse_expr_primary();
	}



	while (true) {
		int bin_op_prec = get_binary_operator_precedence(m_tc.peek(0));
		if (bin_op_prec == 0 || bin_op_prec <= parent_prec) {
			break;
		}
		const Token op = m_tc.peek(0); m_tc.advance();
		std::unique_ptr<Expr> right = this->parse_expr_binary(bin_op_prec);
		left = std::make_unique<ExprBinary>(std::move(left), op, std::move(right));
	}

	return left;
}

auto Parser::parse_expr_primary() -> std::unique_ptr<Expr> {
	std::unique_ptr<Expr> expr;
	if (m_tc.match({ Token::TYPE::L_NUMBER })) {
		const Token lit = m_tc.peek(0); m_tc.advance();
		expr = std::make_unique<ExprLiteral>(lit.lexeme);
	}
	return expr;
}
