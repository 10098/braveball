/// @file util/camera.cpp
/// @author grigoryj

#include "util/camera.h"

namespace util
{
   Camera::Camera():m_x(0), m_y(0), m_obj(0){}
   
   void Camera::followObject(const game::GameObject* o, int min_x_distance, int max_x_distance, int min_y_distance, int max_y_distance)
   {
      m_obj = o;
      m_minXDistance = min_x_distance;
      m_maxXDistance = max_x_distance;
      m_minYDistance = min_y_distance;
      m_maxYDistance = max_y_distance;
   }

   void Camera::unfollow()
   {
      m_obj = 0;
   }

   // Helper function for Camera::update
   // Given an object's coordinate, a camera's coordinate and 
   // bounds on the object's coordinate, generates a new value for the camera's coordinate.
   inline int getNewCoord(int obj_coord, // object's coordinate
                          int cam_coord, // camera's coordinate
                          int min_limit, // minimal difference between the object's and camera's coordinates
                          int max_limit) // maximal difference between the object's and camera's coordinates
   {
      int diff = obj_coord - cam_coord;

      // return a new coordinate for camera so that the difference between the new camera coord and object coord 
      // is between min_limit and max_limit
      if(diff < min_limit)
         return obj_coord - min_limit;
      else if(diff > max_limit)
         return obj_coord - max_limit;
      else
         return cam_coord;
   }

   void Camera::update(game::Engine& ctx)
   {
      if(m_obj) // make sure there is an object to follow
      {
         // Obtain new values for X and Y camera coordinates
         int new_x = getNewCoord(m_obj -> x(), m_x, m_minXDistance, m_maxXDistance);
         int new_y = getNewCoord(m_obj -> y(), m_y, m_minYDistance, m_maxYDistance);

         // Translate the camera by needed amount
         ctx.graphics().translate(m_x - new_x, m_y - new_y);

         // Update coordinates
         m_x = new_x;
         m_y = new_y;
      }
   }

   int Camera::x() const
   {
      return m_x;
   }

   int Camera::y() const
   {
      return m_y;
   }

   const game::GameObject* Camera::getFollowedObject() const
   {
      return m_obj;
   }
}