/// @file util/color_scale.cpp
/// @author grigoryj

#include "util/color_scale.h"
#include <cmath>

namespace util
{
   ColorScale::ColorScale(const graphics::Color& from, const graphics::Color& to):
      m_fromColor(from),
      m_toColor(to)
   {}

   graphics::Color ColorScale::getColor(int scale_begin, int scale_end, int value)
   {
      return graphics::Color(
                              getComponentValue(m_fromColor.r, m_toColor.r, scale_begin, scale_end, value),
                              getComponentValue(m_fromColor.g, m_toColor.g, scale_begin, scale_end, value),
                              getComponentValue(m_fromColor.b, m_toColor.b, scale_begin, scale_end, value),
                              getComponentValue(m_fromColor.a, m_toColor.a, scale_begin, scale_end, value)
                            );
   }

   uint8_t ColorScale::getComponentValue(uint8_t from, uint8_t to, int scale_begin, int scale_end, int value)
   {
      if(scale_begin >= scale_end)
      {
         scale_begin = scale_begin + scale_end;
         scale_end = scale_begin - scale_end;
         scale_begin = scale_begin - scale_end;
      }

      if(value < scale_begin) 
      {
         return from;
      }
      else if(value > scale_end)
      {
         return to;
      }
      else
      {
         int scale_size = static_cast<int>(to) - static_cast<int>(from);
         double scale_factor = static_cast<double>(value - scale_begin)/static_cast<double>(scale_end - scale_begin) * scale_size;
         int offset = static_cast<int>(scale_factor);
         return static_cast<uint8_t>(from + offset);
      }
   }
}