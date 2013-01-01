#include "game/scenes/help_scene.h"

namespace game
{
   HelpScene::HelpScene(game::Engine& e):
      m_screen(e.graphics(), e.graphics().screenContext(), 2, false)
   {
      const char* lines[] = {
         "The evil dark meanies have invaded the Land of Weird Little Creatures.", 
         "You are the only hope!",
         " "
         "Save all the younglings and reach the Exit Portal.", 
         "It will teleport you to a safe place.",
         " ",
         "Use left/right arrows to move", "spacebar to jump", "X to dash when your power meter is red.",
         " ",
         "Beware of the King Meany!", "Nothing can stop him, not even your magical dash attack.", "He's so mean even the sky turns red when he approaches!",
         " ",
         "Press Enter to continue"
      };

      const graphics::Font& f = e.resourceManager().getFont("terminus.fon/8");

      for(int i = 0; i < sizeof(lines)/sizeof(char*); ++i)
         m_textLines.push_back(std::shared_ptr<graphics::Image>(f.renderString(lines[i])));
   }

   void HelpScene::update(game::Engine& ctx)
   {

   }

   void HelpScene::focus(game::Engine& ctx, bool enter)
   {
      if(enter)
      {
         m_screen.activate();
      }
   }

   void HelpScene::keyPressed(Engine& e, Key k)
   {
      if(k == SDLK_RETURN)
         e.setScene("play");
   }


   void HelpScene::draw(game::Engine& ctx)
   {
      ctx.graphics().clear();
      for(int i = 0; i < m_textLines.size(); ++i)
      {
         graphics::Image* img = m_textLines[i].get();
         img -> draw(m_screen.width() / 2 - img -> width() / 2, (m_screen.height()/2 - m_textLines.size() * (img->height() + 7) / 2) + i * (img -> height() + 7));
      }
      m_screen.draw();
      m_screen.activate();
   }
}