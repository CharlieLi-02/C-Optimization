#include "Stack.h"
#include <iostream>
using namespace std;
// If you want a different main function, write it here!
// This file won't be graded - do whatever you want.

int main() {
    string str = "12 3 + 5 4 + 2 / * ";
    AST* root = nullptr;
    root = root->parse(str);
}
