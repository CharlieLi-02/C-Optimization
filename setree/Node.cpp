#include "Node.h"
#include <queue>

Node::Node(){
    data = "";
    left = nullptr;
    right = nullptr;
}

Node::Node(const std::string& value){
    data = value;
    left = nullptr;
    right = nullptr;
}

Node::~Node(){
    delete left;
    delete right;
}

Node* Node::Insert(Node* node, std::string value) {
    if (node == nullptr){
        Node* temp = new Node(value);
        return temp;
    }
    
    if (node != nullptr){
        if (value < node->data){
            node->left = Insert(node->left, value);
        }
        else{
            node->right = Insert(node->right, value);
        }
        //return the (unchanged) node pointer
    }
    return nullptr;
    //might have some issue when node = nnullptr
}

size_t Node::Count(Node* node)
{
    if (!node){
        return 0;
    }

    else {
        return (1 + Count(node->left) + Count(node->right));
    }
}
