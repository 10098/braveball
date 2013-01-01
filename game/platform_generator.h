/// @file game/platform_generator.h
/// @author grigoryj

#include <memory>
#include <list>

#ifndef PLATFORM_GENERATOR_H
#define PLATFORM_GENERATOR_H

namespace game
{
   class Platform;
   class Engine;

   /// Generates platforms and populates them with
   /// game objects in a random manner.
   class PlatformGenerator
   {
   public:
      /// Create a new platform generator and set its parameters
      ///
      /// @param min_blocks minimal length of a platform in blocks
      /// @param max_blocks maximal length of a platform in blocks
      /// @param min_h_gap minimal horizontal gap between platforms
      /// @param max_h_gap maximal horizontal gap between platforms
      /// @param min_v_gap minimal vectical gap between platforms
      /// @param max_v_gap maximal vectical gap between platforms
      /// @param default_y default y coordinate for platform
      /// @param max_x Won't not generate platforms past this X coordinate
      PlatformGenerator(unsigned min_blocks, unsigned max_blocks,
                        unsigned min_h_gap,  unsigned max_h_gap,
                        unsigned min_v_gap,  unsigned max_v_gap,
                        int default_y, 
                        unsigned max_x);
      
      /// Generate a new set of platforms
      ///
      /// Platforms will be appended to the given list. Generation will start with the x-coordinate
      /// equal to that of the last platform in the list (or 0, if the list is empty). Generation will
      /// continue until the total length of all the platforms and horizontal gaps exceeds the given limit (or the internal length limit).
      /// If the internal length limit is hit, a platform with the exit portal will be appended automatically.
      /// @param platform_list the list of platforms to append to
      /// @param length threshold value for total length of the generated platforms and horizontal gaps between them 
      /// @param e Engine context
      /// @return the new total length of the world
      unsigned generatePlatforms(std::list<std::shared_ptr<Platform> >& platform_list, unsigned length, const Engine& e);

      /// X coordinate of the rightmost edge of the world
      int edgeCoord() const;

   private:
      /// Adds game objects to a platform
      ///
      /// @param p the platform to add objects to
      /// @param e engine context
      void addObjectsToPlatform(Platform& p, const Engine& e);

      int m_minBlockLength;   /// Minimal platform length in blocks
      int m_maxBlockLength;   /// Max. platform length in blocks
      int m_minHorizontalGap; /// Minimal horizontal gap between platforms
      int m_maxHorizontalGap; /// Maximal horizontal gap
      int m_minVerticalGap;   /// Minimal vertical gap
      int m_maxVerticalGap;   /// Maximal vertical gap
      int m_defaultYCoord;    /// Default Y coordinate
      unsigned m_maxX;        /// Edge of the world
      bool m_exitPortalGenerated; /// Indicates if an exit portal has already been generated
   };
}

#endif // PLATFORM_GENERATOR