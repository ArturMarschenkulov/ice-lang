#pragma once
#include "token.h"
#include <memory>
#include <vector>

class ExprLiteral;
class ExprBinary;
class ExprUnaryPrefix;
class ExprUnaryPostfix;
class ExprGrouping;
class ExprVariable;
class ExprBlock;

class Expr {
public:
	virtual ~Expr() = default;

	class Visitor {
	public:
		virtual auto visit(const ExprLiteral& epxr) -> void = 0;
		virtual auto visit(const ExprBinary& epxr) -> void = 0;
		virtual auto visit(const ExprUnaryPrefix& expr) -> void = 0;
		virtual auto visit(const ExprUnaryPostfix& expr) -> void = 0;
		virtual auto visit(const ExprGrouping& expr) -> void = 0;
		virtual auto visit(const ExprVariable& expr) -> void = 0;
		virtual auto visit(const ExprBlock& expr) -> void = 0;
	};
	virtual auto accept(Visitor&) const -> void = 0;
};

class StmtExpression;
class StmtDeclVar;
class StmtDeclFn;

class Stmt {
public:
	virtual ~Stmt() = default;

	class Visitor {
	public:
		virtual auto visit(const StmtExpression& expr) -> void = 0;
		virtual auto visit(const StmtDeclVar& expr) -> void = 0;
		virtual auto visit(const StmtDeclFn& expr) -> void = 0;
	};
	virtual auto accept(Visitor&) const -> void = 0;
};

/*===========================
	Expressions
===========================*/

//NOTE: numeric, string/char, boolean, null
class ExprLiteral : public Expr {
public:
	ExprLiteral(
		std::string lit
	) : m_lit(std::move(lit)) {

	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}
	enum class TYPE {
		INTEGER,
		FLOAT,
		BOOL,
		CHAR,
		STRING,
	};
	std::string m_lit;
};

class ExprBinary : public Expr {
public:
	ExprBinary(
		std::unique_ptr<Expr> left,
		Token op,
		std::unique_ptr<Expr> right
	) : m_left(std::move(left))
		, m_op(std::move(op))
		, m_right(std::move(right)) {

	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}
	std::unique_ptr<Expr> m_left;
	Token m_op;
	std::unique_ptr<Expr> m_right;
};
class ExprUnaryPrefix : public Expr {
public:
	ExprUnaryPrefix(Token op, std::unique_ptr<Expr> right)
		: m_op(op), m_expr(std::move(right)) {
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}
	Token m_op;
	std::unique_ptr<Expr> m_expr;
};
class ExprUnaryPostfix : public Expr {
public:
	ExprUnaryPostfix(std::unique_ptr<Expr> right, Token op)
		: m_expr(std::move(right)), m_op(op) {
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}
	std::unique_ptr<Expr> m_expr;
	Token m_op;
};
class ExprGrouping : public Expr {
public:
	ExprGrouping(std::unique_ptr<Expr> expr)
		: m_expr(std::move(expr)) {
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);

	}
	std::unique_ptr<Expr> m_expr;
};
class ExprVariable : public Expr {
public:
	ExprVariable(Token variable)
		: m_variable(variable) {
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);

	}
	Token m_variable;
};

class ExprBlock : public Expr {
public:
	ExprBlock(std::vector<std::unique_ptr<Stmt>>&& stmts)
		: m_stmts(std::move(stmts)) {
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);

	}
	std::vector<std::unique_ptr<Stmt>> m_stmts;
	std::unique_ptr<Expr> m_expr; //optional return expression

};

/*===========================
	Statements
===========================*/

class StmtExpression : public Stmt {
public:
	StmtExpression(std::unique_ptr<Expr> expr)
		: m_expr(std::move(expr)) {
	}

	auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}

	std::unique_ptr<Expr> m_expr;
};

class StmtDeclFn : public Stmt {
public:
	StmtDeclFn(Token op, std::unique_ptr<Expr> expr)
		: m_ident(std::move(op))
		, m_expr(std::move(expr)) {
	}

	auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}

	Token m_ident;
	std::unique_ptr<Expr> m_expr;
};

class StmtDeclVar : public Stmt {
public:
	StmtDeclVar(Token op, std::unique_ptr<Expr> expr)
		: m_ident(std::move(op))
		, m_expr(std::move(expr)) {
	}

	auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}

	Token m_ident;
	std::unique_ptr<Expr> m_expr;
};