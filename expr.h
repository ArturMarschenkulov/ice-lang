#pragma once

#include "token.h"
#include <memory>

class LiteralExpr;
class BinaryExpr;
class PrefixUnaryExpr;
class PostfixUnaryExpr;
class GroupingExpr;

class ExprLiteral;
class ExprBinary;
class ExprUnaryPrefix;
class ExprUnaryPostfix;
class ExprGrouping;

class Expr {
public:
	virtual ~Expr() = default;

	class Visitor {
	public:
		virtual auto visit(const LiteralExpr& epxr) -> void = 0;
		virtual auto visit(const BinaryExpr& epxr) -> void = 0;
		virtual auto visit(const PrefixUnaryExpr& expr) -> void = 0;
		virtual auto visit(const PostfixUnaryExpr& expr) -> void = 0;
        virtual auto visit(const GroupingExpr& expr) -> void = 0;
	};
	virtual auto accept(Visitor&) const -> void = 0;
};

//NOTE: numeric, string/char, boolean, null
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
class PrefixUnaryExpr : public Expr {
public:
	PrefixUnaryExpr(Token op, std::unique_ptr<Expr> right)
		: op(op), expr(std::move(right)) {
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}
	Token op;
	std::unique_ptr<Expr> expr;
}; 
class PostfixUnaryExpr : public Expr {
public:
	PostfixUnaryExpr(std::unique_ptr<Expr> right, Token op)
		: expr(std::move(right)), op(op) {
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}
	std::unique_ptr<Expr> expr;
	Token op;
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
