#ifndef HELP_SCENE_H
#define HELP_SCENE_H

#include "game/engine.h"
#include "game/scene.h"
#include "graphics/retro_screen.h"
#include "graphics/image.h"
#include <vector>
#include <memory>

namespace game
{
   class HelpScene : public Scene
   {
   public:
      explicit HelpScene(game::Engine& eng);

      virtual void focus(Engine& ctx, bool enter = true);
      virtual void update(Engine& ctx);
      virtual void mousePressed(Engine&, int, int, MouseButton) {}
      virtual void mouseReleased(Engine&, int, int, MouseButton) {}
      virtual void keyPressed(Engine&, Key);
      virtual void keyReleased(Engine&, Key) {}
      virtual void draw(Engine& ctx);

   private:
      std::vector<std::shared_ptr<graphics::Image> > m_textLines;
      graphics::RetroScreen m_screen;
   };
}

#endif // HELP_SCENE_H