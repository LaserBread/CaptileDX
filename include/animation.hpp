/* ANIMATION (Header) - Ethan Jones - HW2
 * DESCRIPTION:
 *
 * DEPENDENCIES:
 */

/*======== GUARDS ===========================================================*/
// These are generated on file creation.  Don't change them.
#ifndef ANIMATION_H
#define ANIMATION_H
/*===========================================================================*/

#include <glm/gtc/type_ptr.hpp>

/*======== OTHER FUNCTION PROTOTYPES ========================================*/
// An animation that orbits the mesh around the screen. Results in an ugly
// stretching I don't know how to fix.
glm::mat4 innerStarship(float t);
// An animation that orbits the mesh around the screen. Results in an ugly
// stretching I don't know how to fix.
glm::mat4 outerStarship(float t);
// An animation that wobbles the mesh up and down.
glm::mat4 wobble(float t);

glm::mat4 maxwell_anim(float t);

glm::mat4 teapot_body(float t);
glm::mat4 teapot_lid(float t);
/*===========================================================================*/

#endif
