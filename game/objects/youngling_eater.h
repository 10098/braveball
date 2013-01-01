/// @file game/objects/youngling_eater.h
/// @author grigoryj

#ifndef YOUNGLING_EATER_H
#define YOUNGLING_EATER_H

#include "game/game_object.h"

namespace game
{
   /// Youngling Eater is an invisible object
   /// that follows the camera and "eats" unsaved 
   /// younglings. When a youngling gets "eaten", a 
   /// game over is triggered because no one must be
   /// left behind!
   class YounglingEater : public GameObject
   {
   public:
      /// Create a new eater
      /// @param initial_x Initial value of the x coordinate
      /// @param initial_y Initial value of the y coordinate
      YounglingEater(int initial_x, int initial_y);

      virtual void update(Engine&){}
      virtual void draw(graphics::SpriteBatch&);

   private:
      virtual void collide(GameObject&){}
   };
}

#endif // YOUNGLING_EATER_H