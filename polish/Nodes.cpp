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

std::string Node::prefix() const{
    if(this->Type() == 0){
        std::ostringstream stream;
        stream << this->data;
        return stream.str();
    }
    if(this->Type() == 6){
        return (std::string("~ ") + (this->left)->prefix());
    }
        return (this->notation + " " + (this->left)->prefix() + " " + (this->right)->prefix());
}

Node::Node(std::string str, int style) {
    //std::cout << "constructor is called " << this << std::endl;
    notation = str;
    type = style;
}

Node::Node(double value) {
    //std::cout << "constructor is called " << this << std::endl;
    type = 0;
    data = value;
}

double Node::Evaluate(Node* node) const{
    /*if(node == nullptr) {
        return -1;
    }*/
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
    if (this->Type() == 0){
        return this->data;
    }

    if (this->Type() == 6 && this->left->Type() == 0) {
        return -(this->left->data);
    }
    
    if(this->left != nullptr){
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
        if(v2 == 0) {
            throw std::runtime_error("Division by zero.");
        }
        return v1 / v2;
    }
    if (this->Type() == 5){
        if(v2 == 0) {
            throw std::runtime_error("Division by zero.");
        }
        return fmod(v1,v2);
    }
    }

    return -1;
}

int Node::Type() const{
    return this->type;
}

Node::~Node(){
        if(this->type == 0){
           //std::cout <<  "destructor is called " << this << " on a number " << this->data << std::endl;
        }
        else if(this->type == 6){
           delete (this->left);
           //std::cout <<  "destructor is called " << this << "on a ~ " << std::endl;
        }
        else {
           //std::cout <<  "destructor is called " << this << std::endl;
           delete (this->left);
           delete (this->right);
        }
}


void Node::cleanup(Node* node) {
    //std::cout <<  "destructor is called " << this << std::endl;
    if (node == nullptr) {
        return;
    }
    cleanup(node->left);
    cleanup(node->right);
    free(node);
}

// To format a double for output:
//   std::ostringstream stream;
//   stream << value;
//   return stream.str();
