struct process {
  int pid;
  int arrivalTime, lifeTime;
  int memPieces, memReq;
};

struct memoryPage {
	int pageStart, pageEnd, _pid, pageNum, arrivalTime, endTime;
};
