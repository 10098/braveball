#include "game/engine.h"
#include "game/objects/player.h"
#include "util/color_scale.h"

namespace game
{
   Player::Player(const Engine& e, int x, int y):
      GameObject(x, y),
      m_landSound(e.resourceManager().getSound("land.wav"), false),
      m_jumpSound(e.resourceManager().getSound("jump.wav"), false),
      m_hurtSound(e.resourceManager().getSound("hurt.wav"), false),
      m_heartSound(e.resourceManager().getSound("heart.wav"), false),
      m_heartPtsSound(e.resourceManager().getSound("heart_nopickup.wav"), false),
      m_dashSound(e.resourceManager().getSound("dash.wav"), false),
      m_speed(1),
      m_direction(1),
      m_lives(4),
      m_hurt(false),
      m_reachedExit(false),
      m_hurtTime(-1),
      m_fallSpeed(0),
      m_accelerationThr(15),
      m_posUpdateThr(15),
      m_jumpState(Standing),
      m_moveForward(false),
      m_breakJump(false),
      m_currentPlatformStart(0),
      m_currentPlatformEnd(9000),
      m_minX(0),
      m_dashing(false),
      m_dashStart(0),
      m_power(0),
      m_canDash(false)
   {
      addTag("player");
      const char* anim_names[] = {"run_right", "run_left", "jump_right", "jump_left", "wait_right", "wait_left", "land_right", "land_left", "fall_right", "fall_left"};
      for(int i = 0; i < sizeof(anim_names)/sizeof(char*); ++i)
      {
         m_anims.addAnim(anim_names[i], graphics::Animation(e.resourceManager().getAnimationData(std::string("ball/") + anim_names[i])));
      }
      m_anims.setCurrentAnim("wait_right");
      setWidth(m_anims.currentAnim().frame().w);
      setHeight(m_anims.currentAnim().frame().h);
   }

   void Player::update(Engine& ctx)
   {
      // Animate the sprite
      m_anims.currentAnim().update(ctx.clock());

      // Update jumping state, acceleration and on-screen position
      updatePostHitInvincibility(ctx);
      updateJumpState(ctx);
      updatePosition(ctx);
   }

   void Player::updateJumpState(Engine& ctx)
   {
      const int m_minYDiff = 8; // Minimal jump height
      const int m_maxYDiff = 64; // Max jump height

      // Throttle acceleration
      if(m_accelerationThr(ctx.clock().ticks()))
      {
         if(m_jumpState == Jumping && 
            m_anims.currentAnim().currentFrameNumber() > 2 &&  // The first couple of frames is the ball pushing itself from the ground, so no upward acceleration during those
            m_fallSpeed > -3) // don't accelerate past 3 px
         {
            m_fallSpeed -= 1;
         }
         else if(m_jumpState == Falling && m_fallSpeed < 3)
         {
            m_fallSpeed += 1;         
         }
      }

      // Break the jump if necessary (spacebar released or peak altitude reached),
      // but make sure the player travels a minimal amount of pixels up into the 
      // air before that. 
      int ydiff = m_beforeJumpY - y();
      if(m_jumpState == Jumping && (m_breakJump &&  ydiff > m_minYDiff || ydiff > m_maxYDiff)) 
      {
         m_anims.setCurrentAnim(m_direction == 1 ? "fall_right" : "fall_left");
         m_jumpState = Falling;
         m_jumpSound.stop();
      }
      
      // Fall off the platform after crossing the edge
      if(m_jumpState != Jumping && (x() > m_currentPlatformEnd || x() < m_currentPlatformStart - m_anims.currentAnim().frame().w))
      {
         m_anims.setCurrentAnim(m_direction == 1 ? "fall_right" : "fall_left");
         m_jumpState = Falling;
         m_jumpSound.stop();
      }
   }

   void Player::updatePostHitInvincibility(Engine& ctx)
   {
      if(m_hurt)
      {
         if(m_hurtTime == -1) // Turn the `timer' for post-hit invincibility on, if necessary
         {
            m_hurtTime = ctx.clock().ticks();
         }
         else if(ctx.clock().ticks() - m_hurtTime > 250) // Disable post-hit invincibility at least after 250 ms
         {
            m_hurtTime = -1;
            m_hurt = false;
         }
      }
   }
   
   void Player::updatePosition(Engine& ctx)
   {
      if(m_posUpdateThr(ctx.clock().ticks()))
      {
         setY(y() + m_fallSpeed); // Set vertical position

         // If the player is hurt...
         if(m_hurt) 
         {
            // ...apply knockback.
            int speed_factor = m_kickBackDirection == 1 ? 4 : 2; // Make kick-back a little stronger when going right, to move the player farther from the enemy.
            setX(x() + m_kickBackDirection * m_speed * speed_factor);
         }

         // Otherwise...
         else if((m_dashing || m_moveForward) && // ...if we're moving forward or dashing
                 (m_direction == -1 && x() > m_minX || m_direction != -1))  // and we're remaining within limits 
         {
            // Advance the player forward
            setX(x() + (m_direction) * m_speed * (m_dashing ? 4 : 1));
            if(m_dashing && abs(x() - m_dashStart) > 100) stopDash();
         }
      }
   }

   void Player::draw(graphics::SpriteBatch& b)
   {
      static util::ColorScale dash_alpha_scale(graphics::Color(0xff, 0xff, 0xff, 0x10), graphics::Color(0xff, 0xff, 0xff, 0x00));

      if(!m_hurt)
      {
         // draw a semitransparent track if the player is dashing
         if(m_dashing)
         {
            int d = abs(m_dashStart - x()); // Dash distance
            if(d > 32) d = 32; // limit it to 32 px
            int dash_direction = m_dashStart > x() ? 1 : -1; 
            for(int i = 0; i < d; i++)
               b.add(x() + dash_direction*i, y(), m_anims.currentAnim().frame(), dash_alpha_scale.getColor(0, d, i));         
         }
         b.add(x(), y(), m_anims.currentAnim().frame());
      }
      else // If the ball has post-hit invincibility, draw it semi-transparent
      {
         b.add(x(), y(), m_anims.currentAnim().frame(), graphics::Color(0xff, 0xff, 0xff, 0x60));
      }
   }

