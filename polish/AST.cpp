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
            //std::cout << "Test1: " << token << std::endl; // delete
            //token is operator
            if(token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "~"){
                if(stack.size() < 2) {
                    if (!(token == "~"  && stack.size() > 0)){
                        throw std::runtime_error("Not enough operands.");
                    }
                }
                    if (token == "+"){
                        Node* current = new Node(token, 1);
                        current->right = stack.pop();
                        current->left = stack.pop();
                        stack.push(current);
                        num_operand--;
                    }
                    else if (token == "-") {
                        Node* current = new Node(token, 2);
                        current->right = stack.pop();
                        current->left = stack.pop();
                        stack.push(current);
                        num_operand--;
                    }
                    else if (token == "*") {
                        Node* current = new Node(token, 3);
                        current->right = stack.pop();
                        current->left = stack.pop();
                        stack.push(current);
                        num_operand--;
                    }
                    else if (token == "/") {
                        Node* current = new Node(token, 4);
                        current->right = stack.pop();
                        current->left = stack.pop();
                        if(current->right->data == 0){
                            throw std::runtime_error("Division by zero.");
                        }
                        stack.push(current);
                        num_operand--;
                    }
                    else if (token == "%") {
                        Node* current = new Node(token, 5);
                        current->right = stack.pop();
                        current->left = stack.pop();
                        if(current->right->data == 0){
                            throw std::runtime_error("Division by zero.");
                        }
                        stack.push(current);
                        num_operand--;
                    }
                    else if (token == "~" )
                    {
                        Node* current = new Node(token, 6);
                        current->left = stack.pop();
                        stack.push(current);
                    }
                    // two operand pop from the array
            }
            //test if token is double, if not, throw runtime_error
            else {
                try {
                    double value = std::stod(token);
                        Node* current = new Node(value);
                        stack.push(current);
                        num_operand++;
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
