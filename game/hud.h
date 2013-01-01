/// @file game/hud.h
/// @author grigoryj

#ifndef HUD_H
#define HUD_H

#include "game/engine.h"
#include "game/objects/player.h"
#include "graphics/animation.h"

namespace game
{
   /// Heads-up display
   class HUD
   {
   public:
      /// @param e the engine context
      /// @param p the player object
      HUD(const Engine& e, const Player* p);

      /// Draw the HUD on screen
      /// @param b The sprite batch
      void draw(int x, int y, graphics::SpriteBatch& b) const;

   private:
      graphics::Animation m_heart;
      const graphics::Font& m_font;
      const Player* m_player;
   };
}

#endif // HUD_H
