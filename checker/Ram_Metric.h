#ifndef Ram_Metric_h
#define Ram_Metric_h
#include "Metric.h"
#include "../MYLOGGER_H.h"
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<filesystem>
#include<algorithm> // used in all_of()
#include<cstdlib> // for std::system
namespace fs = std::filesystem;   //  to create/delete/get file info/rename and copy  files, iterate directorie, check file existance.
class Ram_Metric : public Metric{
    private:
  int limit; 
  public: 
  Ram_Metric(int t): limit(t){};
  void update() override{
    std::ifstream mem_file("/proc/meminfo"); // virtual file generated dynamically by kernel at runitme when report needed, fstream is used to open,read and write into file i make it read only 
    std:: string line;
    std::string key;
    int value;
    int memtotal =0;
    int memAvailable =0;
    int anonPages=0;
    int count=0;
    while(std:: getline(mem_file,line) && count <3){
      std:: istringstream iss(line); // similarly i makes stringstream read only
      if(iss>>key>>value){
        if(key =="MemTotal:"){
          memtotal = value;
          count+=1;
        }
        else if(key =="MemAvailable:"){
          memAvailable= value;
          count+=1;
        }
        else if(key =="AnonPages:"){
          anonPages = value;
          count+=1;
        }
      }
    }
    mem_file.close();
    double Usepercent = ((double)(memtotal-memAvailable)/memtotal)*100;
    mylogger& logger = mylogger:: create_instance();

    if(Usepercent> limit){
      logger.set_target(logtarget::FILE,"ram.txt");
      logger.set_pattern("%t %l %v");
      logger.warn("ram greater than set limit ");

      //std::system pauses cpp code and opens shell, paste the string provided and hits enter. if command sucessfull returns 0. 
      bool smem_used= (std::system("command -v smem > /dev/null")==0);
      if(smem_used)
      {
         std::system("smem -H -r -s pss -c 'pid name pss' | head -n 3 > temp_ram.txt"); // pss is more accurate as it does not double count shared chache/library.-r: reverse sort, -s pss(strictly by pss column), -H remove column as we use ours. 
      }
      else{
      std::system("ps -eo pid=,comm=,rss= --sort=-rss | head -n 3 > temp_ram.txt"); // -e all process not just started by user,o means ignore default column format and use our, = mean ignore header name as logge rlaready use it, pid: process_id, comm: commnad/executable which started it, rss: resident set size(ram in kb) -rss( sort by desc) we redirect output to new file and read and delete it later.
      }
      std::ifstream tempfile("temp_ram.txt");
      // std:: string line;
      if(smem_used){
        logger.info("PID  Name  PSS");
      }
      else{
        logger.info("PID  NAME  RSS");
      }
      while(std::getline(tempfile,line)){
              logger.info(line);
                 }
      tempfile.close();
      fs::remove("temp_ram.txt");
      logger.info("got top 3 most ram consuming process");

      if(Usepercent >limit+10){
        
          std::ifstream fresh_file("/proc/meminfo");
      std::string line;
      std::string key;
      long long  val;
      while(std::getline(fresh_file,line)){
        std:: istringstream istr(line);
        istr>>key;
        if(key =="AnonPages:"){
          istr>>val;
                  break;
        }
      }
      anonPages = val;
      fresh_file.close();

        int visibleRssAnon=0;
        for(const std::filesystem::directory_entry& folder : fs::directory_iterator("/proc"))// not looping over a class but over a range provided by that class( here sequence of directory entries returned by iterator,each item produced by the loop = fs::directory_entry
        // folder is an object here, .path function() in this object creates a new object and returns it similarly for .filename() 
        {
          if(!folder.is_directory()){
            continue;
          }
          std::string folder_name = folder.path().filename().string(); 

          //only numeric folder represent processes, not real folder, they are generated  by kernel at runtime 
          if(!std::all_of(folder_name.begin(),folder_name.end(),::isdigit)){ // all_of returns true if all element satisfy condition(last arg), last arg could have been lambda too: [](const char c)->(bool){return std::isdigit(c)}, why global scope here, why? 
            continue;
          }
          // ifstream accepts std::filesystem::path("string") object 
          std::ifstream status_file(folder.path()/"status");// / is overloaded for std::filesystem::path class like:  path operator/(const path& lhs, const path& rhs);
            if(!status_file){
              continue;
            }
            std:: string line;
            std:: string  key;
            int  rssanon=0;
            while(std::getline(status_file,line)){
              std::istringstream iss(line);
              iss>>key;
              if(key =="RssAnon:"){
                iss>>rssanon ; //>> does automatic type converison. 
                visibleRssAnon+=rssanon;
                break;
              }
            }
        }
        int unidentified_mems= anonPages-visibleRssAnon;
        double gap = (double)(unidentified_mems)/memtotal*100;
            std::string s = std::to_string(gap); 

        if(gap > 5){
          logger.error("potential rootkit bug, got gap of: "+s);
        }
        else{

          logger.info("total gap is "+s);
        }
      }
    }



  }


};
#endif
