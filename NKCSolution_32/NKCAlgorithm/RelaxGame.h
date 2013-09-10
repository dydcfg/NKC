#pragma once
#include <string>
#include <list>
#include <iterator>
#define SPEED 5
using namespace std;


class RelaxGame
{
	public:
	list<string> namelist;
	int combat;
	int point;
	RelaxGame();
	~RelaxGame();
	int play(int,int);//是否播放歌曲（0=no ,1=yes），选择第几套放松操做


};
