#pragma once
#include <Windows.h>
#include <string>

class ActionDetector;
class NKCSolution;

class ControlThread
{
public:
	ControlThread(NKCSolution* app);
	~ControlThread(void);
private:   
	DWORD     dThreadID;        // ID of thread
	HANDLE    hThread;        // Handle to thread
	ActionDetector* detector;
	NKCSolution*    mainApp;

	const static int INTERNALTIME = 500;
	const static int UPDATEINTERNALTIME = 500;
	LARGE_INTEGER lastTime;
	LARGE_INTEGER lastUpdateTime;
	LARGE_INTEGER timeFrequency;

public:
	void    Stop();
	void    Run(); // Function which will get executed by thread
	static  DWORD WINAPI ThreadProc(LPVOID Param); // Thread procedure
	void    CreateWorkerThread();
	void    DestroyThread();
	HANDLE  GetThreadHandle();
	DWORD   GetThreadID();

};

