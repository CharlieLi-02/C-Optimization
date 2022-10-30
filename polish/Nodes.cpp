#include "Nodes.h"
#include <math.h>
#include <iomanip>
#include <sstream>
// Implement your AST subclasses' member functions here.
std::string Node::postfix() const{
    /*std::cout << this->left->left->data << std::endl;
    std::cout << this->left->right->data << std::endl;
    std::cout << this->left->notation << std::endl;
    std::cout << this->right->left->left->data << std::endl;
    std::cout << this->right->left->right->data << std::endl;
    std::cout << this->right->left->notation << std::endl;
    std::cout << this->right->right->data << std::endl;
    std::cout << this->notation << std::endl;*/
    if(this->Type() == 6){
        double value = -(this->left->data);
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }
    if(this->Type() == 0){
        return std::to_string(this->data);
    }
    else {
        std::string str = Traversal(this->left) + " " + Traversal(this->right) + " " + this->notation;
    return str;
    }
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
    std::istringstream mystream(postfix());
    stack s (static_cast<int>(postfix().size()));
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
    //std::cout << "Test2: " << "Evaluate executed" << std::endl; // delete

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
        if(node->Type() == 4)  { return v1 / v2;}
        if(node->Type() == 5) { return fmod(v1,v2);}
    }
    return -1;
}

double Node::value() const{
        std::cout << "Test3: " << "Type: " << this->data << this->Type() << " " << this->right << std::endl; // delete
    if (this->Type() == 6 && this->right == nullptr) {
        return -(this->left->data);
    }
    
    double v1 = Evaluate(this->left);
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
    if (this->Type() == 6){
        return -v1;
    }
    return -1;
}

int Node::Type() const{
    return this->type;
}

std::string Node::Traversal(Node* node) const{
    std::string RPN;
    if(node == nullptr){
        return "";
    }
    if(node->Type() == 6){
        double value = -(node->left->data);
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }
    if(node->Type() == 0){
        std::ostringstream oss;
        oss << std::setprecision(8) << std::noshowpoint << node->data;
        std::string str = oss.str();
        return str;
    }
    else {
        std::string left = Traversal(node->left);
        std::string right = Traversal(node->right);
        std::string current = node->notation;
        RPN = left + " " + right + " " + current;
        return RPN;
    }
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
