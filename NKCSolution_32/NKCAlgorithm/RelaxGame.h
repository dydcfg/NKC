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
	int play(int,int);//�Ƿ񲥷Ÿ�����0=no ,1=yes����ѡ��ڼ��׷��ɲ���


};
