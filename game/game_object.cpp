/// @file game/game_object.cpp
/// @author grigoryj

#include "game/game_object.h"

namespace game
{
   bool GameObject::addTag(const std::string& tag)
   {
      return m_tags.insert(tag).second;
   }

   bool GameObject::removeTag(const std::string& tag)
   {
      return m_tags.erase(tag) > 0;
   }

   bool GameObject::hasTag(const std::string& tag)
   {
      return m_tags.find(tag) != m_tags.end();
   }

   void GameObject::collideWith(GameObject& o, bool saw)
   {
      collide(o);
      if(!saw) o.collideWith(*this, true);
   }

   void GameObject::setWidth(int w)
   {
      m_boundingRect.w = w;
   }

   void GameObject::setHeight(int h)
   {
      m_boundingRect.h = h;
   }
}
