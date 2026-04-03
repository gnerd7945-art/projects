#ifndef Netowrk_Metric_H
#define Netowrk_Metric_H
#include "Metric.h"
#include "../MYLOGGER_H.h"
#include <vector>
#include <string>
#include <cstdlib> 


class Network_Metric : public Metric {
private:
    std::vector<std::string> websites;

    bool ping(const std::string& target) {
        std::string command = "ping -c 1 -W 2 " + target + " > /dev/null"; // dev/null special linux file that discards everything writtem to it.
        int result = std::system(command.c_str()); //std::system only accepts const char* arg, .c_str() does that 
        return (result == 0);
    
}


public:
    Network_Metric(std::vector<std::string> sites) : websites(sites) {}

    void update() override {
        mylogger& logger = mylogger::create_instance();
        logger.set_target(logtarget::FILE, "network.txt");
        logger.set_pattern("[%t] [%l] %v");

        // 1. Check primary connection first

        if (!ping("8.8.8.8")) {
            logger.error("Internet DOWN");
            return; // dont check rest 
        }

        for (const auto& site : websites) {
            if (ping(site)) {
                    continue;
            } else {
                logger.warn(site + " is DOWN.");
            }
        }
    }
    std::chrono::seconds timer_func() override{
        return std::chrono::minutes(30);
    }
};
#endif 
