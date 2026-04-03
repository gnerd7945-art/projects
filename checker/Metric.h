#ifndef Metric_h // can't use Metric.h, macros dont let . operator usage. 
#define Metric_h
#include<chrono>
class Metric{
    public: 

    virtual void update()=0; // pure virtual, no {} body here ,
    virtual std::chrono::seconds timer_func(){
        return std::chrono::seconds(5);
    }

    virtual ~Metric(){
    }

}; 
#endif
