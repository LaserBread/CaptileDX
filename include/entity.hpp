/* ENTITY (Header) - Ethan Jones - entity
 * DESCRIPTION:
 *
 * DEPENDENCIES:
 */

/*======== GUARDS ===========================================================*/
// These are generated on file creation.  Don't change them.
#ifndef ENTITY_H
#define ENTITY_H
/*===========================================================================*/

/*======== HEADERS ==========================================================*/
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "light.hpp"
/*===========================================================================*/

/*======= ENTITY =====================================================*/
// The base class for all objects in game.
class Entity {
   private:
    // std::string name;

   public:
    glm::mat4 modelMatrix{1.0f};

    // Entity(const std::string& name);
    Entity() = default;
    virtual ~Entity() = default;

    virtual void setPosition(const glm::vec3& pos);
    virtual void translate(const glm::vec3& delta);
    virtual void rotate(float degrees, const glm::vec3& axis);
    virtual void scale(const glm::vec3& s);
    virtual void scale(const float s);  // Uniform scale
    virtual void resetTransform();

    virtual void draw(const glm::mat4& view, const glm::mat4& projection, const PointLight& light, const glm::mat4& = glm::mat4{1.0f}) const = 0;

    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
};
/*===========================================================================*/

#endif
