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
                        stack.~Stack();
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
                        stack.push(current);
                        num_operand--;
                    }
                    else if (token == "%") {
                        Node* current = new Node(token, 5);
                        current->right = stack.pop();
                        current->left = stack.pop();
                        stack.push(current);
                        num_operand--;
                    }
                    else if (token == "~" ) {
                        Node* current = new Node(token, 6);
                        current->left = stack.pop();
                        stack.push(current);
                    }
                    // two operand pop from the array
            }
            //test if token is double, if not, throw runtime_error
            else {
                try {
                    size_t i = 0;
                        size_t count = 0;
                        char arr[token.size()];
                        bool valid = false;
                            if(token.substr(0,1).compare("+") || token.substr(0,1).compare("-")){
                                    ++i;
                            }
                            for(; i < token.size(); i++){
                                if(!isdigit(token[i])){
                                    ++count ;
                                    arr[count] = token[i];
                                    }
                                }
                            if (count == 0){
                                valid = true;
                            }
                            if (count == 1){
                                if(arr[1] == '.'){
                                    valid = true;
                                }
                            }
                            else if (count == 2){
                                if(arr[1] == 'e' && arr[2] == '-'){
                                    valid = true;
                                }
                            }
                            else if (count == 3){
                                if(arr[1] == '.' && arr[2] == 'e' && (arr[3] == '+' || arr[3] == '-')){
                                    valid = true;
                                }
                            }
                        
                        if(!valid){
                            stack.~Stack();
                            throw std::runtime_error("Invalid token: " + token);
                        }
                            double value = std::stod(token);
                            Node* current = new Node(value);
                            stack.push(current);
                            num_operand++;
                    }
                    catch(const std::invalid_argument& error) { //throw runtime_error
                    stack.~Stack();
                    throw std::runtime_error("Invalid token: " + token);
                }
            }
        }
    
            //convert stack into AST;
            if(num_operand == 0) {
                throw std::runtime_error("No input.");
            }
            
            if(num_operand > 1) {
                throw std::runtime_error("Too many operands.");
            }
    Node * root = stack.pop();
    stack.~Stack();
    return root;
}
