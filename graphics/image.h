/// @file graphics/image.h
/// @author grigoryj

#ifndef IMAGE_H
#define IMAGE_H

#include "graphics/misc.h"
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>
#include <IL/il.h>
#include <string>

namespace graphics
{
   /// Image that can be loaded from a bitmap, TGA or PNG file
   /// Note to future self: don't use this in multithreaded code!
   class Image 
   {
   public:
      /// Type of the image
      enum Type
      {
         PNG = IL_PNG,
         TGA = IL_TGA,
         BMP = IL_BMP,
         PNM = IL_PNM
      };
      
      typedef GLuint Handle;

    public:
      /// Creates invalid image
      Image(){}

      /// Load a new image from memory
      /// @param buf a memory area containing binary data
      /// @param size size of that memory area
      /// @param type of the image
      Image(const void* buf, int size, Type type);

      /// Load an image from file
      /// @param filename name of the file to load from
      explicit Image(const std::string& filename);

      /// Move ctor
      /// Transfers image handle and attrs from an rvalue reference
      Image(Image&& that);

      
      /// Create image from a handle
      explicit Image(Handle handle);

      /// Free the associated resources
      ~Image();

   public:
      void bind() const;
      void draw(int x, int y, int scale_factor = 1) const;
      void draw(int x, int y, const AlignedQuad& q, int scale_factor = 1) const;
      
      int width() const;
      int height() const;

      /// Get the image's handle
      Handle handle() const;

   private:
      static GLuint BoundImage;

      /// No assignment allowed
      Image& operator=(const Image&);

      /// Convert a loaded IL image to an OpenGL 2D texture
      void load();

      GLuint m_handle; /// OpenGL texture

      unsigned m_width;  /// Width of the image in pixels
      unsigned m_height; /// Height of the image in pixels
   };
}

#endif // IMAGE_H
