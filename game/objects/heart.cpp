#include "game/objects/heart.h"

namespace game
{
   Heart::Heart(const Engine& e, int x, int y):
      GameObject(x, y, e.resourceManager().getAnimationData("powerups/heart").frames[0].w, e.resourceManager().getAnimationData("powerups/heart").frames[0].h),
      m_anim(e.resourceManager().getAnimationData("powerups/heart")),
      m_baseY(y),
      m_direction(-1),
      m_heartThr(80),
      m_taken(false)
   {
      addTag("heart");
   }

   void Heart::update(Engine& ctx)
   {
      if(m_heartThr(ctx.clock().ticks())) // Regulate animation speed
      {
         // Change direction when the y-coordinate reaches
         // upper or lower limit
         if(m_direction == -1 && y() <= m_baseY - height()/2 || 
            m_direction == 1 && y() >= m_baseY + height()/2)
         {
            m_direction *= -1;
         }
         setY(y() + m_direction);
      }
   }

   void Heart::draw(graphics::SpriteBatch& b)
   {
      // Draw the heart on screen if it is not taken
      if(!m_taken)
         b.add(x(), y(), m_anim.frame());
   }

   void Heart::collide(GameObject& o)
   {
      if(o.hasTag("player"))
         m_taken = true;
   }
}
