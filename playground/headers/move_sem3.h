#pragma once

#include "global.h"

struct Foo{
    int* i = nullptr;

    Foo() = default;
    Foo(int i)
     : i(new int(i))
    {}

    // resolving problem 1
    Foo(Foo const& other){
        if(other.i != nullptr){
            i = new int(*other.i);
        }
    }

    // copy operator
    Foo& operator=(Foo const& other){

        if(other.i != nullptr){
            i = new int(*other.i);
        }
        return *this;
    }

    // Move operator  && -> rvalue reference
    // lhs = left-hand side ( 1 + 1)
    // rhs = right hand side ( 2 )
    // 1 + 1 = 2

    // lvalue = left values ( i ) value that have an address
    // rvalue = right values ( 42 ) literal value
       // prvalue -> pure rvalue
       // xvalue -> expiring(wygasające) value eg. f2

    // int i = 42
    Foo& operator=(Foo&& other){

        if(other.i != nullptr){
            i = other.i;
            other.i = nullptr;
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

int return_prvalue(){
    return 42;
}

int workspace(){

// example 1
    Foo f1;


    {
        Foo f2;              // expiring value ideal for move
        f2.i = new int(42);

        f1 = std::move(f2); // xvalue (&&)

        std::cout << &(*f2.i) << ", " << &(*f1.i) << "\n";

        f1 = Foo(42);   // prvalue move operator
    }



    return 0;
}
