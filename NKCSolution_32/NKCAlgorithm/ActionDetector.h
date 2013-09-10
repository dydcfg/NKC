#pragma once

class CaptureHeadAlgorithm;

enum Action
{
    LEFT_TURN = 0,
    RIGHT_TURN,
    NOD_HEAD,
	STAY_MID,
	NOTRACKING
};

enum SitState
{
	STAY_GOOD = 0,
	STAY_TERRIBLE
};

enum ActMessage
{
    NONE_MESSAGE = 0,
    NO_ACTION_WARNING,
    LANG_TIME_USING
};


class _declspec(dllexport) ActionDetector
{
public:
    ActionDetector(void);
    ~ActionDetector(void);

	bool Start();//开始监控，会返回不会死循环
	bool Stop();
	bool Advance(Action &curAction,SitState &curSitState,ActMessage &msg);
	bool IsStart(){return isStart;}
private:
	bool GetAction(Action &curAction,SitState &curSitState,ActMessage &msg);
    bool isStart;
	unsigned int  internalCount;
};

