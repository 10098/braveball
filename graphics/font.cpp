/// @file graphics/font.cpp
/// @author grigoryj

#include "graphics/font.h"
#include <SDL/SDL.h>
#include <stdexcept>

#ifndef GL_BGR
#define GL_BGR GL_BGR_EXT
#endif

#ifndef GL_BGRA
#define GL_BGRA GL_BGRA_EXT
#endif

namespace graphics
{
   Font::Font(const std::string& file_name, int font_size)
   {
      // Try to load the font from a file, throw exception on failure
      m_font = TTF_OpenFont(file_name.c_str(), font_size);
      if(NULL == m_font)
      {
         throw std::runtime_error(TTF_GetError());
      }
   }

   Font::~Font()
   {
      TTF_CloseFont(m_font);
   }

   Font::Font(const void* data, int data_size, int font_size)
   {
      // Try to load a font from memory, throw exception on failure.

      // Create a SDL_RWops object from memory (SDL_TTF can load fonts from RWops, but not firectly from memory)
      SDL_RWops* font_rw = SDL_RWFromConstMem(data, data_size);

      if(NULL == font_rw) // Check if the creation of rwops succeeded
      {
         throw std::runtime_error(SDL_GetError());
      }

      // Attempt to load the font from rwops
      m_font = TTF_OpenFontRW(font_rw, 1, font_size);
      if(NULL == m_font)
      {
         throw std::runtime_error(TTF_GetError());
      }
   }

   std::shared_ptr<Image> Font::renderString(const std::string& str) const
   {
      SDL_Color c;
      c.r = 0x00;
      c.g = 0x00;
      c.b = 0x00;
      
      // Render the string onto an SDL_Surface
      struct SDL_Surface* font_surface = TTF_RenderUTF8_Blended(m_font, str.c_str(), c); // This is probably the slowest of all the sdlttf functions, but screw it, i want fonts to look nice
      if(NULL == font_surface)
      {
         throw std::runtime_error(TTF_GetError());
      }
      
      // Now it's time to convert the surface object into a texture object that OpenGL can understand.

      // Save the previously bound texture
      GLint prev_tex;
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &prev_tex);

      GLuint string_texture; // this will be the texture that we'll use for the rendered string
      glGenTextures(1, &string_texture);
      glBindTexture(GL_TEXTURE_2D, string_texture);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      // Determine the pixel format to use with the opengl texture
      int bpp = font_surface -> format -> BytesPerPixel;
      int format;
      if(font_surface -> format -> Rmask == 0x000000ff)
      {
         format = (4 == bpp) ? GL_RGBA : GL_RGB;
      }
      else
      {
         format = (4 == bpp) ? GL_BGRA : GL_BGR;
      }

      // Send the pixel data
      glTexImage2D(GL_TEXTURE_2D, 0, bpp, font_surface->w , font_surface->h, 0, format, GL_UNSIGNED_BYTE, font_surface -> pixels);

      // Rebind the previous texture
      glBindTexture(GL_TEXTURE_2D, prev_tex);

      // Don't need the surface object any more
      SDL_FreeSurface(font_surface);

      // Create and return a new Image object from texture handle
      return std::shared_ptr<Image>(new Image(string_texture));
   }
}
