#ifndef NODE_H
#define NODE_H

#include <string>
#include <cstddef>

// Use this file to declare your Node type.
// Implement Node member functions and helper functions in Node.cpp.
struct Node{
    std::string data;
    size_t count;
    Node* left;
    Node* right;
    Node();
    ~Node();
    Node(const std::string& value);
    Node* Insert(Node* node, std::string value);
    size_t Count(Node* node);
};

#endif
