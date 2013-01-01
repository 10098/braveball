#include "game/objects/platform.h"
#include "game/objects/youngling.h"

namespace game
{
   Platform::Platform(const Engine& e, int x, int y, int l):
      GameObject(x, y, l * m_blockSize, m_blockSize),
      m_blocksNumber(l),  
      m_platformStart(e.resourceManager().getAnimationData("platform/start")),
      m_platformMiddle(e.resourceManager().getAnimationData("platform/middle")),
      m_platformEnd(e.resourceManager().getAnimationData("platform/end"))
   {
      addTag("platform");
   }

   void Platform::draw(graphics::SpriteBatch& b)
   {
      /// Draw the left edge
      b.add(x(), y(), m_platformStart.frame());

      /// Draw the middle section
      for(int a = 0; a < m_blocksNumber - 2; ++a)
      {
         b.add(x() + m_blockSize + 1 + a * m_blockSize, 
               y(), m_platformMiddle.frame());
      }

      /// Draw the right edge
      b.add(x() + m_blockSize + 1 + m_blockSize * (m_blocksNumber - 2),  
            y(), m_platformEnd.frame());

      /// Draw the game objects
      for(auto o = m_objects.begin(); o != m_objects.end(); ++o)
      {
         (*o) -> draw(b);
      }
   }

   void Platform::update(Engine& e)
   {
      /// Update all the game objects
      for(auto o = m_objects.begin(); o != m_objects.end(); ++o)
      {
         (*o) -> update(e);
      }
   }
}
