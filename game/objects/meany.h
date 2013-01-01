/// @file game/objects/meany.h
/// @author grigoryj

#ifndef MEANY_H
#define MEANY_H

#include "game/game_object.h"
#include "util/animation_collection.h"
#include "util/throttle.h"
#include "sound/sound.h"

namespace game
{
   class Meany : public GameObject
   {
   public:
      Meany(const Engine& e, int x, int y, int w, int h, int min_x, int max_x);

      virtual void update(Engine&);
      virtual void draw(graphics::SpriteBatch& b);

   protected:
      util::AnimationCollection m_anims;
      bool m_dead;
      int m_opacity;
      int m_minX;
      int m_maxX;
      int m_direction;
      util::Throttle m_fadeOutThrottle;
      sound::Sound m_meanyDeath;

   private:
      virtual void collide(GameObject& o);
      virtual void collideDashingPlayer(GameObject&);
      virtual void updatePosition(Engine& e) = 0;
   };
}

#endif // MEANY_H