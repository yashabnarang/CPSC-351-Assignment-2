/*
CPSC-351 Assignment 2
Yashab Narang and Kyle Ear
*/

#include "functions.h"

int main()
{

  long int virtualClock = 0;
  long int memorySize = 0;
  long int pageSize = 0;
  int numberProcesses = 0;
  int timeLimit = 100000;
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
    inFile >> tmp.arrivalTime >> tmp.lifetime;
    inFile >> tmp.memory;

    int temp = 0; int total = 0;
    for (int j = 0; j < tmp.memory; j++)
    {
      inFile >> temp;
      total += temp;
    }
    tmp.memory = total;
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
      begin.pid = -1;
      begin.endTime = -1;
      memoryMap.push_back(begin);
    }

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
            procDone = memoryMap[i].pid;
            memoryMap[i].pid = -1;
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
        int endT = virtualClock + inputQueue.front().lifetime;
        int pNum = 1;
        int i = 0;

        if (memoryMap[i].pid == finProcID)
          timeLimit = endT;

        memoryMap[i].arrivalTime = inputQueue.front().arrivalTime;
        turnAroundTimes.push_back(memoryMap[i].endTime - memoryMap[i].arrivalTime);

        while ((inputQueue.front().memory != 0) && (i < numOfPages))
        {
          if (memoryMap[i].pid == -1)
          {
            memoryMap[i].pid = inputQueue.front().pid;
            memoryMap[i].pageNum = pNum;
            memoryMap[i].endTime = endT;
            pNum++;
            freePages--;
            inputQueue.front().memory--;
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
