/* CAMERA (Header) - Ethan Jones - camera
 * DESCRIPTION:
 *
 * DEPENDENCIES:
 */

/*======== GUARDS ===========================================================*/
// These are generated on file creation.  Don't change them.
#ifndef CAMERA_H
#define CAMERA_H
/*===========================================================================*/

/*======== HEADERS ==========================================================*/
#include <glm/gtc/type_ptr.hpp>
/*===========================================================================*/

#define CAM_MOVEREL_ABS 0; // Move By goes by absolute positions.
#define CAM_MOVEREL_HORI 1;
#define CAM_MOVEREL_VERT 2;
/*======= CAMERA =====================================================*/
class Camera {
   public:
    /*-------- MEMBER VARIABLES ---------------------------------------------*/
    glm::vec3 pos;     // Where the camera is
    glm::vec3 direct;  // Which direction the camera is pointed
    glm::vec3 up;      // Which direction is up relative to the camera
    float speed;       // The speed at which the camera travels
    float yaw = -90.f; // default down the negative Z-axis
    float pitch = 0.0f; // default straight ahead
    /*-----------------------------------------------------------------------*/

   public:
    /*-------- MEMBER FUNCTION PROTOTYPES -----------------------------------*/
    Camera(
        glm::vec3 startPos,
        glm::vec3 startDirect,
        glm::vec3 startUp,
        float startSpeed
    );
    Camera();

    glm::mat4 lookAt();  // Constructs a view matrix for the camera.

    glm::vec3 getPos();     // Returns the position as a vec3
    glm::vec3 getDirect();  // Returs the direction as a normalized vec3
    glm::vec3 getUp();      // Returns the up direction

    void setSpeed(float _speed);
    void rotate(float yaw, float pitch);
    void rotate(glm::vec3 newDir);
    /*-----------------------------------------------------------------------*/
};
/*===========================================================================*/

/*======== OTHER FUNCTION PROTOTYPES ========================================*/

/*===========================================================================*/

#endif
