/* CAMERA (Source) - Ethan Jones - camera
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include "camera.hpp"

#include <glm/gtc/type_ptr.hpp>
/*===========================================================================*/

/*======== GLOBALS ==========================================================*/

/*===========================================================================*/

/*======== CAMERA: CONSTRUCTOR ==============================================*/
Camera::Camera(
    glm::vec3 startPos,
    glm::vec3 startDirect,
    glm::vec3 startUp,
    float startSpeed) {
    up = startUp;
    direct = startDirect;
    pos = startPos;
    speed = startSpeed;

    /*-------- INITIALIZE PITCH AND YAW -------------------------------------*/
    pitch = asin(-direct.y);
    yaw = atan2(direct.x, direct.z);
}
/*===========================================================================*/

/*======== CAMERA: CONSTRUCTOR ==============================================*/
Camera::Camera() {
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    direct = glm::vec3(0.0f, 0.0f, -1.0f);
    pos = glm::vec3(0.0f, 0.0f, 3.0f);
    speed = 2.5f;
}
/*===========================================================================*/

/*======== CAMERA: GET POSITION =============================================*/
// Where the camera is
glm::vec3 Camera::getPos(){
    return pos;
}
/*===========================================================================*/

/*======== CAMERA: GET DIRECTION ============================================*/
// The direction the camera is facing.
glm::vec3 Camera::getDirect(){
    return direct;
}
/*===========================================================================*/

/*======== CAMERA: LOOK AT ==================================================*/
glm::mat4 Camera::lookAt() {
    return glm::lookAt(pos, pos + direct, up);
}
/*===========================================================================*/

/*======== CAMERA: MOVE TO ==================================================*/

/*===========================================================================*/

/*======== CAMERA: MOVE BY ==================================================*/
// Relative movement based on the camera's direction. Can be used to emulate
// fly/noclip/swim controls in early FPS games.

/*======== CAMERA: ROTATE ===================================================*/
void Camera::rotate(float yawOffset, float pitchOffset) {
    pitch += pitchOffset;
    yaw += yawOffset;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    direct.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direct.y = sin(glm::radians(pitch));
    direct.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direct = glm::normalize(direct);
}
/*===========================================================================*/
