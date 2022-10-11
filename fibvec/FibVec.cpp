#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include "FibVec.h"
using namespace std;

size_t Fibonacci(int n) {
   int a;
   int b;
   size_t c = 0;
   a = 0;
   b = 1;
   if (n < 0){
      return -1;
      }
   if (n==1) {
      return a;
      }
   if (n==2) {
      return b;
      }
   else{
      int i;
      for(i=0; i<n-1; i++){
         c = a + b;
         a = b;
         b = c;
      }
      return c;
   }
}
// 0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89
FibVec::FibVec() {
    vector = new int[1];
    Capacity = 1;
    Count = 0;
}

FibVec:: ~FibVec() {
    delete [] vector;
}

void FibVec::print() {
    for (size_t i = 0; i < this->count(); i++) {
        cout << this->lookup(i) << endl;
    }
}
// 0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, ...

size_t FibVec::capacity() const {
    return Capacity;
}

size_t FibVec::count() const {
    return Count;
}

void FibVec::resize(size_t size) {
    //cout << index << endl;
    if (size >= Min){
        if (size > Fibonacci(fibIndex)){
            fibIndex ++;
            Capacity = Fibonacci(fibIndex);
            int * temp = new int[Capacity];
            for(size_t i = 0; i < count() - 1; i++){
                * (temp + i) = * (vector + i); //?
            }
            delete [] vector;
            vector = temp;
            //cout << "excute resize1" << endl;//test
        }
        else if (size < Fibonacci(fibIndex - 2)){
            fibIndex --;
            Capacity = Fibonacci(fibIndex);
            int * temp = new int[Capacity];
            for(size_t i = 0; i < count(); i++){
                * (temp + i) = * (vector + i);
            }
            delete [] vector;
            vector = temp;
            //cout << "excute resize2" << endl;//test
        }
    }
}


void FibVec::insert(int value, size_t index){ //complete
    if (index > this->count() || index < Min){
        throw out_of_range("Out of range.");
    }
        Count ++;
        this->resize(this->count());
        int * temp = new int[this->capacity()];
        for(size_t i = 0; i < index; i++){
        * (temp + i) = * (vector + i);
            //cout << *(temp + i) << " test1 ";
    }
        * (temp + index) = value;
            //cout << *(temp + index) << " test2 ";
        for(size_t i = index + 1; i < this->count(); i++){
            //cout << * (vector + i - 1) << "test for vector";
        * (temp + i) = * (vector + i - 1);
            //cout << *(temp + i) << " test3 " << endl;
    }
    delete [] vector;
    vector = temp;
}

int FibVec::lookup(size_t index) const{ //complete
    if (index >= this->count() || index < Min){
        throw out_of_range("Out of Range");
    }
        return *(vector + index);
}

int FibVec::pop() { //complete
    if (Count <= Min) {
        throw underflow_error("Underflow");
    }
    int num = * (vector + Count - 1);
    Count --;
    resize(count());
    int * temp = new int[capacity()];
    for(size_t i = 0; i < count(); i++){
        * (temp + i) = * (vector + i);
    }
    delete [] vector;
    vector = temp;
        return num;
}

void FibVec::push(int value) { //complete
    //cout << "test " << this->capacity() + 1 << endl;//test
    Count ++;
    resize(this->count());
    //cout << "test " << this->capacity() << endl;//test
    int * temp = new int[this->capacity()];
    //cout << "test1 " << this->count() << endl;//test
    for(size_t i = 0; i < this->count() - 1; i++){
        * (temp + i) = * (vector + i);
        //cout << "test1 " <<  * (temp + i) << endl;//test
    }
    * (temp + this->count() - 1) = value;
        //cout << "test2 " <<  value << endl;
    delete [] vector;
    vector = temp;
}

int FibVec::remove(size_t index){
    if (index >= count() || index < Min){
        throw out_of_range("Out of Range");
    }
    int num = * (vector + index);
    resize(count() - 1);
    Count --;
    int * temp = new int[capacity()];
    for(size_t i = 0; i < index; i++){
    * (temp + i) = * (vector + i);
}
    for(size_t i = count() ; i > index; i--){
    * (temp + i - 1) = * (vector + i);
}
    delete [] vector;
    vector = temp;
    return num;
}


