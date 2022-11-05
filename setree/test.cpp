#include "Set.h"
//#include "Set.h"
#include <iostream>

// Use this file to test your Set class.
// This file won't be graded - do whatever you want.

int main() {
    Set tree1;
    tree1.insert("d");
    tree1.insert("b");
    tree1.insert("a");
    tree1.insert("c");
    tree1.insert("e");
    tree1.insert("f");
    tree1.insert("f");
    
    std::cout << "attempt to execute count function: " << std::endl;
    std::cout <<  tree1.count() << std::endl << std::endl;

    
    std::cout << "attempt to execute print function: " << std::endl;
    tree1.print();
    std::cout << std::endl << std::endl;
    
    std::cout << "attempt to execute notation function: " << std::endl;
    std::cout <<  "results: "  << std::endl;
    tree1.debug();
    std::cout << std::endl << std::endl;
    
    std::cout << "attempt to execute clear function" << std::endl;
    tree1.clear();

    std::cout << "attempt to execute count function after clear: " << std::endl;
    std::cout <<  tree1.count() << std::endl << std::endl;
    
    std::cout << "attempt to execute print function after Clear" << std::endl;
    tree1.print();
    std::cout << std::endl << std::endl;
    
    /*std::cout << std::endl;
    std::cout <<  tree.remove("A") << std::endl;
    std::cout <<  tree.remove("B") << std::endl;
    std::cout <<  tree.remove("C") << std::endl;
    std::cout <<  tree.remove("D") << std::endl;
    std::cout <<  tree.remove("E") << std::endl;
    std::cout <<  tree.remove("F") << std::endl;
    std::cout <<  tree.remove("G") << std::endl;
    std::cout <<  tree.remove("H") << std::endl;
    std::cout <<  tree.remove("I") << std::endl;
    std::cout <<  tree.remove("J") << std::endl;
    std::cout <<  tree.remove("K") << std::endl;
    std::cout <<  tree.remove("K") << std::endl << std::endl;
    
    std::cout << "print final result: " << std::endl;
    tree.print();
    std::cout << std::endl << std::endl;
    
    std::cout << "attempt to execute count function: " << std::endl;
    std::cout <<  tree.count() << std::endl << std::endl;*/
    
    return 0;
}

/*Node* mRoot = new Node("C");
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
mRoot->Insert(mRoot, "J");
mRoot->Insert(mRoot, "J");

//std::cout << "attempt to print the initialized Node: " << mRoot->data << std::endl << std::endl;

//std::cout << "attempt to print the second Node: " << mRoot->left->data << std::endl << std::endl;

//std::cout << "attempt to print the third Node: " << mRoot->right->right->data << std::endl << std::endl;

//std::cout << "attempt to execute Count function: " << mRoot->Count(mRoot) << std::endl << std::endl;

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



std::cout << "attempt to execute nthLargest function" << std::endl;
std::cout << "print nthLargest value: " << mRoot->nthLargest(mRoot, 6)->data << std::endl;
std::cout << std::endl << std::endl;

std::cout << "attempt to execute remove function" << std::endl;
std::cout << "print removed value: " << mRoot->Remove(mRoot, "D")->data << std::endl;
mRoot->Traversal(mRoot);
std::cout << std::endl << std::endl;

std::cout << "attempt to execute Clear function" << std::endl;
mRoot->Clear(mRoot);

std::cout << "attempt to execute Traversal function after Clear" << std::endl;
mRoot->Traversal(mRoot);
std::cout << std::endl << std::endl;*/
