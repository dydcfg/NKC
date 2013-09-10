#include "stdafx.h"
#include "ActionDetector.h"
#include "CaptureHeadAlgorithm.h"
#include <list>
#include <Windows.h>

#ifdef _DEBUG
#include <fstream>
std::ofstream dbgFile;
#endif

Action curAction;
CaptureHeadAlgorithm *alg = NULL;
std::list<HeadState> stateList;
const int STATECHAIN = 30;
const int WARNINGCHAIN = 600;
const int MAXLEN = WARNINGCHAIN;

ActionDetector::ActionDetector(void):isStart(false),internalCount(0)
{
    alg = new CaptureHeadAlgorithm;
    stateList.resize(STATECHAIN,HeadState());
#ifdef _DEBUG
    dbgFile.open("detector_debugFile.txt");
#endif
}

ActionDetector::~ActionDetector(void)
{
    if (alg != NULL)  delete alg;
}


bool ActionDetector::Advance(Action& act,SitState& sit,ActMessage &msg)
{
    if (!isStart) return false;

    HeadState curState;
    alg->getpos(curState);
    if (curState.angle_x == 0 &&
        curState.angle_y == 0 &&
        curState.angle_z == 0)
    {
        act = NOTRACKING;
        sit = STAY_GOOD;
        stateList.clear();
        if (internalCount > 0) internalCount -= 2;
        return false;
    }

#ifdef _DEBUG
    dbgFile << curState.angle_x <<"\t" << curState.angle_y <<"\t" << curState.angle_z << std::endl;
#endif

    if (stateList.size() == MAXLEN)
    {
        stateList.pop_front();
    }
    stateList.push_back(curState);

    internalCount++;

    if (!GetAction(act,sit,msg))
    {
        return false;
    }
    return true;
}

bool ActionDetector::GetAction(Action &curAction,SitState &curSitState,ActMessage &curMessage)
{
    curMessage = NONE_MESSAGE;
    curAction = STAY_MID;
    const float limitangle = 15.0f * 3.14f /180.f;
    const float nodlimitangle = 25.0f *3.14f /180.f;


    std::list<HeadState>::reverse_iterator  itr;
    itr = stateList.rbegin();
    HeadState &endState = *itr;
    if (endState.angle_z > limitangle)
    {
        curAction = LEFT_TURN;
    }
    else
    {
        if (endState.angle_z < -1 * limitangle)
        {
            curAction = RIGHT_TURN;
        }
        else
        {
            if (endState.angle_x < -1 * nodlimitangle)
            {
                curAction = NOD_HEAD;
            }
            else
            {
                curAction = STAY_MID;
            }
        }
    }
    const float NORMAL_THRESHOLD_X = 30.0f * 3.14f /180.f;
    const float NORMAL_THRESHOLD_Z = 10.0f * 3.14f /180.f;
    float anglex = 0;
    float anglez = 0;
    itr = stateList.rbegin();
    for(int count = 0; itr != stateList.rend() && count < STATECHAIN; ++count, ++itr)
    {
        HeadState &cur = *itr;
        anglex += abs(cur.angle_x);
        anglez += abs(cur.angle_z);
    }
    anglex /= STATECHAIN;
    anglez /= STATECHAIN;

    if (anglex < NORMAL_THRESHOLD_X && anglez < NORMAL_THRESHOLD_Z)
    {
        curSitState = STAY_GOOD;
    }
    else
    {
        curSitState = STAY_TERRIBLE;

    }

    if (internalCount % WARNINGCHAIN == 0 && stateList.size() >= WARNINGCHAIN)
    {
        int moveAction = 0;
        const int MOVE_THREAD = 3;
        //const float MOVE_THRESHOLD_X = 15.0f * 3.14f /180.f;
        const float MOVE_THRESHOLD_Y = 15.0f * 3.14f /180.f;
        const float MOVE_THRESHOLD_Z = 15.0f * 3.14f /180.f;
        itr = stateList.rbegin();
        for(int count = 0; itr != stateList.rend() && count < WARNINGCHAIN; ++count, ++itr)
        {
            HeadState &cur = *itr;
            if (//abs(cur.angle_x) > MOVE_THRESHOLD_X ||
                abs(cur.angle_y) > MOVE_THRESHOLD_Y ||
                abs(cur.angle_z) > MOVE_THRESHOLD_Z)
            {
                moveAction++;
                if (moveAction >= MOVE_THREAD) break;
            }
        }

        if (moveAction < MOVE_THREAD)
        {
            curMessage = NO_ACTION_WARNING;
        }
    }

    if (internalCount > 7200)
    {
        curMessage = LANG_TIME_USING;
    }
    return true;
}

bool ActionDetector::Start()
{
    if (isStart) return true;
    isStart = alg->start();
    return isStart;
}

bool ActionDetector::Stop()
{
    if (!isStart) return true;
    isStart = !alg->stop();
    return true;
}