   int Player::lives() const { return m_lives; }

   int Player::power() const { return m_power; }
   
   int Player::dashLength() const { return m_dashing ? abs(m_dashStart - x())/2 : 0; }
   
   bool Player::canDash() const { return m_canDash; }
   
   bool Player::reachedExit() const { return m_reachedExit; }
   
   void Player::setMinX(int min_x){ m_minX = min_x; }

   void Player::collide(GameObject& o)
   {
      if(o.hasTag("platform"))
         collidePlatform(o);
      else if(o.hasTag("youngling"))
         collideYoungling(o);
      else if(o.hasTag("heart"))
         collideHeart(o);
      else if(o.hasTag("meany"))
         collideMeany(o);
      else if(o.hasTag("exit_portal"))
         collideExitPortal(o);
      else if(o.hasTag("king"))
         collideKing(o);
   }

   void Player::collidePlatform(GameObject& p)
   {
      // "Latch" onto a platform if we land onto it  
      if(y() + m_anims.currentAnim().frame().h - p.y() <= 4 && m_jumpState != Jumping)
      {
         setY(p.y() - height());
         if(m_jumpState == Falling)
         {
            m_anims.setCurrentAnim(m_direction == 1 ? "land_right" : "land_left");
            m_jumpState = Landing;
            m_landSound.play();
         }
         else if(m_jumpState == Landing && m_anims.currentAnim().currentFrameNumber() == m_anims.currentAnim().frameCount() - 1)
         {
            m_anims.setCurrentAnim(m_direction == 1 ? (m_moveForward ? "run_right" : "wait_right") : (m_moveForward ? "run_left" : "wait_left"));
            m_jumpState = Standing;
         }
         m_fallSpeed = 0;
         m_currentPlatformStart = p.x();
         m_currentPlatformEnd = p.x() + p.width();
      }
   }

   void Player::collideMeany(GameObject& o)
   { 
      // Count the hit if the player doesn't have post-hit invincibility or dash
      if(!m_dashing && !m_hurt && m_lives > 0)
      {
         m_lives--; 
         m_hurt = true;
         m_kickBackDirection = o.x() <= x() ? 1 : -1;
         m_hurtSound.play();
      }
   }

   void Player::collideHeart(GameObject& o)
   {
      o.removeTag("heart");
      if(m_lives < 4)
      {
         m_lives++;
         m_heartSound.play();
      }
      else
      {
         addPower(1);
         m_heartPtsSound.play();
      }
   }

   void Player::collideYoungling(GameObject& o)
   {
      o.removeTag("youngling");
      addPower(5);
   }

   void Player::collideExitPortal(GameObject&)
   {
      m_reachedExit = true;
   }

   void Player::collideKing(GameObject&)
   {
      m_lives = 0;
   }

   void Player::dash()
   {
      if(m_canDash)
      {
         addTag("player_dash");
         m_dashing = true;
         m_dashStart = x();
         m_dashSound.play(true);
         m_anims.setCurrentAnim(m_direction == 1 ? "run_right" : "run_left");
      }
   }

   void Player::stopDash()
   {
      if(m_dashing)
      {
         m_power -= dashLength(); 
         if(m_power <= 0) 
         {
            m_power = 0;
            m_canDash = false;
         }
         m_dashing = false;
         removeTag("player_dash");
         m_dashSound.stop();

         // Stop if we weren't dashing while running
         if(!m_moveForward && m_jumpState == Standing)
             m_anims.setCurrentAnim(m_direction == 1 ? "wait_right" : "wait_left");
      }
   }

   void Player::jump()
   {
      if(m_jumpState == Standing || m_jumpState == Landing)
      {
         // initiate the jump
         m_beforeJumpY = y();
         m_breakJump = false;
         m_anims.setCurrentAnim(m_direction == 1 ? "jump_right" : "jump_left");
         m_jumpState = Jumping;
         m_jumpSound.play(true);
      }
   }

   void Player::stopJump()
   {
      // Jump will be stopped on next update
      if(m_jumpState == Jumping)
      {
         m_breakJump = true; // will be taken care of in updateJumpState
      }
   }

   void Player::walkRight()
   {
      if(m_jumpState == Standing) m_anims.setCurrentAnim("run_right");
      m_moveForward = true;
      m_direction = 1;
   }

   void Player::walkLeft()
   {
      if(m_jumpState == Standing) m_anims.setCurrentAnim("run_left");
      m_moveForward = true;
      m_direction = -1;
   }

   void Player::stopWalkRight()
   {
      if(m_direction == 1)
      {
         if(m_jumpState == Standing) m_anims.setCurrentAnim("wait_right");
         m_moveForward = false;
      }
   }

   void Player::stopWalkLeft()
   {
      if(m_direction == -1)
      {
         if(m_jumpState == Standing) m_anims.setCurrentAnim("wait_left");
         m_moveForward = false;
      }
   }

   void Player::addPower(int amount)
   {
      if(!m_canDash) // only add power when the power meter is green
      {
         if(m_power < MaxPowerLevel) // don't add past max power level
         {
            m_power += amount;
         }
         if(m_power >= MaxPowerLevel) 
         {
            m_power = MaxPowerLevel;
            m_canDash = true;
         }
      }
   }
}
