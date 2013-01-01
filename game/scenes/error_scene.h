/// @file game/scenes/error_scene.h
/// @author grigoryj

#ifndef ERROR_SCENE_H
#define ERROR_SCENE_H

#include "game/engine.h"
#include "game/scene.h"
#include "graphics/image.h"
#include "sound/sound.h"

namespace game
{
   class EndScene : public Scene
   {
   public:
      explicit EndScene(const game::Engine& eng);

      virtual void focus(Engine& ctx, bool enter = true);
      virtual void update(Engine& ctx);
      virtual void mousePressed(Engine&, int, int, MouseButton) {}
      virtual void mouseReleased(Engine&, int, int, MouseButton) {}
      virtual void keyPressed(Engine&, Key) {}
      virtual void keyReleased(Engine&, Key) {}
      virtual void draw(Engine& ctx);

   private:
      const graphics::Image& m_endImg;
      sound::Sound m_music;
   };
}

#endif // ERROR_SCENE_H