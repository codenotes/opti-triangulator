#include "ColorCon.h"

#include <stdlib.h>




void ColorCon::cls(HANDLE hConsole, int x, int y, int size, char ch)
{						//X is col, y is row
	COORD coordScreen = { 3, 10 };    /* here's where we'll home the
									  cursor and where the draw will begin*/
	BOOL bSuccess;
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
	DWORD dwConSize;                 /* number of character cells in
									 the current buffer */

									 /* get the number of character cells in the current buffer */

	coordScreen.X = x;
	coordScreen.Y = y;
	dwConSize = size;

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	//		PERR(bSuccess, "GetConsoleScreenBufferInfo");
	//dwConSize = (csbi.dwSize.X) * (csbi.dwSize.Y);

	/* fill the entire screen with blanks */

	bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR)ch,
		dwConSize, coordScreen, &cCharsWritten);
	//	PERR(bSuccess, "FillConsoleOutputCharacter");

	/* get the current text attribute */

	bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
	//PERR(bSuccess, "ConsoleScreenBufferInfo");

	/* now set the buffer's attributes accordingly */

	bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten);
	//PERR(bSuccess, "FillConsoleOutputAttribute");

	/* put the cursor at (0, 0) */

	//bSuccess = SetConsoleCursorPosition(hConsole, coordScreen);
	//PERR(bSuccess, "SetConsoleCursorPosition");
	return;
}






BOOL
ColorCon::printConsole(char* String, int x,
	IN WORD y, WORD attrib
	)
{
	CHAR_INFO Buffer[100];
	COORD BufferSize;
	COORD BufferCoord;
	SMALL_RECT WriteRegion;
	int i;
	BOOL Success;

	int slen = strlen(String);

	BufferSize.X = slen;//10;
	BufferSize.Y = 1;
	BufferCoord.X = 0;
	BufferCoord.Y = 0;
	WriteRegion.Left = x; //was 0
	WriteRegion.Top = y;
	WriteRegion.Right = x + slen;// 14;
	WriteRegion.Bottom = y;

	//if unicode
#ifdef UNICODE
	wchar_t* wc_out = new wchar_t[slen + 1];
	size_t result = mbstowcs(wc_out, String, slen);

	for (i = 0; i < slen; i++)
	{
		Buffer[i].Char.UnicodeChar = wc_out[i];
		Buffer[i].Attributes = attrib;
	}

	delete wc_out;
#else
	for (i = 0; i < slen; i++)
	{
		Buffer[i].Char.AsciiChar = String[i];
		Buffer[i].Attributes = attrib;
	}
#endif	

	Success = WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE),
		Buffer,
		BufferSize,
		BufferCoord,
		&WriteRegion
		);
	return Success;
}




void ColorCon::clearBox(int startX, int startY, int colWidth, int rowWidth, TCHAR ch)
{

	for (size_t i = 0; i < rowWidth; i++)
	{
		cls(GetStdHandle(STD_OUTPUT_HANDLE), startX, startY + i, colWidth, ch);
	}



}


int ColorCon::ScrollByAbsoluteCoord(int iRows)

{

	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	SMALL_RECT srctWindow;



	// Get the current screen buffer size and window position.

	if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))

	{

		//			wprintf(L"GetConsoleScreenBufferInfo() failed, error %d\n", GetLastError());

		return 1;

	}



	// Set srctWindow to the current window size and location.

	srctWindow = csbiInfo.srWindow;



	// Check whether the window is too close to the screen buffer top

	if (srctWindow.Top >= iRows)

	{

		srctWindow.Top -= iRows;     // move top up

		srctWindow.Bottom -= iRows;  // move bottom up



		if (!SetConsoleWindowInfo(

			hStdout,          // screen buffer handle

			TRUE,             // absolute coordinates

			&srctWindow))     // specifies new location

		{

			//		wprintf(L"SetConsoleWindowInfo() failed, error %d\n", GetLastError());

			return 1;

		}

		return iRows;

	}

	else

	{

		//wprintf(L"\nCannot scroll; the window is too close to the top.\n");

		return 1;

	}

}



int ColorCon::ScrollByRelativeCoord(int iRows)

{

	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	SMALL_RECT srctWindow;



	// Get the current screen buffer window position.

	if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))

	{

		//			wprintf(L"GetConsoleScreenBufferInfo(), error %d\n", GetLastError());

		return 1;

	}



	// Check whether the window is too close to the screen buffer top

	if (csbiInfo.srWindow.Top >= iRows)

	{

		srctWindow.Top = -iRows;     // move top up

		srctWindow.Bottom = -iRows;  // move bottom up

		srctWindow.Left = 0;         // no change

		srctWindow.Right = 0;        // no change



		if (!SetConsoleWindowInfo(

			hStdout,          // screen buffer handle

			FALSE,            // relative coordinates

			&srctWindow))     // specifies new location

		{

			//				wprintf(L"SetConsoleWindowInfo(), error %d\n", GetLastError());

			return 0;

		}

		return iRows;

	}

	else

	{

		//			wprintf(L"\nCannot scroll; the window is too close to the top.\n");

		return 0;

	}

}



