#ifndef MYLOGGER_H
#define MYLOGGER_H

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip> 
#include <ctime>   
#include <fstream> // for File I/O

enum class loglevel {
    INFO,
    WARN,
    ERROR
};

enum class logtarget {
    CONSOLE,
    FILE,
    BOTH
};

class mylogger {
private:
    mylogger()= default; // private so mylogger logger cannot be used. 
    std::string pattern = ""; 
    logtarget target = logtarget::CONSOLE; 
    std::ofstream file_stream;

    std::string RESET = "\033[0m";
    std::string RED = "\033[31m";
    std::string YELLOW = "\033[33m";
    std::string GREEN = "\033[32m";

    ~mylogger() {
        if (file_stream.is_open()) {
            file_stream.close();
        }
    }
    // delete copy constructor and copy assignment constructor 
    mylogger(const mylogger&) = delete; //delete copyconstructor so no one  accieently calls it 
    // mylogger& operator=(const mylogger&) = delete; // delete = overloaded  assignemnt for creating new cinstructor, prevents: mylogger logA = mylogger::get_instance();
    std::string timestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);  // making it c language type 
        std::tm* now_tm = std::localtime(&now_c); // so that we can get human readable output instead of cpp 
        std::stringstream ss1;
        ss1 << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S"); 
        return ss1.str();
    }

    std::string parse(loglevel current_level, const std::string &msg, bool use_color) {
        std::stringstream ss;
        
        for (size_t i = 0; i < pattern.size(); i++) {
            if (pattern[i] == '%') {
                ++i;
                while (i < pattern.size() && pattern[i] == ' ') { ++i; }
                if (i >= pattern.size()) { break; }
                char flag = pattern[i];

                if (flag == 't') {
                    ss << timestamp();
                } 
                else if (flag == 'v') {
                    ss << msg; 
                } 
                else if (flag == 'l') {
                    if (current_level == loglevel::INFO) {
                        ss << (use_color ? GREEN + "INFO" + RESET : "INFO");
                    } 
                    else if (current_level == loglevel::WARN) {
                        ss << (use_color ? YELLOW + "WARN" + RESET : "WARN");
                    } 
                    else if (current_level == loglevel::ERROR) {
                        ss << (use_color ? RED + "ERROR" + RESET : "ERROR");
                    }
                }
            } 
            else {
                ss << pattern[i];
            }
        }
        return ss.str();
    }

    void dispatch_log(loglevel level, const std::string& msg) {
        if (target == logtarget::CONSOLE || target == logtarget::BOTH) {
            std::cout << parse(level, msg, true) << std::endl;
        }
        if (target == logtarget::FILE  || target == logtarget::BOTH) {
            if (file_stream.is_open()) {
                file_stream << parse(level, msg, false) << std::endl;
            }
        }
    }

public: 

    void set_pattern(const std::string& S) { 
        pattern = std::move(S);
    }

    void show_pattern() {
        std::cout << "Current Pattern: " << pattern << std::endl;
    }
    void set_target(logtarget t, const std::string& filename = "log.txt") {
        target = t;
        if (target == logtarget::FILE || target == logtarget::BOTH) {
            if (file_stream.is_open()) file_stream.close();
            file_stream.open(filename, std::ios::app); // Append mode
        }
    }
    static mylogger& create_instance(){
        static  mylogger logger; // makes it permanenet so does not destroy after funtion ends 
        return logger;
        
    }

    void info(const std::string& msg) {
         dispatch_log(loglevel::INFO, msg); }

    void warn(const std::string& msg) { 
        dispatch_log(loglevel::WARN, msg); }

    void error(const std::string& msg){
         dispatch_log(loglevel::ERROR, msg); }


};
#endif