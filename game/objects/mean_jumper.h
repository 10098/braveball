/// @file game/objects/mean_jumper.h
/// @author grigoryj

#ifndef MEAN_JUMPER_H
#define MEAN_JUMPER_H

#include "game/objects/meany.h"
#include "util/throttle.h"

namespace game
{
   class Engine;

   class MeanJumper : public Meany
   {
   public:
      MeanJumper(const Engine& e, int x, int y, int min_x, int max_x);

   private:
      virtual void updatePosition(Engine& e);

      enum JumperState
      {
         Standing,
         Jumping,
         Landing
      };

      JumperState m_state;
      int m_target;
      int m_startX;
      int m_startY;
      util::Throttle m_jumpThrottle;
      util::Throttle m_jumpMoveThrottle;
   };
}

#endif // MEAN_JUMPER_H