#include "parser.h"
#include "token.h"

#include <iostream>
#include <ostream>
#include <cassert>
#include <utility>


/*
Grammar notation remainder

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


stmt
	= expr ';'
*/



class SymbolTable {
public:
	auto contains(const std::string symbol) -> bool {
		bool found = false;
		for (int i = 0; i < m_symbols.size(); i++) {
			if (m_symbols[i] == symbol) {
				found = true;
			}
		}
		return found;
	}
	auto add(const std::string symbol) -> void {
		m_symbols.push_back(symbol);
	}
public:
	std::vector<std::string> m_symbols;
};
SymbolTable g_symbol_table;


struct BindingPower {
	float left = 0.0f;
	float right = 0.0f;

	auto is_zero() -> bool {
		if (left == 0.0f && right == 0.0f) {
			return true;
		} else {
			return false;
		}
	}
};

static auto get_infix_binding_power(const Token& token) -> BindingPower {
	if (token.type != Token::TYPE::S_PUNCTUATOR) {
		return { 0, 0 };
	};

	enum class ASSOCIATIVITY {
		LEFT,
		RIGHT,
		//NONE,
	};

	auto get_asso = [](ASSOCIATIVITY asso) -> std::pair<float, float> {
		switch (asso) {
			case ASSOCIATIVITY::LEFT:
			{
				return { 1.0f, 0.0f };
			} break;
			case ASSOCIATIVITY::RIGHT:
			{
				return { 0.0f, 1.0f };
			} break;
			default: assert(false);
		}
	};


	BindingPower binding_power;


	const std::string& op = token.lexeme;
	const float eps = 0.1f;
	if (
		op == "*" ||
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
	} else if (
		op == "="
		) {
		ASSOCIATIVITY asso = ASSOCIATIVITY::RIGHT;
		const float prec = 1.0f;

		auto [fl, fr] = get_asso(asso);
		binding_power = { prec - fl * eps, prec + fr * eps };
	}

}

/*===========================
	Parsing Expressions
===========================*/

static auto parse_expr(Parser* self) -> std::unique_ptr<Expr>;
static auto parse_stmt(Parser* self) -> std::unique_ptr<Stmt>;

static auto parse_expr_literal(Parser* self) -> std::unique_ptr<Expr> {

}

static auto parse_expr_block(Parser* self) -> std::unique_ptr<Expr> {

}

static auto parse_expr_primary(Parser* self) -> std::unique_ptr<Expr> {
	std::unique_ptr<Expr> expr;

	if (self->m_tc.match(
		{
			Token::TYPE::L_INTEGER, Token::TYPE::L_FLOAT,
			Token::TYPE::L_TRUE, Token::TYPE::L_FALSE,
			Token::TYPE::L_STRING, Token::TYPE::L_CHAR
		})) {
		const Token lit = self->m_tc.peek(0);
		self->m_tc.advance();
		expr = std::make_unique<ExprLiteral>(lit.lexeme);
	} else if (self->m_tc.peek(0).type == Token::TYPE::S_IDENTIFIER) {
		const Token var = self->m_tc.peek(0);
		if (g_symbol_table.contains(var.lexeme) == false) {
			//TODO: Error. Variable not declared.
			assert(false && "not declared variable");
		}
		self->m_tc.advance();
		expr = std::make_unique<ExprVariable>(var);
	} else if (self->m_tc.peek(0).lexeme == "(") {
		self->m_tc.advance();
		expr = parse_expr(self);
		//self->m_tc.consume
		if (self->m_tc.peek(0).lexeme == ")") {
			self->m_tc.advance();
			expr = std::make_unique<ExprGrouping>(std::move(expr));
		} else {
			assert(false && "there was a problem!");
		}
	} else if (self->m_tc.peek(0).lexeme == "{") {
		self->m_tc.advance();
		std::vector<std::unique_ptr<Stmt>> stmts;
		for(;;) {
			std::unique_ptr<Stmt> stmt = parse_stmt(self);
			stmts.push_back(std::move(stmt));
			if(self->m_tc.peek(0).lexeme == "}") {
				break;
			}
		}

		expr = std::make_unique<ExprBlock>(std::move(stmts));
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
//NOTE: Research more about Pratt parsing!!
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

/*===========================
	Parsing Statements
===========================*/

static auto parse_stmt_decl_var(Parser* self) -> std::unique_ptr<Stmt> {
	if (self->m_tc.peek(0).type != Token::TYPE::KW_VAR) {
		return nullptr;
	}

	self->m_tc.advance(); //skip "var"
	Token ident = self->m_tc.peek(0); // peek identifier
	self->m_tc.advance(); //skip identifier

	auto t = self->m_tc.peek(0).lexeme;
	if (t == ":=") {
		self->m_tc.advance();

	}
	std::unique_ptr<Expr> expr = parse_expr(self);
	std::unique_ptr<Stmt> stmt = std::make_unique<StmtDeclVar>(ident, std::move(expr));
	
	assert(self->m_tc.peek(0).lexeme == ";");
	self->m_tc.advance(); // skip ";"

	if (g_symbol_table.contains(ident.lexeme) == true) {
		//TODO: Made here an error? Multiple declaration
	} else {
		g_symbol_table.add(ident.lexeme);
	}


	return stmt;
}
static auto parse_stmt(Parser* self) -> std::unique_ptr<Stmt> {
	std::unique_ptr<Stmt> stmt;

	if (self->m_tc.peek(0).type == Token::TYPE::KW_VAR) {
		stmt = parse_stmt_decl_var(self);
	} else {
		std::unique_ptr<Expr> expr = parse_expr(self);
		if (self->m_tc.peek(0).lexeme == ";") {
			self->m_tc.advance();
			stmt = std::make_unique<StmtExpression>(std::move(expr));
		} else {
			self->m_tc.expect(";");
		}
	}
	return stmt;
}


auto Parser::parse_tokens(const std::vector<Token>& tokens) -> std::vector<std::unique_ptr<Stmt>> {
	m_tc = { tokens };


	std::vector<std::unique_ptr<Stmt>> stmts;
	while (true) {
		std::unique_ptr<Stmt> stmt = parse_stmt(this);
		stmts.emplace_back(std::move(stmt));
		//m_tc.advance();

		if (m_tc.peek(0).type == Token::TYPE::SKW_EOF) {
			break;
		}
	}



	return stmts;

}
