/* ANIMATION (Source) - Ethan Jones - HW2
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include "animation.hpp"

#include <GL/glew.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <glm/glm.hpp>  // Basic include for GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "inits.h"
/*===========================================================================*/

/*======== WOBBLE ===========================================================*/
// An animation that wobbles the mesh up and down.
glm::mat4 wobble(float t) {
    float yOffset = 0.5f * sin(t * glm::pi<float>());
    glm::mat4 mv = glm::mat4(1.0f);
    return glm::translate(mv, glm::vec3(0.0f, yOffset, 0.0f));
}
/*===========================================================================*/

/*======== OUTER STARSHIP ===================================================*/
// An animation that orbits the mesh around the screen. Results in an ugly
// stretching I don't know how to fix.
glm::mat4 outerStarship(float t) {
    float yOffset = (1.0f - (0.125 / 2)) * sin(t * 0.5 * glm::pi<float>());
    // Compensate for the aspect ratio.
    float xOffset = (1.0f - (0.125 / 2)) * cos(t * 0.5 * glm::pi<float>()) * aspect;

    glm::mat4 mv = glm::mat4(1.0f);
    mv = glm::translate(mv, glm::vec3(xOffset, yOffset, 0.0f));
    // Rotate about the Z-Axis
    return glm::rotate(mv, t * 0.5f * glm::pi<float>(), glm::vec3(0, 0, 1));
}
/*===========================================================================*/

/*======== INNER STARSHIP ===================================================*/
// An animation that orbits the mesh around the screen. Results in an ugly
// stretching I don't know how to fix.
glm::mat4 innerStarship(float t) {
    // Second verse, same as the first. But we negate t to make it go the other
    // way.
    float yOffset = 0.7f * sin(-t * 0.5 * glm::pi<float>());
    float xOffset = 0.7f * cos(-t * 0.5 * glm::pi<float>()) * aspect;

    glm::mat4 mv = glm::mat4(1.0f);
    mv = glm::translate(mv, glm::vec3(xOffset, yOffset, 0.0f));
    return glm::rotate(mv, (-t * 0.5f * glm::pi<float>()) - glm::pi<float>(), glm::vec3(0, 0, 1));
}
/*===========================================================================*/


/*======== MAXWELL BOUNCE ===================================================*/
// The little bounce animation that the cat model does.
glm::mat4 maxwell_rotate(float t) {
    float progress;
    float easedProgress;  // Quadratic ease-in

    float angle;
    glm::mat4 rotation;

    // Over the course of 1 second, this animation is divided into 4 parts
    switch (int(fmod(t, 1.0f) * 4.0f)) {
        // Bounce up
        case 0:
            progress = std::clamp((float)fmod(t, 0.25f) * 4.0f, 0.0f, 1.0f);
            easedProgress = progress * progress;  // Quadratic ease-in

            angle = glm::radians(30.0f) * easedProgress;
            break;
        // Bounce back
        case 1:
            progress = std::clamp(1.0f - (float)fmod(t, 0.25f) * 4.0f, 0.0f, 1.0f);
            easedProgress = progress * progress;  // Quadratic ease-in

            angle = glm::radians(30.0f) * easedProgress;
            break;
        // Bounce up the other way
        case 2:
            progress = std::clamp((float)fmod(t, 0.25f) * 4.0f, 0.0f, 1.0f);
            easedProgress = progress * progress;  // Quadratic ease-in

            angle = glm::radians(-30.0f) * easedProgress;
            break;
        // Bounce back
        case 3:
            progress = std::clamp(1.0f - (float)fmod(t, 0.25f) * 4.0f, 0.0f, 1.0f);
            easedProgress = progress * progress;  // Quadratic ease-in

            angle = glm::radians(-30.0f) * easedProgress;
            break;
    }
    // Apply the rotation
    rotation = glm::rotate(
        glm::mat4(1.0f),
        angle,
        glm::vec3(1.0f, 0.0f, 0.0f)  // Rotate around Y
    );
    rotation = glm::translate(rotation, glm::vec3(0.0f, 10.0f * easedProgress, 0.0f));
    return (rotation);
}
/*===========================================================================*/

/*======== MAXWELL ANIMATION ================================================*/
// Moves Maxwell out to the foreground so we can see him, then applies the
// bounce.
glm::mat4 maxwell_anim(float t) {
    glm::mat4 model = glm::mat4(1.0f);  // Always start with the identiy matrix
    model = glm::translate(model, glm::vec3(0.0f, -10.0f, -50.0f));
    return (model * glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.0f, 1.0f, 0.0f)) * maxwell_rotate(t));
}
/*===========================================================================*/

/*======== TEAPOT SPIN ======================================================*/
// Spins the teapot around and revolves it around Maxwell.
glm::mat4 teapot_spin(float t) {
    return (
        // Move to Maxwell
        glm::translate(idMatrix, glm::vec3(0.0f, -10.0f, -50.0f)) *
        // Revolve
        glm::rotate(idMatrix, -t * glm::pi<float>() / 6.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
        // Offset
        glm::translate(idMatrix, glm::vec3(20.0f, 5.0f, 0.0f)) *
        // Spin
        glm::rotate(idMatrix, t * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) *
        // Grow
        glm::scale(idMatrix, glm::vec3(2.0f)));
}
/*===========================================================================*/

/*======== TEAPOT BODY ======================================================*/
// Animates the body of the teapot lid moving up and down.
glm::mat4 teapot_body(float t) {
    return (
        glm::translate(idMatrix, glm::vec3(0.0f, 2.5f * cos(t * glm::pi<float>()) - 2.5f, 0.0f)) *
        teapot_spin(t));
}
/*===========================================================================*/

/*======== TEAPOT LID =======================================================*/
// Animate the teapot lid moving up and down.
glm::mat4 teapot_lid(float t) {
    return (
        glm::translate(idMatrix, glm::vec3(0.0f, 2.5f - 2.5f * cos(t * glm::pi<float>()), 0.0f)) *
        teapot_spin(t));
}
/*===========================================================================*/
