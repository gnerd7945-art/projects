// first most based class constructor called then child constructor called, constructor can only call immediate base  class constructor , in destructor child destructor called first

//defining custom cunstructor removes deafult one.// child class is responsible for specifying which parent class constructor will be called. 

// when we call a function in derived class, first compiler check if any function of that name exist in derived class, if n matched then check parmas for which match best, if no function then check on base class with same logic. 

//a function that is defined as private in the base class can be reit to chain it's objects or only base object can be chained?
// derived class can use protected function of base class in there own class not in main or other outside function. 
 // parent(protected/public)-> can be called by child(inherited private) inside it's class but not it's child, had child used protected then it's child too could do the same.
/*
The Construction Sequence:

    // when we make a derived object, computer allocated one single continous memory large enought to hold (base + derived variables).
       base object only allocated memory for base class it does not know derived class even exist 

    The Base Constructor runs first. It doesn't create a new object; it simply initializes the first part of that memory block.

    The Derived Constructor runs next. It initializes the remaining part of that same memory bloc
    2) if we have a pointer  of base class to derived class object, and both have one same function signature then normally when we call that function using
    pointer, it would resolve to base object function because it refrences to base portion of derived object. But if that function is virtual, then it tells program
    to look for more derived  versions of the  function available for derived object. 

    3)normal derived object can see overall mem block, base  pointer to derived can only see what is in base part  of that memory block(and other parent part if that base is also derived.( can only look up)
     , when a class has any virtual function the compiler adds one hidden member to the Base part of the object: the vptr (Virtual Pointer).
     and when times comes to clear memory using destructor then if virtual function is used then destructor must eb virtual as well since otherwise compiler will call base pointer destructor if not specified,
     using virtual in destructor again tells compiler to check vtable and call correct classe's destructor.

     How the "Bridge" works:

    The Look: The Base* ptr looks at the Base portion of the memory.

    The Discovery: It sees the vptr sitting right there in that Base portion.

    The Redirection: Even though the pointer is a Base*, the vptr inside the object points to the Derived Class's Vtable.

    The Jump: The CPU follows the address in that vtable, which leads it directly to the code sitting in the Derived part of memory.

4)  base* ptr = new derived(); manual way
    std::unique_ptr<class_name> ptr = std::make_unique<class_name>(params);

5)  child_ptr = (child_ptr)parent_obj is allowed with typecasting, it can access base class mem and func, but if tried to access child class func, garbage read.

6)if we have even one pure virtual function then that class become abstract and it cant be initialized directly and 
derived class must override it otherwise it will become abstract as well, on contrary having just virtual function in in base 
class still let us initialize it's object directly and derived class wont be abstract aswell. override keyword just make
 compiler recheck if that function is for overriding or not. without it also works.

 7) although no body in abstract class for virtual function, outside class we can define body for it, so that when child goes
    to override it, using scope resoultion is can use common code in it. 

*/

#include "child.h"
class D :public B, public  C  {
    public: 
    using B::common_method; // 1) either define here or define in main
    int val;
    int imp_data; // public here 

    D(int&ref1):val(ref1){
        std::cout<<"class D constructor (for lvalue and  objs)with value:"<<val<<std::endl;
    }
    D(const int&ref1):val(ref1){
        std::cout<<"class D constructor(for literlas/rvalue and objs(works for lvalue too) with value:"<<val<<std::endl;
    }
    void func(){
        std::cout<<"D func"<<std::endl;
    }
    ~D(){
        std::cout<<"class D destructor"<<std::endl;
    }
};
class E : public D{
    public:
    int val;
    using D::func; // let us override 
    
//  E(int ref1):val(ref1){ // we did not expilicitly mention D, compiler tries to call defualt D constructor which do not exist. 
//     std::cout<<"class E constructor with value:"<<val<<std::endl;
//  }
    E(int ref1):D(ref1), val(ref1){  // although literal was passed to E, here it got a name ref1 becoming lvalue, and when we call D(ref1), lvalue constructor executes. Rvalue constrcutor also works but compiler prefers more explicit one. 
        std::cout<<"class E constructor with value:"<<val<<std::endl;
 // to explicitly call rvalue constructor do D(std::move(val))
}
    void func(int x){
        std::cout<<"E func"<<std::endl;
        // use functionality of base class function
        D::func();
    }
 ~E(){
    std::cout<<"class E destructor"<<std::endl;
 }
 private:
    using D::imp_data;
};
int main(){
    C objc1;
    C objc2;
    objc1.funcC(&objc2);
    B objb1;
    objb1.funcB(B());
    // objc1.funcA(); will throw error no public used.  
    std::cout<< "\n"<<std::endl;
    int x =5;
    D objd1(x);
    D objd2(5);
    // objd1.common_method(); error, compiler can't decide whose method to call 

    objd1.B::common_method(); // 2)specify which class to use or in use using in class D 

   //  objd1.funcA; // diamond problem 
    std::cout<<"\n"<<std::endl;
    E obje1(10);
        std::cout<<"\n"<<std::endl;

    obje1.func(5); 
    obje1.func();// parent func overshadowed  ( again use scope resolution or using keyword or virtual override)  

    //obje1.imp_data =10; // private in derived 
    objd1.imp_data =11;// public in base 

 //D* d_pointer = new B(10);  // ( not is-a)
  B* d_pointer = new D(5); // slicing ( is-a)
  B* ptr = (B*)new A();
  std::cout<<"\n"<<std::endl;
  ptr->funcA();


 
    


}