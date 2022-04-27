#pragma once
#include "parser.h"
#include "ast.h"

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
class ASTPrinter : public Expr::Visitor, public Stmt::Visitor {
public:
    auto print(const std::unique_ptr<Expr>& expr) -> std::string {
        stream << indent;
        expr->accept(*this);
        stream << '\n';
        return stream.str();
    }

    auto print(const std::unique_ptr<Stmt>& stmt) -> std::string {
        stream << "==========AST-TREE==========\n";
        stream << indent;
        stmt->accept(*this);
        stream << "=========~AST-TREE==========\n";
        return stream.str();
    }

    virtual auto visit(const ExprLiteral& expr) -> void override {
        std::string indent_str = get_indent();

        std::string str;
        str = expr.m_lit;
        stream << indent_str;
        stream << "ExprLiteral " << str;
        stream << "\n";
        indent_level--;
    }
    virtual auto visit(const ExprBinary& expr) -> void override {
        std::string indent_str = get_indent();

        stream << indent_str;
        stream << "ExprBinary " << expr.m_op.lexeme << "\n";
        expr.m_left->accept(*this);
        // stream << indent_str; stream << "Token " << expr.m_op.lexeme; stream << "\n";
        expr.m_right->accept(*this);

        indent_level--;
    }
    virtual auto visit(const ExprUnaryPrefix& expr) -> void override {
        indent_level++;
        std::string indent_str = get_indent();

        stream << indent_str;
        stream << "ExprUnaryPrefix";
        stream << "\n";
        // expr.left->accept(*this); //stream << "\n";
        stream << /*"     " <<*/ indent_str;
        stream << "Token " << expr.m_op.lexeme;
        stream << "\n";
        expr.m_expr->accept(*this); // stream << "\n";

        indent_level--;
    }
    virtual auto visit(const ExprUnaryPostfix& expr) -> void override {
        indent_level++;
        std::string indent_str = get_indent();


        stream << indent_str;
        stream << "ExprUnaryPostfix";
        stream << "\n";
        expr.m_expr->accept(*this); // stream << "\n";
        stream << /*"     " <<*/ indent_str;
        stream << "Token " << expr.m_op.lexeme;
        stream << "\n";

        indent_level--;
    }

    virtual auto visit(const ExprGrouping& expr) -> void override {
        std::string indent_str = get_indent();

        stream << indent_str;
        stream << "ExprGrouping";
        stream << "\n";
        expr.m_expr->accept(*this); // stream << "\n";

        indent_level--;
    }

    virtual auto visit(const ExprVariable& expr) -> void override {
        std::string indent_str = get_indent();

        stream << indent_str << "ExprVariable " << expr.m_variable.lexeme << "\n";
        // expr.m_expr->accept(*this);


        indent_level--;
    }
    virtual auto visit(const ExprBlock& expr) -> void override {
        std::string indent_str = get_indent();

        stream << indent_str << "ExprBlock "
               << "\n";
        for (int i = 0; i < expr.m_stmts.size(); i++) { expr.m_stmts[i]->accept(*this); }
        if (expr.m_expr != nullptr) { expr.m_expr->accept(*this); }

        indent_level--;
    }




    virtual auto visit(const StmtExpression& expr) -> void override {
        std::string indent_str = get_indent();

        stream << indent_str;
        stream << "StmtExpression";
        stream << "\n";
        expr.m_expr->accept(*this);

        indent_level--;
    }
    virtual auto visit(const StmtDeclVar& expr) -> void override {
        std::string indent_str = get_indent();

        stream << indent_str;
        stream << "StmtDeclVar " << expr.m_ident.lexeme << "\n";
        expr.m_expr->accept(*this);


        indent_level--;
    }
    virtual auto visit(const StmtDeclFn& expr) -> void override {
        std::string indent_str = get_indent();
        stream << indent_str;
        stream << "StmtDeclFn " << expr.m_ident << "\n";
        indent_level--;
        // TODO: implement!
    }
    virtual auto visit(const StmtDeclTypeUnit& stmt) -> void override {
        std::string indent_str = get_indent();
        stream << indent_str;
        stream << "StmtDeclTypeUnit " << stmt.m_ident << "\n";
        indent_level--;
        // TODO: implement!
    }
    virtual auto visit(const StmtDeclTypeStruct& stmt) -> void override {
        std::string indent_str = get_indent();
        stream << indent_str;
        stream << "StmtDeclTypeUnit " << stmt.m_ident << "\n";
        indent_level--;
        // TODO: implement!
    }




private:
    auto get_indent() -> std::string {
        enum class MODE {
            TREE,
            LISP
        };
        MODE        mode = MODE::LISP;
        std::string indent_str;
        switch (mode) {
            case MODE::TREE: {
                indent_level++;

                for (int i = 0; i < indent_level; i++) {
                    if (i == (indent_level - 1)) {
                        indent_str += " |___";
                    } else if (false) {
                        indent_str += " |   ";
                    } else {
                        indent_str += "     ";
                    }
                }

            } break;
            case MODE::LISP: {
                indent_level++;
                for (int i = 0; i < indent_level; i++) {
                    if (indent_level == (i + 1)) {
                        indent_str += "|";
                    } else {
                        indent_str += ".";
                    }
                }

            } break;
        }

        return indent_str;
    }

public:
    int               indent_level = 0;
    std::string       indent = "";
    std::stringstream stream;
    // std::vector<bool> level_stack;
};