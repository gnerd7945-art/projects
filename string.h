/*
new knowledge:-
1) s[i] = *(s+i), automatic derefrecne // go to ith address and derefrence 

2) in std:: string name ="sugam"; when we pass name(func(name)), function(cosnt std::string &n) takes name as reference without & it is copy.

3) in charachter array: const char* copies pointer not  data and is acceptable 

4) string s = "sugam" is equivalent to parameter constructor calling, string s = s1 is copy constructor same as string s(s1), string s;(defualt constructor)
then s = s1 is assignemnt operator , string s(std::move(obj)) and string s = std:move(obj) mask permanent object and compiler thinks it has rvalue and look for a constructor that accepts rvalue as input. 

5)String(String&& obj) noexcept 
    : data(std::exchange(obj.data, nullptr)),  std::exchange help us take a value and assign different one in single line of code.
      size(std::exchange(obj.size, 0)), 
      capacity(std::exchange(obj.capacity, 0)) 
{
    // The body is now completely empty!
}
 6) If you allocate a single item: int* p = new int; -> You must use delete p;  deletes memory p points to in heap not pointer itself it gets deleted when goes out of scope.
If you allocate an array: char* data = new char[10]; -> You must use delete[] data; deletes memory  data points to in heap, not pointer itself, pointer gets deleted when  it goes out of scope 

7) func(String& val), func(obj)-> reference, func("name")-> calls paramterized constructor but fails at binding. 
   func(string val), func("name")-> parameterized constructor called
   func(const String val),func("name")-> paramterized icnstructor called but makes val obj immutable. 
   func(const String& val), func("name")-> calls paramterized constructor, if object passed then makes reference, without & if arg passed is object then copy constructor would have been called. 
   func(String val), func(obj)-> copy constructor is called.
   fuinc(String val), func(std::move(obj))-> move constructor is called 
8) overload operators cannot be made static    
9) + has more priority than =
10) if + is class member function, then s1+ "abc" :  s1.operator+("new string"), "abc"+s1 will fail. thus we use friend function here, which does not belong to this class, s1+"new string": operator+(s1,"new string")
11) frined class can access internal variables, but of who? since it is global it does not know that and hence a local object needs to be created of the class which uses friend.
12) Member functions always force their own class to be on the left side.
*/ 
#include<iostream>
#include<stdexcept>

static int size_calc(const char* input){  //size calculator
    int count =0;
    for(int i =0;input[i] != '\0';i++){
        count+=1;
    }
    return count;
}
class String{
    private:
 char* data; 
 int size =0;// ignores null pointer 
 int capacity=0; // memory holder ( inittial size 0)
  public:

