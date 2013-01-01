/// @file graphics/retro_screen.h
/// @author grigoryj

#include "graphics/graphics.h"
#include "graphics/context.h"
#include "graphics/screen_context.h"
#include "graphics/canvas.h"

#ifndef RETRO_SCREEN_H
#define RETRO_SCREEN_H

namespace graphics
{
   /// Use this to add a "retro" effect to the game,
   /// which is actually just a pixelated picture with
   /// scanlines.
   class RetroScreen : public Context
   {
   public:
      /// Create a new retroscreen effect
      /// @param g Graphics object
      /// @param target Target graphics context
      /// @param scale How many times should the retroscreen be smaller than the target context
      /// @param enable_scanlines Whether to use scanline effect
      RetroScreen(Graphics& g, Context& target, unsigned scale, bool enable_scanlines);

      /// Activate the context
      void activate();

      /// @return width of the context
      int width() const;

      /// @return height of the context
      int height() const;

      /// Draw this context on screen. The picture will 
      /// be strectched to cover the entire screen (and 
      /// pixels will become larger as a result). Scanline effect 
      /// will also be applied if enabled. 
      /// The target context will be active after this operation.
      void draw();

   private:
      Context& m_targetContext; /// Target context
      Canvas m_canvas;          /// Drawing operations will happen on this canvas
      Canvas m_scanlineCanvas;  /// Scanlines will be drawn on this canvas
      bool m_enableScanlines;   /// Set to true if scanlines are enabled
      unsigned m_scale;         /// Scale factor
   };
}

#endif // RETRO_SCREEN_H