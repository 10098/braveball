/// @file graphics/animation.cpp
/// @author grigoryj

#include "graphics/animation.h"

namespace graphics
{
   Animation::Animation(const AnimationData& data):
      m_frames(data.frames),
      m_animThr(data.frame_duration),
      m_active(true),
      m_loop(data.loop),
      m_currentFrame(0)
   {}

   void Animation::play()
   {
      m_currentFrame = 0;
      m_active = true;
   }

   void Animation::stop()
   {
      m_active = false;
   }

   void Animation::update(const util::Clock& clk)
   {
      if(m_active)
      {
         if(m_animThr(clk.ticks()))
         {
            m_currentFrame++;
            if(m_loop) m_currentFrame  = m_currentFrame % m_frames.size();
            else if(m_currentFrame >= m_frames.size())
             {
                m_currentFrame--;
                m_active = false;
             }
         }
      }
   }

   bool Animation::isActive() const
   {
      return m_active;
   }

   unsigned int Animation::frameCount() const
   {
      return m_frames.size();
   }

   const AlignedQuad& Animation::frame() const
   {
      return m_frames[m_currentFrame];
   }

   void Animation::setCurrentFrame(unsigned int f)
   {
      if(f > frameCount()) throw std::runtime_error("");
      else m_currentFrame = f;
   }

   unsigned int Animation::currentFrameNumber() const
   { 
      return m_currentFrame; 
   }

}
