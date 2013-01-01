/// @file game/scenes/end_scene.h
/// @author grigoryj

#ifndef END_SCENE_H
#define END_SCENE_H

#include "game/engine.h"
#include "game/scene.h"
#include "game/objects/player.h"
#include "graphics/image.h"
#include "graphics/animation.h"
#include "graphics/retro_screen.h"
#include "sound/sound.h"
#include "util/throttle.h"
#include "graphics/sprite_batch.h"

namespace game
{
   class EndScene : public Scene
   {
   public:
      explicit EndScene(game::Engine& eng);

      virtual void focus(Engine& ctx, bool enter = true);
      virtual void update(Engine& ctx);
      virtual void mousePressed(Engine&, int, int, MouseButton) {}
      virtual void mouseReleased(Engine&, int, int, MouseButton) {}
      virtual void keyPressed(Engine&, Key);
      virtual void keyReleased(Engine&, Key) {}
      virtual void draw(Engine& ctx);

   private:
      const graphics::Image& m_atlas;
      graphics::RetroScreen m_screen;
      graphics::Animation m_credits;
      util::Throttle m_creditsThr;
      std::shared_ptr<graphics::Image> m_thanqImg;
      int m_creditsPos;
      sound::Sound m_music;
      game::Player m_player;
      graphics::SpriteBatch m_spriteBatch;
   };
}

#endif // END_SCENE_H