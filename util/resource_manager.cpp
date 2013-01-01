/// @file util/resource_manager.cpp
/// @author grigoryj

#include "util/resource_manager.h"
#include "util/default_resource_data.h"
#include <boost/lexical_cast.hpp>
#include <fstream>

namespace
{
   struct membuf: std::streambuf {
       membuf(char const* base, size_t size) {
           char* p(const_cast<char*>(base));
           this->setg(p, p, p + size);
       }
   };

   struct imemstream: std::istream {
       imemstream(char const* base, size_t size)
           : mb(base, size), std::istream(&mb) {
       }
   private:
      membuf mb;
   };
}

namespace util
{   
   ResourceManager::ResourceManager():
      m_resFile("resources.res"),
      m_defaultImage(DEFAULT_IMAGE_DATA, sizeof(DEFAULT_IMAGE_DATA), graphics::Image::PNG),
      m_defaultFont(DEFAULT_FONT_DATA, sizeof(DEFAULT_FONT_DATA), 8),
      m_defaultSound(0, 0)
   {
      m_defaultAnimation.frames.push_back(graphics::AlignedQuad(0, 0, 32, 32));
      m_defaultAnimation.frame_duration = 60;
      m_defaultAnimation.loop = false;
   }

   void ResourceManager::loadAnimation(const std::string& name) const
   {
      const ResourceData& animation_res = m_resFile.getResource("animations/" + name + ".txt");
      imemstream in(reinterpret_cast<const char*>(animation_res.data), animation_res.size);

      unsigned frames_count, frame_duration;
      bool loop;
      std::vector<graphics::AlignedQuad> frames;
      
      in >> frames_count >> frame_duration >> loop;
      m_animationMap[name].reset(new graphics::AnimationData());
      auto anim_data = m_animationMap.find(name);
      anim_data -> second -> frame_duration = frame_duration;
      anim_data ->second -> loop = loop;
      for(unsigned i = 0; i < frames_count; ++i)
      {
         int x,y,w,h;
         in >> x >> y >> w >> h;
         anim_data -> second -> frames.push_back(graphics::AlignedQuad(x, y, w, h));
      }
   }

   void ResourceManager::loadImage(const std::string& path) const
   {
      const ResourceData& image_res_data = m_resFile.getResource("images/" + path + ".png");
      m_imageMap[path].reset(new graphics::Image(image_res_data.data, image_res_data.size, graphics::Image::PNG));
   }

   void ResourceManager::loadSound(const std::string& path) const
   {
      const ResourceData& snd_res_data = m_resFile.getResource("sounds/" + path);
      m_soundMap[path].reset(new sound::SoundData(snd_res_data.data, snd_res_data.size));
   }

   void ResourceManager::loadFont(const std::string& path_and_size) const
   {
      int font_size = 8;

      // If the last component of the path is numeric, treat it as the size of the font to load
      size_t last_slash = path_and_size.find_last_of('/');
      std::string font_path = path_and_size.substr(0, last_slash);
      std::string size_str = path_and_size.substr(last_slash + 1);
      
      try
      {
         font_size = boost::lexical_cast<int>(size_str);
         const ResourceData& font_res_data = m_resFile.getResource("fonts/" + font_path);
         std::shared_ptr<graphics::Font> loaded_font(new graphics::Font(font_res_data.data, font_res_data.size, font_size));
         m_fontMap[path_and_size] = loaded_font;
      }
      catch(const boost::bad_lexical_cast&)
      {
         throw ResourceInvalidException(path_and_size, "bad font size");
      }
      catch(const std::runtime_error& e)
      {
         throw ResourceInvalidException(path_and_size, e.what());
      }
   }
}