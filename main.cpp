#include <iostream>
#include <string>
#include "llvm/IR/LLVMContext.h"
#include "Token.h"





int main() {


    std::cout << "This is the start of ice-lang" << std::endl;


    llvm::LLVMContext TheContext;

    Token token;
    token.type = TOKEN_TYPE::KW_RETURN;

    std::cout << to_string(token.type) << std::endl;
    return 0;
}
