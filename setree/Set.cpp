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
    if (mRoot)
    {
        clear(mRoot->left);
        clear(mRoot->right);
        delete node;
    }
}

bool Set::contains(const std::string& value) const{
    
}

size_t Set::count() const{
    
}

void Set::debug(){
    
}

size_t Set::insert(const std::string& value){
    if (mRoot == nullptr){
        mRoot->data = value;
    }
    mRoot->Insert(mRoot, value);
}

const std::string& Set::lookup(size_t n) const{
    
}

void Set::print() const{
    
}

size_t Set::remove(const std::string& value){
    
}
