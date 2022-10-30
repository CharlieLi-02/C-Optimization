#ifndef NODES_H
#define NODES_H
#include <iostream>
#include "AST.h"

// Declare your AST subclasses here.
// Implement their member functions in Nodes.cpp.
class Node: public AST{
private:
    int type;
public:
    double data = 0;
    std::string notation = "";
    Node(std::string str, int style);
    Node(double value);
    ~Node();
    void Cleanup(Node* node);
    Node* left;
    Node* right;
    double Evaluate(Node* node) const;
    std::string Traversal(Node* node) const;
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
    int Type() const;
};
/*
class Number: public Node{
private:
    int type = 0;  //Number type is 0 or false;
public:
    double data;
    Number(double value);
};

class Addition: public Node{
private:
    int type = 1;  //Operator type is 1;
public:
    Node* left = nullptr;
    Node* right = nullptr;
    Addition(Node* node1, Node* node2);
};

class Subtraction: public Node{
private:
    int type = 2; //Operator type is 2;
public:
    Node* left = nullptr;
    Node* right = nullptr;
    Subtraction(Node* node1, Node* node2);
};

class Multiply: public Node{
private:
    std::string notation;
    int type = 3; //Operator type is 3;
public:
    Node* left = nullptr;
    Node* right = nullptr;
    Multiply(Node* node1, Node* node2);
};

class Division: public Node{
private:
    std::string notation;
    int type = 4; //Operator type is 4;
public:
    Node* left = nullptr;
    Node* right = nullptr;
    Division(Node* node1, Node* node2);
};

class Remainder: public Node{
private:
    std::string notation;
    int type = 5;//Operator type is 5;
public:
    Node* left = nullptr;
    Node* right = nullptr;
    Remainder(Node* node1, Node* node2);
};

class Negation: public Node{
private:
    std::string notation;
    int type = 6; //Operator type is 6;
public:
    Node* left = nullptr;
    Node* right = nullptr;
    Negation(Node* node1, Node* node2);
};*/

#endif
