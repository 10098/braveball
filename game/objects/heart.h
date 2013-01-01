/// @file game/objects/heart.h
/// @author grigoryj

#ifndef HEART_H
#define HEART_H

#include "game/game_object.h"
#include "game/engine.h"
#include "util/throttle.h"

namespace game
{
   /// Heart game object
   ///
   /// When walked over, a heart gives the ball 
   /// an additional life and disappears, if the
   /// ball has less than 3 lifes. Otherwise, the 
   /// heart just silently disappears.
   class Heart : public GameObject
   {
   public:
      /// @param e Engine context
      /// @param x x-coordinate for the heart
      /// @param y y-coordinate for the heart
      Heart(const Engine& e, int x, int y);

      /// Updates the internal state.
      ///
      /// Doesn't do much, just animates the heart by
      /// jiggling it up and down the Y axis.
      /// @param ctx Engine context
      virtual void update(Engine& ctx);

      /// Draws the heart sprite on screen
      /// @param b The sprite batch
      virtual void draw(graphics::SpriteBatch& b);

   private:
      /// Handles collisions with Heart
      virtual void collide(GameObject& o);

      graphics::Animation m_anim; /// Heart animation
      int m_baseY;                /// Base value for Y 
      int m_direction;            /// Direction in which the heart sprite is traveling along the Y axis
      util::Throttle m_heartThr;  /// Throttle for the animation
      bool m_taken;               /// Is true if the heart has been taken by the ball
   };
}

#endif // HEART_H