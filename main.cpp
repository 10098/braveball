/// @file main.cpp
/// @author grigoryj

#include "game/engine.h"

int main(int argc, char* argv[])
{
   game::Engine eng;
   eng.run(argc, argv, "Brave Ball");
   return 0;
}
