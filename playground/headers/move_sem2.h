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

    Foo& operator=(Foo const& other){

        if(other.i != nullptr){
            i = new int(*other.i);
        }
        return *this;
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

    {
        Foo f2;
        f2.i = new int(42);

        f1 = f2;     // heap allocation twice !!

        std::cout << &(*f2.i) << ", " << &(*f1.i) << "\n";
    }



    return 0;
}
