#include "Node.h"

Node::Node(){
    data = "";
    left = nullptr;
    right = nullptr;
}

Node::~Node(){
    delete left;
    delete right;
}
