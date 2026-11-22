/* INITS (Header) - Ethan Jones - %%PROJECTNAME%%
 * DESCRIPTION:
 *
 * DEPENDENCIES:
 */

/*======== GUARDS ===========================================================*/
// These are generated on file creation.  Don't change them.
#ifndef INITS_H
#define INITS_H
/*===========================================================================*/

#include "shader.hpp"

// Forward declare to stop the compiler from bitching. This is 45 minutes of my
// life.
struct GLFWwindow;

#define idMatrix glm::mat4(1.0f)

/*======== SHADER DATA LOCATIONS ============================================*/
// If I was smart, the shaders would be their own objects, but I dumb and
// already went crazy with the meshes, so dw.
extern GLint mLoc;  // Stores the location of GL's model-space matrix.
extern GLint vLoc;  // Stores the location of GL's view-space matrix.
extern GLint pLoc;  // Stores the location of GL's projection-space matrix.
/*===========================================================================*/


/*======== VARIABLES ========================================================*/
extern glm::mat4 proj;  // The projection matrix of the camera
extern int windowWd;    // Current window width
extern int windowHt;    // Current window height
extern float aspect;    // Window aspect ratio
extern float delta;
extern float lastFrame;
extern float lastX;
extern float lastY;
extern bool firstMouse;
const float mouseSensitivity = 0.1f;
/*===========================================================================*/

/*======== FUNCTIONS ========================================================*/
int startGlew();  // Starts GLEW and reports the results.
std::string loadShaderSource(const char* filePath);
GLuint buildShaderProgram(const char* vertPath, const char* fragPath);
void whenWindowResized(GLFWwindow* window, int width, int height);
/*===========================================================================*/

#endif
