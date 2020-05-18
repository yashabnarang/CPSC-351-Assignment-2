/*
CPSC-351 Assignment 2
Yashab Narang and Kyle Ear
*/

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct process {
  int pid;
  int arrivalTime, lifeTime;
  int memPieces, memReq;
};

struct memPage {
	int pageStart, pageEnd, _pid, pageNum, arrivalTime, endTime;
};

void getInputQueue(queue<process> _inputQueue) {
  cout << "Input Queue:[";
  while (!_inputQueue.empty()) {
    cout << _inputQueue.front().pid << " ";
    _inputQueue.pop();
  }
  cout << "]" << endl;
}

void printMemoryMap(vector<memPage>MemMap, int PageTotal)
{
    cout << "\tMemory Map:" << endl;

    int Start = 0;
    int End = 0;

    for (int x = 0; x < PageTotal; x++)
    {    memPage N = MemMap [x];

   	 if (N._pid == -1)
   	 {
   	   Start = N.pageStart;
   	 }

   	 End = N.pageEnd;

   	 if ( x == PageTotal -1)
   	 {
   	   cout << "\t\t" << Start << "-" << End << ":";
   	   cout << "\t Free Frame(s) \n";
   	 }
    else
    {
   	 if (End != 0)
   	 { cout << "\t\t" << Start << "-" << End << ":";

   	 cout << "\t Free Frame(s) \n";

   	 cout<< "\t\t" << N.pageStart << "-" << N.pageEnd << ":" ;
   	 cout<<"\tProcess" << N._pid << "\tPage" << N.pageNum << endl ;

   	 }

   	 else {
   		 cout << "\t\t" << N.pageStart << "-" << N.pageEnd << ":" ;

   		 cout << "\tProcess" << N._pid << "\tPage" << N.pageNum << endl;


   		 }
    }
    }
}

int main() {

  long int memSize = 0;
  long int pageSize = 0;
  char filename;
  size_t result;
  long int virtualClock = 0;
  int numProcess;

  queue<process> inputQueue, waitQueue;
  process tempProcess;

  fprintf(stderr, "Please enter in a memory size (must be <= 30,000): ");
  scanf("%ld", &memSize);

  if(memSize > 30000) {
      fprintf(stderr, "The memory size is greater than the maximum size (30,000): %ld\n", memSize);
      exit(-1);
  }

  fprintf(stderr, "Please enter in a page size (1: 100, 2: 200, 3: 400): ");
  scanf("%ld", &pageSize);

  if(pageSize != 1 && pageSize != 2 && pageSize != 3 ) {
      fprintf(stderr, "The page size is invalid: %ld\n", pageSize);
      exit(-1);
  }
  switch (pageSize) {
    case 1:
      pageSize = 100;
      break;
    case 2:
      pageSize = 200;
      break;
    case 3:
      pageSize = 400;
      break;
  }

  int freePages = memSize / pageSize;
  fprintf(stderr, "free pages = %d\n", freePages);

  fprintf(stderr, "Please enter the name of the workload file: ");
  scanf("%s", &filename);

  ifstream inputFile;
  inputFile.open(&filename);
  if (!inputFile) {
    perror("Error reading file.");
    exit(-1);
  }

  inputFile >> numProcess;

  for (int i = 0; i < numProcess; i++) {

    process tempProcess;

    inputFile >> tempProcess.pid;
    if (inputFile.fail()) {
      perror("Error reading process id (line 1).");
      exit(-1);
    }

    inputFile >> tempProcess.arrivalTime >> tempProcess.lifeTime;
    if (inputFile.fail()) {
      perror("Error reading times (line 2).");
      exit(-1);
    }

    inputFile >> tempProcess.memPieces;
    if (inputFile.fail()) {
      perror("Error reading address space (line 3).");
      exit(-1);
    }

    int total = 0;
    int temp = 0;
    for (int j = 0; j < tempProcess.memPieces; j++) {
      inputFile >> temp;
      total += temp;
    }
    tempProcess.memReq = total;

    waitQueue.push(tempProcess);
  }

    memPage begin;
    vector<memPage> memoryMap;
    vector<int> turnAroundTimes;
    int numOfPages = memSize/pageSize;
    int finProcID = waitQueue.back().pid;
    for (int k = 0; k < numOfPages; k++){
      begin.pageStart = k * pageSize;
      begin.pageEnd = ((k + 1) * pageSize) - 1;
      begin.pageNum = 0;
      begin._pid = -1;
      begin.endTime = -1;
      memoryMap.push_back(begin);
    }

    int timeLimit = 100000;

    while (virtualClock != timeLimit) {
      bool print = true;
      bool waitTest = false;
      bool printMap = false;
      process frontP = waitQueue.front();

      if (!waitQueue.empty())
      {
        waitTest = (frontP.arrivalTime == virtualClock);
      }

      while(waitQueue.front().arrivalTime == virtualClock)
      {
        cout << "t= " << virtualClock << endl;
        cout << "Process " << waitQueue.front().pid << " arrives" << endl;
        inputQueue.push(waitQueue.front());
        waitQueue.pop();
        getInputQueue(inputQueue);
        printMap = true;
        print = false;
        if (!waitQueue.empty()) {
          waitTest = (frontP.arrivalTime == virtualClock);
        }
        else {
          waitTest = false;
        }
      }
      bool pprint = true;

    for (int i = 0; i < numOfPages; i++){
        int procDone = 0;

        if(memoryMap[i].endTime == virtualClock) {
            procDone = memoryMap[i]._pid;
            memoryMap[i]._pid = -1;
            memoryMap[i].pageNum = 0;
            memoryMap[i].endTime = 0;
            freePages++;
        }

        if ((procDone != 0) && (pprint)) {
            if (print) {
              cout << "virtual clock = " << virtualClock << endl;
              print = false;
            }
            cout << "\t\tProcess " << procDone << " completes\n";
            printMap = true;
            pprint = false;
        }

    }

      while ((!inputQueue.empty()) && (freePages > 0)) {
        if (print) {
          cout << "virtual clock = " << virtualClock << endl;
          print = false;
        }
        cout << "\t\tMM moves Process " << inputQueue.front().pid << " to memory\n";
        printMap = true;
        int endT = virtualClock + inputQueue.front().lifeTime;
        int pNum = 1;
        int i = 0;

        if (memoryMap[i]._pid == finProcID) {
          timeLimit = endT;
        }

        memoryMap[i].arrivalTime = inputQueue.front().arrivalTime;
        turnAroundTimes.push_back(memoryMap[i].endTime - memoryMap[i].arrivalTime);

        while ((inputQueue.front().memReq != 0) && (i < numOfPages)) {
          if (memoryMap[i]._pid == -1) {
            memoryMap[i]._pid = inputQueue.front().pid;
            memoryMap[i].pageNum = pNum;
            memoryMap[i].endTime = endT;
            pNum++;
            freePages--;
            inputQueue.front().memReq--;
          }
          i++;
        }
        inputQueue.pop();
      }

    if(printMap) {
      printMemoryMap(memoryMap, numOfPages);
    }
    virtualClock++;

  }

  int total = 0;
  for (int i = 0; i < turnAroundTimes.size(); i++) {
    total += turnAroundTimes[i];
  }
  double turnAround = total / turnAroundTimes.size();
  cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << turnAround << "\n" << endl;

  return 0;

}
