/// @file util/color_scale.h
/// @author grigoryj

#ifndef COLOR_SCALE_H
#define COLOR_SCALE_H

#include "graphics/misc.h"

namespace util
{
   /// Provide a way to pick a color that is "between" two
   /// edge colors
   class ColorScale
   {
   public:
      /// Create a new linear color scale
      /// @param from The color at the start of the scale
      /// @param to The color at the end
      ColorScale(const graphics::Color& from, const graphics::Color& to);
      
      /// @param scale_begin A value on the scale that corresponds to the first color
      /// @param scale_end A value that corresponds to the second color
      /// @param value The actual position on the scale for which we want to get the color
      /// @return a color "between" the two colors corresponding to the given parameters
      graphics::Color getColor(int scale_begin, int scale_end, int value);

   private:
      /// @return The value for a color component corresponding to the input parameters
      uint8_t getComponentValue(uint8_t from_value, uint8_t to_value, int scale_begin, int scale_end, int value);

      graphics::Color m_fromColor; /// Color at the beginning
      graphics::Color m_toColor;   /// Color at the end
   };
}

#endif // COLOR_SCALE_H