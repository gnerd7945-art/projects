**** custom String class, named String***

# 🚀 High-Performance Custom C++ String Class


 API Quick Reference & Usage:-

### 1. Initialization & Moves
```cpp
String s1("Hello");               // Parameterized Constructor
String s2 = s1;                   // Deep Copy Constructor
String s3(std::move(s1));         // Move Constructor steals  resources)

String s4 = s2 + " World";        // Overloaded + (Returns new String)
s4 += "!";                        // Overloaded += (return object)
s4.append(" I am fast.");         //  append

String text("some string.."); 
text.replace(4, 5, "super-fast");   // somesuper-fast
text.insert(0, "HII");         //HIIsomesuper-fast
text.erase(0, 6);                //super-fast

String word = text.substr(4, 10); // extract
std::cout<<text<<std::endl; // get output( pverloaded) 
When mutating an array in-place without new allocations:

    1)Growing the String: You must shift the tail to the RIGHT by looping BACKWARDS (from size down to pos) to prevent the "smear effect" (overwriting your own data).

    2)Shrinking the String: You must shift the tail to the LEFT by looping FORWARDS to safely close the gap.