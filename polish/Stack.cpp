#include "Stack.h"
// Implement your Stack member functions here.
// Constructor to initialize the stack

Stack::Stack(int size)
{
    arr = new Node* [size];
    capacity = size;
    top = -1;
}
 
// Destructor to free memory allocated to the stack
Stack::~Stack() {
    std::cout <<  "stack destructor is called " << arr[i] << std::endl;
    for(int i = 0; i <= top; i++) {
        if(arr[i] != nullptr) {
            delete arr[i];
        }
    }
}
 
// Utility function to add an element `x` to the stack
void Stack::push(Node* node)
{
    if (full())
    {
        exit(EXIT_FAILURE);
    }
 
    // Inserting node;
    //std::cout << node << " is added to the stack" << std::endl;
    arr[++top] = node;
}
 
// Utility function to pop a top element from the stack
Node* Stack::pop()
{
    // check for stack underflow
    if (empty())
    {
        exit(EXIT_FAILURE);
    }
    // decrease stack size by 1 and (optionally) return the popped element
    return arr[top--];
}
 
// Utility function to return the top element of the stack
Node* Stack::peek()
{
    if (!empty()) {
        return arr[top];
    }
    else {
        exit(EXIT_FAILURE);
    }
}
 
// Utility function to return the size of the stack
int Stack::size() {
    return top + 1;
}
 
// Utility function to check if the stack is empty or not
bool Stack::empty() {
    return top == -1;               // or return size() == 0;
}
 
// Utility function to check if the stack is full or not
bool Stack::full() {
    return top == capacity - 1;     // or return size() == capacity;
}
