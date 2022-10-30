#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <cstdlib>
#include <cstddef>

class Stack {
    AST* *arr;
    int top;
    int capacity;
 
public:
    Stack(int size = 0); // constructor
    ~Stack(); // destructor
    void push(int);
    int pop(); // pop a top element
    int peek(); // return a top element
    int size(); //  return size
    bool empty(); // check if the stack is empty or not, return true if empty
    bool full(); // check if the stack is full or not, return true if full
};

#endif

