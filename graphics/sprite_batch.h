/// @file graphics/sprite_batch.h
/// @author grigoryj

#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include "graphics/misc.h"
#include "graphics/image.h"
#include "graphics/misc.h"

namespace graphics
{
   /// SpriteBatch allows to draw a bunch of sprites quickly
   /// without having to call glBegin/glEnd for each one.
   /// It's associated with a single texture atlas from which the sprites are drawn.
   class SpriteBatch
   {
   public:
      /// @param img The texture atlas
      explicit SpriteBatch(const Image& img);

      /// Begin drawing operations
      void begin();

      /// Add a new sprite
      /// @param x x position on the target (e.g. screen)
      /// @param y y position on the target
      /// @param src_rect rectangle specifying which portion of the texture atlas to draw
      void add(int x, int y, const AlignedQuad& src_rect);

      /// Same as the previous one, but adds color.
      /// After the operation is complete, the color is restored to the one that was set before calling begin()
      void add(int x, int y, const AlignedQuad& src_rect, const graphics::Color& c);

      /// Same, but with individual color components as arguments
      void add(int x, int y, const AlignedQuad& src_rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff);

      /// End drawing operations
      void end();

   private:
      const Image& m_img; /// The associated texture atlas
      GLfloat m_color[4]; /// Needed to store the previous color for operations that change color temporarily

      SpriteBatch& operator=(const SpriteBatch&);
   };
}

#endif // SPRITE_BATCH