/// @file game/objects/exit_portal.cpp
/// @author grigoryj

#include "game/objects/exit_portal.h"

namespace game
{
   ExitPortal::ExitPortal(const Engine& e, int x, int y) : 
      GameObject(x, y, 6, 24), 
      m_exitSign(e.resourceManager().getAnimationData("exitportal/exit_sign")),
      m_portal(e.resourceManager().getAnimationData("exitportal/portal"))
   {
      addTag("exit_portal");
   }

   void ExitPortal::update(Engine& e)
   {
      m_portal.update(e.clock());
   }

   void ExitPortal::draw(graphics::SpriteBatch& b)
   {
      int x_pos = x() - (m_portal.frame().w - width())/2;
      b.add(x_pos, y(), m_portal.frame());
      b.add(x_pos - 40, y() + abs(m_exitSign.frame().h - m_portal.frame().h), m_exitSign.frame());
   }
}