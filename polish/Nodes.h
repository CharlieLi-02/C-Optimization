#ifndef NODES_H
#define NODES_H

#include "AST.h"

// Declare your AST subclasses here.
// Implement their member functions in Nodes.cpp.
class Node: public AST{
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
};

class Number: public AST{
private:
    double data;
    int type = 0;  //Number type is 0 or false;
public:
    Number(double value);
    int Type();
    double Value();
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
};

class Addition: public AST{
private:
    AST* left = nullptr;
    AST* right = nullptr;
    int type = 1;  //Operator type is 1;
public:
    Addition(AST* node1, AST* node2);
    int Type();
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
};

class Subtraction: public AST{
private:
    AST* left = nullptr;
    AST* right = nullptr;
    int type = 2; //Operator type is 2;
public:
    Subtraction(AST* node1, AST* node2);
    int Type();
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
};

class Multiply: public AST{
private:
    AST* left = nullptr;
    AST* right = nullptr;
    std::string notation;
    int type = 3; //Operator type is 3;
public:
    Multiply(AST* node1, AST* node2);
    int Type();
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
};

class Division: public Node{
private:
    AST* left = nullptr;
    AST* right = nullptr;
    std::string notation;
    int type = 4; //Operator type is 4;
public:
    Division(AST* node1, AST* node2);
    int Type();
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
};

class Remainder: public Node{
private:
    AST* left = nullptr;
    AST* right = nullptr;
    std::string notation;
    int type = 5;//Operator type is 5;
public:
    Remainder(AST* node1, AST* node2);
    int Type();
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
};

class Negation: public Node{
private:
    AST* left = nullptr;
    AST* right = nullptr;
    std::string notation;
    int type = 6; //Operator type is 6;
public:
    Negation(AST* node1, AST* node2);
    int Type();
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
};

#endif
