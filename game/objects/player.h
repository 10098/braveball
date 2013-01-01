/// @file game/objects/player.h
/// @author grigoryj

#ifndef PLAYER_H
#define PLAYER_H

#include "game/game_object.h"
#include "sound/sound.h"
#include "util/throttle.h"
#include "util/animation_collection.h"

namespace game
{
   class GameObject;
   class Engine;

   /// Player class
   /// Responds to user input, 
   /// handles collisions with various 
   /// game objects.
   class Player : public GameObject
   {
   public:
      Player(const game::Engine& e, int x, int y);

      /// Updates the animation, position, etc.
      virtual void update(Engine& ctx);

      /// Displays the character on screen
      virtual void draw(graphics::SpriteBatch& b);

      /// @return The number of lives the player has
      int lives() const;

      /// @return The amount of power the player collected by saving younglings
      int power() const;

      /// @return If the player is dashing, returns how far he has dashed, 0 otherwise
      int dashLength() const;

      /// @return true if the player can dash (power meter has reached maximum value), false otherwise
      bool canDash() const;

      /// @return true if the player has reached the Exit Portal, false otherwise
      bool reachedExit() const;

      /// Initiates a jump
      void jump();
      
      /// Stops the jump (if the player is jumping)
      void stopJump();

      /// Starts a dash
      void dash();

      /// Stops a dash
      void stopDash();

      /// Starts walking right
      void walkRight();

      /// Starts walking left
      void walkLeft();

      /// Stops walking right
      void stopWalkRight();

      /// Stops walking left
      void stopWalkLeft();

      /// Sets the minimum allowed X coordinate for the player
      void setMinX(int min_x);

      static const int MaxPowerLevel = 128;

   private:
      virtual void collide(GameObject& o);

      /// Handles collision with a platform.
      /// Makes sure the player doesn't fall down 
      /// when standing on a platform.
      void collidePlatform(GameObject&);

      /// Handles collision with an enemy.
      /// Reduces life by 1
      void collideMeany(GameObject&);

      /// Handles collision with a heart.
      /// If the player has a full life bar, adds 1 power
      /// Otherwise adds 1 hit point.
      void collideHeart(GameObject&);

      /// Handles collision with the King Meany
      /// Death on impact!
      void collideKing(GameObject&);

      /// Handles collision with a Youngling.
      /// Adds 5 power
      void collideYoungling(GameObject&);

      /// Handles collision with the Exit Portal.
      /// Sets an internal flag
      void collideExitPortal(GameObject&);

      void updateJumpState(Engine& ctx);
      void updatePostHitInvincibility(Engine& ctx);
      void updatePosition(Engine& ctx);
      void addPower(int amount);

      enum JumpState
      {
         Standing,
         Jumping,
         Falling,
         Landing
      };

      util::AnimationCollection m_anims; /// Collection of player's animations

      sound::Sound m_landSound; /// Sound emitted when landing
      sound::Sound m_jumpSound; /// Sound emitted when jumping
      sound::Sound m_hurtSound; /// Sound emitted when the player is hurt
      sound::Sound m_heartSound;/// Sound emitted when a hit point is added after colliding a heart
      sound::Sound m_heartPtsSound; /// Sound emitted when a power point is added after colliding a heart
      sound::Sound m_dashSound; /// Sound emitted when dashing

      int m_lives; /// The number of hit points the player has
      bool m_hurt; /// Indicates if the player is hurt
      bool m_reachedExit; /// Indicates if the player has reached the exit portal
      int64_t m_hurtTime; /// Indicates how long the player has retained post-hit invincibility
      int m_speed; /// How fast the player should move
      int m_direction; /// In which direction the player should move (-1 left, 1 right)
      int m_kickBackDirection; /// Direction of post-hit knockback
      int m_fallSpeed; /// How fast the player falls 

      util::Throttle m_accelerationThr; /// Throttles acceleration 
      util::Throttle m_posUpdateThr; /// Throttles position update

      JumpState m_jumpState;
      bool m_moveForward; /// Is the character supposed to be moving or standing still?
      bool m_breakJump;   /// Should we stop the jump?
      int m_beforeJumpY;  /// What was the Y coordinate when the jump started?
      int m_currentPlatformStart; /// Where is the start of the current platform?
      int m_currentPlatformEnd;   /// Where is the end of the current platform?
      int m_minX; /// Minimal allowed X coordinate?
      bool m_canDash; /// Can the player use the dash attack?
      bool m_dashing; /// Is the player dashing right now?
      int m_dashStart; /// Where did the dash start?
      int m_power; /// How much powe does the player have?
   };
}

#endif // PLAYER_H
