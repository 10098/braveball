#include "game/platform_generator.h"
#include "game/engine.h"
#include "game/objects/platform.h"
#include "game/objects/youngling.h"
#include "game/objects/mean_walker.h"
#include "game/objects/mean_jumper.h"
#include "game/objects/mean_flyer.h"
#include "game/objects/heart.h"
#include "game/objects/exit_portal.h"
#include <stdexcept>

namespace game
{
   PlatformGenerator::PlatformGenerator(unsigned min_blocks, unsigned max_blocks,
                                        unsigned min_h_gap,  unsigned max_h_gap,
                                        unsigned min_v_gap,  unsigned max_v_gap,
                                        int default_y, 
                                        unsigned max_x):
      m_minBlockLength(min_blocks), 
      m_maxBlockLength(max_blocks),
      m_minHorizontalGap(min_h_gap), 
      m_maxHorizontalGap(max_h_gap),
      m_minVerticalGap(min_v_gap), 
      m_maxVerticalGap(max_v_gap),
      m_defaultYCoord(default_y),
      m_maxX(max_x),
      m_exitPortalGenerated(false)
   {
      // Verify that limits are ordered properly
      if(m_minBlockLength > m_maxBlockLength ||
         m_minHorizontalGap > m_maxHorizontalGap ||
         m_minVerticalGap > m_maxVerticalGap)
      {
         throw std::runtime_error("Bad platform generator parameters");
      }
   }

   int PlatformGenerator::edgeCoord() const
   {
      return m_maxX;
   }

   unsigned PlatformGenerator::generatePlatforms(std::list<std::shared_ptr<Platform> >& platforms_list, unsigned length, const Engine& e)
   {
      // Some constants
      const int LastPlatformWidth = 6; // Width of the very last platform (containing the Exit Portal), in blocks

      // Determine the X coordinate from which to start generating platforms
      unsigned start_from = platforms_list.empty() ? 0 : platforms_list.back() -> x() + platforms_list.back() -> width() + m_minHorizontalGap;

      // This will be the Y coordinate for the next platform. Initialize to the same y coordinate as the last platform in the list,
      // or the default Y coordinate if the list is empty.
      int y = platforms_list.empty() ? m_defaultYCoord : platforms_list.back() -> y();

      // Total length of the world in pixels.
      // At this point, start_from *is* the length of the entire world.
      unsigned total_pixel_length = start_from;

      srand(static_cast<unsigned>(time(NULL))); // randomize

      // Iterate until total world length reaches the limit   
      while(total_pixel_length < start_from + length)
      {
         // If we hit the max. width limit...
         if(total_pixel_length >= m_maxX)
         {
            // ...and the exit portal hasn't been generated yet...
            if(!m_exitPortalGenerated)
            {
               // Add a new platform to the world
               platforms_list.push_back(std::shared_ptr<Platform>(new Platform(e, total_pixel_length, y, LastPlatformWidth)));

               // Create and position the exit portal
               GameObjectPtr exit_portal(new ExitPortal(e, platforms_list.back() -> x() + platforms_list.back() -> width() / 2, 0));
               exit_portal -> setY( platforms_list.back() -> y() - exit_portal -> height());

               // Plop the exit portal onto the new platform
               platforms_list.back() -> addObject(exit_portal);
               m_exitPortalGenerated = true;

               // Return the correct length
               total_pixel_length += platforms_list.back() -> width(); 
            }
            return total_pixel_length;
         }

         // Use a random value for the length of the next platform
         unsigned length = m_minBlockLength + rand() % (m_maxBlockLength - m_minBlockLength); // length of the next platform, in blocks

         // Generate a new platform
         platforms_list.push_back(std::shared_ptr<Platform>(new Platform(e, total_pixel_length, y, length)));
         Platform& p = *platforms_list.back();

         // Add objects to the new platform
         addObjectsToPlatform(p, e);

         // Add a horizontal gap of random length (within the given limits)
         total_pixel_length += length * 16 + (m_minHorizontalGap + rand() % (m_maxHorizontalGap - m_minHorizontalGap));

         // Add a vertical displacement (withing the given limits)
         y += (rand() % 2 ? -1 : 1) * (m_minVerticalGap + rand() % (m_maxVerticalGap - m_minVerticalGap));
      }

      return total_pixel_length; // Done generating new platforms, return the world length
   }

