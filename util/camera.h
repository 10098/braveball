/// @file util/camera.h
/// @author grigioryj

#ifndef CAMERA_H
#define CAMERA_H

#include "graphics/context.h"
#include "game/game_object.h"
#include "game/engine.h"

namespace util
{
   /// A camera which can "follow" a game object
   class Camera
   {
   public:
      Camera();

      /// Locks the camera on a game object
      /// @param o The object to follow
      /// @param min_x_distance The minimum distance from the object to the left edge of the screen
      /// @param max_x_distance The maximum distance from the object to the left edge of the screen
      /// @param min_y_distance The minimum distance from the object to the top edge of the screen
      /// @param max_y_distance The maximum distance from the object to the top edge of the screen
      void followObject(const game::GameObject* o, int min_x_distance, int max_x_distance, int min_y_distance, int max_y_distance);

      /// Stop following the object that is currently being followed (if any)
      void unfollow();

      /// Update the camera position
      void update(game::Engine&);

      /// Get camera's x coordinate
      int x() const;

      /// Get camera's y coordinate
      int y() const;

      /// @return a pointer to the object that is being followed by this camera or null if no object is currently being followed
      const game::GameObject* getFollowedObject() const;

   private:
      const game::GameObject* m_obj; /// The object that is being followed
      int m_x; /// Camera's x coordinate
      int m_y; /// Camera's y coordinate
      int m_minXDistance; /// Min distance from followed object to the left edge of the screen
      int m_maxXDistance; /// Max distance from followed object to the left edge of the screen
      int m_minYDistance; /// Min distance from followed object to the top edge of the screen
      int m_maxYDistance; /// Max distance from followed object to the top edge of the screen
   };
}

#endif // CAMERA_H