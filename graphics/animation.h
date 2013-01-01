/// @file graphics/animation.h
/// @author grigoryj

#ifndef ANIMATION_H
#define ANIMATION_H

#include "graphics/misc.h"
#include "util/throttle.h"
#include "util/clock.h"
#include <vector>

namespace  graphics
{
   struct AnimationData
   {
      std::vector<graphics::AlignedQuad> frames;
      int frame_duration;
      bool loop;
   };

   /// Plays a sequence of axis-aligned quads
   class Animation
   {
   public:
      explicit Animation(const AnimationData& data);

      /// Updates the current frame quad 
      void update(const util::Clock& clk);

      /// Get the current frame quad
      const AlignedQuad& frame() const;

      /// Set the current frame quad
      /// @throw rutime_error if index is out of bounds
      void setCurrentFrame(unsigned int f);

      /// Get the total number of frames
      unsigned int frameCount() const;

      /// Returns the number of the current frame
      unsigned int currentFrameNumber() const;

      /// Return true of the animation is over
      bool isActive() const;

      /// (Re)starts the animation
      void play();

      /// Freezes the animation at the current frame
      void stop();
      
   private:
      Animation& operator=(const Animation&);

      const std::vector<AlignedQuad>& m_frames;
      util::Throttle m_animThr;
      bool m_active;
      bool m_loop;
      unsigned int m_currentFrame;
   };
}

#endif // ANIMATION_H