#include "Node.h"

Node::Node(std::string str){
    //std::cout <<  "constructor is called on: " << this << std::endl;
    data = str;
    left = nullptr;
    right = nullptr;
}


Node::~Node(){
    //std::cout <<  "destructor is called on: " << this << std::endl;
    if(this->left != nullptr && this->left != nullptr){
        //std::cout <<  "condition1 is called: " << std::endl;
        delete (this->left);
        delete (this->right);
    }
    else if(this->left != nullptr){
        //std::cout <<  "condition2 is called: " << std::endl;
        delete (this->left);
    }
    else if(this->right != nullptr){
        //std::cout <<  "condition3 is called: " << std::endl;
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
    if (node == nullptr) {
        return 0;
    }
    else {
        return 1 + Count(node->left) + Count(node->right);
    }
}

void Node::Print(struct Node* node){
    //std::cout << "Print executed!" << std::endl;
    if(node == nullptr){
        std::cout << "-";
        return;
    }
        
    if (node->left != nullptr || node->right != nullptr) {
        //std::cout << "Print left executed!" << std::endl;
        std::cout << "(";
        Print(node->left);
        std::cout << " ";
    }
    
    //std::cout << "Print Middele executed!" << std::endl;
    std::cout << node->data;
    
    if (node->left != nullptr || node->right != nullptr) {
        //std::cout << "Print right executed!" << std::endl;
        std::cout << " ";
        Print(node->right);
        std::cout << ")";
    }
}

Node* Node::Insert(struct Node* node, std::string str) {
    /* If the tree is empty, add a new node */
        if (node == nullptr){
            node = new Node(str);
            return node;
            //delete temp;
        }
        
        /* If the tree has this value, return nullptr */
        if (str < node->data) {
            node->left = Insert(node->left, str);
        }
        else {
            node->right = Insert(node->right, str);
        }
        /* only return nullptr when one already exists*/
        return node;
}

Node* Node::Remove(struct Node* node, std::string str) {
    if (node == nullptr)
        return node;
    if (str < node->data)
        node->left = Remove(node->left, str);
  
    // If the key to be deleted is
    // greater than the root's
    // key, then it lies in right subtree
    else if (str > node->data)
        node->right = Remove(node->right, str);
  
    // if key is same as root's key, then This is the node
    // to be deleted
    else {
        // node has no child
        if (node->left == nullptr && node->right==nullptr) {
            return nullptr;
        }
        // node with only one child or no child
        else if (node->left == nullptr) {
            Node* temp = node->right;
            free(node);
            return temp;
        }
        else if (node->right == nullptr) {
            Node* temp = node->left;
            free(node);
            return temp;
        }
  
        // node with two children: Get the inorder successor
        // (smallest in the right subtree)
        Node* temp = nthLargest(node->left, 1);
        std::cout << temp << std::endl;
        // Copy the inorder successor's content to this node
        node->data = temp->data;
  
        // Delete the inorder successor
        node->left = Remove(node->left, temp->data);
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

void Node::Traversal(Node* node){
    if(node == nullptr){
        return;
    }
    else
    {
        Traversal(node->left);
        std::cout << node->data << " ";
        Traversal(node->right);
    }
}

void Node::Traversal_Reverse(Node* node){
    if (node)
    {
        Traversal_Reverse(node->right);
        std::cout << node->data << " ";
        Traversal_Reverse(node->left);
    }
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
