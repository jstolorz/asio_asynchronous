#pragma once

#include "global.h"

struct Foo{
   int* i = nullptr;

   Foo() = default;

   // resolving problem 1
   Foo(Foo const& other){
       if(other.i != nullptr){
           i = new int(*other.i);
       }
   }

   ~Foo(){
       if(i != nullptr){
           delete i;
           i = nullptr;
       }
   }
};

void funcByValue(Foo other);  // value semantics -> copy
void funcByRef(Foo& other);   // reference semantics

int workspace(){

// example 1
    Foo f1;
    f1.i = new int(42);   // heap allocation -> expensive


    Foo f2(f1);    // 1. error f1 and f2 point i in the same address of the memory so destructor deleted twice !!!

    std::cout << &(*f2.i) << ", " << &(*f1.i) << "\n";

    return 0;
}
