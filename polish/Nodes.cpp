#include "Nodes.h"
#include <math.h>
#include <iomanip>
#include <sstream>
#include <string>
// Implement your AST subclasses' member functions here.
std::string Node::Traversal(Node* node) const{
  // delete
    if(node == nullptr){
        return "erro";
    }
    if(node->Type() == 0){
        double value = node->data;
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }
    if(node->Type() == 6){
        std::string str = Traversal(node->left) + std::string(" ~");
        return str;
    }
    std::string str = Traversal(node->left) + " " + Traversal(node->right) + " " + node->notation;
    return str;
}

std::string Node::postfix() const{
    if(this->Type() == 6){
        std::string str = Traversal(this->left) + std::string(" ~");
        return str;
    }
    if(this->Type() == 0){
        double value = this->data;
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }
    std::string str = Traversal(this->left) + " " + Traversal(this->right) + " " + this->notation;
    return str;
}

struct stack {
    std::string *arr;
    int top;
    int capacity;
    stack(int size = 0){
        arr = new std::string [size];
        capacity = size;
        top = -1;
    }
    ~stack(){ // destructor
        delete[] arr;
    }
    void push(std::string str){
        if (top == capacity - 1)
        {
            exit(EXIT_FAILURE);
        }
            arr[++top] = str;
        }
    std::string pop(){
        if (top == -1)
        {
            exit(EXIT_FAILURE);
        }
        return arr[top--];
    }
    std::string peek(){
        if (top == -1)
        {
            exit(EXIT_FAILURE);
        }
        return arr[top];
    }
    bool empty(){
        return top == -1;
    }
};

Node::~Node(){
    
}

std::string Node::prefix() const{
    std::string target = this->postfix();
    std::istringstream mystream(target);
    stack s (static_cast<int>(target.size()));
    std::string token;
        while(mystream >> token) {
            if (token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "~" || token == "/" ) {
                std::string op1 = s.peek();
                s.pop();
                std::string op2 = s.peek();
                s.pop();
                // concat the operands and operator
                std::string temp = token + " " + op2 + " " + op1;
                // Push string temp back to stack
                s.push(temp);
            }
            // if symbol is an operand
            else {
                // push the operand to the stack
                s.push(token);
            }
        }
        std::string str = "";
        while (!s.empty()) {
            str = str + s.peek();
            s.pop();
        }
        return str;
}

Node::Node(std::string str, int style) {
    notation = str;
    type = style;
}

Node::Node(double value) {
    type = 0;
    data = value;
}

double Node::Evaluate(Node* node) const{
    if(node == nullptr) {
        //throw std::EvaluatorException("Incorrect syntax tree!");
    }
    if(node->Type() == 0) {
        return node->data;
    }
    else if(node->Type() == 6) {
        return -(Evaluate(node->left));
    }
    else {
        double v1 = Evaluate(node->left);
        double v2 = Evaluate(node->right);
        if(node->Type() == 1) { return v1 + v2;}
        if(node->Type() == 2) { return v1 - v2;}
        if(node->Type() == 3)  { return v1 * v2;}
        if(node->Type() == 4)  {
            if(v2 == 0) {
                throw std::runtime_error("Division by zero.");
            }
            return v1 / v2;
        }
        if(node->Type() == 5) {
            if(v2 == 0) {
                throw std::runtime_error("Division by zero.");
            }
            return fmod(v1,v2);
        }
    }
    return -1;
}

double Node::value() const{
    //std::cout << "Test3: " << "Type: " << this->data << this->Type() << " " << this->right << std::endl; // delete
    if (this->Type() == 6 && this->left->Type() == 0) {
        return -(this->left->data);
    }

    double v1 = Evaluate(this->left);
    if (this->Type() == 6){
        return -v1;
    }
    double v2 = Evaluate(this->right);
    if (this->Type() == 1) {
        return v1 + v2;
    }
    if (this->Type() == 2) {
        return v1 - v2;
    }
    if (this->Type() == 3){
        return v1 * v2;
    }
    if (this->Type() == 4)  {
        return v1 / v2;
    }
    if (this->Type() == 5){
        return fmod(v1,v2);
    }

    return -1;
}

int Node::Type() const{
    return this->type;
}


/*
Number::Number(double value){
    data = value;
}

Addition::Addition(Node* node1, Node* node2){
    left = node1;
    right = node2;
}

Subtraction::Subtraction(Node* node1, Node* node2){
    left = node1;
    right = node2;
}

Multiply::Multiply(Node* node1, Node* node2){
    left = node1;
    right = node2;
}

Division::Division(Node* node1, Node* node2){
    left = node1;
    right = node2;
}

Remainder::Remainder(Node* node1, Node* node2){
    left = node1;
    right = node2;
}

Negation::Negation(Node* node1, Node* node2) {
    left = node1;
    right = node2;
}*/
// To format a double for output:
//   std::ostringstream stream;
//   stream << value;
//   return stream.str();
