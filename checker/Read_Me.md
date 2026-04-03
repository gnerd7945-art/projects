this project  checks memory/ disk space and network on a computer, report anomaly if any in memory usage by printing top 3 processses 
and if there is a difference  in ps aux reported processes and /proc/meminfo  kernel generated virtual folder report.( ie if a process is hiding itself from linux command)
it also checks for disk used in main mount(/) folder and network availibility along with targeted websites.
    Just run the program and add threshold. 
    shortcomings:-
    1)if swarm attack problem is there then those process could be ignored by this program.
    2)does not specify exact malicious process if root level access is there for that process. 

    future work:-
    1) make it swarm resistant
    2) have condition based action automation if anomility detected. 