 //0) defualt constructor
  String(): data(new char[1]), capacity(1) // ask for 1 slot(0th)
{
  data[0] = '\0';  
}
//1) paramter constructor
String(const char* s){ // s is a pointer to literal, string s("sugam"), s points to address of s in sugam since sugam is a literal and stored in read only memory, we have to use const, promising compiler not to change it.
    if(s == nullptr){
        data = new char[1];
        data[0] ='\0';
        capacity=1;
        return;
    }
        size = size_calc(s); //3 
        capacity = size+ 1; //4 
    data = new char[capacity]; //0,1,2,3
    for(int i =0;i<size;i++){
        data[i] = s[i];
    }

    data[size] = '\0';
        std::cout<< "paramaeter  called"<<std::endl;

    
}
//2)copy constructor( making another object using past object)
String( const String& obj): size(obj.size),capacity(obj.capacity){
data = new char[capacity]; 
for(int i =0;i<size;i++){
    data[i] = obj.data[i];
}
data[size] = '\0';
        std::cout<< "copy  called"<<std::endl;


}
// 3) move constructor 
String( String &&obj) noexcept: data(obj.data),size(obj.size),capacity(obj.capacity){
 obj.data = nullptr;
 obj.size =0;
 obj.capacity=0;
 std::cout<<"move called"<<std::endl;
}
//4) destructor 
~String(){
delete[] data; 
}
public: 
// 5) overloading [] operator
char& operator[](int index){
    if(index>=size || index<0){
        throw std::out_of_range("index out of range");
    }
    return data[index];
    
}
 // 6) overloading = operator( move and copy assignment)
 //copy
 String& operator=(String& obj){
    delete [] data; // if current obj already has some string delete it to get new string 
    capacity = obj.capacity;
    size = obj.size;
    data = new char[capacity];
for(int i =0;i<size;i++){
    data[i] = obj.data[i];
}
 data[size] = '\0';
 std::cout<<" copy assignment called"<<std::endl;
  return *this;
 }
 //move 
 String& operator=( String&& obj){
    delete[] data;  // delete old string 
    capacity =obj.capacity;
    size =obj.size;
    data = obj.data;
    obj.data = nullptr;
    std::cout<<"move assignment called"<<std::endl;
    return *this;
 }
 //7) overloading + operator, first making a friend function so that we could do s = "abc"+s1 and s= s1+"abc" both
 friend String operator+(const String& obj1, const String& obj2){ //inline friend
    String s;
    delete[] s.data;
     s.size =obj1.size + obj2.size;
     s.capacity = s.size+1;
     s.data = new char[s.capacity];
     s.data[s.size] ='\0';
     for(int i =0;i<obj1.size;i++){
     s.data[i] = obj1.data[i];
     }
     for(int i =0;i<obj2.size;i++){
        s.data[obj1.size +i] = obj2.data[i];
     }
     std::cout<<"+operator used"<<std::endl;
     return std::move(s);
 }
 //8) overloading << operator, it is of std::ostream object. declared friend so we dont need to write s<<cout 
 friend std::ostream& operator<<(std::ostream &obj1, const String& obj2){
  if(obj2.data != nullptr){
    obj1<<obj2.data;
  }
  return obj1;
 }
 // 9) overloading += operator
 String& operator+=( const String& obj){
    
    //  char* s = new char[size+obj.size+1];
    //  for(int i =0;i<size;i++){
    //     s[i] = data[i];
    //  }
    //  for(int i =0;i<obj.size;i++){
    //     s[size+i] = obj.data[i];
    //  }
    //  delete[] data;// clear heap where data used to point
    //  data = s;
    //  size = size+obj.size;
    //  capacity = size+1;
    //  data[size] ='\0';
    //  return *this;
    this->append(obj.data);
    return *this;
}
// functions starts here 
// 1) append() method. 
void append(const char* input){
   if(input == nullptr) return;
   int input_size = size_calc(input);
   if(size+input_size+1>capacity){
    capacity = 2*capacity;
    if(capacity < size+input_size+1){
        capacity = size+input_size+1;
    }
    char* s = new char[capacity];
    for(int i =0;i<size;i++){
        s[i] = data[i];
    }
    delete[] data;
    data = s;
   }
   for(int i =0;i<input_size;i++){
        data[size+i] = input[i];
    }
    size += input_size;
    data[size] ='\0';

}
// 2) substr() method
String substr(int pos, int length = -1){  // length 1 based.  
    if(pos<0 || pos > size){
        throw std::out_of_range("invalid position");
    }
    if(length ==0){
        return std::move(String(""));  
    }
    if(length > size-pos || length ==-1){
        length = size-pos;
    }
    char* s = new char[length+1];
    for( int i =0;i<length;i++){
        s[i] = data[pos+i];
    }
    String s1;
    delete[] s1.data;
    s1.data = s;
    s1.size = length;
    s1.capacity = length+1;
    s1.data[length] = '\0';
    return std::move(s1);
}
// 3)insert // s= sugam, insert= ami pos 2,l pos 0 based 
String& insert(int pos, const char* str) {
    if (pos < 0 || pos > size) 
    {
        throw std::out_of_range("invalid position");
    }
    if (str == nullptr)
    { 
        return *this;
    }

    int str_len = size_calc(str);
    if (str_len == 0) return *this;

    // 1. Check if need to grow  heap memory
    if (size + str_len + 1 > capacity) {
        int new_capacity = capacity * 2;
        if (new_capacity < size + str_len + 1) {
            new_capacity = size + str_len + 1;
        }

        char* new_data = new char[new_capacity];

        for (int i = 0; i < pos; i++) 
        {
            new_data[i] = data[i];
        }
        
        for (int i = 0; i < str_len; i++){

        new_data[pos + i] = str[i];
        }
        
        for (int i = pos; i <= size; i++) { 
            new_data[pos + str_len + (i - pos)] = data[i]; // adding after  pos data and inserted data 
        }

        delete[] data;
        data = new_data;
        capacity = new_capacity;
    } 
    else {
        // Shift characters to the right( sugam , insert ami at 2 = sugamiaum), 
        // Starting at 'size' means  also shifting the \0
        for (int i = size; i >= pos; i--) {
            data[i + str_len] = data[i];
        }
        
        for (int i = 0; i < str_len; i++) {
            data[pos + i] = str[i];
        }
    }   
    size = size + str_len;
    std::cout<<"my insert called"<<std::endl;
    return *this;
}
String& replace(int pos, int length_to_remove, const String& s){

 if(pos<0 || pos> size || length_to_remove <0){
    throw std::out_of_range("invalid pos or length is less than 0");
 }
 if(pos+length_to_remove > size){
    length_to_remove =size-pos;
 }
 
 if(size-length_to_remove +s.size+1> capacity){
    capacity*=2;
 
 if(capacity<size+s.size+1 - length_to_remove){
    capacity = size+s.size+1 - length_to_remove;
 }
 char* new_s = new char[capacity];
 for(int i =0;i<pos;i++){
    new_s[i] = data[i]; // before pos data
 }
 for(int i =0;i<s.size;i++){
    new_s[pos+i] = s.data[i]; // insert string data 
 }
 for(int i = pos+length_to_remove;i<=size;i++){ // <= size ensures \0 is added( data[size])
    new_s[pos+s.size+i-(pos+length_to_remove)] = data[i];  // insert after pos+s.size, to insert sequentially, we used pos+length_to_remove as baseline.
 } 
 delete[] data;
 data = new_s;
 size = size-length_to_remove+s.size;
 }
 else{ // need to decide if replace shrinking or expanding the string
     int required_size = s.size- length_to_remove;
    if(required_size >0){
        for(int i =size;i>=pos+length_to_remove;i--){ 
            data[i+required_size] = data[i];
        }
        for(int i =0;i <s.size;i++){
            data[pos+i] = s.data[i];
        }
    }
    else{
        for(int i=0;i<s.size;i++){
            data[pos+i] = s.data[i];
        }
        for(int i =pos+length_to_remove;i <=size;i++){
            data[pos+s.size+i -(pos+length_to_remove)] = data[i];
        }
    }
    size = size+s.size- length_to_remove;
  }
 std::cout<<"my replace called"<<std::endl;
return *this;
}

void erase(int pos, int length_to_remove){
    if(pos<0 || pos> size || length_to_remove<0){
        throw std::out_of_range("invalid pos or lenght to remove");
    }
    if(pos+length_to_remove>size){
        length_to_remove= size-pos;
    }
     for(int i =pos+length_to_remove;i <=size;i++){
            data[i -length_to_remove] = data[i];
        }
        size = size-length_to_remove;
        std::cout<<"my erase called"<<std::endl;
}
void clear(){
    data[0] = '\0';
}
 
};


//---------------------------------------------------------------------------------------------------------------------------------------------------------------
// A non-const reference (String&) means the function intends to modify the object passed to it.
// But the object being passed is a temporary ("name"),initially it's scope is limited to  func("name");, the moment ";" is hit, it gets destroy, but first it goes into function and is alive till scope of func.
//  compiler thinks we want to modify obj in funxtion, although out purpose is to read, now the moment function ends our modification ends up in vain. thus cpp does not allow use to modify rvalue 

// To prevent you from accidentally modifying a ghost object and losing your data, the creators of C++ made it illegal to attach a normal reference to a temporary object. thus we use const
// doing const String& myref = String("sugam"); works, here temporary object will be alive til myref is alive. 

