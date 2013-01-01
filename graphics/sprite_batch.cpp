/// @file graphics/sprite_batch.cpp
/// @author grigoryj

#ifdef _MSC_VER
#include <windows.h>
#endif
#include "GL/gl.h"
#include "graphics/sprite_batch.h"

namespace graphics
{
   SpriteBatch::SpriteBatch(const Image& img):m_img(img){}

   void SpriteBatch::begin()
   {
      glGetFloatv(GL_CURRENT_COLOR, m_color);
      m_img.bind();
      glBegin(GL_QUADS);
   }

   void SpriteBatch::end()
   {
      glEnd();
   }

   void SpriteBatch::add(int x, int y, const AlignedQuad& q)
   {
      glTexCoord2i(q.tl.x, q.tl.y);
      glVertex2i(x, y);

      glTexCoord2i(q.tl.x + q.w, q.tl.y);
      glVertex2i(x + q.w, y);

      glTexCoord2i(q.tl.x + q.w, q.tl.y + q.h);
      glVertex2i(x + q.w, y + q.h);

      glTexCoord2i(q.tl.x, q.tl.y + q.h);
      glVertex2i(x, y + q.h);
   }

   void SpriteBatch::add(int x, int y, const AlignedQuad& q, const graphics::Color& c)
   {
      add(x, y, q, c.r, c.g, c.b, c.a);
   }

   void SpriteBatch::add(int x, int y, const AlignedQuad& q, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
   {
      glColor4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
      add(x, y, q);
      glColor4f(m_color[0], m_color[1], m_color[2], m_color[3]);
   }

}
