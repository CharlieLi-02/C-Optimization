#include "Node.h"

Node::Node(std::string str){
    data = str;
    left = nullptr;
    right = nullptr;
}

void Node::Delete(struct Node* node){
    if (node)
    {
        Delete(node->left);
        Delete(node->right);
        delete node;   // visit => delete node
        node = nullptr;
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

Node* Node::Insert(struct Node* node, std::string str)
{
    /* If the tree is empty, add a new node */
    if (node == nullptr){
        Node* temp = new Node(str);
        return temp;
    }
    /* If the tree has this value, return nullptr */
    if (Check(node, str) != nullptr){
        return nullptr;
    }
    if (str < node->data) {
        node->left = Insert(node->left, str);
    }
    else if (str > node->data){
        node->right = Insert(node->right, str);
    }
    /* only return nullptr when one already exists*/
    return node;
}

Node* Node::Remove(struct Node* node, std::string str) {
    if (node == nullptr) {
        return node; //return nullptr when node = Null;
    }
    if (str < node->data){
        node->left = Remove(node->left, str);
    }
    else if (str > node->data) {
        node->right = Remove(node->right, str);
    }
    // if value is same as str, then this is the node to be deleted
    else {
        // Case1: node has no child
        if (node->left== nullptr && node->right== nullptr)
            return nullptr;
        // Case2: node with only one child or no child
        else if (node->left == nullptr) {
            Node* temp = node->right;
            free(node);
            return temp;
        }
        else if (node->right == NULL) {
            Node* temp = node->left;
            free(node);
            return temp;
        }
        // Case3: node with two children
        // (smallest in the right subtree)
        else{
            Node* temp = nthLargest(node, 1);
            // Copy the inorder successor's content to this node
            node->data = temp->data;
            // Delete the inorder successor
            node = Remove(node, temp->data);
        }
    }
    return node;
}

/* If the tree is empty, add a new node */
Node* Node::Check(struct Node* node, std::string str){
    Node* current = node;
    while (current != nullptr){
        if (current->data == str) {
            return current; //return non-nullptr if exist
        }
        else if (str < node->data) {
            current = current->left;
        }
        else if (str > node->data) {
            current = current->right;
        }
    }
    return nullptr;
}

void Node::Traversal(Node* node){
    if (node)
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

void Node::Clear(Node* node){
    if (node)
    {
        Clear(node->left);
        node->data = "";
        Clear(node->right);
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
