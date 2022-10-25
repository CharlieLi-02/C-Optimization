#include "Node.h"

Node::Node(std::string str){
    data = str;
    left = nullptr;
    right = nullptr;
}

Node::~Node(){
    delete left;
    delete right;
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
    if (node == NULL){
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

/*Node* Node::Remove(struct Node* node, std:string str)
{

    if (node == NULL) {
        return node;
    }

    if (str < node->data){
        node->left = deleteNode(node->left, key);
    }
    // If the key to be deleted is
    // greater than the root's
    // key, then it lies in right subtree
    else if (key > root->key) {
        node->right = deleteNode(node->right, key);
    }
    // if key is same as root's key, then This is the node
    // to be deleted
    else {
        // node has no child
        if (node->left== NULL && node->right== NULL)
            return nullptr;
        
        // node with only one child or no child
        else if (noderoot->left == NULL) {
            struct node* temp = node->right;
            free(node);
            return temp;
        }
        
        else if (node->right == NULL) {
            struct node* temp = node->left;
            free(node);
            return temp;
        }
  
        // node with two children: Get the inorder successor
        // (smallest in the right subtree)
        struct node* temp = minValueNode(root->right);
  
        // Copy the inorder successor's content to this node
        root->key = temp->key;
  
        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}*/

/* If the tree is empty, add a new node */
Node* Node::Check(struct Node* node, std::string str){
    Node* current = node;
    
    while (current != nullptr){
        if (current->data == str) {
            return nullptr; //return nullptr if exist
        }
        else if (str < node->data) {
            current = current->left;
        }
        else if (str > node->data) {
            current = current->right;
        }
    }
    return current;
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

void Node::Search(Node* node, int n){
    //std::cout << "Search funtion executed" << std::endl;
    std::cout << n - 1 << " " << std::endl;
    if (node)
    {
        Search(node->left, n);
        
        std::cout << node->data << " " << std::endl;

        /*if(n == 0){
            std::cout << node->data << " " << std::endl;
            std::cout << "terminated" << std::endl;
            return;
        }*/
        Search(node->right, n);
    }
}
