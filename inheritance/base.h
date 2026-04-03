#include<iostream>
class A{
    public: 
    A(){
        std::cout<<"i am A class constructor"<<std::endl;
    }
    void funcA(){
        std::cout<<"this is class A method "<<std::endl;
    }
    ~A(){
        std::cout<<"i am A class destructor"<<std::endl;
    }

};