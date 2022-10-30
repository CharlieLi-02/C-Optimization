#include "AST.h"
#include "Nodes.h"
#include "Stack.h"
#include <iostream>
#include <sstream>

AST* AST::parse(const std::string& expression) {
    std::string str = expression;
    int count =  static_cast<int>(str.size());
    int num_operand = 0;
    Stack stack(count);
    std::istringstream mystream(str);
    std::string token;
        while(mystream >> token) {
            //token is operator
            if(token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "~" || token == "/" ){
                if(stack.size() < 2) {
                    throw std::runtime_error("Not enough operands.");
                }
                
                else {
                    if (token == "+"){
                        AST* target1 = stack.pop();
                        AST* target2 = stack.pop();
                        Addition* current = new Addition(target1, target2);
                        stack.push(current);
                        num_operand++;
                    }
                    else if (token == "-") {
                        AST* target1 = stack.pop();
                        AST* target2 = stack.pop();
                        Subtraction* current = new Subtraction(target1, target2);
                        stack.push(current);
                        num_operand++;
                    }
                    else if (token == "*") {
                        AST* target1 = stack.pop();
                        AST* target2 = stack.pop();
                        Multiply* current = new Multiply(target1, target2);
                        stack.push(current);
                        num_operand++;
                    }
                    else if (token == "/") {
                        AST* target1 = stack.pop();
                        AST* target2 = stack.pop();
                        Division* current = new Division(target1, target2);
                        stack.push(current);
                        num_operand++;
                    }
                    else if (token == "%") {
                        AST* target1 = stack.pop();
                        AST* target2 = stack.pop();
                        Remainder* current = new Remainder(target1, target2);
                        stack.push(current);
                        num_operand++;
                    }
                    else if (token == "~" )
                    {
                        AST* target1 = stack.pop();
                        AST* target2 = stack.pop();
                        Negation* current = new Negation(target1, target2);
                        stack.push(current);
                        num_operand++;
                    }
                    num_operand = num_operand - 2; // two operand pop from the array
                }
            }
            //test if token is double, if not, throw runtime_error
            else {
                try {
                    if(std::stod(token)){
                    double value = std::stod(token);
                        Number* temp = new Number(value);
                        stack.push(temp);
                        num_operand++;
                    }
                }
                catch(const std::invalid_argument& error) { //throw runtime_error
                    throw std::runtime_error("Invalid token: " + token);
                }
            }
        }
    //std::cout << "Test: whether quit the main" << std::endl;
    
    //convert stack into AST;
    if(num_operand == 0) {
        throw std::runtime_error("No input.");
    }
    
    if(num_operand > 1) {
        throw std::runtime_error("Too many operands.");
    }
    
    return stack.pop();
}
