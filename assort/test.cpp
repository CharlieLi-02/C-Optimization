
#include "List.h"
#include <iostream>

// Use this file to test your List class!
// This file won't be graded - do whatever you want.

int main() {
    List list1;

    std::cout << "This program doesn't do anything interesting yet...\n";
    list1.print();
    std::cout << std::endl;
    std::cout << list1.count() << std::endl;
    std::cout << std::endl;
    
    list1.insert("Atropos");
    list1.print();
    std::cout << std::endl;
    
    list1.insert("Clotho");
    list1.print();
    std::cout << std::endl;
    
    list1.insert("Lachesis");
    list1.print();
    std::cout << std::endl;
    
    list1.insert("Aa");
    list1.print();
    std::cout << std::endl;
    
    list1.insert("Ca");
    list1.print();
    std::cout << std::endl;
    
    list1.insert("Lb");
    list1.print();
    std::cout << std::endl;
    //std::cout << list.count() << std::endl;
    //std::cout << "print the value at index = 1 " << list.lookup(0);

    list1.print(true);
    std::cout << std::endl;
    
    list1.print();
    std::cout << std::endl;
    
    //list1.remove(0);
    //list1.print();
    //std::cout << std::endl;

    //std::cout << list.count() << std::endl;
    
    /*Test:Copy & Move{
    std::cout << std::endl;
    List* list2 = new List(list1);
    list2->print();
    std::cout << "list2" << std::endl;
    
    List* list3 = new List(list1);
    list3->print();
    std::cout << "list3" << std::endl;
    std::cout << std::endl*/
    
    
    //list1.print();
    std::cout << list1.remove(0) << std::endl;
    list1.print();
    std::cout << list1.remove(3) << std::endl;
    list1.print();
    std::cout << list1.remove(0) << std::endl;
    //std::cout << std::endl;
    list1.print();
    std::cout << list1.remove(0) << std::endl;
    list1.print();
    std::cout << list1.remove(0) << std::endl;
    //list1.print();
    std::cout << list1.remove("Lb") << std::endl;
    //list1.print();
    //std::cout << list1.remove(2) << std::endl;
    //list1.print();
    //std::cout << list1.remove(4) << std::endl;
    //list1.print();
    std::cout << std::endl;
    
    List list4;
    list4.insert("Test1");
    list4.insert("Test1");
    list4.insert("Test2");
    list4.insert("Test2");
    list4.insert("Test2");
    list4.insert("Test2");
    std::cout << list4.count() << std::endl;
    std::cout << list4.remove("Test1") << std::endl;
    //list4.print();
    //list.insert("Atropos");
    //list.insert("Clotho");
    //list.insert("Lachesis");
    
    
    return 0;
}
