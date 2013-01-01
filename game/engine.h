/// @file game/engine.h
/// @author grigoryj

#ifndef ENGINE_H
#define ENGINE_H

#include "game/scene.h"
#include "graphics/graphics.h"
#include "graphics/font.h"
#include "sound/sound_system.h"
#include "util/clock.h"
#include "util/resource_manager.h"

#include <unordered_map>
#include <memory>

namespace game
{
   /// Maintains the game scenes and various engine subsystems.
   class Engine
   {
   public:
      /// Sets the active scene
      /// @param name The name of the scene to set as active
      /// @throw runtime_error if the specified scene does not exist
      void setScene(const std::string& name);

      /// @return the clock object
      util::Clock& clock();

      /// @return the graphics object
      graphics::Graphics& graphics();

      /// @return const version for graphics()
      const graphics::Graphics& graphics() const;

      /// @return the resource manager
      const util::ResourceManager& resourceManager() const;

      /// Performs all the necessary initializations and launches the game from the first scene.
      /// Returns when the player exits the game.
      void run(int argc, char* argv[], const std::string& window_title = "tractorengine");

   private:
      typedef std::unordered_map<std::string, std::unique_ptr<Scene> > SceneMap;

      SceneMap m_scenes;                               /// Maps scene names to pointers to scenes
      SceneMap::iterator m_currentScene;               /// Points to the active scene
      std::shared_ptr<graphics::Graphics> m_graph;     /// Graphics subsystem
      util::Clock m_clock;                             /// Clock
      std::shared_ptr<util::ResourceManager> m_resMan; /// Resource manager
      sound::SoundSystem m_sound;                      /// Sound subsystem
   };
}

#endif // ENGINE_H
