/* LIGHT (Header) - Ethan Jones - light
 * DESCRIPTION:
 *
 * DEPENDENCIES:
 */

/*======== GUARDS ===========================================================*/
// These are generated on file creation.  Don't change them.
#ifndef LIGHT_H
#define LIGHT_H
/*===========================================================================*/

/*======== HEADERS ==========================================================*/
#include <glm/gtc/type_ptr.hpp>
/*===========================================================================*/

/*======= POINT LIGHT =====================================================*/
// A light that shines from a point.
class PointLight {
    private:
    /*-------- MEMBER VARIABLES ---------------------------------------------*/
    glm::vec3 _pos{0.0f, 0.0f, 0.0f};
    glm::vec3 _amb{0.15f, 0.15f, 0.15f};
    glm::vec3 _diff{0.8f, 0.8f, 0.8f};
    glm::vec3 _spec{1.0f, 1.0f, 1.0f};
    /*-----------------------------------------------------------------------*/

   public:
    PointLight() = default;
    PointLight(
        const glm::vec3& position,
        const glm::vec3& ambient,
        const glm::vec3& diffuse,
        const glm::vec3& specular);

    void translate(const glm::vec3& delta);
    void setPos(const glm::vec3& position);
    glm::vec3 pos() const;

    void setAmb(const glm::vec3& ambient);
    glm::vec3 amb() const;

    void setDiff(const glm::vec3& diffuse);
    glm::vec3 diff() const;

    void setSpec(const glm::vec3& specular);
    glm::vec3 spec() const;
};
/*===========================================================================*/

/*======== OTHER FUNCTION PROTOTYPES ========================================*/

/*===========================================================================*/

#endif
