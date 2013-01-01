#include "graphics/image.h"
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>
#include <IL/il.h>
#include <vector>
#include <stdexcept>
#include <fstream>

namespace graphics
{
   GLuint Image::BoundImage = 0;

   Image::Image(const void* data, int size, Image::Type type)
   {
      if(!ilLoadL(type, data, size))
      {
         throw std::runtime_error("Failed to load image from memory");
      }

      load();
   }

   Image::Image(const std::string& file_name)
   {
      if(!ilLoadImage(file_name.c_str()))
      {
         throw std::runtime_error("Failed to load image from file");
      }

      load();
   }

   Image::Image(Image&& that)
   {
      m_handle = that.m_handle;
      m_height = that.m_height;
      m_width = that.m_width;
      that.m_handle = 0;
   }

   Image::~Image()
   {
      if(m_handle)
      {
         GLint prev_tex;
         glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_tex);

         if(static_cast<GLuint>(prev_tex) == m_handle)
         {
            glBindTexture(GL_TEXTURE_2D, 0);
         }

         glDeleteTextures(1, &m_handle);
      }
   }
   
   Image::Image(Image::Handle h):m_handle(h)
   {
      // Get the handle of the previously bound texture
      GLint prev_tex;
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_tex);

      glBindTexture(GL_TEXTURE_2D, m_handle);
      glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  (GLint*)&m_width);
      glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, (GLint*)&m_height);
      
      // Rebind the previously bound texture
      glBindTexture(GL_TEXTURE_2D, prev_tex);
   }

   void Image::load() 
   {
      // Convert the image to a convenient format
      m_width = ilGetInteger(IL_IMAGE_WIDTH);
      m_height = ilGetInteger(IL_IMAGE_HEIGHT);
      std::vector<uint8_t> data(m_width*m_height*4);
      ilCopyPixels(0, 0, 0, m_width, m_height, 1, IL_RGBA, IL_UNSIGNED_BYTE, &(data[0]));
      ILenum error = ilGetError();
      if(error != IL_NO_ERROR)
         throw std::runtime_error("Failed to convert image into RGBA format");
      ilBindImage(0);
      
      // Upload image data to video device
      GLint prev_tex;
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_tex);

      glGenTextures(1, &m_handle);
      glBindTexture(GL_TEXTURE_2D, m_handle);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
      glBindTexture(GL_TEXTURE_2D, prev_tex);
   }

   void Image::bind() const
   {
      if(m_handle != BoundImage)
      {
         glBindTexture(GL_TEXTURE_2D, m_handle);
         glMatrixMode(GL_TEXTURE);
         glLoadIdentity();
         glScaled(1.0/m_width, 1.0/m_height, 1);
         glMatrixMode(GL_MODELVIEW);
         BoundImage = m_handle;
      }
   }

   void Image::draw(int x, int y, int scale_factor) const
   {
      draw(x, y, AlignedQuad(0, 0, m_width, m_height), scale_factor);  
   }

   void Image::draw(int x, int y, const AlignedQuad& q, int scale_factor) const
   {
      bind();
      glBegin(GL_QUADS);
         glTexCoord2i(q.tl.x, q.tl.y);
         glVertex2i(x, y);

         glTexCoord2i(q.tl.x + q.w, q.tl.y);
         glVertex2i(x + q.w * scale_factor, y);

         glTexCoord2i(q.tl.x + q.w, q.tl.y + q.h);
         glVertex2i(x + q.w * scale_factor, y + q.h * scale_factor);

         glTexCoord2i(q.tl.x, q.tl.y + q.h);
         glVertex2i(x, y + q.h * scale_factor);     
      glEnd();  
   }

   int Image::width() const
   {
      return m_width;
   }

   int Image::height() const
   {
      return m_height;
   }

   Image::Handle Image::handle() const
   {
      return m_handle;
   }
}

