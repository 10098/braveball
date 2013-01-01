/// @file mean_jumper.cpp
/// @author grigoryj

#include "game/objects/mean_jumper.h"
#include "game/engine.h"

namespace game
{
   MeanJumper::MeanJumper(const Engine& e, int x, int y, int min_x, int max_x):
      Meany(e, x, y, 10, 10, min_x, max_x),
      m_state(Standing),
      m_jumpThrottle(1000),
      m_jumpMoveThrottle(14)
   {
      const char* anim_names[] = {"jump_left", "jump_right"};
      for(int i = 0; i < sizeof(anim_names)/sizeof(char*); ++i)
      {
         m_anims.addAnim(anim_names[i], graphics::Animation(e.resourceManager().getAnimationData(std::string("meanies/jumper/") + anim_names[i])));
      }
      m_anims.setCurrentAnim("jump_right");
   }
   
   void MeanJumper::updatePosition(Engine& e)
   {
      if(m_state == Standing) // If standing still, initiate jump
      {
         if(m_jumpThrottle(e.clock().ticks()))
         {
            // Decide on the target for the jump.
            // We'll jump in the direction where more space is available.
            //bool s = x() - m_minX > m_maxX - x();
            m_direction *= -1; //s ? -1 : 1;
            int jump_length_limit = abs(x() + m_direction*32 - (m_direction==-1 ? m_minX : m_maxX));
            m_target = x() + m_direction * (32 + rand() % min(24, jump_length_limit));
            m_startX = x();
            m_startY = y();

            // Change animation if necessary
            m_anims.setCurrentAnim(m_direction == 1 ? "jump_left" : "jump_right");

            // Change state
            m_state = Jumping;
         }
      }
      else if(m_state == Jumping)
      {
         if(m_jumpMoveThrottle(e.clock().ticks()))
         {
            if(m_anims.currentAnim().currentFrameNumber() > 2) {
               setX(x() + m_direction);
               setY(abs(m_startX - x()) < abs(m_startX - m_target)/2 ? y() - 1 : y() + 1);
            }

            if(m_direction == -1 && x() <= m_target || m_direction == 1 && x() >= m_target){
               setX(m_target);
               setY(m_startY);
               m_anims.setCurrentAnim(m_direction == 1 ? "jump_left" : "jump_right");
               m_state = Landing;
            }
         }
      }
      else if(m_state == Landing)
      {
         if(m_anims.currentAnim().currentFrameNumber() > 3)
            m_state = Standing;
      }
   }
}
