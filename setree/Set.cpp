#include "Set.h"

Set::Set(){
    mRoot = nullptr;
}

Set::Set(const Set& other){
    Node* temp = (this->mRoot)->Copy(other.mRoot);
    mRoot = temp;
}

Set::Set(Set&& other){
    mRoot = other.mRoot;
    other.mRoot = nullptr;
}

Set::~Set(){
    if(mRoot != nullptr){
        delete mRoot;
    }
}

size_t Set::clear(){
    //std::cout << "clear executed on: " << this << std::endl;
    size_t count = mRoot->Count(mRoot);
    delete mRoot;
    mRoot = nullptr;
    return count;
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
    std::cout << mRoot->Notation(mRoot);
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
    if(location <= 0 || location > mRoot->Count(mRoot)){
        throw std::out_of_range("Out of Range");
    }
    return (mRoot->nthLargest(mRoot, location))->data;
}

void Set::print() const{
    if(mRoot == nullptr){
        std::cout << "-";
    }
    else {
        mRoot->Print(mRoot);
    }
    std::cout << std::endl;
}

size_t Set::remove(const std::string& value){
    if(mRoot->Remove(mRoot, value) != nullptr){
        return 1;
    }
    return 0;
}
