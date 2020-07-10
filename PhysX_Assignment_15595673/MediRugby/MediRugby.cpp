#include <iostream>
#include "VisualDebugger.h"
#if       _WIN32_WINNT < 0x0500
#undef  _WIN32_WINNT
#define _WIN32_WINNT   0x0500
#endif
#include <windows.h> 

using namespace std;

void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

int main()
{
	//HIDES THE CONSOLE
	//HideConsole();
	try 
	{ 
		VisualDebugger::Init("Medieval Rugby", 1600, 900); 
	}
	catch (Exception exc) 
	{ 
		cerr << exc.what() << endl;
		return 0; 
	}

	VisualDebugger::Start();

	return 0;
}