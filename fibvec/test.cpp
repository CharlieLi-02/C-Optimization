#include "FibVec.h"
#include <iostream>

// Use this file to test your FibVec class!
// This file won't be graded - do whatever you want.


int main() {

    FibVec test;
    /*test.push(1);
    test.print();
    cout << endl;
    test.push(1);
    test.print();
    cout << endl;
    test.push(1);
    test.print();
    cout << endl;*/
    //test.insert(2, 0);
    //test.insert(3, 0);
    size_t index = 0;
    for(int i = 0; i <= 50; i ++){
        test.insert(i, index);
        //cout << "Capacity:" << test.capacity() << endl;
        //test.print();
        //cout << endl;
    }
    test.print();
    cout << endl;
    
    for(int i = 0; i <= 50; i++){
        cout << test.pop() << endl;
        cout << "[ " << test.count() << "/" << test.capacity() << " ]" << endl;
        //test.print();
        //cout << endl;
    }
    cout << endl << "after pop:" << endl;
    test.print();
    cout << endl;
    
    
    for(int i = 0; i <= 50; i++){
        test.push(i);
        //cout << "Capacity:" << test.capacity() << endl;
    }
    cout << endl << "after push:" << endl;
    test.print();
    cout << endl;
    
    for(int i = 2; i < test.count(); i = i + 2){
        cout << test.remove(i) << endl;
    }
    
    cout << test.capacity() << endl;
    cout << test.count() << endl;
    cout << endl << "final result: ";
    
    test.print();
    
    cout << endl;
    return 0;
}
