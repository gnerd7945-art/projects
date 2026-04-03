#include "MYLOGGER_H.h" 
#include <iostream>
#include "string.h"

class A {
private:
    mylogger& class_logger = mylogger::create_instance();

public:
    A() {
        class_logger.info("Class A object created");
    }

};

int main() {    
     mylogger& main_logger = mylogger::create_instance();
     main_logger.set_level(loglevel::ERROR);
     main_logger.set_pattern("[%l] %t: %v");
     main_logger.set_target(logtarget::FILE,"log.txt");
     main_logger.info("some info");
     main_logger.error("warn");
    
    // main_logger.set_target(logtarget::BOTH);
    // main_logger.set_pattern("[%t] [%l] %v"); 

    // main_logger.info("System starting up ");

    A my_object; 
   // mylogger logger_dup;  // cant be done because it is private. 

   // mylogger logger_dup(main_logger); copy construcotr is deleted. -

   // mylogger logger_dup ; //copy constructor deleted. 

   // mylogger logger_dup = mylogger::create_instance(); copy construcotr called

       String s ="sugam";
       s.insert(2,"ami");
       std::cout<<s<<std::endl;
       s.replace(1,3,"ami");
       std::cout<<s<<std::endl;
       String s1;
       s1 = std::move("hello world");
              std::cout<<s1<<std::endl;
              
       s1.insert(3," NEW").insert(5,"WORLD");
              std::cout<<s1<<std::endl;

        s1+="GOOD AFTERNOON";
              std::cout<<s1<<std::endl;

        s1 = s1+s;
             std::cout<<s1<<std::endl;
        String s2("hello...");
        String s3 = s2.substr(1,3);
         std::cout<<s2<<std::endl;

         std::cout<<s3<<std::endl;

        String s4 ="new world";
        String s5 = s4;
        std::cout<<s4<<std::endl;
        String s6;
        s6 = s4;
        String s7;
        s7 =std::move(s3);
        std::cout<<"\n"<<std::endl;


        String s8(std::move("s7")); // move assignment not working
        std::cout<< "\n"<<std::endl;

        String s9 = "hi";
        String S10(std::move(s9));
            std::cout<< "\n"<<std::endl;

        String str = String("hi"); // logically: 
        //Step 1: Create a temporary String object from "hi" (Parameterized).

        // Step 2: Move that temporary into str (Move Constructor).

        // Step 3: Destroy the temporary (Destructor).
        // but copy ellison just directly does: String str("hi")
        std::cout<<str<<std::endl;    


    return 0;
}