/// @file util/animation_collection.h
/// @author grigoryj

#ifndef ANIMATION_COLLECTION_H
#define ANIMATION_COLLECTION_H

#include "graphics/animation.h"
#include <unordered_map>
#include <string>

namespace util
{
   /// A convenient way of managing multiple animations.
   /// Useful for game objects that have to juggle a bunch
   /// of animations (e.g. walk_right, walk_left, etc.)
   class AnimationCollection
   {
   public:
      void addAnim(const std::string& name, const graphics::Animation& anim);
      void setCurrentAnim(const std::string& anim_name);
      graphics::Animation& currentAnim();

   private:
      typedef std::unordered_map<std::string, graphics::Animation> AnimationMap;
      typedef AnimationMap::iterator AnimationIterator;
      
      AnimationMap m_anims;
      AnimationIterator m_currentAnim;
   };
}

#endif // ANIMATION_COLLECTION_H