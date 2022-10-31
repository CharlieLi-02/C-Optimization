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
    void cleanup(Node* node);
    Node* left;
    Node* right;
    double Evaluate(Node* node) const;
    std::string Traversal(Node* node) const;
    std::string prefix() const;
    std::string postfix() const;
    double value() const;
    int Type() const;
};


#endif
