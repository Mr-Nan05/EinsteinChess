#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "ClientSocket.h"
#include <iostream>
#include <string>
#include <list>
#include <ctime>


using namespace std;


//默认己方为蓝方，如果不是，则转化为蓝方，dice转为1-6
class Einstein
{
private:
	ClientSocket clientsocket;
	std::list<std::string> logger;
	std::string filename; // log filename
	int chessboard[25];
	int dice;

public:
	Einstein();
	~Einstein();
	int parse(std::string s);
	int handle();
	int logging(std::string s);
	int writelog();
};
