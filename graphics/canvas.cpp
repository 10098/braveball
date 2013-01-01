#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/glew.h>
#include "graphics/canvas.h"
#include <stdexcept>

namespace graphics
{
   Canvas::Canvas(unsigned width, unsigned height) : m_width(width), m_height(height)
   {
	   GLint prev_tex;
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_tex);
	   glGenTextures(1, &m_tex);
      glBindTexture(GL_TEXTURE_2D, m_tex);
	   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_INT, NULL);
      glBindTexture(GL_TEXTURE_2D, 0);

      glGenFramebuffers(1, &m_fbo);
      glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);
      GLenum fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glBindTexture(GL_TEXTURE_2D, prev_tex);

      if(fbo_status != GL_FRAMEBUFFER_COMPLETE)
      {
         throw std::runtime_error("Failed to set up a framebuffer for the canvas");
      }

      m_img.reset(new Image(m_tex));
   }

   Canvas::~Canvas()
   {
      // Free the associated texture and frame buffer
      glDeleteFramebuffers(1, &m_fbo);

      if(m_tex)
      {
         GLint prev_tex;
         glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_tex);

         if(static_cast<GLuint>(prev_tex) == m_tex)
         {
            glBindTexture(GL_TEXTURE_2D, 0);
         }

         glDeleteTextures(1, &m_tex);
      }
   }

   void Canvas::activate()
   {
      glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
      glBindTexture(GL_TEXTURE_2D, 0);
      glMatrixMode(GL_PROJECTION);
	   glViewport(0, 0, m_width, m_height);
      glLoadIdentity();
	   glOrtho(0, m_width, 0, m_height, -1, 1);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
   }

   int Canvas::width() const
   {
      return m_width;
   }

   int Canvas::height() const
   {
      return m_height;
   }

   const Image& Canvas::image() const
   {
      return *m_img;
   }
}