   void PlatformGenerator::addObjectsToPlatform(Platform& p, const Engine& e)
   {
      // Some constants
      const int MaxYounglingClusters = 4;
      const int MinYounglingClusters = 2;
      const int MaxYounglingsInCluster = 3;
      const int MinYounglingsInCluster = 3;

      if(p.x() > m_minBlockLength * 16) // don't add objects to the very first platform
      {
         // Add younglings.
         // Younglings come in groups of 1, 2 or 3.
         // There can be up to 4 groups of younglings on one platform
         // There's at least two groups on each platform.
         // They are positioned randomly.
         int clusters = MinYounglingClusters + rand() % (MaxYounglingClusters - MinYounglingClusters + 1);
         GameObjectPtr next_youngling(new Youngling(e, 0, 0));
         int max_cluster_width = next_youngling -> width() * MaxYounglingsInCluster;
         int yng_x = rand() % (p.width() - max_cluster_width); // X coordinate offset from the start of the platform
         int yng_y = p.y() - next_youngling -> height(); // Y coordinate

         // Place groups one after another, leaving some space between them. 
         // If we exceed the length of the platform, wrap around.
         for(int c = 0; c < clusters; ++c) // Generate younglings for each group
         {
            int yng_count = MinYounglingsInCluster + rand() % (MaxYounglingsInCluster - MinYounglingsInCluster + 1); 
            yng_x = (yng_x + max_cluster_width) % (p.width() - max_cluster_width);

            // Place each youngling in the group
            for(int c = 0; c < yng_count; ++c)
            {
               next_youngling.reset(new Youngling(e, p.x() + yng_x, yng_y));
               p.addObject(GameObjectPtr(next_youngling));
               yng_x += next_youngling -> width();
            }
         }

         // Add meanies.
         // Meanies come in the following combinations:
         // a single Walker
         // a single Jumper
         // Jumper - Walker
         // Jumper - Flyer
         // Walker - Flyer
         int r = rand() % 5;
         switch(r)
         {
         case 0:
            p.addObject(GameObjectPtr(new MeanWalker(e, p.x() + 16, p.y() - 9, p.x(), p.x() + p.width()))); 
            break;

         case 1:
            p.addObject(GameObjectPtr(new MeanJumper(e, p.x() + p.width()/2, p.y() - 10,  p.x(), p.x() + p.width())));
            break;

         case 2:
            p.addObject(GameObjectPtr(new MeanWalker(e, p.x() + 16, p.y() - 9, p.x(), p.x() + p.width()))); 
            p.addObject(GameObjectPtr(new MeanJumper(e, p.x() + p.width()/2, p.y() - 10,  p.x(), p.x() + p.width())));
            break;

         case 3:
            p.addObject(GameObjectPtr(new MeanFlyer(e, p.x() + p.width()/4, p.y() - 32,  p.x(), p.x() + p.width())));
            p.addObject(GameObjectPtr(new MeanWalker(e, p.x() + 16, p.y() - 9, p.x(), p.x() + p.width()))); 
            break;

         case 4:
            p.addObject(GameObjectPtr(new MeanFlyer(e, p.x() + p.width()/4, p.y() - 32,  p.x(), p.x() + p.width())));
            p.addObject(GameObjectPtr(new MeanJumper(e, p.x() + p.width()/2, p.y() - 10,  p.x(), p.x() + p.width())));
            break;
         }

         // Add a heart with 1/3 probability
         if(rand() % 3 == 0)
         {
            p.addObject(GameObjectPtr(new Heart(e, p.x() + rand() % p.width(), p.y() - (16 + rand() % 5))));
         }
      }
   }
}
