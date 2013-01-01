/// @file graphics.cpp
/// @author grigoryj

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/glew.h>
#include "graphics/graphics.h"
#include <SDL/SDL.h>
#include <SDL_ttf.h>
#include <GL/gl.h>
#include <IL/il.h>
#include <stdexcept>

namespace graphics
{
   Graphics::~Graphics()
   {
      TTF_Quit();
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
      ilShutDown();
   }

   void Graphics::init(unsigned width, unsigned height, bool fullscreen)
   {
      // Prepare the flags
      Uint32 flags = SDL_OPENGL;
      if(fullscreen) flags |= SDL_FULLSCREEN;
      
      // Initialize SDL's video subsystem
      if(SDL_Init(SDL_INIT_VIDEO) == -1)
      {
         throw std::runtime_error(SDL_GetError());
      }

      // Attempt to set the video mode
      SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
      SDL_Surface* s = SDL_SetVideoMode(width, height, 0, flags);
      if(NULL == s)
      {
         throw std::runtime_error(SDL_GetError());
      }

      // Initialize SDL_TTF
      if(TTF_Init() == -1)
      {
         throw std::runtime_error(TTF_GetError());
      }

      // Now set up OpenGL
      glewInit();
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_DEPTH_TEST);
      m_ctx.activate();

      // Set up DevIL
      ilInit();
   }

   Context& Graphics::screenContext()
   {
      return m_ctx;
   }

   const Context& Graphics::screenContext() const
   {
      return m_ctx;
   }

   void Graphics::pushMatrix()
   {
      glPushMatrix();
   }

   void Graphics::popMatrix()
   {
      glPopMatrix();
   }

   void Graphics::resetMatrix()
   {
      glLoadIdentity();
   }

   void Graphics::translate(int x, int y)
   {
      glTranslated(x, y, 0);
   }

   void Graphics::rotate(float degrees)
   {
      glRotatef(degrees, 0, 0, 1);
   }

   void Graphics::setColor(const Color& c)
   {
      setColor(c.r, c.g, c.b, c.a);
   }

   void Graphics::setClearColor(const Color& c)
   {
      setClearColor(c.r, c.g, c.b, c.a);
   }

   void Graphics::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
   {
      glColor4f(r / 255.0f,
                g / 255.0f,
                b / 255.0f,
                a / 255.0f);
   }

   void Graphics::setClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
   {
      glClearColor(r / 255.0f,
                   g / 255.0f,
                   b / 255.0f,
                   a / 255.0f);
   }

   void Graphics::clear()
   {
      glClear(GL_COLOR_BUFFER_BIT);
   }

   void Graphics::drawRect(int x, int y, unsigned w, unsigned h)
   {
      glBegin(GL_QUADS);
         glVertex2i(x, y);
         glVertex2i(x + w, y);
         glVertex2i(x + w, y + h);
         glVertex2i(x, y + h);
      glEnd();
   }

   void Graphics::drawGradient(const Color& from, const Color& to, int x, int y, unsigned w, unsigned h)
   {
      glBegin(GL_QUADS);
         setColor(from);
         glVertex2i(x, y);
         glVertex2i(x + w, y);
         setColor(to);
         glVertex2i(x + w, y + h);
         glVertex2i(x, y + h);
      glEnd();
   }

   void Graphics::drawLine(int x1, int y1, int x2, int y2)
   {
      glBegin(GL_LINES);
         glVertex2i(x1, y1);
         glVertex2i(x2, y2);
      glEnd();
   }

   void Graphics::drawPoint(int x, int y)
   {
      glBegin(GL_POINTS);
         glVertex2i(x, y);
      glEnd();
   }

   void Graphics::flip()
   {
      glFlush();
      SDL_GL_SwapBuffers();
   }

   void Graphics::resetBoundImage()
   {
      glBindTexture(GL_TEXTURE_2D, 0);
   }
}
