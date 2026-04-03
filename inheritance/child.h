#include "base.h"

class B  : public A{ 
    public:
 B(){
    std::cout<<"i am class B constructor"<<std::endl;
 }
 void funcB(const B& const_ref ){
    std::cout<<"i am B class function"<<std::endl;
 }
 void common_method(){
    std::cout<<" i am B's common method"<<std::endl;
 }
 friend B operator+(const B& obj1, const B& obj2){
    B b;
    std::cout<<" hi from b "<<std::endl;
    return b;
 }
~B(){
    std::cout<<"i am class B destructor"<<std::endl;
}
};
class K : public B {
    public:
     friend K operator+(K& obj1, K& obj2){ 
        K k;
            std::cout<< "hi from k"<<std::endl;
            //  B::operator+();  will not be able to reuse code here since operator+ is not part of B class. thus typecast K 
            (B)k= (B)obj1+ (B)obj2;
            return k;

     }
};
class C : A{ //  can call A's method in it's class but cannot use it's own object to call it 
    public:
    C(){
        std::cout<<"class C constructor"<<std::endl;
    }
    void funcC(C* ptr){
        std::cout<<"i am class C function"<<std::endl;
        funcA();
    }
    void common_method(){
        std::cout<<" i am C's common method"<<std::endl;
    }
    ~C(){
        std::cout<<"class C destructor"<<std::endl;
    }
};
