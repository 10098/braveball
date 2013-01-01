/// @file game/objects/yougling.h
/// @author grigoryj

#ifndef YOUNGLING_H
#define YOUNGLING_H

#include "game/game_object.h"
#include "sound/sound.h"
#include "util/throttle.h"

namespace game
{
   /// Younglings jump up and down on the platforms
   /// and wait to be saved by the brave ball.
   class Youngling : public GameObject
   {
   public:
      /// @param e engine context
      /// @param x initial x position
      /// @param y initial y position
      Youngling(const Engine& e, int x, int y);

      /// Update internal state
      /// @param ctx Engine context
      virtual void update(Engine& ctx);

      /// Draw on screen
      /// @param b The sprite batch
      virtual void draw(graphics::SpriteBatch& b);

   private:
      virtual void collide(GameObject& o);
      void collidePlayer(GameObject&);
      void collideEater(GameObject&);

      static const char* m_animations[]; /// Names of animations for differently colored younglings
      graphics::Animation m_anim; /// Animation to use
      bool m_saved;               /// Set to true when the youngling is saved
      graphics::Color m_color;    /// Used to fade out the sprite
      sound::Sound m_saveSound;   /// Save sound effect
      int m_flyUpTo;              /// Where to fly up when saved
      util::Throttle m_fadeOutThrottle; /// Throttle the fly up & fade out
   };
}

#endif // YOUNGLING_H