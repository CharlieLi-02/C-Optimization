#include <cstddef>
using namespace std;

class FibVec {
    public:
        FibVec();
        //The default constructor creates an empty vector with a capacity of one.
    
        ~FibVec();
        //The destructor cleans up any allocated memory.
    
        void resize(size_t index);
    
        size_t capacity() const;
        //The capacity function returns the total size of the storage buffer.
    
        size_t count() const;
        //The count function returns the number of items stored in the vector.
    
        void insert(int value, size_t index);
        //The insert function takes two arguments: the first is a value and the second is an index. It stores the value at the given index. If the index is invalid, it throws a std::out_of_range exception.
    
        int lookup(size_t index) const;
        //The lookup function takes one argument: an index. It returns the value stored at that index. If the index is invalid, it throws a std::out_of_range exception.
        
        int pop();
        //The pop function takes no arguments. It removes and returns the last value in the vector. If the vector is empty, it throws a std::underflow_error.
    
        void push(int index);
        //The push function takes one argument: a value. It inserts that value at the end of the vector. It has no return value.
    
        int remove(size_t index);
        //The remove function takes one argument: an index. It removes the value stored at that index and returns it. If the index is invalid, it throws a std::out_of_range exception.
    
        void print();
    
    protected:
        int fibIndex = 2;
        int data[1];
        int *vector;
        size_t Capacity;
        size_t Count;
        size_t const Min = 0;
};
