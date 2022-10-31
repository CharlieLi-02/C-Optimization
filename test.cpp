#include <iostream>
#include "AST.h"
// If you want a different main function, write it here!
// This file won't be graded - do whatever you want.

int main() {
std::cout << "> " << std::endl;
std::string line;
while(std::getline(std::cin, line)) {
    AST* ast = nullptr;

    ast = AST::parse(line);

    try {
    
    std::string result1 = ast->prefix();
    std::cout << result1<< '\n';
        
    std::string result2 = ast->postfix();
    std::cout << result2 << '\n';
        
    double result3 = ast->value();
    std::cout << result3 << '\n';
    }
    
    catch(const std::runtime_error& error) {
        std::cout << "Runtime error: " << error.what() << '\n';
    }

    delete ast;
    std::cout << "> ";
    }
}
