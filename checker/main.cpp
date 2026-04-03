#include<iostream>
#include<string>
#include<algorithm> //for transformation, replace
#include<cctype> // for tolower
#include<sstream>  // for sstream
#include<memory> // for smart ptrs
#include<vector>
#include "Ram_Metric.h"
#include "Disk_Metric.h"
#include "Network_Metric.h"
#include<thread>
#include<chrono>
#include<fstream>
int main(){
   //  Metric* metric;  like to do this, but will use smart pointers

   std::vector<std::unique_ptr<Metric>> vec; 

    std::cout<<" enter which metric to monitor(disk/ram/network)"<<std::endl;
    std::string input;
    std::getline(std::cin>>std::ws,input); // ws is used to flush accidental \n charachter left in buffer by prevous cin if used( good practice to do this)

    //std::transform(input.begin(),input.end(),input.begin(),[](char&a)->char{return std::tolower(a);}); // 3rd input specify that transformation is done in input itself.we use transportation since it allow us to save result on any other string and explicitly says we are doing range based logic
    for(char& c: input){
        c= std::tolower(c);
    }
    std::replace(input.begin(),input.end(), ',', ' '); 

    std::stringstream ss(input); // skips leading whitespace/tabs/newline, reads char until whitespace
    std::string word;

    while(ss>>word){ // dumps stored chars in word  and moves to next char after space , then for loops runs once( with prev word) and repeat.
     if(word == "disk"){
                std::cout<<"enter limit value for disc "<<std::endl;
                double x;
                std::cin>> x;
        // std::unique_ptr<Metric> metric = std::make_unique<Disc_Metric>(60); bad programming, will die as go  out of scope and we need it till end
        std::ofstream("disk.txt",std::ios::trunc).close();// truncate erases old and closes file to safely let logger open it
        vec.push_back(std::make_unique<Disk_Metric>(x));
     }
     else if(word == "ram"){
        std::ofstream("ram.txt", std::ios::trunc).close();
        std::cout<<"enter limit value for ram"<<std::endl;
        double x;
        std::cin>> x;
        std::cout<<std::endl;
            vec.push_back(std::make_unique<Ram_Metric>(x));
     }
    else if (word == "network") {
            std::cout << "\n-->  Enter websites to monitor: ";
            std::string site_input;
            std::getline(std::cin>> std::ws, site_input);

            std::vector<std::string> target_sites;
            std::stringstream site_ss(site_input);
            std::string site;
            
            while (site_ss >> site) {
                target_sites.push_back(site);
            }

            // Create the network metric with the user's custom list
            std::ofstream("network.txt",std::ios::trunc).close();
            vec.push_back(std::make_unique<Network_Metric>(target_sites));
        }
    }
    std::vector<std::chrono::steady_clock::time_point> timer;
    timer.reserve(vec.size()); // preallocates, not needed.. 

    const auto start = std::chrono::steady_clock::now(); // measure time duration not wall clock like system_clock
    for(std::unique_ptr<Metric>& m: vec){
        timer.push_back(start + m->timer_func());
    }

    while(true){
         auto now = std::chrono::steady_clock::now();

        for(std::size_t i = 0; i < vec.size(); i++){
            if(now >= timer[i]){
                vec[i]->update();
                timer[i] = std::chrono::steady_clock::now() + vec[i]->timer_func();
            }
        }
         std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}
