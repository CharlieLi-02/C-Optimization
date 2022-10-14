#include "List.h"
#include <iostream>
#include <algorithm>
//using namespace std; //whether need to used ?

const size_t Min = 0;

List::List(){
    head = nullptr;
}

List::List(const List& other) {
    Node** temp = &head;

    for(Node* list = other.head; list; list = list->next)
    {
        (*temp) = new Node(*list);
        temp = &((*temp)->next);
    }
    temp = nullptr;
}

List::List(List&& other){
    for(Node* list = other.head; list; list = list->next){
        head->data = list->data;
        head = head->next;
    }
}


List::~List(){
    Node* current = head;
    while(current != nullptr) {
        Node* temp = current;
        current = current -> next;
        delete temp;
    }
    head = nullptr;
}

size_t List::count() const{
    Node* temp = head;
    size_t count = 0;
    
    while(temp) { //!   !
        count ++;
        temp = temp->next;
    }
    return count;
}

void List::insert(const std::string& value){ // higher piority
    Node* test = new Node;
    test->data = value;
    test->next = nullptr;
    
    if(!head) { //!   !
        head = test;
        //std::cout << "insert() executed!" << " " << head->data << " " << "inserted" << std::endl;
        return;
    }
    
    Node* current = head;
    Node* pre = nullptr;
    std::string str;
    while(current != nullptr && value.compare((current->data)) > 0){
        pre = current;
        current = current->next;
        if(current != nullptr){
            str = current->data;
        }
    }
    
    if(pre == nullptr){
        pre = head;
        head = test;
        head->next = pre;
        return;
    }
    
    pre->next = test;
    test->next = current;
    //std::cout << (pre->next)->data << " " << std::endl;
    
    //std::cout << "insert() executed!" << " " << (head->next)->data << " " << "inserted" << std::endl;
}


const std::string& List::lookup(size_t index) const{
    if(index >= this->count() || index < Min){
        throw std::out_of_range("Out of Range");
    }
    Node* current = head;
    std::string *str;
    while(index > 0){
        current = current->next;
        index --;
    }
    str = &(current->data);
    return *str;
}

void List::print(bool reverse) const{ // higher piority
    size_t index = count();
    std::cout << "[";
    Node* temp;
    
    if(index > Min){
        
            if(reverse){
            while(index > 1){
                index --;
                std::cout << lookup(index) << ", ";
            }
            std::cout << lookup(0);
        }
        
            else {
            temp = head;
            while(index > 1){
                std::cout << temp->data << ", ";
                temp = temp->next;
                index --;
            }
            std::cout << temp->data;
        }
    }
    
    std::cout << "]" << std::endl;
}

std::string List::remove(size_t index){
    if(index >= this->count() || index < Min){
        throw std::out_of_range("Out of Range");
    }
    Node* current = head;
    std::string str;
    
    if(count() == Min + 1 || index == Min){
        Node* temp = head;
        head = head->next;
        return temp->data;
    }

    Node* pre = nullptr;
    while(index > 1){
        pre = current;
        current = current->next;
        index --;
    }
    
    str = current->data;
    Node* temp = current->next;
    current = nullptr;
    pre->next = temp;
    
    return str;
}

size_t List::remove(const std::string& value){
    size_t count = 0;
    size_t index = 0;
    Node* temp = head;
    while (temp) {
        if(value.compare(temp->data)){
            count ++;
            remove(index);
        }
        temp = temp->next;
        index ++;
    }
    return count;
}
