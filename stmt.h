#pragma once
#include <memory>
#include "expr.h"

class StmtExpression;
class StmtDeclVar;
//class StmtVar;
//class StmtBlock;
//class StmtPrint;

class Stmt {
public:
	virtual ~Stmt() = default;

	class Visitor {
	public:
		virtual auto visit(const StmtExpression& expr) -> void = 0;
		virtual auto visit(const StmtDeclVar& expr) -> void = 0;
		//virtual auto visit(const StmtBlock& expr) -> void = 0;
		//virtual auto visit(const StmtPrint& expr) -> void = 0;
	};
	virtual auto accept(Visitor&) const -> void = 0;
};

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

class StmtDeclVar : public Stmt {
public:
	StmtDeclVar(Token op, std::unique_ptr<Expr> expr)
		: m_ident(std::move(op))
		, m_expr(std::move(expr)) 
	{
	}

	auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}

	Token m_ident;
	std::unique_ptr<Expr> m_expr;
};