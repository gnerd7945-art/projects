#ifndef Disk_Metric_H
#define Disk_Metric_H
#include "Metric.h"
#include "../MYLOGGER_H.h"
#include <filesystem>
#include <string>

class Disk_Metric : public Metric {
private:
    double threshold;
    std::string path;
public:
    Disk_Metric(double t, std::string p = "/") : threshold(t), path(p) {}

    void update() override {
        mylogger& logger = mylogger::create_instance();
        logger.set_target(logtarget::FILE, "disk.txt");
        logger.set_pattern("[%t] [%l] %v");

        auto space = std::filesystem::space(path);
        double usedPercent = 100.0 * (space.capacity - space.available) / space.capacity;

        if (usedPercent > threshold) {
            logger.warn("Disk Space Low on " + path + ": " + std::to_string(usedPercent) + "%");
        } 
    }
};
#endif
