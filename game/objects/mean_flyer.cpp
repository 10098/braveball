/// @file game/objects/mean_flyer.cpp
/// @author grigoryj

#include "game/objects/mean_flyer.h"

namespace game
{
   MeanFlyer::MeanFlyer(const Engine& e, int x, int y, int min_x, int max_x):
      Meany(e, x, y, 16, 7, min_x, max_x),
      m_flyThrottle(15),
      m_upDownThrottle(20),
      m_baseY(y)
   {
      m_anims.addAnim("fly_left", graphics::Animation(e.resourceManager().getAnimationData("meanies/flyer/fly_left")));
      m_anims.addAnim("fly_right", graphics::Animation(e.resourceManager().getAnimationData("meanies/flyer/fly_right")));
      m_anims.setCurrentAnim("fly_right");
   }

   void MeanFlyer::updatePosition(Engine& e)
   {
      if(m_flyThrottle(e.clock().ticks()))
      {
         if(m_direction == 1 && x() > m_maxX || m_direction == -1 && x() < m_minX) {
            m_direction *= -1;
            m_anims.setCurrentAnim(m_direction == -1 ? "fly_left" : "fly_right");
         }

         setX(x() + m_direction);
         setY(m_baseY + 10 * sin(x() / 10.0));
      }
   }
}