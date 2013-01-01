#include "game/objects/threat.h"
#include "game/engine.h"

namespace game
{
   Threat::Threat(const game::Engine& e, int x, int y):
      GameObject(x, y, 15, 9),
      m_movementThr(15),
      m_anim(e.resourceManager().getAnimationData("meanies/king/king"))
   {
      addTag("king");
   }

   void Threat::update(Engine& ctx)
   {
      m_anim.update(ctx.clock());
      if(m_movementThr(ctx.clock().ticks()))
      {
         setX(x() + 1);
      }
   }

   void Threat::draw(graphics::SpriteBatch& b)
   {
      b.add(x(), y(), m_anim.frame());
   }
}
