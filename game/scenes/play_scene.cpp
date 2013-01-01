/// @file game/scenes/play_scene.cpp
/// @author grigoryj

#include "game/scenes/play_scene.h"
#include "game/hud.h"
#include "game/objects/player.h"
#include "game/objects/threat.h"
#include "game/objects/youngling_eater.h"
#include "game/objects/youngling.h"
#include "game/objects/mean_walker.h"
#include "game/objects/heart.h"
#include "game/objects/platform.h"
#include "game/platform_generator.h"
#include "game/engine.h"
#include "sound/sound.h"
#include "graphics/image.h"
#include "graphics/sprite_batch.h"
#include "graphics/retro_screen.h"
#include "util/camera.h"
#include <iostream>

namespace game
{
   PlayScene::PlayScene(Engine& eng):
      m_atlas(eng.resourceManager().getImage("atlas")),
      m_mountains(eng.resourceManager().getImage("mountains")),
      m_font(eng.resourceManager().getFont("terminus.fon/8")),
      m_gameOverScreen(m_font.renderString("PRESS ENTER TO TRY AGAIN!")),
      m_pausedScreen(m_font.renderString("QUIT GAME? (Y/N)")),
      m_pressXToDash(m_font.renderString("PRESS X TO DASH!")),
      m_batch(new graphics::SpriteBatch(m_atlas)),
      m_screen(new graphics::RetroScreen(eng.graphics(), eng.graphics().screenContext(), 2, true)),
      m_cam(new util::Camera()),
      m_status(Playing),
      m_fallSound(new sound::Sound(eng.resourceManager().getSound("fall.wav"), false)),
      m_music(new sound::Sound(eng.resourceManager().getSound("music.mp3"), true, false)),
      m_gameOverSound(new sound::Sound(eng.resourceManager().getSound("game_over.wav"), false)),
      m_upperScreenColorScale(graphics::Color(0x1f, 0x1f, 0x1f), graphics::Color(0x39, 0xb7, 0xfe)),
      m_midScreenColorScale(graphics::Color(0xff, 0x44, 0x44), graphics::Color(0xff, 0xff, 0xff))
   {
      reset(eng); // Prepare for first use
   }

   void PlayScene::reset(Engine& ctx)
   {
      // Reset all the game objects.
      m_player.reset(new Player(ctx, 0, m_screen -> height()/2 - 10));
      m_threat.reset(new Threat(ctx, -512, 0));
      m_eater.reset(new YounglingEater(0,0));
      m_platforms.clear();
      m_platformGen.reset(new PlatformGenerator(10, 15, 16, 55, 16, 30, m_screen -> height()/2, 5120));

      // Reset the HUD, tying it with the new player object
      m_hud.reset(new HUD(ctx, m_player.get()));

      // Lock the camera on the new Player object
      m_cam -> followObject(m_player.get(),  0, m_screen -> width() / 4, m_screen -> width()/8, m_screen -> height()/2 - 10);

      // Reset the transformation matrix.
      ctx.graphics().resetMatrix();
   }

   void PlayScene::focus(Engine& ctx, bool enter)
   {
      if(enter) // If we are entering this scene
      {
         // Bind the texture atlas, activate the "retro screen" 
         // canvas and play our happy chiptune.
         m_screen -> activate();
         m_music -> play(true);
      }
      else // If we're leaving the scene
      {
         ctx.graphics().screenContext().activate();

         // Stop the music.
         m_music -> stop();
      }
   }
   
