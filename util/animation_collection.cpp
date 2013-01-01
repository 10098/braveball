/// @file util/animation_collection.cpp
/// @author grigoryj

#include "util/animation_collection.h"

namespace util
{
   void AnimationCollection::addAnim(const std::string& name, const graphics::Animation& animation)
   {
      m_anims.insert(std::make_pair(name, animation));
   }

   void AnimationCollection::setCurrentAnim(const std::string& name)
   {
      m_currentAnim = m_anims.find(name);
      if(m_currentAnim == m_anims.end())
      {
         throw std::runtime_error("Animation not found");
      }
      m_currentAnim -> second.play();
   }

   graphics::Animation& AnimationCollection::currentAnim()
   {
      if(m_currentAnim == m_anims.end())
      {
         throw std::runtime_error("Current animation not set");
      }
      return m_currentAnim -> second;
   }
}