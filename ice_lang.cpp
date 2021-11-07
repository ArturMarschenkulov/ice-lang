#include "ice_lang.h"
#include "tokenizer.h"
#include "token.h"
#include "parser.h"

#include "llvm/IR/LLVMContext.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <span>


static auto contains_options(const std::vector<std::string>& cl_args) -> bool {
    bool result = false;
    for (size_t i = 2; i < cl_args.size(); i++) {
        // Since options usually begin with an "-" or "--"
        if('-' == cl_args[i][0]) {
            result = true;
        }
    }
    return result;
}
static auto to_string(const std::ostream& file) -> std::string {
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

auto IceContext::main(const std::vector<std::string>& cl_args) -> bool {
    
    if(cl_args.size() < 1) {
        // This should never be reached
        assert(false, "there is an error");
    } else if(cl_args.size() == 1) {
        // no arguments

    } else if(cl_args.size() > 1) {

        bool has_options = contains_options(cl_args);
        
        // if it has no options then the first argument has to be the file
        if(has_options == true) {
            std::cout << "not implemented yet" << std::endl;
            assert(false, "not implemented yet");
            std::cout << "not implemented yet" << std::endl;
        }
        std::string filename = cl_args[1];
        std::ofstream file(filename, std::ios_base::in);
        if(file.good() == false) {
            std::cout << "ice-lang: error: no such file or directory: " << filename << std::endl;
            return false;
        }
        if(file.is_open()) {
            std::string file_as_string = to_string(file);
            std::cout << "Tokenizer stage: " << std::endl;
            Tokenizer tokenizer;
            std::vector<Token> tokens = tokenizer.scan_tokens(file_as_string);
            for (size_t i = 0; i < tokens.size(); i++) {
                std::cout << tokens[i] << std::endl;
            }
            Parser parser;
            parser.parse_tokens(tokens);
            
        }
    }


    llvm::LLVMContext TheContext;
    return true;
}