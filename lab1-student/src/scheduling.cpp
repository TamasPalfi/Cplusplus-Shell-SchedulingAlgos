#include <string.h>
#include <fstream>
#include <iostream>
#include <queue>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "scheduling.h"

using namespace std;

pqueue_arrival read_workload(string filename)
{
  pqueue_arrival workload;
  //set up variable to handle input stream to read file in
  ifstream in;
  //check to see if file is open already
  if(in.is_open()){
    //file is open, print Error msg
    cout << "ERROR: file was already open";
    //quit methodd
    exit(1);
  }
  //open file
  in.open(filename);
  /*//check if the file is empty
  if(in.peek() == ifstream::traits_type::eof()){
    //file is empty, print Error msg
    cout << "ERROR: file was empty";
    //quit method
    exit(1);
    }*/ //file is good, proceed
  //set up string variable to take each Process data
  string line;
  //loop through each line to get proccesses data
  while(getline(in,line)){
    //create new process data struct
    Process p;
    //split the line data into tokens using char * array to store token, strcpy, and strtok.
    char* lineArr = new char[line.length()+1];
    strcpy(lineArr, line.c_str());
    char* token = strtok(lineArr, " ");
    //set up int count
    int c = 0;
    //fill in process data
    while(token != NULL){
      //add arrival time
      if(c == 0){
	p.arrival = atoi(token);
      }
      else if(c == 1){
	p.duration = atoi(token);
      }
      //increment counter
      c++;
      //call strtok again to continue search
      token = strtok(NULL, " ");
    }
    //all process information set up, add to pqueue
    workload.push(p);
  }
  //show_workload(workload);
  //cout << workload.size();
  //cout << "\n";
  //cout << workload.top().arrival;
  //cout << "\n";
  return workload;
}

void show_workload(pqueue_arrival workload)
{
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty())
  {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes)
{
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty())
  {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion << endl;
    xs.pop_front();
  }
}

list<Process> fifo(pqueue_arrival workload)
{
  list<Process> complete;
  
  //set up time variable to track for start time & completion time
  int time = 0;
  //set time to first process's arrival time
  time = workload.top().arrival;
    
  //loop through each process in workload
  while(!workload.empty()){
    //get the next process to have arrived
    Process p = workload.top();  //TODO :: HANDLE IF next job  arrival time is after first job completion
    workload.pop();
    //set other process params accordingly
    p.first_run = time;
    p.completion = time + p.duration;
    //set time to end of last process
    time = p.completion;
    //add to completed processes list
    complete.push_back(p);
  }     
  return complete;
}

list<Process> sjf(pqueue_arrival workload)
{
  list<Process> complete;

  //make new pqueue_duration to track arrived processes
  pqueue_duration real;
  //set up time variable to track
  int time = 0;
  //set time to first processes arrival time
  time = workload.top().arrival;
  //check for empty arrival queue or duration queue
  while(!workload.empty() || !real.empty()){
    //check for if any new processes arrived
    while(!workload.empty() && workload.top().arrival <= time){
      Process x = workload.top();
      //push the process into data struct holding processes
      real.push(x);
      //take that process out of the workload
      workload.pop();
    }  //all newly arrived processes are in struct
    //get the next process to analyze
    Process p = real.top();
    real.pop();
    //set other params
    p.first_run = time;
    p.completion = time + p.duration;
    //set time to end completed process
    time = p.completion;
    //add to completed processes list in correct order
    complete.push_back(p);
    }
  //return the finished list
  return complete;
}

list<Process> stcf(pqueue_arrival workload)
{
  list<Process> complete; 

  //make new pqueue_duration to track arrived processes
  pqueue_duration real;
  //set up time variable for tracking
  int time = 0;
  //set time to first processes arrival time
  time = workload.top().arrival;
  //check if any new processes have arrived
  while(!workload.empty() && workload.top().arrival == time){
    Process p = workload.top();
    p.first_run = -1;
    //push process into struct holding processes
    real.push(p);
    //take that process out of workload
    workload.pop();
  } //all newly arrived process in arrived struct
  //check if arrived is empty
  while(!real.empty()){
    //get next process
    Process p = real.top();
    real.pop();
   //set params
    if(p.first_run == -1){
      //set value correctly
      p.first_run = time;
    }
    //decrement duration
    p.duration = p.duration - 1;
    //increment time
    time += 1; 
    //check for if completed process
    if(p.duration == 0){
      p.completion = time;
      //add it to completed list
      complete.push_back(p);
    } else{ //add process back to duration queue
      real.push(p);
    }
    while(!workload.empty() && workload.top().arrival == time){
      Process p = workload.top();
      p.first_run = -1;  
      real.push(p);
      workload.pop();
    }
  }
  return complete;
}

list<Process> rr(pqueue_arrival workload)
{
  list<Process> complete;

  //not using a duration queue but a list
  //process everything simuleeatonously
  
  //make new pqueue_duration for arrived processes
  //pqueue_arrival arrived; 
  list<Process> arrived;
  //set variable up to track the time
  int time = 0;
  //set time to first processes arrival
  time = workload.top().arrival;
  //check if any new processes have arrived
  while(!workload.empty() && workload.top().arrival == time){
    Process p = workload.top();
    p.first_run = -1;
    //push process into struct tracking "arrived"
    arrived.push_back(p);
    //take that process out of the workload
    workload.pop();
  } //all newly arrived process in structure
  //cout << arrived.size();
  //cout << "\n";
  //check if arrived is empty
  while(!arrived.empty()){
    //get next process
    Process p = arrived.front();
    arrived.pop_front();
    //set params
    if(p.first_run == -1){
      //set value correctly
      p.first_run = time;
    }
    //decrement duration
    p.duration = p.duration - 1;
    //increment time
    time += 1;
    //check for if completed process
    if(p.duration == 0){
      p.completion = time;
      //add to completed list
      complete.push_back(p);
    } else { //add process back to queue of available processes
      arrived.push_back(p);
    }
  }
  return complete;
}

float avg_turnaround(list<Process> processes)
{
  //set up variable to hold res
  float res = 0;
  //get size of processes list
  float size = (float) processes.size();
  //loop through each process and add turnaround time to res
  while(!processes.empty()){
    //get process
    Process p = processes.front();
    processes.pop_front();
    //int variable to hold the turnaround time
    float tt = (float) p.completion -(float) p.arrival;
    res += tt; 
  }
  return res/size;
}

float avg_response(list<Process> processes)
{
  //set up variable to hold result
  float res = 0;
  //get size of processes list
  float size = (float) processes.size(); 
  //loop through each process and add response time to res
  while(!processes.empty()){
    //get process
    Process p = processes.front();
    processes.pop_front();
    //int variable to hold response time
    float rt = (float) p.first_run - (float) p.arrival;
    res += rt;
  }
  return res/size;
}

void show_metrics(list<Process> processes)
{
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_t << endl;
  cout << "Average Response Time:   " << avg_r << endl;
  }