   void PlayScene::keyPressed(Engine& ctx, Key key)
   {
      // If we're currently playing the game
      if(m_status == Playing)
      {
         if(key == SDLK_SPACE) // Pressing space triggers a jump
         {
            m_player -> jump();
         }
         else if(key == SDLK_RIGHT) // Pressing right arrow makes the character walk right (or steer right, if it's in a jump)
         {
            m_player -> walkRight();
         }
         else if(key == SDLK_LEFT) // Similar for left arrow
         {
            m_player -> walkLeft();
         }
         else if(key == SDLK_ESCAPE) // Pressing Escape pauses the game
         {
            m_status = Paused;
         }
         else if(key == SDLK_x)
         {
            m_player -> dash();
         }
         else if(key == SDLK_m)
         {
            if(m_music -> isPlaying())
               m_music -> stop(); // Mute
            else
               m_music -> play(true);
         }
      }

      // If the game is paused
      else if(m_status == Paused)
      {
         // The message on screen is "do you want to quit the game?"

         if(key == SDLK_y) // Quit to main menu if the player presses 'Y'
         {
            reset(ctx);
            ctx.setScene("splash");
            m_status = Playing;
         }
         else if(key == SDLK_n) // Resume game if the player presses 'N'
         {
            m_status = Playing;
         }
      }

      // If the game is over
      else if(m_status == GameOver)
      {
         if(key == SDLK_RETURN)
            m_status = Playing;
      }
   }

   void PlayScene::keyReleased(Engine&, Key key)
   {
      // If we're currently playing the game
      if(m_status == Playing)
      {
         if(key == SDLK_SPACE) // Releasing the jump button stops the jump
         {
            m_player -> stopJump();
         }
         else if(key == SDLK_RIGHT) // If the right arrow key is released...
         {
            m_player -> stopWalkRight(); //...stop walking right, but...

            //...if the left arrow key is still pressed, start walking left.
            // this gives the controls a much smoother feel.
            if(SDL_GetKeyState(NULL)[SDLK_LEFT])
               m_player -> walkLeft();
         }
         else if(key == SDLK_x)
         {
            m_player -> stopDash();
         }
         else if(key == SDLK_LEFT) // Similar for left arrow key
         {
            m_player -> stopWalkLeft();
            if(SDL_GetKeyState(NULL)[SDLK_RIGHT])
               m_player -> walkRight();
         }
      }
   }

   void PlayScene::die(const std::string& cause, bool play_sound)
   {
      m_deathCause = m_font.renderString(cause);
      m_status = GameEnding;
      if(play_sound)
         m_gameOverSound -> play();
   }

   void PlayScene::endGame(Engine& ctx)
   {
      // Reset game objects and set status code to GameOver
      reset(ctx);
      m_status = GameOver;
   }

   void PlayScene::update(Engine& ctx)
   {
      if(m_status == Playing)
      {
         if(m_player -> reachedExit())
         {
            endGame(ctx);
            m_status = Playing;
            ctx.setScene("end");
            return;
         }

         m_player -> update(ctx);

         if(m_player -> x() - m_threat -> x() > 512)
         {
            m_threat -> setX(m_player -> x() - 512);
         }
         m_threat -> update(ctx);
         m_threat->setY(m_player->y()); // Just to make sure it can always hit the player
         
         updateCamera(ctx);   
         m_player->setMinX(m_cam -> x()); // Player can't walk past the left edge of the screen

         // The youngling eater follows the camera. If any younglings go off-screen, they'll get omnomnommed.
         m_eater->setX(m_cam -> x());
         m_eater->setY(m_cam -> y());

         // Update objects on platforms
         updatePlatforms(ctx);
         
         // Do collision detection
         detectCollisions(ctx);

         // Game over if the player has lost all the lifes
         if(m_player -> lives() <= 0)
            die("THE MEANIES GOT YOU!", true);
      }
      if(m_status == GameEnding && !m_gameOverSound -> isPlaying())
      {
         endGame(ctx);
      }
   } 

   void PlayScene::updateCamera(Engine& ctx)
   {
      // Drawing contents of m_screen to the actual screen causes
      // transformation matrix reset, so we need to preserve these
      // transformations...
      ctx.graphics().popMatrix();
      m_cam -> update(ctx);
      ctx.graphics().pushMatrix();
   }

