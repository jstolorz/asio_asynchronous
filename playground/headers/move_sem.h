#pragma once

struct Foo{
   int* i = nullptr;

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

    Foo f1;
    f1.i = new int(42);   // heap allocation -> expensive


    Foo f2(f1);    // error f1 and f2 point i in the same address of the memory so destructor deleted twice !!!

    return 0;
}
