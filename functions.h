#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>

using namespace std;

struct process
{
  int arrivalTime, lifetime, memory, memoryRequired, pid;
};

struct memoryPage
{
	int pageStart, pageEnd, pid, pageNum, arrivalTime, endTime;
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

   	 if (N.pid == -1)
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
       cout<<"\tProcess" << N.pid << "\tPage" << N.pageNum << endl;
   	 }

   	 else
     {
   		 cout << "\t\t" << N.pageStart << "-" << N.pageEnd << ":";
   		 cout << "\tProcess" << N.pid << "\tPage" << N.pageNum << endl;
     }
   }
 }
}
