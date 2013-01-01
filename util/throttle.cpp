#include "util/throttle.h"

namespace util
{
   Throttle::Throttle(int min_period):
      m_minPeriod(min_period),
      m_lastActive(0)
   {}
   
   Throttle::Throttle():m_minPeriod(0), m_lastActive(0)
   {}

   bool Throttle::operator()(uint64_t timestamp)
   {
      // Pretty simple, check if more than minimum period of time passed 
      // since the last invocation, if yes - return true and update timestamp,
      // otherwise return false.
      if(timestamp - m_lastActive > m_minPeriod)
      {
         m_lastActive = timestamp;
         return true;
      }
      else
      {
         return false;
      }
   }
}