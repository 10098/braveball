/// @file game/objects/exit_portal.h
/// @author grigoryj

#ifndef EXIT_PORTAL_H
#define EXIT_PORTAL_H

#include "game/game_object.h"

namespace game
{
   /// The exit portal appears at the end of the game, on the last platform.
   /// The player must jump into it to complete the game.
   class ExitPortal : public GameObject
   {
   public:
      ExitPortal(const Engine& e, int x, int y);

      virtual void update(Engine&);
      virtual void draw(graphics::SpriteBatch&);

   private:
      virtual void collide(GameObject&){}

      graphics::Animation m_exitSign;
      graphics::Animation m_portal;
   };
}

#endif // EXIT_PORTAL_H
