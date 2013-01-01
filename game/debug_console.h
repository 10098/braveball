/// @file game/debug_console.h
/// @author grigoryj

#ifndef DEBUG_CONSOLE_H
#define DEBUG_CONSOLE_H

namespace game
{
   /// Need this to be able to see stuff the program prints to stdout on Windows.
   /// Assists debugging a great deal. Implementation copypasted from somewhere on the interwebz.
   void initDebugConsole();
}


#endif // DEBUG_CONSOLE_H