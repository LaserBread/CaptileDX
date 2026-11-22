/* GOURAUD SHADER (Source) - Ethan Jones - shader
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include "shader.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>  // Basic include for GLM
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "inits.h"
#include "light.hpp"
/*===========================================================================*/

/*%%%GOURAUDSHADER: CONSTRUCTOR%%%*/
// New GouraudShader
GouraudShader::GouraudShader() : LitShader(GOURAUD_V, GOURAUD_F) {};
/*%%%%%%*/

/*%%%GOURAUDSHADER: USE SHADER%%%*/
void GouraudShader::use(const glm::mat4& proj, const glm::mat4& view) {
    glUseProgram(_vfProgLoc);
    sendP(proj);
    sendV(view);
}
/*%%%%%%*/