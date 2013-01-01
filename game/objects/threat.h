/// @file game/objects/threat.h
/// @author grigoryj

#ifndef THREAT_H
#define THREAT_H

#include "game/game_object.h"
#include "graphics/animation.h"

namespace game
{
   class Threat : public GameObject
   {
   public:
      Threat(const game::Engine& e, int x, int y);

      void update(Engine& x);
      void draw(graphics::SpriteBatch&);

   private:
      void collide(GameObject&){}

      util::Throttle m_movementThr;
      graphics::Animation m_anim;
   };
}


#endif // THREAT_H