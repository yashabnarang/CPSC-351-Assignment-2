/*
CPSC-351 Assignment 2
Yashab Narang and Kyle Ear
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>

using namespace std;

struct process
{
  int pid;
  int arrivalTime, lifeTime;
  int memPieces, memReq;
};

struct memoryPage
{
	int pageStart, pageEnd, _pid, pageNum, arrivalTime, endTime;
};

void getInputQueue(queue<process> _inputQueue)
{
  cout << "\tInput Queue:[";
  while (!_inputQueue.empty())
  {
    cout << _inputQueue.front().pid;
    _inputQueue.pop();
    if(!_inputQueue.empty())
      cout << " ";
  }
  cout << "]" << endl;
}

void printMemoryMap(vector<memoryPage>MemMap, int PageTotal)
{
    cout << "\tMemory Map:" << endl;

    int Start = 0;
    int End = 0;

    for (int x = 0; x < PageTotal; x++)
    {
      memoryPage N = MemMap [x];

   	 if (N._pid == -1)
   	   Start = N.pageStart;
   	 End = N.pageEnd;

   	 if ( x == PageTotal -1)
   	 {
   	   cout << "\t\t" << Start << "-" << End << ":";
   	   cout << "\t Free Frame(s) \n";
   	 }
    else
    {
   	 if (End != 0)
   	 {
       cout << "\t\t" << Start << "-" << End << ":";

       cout << "\t Free Frame(s) \n";
       cout<< "\t\t" << N.pageStart << "-" << N.pageEnd << ":";
       cout<<"\tProcess" << N._pid << "\tPage" << N.pageNum << endl;
   	 }

   	 else
     {
   		 cout << "\t\t" << N.pageStart << "-" << N.pageEnd << ":";
   		 cout << "\tProcess" << N._pid << "\tPage" << N.pageNum << endl;
     }
   }
 }
}

int main()
{

  long int memorySize = 0;
  long int pageSize = 0;

  long int virtualClock = 0;
  int numberProcesses = 0;
  int freePages = 0;
  char file;
  size_t result;

  queue<process> inputQueue, waitQueue;
  process tmp;

  //User Input
  cout << "Enter Memory Size <= 30,000: ";
  cin >> memorySize;

  while(memorySize > 30000)
  {
    cout << "Memory Too Much. Please Enter Memory Size <= 30,000: ";
    cin >> memorySize;
  }

  cout << "Enter Page Size (1: 100, 2: 200, 3: 400): ";
  cin >> pageSize;

  while(pageSize < 1 || pageSize > 3)
  {
    cout << "Invalid Page Size. Enter Page Size (1: 100, 2: 200, 3: 400): ";
    cin >> pageSize;
  }

  if(pageSize == 1)
    pageSize = 100;
  if(pageSize == 2)
    pageSize = 200;
  if(pageSize == 3)
    pageSize = 400;

  freePages = memorySize / pageSize;

  cout << "Enter Workload File Name: ";
  cin >> &file;

  ifstream inFile;
  inFile.open(&file);

  while(!inFile)
  {
    cout << "Invalid File Name. Enter Valid File Name: ";
    cin >> &file;
    inFile.open(&file);
  }

  inFile >> numberProcesses;

  for (int i = 0; i < numberProcesses; i++)
  {
    process tmp;

    inFile >> tmp.pid;
    inFile >> tmp.arrivalTime >> tmp.lifeTime;
    inFile >> tmp.memPieces;

    int total = 0;
    int temp = 0;
    for (int j = 0; j < tmp.memPieces; j++)
    {
      inFile >> temp;
      total += temp;
    }
    tmp.memReq = total;

    waitQueue.push(tmp);
  }

    memoryPage begin;
    vector<memoryPage> memoryMap;
    vector<int> turnAroundTimes;
    int numOfPages = memorySize / pageSize;
    int finProcID = waitQueue.back().pid;

    for (int k = 0; k < numOfPages; k++)
    {
      begin.pageStart = k * pageSize;
      begin.pageEnd = ((k + 1) * pageSize) - 1;
      begin.pageNum = 0;
      begin._pid = -1;
      begin.endTime = -1;
      memoryMap.push_back(begin);
    }

    int timeLimit = 100000;

    while (virtualClock != timeLimit)
    {
      bool print = true;
      bool waitTest = false;
      bool printMap = false;
      process frontP = waitQueue.front();

      if (!waitQueue.empty())
        waitTest = (frontP.arrivalTime == virtualClock);

      while(waitQueue.front().arrivalTime == virtualClock)
      {
        cout << "t = " << virtualClock << ": Process " << waitQueue.front().pid << " arrives" << endl;
        inputQueue.push(waitQueue.front());
        waitQueue.pop();
        getInputQueue(inputQueue);
        printMap = true;
        print = false;
        if (!waitQueue.empty())
          waitTest = (frontP.arrivalTime == virtualClock);
        else
          waitTest = false;
      }
      bool pprint = true;

    for (int i = 0; i < numOfPages; i++)
    {
        int procDone = 0;

        if(memoryMap[i].endTime == virtualClock)
        {
            procDone = memoryMap[i]._pid;
            memoryMap[i]._pid = -1;
            memoryMap[i].pageNum = 0;
            memoryMap[i].endTime = 0;
            freePages++;
        }

        if ((procDone != 0) && (pprint))
        {
            if (print)
            {
              cout << "virtual clock = " << virtualClock << endl;
              print = false;
            }

            cout << "\t\tProcess " << procDone << " completes\n";
            printMap = true;
            pprint = false;
        }
    }

      while ((!inputQueue.empty()) && (freePages > 0))
      {
        if (print)
        {
          cout << "virtual clock = " << virtualClock << endl;
          print = false;
        }
        cout << "\tMM moves Process " << inputQueue.front().pid << " to memory\n";
        printMap = true;
        int endT = virtualClock + inputQueue.front().lifeTime;
        int pNum = 1;
        int i = 0;

        if (memoryMap[i]._pid == finProcID)
          timeLimit = endT;

        memoryMap[i].arrivalTime = inputQueue.front().arrivalTime;
        turnAroundTimes.push_back(memoryMap[i].endTime - memoryMap[i].arrivalTime);

        while ((inputQueue.front().memReq != 0) && (i < numOfPages))
        {
          if (memoryMap[i]._pid == -1)
          {
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

    if(printMap)
      printMemoryMap(memoryMap, numOfPages);

    virtualClock++;
  }

  int total = 0;
  for (int i = 0; i < turnAroundTimes.size(); i++)
    total += turnAroundTimes[i];

  double turnAround = total / turnAroundTimes.size();
  cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << turnAround << "\n" << endl;

  return 0;
}
