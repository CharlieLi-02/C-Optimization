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

std::string Node::nthLargest(Node* node, int n){
        Node* current = node;
        Node* target = nullptr;
     
        // count variable to keep count of visited Nodes
        int count = 0;
     
        while (current != NULL) {
            // if right child is NULL
            if (current->right == NULL) {
     
                // first increment count and check if count = k
                if (++count == n)
                    target = current;
     
                // otherwise move to the left child
                current = current->left;
            }
     
            else {
                // find inorder successor of current Node
                Node* succ = current->right;
     
                while (current->left != nullptr && current->left != current){
                    succ = succ->left;
     
                    if (succ == nullptr) {
     
                        // set left child of successor to the
                        // current Node
                        succ  = current;

                        // move current to its right
                        current = current->right;
                    }
                        // restoring the tree back to original binary
                        //  search tree removing threaded links
                    else {
                        succ = nullptr;
                        if (++count == n) {
                            target = current;
                        }
                        // move current to its left child
                        current = current->left;
                    }
                }
            }
        }
        
    return target->data;
}
