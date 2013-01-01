/// @file game/game_object.h
/// @author grigoryj

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "graphics/misc.h"
#include "graphics/sprite_batch.h"
#include "game/engine.h"
#include <unordered_set>
#include <memory>

namespace game
{
   /// Base class for all the game objects.
   class GameObject
   {
   public:
      /// Create a new game object
      ///
      /// @param x The x coordinate of the bounding box
      /// @param y The y coordinate of the bounding box
      /// @param width Width of the bounding box
      /// @param height Height of the bounding box
      GameObject(int x, int y, int width = 0, int height = 0):m_boundingRect(x, y, width, height){}

      /// @return the x coordinate
      int x() const{ return m_boundingRect.tl.x; }

      /// @return the y coordinate
      int y() const{ return m_boundingRect.tl.y; }

      /// @return Width of the bounding box
      int width() const { return m_boundingRect.w; }
      
      /// @return Height of the bounding box
      int height() const { return m_boundingRect.h; }

      /// @return The bounding box of the object
      const graphics::AlignedQuad& boundingBox() const { return m_boundingRect; }
      
      /// Set the x position
      void setX(int x){ m_boundingRect.tl.x = x; }

      /// Set the y position
      void setY(int y){ m_boundingRect.tl.y = y; }

      /// Updates the internal state of the object.
      ///
      /// Child classes should implement the necessary
      /// updates her (e.g. move, die, etc.). This method
      /// is called every frame.
      /// @param ctx The engine context
      virtual void update(Engine& ctx) = 0;

      /// Render the object.
      ///
      /// Child classes should implement drawing the object
      /// on the screen in this method. The method is called
      /// every frame by the engine.
      /// @param b The sprite batch to use.
      virtual void draw(graphics::SpriteBatch& b) = 0;

      /// Wrapper around the collide() pure virtual method
      void collideWith(GameObject& o, bool saw = false);


      /// Adds a `tag' to the object. Adding the same tag more than one time will have no effect.
      /// @param tag the tag to be added. 
      /// @return true if the object didn't previously have that tag
      bool addTag(const std::string& tag);

      /// Removes a tag. Has no effect if the object previously didn't have that tag
      /// @param tag the tag to be removed
      /// @return true if the tag was removed, false if the object didn't have that tag
      bool removeTag(const std::string& tag);

      /// @param tag
      /// @return true if the object has the given tag, false otherwise
      bool hasTag(const std::string& tag);

   protected:
      /// @param w the new width
      void setWidth(int w);

      /// @param h the new height
      void setHeight(int h);

   private:
      /// Handle collision
      ///
      /// Called by the engine when the object collides with 
      /// another object. 
      virtual void collide(GameObject& o) = 0;

      graphics::AlignedQuad m_boundingRect; /// The bounding box of the game object
      std::unordered_set<std::string> m_tags; /// Set of tags
   };

   typedef std::shared_ptr<GameObject> GameObjectPtr;
}

#endif // GAME_OBJECT_H
