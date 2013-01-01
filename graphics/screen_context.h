/// @file graphics/screen_context.h
/// @author grigoryj

#ifndef SCREEN_CONTEXT_H
#define SCREEN_CONTEXT_H

#include "graphics/context.h"
#include <SDL/SDL.h>

namespace graphics
{
   /// Drawing context for the main screen
   class ScreenContext : public Context
   {
   public:
      /// Sets the screen as the current drawing context
      void activate();

      /// @return screen width
      int width() const;

      /// @return screen height
      int height() const;

   private:
      /// Checks if SDL has a valid video surface for the screen
      /// @throw runtime_error if not
      SDL_Surface* videoSurface() const;
   };
}

#endif //SCREEN_CONTEXT_H
