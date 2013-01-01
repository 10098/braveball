/// @file game/scenes/play_scene.h
/// @author grigoryj

#ifndef PLAY_SCENE_H
#define PLAY_SCENE_H

#include "game/scene.h"
#include "util/color_scale.h"
#include "game/scenes/play_scene_fwd.h"
#include <list>

namespace game
{

   /// Main scene
   /// Most of the game logic happens here
   class PlayScene : public Scene
   {
   public:
      explicit PlayScene(Engine& eng);

   public:
      /// Called when the scene comes into focus
      virtual void focus(Engine& ctx, bool enter = true);

      /// Called every frame to update the state of the objects
      virtual void update(Engine& ctx);

      virtual void mousePressed(Engine&, int, int, MouseButton) {}
      virtual void mouseReleased(Engine&, int, int, MouseButton) {}

      /// Handles key presses
      virtual void keyPressed(Engine& ctx, Key key);

      /// Called when a key is released
      virtual void keyReleased(Engine& ctx, Key key);

      /// Renders the gamestate
      virtual void draw(Engine& ctx);

   private:
      /// Draws a gradient background.
      /// The background changes colors depending on how close
      /// the Threat is to the player.
      void drawBackground(Engine&);

      /// Triggers the "game over" sequence
      void die(const std::string& cause, bool play_sound = false);

      /// Resets the scene and transitions the status to `game over'.
      void endGame(Engine&);

      /// Reset the scene, returning all the objects to their initial state.
      /// Called when starting a new game.
      void reset(Engine&);

      /// Updates the camera position. Also locks the camera on the Threat,
      /// if it gets too close to the player.
      void updateCamera(Engine& ctx);

      /// Updates the platforms list and all the objects on the platforms
      void updatePlatforms(Engine& ctx);

      /// Detects collisions
      void detectCollisions(Engine& ctx);

      /// Checks if any unsaved, off-screen younglings collide
      /// with the Youngling Eater (if yes, ends the game).
      bool eatOffscreenYounglings(const std::list<std::shared_ptr<Platform> >::iterator& j);

   private:
      /// Game status codes
      enum Status
      {
         Playing,
         Paused,
         GameEnding,
         GameOver
      };

      // Images
      const graphics::Image& m_atlas;     /// The texture atlas
      const graphics::Image& m_mountains; /// The texture atlas
      const graphics::Font& m_font; 
      std::shared_ptr<graphics::Image> m_gameOverScreen; /// An image for the "game over" screen
      std::shared_ptr<graphics::Image> m_pausedScreen;   /// An image for the "paused" screen
      std::shared_ptr<graphics::Image> m_pressXToDash;   /// "Press X to Dash!" label

      // Drawing stuff
      std::shared_ptr<graphics::SpriteBatch> m_batch;  /// Sprite batch for faster drawing
      std::shared_ptr<graphics::RetroScreen> m_screen; /// Canvas with a "retro screen" effect

      // Objects
      std::shared_ptr<Player> m_player;                  /// The player object
      std::shared_ptr<Threat> m_threat;                  /// The Unseen Threat
      std::shared_ptr<YounglingEater> m_eater;           /// An invisible object that triggers game over upon collision with younglings
      std::list<std::shared_ptr<Platform> > m_platforms; /// List of platforms. All the enemies and younglings are children of platforms.
      std::shared_ptr<HUD> m_hud;                        /// Heads-up display
      std::shared_ptr<PlatformGenerator> m_platformGen;  /// Encapsulates the algorithm for platform generation
      std::shared_ptr<util::Camera> m_cam;               /// Camera that follows the player around

      // Sounds
      std::shared_ptr<sound::Sound> m_fallSound; /// Sound of the player falling down
      std::shared_ptr<sound::Sound> m_music;     /// Background music
      std::shared_ptr<sound::Sound> m_gameOverSound; /// Game over sound


      // Other stuff
      Status m_status;                          /// Game status (playing, paused, game over?)
      std::shared_ptr<graphics::Image> m_deathCause;

      util::ColorScale m_upperScreenColorScale; /// Color scale for the upper section of the background
      util::ColorScale m_midScreenColorScale;   /// Color scale for the middle section of the background

      /// Don't generate assignment operator, mkay?
      PlayScene& operator=(const PlayScene&);
   };
}

#endif // PLAY_SCENE_H
