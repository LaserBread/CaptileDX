/* FLAT SHADER (Source) - Ethan Jones - shader
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

/*%%%FLATSHADER: CONSTRUCTOR%%%*/
// New FlatShader
FlatShader::FlatShader() : LitShader(FLAT_V, FLAT_F) {};
/*%%%%%%*/

/*%%%FLATSHADER: USE SHADER%%%*/
void FlatShader::use(const glm::mat4& proj, const glm::mat4& view) {
    glUseProgram(_vfProgLoc);
    sendP(proj);
    sendV(view);
}
/*%%%%%%*/