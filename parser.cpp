#include "parser.h"
#include "token.h"
#include "ast_printer.h"
#include <iostream>
#include <ostream>



auto Parser::parse_tokens(const std::vector<Token>& tokens) -> void {
	//TokenCursor tc = { tokens };
	m_tc = { tokens };

	std::unique_ptr<Expr> expr = this->parse_expr_binary(0);
	
	ASTPrinter printer;
	printer.print(expr);
	std::cout << printer.stream.str() << std::endl;

}

auto Parser::parse_expr_binary(int parent_prec) -> std::unique_ptr<Expr> {

	std::unique_ptr<Expr> left;
	int un_op_prec = get_unary_operator_precedence(m_tc.peek(0));
	if (un_op_prec != 0 && un_op_prec >= parent_prec) {
		const Token op = m_tc.peek(0); m_tc.advance();
		std::unique_ptr<Expr> right = this->parse_expr_binary(un_op_prec);
		//left = std::make_unique<UnaryExpr>(op, std::move(right));
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
