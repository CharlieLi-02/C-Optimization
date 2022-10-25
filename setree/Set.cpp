#include "Set.h"

Set::Set(){
    mRoot = new Node();
}

Set::Set(const Set& other){

}

Set::Set(Set&& other){
    mRoot = other.mRoot;
    other.mRoot = nullptr;
}

Set::~Set(){
    delete mRoot;
}

size_t Set::clear(){
    return -1;
}

bool Set::contains(const std::string& value) const{
    return -1;
}

size_t Set::count() const{
    return -1;
}

void Set::debug(){
    
}

size_t Set::insert(const std::string& value){
    return -1;
}

const std::string& Set::lookup(size_t n) const{
    std::string str = "Hello World";
    return str;
}

void Set::print() const{
    
}

size_t Set::remove(const std::string& value){
    return -1;
}
