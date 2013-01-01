/// @file game/scene.h
/// @author grigoryj

#ifndef SCENE_H
#define SCENE_H

#include <SDL/SDL.h>

namespace game
{
   class Engine;
   typedef int MouseButton;
   typedef SDLKey Key;

   class Scene
   {
   public:
      virtual void focus(Engine& ctx, bool enter = true) = 0;
      virtual void update(Engine& ctx) = 0;
      virtual void mousePressed(Engine& ctx, int x, int y, MouseButton btn) = 0;
      virtual void mouseReleased(Engine& ctx, int x, int y, MouseButton btn) = 0;
      virtual void keyPressed(Engine& ctx, Key key) = 0;
      virtual void keyReleased(Engine& ctx, Key key) = 0;
      virtual void draw(Engine& ctx) = 0;
   };
}

#endif // SCENE_H
