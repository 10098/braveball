#include "game/objects/mean_walker.h"

namespace game
{
   MeanWalker::MeanWalker(const Engine& e, int x, int y, int min_x, int max_x):
      Meany(e, x, y, e.resourceManager().getAnimationData("meanies/walker/walk_left").frames[0].w, e.resourceManager().getAnimationData("meanies/walker/walk_left").frames[0].h, min_x, max_x),
      m_moveThrottle(15)
   {
      m_anims.addAnim("walk_left", graphics::Animation(e.resourceManager().getAnimationData("meanies/walker/walk_left")));
      m_anims.addAnim("walk_right", graphics::Animation(e.resourceManager().getAnimationData("meanies/walker/walk_right")));
      m_anims.setCurrentAnim("walk_right");
   }

   void MeanWalker::updatePosition(Engine& e)
   {
      if(m_moveThrottle(e.clock().ticks()))
      {
         // Switch walking direction when we reach the 
         // limits of the walking area.
         if(m_direction == 1 && x() > m_maxX || m_direction == -1 && x() < m_minX)
         {
            m_direction *= -1;
            m_anims.setCurrentAnim(m_direction == -1 ? "walk_left" : "walk_right");

         }
         setX(x() + m_direction);
      }
   }
}