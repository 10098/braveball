#include "game/objects/youngling_eater.h"
#include <limits>

#ifdef max
#undef max
#endif

namespace game
{
   YounglingEater::YounglingEater(int initial_x, int initial_y):
      GameObject(initial_x, initial_y, 3, 9000)   
   {
      addTag("eater");
   }

   void YounglingEater::draw(graphics::SpriteBatch& b)
   {
      // b.add(x(), y(), graphics::AlignedQuad(0, 0, width(), height()), graphics::Color(0xff, 0x00, 0x00));
   }

}