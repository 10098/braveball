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

   void YounglingEater::draw(graphics::SpriteBatch&)
   {
      // There was some drawing code here for debugging purposes but it was removed later
   }

}