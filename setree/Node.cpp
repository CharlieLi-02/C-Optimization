#include "Node.h"

Node::Node(std::string str){
    data = str;
    left = nullptr;
    right = nullptr;
}

Node::~Node(){
    if(this->left != nullptr && this->left != nullptr){
        delete (this->left);
        delete (this->right);
    }
    else if(this->left != nullptr){
        delete (this->left);
    }
    else if(this->right != nullptr){
        delete (this->right);
    }
}

Node* Node::Copy(struct Node* target){
    if (target)
    {
        Node* copy = new Node(target->data);
        copy->left = Copy(target->left);
        copy->right = Copy(target->right);
        return copy;
    }
    return nullptr;
}

size_t Node::Count(Node* node){
    if (!node) {
        return 0;
    }
    else {
        return 1 + Count(node->left) + Count(node->right);
    }
}

void Node::Print(struct Node* node){
    if(node == nullptr){
        std::cout << "-";
        return;
    }
    if (node->left != nullptr || node->right != nullptr) {
        std::cout << "(";
        Print(node->left);
        std::cout << " ";
    }
    std::cout << node->data;
    if (node->left != nullptr || node->right != nullptr) {
        std::cout << " ";
        Print(node->right);
        std::cout << ")";
    }
}

Node* Node::Insert(struct Node* node, std::string str) {
        if (node == nullptr){
            node = new Node(str);
            return node;
        }
        else if (str < node->data) {
            node->left = Insert(node->left, str);
        }
        else {
            node->right = Insert(node->right, str);
        }
        /* only return nullptr when one already exists*/
        return node;
}

Node* Node::Remove(struct Node* node, std::string str) {
    if (node == nullptr) {
        return node;
    }
    else if (str < node->data) {
        node->left = Remove(node->left, str);
    }
    else if (str > node->data) {
        node->right = Remove(node->right, str);
    }
    else {
        // node has no child
        if (node->left == nullptr && node->right==nullptr) {
            operator delete (node);
            return nullptr;
        }
        // node with only one child
        else if (node->left == nullptr) {
            Node* temp = node->right;
            operator delete (node);
            return temp;
        }
        else if (node->right == nullptr) {
            Node* temp = node->left;
            operator delete (node);
            return temp;
        }
        // node with two child
        else {
        Node* temp = nthLargest(node->left, 1);
        // Copy the inorder successor's content to this node
        node->data = temp->data;
        // Delete the inorder successor
        node->left = Remove(node->left, temp->data);
        }
    }
    return node;
}

/* If the tree is empty, add a new node */
Node* Node::Check(struct Node* node, std::string str){
    Node* current = node;
    
    while (current != nullptr){
        if (current->data == str) {
            return current; //only return non-nullptr if exist
        }
        else if (str < current->data) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    return nullptr;
}

Node* Node::nthLargest(Node* node, size_t n){
        Node* current = node;
        Node* target = nullptr;
        size_t count = 0;
     
        while (current != nullptr) { // if right child is nullptr
            if (current->right == nullptr) { // first increment count and check if count = k
                if (++count == n)
                    target = current;
                current = current->left;  // otherwise move to the left child
            }
     
            else { // find inorder successor of current Node
                Node* succ = current->right;
                while (succ->left != nullptr && succ->left != current) {
                     succ = succ->left;
                }
                 if (succ->left == nullptr) {
                     // set left child of successor to the
                     // current Node
                     succ->left = current;
                     // move current to its right
                     current = current->right;
                 } // restoring the tree back to original binary //  search tree removing threaded links
                 else {
                     succ->left = nullptr;
                     if (++count == n) {
                         target = current;
                     }
                     // move current to its left child
                     current = current->left;
                 }
            }
        }
    return target;
}

std::string Node::Notation(Node* node){
    if (node == nullptr){
        return "";
    }

    if (node->left == nullptr && node->right == nullptr){
        return node->data;
    }
    
    std::string str_left = "";
    if (node->left != nullptr){
        str_left = Notation(node->left);
    }
    else {
        str_left = "-";
    }
    
    std::string str_right = "-";
    if(node->right != nullptr){
        str_right = Notation(node->right);
    }
    else {
        str_right = "-";
    }

    std::string notation = "(" + str_left + " " + node->data + " " + str_right + ")";
    return notation;
}
