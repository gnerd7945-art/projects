1) this  project let's user set flags to save logs accordingly.
  eg: "[%t] [%l] %v" where t is to show timestamps, l is to show level, v if to show message. 

2) this is a singleton class project this to create an instance use:-
    mylogger& logger = mylogger::create_instance();
     this creates reference to mylogger::create_instance() so that you don't have to type it again and again.

3) levels and functions:-
 logger.info(msg), logger.warn(msg), logger.error(msg)

 to set pattern and target( where to save):-
  logger.set_pattern() , logger.set_target().. 

  eg:-

    logger.set_target(logtarget::BOTH); // logtarget is enum class.  options in that class: CONSOLE,FILE,BOTH
    logger.set_pattern("[%t] [%l] %v");
     file is saved as log.txt in current folder. 
     

shortcomings:-
1) only one object for now.
2) no thread support 
3) only 3 levels
4) parser runs for every message you log. 
5) cannot explicitly store file at certian location specified
6) no file rotation. 





