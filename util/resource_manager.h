/// @file util/resource_manager.h
/// @author grigoryj

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "graphics/animation.h"
#include "graphics/image.h"
#include "graphics/font.h"
#include "sound/sound.h"
#include "util/resource_file.h"
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <functional>

namespace util
{
   /// Loads resources into memory and provides simple access to them.
   /// As such, this is more an "accessor" than a "manager", because it's dumb and 
   /// doesn't try to decide which resources to load and which to unload.
   /// For convenience, it provides some default placeholder/resource when a requested
   /// resource doesn't exist (forgetting to update the resource file shouldn't derail the entire game).
   class ResourceManager
   {
   public:
      ResourceManager();

      /// Look up an animation
      /// @param key Name of the animation
      /// @return const reference to the object representing the required animation or a default animation if the requested animation has not been found
      const graphics::AnimationData& getAnimationData(const std::string& key) const
      { return lookupResource(m_animationMap, key, std::bind(&ResourceManager::loadAnimation, this, std::placeholders::_1), m_defaultAnimation); }

      /// Look up an image
      /// @param key Name of the image
      /// @return const ref to the object representing the required image. Will return a default image if the image can't be found or loaded
      const graphics::Image& getImage(const std::string& key) const
      { return lookupResource(m_imageMap, key, std::bind(&ResourceManager::loadImage, this, std::placeholders::_1), m_defaultImage); }

      /// Look up a font
      /// @param key Specifies which font to load, and what size (e.g. bitmap/superpixelfont/8 will load /fonts/bitmap/superpixelfont.ttf with size 8px)
      /// @return const ref to the font object. Will return a default font if the font isn't there or cannot be loaded.
      const graphics::Font& getFont(const std::string& key) const
      { return lookupResource(m_fontMap, key, std::bind(&ResourceManager::loadFont, this, std::placeholders::_1), m_defaultFont); }

      const sound::SoundData& getSound(const std::string& key) const
      { return lookupResource(m_soundMap, key, std::bind(&ResourceManager::loadSound, this, std::placeholders::_1), m_defaultSound); }

   private:
      // Looking up different types of resources is pretty similar, so let's have a generic routine for that.
      template <class T>
      const T& lookupResource(const std::unordered_map<std::string, std::shared_ptr<T> >& map, 
                              const std::string& key, 
                              const std::function<void(const std::string&)>& loader,
                              const T& default_result) const
      {
         try
         {
            auto it = map.find(key);
            if(it == map.end())
            {
               loader(key);  // Load the resource (using the custom loader function) the first time it's requested
               it = map.find(key); // By this point, it != map.end (if the resource isn't found, an exception is thrown).            
            }
            return *(it -> second);
         }
         catch(const ResourceNotFoundException&)
         {
            return default_result;
         }
      }

      /// Load an animation
      void loadAnimation(const std::string& path) const;
      
      /// Load an image
      void loadImage(const std::string& path) const;

      /// Load a font
      void loadFont(const std::string& path) const;

      /// Loads a sound
      void loadSound(const std::string& path) const;

      /// Map of animations
      mutable std::unordered_map<std::string, std::shared_ptr<graphics::AnimationData> > m_animationMap;
      
      /// Map of images
      mutable std::unordered_map<std::string, std::shared_ptr<graphics::Image> > m_imageMap;

      /// Map of fonts
      mutable std::unordered_map<std::string, std::shared_ptr<graphics::Font> > m_fontMap;

      /// Map of sounds
      mutable std::unordered_map<std::string, std::shared_ptr<sound::SoundData> > m_soundMap;

      /// The resource file
      ResourceFile m_resFile;

      /// Canvas for the default image
      graphics::Image m_defaultImage;

      /// Default animation to load when a requested animation isn't found
      graphics::AnimationData m_defaultAnimation;

      /// Default font
      graphics::Font m_defaultFont;

      /// Default sound
      sound::SoundData m_defaultSound;
   };
}

#endif // RESOURCE_MANAGER_H
