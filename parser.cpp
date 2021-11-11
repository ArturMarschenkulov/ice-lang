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


auto Parser::parse_tokens(const std::vector<Token>& tokens) -> void {
	//TokenCursor tc = { tokens };
	m_tc = { tokens };

	std::unique_ptr<Expr> expr = this->parse_expr_binary(0);

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
	std::unique_ptr<Expr> right = this->parse_expr_binary(parent_prec);
	auto expr = std::make_unique<PrefixUnaryExpr>(op, std::move(right));
	return expr;
}
auto Parser::parse_expr_unary_postfix(int parent_prec) -> std::unique_ptr<Expr> {


	std::unique_ptr<Expr> left = this->parse_expr_binary(parent_prec);
	//Token op = m_tc.peek(0); m_tc.advance();
	//auto expr = std::make_unique<PostfixUnaryExpr>(std::move(left), op);
	return left;
}
auto Parser::parse_expr_binary(int parent_prec) -> std::unique_ptr<Expr> {

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
		left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
	}

	return left;
}

auto Parser::parse_expr_primary() -> std::unique_ptr<Expr> {
	std::unique_ptr<Expr> expr;
	if (m_tc.match({ Token::TYPE::L_NUMBER })) {
		const Token lit = m_tc.peek(0); m_tc.advance();
		expr = std::make_unique<LiteralExpr>(lit.lexeme);
	}
	return expr;
}
