#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

class ColorCon
{
private:

	int lastPosRow = 0;
	HANDLE hStdout;


public:

	void test()
	{

	}

	void cls(HANDLE hConsole, int x, int y, int size, char ch = ' ');

	BOOL printConsole(char * String, int x, IN WORD y, WORD attrib);

	template<typename... Args>
	inline void sprintConsole(int x, int y, WORD attrib, char * String, Args... args)
	{
		char temp[512];
		CONSOLE_SCREEN_BUFFER_INFO lp;
		int gTop = 0;
		//static int gNewY = 0;
		//static int gLastY = 0;
		int gBottom = 0;
		//int gLastPosRow = 0;
		int posRow = 0;
		HANDLE handle;
		int adv;


		COORD eraseCoord;

		handle = GetStdHandle(STD_OUTPUT_HANDLE);

		GetConsoleScreenBufferInfo(handle, &lp);
		//gNewY = lp.dwCursorPosition.Y - gLastY;
		gBottom = lp.srWindow.Bottom;
		gTop = lp.srWindow.Top;

		//		logit << "should delete previous row at:" << lastPosRow << endl;
		cls(handle, x, lastPosRow, strlen(String) + 1, ' ');
		//cout << gTop;

		//below, former is never the case == never happens...why, yet it works?
		//posRow = (gCurY == gBottom) ? gTop + gNewY  +y: gTop + y;
		posRow = gTop + y;
		lastPosRow = posRow;
		//Y is row

		//		logit << "gNewY:" << gNewY << " gBottom:" << gBottom << " gTop:" << gTop << " gPosRow:" << posRow << std::endl;

		adv = gBottom - gTop; //this is how many times we've been scrolled and/or lines that someone else has printed. 



							  //	gTop = lp.srWindow.Top;

							  //if(y==-1)
							  //printf("posrow:%d\tadvanceY:%d\tgTop:%d\n", posRow, gNewY,gTop);
							  //erase the old


							  //end erase


		sprintf(temp, String, args...);

		//erase previous
		//if(gCurY == gBottom)//this is never the case...hmmm

		//cls(handle, x, posRow-1, 3,'X');
		//printConsole(temp, x, gLastPosRow , FOREGROUND_GREEN);
		//end erase

		//		printConsole(temp, x, (anchorRow) ? posRow : y, attrib);
		printConsole(temp, x, posRow, attrib);

		//	gLastPosRow = posRow;
		//	gLastY = lp.dwCursorPosition.Y;

	}




	void clearBox(int startX, int startY, int colWidth, int rowWidth, TCHAR ch = 'X');
	int ScrollByAbsoluteCoord(int iRows);
	int ScrollByRelativeCoord(int iRows);

	ColorCon::ColorCon()
	{
	}


	ColorCon::~ColorCon()
	{
	}



};

