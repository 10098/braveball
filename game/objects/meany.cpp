/// @file game/objects/meany.cpp
/// @author grigoryj

#include "game/objects/meany.h"

namespace game
{
   Meany::Meany(const Engine& e, int x, int y, int w, int h, int min_x, int max_x):
      GameObject(x, y, w, h),
      m_dead(false),
      m_opacity(0xff),
      m_minX(min_x),
      m_maxX(max_x),
      m_direction(1),
      m_fadeOutThrottle(2), 
      m_meanyDeath(e.resourceManager().getSound("meany_die.wav"), false)
   {
      addTag("meany");
   }

   void Meany::collide(GameObject& o) 
   {
      if(o.hasTag("player_dash"))
         collideDashingPlayer(o);
   }

   void Meany::collideDashingPlayer(GameObject&)
   {
      if(!m_dead)
      { 
         removeTag("meany");
         m_dead = true; 
         m_meanyDeath.play(); 
      } 
   }

   void Meany::draw(graphics::SpriteBatch& b)
   {
      b.add(x(), y(), m_anims.currentAnim().frame(), graphics::Color(0xff, 0xff, 0xff, m_opacity));
   }

   void Meany::update(Engine& e)
   {
      if(!m_dead)
      {
         m_anims.currentAnim().update(e.clock());
         updatePosition(e);
      }
      else if(m_fadeOutThrottle(e.clock().ticks()) && m_opacity > 0)
      {
         m_opacity -= 10;
         if(m_opacity < 0) m_opacity = 0;
      }
   }
}
