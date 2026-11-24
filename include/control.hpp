/* CONTROL (Header) - Ethan Jones - re
 * DESCRIPTION:
 *
 * DEPENDENCIES:
*/

/*======== GUARDS ===========================================================*/
//These are generated on file creation.  Don't change them.
#ifndef CONTROL_H
#define CONTROL_H
/*===========================================================================*/


/*======== HEADERS ==========================================================*/
#include <memory>
#include "shader.hpp"

struct GLFWwindow;
class Camera;
/*===========================================================================*/
extern bool paused;
extern bool pickedUpLight;

/*======= CONTROL =====================================================*/
//Setup Controls
void setControls(GLFWwindow* window);
void setCamera(Camera* inCam);
void frameDispatch(GLFWwindow* window, float delta);
double getTime();
/*===========================================================================*/


/*======== OTHER FUNCTION PROTOTYPES ========================================*/

/*===========================================================================*/

#endif
