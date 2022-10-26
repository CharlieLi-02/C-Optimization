#include "Set.h"

Set::Set(){
    mRoot = new Node();
}

Set::Set(const Set& other){
    Node* temp = mRoot->Copy(other.mRoot);
    mRoot = temp;
}

Set::Set(Set&& other){
    mRoot = other.mRoot;
    other.mRoot = nullptr;
}

Set::~Set(){
    delete mRoot;
}

size_t Set::clear(){
    mRoot->Clear(mRoot);
    return mRoot->Count(mRoot);
}

bool Set::contains(const std::string& value) const{
    if(mRoot->Check(mRoot, value) != nullptr){
        return true;
    }
    return false;
}

size_t Set::count() const{
    return mRoot->Count(mRoot);
}

void Set::debug(){
    mRoot->Traversal(mRoot);
}

size_t Set::insert(const std::string& value){
    if(mRoot->Check(mRoot, value) != nullptr) {
        return 0;
    }
    else {
        mRoot = mRoot->Insert(mRoot, value);
    }
    return 1;
}

const std::string& Set::lookup(size_t n) const{
    size_t location = mRoot->Count(mRoot) - n;
    return (mRoot->nthLargest(mRoot, location))->data;
}

void Set::print() const{
    
}

size_t Set::remove(const std::string& value){
    if(mRoot->Remove(mRoot, value) != nullptr){
        return 1;
    }
    return 0;
}
