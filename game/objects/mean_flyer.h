/// @file game/objects/mean_flyer.h
/// @author grigoryj

#ifndef MEAN_FLYER_H
#define MEAN_FLYER_H

#include "game/objects/meany.h"
#include "util/throttle.h"

namespace game
{
   class MeanFlyer : public Meany
   {
   public:
      MeanFlyer(const Engine& e, int x, int y, int min_x, int max_x);
   
   private:
       virtual void updatePosition(Engine& ctx);

       util::Throttle m_flyThrottle;
       util::Throttle m_upDownThrottle;
       int m_baseY;
   };
}

#endif // MEAN_FLYER_H