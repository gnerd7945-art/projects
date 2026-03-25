#include "my_logger.h" 
#include <iostream>

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
    
    main_logger.set_target(logtarget::BOTH);
    main_logger.set_pattern("[%t] [%l] %v"); 

    main_logger.info("System starting up ");

    A my_object; 
   // mylogger logger_dup;  // cant be done because it is private. 

   // mylogger logger_dup(main_logger); copy construcotr is deleted. -

   // mylogger logger_dup ; //copy constructor deleted. 

   // mylogger logger_dup = mylogger::create_instance(); copy construcotr called
    return 0;
}