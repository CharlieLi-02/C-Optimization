#include "Node.h"
//#include "Set.h"
#include <iostream>

// Use this file to test your Set class.
// This file won't be graded - do whatever you want.

int main() {
    Node* mRoot = new Node("C");
    mRoot->Insert(mRoot, "A");
    mRoot->Insert(mRoot, "D");
    mRoot->Insert(mRoot, "B");
    mRoot->Insert(mRoot, "B");
    mRoot->Insert(mRoot, "E");
    mRoot->Insert(mRoot, "F");
    mRoot->Insert(mRoot, "C");
    mRoot->Insert(mRoot, "G");
    mRoot->Insert(mRoot, "H");
    mRoot->Insert(mRoot, "I");

    std::cout << "attempt to print the initialized Node: " << mRoot->data << std::endl << std::endl;
    
    //std::cout << "attempt to print the second Node: " << mRoot->left->data << std::endl << std::endl;
    
    std::cout << "attempt to print the third Node: " << mRoot->right->right->data << std::endl << std::endl;
    
    std::cout << "attempt to execute Count function: " << mRoot->Count(mRoot) << std::endl << std::endl;
    
    std::cout << "attempt to execute Check function: ";
    Node* temp = mRoot->Check(mRoot, "Test5");
    if(temp) {
        std::cout << temp->data;
    }
    else {
        std::cout << "No this value found in tree";
    }
    std::cout << std::endl << std::endl;
    
    std::cout << "attempt to execute Traversal function" << std::endl;
    mRoot->Traversal(mRoot);
    std::cout << std::endl << std::endl;
    
    std::cout << "attempt to execute Traversal_Reverse function" << std::endl;
    mRoot->Traversal_Reverse(mRoot);
    std::cout << std::endl << std::endl;
 
    /*std::cout << "attempt to execute Clear function" << std::endl;
    mRoot->Clear(mRoot);*/
    
    std::cout << "attempt to execute Traversal function after Clear" << std::endl;
    mRoot->Traversal(mRoot);
    std::cout << std::endl << std::endl;
    
    std::cout << "attempt to execute nthLargest function" << std::endl;
    mRoot->nthLargest(mRoot, 6);
    std::cout << std::endl << std::endl;

    return 0;
}

