#pragma once

#include "token.h"
#include <memory>

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
		virtual auto visit(const ExprLiteral& epxr) -> void = 0;
		virtual auto visit(const ExprBinary& epxr) -> void = 0;
		virtual auto visit(const ExprUnaryPrefix& expr) -> void = 0;
		virtual auto visit(const ExprUnaryPostfix& expr) -> void = 0;
        virtual auto visit(const ExprGrouping& expr) -> void = 0;
	};
	virtual auto accept(Visitor&) const -> void = 0;
};

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
		: op(op), expr(std::move(right)) {
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}
	Token op;
	std::unique_ptr<Expr> expr;
}; 
class ExprUnaryPostfix : public Expr {
public:
	ExprUnaryPostfix(std::unique_ptr<Expr> right, Token op)
		: expr(std::move(right)), op(op) {
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
	}
	std::unique_ptr<Expr> expr;
	Token op;
};
class ExprGrouping: public Expr {
public:
	ExprGrouping(std::unique_ptr<Expr> expr)
		: expr(std::move(expr)){
	}
	virtual auto accept(Visitor& visitor) const -> void override {
		visitor.visit(*this);
		
	}
	std::unique_ptr<Expr> expr;
};
