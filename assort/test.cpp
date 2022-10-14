#include "List.h"
#include <iostream>

// Use this file to test your List class!
// This file won't be graded - do whatever you want.

int main() {
    List list;

    std::cout << "This program doesn't do anything interesting yet...\n";
    list.print();
    std::cout << std::endl;
    std::cout << list.count() << std::endl;
    std::cout << std::endl;
    
    list.insert("Atropos");
    list.print();
    std::cout << std::endl;
    
    list.insert("Clotho");
    list.print();
    std::cout << std::endl;
    
    list.insert("Lachesis");
    list.print();
    std::cout << std::endl;
    
    list.insert("Aa");
    list.print();
    std::cout << std::endl;
    
    list.insert("Ca");
    list.print();
    std::cout << std::endl;
    
    list.insert("Lb");
    list.print();
    std::cout << std::endl;
    //std::cout << list.count() << std::endl;
    //std::cout << "print the value at index = 1 " << list.lookup(0);
    std::cout << std::endl;
    list.print(true);
    std::cout << std::endl;
    
    //list.remove(0);
    //list.print();
    std::cout << std::endl;

    //std::cout << list.count() << std::endl;
    std::cout << std::endl;
    
    //list.insert("Atropos");
    //list.insert("Clotho");
    //list.insert("Lachesis");
    
    
    return 0;
}
