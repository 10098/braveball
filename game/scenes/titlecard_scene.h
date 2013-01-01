/// @file game/scenes/titlecard_scene.h
/// @author grigoryj

#ifndef TITLECARD_SCENE_H
#define TITLECARD_SCENE_H

#include "game/scene.h"
#include "game/game_object.h"
#include "sound/sound.h"
#include "graphics/image.h"
#include "graphics/animation.h"
#include "graphics/font.h"
#include "graphics/retro_screen.h"
#include "graphics/sprite_batch.h"
#include <list>
#include <memory>

namespace game
{
   /// The title card. Shown when the game starts.
   class TitleCardScene : public Scene
   {
   public:
      explicit TitleCardScene(Engine& eng);

   public:
      virtual void focus(Engine& ctx, bool enter = true);
      virtual void update(Engine& ctx);
      virtual void mousePressed(Engine&, int, int, MouseButton) {}
      virtual void mouseReleased(Engine&, int, int, MouseButton) {}
      virtual void keyPressed(Engine&, Key);
      virtual void keyReleased(Engine&, Key) {}
      virtual void draw(Engine& ctx);

   private:
      const graphics::Image& m_atlas;

      graphics::Animation m_title;
      graphics::RetroScreen m_screen;
      graphics::SpriteBatch m_spriteBatch;
      sound::Sound m_music;
      int m_titleX;
      int m_titleY;
      std::list<std::shared_ptr<GameObject> > m_objects;
      std::shared_ptr<graphics::Image>  m_pressAnyKey;
      bool m_seenHelp;
   };
}

#endif // TITLECARD_SCENE_H
