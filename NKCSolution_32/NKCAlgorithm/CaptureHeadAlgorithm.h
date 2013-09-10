#pragma once
#include <iostream>
#include <ctype.h>

using namespace std;


struct HeadState
{
	float head_x,head_y,head_z,angle_x,angle_y,angle_z;

	HeadState():
        head_x(0),head_y(0),head_z(0),angle_x(0),angle_y(0),angle_z(0)
	{
	}
	~HeadState()
	{
	}
};



class CaptureHeadAlgorithm
{
public:
    CaptureHeadAlgorithm();
    ~CaptureHeadAlgorithm();
	bool init();
	bool stop();
	bool start();
	bool end();
	bool getpos(HeadState &state);//开始更新，1为异常退出，0为正常

};

