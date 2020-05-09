	#include "Einstein.h"

class myEinstein
{
private:
	string beginTime;//对战开始时间
	string endTime;//结束时间
	string myBoard = "[[3, 6, 2, 0, 0], [5, 1, 0, 0, 0], [4, 0, 0, 0, 8], [0, 0, 0, 10, 7], [0, 0, 9, 11, 12]]|1";

	int chessboard[25];
	int dice;
	int reverseboard[25];
	int realBoard[25];//储存真实棋盘信息
	int staticValue[25] = { 1000,12,6,3,1,12,8,4,2,1,6,4,4,2,1,3,2,2,2,1,1,1,1,1,0 };
	int dynamicValue[5][5];
	int amount;//计算回合数
	int realdice;
	int chesscount;
	int lastEnding = 1;
	int lastCount = 0;
	int r = 1;//随机数

	time_t btime;//begin
	time_t etime;//end

	bool handCheck = 1;//判断执棋方,1<==>human
	bool rightDown;
	bool win = 0;
public:
	friend class Einstein;
	int evaluate(int d, int &value);
	int getValue(int d, int &m);//比较不同dice的价值
	int diceChange(int d);//改变dice
	int singleValue(int i, int j, int d[][5], int &m, int *v);//落子位置最终的价值
	int getResult();//return 1<==>win,-1<==>lose
	int beginCheck();//判断是否是新开局
	int getDice();//获得骰子
	int updateDice(int &l, int &r);
	int chessValue(int i, int j, int d[][5], int &v);//获取落子此处的价值


	void knowSite();
	void site();
	void strToch(string s, char *c);
	void boardUpdate(int m);
	void it(int i, char *s);


	string getEndding(int &c);//判断是否结束本局
	string lastingTime();//获得本局持续时间
	string getTime(bool b);//判断输出内容是否包含秒,0<==>包含second
	string getMove(int m);//获得己方行棋string
	string getBoard();//转化棋盘信息，以便储存
	string humanControl();//手动控制，用于debug
	string myMove(int c, int m, int d);
};

