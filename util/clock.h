/// @file util/clock.h
/// @author grigoryj

#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

namespace util
{
   /// Just a wrapper around SDL's clock
   class Clock
   {
   public:
      /// Get the current number of milliseconds since some fixed point in the past
      uint64_t ticks() const;
   };
}

#endif // CLOCK_H