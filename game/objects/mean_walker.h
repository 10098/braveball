#ifndef MEAN_WALKER_H
#define MEAN_WALKER_H

#include "game/objects/meany.h"
#include "game/engine.h"
#include "util/animation_collection.h"
#include "util/throttle.h"

namespace game
{
   /// Mean Walker game object
   ///
   /// Mean Walker walks the platform left and right.
   /// If the ball collides with it, the walker bites it,
   /// and the ball loses a life.
   class MeanWalker : public Meany
   {
   public:
      /// @param e Engine context
      /// @param x initial x position
      /// @param y initial y position
      /// @param min_x left edge of the walking area
      /// @param max_x right edge of the walking area
      MeanWalker(const Engine& e, int x, int y, int min_x, int max_x);
   
   private:
      virtual void updatePosition(Engine& e);

      util::Throttle m_moveThrottle;
   };
}

#endif // MEAN_WALKER_H