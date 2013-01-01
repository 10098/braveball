///@file graphics/font.h
///@author grigoryj

#ifndef FONT_H
#define FONT_H

#include "graphics/misc.h"
#include "graphics/image.h"
#include <SDL_ttf.h>
#include <memory>
#include <string>

namespace graphics
{
   /// Can load fonts from files or memory and render strings
   class Font
   {
   public:
      /// Load font from file
      /// @param file_name Font's file name
      /// @param size the size of the font to load
      explicit Font(const std::string& file_name, int font_size);

      /// Load font from memory
      /// @param data Font data
      /// @param data_size Size of font data
      /// @param size Font size to load
      Font(const void* data, int data_size, int font_size);

      /// Frees the allocated resources
      ~Font();

      /// Renders the given text as string and returns an image
      /// @param str The string to render
      /// @retunr A shared pointer to an image object containing the rendered string
      std::shared_ptr<Image> renderString(const std::string& str) const ;/*, const Color& color);*/

   private:
      TTF_Font* m_font; /// The font resource
   };
}

#endif // FONT_H