   void PlayScene::updatePlatforms(Engine& ctx)
   {
      // Generate new platforms as necessary 
      if(m_platforms.empty() || m_platforms.back() -> x() - m_cam -> x() <= m_screen -> width())
         m_platformGen -> generatePlatforms(m_platforms, ctx.graphics().screenContext().width()*2, ctx);

      // Erase platforms that are beyond the left edge of the screen (there'll be just one)
      if(!m_platforms.empty()) { // Only try to do this if there are some platforms
         const std::shared_ptr<Platform>& p = m_platforms.front(); // This is the one platform that *may* be beyond the left edge
         if(p -> x() + p -> width() < m_cam -> x()) // If it's completely off screen, remove it
            m_platforms.pop_front();
      }

      // Update objects on visible platforms
      auto end = m_platforms.end();
      int left_edge =  m_cam -> x() + m_screen -> width(); // X coordinate of the screen's left edge
      for(auto i = m_platforms.begin(); i !=end && (*i) -> x() < left_edge; ++i) 
         (*i) -> update(ctx);
   }

   void PlayScene::detectCollisions(Engine&)
   {
      // Test for collision with the Threat
      if(m_player -> boundingBox().collidesWith(m_threat -> boundingBox()))
         m_threat -> collideWith(*m_player);

      // Find the platform that the player is on ("current platform")
      auto current_platform = m_platforms.begin();
      for(int i = 0; i < m_platforms.size() - 1 && (*current_platform) -> x() + (*current_platform) -> width() < m_player -> x(); ++i,++current_platform);

      // By now, current_platform should be one of the following (unless it's m_platforms.end()):
      // 1. the platform that the player is standing on
      // 2. the platform that the player was standing on before making a jump a little while ago
      // 3. the platform that the playes is soon going to land on
      if(current_platform != m_platforms.end()) 
      {
         // Test for collision with the platform itself
         if(m_player -> boundingBox().collidesWith((*current_platform)->boundingBox()))
            (*current_platform) -> collideWith(*m_player);

         // Test for collisions with objects on the current platform
         auto end = (*current_platform) -> objectsEnd();
         for(auto i = (*current_platform) -> objectsBegin(); i != end; ++i)
         {
            if(m_player -> boundingBox().collidesWith((*i)->boundingBox()))
            {
               (*i) -> collideWith(*m_player);
            }
         }

         // Take care of the offscreen younglings
         if(eatOffscreenYounglings(current_platform))
         {
            die("YOU HAVE LEFT SOMEONE BEHIND!");
            return;
         }

         // Game over if the player falls too far down, play the 
         // falling sound a bit earlier than actually ending the game.
         if(m_player -> y() > (*current_platform) -> y() + (*current_platform) -> height() + 64)
            m_fallSound -> play();
         if(m_player -> y() > (*current_platform) -> y() + (*current_platform) -> height() + 200)
         {
            die("YOU FELL INTO THE ABYSS!");
            return;
         }
      }
   }

   bool PlayScene::eatOffscreenYounglings(const std::list<std::shared_ptr<Platform> >::iterator& current_platform)
   {
      std::list<std::shared_ptr<Platform> >::iterator tmp = current_platform; 
      auto end = ++tmp; // needed to include the current platform into search as well

      // Search all platforms up to and including the current platform for younglings that got off-screen
      for(auto p = m_platforms.begin(); p != end; ++p)
      {
         for(auto i = (*p) -> objectsBegin(); i != (*p) -> objectsEnd(); ++i)
         {
            if(m_eater -> boundingBox().collidesWith((*i)->boundingBox()))
            {
               (*i) -> collideWith(*m_eater);
               if((*i) -> hasTag("dead_youngling")) return true;
            }
         }
      }

      return false;
   }

