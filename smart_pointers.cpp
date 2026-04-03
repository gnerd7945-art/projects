// use to avoid memory leak via early returns or throw, or maybe simply forgetting to use delete. 

// smart pointers are allocated on stack and when they go out of scope, they call there destructor which have delete in there code to remove memory of object.

// 1) to create unique pointer, instead of typing syntax of unique pointer, use make_unique since it's syntax is simpler and more meory safe 
// order of compilation is not fixed in compiler it might first allocate memory, then call fucntion which throws exception( return) thus unique pointer not able to wrap around that memory and that memory leaked. 
// although mem allocation is with unique pointer, order is not always l-r: process_data(std::unique_ptr<Document>(new Document()), get_priority_from_server());
// here new Document ahppended first, get_priority_from_server() gets called ( failed, thus return) cannot execute unique_ptr argument. 
// make unique is a single function call. it allocates memory and immediately calls unique pointer. 

// new int[n] => returns pointer(int*) 
#include<iostream>

class unique_pointer{    
    private:
        int* ptr;      
    public: 

    unique_pointer(){
        ptr = nullptr;
    };
    unique_pointer(int* input): ptr(input){
        std::cout<<"params called"<<std::endl;
    }; // parameterized constructor 

    

    unique_pointer(unique_pointer&& obj): ptr(obj.ptr){ // move constructor 
        obj.ptr = nullptr;
        std::cout<<" move cons called"<<std::endl;
    }

    void operator=(unique_pointer&& obj){ // move assignment 
        delete[] ptr;
        ptr = obj.ptr;
        obj.ptr = nullptr;
        std::cout<<"move assignment called"<<std::endl;
    } 
    

    unique_pointer(const unique_pointer& obj) = delete; // copy constructor deleted here const not necessarty since compiler just need signature on what to delete, otherwise required for unique_pointer p =unique_pointer(new int[5]), first calls parameterized constructor then ,move constructor if copy ellison diabled 
    unique_pointer& operator=(const unique_pointer& obj) = delete; // copy assignment deleted. 

    // overriding * 
    int& operator*()
    {
        return *ptr;
    }
    // overriding [] to access more elements,  this.operator[](int index) { return ptr[index] }
    int& operator[](int index){
        return ptr[index];
    }
    // check if null 
    bool is_null(const unique_pointer& obj){
        if(obj.ptr ==nullptr){
            return true;
        }
        return false;
    }
    ~unique_pointer(){ // when goes out of scope, delete it 
        std::cout<<"destructor called"<<std::endl;
        delete[] ptr;
    }

};
int main(){
    unique_pointer un = new int;
    *un = 10;
    std::cout<< *un<< std::endl;

    unique_pointer p = std::move(un);
    std::cout<<*p<<std::endl;
     unique_pointer p1;
     p1 = std::move(p);



     
}