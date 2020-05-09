#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "ClientSocket.h"
#include <iostream>
#include <string>
#include <list>
#include <ctime>


using namespace std;


//Ĭ�ϼ���Ϊ������������ǣ���ת��Ϊ������diceתΪ1-6
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
