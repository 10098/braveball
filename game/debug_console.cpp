#include "game/debug_console.h"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>

namespace game
{
   void initDebugConsole()
   {
// Make sure the console is created only when building with MS VS in Debug mode.
#ifdef _MSC_VER
#ifdef _DEBUG // I had to add this in Project Properties
      //Create a console for this application
      AllocConsole();

      //Redirect unbuffered STDOUT to the console
	   HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
      int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
      FILE *COutputHandle = _fdopen(SystemOutput, "w" );
      *stdout = *COutputHandle;
      setvbuf(stdout, NULL, _IONBF, 0);

      //Redirect unbuffered STDERR to the console
	   HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
      int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
      FILE *CErrorHandle = _fdopen(SystemError, "w" );
      *stderr = *CErrorHandle;
      setvbuf(stderr, NULL, _IONBF, 0);

      //Redirect unbuffered STDIN to the console
	   HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
      int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
      FILE *CInputHandle = _fdopen(SystemInput, "r" );
      *stdin = *CInputHandle;
      setvbuf(stdin, NULL, _IONBF, 0);
    
      //make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
      std::ios::sync_with_stdio(true);
#endif // _DEBUG
#endif // _MSC_VER
   }
}