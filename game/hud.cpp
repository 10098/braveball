/// @file game/hud.cpp
/// @author grigoryj

#include "game/hud.h"

namespace game
{
   HUD::HUD(const Engine& e, const Player* p):
      m_heart(e.resourceManager().getAnimationData("powerups/heart")),
      m_font(e.resourceManager().getFont("terminus.fon/8")),
      m_player(p)
   {}

   void HUD::draw(int x, int y, graphics::SpriteBatch& b) const
   {
      const int GapBetweenHearts = 3;
      const int MaxHearts = 3;
      const int PowerBarOffset = 32;
      const int PowerBarBorderWidth = 2;
      const int PowerbarTextureY = 114;

      /// Draw the remaining lives
      for(int i = 0; i < m_player -> lives() - 1; ++i)
      {
         b.add(x + i * (m_heart.frame().w + GapBetweenHearts), y, m_heart.frame());
      }

      int powerbar_pos = x + (m_heart.frame().w + GapBetweenHearts) * MaxHearts + PowerBarOffset;
      int powerbar_width = Player::MaxPowerLevel;
      int powerbar_height = m_heart.frame().h;

      // Draw a black border around the power bar
      b.add(powerbar_pos - 1, y - 1,
            graphics::AlignedQuad(0, 0, powerbar_width + PowerBarBorderWidth, powerbar_height + PowerBarBorderWidth), 
            graphics::Color(0x00, 0x00, 0x00));

      // Draw a gray background
      b.add(powerbar_pos, y, 
            graphics::AlignedQuad(0, PowerbarTextureY, powerbar_width, powerbar_height), 
            graphics::Color(0xaa, 0xaa, 0xaa));

      // Draw the power level
      b.add(powerbar_pos, y, 
            graphics::AlignedQuad(0, PowerbarTextureY, m_player -> power(), powerbar_height), 
            m_player -> canDash() ? graphics::Color(0xff, 0x00, 0x00) : graphics::Color(0x00, 0xff, 0x00)); // red if the player is able to dash, green otherwise

      // Indicate the power spent on a dash with a yellow rectangle
      int cost = m_player -> dashLength();
      bool negative_power = cost > m_player -> power();
      b.add(powerbar_pos + (m_player -> power() - (negative_power ? 0 : cost)), y, 
            graphics::AlignedQuad(0, PowerbarTextureY, (negative_power ? 0 : cost), powerbar_height), 
            graphics::Color(0xff, 0xff, 0x00));
   }
}
