#include "Nodes.h"

// Implement your AST subclasses' member functions here.
std::string Node::prefix() const{
    return "";
}
std::string Node::postfix() const{
    return "";
}
double Node::value() const{
    return -1;
}

Number::Number(double value){
    data = value;
}
int Number::Type(){
    return type;
}
double Number::Value(){
    return data;
}
std::string Number::prefix() const{
    return "";
}
std::string Number::postfix() const{
    return "";
}
double Number::value() const{
    return -1;
}

Addition::Addition(AST* node1, AST* node2){
    left = node1;
    right = node2;
}
int Addition::Type(){
    return type;
}
std::string Addition::prefix() const{
    return "";
}
std::string Addition::postfix() const{
    return "";
}
double Addition::value() const{
    return -1;
}

Subtraction::Subtraction(AST* node1, AST* node2){
    left = node1;
    right = node2;
}
int Subtraction::Type(){
    return type;
}
std::string Subtraction::prefix() const{
    return "";
}
std::string Subtraction::postfix() const{
    return "";
}
double Subtraction::value() const{
    return -1;
}

Multiply::Multiply(AST* node1, AST* node2){
    left = node1;
    right = node2;
}
int Multiply::Type(){
    return type;
}
std::string Multiply::prefix() const{
    return "";
}
std::string Multiply::postfix() const{
    return "";
}
double Multiply::value() const{
    return -1;
}

Division::Division(AST* node1, AST* node2){
    left = node1;
    right = node2;
}
int Division::Type(){
    return type;
}
std::string Division::prefix() const{
    return "";
}
std::string Division::postfix() const{
    return "";
}
double Division::value() const{
    return -1;
}

Remainder::Remainder(AST* node1, AST* node2){
    left = node1;
    right = node2;
}
int Remainder::Type(){
    return type;
}
std::string Remainder::prefix() const{
    return "";
}
std::string Remainder::postfix() const{
    return "";
}
double Remainder::value() const{
    return -1;
}

Negation::Negation(AST* node1, AST* node2){
    left = node1;
    right = node2;
}
int Negation::Type(){
    return type;
}
std::string Negation::prefix() const{
    return "";
}
std::string Negation::postfix() const{
    return "";
}
double Negation::value() const{
    return -1;
}
// To format a double for output:
//   std::ostringstream stream;
//   stream << value;
//   return stream.str();
