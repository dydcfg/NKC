#include "ControlThread.h"
#include "Base.h"
#include "../NKCAlgorithm/ActionDetector.h"
#include "nkcsolution.h"

ControlThread::ControlThread(NKCSolution* app)
{
    // Initialize members
    dThreadID    = 0;
    hThread    = NULL;
    mainApp = app;
    detector = new ActionDetector;
    QueryPerformanceFrequency(&timeFrequency);
    QueryPerformanceCounter(&lastTime);
    QueryPerformanceCounter(&lastUpdateTime);

}

ControlThread::~ControlThread(void)
{
    detector->Stop();
    delete detector;
}

DWORD WINAPI ControlThread::ThreadProc(LPVOID Param)
{
    // Create object of ThreadPoolMgr
    ControlThread* ptrThread = (ControlThread*)Param;

    BOOL bShutDown = FALSE;

    while( !bShutDown )
    {
        if (appState.isRunning)
        {
            ptrThread->Run();
        }
        else
        {
            ptrThread->Stop();
            Sleep(100);
        }
    }
    return 0;
}

void ControlThread::Stop()
{   
    try
    {
        detector->Stop();
    }
    catch(...)
    {
        emit mainApp->failmessage();
        return;
    }
}

void ControlThread::Run()
{   
    try
    {
        detector->Start();
        Action curAction;
		SitState curSitState;
        ActMessage curMessage;
        if (detector->Advance(curAction,curSitState,curMessage))
        {
            appState.doShortcut(curAction);
        }

        //time control
        LARGE_INTEGER curTime;
        QueryPerformanceCounter(&curTime);

        double inte =(double)(curTime.QuadPart-lastUpdateTime.QuadPart)/(double)timeFrequency.QuadPart * 1000;
        if (inte > UPDATEINTERNALTIME)
        {
			if (curAction != appState.curAction || curSitState != appState.curSitState || appState.curMessage != curMessage)
            {
                appState.curAction = curAction;
				appState.curSitState = curSitState;
				appState.curMessage = curMessage;
                emit mainApp->updatemessage();
                lastUpdateTime = curTime;
            }
        }

        inte =(double)(curTime.QuadPart-lastTime.QuadPart)/(double)timeFrequency.QuadPart * 1000;
        if (inte < INTERNALTIME)
        {
            Sleep((int)(INTERNALTIME - inte));
            //QueryPerformanceFrequency(&curTime);
            //inte =(double)(curTime.QuadPart-lastTime.QuadPart)/(double)timeFrequency.QuadPart;
        }

        lastTime = curTime;
    }
    catch(...)
    {
        appState.isRunning = false;
        emit mainApp->nocameramessage();
        emit mainApp->updatemessage();
        return;
    }
}


void ControlThread::CreateWorkerThread()
{
    hThread = CreateThread(NULL, NULL, ThreadProc, (LPVOID)this, NULL, &dThreadID);
}

void ControlThread::DestroyThread()
{
    TerminateThread(hThread,0);
}

HANDLE ControlThread::GetThreadHandle()
{
    return hThread;
}

DWORD ControlThread::GetThreadID()
{
    return dThreadID;
}