   void PlayScene::draw(Engine& ctx)
   {
      // Wipe the screen
      if(m_status == Playing) // If we're playing
      {  
         ctx.graphics().clear();
         drawBackground(ctx); // Draw the gradient background
         // Draw all the objects on all of the visible platforms
         m_batch -> begin();
         for(auto i = m_platforms.begin(); i != m_platforms.end() && (*i) -> x() < m_cam -> x() + m_screen -> width(); ++i)
         {
            (*i) -> draw(*m_batch);
         }
         
         // Draw the player
         m_player -> draw(*m_batch);

         // Draw the King Meany
         m_threat -> draw(*m_batch);

         // Draw the heads-up display
         m_hud -> draw(m_cam -> x() + 16, m_cam -> y() + 16, *m_batch);

         // For debug purposes
         m_eater -> draw(*m_batch);

         m_batch -> end();

         ctx.graphics().pushMatrix();
         ctx.graphics().resetMatrix();
         if(m_player -> canDash())
         {
            m_pressXToDash -> draw(m_screen -> width() / 2 - m_pressXToDash -> width() / 2, 64);
         }

         // Draw distance to exit portal
         long long distance_to_dest = m_platformGen -> edgeCoord() - m_player -> x();
         std::shared_ptr<graphics::Image> i =  m_font.renderString(distance_to_dest > 0 ? std::to_string(distance_to_dest) + "M TO RUN" : "ALMOST THERE!");
         i -> draw(0, m_screen -> height() - i -> height() - 2);

         ctx.graphics().popMatrix();
      }
      else if(m_status == Paused)
      {
         ctx.graphics().clear();
         ctx.graphics().resetMatrix();
         m_pausedScreen -> draw(m_screen -> width() / 2 - m_pausedScreen -> width() / 2, m_screen -> height() / 2 - m_pausedScreen -> height() / 2);
      }
      else if(m_status == GameOver || m_status == GameEnding)
      {
         ctx.graphics().resetMatrix();
         ctx.graphics().drawRect(m_screen -> width() / 2 - m_deathCause  -> width() / 2 - 1, 63, m_deathCause -> width() + 2, m_deathCause -> height() + 2);
         ctx.graphics().drawRect(m_screen -> width() / 2 - m_gameOverScreen  -> width() / 2 - 1, 127, m_gameOverScreen -> width() + 2, m_gameOverScreen -> height() + 2);
         m_deathCause  -> draw(m_screen -> width() / 2 - m_deathCause  -> width() / 2, 64);         
         m_gameOverScreen -> draw(m_screen -> width() / 2 - m_gameOverScreen -> width() / 2, 128);
      }

      // Slap the canvas onto the screen
      m_screen -> draw();

      // Reactivate the canvas!
      m_screen -> activate();
   }

   void PlayScene::drawBackground(Engine& ctx)
   {
      graphics::Color upper_color = m_upperScreenColorScale.getColor(0, 300, m_player->x()-m_threat->x());
      graphics::Color middle_color = m_midScreenColorScale.getColor(0, 300, m_player->x()-m_threat->x());
      graphics::Color bottom_color(middle_color.r - 0x0f, middle_color.g - 0x0f, middle_color.b - 0x0f);

      ctx.graphics().resetBoundImage();
      ctx.graphics().pushMatrix();
      ctx.graphics().resetMatrix();

      ctx.graphics().drawGradient(upper_color, middle_color,  0, 0, m_screen -> width(), m_screen -> height()/2);
      ctx.graphics().drawGradient(middle_color, bottom_color, 0, m_screen -> height()/2, m_screen -> width(), m_screen -> height()/2);

      ctx.graphics().setColor(0xff, 0xff, 0xff);

      m_mountains.draw(0, m_screen->height()/2 - m_mountains.height(), graphics::AlignedQuad((m_cam -> x() / 3) % m_mountains.width(), 0, m_screen->width(), m_mountains.height()));
      ctx.graphics().resetBoundImage();
      ctx.graphics().drawGradient(graphics::Color(0xff, 0xff, 0xff, 0x00), middle_color, 0, 0, m_screen -> width(), m_screen -> height()/2);
      ctx.graphics().setColor(0xff, 0xff, 0xff);
      ctx.graphics().popMatrix();
   }
}
