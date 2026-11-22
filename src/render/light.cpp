/* LIGHT (Source) - Ethan Jones - light
 * DESCRIPTION:
 *
*/

/*======== HEADERS ==========================================================*/
#include "light.hpp"
/*===========================================================================*/


/*======== GLOBALS ==========================================================*/

/*===========================================================================*/


// Parameterized constructor
PointLight::PointLight(
    const glm::vec3& position,
    const glm::vec3& ambient,
    const glm::vec3& diffuse,
    const glm::vec3& specular)
    : _pos(position), _amb(ambient), _diff(diffuse), _spec(specular) {}

// Translation
void PointLight::translate(const glm::vec3& delta) {
    _pos += delta;
}

// Position Setters and Getters
void PointLight::setPos(const glm::vec3& position) {
    _pos = position;
}

glm::vec3 PointLight::pos() const{
    return _pos;
}

// Ambient Setters and Getters
void PointLight::setAmb(const glm::vec3& ambient) {
    _amb = ambient;
}

glm::vec3 PointLight::amb() const{
    return _amb;
}

// Diffuse Setters and Getters
void PointLight::setDiff(const glm::vec3& diffuse) {
    _diff = diffuse;
}

glm::vec3 PointLight::diff() const{
    return _diff;
}

// Specular Setters and Getters
void PointLight::setSpec(const glm::vec3& specular) {
    _spec = specular;
}

glm::vec3 PointLight::spec() const{
    return _spec;
}