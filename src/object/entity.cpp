/* ENTITY (Source) - Ethan Jones - entity
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include "entity.hpp"

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

/*======== GLOBALS ==========================================================*/

/*===========================================================================*/

void Entity::setPosition(const glm::vec3& pos) {
    modelMatrix[3] = glm::vec4(pos, 1.0f);
}

void Entity::translate(const glm::vec3& delta) {
    modelMatrix = glm::translate(modelMatrix, delta);
}

void Entity::rotate(float degrees, const glm::vec3& axis) {
    modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), axis);
}

void Entity::scale(const glm::vec3& s) {
    modelMatrix = glm::scale(modelMatrix, s);
}

void Entity::scale(float s) {
    modelMatrix = glm::scale(modelMatrix, glm::vec3(s));
}

void Entity::resetTransform() {
    modelMatrix = glm::mat4(1.0f);
}
