/// @file game/engine.cpp
/// @author grigoryj

#include "game/engine.h"
#include "game/scenes/titlecard_scene.h"
#include "game/scenes/help_scene.h"
#include "game/scenes/play_scene.h"
#include "game/scenes/end_scene.h"
#include "game/debug_console.h"
#include <fstream>
#include <iostream>

namespace game
{
   void Engine::run(int, char* [], const std::string& window_title)
   {
      try
      {
         // Initialize subsystems
         initDebugConsole();
         m_graph.reset(new graphics::Graphics());
         m_graph -> init(1024, 768, false);
         SDL_WM_SetCaption(window_title.c_str(), window_title.c_str());

         m_resMan.reset(new util::ResourceManager());
         
         // Create the scenes
         m_scenes["splash"].reset(new TitleCardScene(*this));
         m_scenes["help"].reset(new HelpScene(*this));
         m_scenes["play"].reset(new PlayScene(*this));
         m_scenes["end"].reset(new EndScene(*this));

         // Set current active screen to titlecard
         m_currentScene = m_scenes.find("splash");
         m_currentScene -> second -> focus(*this);

         // Main loop
         bool exit = false;
         SDL_Event e;
         while(!exit)
         {
            // Process events from the SDL event queue
            while(SDL_PollEvent(&e))
            {
               switch(e.type)
               {
               case SDL_KEYDOWN:
                  m_currentScene -> second -> keyPressed(*this, e.key.keysym.sym);
                  break;

               case SDL_KEYUP:
                  m_currentScene -> second -> keyReleased(*this, e.key.keysym.sym);
                  break;

               case SDL_MOUSEBUTTONDOWN:
                  m_currentScene -> second -> mousePressed(*this, e.button.x, e.button.y, e.button.button);
                  break;

               case SDL_MOUSEBUTTONUP:
                  m_currentScene -> second -> mousePressed(*this, e.button.x, e.button.y, e.button.button);
                  break;

               case SDL_QUIT:
                  exit = true;
                  break;
               }            
            }

            m_currentScene -> second -> update(*this); // Update gamestate
            m_currentScene -> second -> draw(*this);   // Render frame
            m_graph -> flip();
         }
      }
      catch(const std::exception& e)
      {
         std::ofstream error_file("error.txt");
         if(error_file.is_open())
         {
            error_file << e.what() << std::endl;
            error_file.close();
         }
         std::cerr << e.what() << std::endl;
      }
   }

   void Engine::setScene(const std::string& name)
   {
      SceneMap::iterator i = m_scenes.find(name);
      if(i != m_scenes.end())
      {
         m_currentScene -> second -> focus(*this, false);
         m_currentScene = i;
         m_currentScene -> second -> focus(*this);
      }
      else
      {
         throw std::runtime_error(("Scene not found: " + name).c_str());
      }
   }

   util::Clock& Engine::clock()
   {
      return m_clock;
   }

   graphics::Graphics& Engine::graphics() 
   {
      return *m_graph;
   }

   const graphics::Graphics& Engine::graphics() const 
   { 
      return *m_graph; 
   }

   const util::ResourceManager& Engine::resourceManager() const
   {
      return *m_resMan;
   }
}
