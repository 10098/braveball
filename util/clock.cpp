/// @file util/clock.cpp
/// @author grigoryj

#include "util/clock.h"
#include <SDL/SDL.h>

namespace util
{
   uint64_t Clock::ticks() const
   {
      return SDL_GetTicks();
   }
}