myEinstein Ein;

	Einstein::Einstein()
	{
		this->clientsocket.connectServer();
		clientsocket.sendMsg(ID);
		filename = Ein.getTime(1) + ".log";
		Ein.chesscount = 0;
		Ein.amount = 0;
	
	}

	Einstein::~Einstein()
	{
		this->clientsocket.close();
	}

	int Einstein::parse(std::string s)
	{
		if (s == "close"||s=="")
			return 0;
		int site = 0, length = s.length();
		for (int i=0; i < 25&&site<length; i++)
		{
			while (1)
			{
				if (s[site]<'0' || s[site]>'9')
					site++;
				else {
					if (s[site + 1]<'0' || s[site + 1]>'9')
						chessboard[i] = s[site] - '0';
					else chessboard[i] = 10 * (s[site] - '0') + s[site + 1] - '0';
					site += 2;
					break;
				}
			}
			//if(!amount)
		}

		/*for (; site < length; site++)
		{
			if (s[site] >= '0'&&s[site] <= '9')
			{
				if (site == length - 1)
					dice = s[site] - '0';
				else dice = 10 * (s[site] - '0') + s[site + 1] - '0';
				break;
			}
		}*/

		for (int i = 0; i < 25; i++)
			Ein.realBoard[i] = chessboard[i];
		if (s[length - 2]<'0'||s[length - 2] > '9')
			dice = s[length - 1] - '0';
		else dice = 10 * (s[length - 2] - '0') + s[length - 1] - '0';
		Ein.realdice = dice;
		for (int i = 0; i < 25; i++)
			Ein.chessboard[i] = chessboard[i];
		Ein.dice = dice;
		return 1;
	}

	int Einstein::handle()
	{
		int fightCount = 0;
		clientsocket.recvMsg();
		string Msg = clientsocket.getRecvMsg();
		if (!parse(Msg))
		{
			string e = Ein.getEndding(fightCount);
			logging(e);
			string t=Ein.lastingTime();
			logging(t);
			logger.push_back("=======================================================================\n");
			return 0;
		}
		else {
			logging(Msg);
			if (Ein.beginCheck())
				Ein.site();
			else Ein.knowSite();

			string e = Ein.getEndding(fightCount);

			cout << Ein.lastCount << endl;
			if (e == "")
			{
				int c = 0;
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
					{
						if (Ein.chessboard[i * 5 + j] > 0)
							c++;
						if (Ein.chessboard[5 * (4 - i) + 4 - j] > 0)
							c++;
					}
				if (c > Ein.lastCount&&c >= 11 && !Ein.win)
				{
					
					logging("You lose!\n");
					string t = Ein.lastingTime();
					logging(t);
					logger.push_back("=======================================================================\n");
				}
			}
			int m = Ein.diceChange(Ein.dice);
			string movement = Ein.getMove(m);
			string newBoard = Ein.getBoard();
			char newMsg[1024] = { '\0' };
			Ein.strToch(newBoard, newMsg);
			string newBoard1 = newMsg;
			logging(newBoard1);
			char myMsg[1024] = { '\0' };
			Ein.strToch(movement, myMsg);
			clientsocket.sendMsg(myMsg);
			e = Ein.getEndding(fightCount);
			if (e != "")
			{
				logging(e);
				string t = Ein.lastingTime();
				logging(t);
				logger.push_back("=======================================================================\n");
			}
			else
			{
				Ein.lastCount = 0;
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
					{
						if (Ein.chessboard[i * 5 + j] > 0)
							Ein.lastCount++;
						if (Ein.chessboard[5 * (4 - i) + 4 - j] > 0)
							Ein.lastCount++;
					}
				Ein.win = 0;
			}
			return 1;
		}
	}

	int Einstein ::logging(std::string s)
	{
	
		string singleStep;
		singleStep = singleStep + Ein.getTime(0) + s;
		logger.push_back(singleStep);
		cout << singleStep << endl;
	  

		return 0;
	}

	int Einstein::writelog()
	{
		filename =/*"C:\\Users\\宋超群\\Desktop\\" +*/filename;
		char data[1024] = { '\0' };
		Ein.strToch(filename, data);
		FILE *fp = fopen(data,  "a" );
		list<string>::iterator it = logger.begin();
		for (; it != logger.end(); it++)
		{
			char log[1024] = { '\0' };
			Ein.strToch((*it), log);
			fputs(log, fp);
			fputc('\n', fp);
		}
		if (Ein.getResult()==-1)
			fputs("You Lose!\n", fp);
		else if (Ein.getResult() == 1)
			fputs("You Win!\n", fp);
		fclose(fp);
		return 0;
	}

	int myEinstein::evaluate(int d, int &value)
	{
		int movement = 0;
		value = getValue(d, movement);
		return movement;

	}

	int myEinstein::getValue(int d,int &m)
	{
		int si, sj, v[3];
		int douBoard[5][5];
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				douBoard[i][j] = Ein.chessboard[i * 5 + j];
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				if (douBoard[i][j] == d)
				{
					si = i;
					sj = j;
					v[0] = staticValue[(i - 1) * 5 + j];
					v[1] = staticValue[(i - 1) * 5 + j - 1];
					v[2] = staticValue[i * 5 + j - 1];
					return singleValue(si, sj, douBoard, m, v);
				}
	}

	int myEinstein::singleValue(int i, int j,int d[][5],int &m,  int *v)
	{
		int v1 = v[0], v2 = v[1], v3 = v[2], temp[3] = { 0 }, t1[3] = { 0 }, t2[3] = { 0 };

		if (i == 0 && j == 0)
			return 10000;
		else if (i == 0)
		{
			m = 3;
			v3 += 3;
			return v3;
		}
		else if (j == 0)
		{
			m = 1;
			v1 += 3;
			return v1;
		}
		else if (i == 1 && j == 1)
		{
			m = 2;
			return 10000;
		}

		chessValue(i - 1, j, d, v1);
		chessValue(i - 1, j-1, d, v2);
		chessValue(i, j-1, d, v3);

		if (v1 > v2&&v1 > v3)
		{
			m = 1;
			return v1;
		}
		else if (v2 >= v1 && v2 >= v3)
		{
			m = 2;
			return v2;
		}
		else if (v3 > v2&&v3 > v1)
		{
			m = 3;
			return v3;

		}
		else if (v2 == v1 || v2 == v3)
		{
			m = 2;
			return v2;
		}
		else return (singleValue(i - 1, j, d, m, v) >= singleValue(i, j - 1, d, m, v)) ? singleValue(i - 1, j, d, m, v) : singleValue(i, j - 1, d, m, v);
	}

	int myEinstein::chessValue(int i, int j, int d[][5],int &v)
	{

		int mySites = 0, opposite = 0;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				if (d[i][j] > 6)
					opposite++;
				else if (d[i][j] > 0)
					mySites++;

		int  temp[3] = { 0 };
		if (d[i][j] > 6)
			v += 1;
		if (i)
			if (d[i - 1][j] > 6)
				temp[0] = 1;
		if (i&j)
			if (d[i - 1][j - 1] > 6)
				temp[1] = 1;
		if (j)
			if (d[i][j - 1] > 6)
				temp[2] = 1;
		if (temp[1] && temp[0] && temp[2])
			if (mySites == 2)
				v -= 5;
			else if (mySites == 1)
				v -= 10;
			else v -= 1;
		if (temp[0] && temp[2])
			if (mySites == 2)
				v -= 1;
			else if (mySites == 1)
				v -= 3;
		if (temp[1]&&(temp[0]||temp[2]))
			if (mySites == 2)
				v -= 1;
			else if (mySites == 1)
				v -= 5;
		if (d[i][j] > 6 && (i >= 3 && j >= 3||i>3&&j>2||j>3&&i>2))
			if (opposite == 1)
				v += 100;
			else v += 1;
		else if (d[i][j] > 6)
			if (opposite == 2)
				v += 2;
			else if (opposite == 1)
				v += 100;
		/*if (mySites == 3&& d[i][j] > 0 && d[i][j] <= 6)
				v --;*/
		if (mySites == 2 && d[i][j] > 0 && d[i][j] <= 6)
			v -= 2;
		return v;
	}

	int myEinstein::getResult()
	{
		int opposite = 0;
		for (int i = 0; i < 25; i++)
			if (Ein.chessboard[i] > 6)
				opposite++;
		if (Ein.chessboard[0] > 0 && Ein.chessboard[0] <= 6||opposite==0)
			return 1;//win
		else return 0;
		//else {
		//
		//	int c = 0;
		//	for (int i = 0; i < 3; i++)
		//		for (int j = 0; j < 3; j++)
		//		{
		//			if (Ein.chessboard[i * 5 + j] > 0)
		//				c++;
		//			if (Ein.chessboard[5 * (4 - i) + 4 - j] > 0)
		//				c++;
		//		}
		//	if (c>Ein.lastCount&&c>=11&&Ein.lastCount!=0&&!Ein.win)
		//		if (Ein.lastEnding == 2)
		//		{
		//			cout << "c:" << c << endl;
		//			return -1;//lose
		//		}
			
	}

	int myEinstein::beginCheck()
	{
		int c = 0;
		for (int i = 0; i <= 2; i++)
			for (int j = 0; j <= 2 - i; j++)
				if (Ein.chessboard[5 * i + j] == 0 || realBoard[5 * (4 - i) + 4 - j] == 0)
					c++;
		if (c > 1)
			return 0;
		else
		{
			beginTime = getTime(0);
			time(&btime);
			return 1;
		}

	}

	int myEinstein::getDice()
	{
		r =abs(r * rand() % 233333);
		if (handCheck)
		{
			int temp;
				temp = r % 13;
				if (temp <= 6)
				{
					temp += 6;
					return temp;
				}
		}
		else
		{
			int temp = r % 7;
			return temp;
		}
	}

	int myEinstein::diceChange(int d)
	{

		int value = 0, v1 = 0, v2 = 0;
		for (int i = 0; i < 25; i++)
			if (Ein.chessboard[i] == d)
				return evaluate(d, value);//get movement
		int dl = 0, dr = 0;
		updateDice(dl, dr);
		int m1 = evaluate(dl, v1), m2 = evaluate(dr, v2);
		Ein.dice = (v1 > v2) ? dl : dr;
		return (v1 > v2) ? m1 : m2;
	}

	int myEinstein::updateDice(int &l, int &r)
	{
		bool k = 0, j = 0;
		for (l= Ein.dice; l > 1; l--)
		{
			for (int i = 0; i < 25; i++)
				if (Ein.chessboard[i] == l)
					j = 1;
			if (j)
				break;
		}
		for (r = Ein.dice; r < 6; r++)
		{
			for (int i = 0; i < 25; i++)
				if (Ein.chessboard[i] == r)
					k = 1;
			if (k)
				break;
		}
		if (!j)
			l = r;
		else if (!k)
			r = l;
		return 0;
	}


	void myEinstein::site()
	{
		realdice = Ein.dice;
		if (Ein.dice<= 6)
		{
			int temp[25];
			rightDown = 0;
			for (int i = 0; i < 25; i++)
				reverseboard[i] = Ein.chessboard[24 - i];
			for (int i = 0; i < 25; i++)
				temp[i] = reverseboard[i];
			for (int i = 0; i < 25; i++)
				reverseboard[i] = Ein.chessboard[i];
			for (int i = 0; i < 25; i++)
				Ein.chessboard[i] = temp[i];
		}
		else
		{
			rightDown = 1;
			for (int i = 0; i < 25; i++)
			{
				if (Ein.chessboard[i] <= 6&& Ein.chessboard[i] > 0)
					Ein.chessboard[i] += 6;
				else if(Ein.chessboard[i] > 6)
					Ein.chessboard[i] -= 6;
			}
			Ein.dice -= 6;
		}
	
	}

	void myEinstein::strToch(string s, char *c)
	{
		int length = s.length();
		for (int i = 0; i < length; i++)
			c[i] = s[i];
	}

	void myEinstein::boardUpdate(int m)
	{
		if (!rightDown)
		{
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					if (realBoard[i * 5 + j] == realdice)
					{
						switch (m)
						{
						case 1: realBoard[i * 5 + j + 5] = realBoard[i * 5 + j];
							realBoard[i * 5 + j] = 0;
							Ein.chessboard[i * 5 + j - 5] = Ein.chessboard[i * 5 + j];
							Ein.chessboard[i * 5 + j] = 0;
							break;
						case 2:realBoard[i * 5 + j + 6] = realBoard[i * 5 + j];
							realBoard[i * 5 + j] = 0;
							Ein.chessboard[i * 5 + j - 6] = Ein.chessboard[i * 5 + j];
							Ein.chessboard[i * 5 + j] = 0; break;
						case 3:realBoard[i * 5 + j + 1] = realBoard[i * 5 + j];
							realBoard[i * 5 + j] = 0; 
							Ein.chessboard[i * 5 + j - 1] = Ein.chessboard[i * 5 + j];
							Ein.chessboard[i * 5 + j] = 0; break;
						}
					}
				}
			}
		}
		else for(int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (realBoard[i * 5 + j] == realdice)
				{
					switch (m)
					{
					case 1: realBoard[i * 5 + j - 5] = realBoard[i * 5 + j];
						realBoard[i * 5 + j] = 0;
						Ein.chessboard[i * 5 + j - 5] = Ein.chessboard[i * 5 + j];
						Ein.chessboard[i * 5 + j] = 0;
						break;
					case 2:realBoard[i * 5 + j - 6] = realBoard[i * 5 + j];
						realBoard[i * 5 + j] = 0;
						Ein.chessboard[i * 5 + j - 6] = Ein.chessboard[i * 5 + j];
						Ein.chessboard[i * 5 + j] = 0; break;
					case 3:realBoard[i * 5 + j - 1] = realBoard[i * 5 + j];
						realBoard[i * 5 + j] = 0;
						Ein.chessboard[i * 5 + j - 1] = Ein.chessboard[i * 5 + j];
						Ein.chessboard[i * 5 + j] = 0; break;
					}
				}
			}
		}
	}

	string myEinstein::getEndding(int &c)
	{
		if (getResult())
		{
			Ein.win = 1;
			return string("You Win!\n");
			c++;
		} 
		else
			return "";
	}

	void myEinstein::knowSite()
	{
		realdice = Ein.dice;
		if (!rightDown)
		{
			int temp[25];
			for (int i = 0; i < 25; i++)
				reverseboard[i] = Ein.chessboard[24 - i];
			for (int i = 0; i < 25; i++)
				temp[i] = reverseboard[i];
			for (int i = 0; i < 25; i++)
				reverseboard[i] = Ein.chessboard[i];
			for (int i = 0; i < 25; i++)
				Ein.chessboard[i] = temp[i];
		}
		else
		{
			for (int i = 0; i < 25; i++)
			{
				if (Ein.chessboard[i] <= 6 && Ein.chessboard[i] > 0)
					Ein.chessboard[i] += 6;
				else if (Ein.chessboard[i] > 6)
					Ein.chessboard[i] -= 6;
			}
			Ein.dice -= 6;
		}
	}

	void myEinstein::it(int i, char *s)
	{
		if (i == 0)
		{
			s[0] = '0';
			s[1] = '\0';
			return;
		}
		int j = 0, k = 0;
		char s1[1024] = { '\0' };
		for (; i > 0; j++)
		{
			s1[j] = (i % 10) + '0';
			i /= 10;
		}
		j--;
		for (; k <= j; k++)
			s[k] = s1[j - k];
		s[k] = '\0';

	}

	string myEinstein::lastingTime()
	{
		time(&etime);
		unsigned runTime = (unsigned)difftime(etime, btime);
		char runtime[81] = { '0' };
		it(runTime, runtime);
		string t = "runTime:" + (string)runtime + " s\n";
		return t;
	}



	string myEinstein::getMove(int m)
	{
		string move;
		if (Ein.dice <= 0)
			Ein.dice += 6;
		else if (Ein.dice > 6)
			Ein.dice -= 6;
		diceChange(Ein.dice);
		if (!rightDown)
		{
			char chess[10];
		
			realdice = Ein.dice;
			realdice = diceChange(realdice);
			it(realdice, chess);
			switch (m)
			{
			case 1:move = move + chess + "|down";	break;
			case 2:move = move + chess + "|rightdown"; break;
			case 3:move = move + chess + "|right"; break;
			}
		}
		else {
			char chess[10];
			realdice = Ein.dice + 6;
			it(realdice, chess);
			switch (m)
			{
			case 1:move = move + chess + "|up"; break;
			case 2:move = move + chess + "|leftup"; break;
			case 3:move = move + chess + "|left"; break;
			}
		}
		boardUpdate(m);
		return move;
	}

	string myEinstein::getTime(bool b)
	{
		time_t rawtime;
		struct tm * ptminfo;
		time(&rawtime);
		ptminfo = localtime(&rawtime);

		//printf("%02d-%02d-%02d %02d-%02d-%02d\n",
			//ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
			//ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
		if (b)
		{
			char y[10], m[10], d[10];
			string data;//date
			it( ptminfo->tm_year + 1900, y);
			it(ptminfo->tm_mon + 1, m);
			it(ptminfo->tm_mday, d);
			data = data + y + '-' + m + '-' + d;
			return data;
		}
		else
		{
			char y[10], m[10], d[10], h[10], min[10], s[10];
			string data;
			it(ptminfo->tm_year + 1900, y);
			it(ptminfo->tm_mon + 1, m);
			it(ptminfo->tm_mday, d);
			it(ptminfo->tm_hour, h);
			it(ptminfo->tm_min, min);
			it(ptminfo->tm_sec, s);
			data = data + y + '-' + m + '-' + d + ' ' + h + '-' + min + '-' + s+':';
			return data;
		}
	}

	string myEinstein::getBoard()
	{
		char d[10];
		it(realdice, d);
		string board;
		board += "[";
		for (int i = 0; i < 5; i++)
		{
			board += '[';
			for (int j = 0; j < 4; j++)
			{
				char c[10];
				it(realBoard[i * 5 + j], c);
				board += c;
				board += ", ";
			}
			if (i == 4)
			{
				char c[10];
				it(realBoard[24], c);
				board = board + c + "]]|" + d;
			}
			else {
				char c[10];
				it(realBoard[(i+1)*5-1], c);
				board = board + c + "], ";
			}
		}
		return board;
	}

	string myEinstein::humanControl()
	{
		int myDice, move, chess;
		handCheck = 1;
		myDice = getDice();
		cin >>chess>> move;
		handCheck = 0;
		return myMove(chess,move,myDice);
	}

	string myEinstein::myMove(int c,int m,int d)
	{
		int aiDice = getDice();
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (realBoard[i * 5 + j] == c)
				{
					switch (m)
					{
					case 1:realBoard[5 * (i - 1) + j] = realBoard[i * 5 + j];
						realBoard[i * 5 + j] = 0; break;
					case 2:realBoard[5 * (i - 1) + j-1] = realBoard[i * 5 + j];
						realBoard[i * 5 + j] = 0; break;
					case 3:realBoard[5 * i + j-1] = realBoard[i * 5 + j];
						realBoard[i * 5 + j] = 0; break;
					}
				}
			}
		}
		return getBoard();
	}