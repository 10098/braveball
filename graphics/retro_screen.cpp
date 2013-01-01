#include "graphics/retro_screen.h"

namespace graphics
{
   RetroScreen::RetroScreen(Graphics& g, Context& target, unsigned scale, bool enable_scanlines):
      m_targetContext(target),
      m_canvas(target.width()/scale, target.height()/scale),
      m_scanlineCanvas(target.width(), target.height()),
      m_enableScanlines(enable_scanlines),
      m_scale(scale)
   {
      if(enable_scanlines)
      {
         m_scanlineCanvas.activate();
         g.setColor(0x00, 0x00, 0x00, 0x37);
         for(int i = 0; i < m_scanlineCanvas.height(); i += 3)
         {
            g.drawLine(0, i, m_scanlineCanvas.width(), i);
         }
         g.setColor(0xff, 0xff, 0xff);
      }
      activate();
   }

   void RetroScreen::activate()
   {
      m_canvas.activate();
   }

   int RetroScreen::width() const
   {
      return m_canvas.width();
   }

   int RetroScreen::height() const
   {
      return m_canvas.height();
   }

   void RetroScreen::draw()
   {
      m_targetContext.activate();
      m_canvas.image().draw(0, 0, m_scale);
      if(m_enableScanlines)
      {
         m_scanlineCanvas.image().draw(0, 0);
      }
   }
}
