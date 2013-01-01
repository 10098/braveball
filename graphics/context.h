/// @file graphics/context.h
/// @author grigoryj

#ifndef CONTEXT_H
#define CONTEXT_H

#include "graphics/misc.h"

namespace graphics
{
   /// Abstract class representing a drawing context.
   /// A drawing context may be a screen or a canvas or something similar.
   class Context
   {
   public:
      /// Activates the given context. All drawing operations will be performed within 
      /// that context.
      virtual void activate() = 0;

      /// @return the width of the drawing context
      virtual int width() const = 0;

      /// @return the height of the drawing context
      virtual int height() const = 0;
   };
}

#endif // CONTEXT_H
