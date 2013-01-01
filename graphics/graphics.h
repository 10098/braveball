/// @file graphics/graphics.h
/// @author grigoryj

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "graphics/context.h"
#include "graphics/screen_context.h"

namespace graphics
{
   /// Contains routines to set up and shut down the two-dimensional
   /// graphics subsystem, also routines for drawing some 
   /// primitives and manipulating the transformation matrix.
   class Graphics
   {
   public:
      /// Shuts down the graphics subsystem
      ~Graphics();
   
   public:
      /// Initializes the graphics subsystem
      void init(unsigned width, unsigned height, bool fullscreen);

      /// Get a reference to the screen's drawing context
      Context& screenContext();

      /// Get a non-modifiable version of the screen's drawing context 
      /// (see graphics/context.h for a definition of what "drawing context" is)
      const Context& screenContext() const;

      /// Retrieve a transformation matrix from the top of the stack
      void popMatrix();

      /// Push the current transformation matrix to the stack
      void pushMatrix();

      /// Sets the current transformation matrix to identity
      void resetMatrix();

      /// Translates the current coordinate system origin by (x, y)
      void translate(int x, int y);

      /// Rotates the current coordinate system around its origin
      /// by a given amount of degrees
      void rotate(float degrees);

      /// Set the current drawing color
      /// @param color an RGBA color
      void setColor(const Color& c);

      /// Version of setColor that accepts separate color components
      void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff);

      /// Set the current `clear' color
      /// @param colro an RGBA color
      void setClearColor(const Color& c);

      /// Version of setColor that accepts separate color components
      void setClearColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff);

      /// Clear the display to the current clear color
      void clear();

      /// Draw an axis-aligned rectangle
      void drawRect(int x, int y, unsigned w, unsigned h);

      /// Draw a gradient axis-aligned rectangle
      void drawGradient(const Color& from, const Color& to, int x, int y, unsigned w, unsigned h);

      /// Draw a line between two points
      void drawLine(int x1, int y1, int x2, int y2);

      /// Draw a single point
      void drawPoint(int x, int y); 

      /// Flip front and back buffers
      void flip();

      /// Resets the currently bound image
      void resetBoundImage();
      
   private:
      ScreenContext m_ctx; /// Screen context
   };
}

#endif // GRAPHICS_H
