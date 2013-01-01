/// @file util/throttle.h
/// @author grigoryj

#include <stdint.h>

#ifndef THROTTLE_H
#define THROTTLE_H

namespace util
{
   /// A means for regulating the period with which
   /// certain things (like changing frame in an animation)
   /// may occur.
   class Throttle
   {
   public:
      explicit Throttle(int min_period);
      Throttle();

      bool operator()(uint64_t timestamp);

   private:
      int m_minPeriod;
      uint64_t m_lastActive;
   };
}

#endif // THROTTLE_H