#pragma once

#include "global.h"

struct Foo{

    // Special Member Function (if not created by user the compiler automatically create one )
    // if you redefine onse of this you have to define the rest of it !!! (rule of 5 -> formally rule of 3 )
    Foo() = default;                             // default constructor
    Foo(Foo const& other) = default;             // copy constructor;
    Foo& operator=(Foo const& other) = default;  // copy assigment operator
    Foo(Foo&& other) = default;                  // move constructor
    Foo& operator=(Foo&& other) = default;       // move assigment operator
    ~Foo() = default;                            // destructor

    int* i = nullptr;
};

int workspace(){
    return 0;
}
