#pragma once

#include "token.h"
#include <memory>

class LiteralExpr;
class BinaryExpr;
class UnaryExpr;
class GroupingExpr;

class Expr {
public:
	virtual ~Expr() = default;

	class Visitor {
	public:
		virtual auto visit(const LiteralExpr& epxr) -> void = 0;
		virtual auto visit(const BinaryExpr& epxr) -> void = 0;
        virtual auto visit(const UnaryExpr& expr) -> void = 0;
        virtual auto visit(const GroupingExpr& expr) -> void = 0;
	};
	virtual auto accept(Visitor&) const -> void = 0;
};

class LiteralExpr : public Expr {
public:
	LiteralExpr(
		std::string lit
	) : m_lit(std::move(lit)) {

	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}
	std::string m_lit;
};

class BinaryExpr : public Expr {
public:
	BinaryExpr(
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
//TODO: Probably rename to PrefixUnaryExpr
class UnaryExpr : public Expr {
public:
	UnaryExpr(Token op, std::unique_ptr<Expr> right)
		: op(op), right(std::move(right)) {
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}
	Token op;
	std::unique_ptr<Expr> right;
};
class GroupingExpr: public Expr {
public:
	GroupingExpr(std::unique_ptr<Expr> expr) 
		: expr(std::move(expr)){
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
		
	}
	std::unique_ptr<Expr> expr;
};
