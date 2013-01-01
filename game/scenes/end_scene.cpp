/// @file game/scenes/end_scene.cpp
/// @author grigoryj

#include "game/scenes/end_scene.h"
#include <iostream>

namespace game
{  
   EndScene::EndScene(Engine& e):
      m_atlas(e.resourceManager().getImage("atlas")),
      m_screen(e.graphics(), e.graphics().screenContext(), 2, true),
      m_credits(e.resourceManager().getAnimationData("credits")),
      m_creditsThr(20),
      m_music(e.resourceManager().getSound("end_music.wav"), true, false),
      m_player(e, 0, 0),
      m_thanqImg(e.resourceManager().getFont("terminus.fon/15").renderString("THANKS FOR PLAYING! PRESS ESC TO RETURN TO THE MAIN MENU.")),
      m_spriteBatch(m_atlas)
   {
   }

   void EndScene::focus(Engine&, bool enter)
   {
      if(enter)
      {
         m_creditsPos = m_screen.height()/2;
         m_player.setX(m_screen.width() / 2 - m_player.width() / 2);
         m_player.setY(m_creditsPos + m_credits.frame().h + 32);
         m_music.play(true);
      }
      else
      {
         m_music.stop();
      }
   }
   
   void EndScene::keyPressed(Engine& ctx, Key k)
   {
      if(k == SDLK_ESCAPE)
         ctx.setScene("splash");
   }

   void EndScene::update(Engine& ctx)
   {
      if(m_creditsThr(ctx.clock().ticks())){
         m_creditsPos -= 1;
         m_player.update(ctx);
         if(m_player.y() > m_screen.height()/2) 
            m_player.setY(m_player.y() - 1);
      }
   }

   void EndScene::draw(Engine& ctx)
   {
      ctx.graphics().clear();
      m_spriteBatch.begin();
      m_spriteBatch.add(m_screen.width()/2 - m_credits.frame().w / 2, m_creditsPos, m_credits.frame());
      m_player.draw(m_spriteBatch);
      m_spriteBatch.end();
      m_thanqImg -> draw(m_screen.width()/2 - m_thanqImg -> width() / 2, m_player.y() + m_player.height() + 16); 
      m_screen.draw();
      m_screen.activate();
   }
}
