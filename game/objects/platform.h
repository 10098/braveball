/// @file game/objects/platform.h
/// @author grigoryj

#ifndef PLATFORM_H
#define PLATFORM_H

#include "game/game_object.h"
#include "game/engine.h"

namespace game
{
   /// Platform game object
   ///
   /// The platform is a bit special, because it acts
   /// as a container for game object that stand on that
   /// platform (except for the player).
   /// This allows us to do fast collision detection while
   /// not resorting to any techniques more advanced than
   /// dumb brute force. The price to pay is gameplay flexibility,
   /// for example, implementing objects which can transition between
   /// platforms (e.g. projectiles) becomes tricky.
   class Platform : public GameObject
   {
      typedef std::list<std::shared_ptr<GameObject> > ObjectContainer;
      typedef ObjectContainer::iterator iterator;

   public:
      /// Create a new platform
      /// @param e engine context
      /// @param x x coordinate of the platform
      /// @param y y coordinate of the platform
      /// @param l platform length in "blocks"
      Platform(const Engine& e, int x, int y, int l);

      virtual void update(Engine&);
      virtual void draw(graphics::SpriteBatch& b);

      /// Add a new game object to the platform
      void addObject(const ObjectContainer::value_type& o) { m_objects.push_back(o); }

      /// @return Iterator pointing to the first game object that was added to this platform
      iterator objectsBegin(){ return m_objects.begin(); }

      /// @return end iterator
      iterator objectsEnd(){ return m_objects.end(); }

   private:
      virtual void collide(GameObject& o){}

      static const int m_blockSize = 16; // Block width and height (in pixels)

      const graphics::Animation m_platformStart;   /// Platform left edge sprite
      const graphics::Animation m_platformMiddle;  /// Platform `middle' sprite
      const graphics::Animation m_platformEnd;     /// Platform right edge sprite

      int m_blocksNumber;  /// How many blocks the platform contains
      ObjectContainer m_objects; /// Game objects standing on this platform
   };
}

#endif // PLATFORM_H