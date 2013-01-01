/// @file graphics/screen_context.cpp
/// @author grigoryj

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/glew.h>
#include "graphics/screen_context.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <stdexcept>

namespace graphics
{
   void ScreenContext::activate()
   {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      
      unsigned w = width();
      unsigned h = height();
      
      glViewport(0, 0, w, h);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0, w, h, 0, -1, 1);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
   }
   
   int ScreenContext::width() const
   {
      return videoSurface() -> w;
   }

   int ScreenContext::height() const
   {
      return videoSurface() -> h;
   }

   SDL_Surface* ScreenContext::videoSurface() const
   {
      SDL_Surface* s = SDL_GetVideoSurface();
      if(NULL == s)
         throw std::runtime_error("Invalid screen context");
      return s;
   }
}
