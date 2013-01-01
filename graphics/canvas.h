#ifndef GRAPHICS_CANVAS_H
#define GRAPHICS_CANVAS_H

#include "graphics/image.h"
#include "graphics/context.h"
#include <memory>

namespace graphics
{
   /// A canvas (an off-screen rendering target)
   class Canvas : public Context
   {
   public:
      /// Create a new canvas with the give width and height.
      /// NOTE: the dimensions better be powers of two
      Canvas(unsigned width, unsigned height);

      /// Frees the resources associated with the canvas
      ~Canvas();
   
      /// Sets the canvas as the current drawing context
      void activate();

      /// Get the width of the canvas
      int  width() const;

      /// Get the height of the canvas
      int height() const;

      /// Get the image of the canvas
      const Image& image() const;

   private:
      GLuint m_fbo; /// The frame buffer object
      GLuint m_tex; /// The texture object
      unsigned m_width; /// Width of the canvas
      unsigned m_height; /// Height of the canvas
      std::unique_ptr<Image> m_img; /// Image object
   };
}
#endif // GRAPHICS_CANVAS_H

