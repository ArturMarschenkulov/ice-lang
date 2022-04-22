#include "ice_lang.h"
#include "tokenizer.h"
#include "token.h"
#include "parser.h"
#include "ast_printer.h"

#include "misc/result.h"
#include "3rd-party/result.hpp"

#include "llvm/IR/LLVMContext.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <span>


static auto contains_flags(const std::vector<std::string>& cl_args) -> bool {
    bool result = false;

    std::vector<std::string> flags;
    for (size_t i = 1; i < cl_args.size(); i++) {
        // Since options usually begin with an "-" or "--"
        if ('-' == cl_args[i][0]) {
            result = true;
            flags.push_back(cl_args[i]);
        }
    }


    if (result == true) {
        std::vector<const char*> valid_flags = {
            "--version", "-v", "--help", "--print-ast", "--print-token",
        };
        // Check whether those flags are valid
        bool is_valid = false;
        for (size_t i = 0; i < flags.size(); i++) {
            for (size_t j = 0; j < valid_flags.size(); j++) {
                if (flags[i] == valid_flags[j]) { is_valid = true; }
            }
            if (is_valid == false) {
                std::cout << "ice-lang: error: unrecognized command-line option '" << flags[i] << "'\n";
            }
        }
    }

    return result;
}
static auto handle_flags(const std::vector<std::string>& cl_args, IceContext* ctx) -> void {
    std::vector<const char*> options = {
        "--version",
        "--help",
        "--print-ast",
        "--print-token",
    };

    // Check whether cl_args is in options


    for (auto& arg : cl_args) {
        if ("-v" == arg || "--version" == arg) {
            ctx->state.file_needed = false;
            std::cout << "version 0.0" << std::endl;
        } else if ("--print-ast" == arg) {
            ctx->state.print_ast = true;
        } else if ("--print-token" == arg) {
            ctx->state.print_token_stream = true;
        }
    }
}
static auto to_string(const std::ostream& file) -> std::string {
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}


auto IceContext::main(const std::vector<std::string>& cl_args) -> bool {


    if (cl_args.size() < 1) {
        // This should never be reached
        assert(("there is an error", false));
    } else if (cl_args.size() == 1) {
        std::cout << "ice-lang: error: no input files" << std::endl;
    } else if (cl_args.size() > 1) {
        const bool print_args = true;
        if (true == print_args) {
            std::cout << "Argument list: ";
            for (auto& arg : cl_args) { std::cout << arg << ", "; }
            std::cout << std::endl;
        }


        bool has_options = contains_flags(cl_args);

        // if it has no options then the first argument has to be the file
        if (has_options == true) { handle_flags(cl_args, this); }

        // TODO: Maybe do the state management more functional?

        if (true == state.file_needed) {
            // NOTE: the actual file should be always be the last argument. At least for now that is how we are handling
            // it.

            std::string   filename = cl_args.back();
            std::ofstream file(filename, std::ios_base::in);
            if (file.good() == false) {
                std::cout << "ice-lang: error: no such file or directory: " << filename << std::endl;
                return false;
            }
            if (file.is_open()) {
                std::string        file_as_string = to_string(file);
                Tokenizer          tokenizer;
                std::vector<Token> tokens = tokenizer.scan_tokens(file_as_string);

                if (true == state.print_token_stream) {
                    for (size_t i = 0; i < tokens.size(); i++) { std::cout << tokens[i] << std::endl; }
                }

                Parser                                    parser;
                const std::vector<std::unique_ptr<Stmt>>& stmts = parser.parse_tokens(tokens);

                if (true == state.print_ast) {
                    ASTPrinter printer;
                    for (int i = 0; i < stmts.size(); i++) { printer.print(stmts[i]); }
                    std::cout << printer.stream.str() << std::endl;
                }
            }
        }
    }


    // llvm::LLVMContext TheContext;
    return true;
}