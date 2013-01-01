/// @file graphics/misc.h
/// @author grigoryj

#ifndef GRAPHICS_MISC_H
#define GRAPHICS_MISC_H

#include <stdint.h>

/// Contains graphics-related classes and subroutines
namespace graphics
{
   /// Representation of an RGBA color
   struct Color
   {
      Color(uint8_t red, uint8_t grn, uint8_t blu, uint8_t alph = 255): r(red),g(grn),b(blu),a(alph){}
      uint8_t r;
      uint8_t g;
      uint8_t b;
      uint8_t a;
   };

   /// A two-dimensional vertex
   struct Vertex
   {
      Vertex(int ix, int iy):x(ix),y(iy){}
      Vertex():x(0),y(0){}
      int x;
      int y;
   };
   
   /// An arbitrary two-dimensional quad
   struct Quad
   {
     Vertex v[4];
   };

   /// An axis-aligned quad
   struct AlignedQuad
   {
      AlignedQuad(int ix, int iy, int iw, int ih):tl(ix,iy),w(iw),h(ih){}
      Vertex tl; /// Top left corner
      int w;     /// Width
      int h;     /// Height

      bool collidesWith(const AlignedQuad& q) const
      {
         return lineSegmentsOverlap(tl.x, tl.x + w, q.tl.x, q.tl.x + q.w) && lineSegmentsOverlap(tl.y, tl.y + h, q.tl.y, q.tl.y + q.h);
      }

   private:
      bool lineSegmentsOverlap(int x11, int x12, int x21, int x22) const
      {
         return !(x11 < x21 && x12 < x21 || x21 < x11 && x22 < x11);
      }
   };
}

#endif // GRAPHICS_MISC_H
