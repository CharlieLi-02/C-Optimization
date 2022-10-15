#include "List.h"
#include <iostream>
#include <algorithm>
//using namespace std; //whether need to used ?

const size_t Min = 0;

List::List(){
    head = nullptr;
}

List::List(const List& other)   {
    //std::cout << "copy constructor called!" << std::endl;
    if (other.head == nullptr) {
        head = nullptr;
        return;
    }
        head = new Node;
        Node *list = other.head;
        head->data = list->data;
        //std::cout << "test" << std::endl;
        
        while (list != nullptr) {
            list = list->next;
            Node* temp = new Node;
            temp->data = list->data;
            temp->next = nullptr;
            //std::cout << "test" << std::endl;
            head->next = temp;
            delete temp;
            head = head->next;
        }
    delete list;
}

List::List(List&& other){
    head = other.head;
    other.head = nullptr;
}

List::~List(){ // dont chage
    Node* current = head;
    while(current != nullptr) {
        Node* temp = current;
        current = temp->next;
        delete temp;
    }
}

size_t List::count() const{
    Node* current = head;
    size_t count = 0;
    
    while(current) {
        //std::cout << "countonce ";
        count ++;
        current = current->next;
    }
    delete current;
    return count;
}

void List::insert(const std::string& value){ // higher piority
    Node* test = new Node();
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
    size_t index = this->count();
    std::cout << "[";
    
    if(index > Min){
        
            if(reverse){
            while(index > 1){
                index --;
                std::cout << lookup(index) << ", ";
            }
            std::cout << lookup(0);
        }
        
            else {
                size_t cur = 0;
            while(cur < index - 1){
                std::cout << lookup(cur) << ", ";
                cur ++;
            }
            std::cout << lookup(count() - 1);
        }
    }
    
    std::cout << "]" << std::endl;
}

std::string List::remove(size_t index){
    if(index >= this->count() || index < Min){
        throw std::out_of_range("Out of Range");
    }
    
    std::string str;
    if(count() == Min + 1 || index == Min){
        Node* temp = head;
        head = head->next;
        str = temp->data;
        delete temp;
        return str;
    }

    
    Node* current = head;
    Node* pre = nullptr;
    while(index > 0){
        pre = current;
        current = current->next;
        index --;
    }
    
    str = current->data;
    pre->next = current->next;;
    
    delete current;
    
    return str;
}

size_t List::remove(const std::string& value){
    size_t count = 0;
    Node* current = head;
    while (current != NULL){
        for (size_t i = 0; i < this->count(); i++){
            if (current->data == value){
                remove(i);
                count ++;
                current = head;
                break;
            }
            current = current->next;
        }
    }
    return count;
}
