#include "game/objects/youngling.h"

namespace game
{
   const char* Youngling::m_animations[] = {"youngling/1", "youngling/2", "youngling/3", "youngling/4"};

   Youngling::Youngling(const Engine& e, int x, int y):
      GameObject(x, y, 6, 16),
      m_anim(e.resourceManager().getAnimationData(m_animations[rand()%(sizeof(m_animations) / sizeof(char*))])),
      m_saved(false),
      m_color(0xff, 0xff, 0xff, 0xff),
      m_saveSound(e.resourceManager().getSound("save.wav"), false),
      m_flyUpTo(y - e.graphics().screenContext().height()),
      m_fadeOutThrottle(12)
   {
      m_anim.setCurrentFrame(rand()%m_anim.frameCount());
      addTag("youngling");
   }

   void Youngling::update(Engine& e)
   {
      // Update animation if it's not saved yet
      if(!m_saved)
      {
         m_anim.update(e.clock());
      }
      else if(m_fadeOutThrottle(e.clock().ticks())) // Fly up and fade out otherwise
      {
         if(y() + 16 > m_flyUpTo) setY(y() - 3);
         if(m_color.a > 0) m_color.a -= m_color.a > 15 ? 15 : m_color.a;
      }
   }

   void Youngling::draw(graphics::SpriteBatch& b)
   {
      b.add(x(), y(), m_anim.frame(), m_color);
   }

   void Youngling::collide(GameObject& o)
   {
      if(o.hasTag("player"))
         collidePlayer(o);
      else if(o.hasTag("eater"))
         collideEater(o);
   }

   void Youngling::collidePlayer(GameObject& o)
   {
      if(!m_saved)
      {
         m_saved = true; // Become saved once the ball runs over us.
         m_saveSound.play();
      }
   }

   void Youngling::collideEater(GameObject& o)
   {
      if(!m_saved)
         addTag("dead_youngling");
   }
}
