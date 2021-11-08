#pragma once
#include "parser.h"
#include "expr.h"

#include <vector>
#include <sstream>
#include <stack>


/*
	(+ 1 2)
|___BinExpr
	|___NumExpr
	|	|___NumToken 1
	|___PlusToken +
	|___NumExpr
		|___NumToken 2




*/


/*
 |___BinExpr
	  |___BinExpr
	  |	   |___NumExpr
	  |	   |	|___111
	  |	   |___Token +
	  |	   |___BinExpr
	  |			|	 |___NumExpr
	  |			|___BinExpr
	  |			|	 |	  |___222
	  |			|	 |___Token *
	  |			|	 |___NumExpr
	  |			|		  |___333
	  |			|___Token *
	  |			|___NumExpr
	  |				 |___2
	  |___Token +
	  |___NumExpr
		   |___2



*/
class ASTPrinter : public Expr::Visitor {
public:
	auto print(std::unique_ptr<Expr>& expr) -> std::string {
		stream << indent;
		expr->accept(*this);
		stream << '\n';
		return stream.str();
	}

	virtual auto visit(const BinaryExpr& expr) -> void override {
		std::string indent_str = get_indent();

		stream << indent_str; stream << "BinaryExpr"; stream << "\n";
		expr.m_left->accept(*this); //stream << "\n";
		stream << "     " << indent_str; stream << "Token " << expr.m_op.lexeme; stream << "\n";
		expr.m_right->accept(*this); //stream << "\n";

		indent_level--;
	}
	virtual auto visit(const LiteralExpr& expr) -> void override {
		std::string indent_str = get_indent();

		std::string str;
        //TODO:
		// if (std::holds_alternative<std::string>(expr.m_lit) == true) {
		// 	str = std::to_string(std::get<int>(expr.value));
		// } else if (std::holds_alternative<bool>(expr.m_lit) == true) {
		// 	if (std::get<bool>(expr.value) == true) {
		// 		str = "true";
		// 	} else {
		// 		str = "false";
		// 	}
		// } else {
		// 	str = "ERROROROROR";
		// }
		stream << indent_str; stream << "LiteralExpr"; stream << "\n";
		stream << "     " << indent_str; stream << str; stream << "\n";
		indent_level--;

	}
	virtual auto visit(const GroupingExpr& expr) -> void override {
		std::string indent_str = get_indent();

		stream << indent_str; stream << "GroupingExpr"; stream << "\n";
		expr.expr->accept(*this); //stream << "\n";

		indent_level--;
	}
	virtual auto visit(const UnaryExpr& expr) -> void override {
		indent_level++;
		std::string indent_str;
		for (int i = 0; i < indent_level; i++) {
			if (i == (indent_level - 1)) {
				indent_str += " |___";
			} else if (false) {
				indent_str += " |   ";
			} else {
				indent_str += "     ";
			}
		}

		stream << indent_str; stream << "UnaryExpr"; stream << "\n";
		//expr.left->accept(*this); //stream << "\n";
		stream << "     " << indent_str; stream << "Token " << expr.op.lexeme; stream << "\n";
		expr.right->accept(*this); //stream << "\n";

		indent_level--;
	}

private:
	auto get_indent() -> std::string {
		indent_level++;
		std::string indent_str;
		for (int i = 0; i < indent_level; i++) {
			if (i == (indent_level - 1)) {
				indent_str += " |___";
			} else if (false) {
				indent_str += " |   ";
			} else {
				indent_str += "     ";
			}
		}
		return indent_str;
	}
public:
	int indent_level = 0;
	std::string indent = "";
	std::stringstream stream;
	//std::vector<bool> level_stack;
};