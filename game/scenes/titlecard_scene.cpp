#include "game/scenes/titlecard_scene.h"
#include "game/engine.h"
#include "util/resource_manager.h"
#include "game/objects/youngling.h"
#include "game/objects/player.h"
#include "game/objects/mean_flyer.h"

namespace game
{
   TitleCardScene::TitleCardScene(Engine& eng):
      m_atlas(eng.resourceManager().getImage("atlas")),
      m_title(eng.resourceManager().getAnimationData("splash")),
      m_music(eng.resourceManager().getSound("intro.mod"), true, true),
      m_screen(eng.graphics(), eng.graphics().screenContext(), 2, true),
      m_titleX(m_screen.width() / 2 - m_title.frame().w / 2),
      m_titleY(m_screen.height() / 2 - m_title.frame().h / 2),
      m_spriteBatch(m_atlas),
      m_pressAnyKey(eng.resourceManager().getFont("terminus.fon/8").renderString("PRESS ANY KEY TO PLAY")),
      m_seenHelp(false)
   {
      m_music.setVolume(0.4f);
      m_objects.push_back(std::shared_ptr<GameObject>(new Youngling(eng, m_titleX, m_titleY - 16)));
      m_objects.push_back(std::shared_ptr<GameObject>(new Player(eng, m_titleX + 72, m_titleY + 60)));
      m_objects.push_back(std::shared_ptr<GameObject>(new MeanFlyer(eng, m_titleX + 188, m_titleY - 32, m_titleX, m_titleX + m_title.frame().w)));
   }

   void TitleCardScene::focus(Engine& ctx, bool enter)
   {
      if(enter)
      {
         m_music.play(true);
         m_screen.activate();
         ctx.graphics().setClearColor(0xff, 0xff, 0xff);
      }
      else
      {
         m_music.stop();
      }
   }

   void TitleCardScene::update(Engine& ctx)
   {
      for(auto i = m_objects.begin(); i != m_objects.end(); ++i)
         (*i) -> update(ctx);
   }

   void TitleCardScene::keyPressed(Engine& ctx, Key key)
   {
      if(key == SDLK_ESCAPE)
      {
         SDL_Event e;
         e.quit.type = SDL_QUIT;
         SDL_PushEvent(&e);
      }
      else
      {
         if(!m_seenHelp)
         {
            m_seenHelp = true;
            ctx.setScene("help");
         }
         else
         {
            ctx.setScene("play");
         }
      }
   }

   void TitleCardScene::draw(Engine& ctx)
   {
      ctx.graphics().clear();
      m_spriteBatch.begin();
      m_spriteBatch.add(m_titleX, m_titleY, m_title.frame());
      for(auto i = m_objects.begin(); i != m_objects.end(); ++i)
         (*i) -> draw(m_spriteBatch);
      m_spriteBatch.end();
      m_pressAnyKey -> draw(m_screen.width() / 2 - m_pressAnyKey -> width() / 2, m_titleY + m_title.frame().h + 16);
      m_screen.draw();
      m_screen.activate();
   }
}
