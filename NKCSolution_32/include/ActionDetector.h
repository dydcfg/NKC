#pragma once
#include <Windows.h>
#include <string>

class ActionDetector;

class ControlThread
{
public:
    ControlThread(void);
    ~ControlThread(void);
private:   
    DWORD     dThreadID;        // ID of thread
    HANDLE    hThread;        // Handle to thread
    ActionDetector* detector;
public:
	void    Stop();
    void    Run(); // Function which will get executed by thread
    static  DWORD WINAPI ThreadProc(LPVOID Param); // Thread procedure
    void    CreateWorkerThread();
    void DestroyThread();
    HANDLE  GetThreadHandle();
    DWORD   GetThreadID();

};

