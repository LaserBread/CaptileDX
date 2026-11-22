/* LIT SHADER (Source) - Ethan Jones - shader
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

/*%%%LITSHADER: CONSTRUCTOR%%%*/
LitShader::LitShader(const char* v, const char* f) : Shader(v, f) {
    _mLoc = glGetUniformLocation(_vfProgLoc, "uM");
    _vLoc = glGetUniformLocation(_vfProgLoc, "uV");
    _pLoc = glGetUniformLocation(_vfProgLoc, "uP");
    _nLoc = glGetUniformLocation(_vfProgLoc, "uN");

    _useTexLoc = glGetUniformLocation(_vfProgLoc, "useTexture");
    _colLoc = glGetUniformLocation(_vfProgLoc, "uObjectColor");
    _texLoc = glGetUniformLocation(_vfProgLoc, "tex");

    _shineLoc = glGetUniformLocation(_vfProgLoc, "uShininess");
    _ambLoc = glGetUniformLocation(_vfProgLoc, "uLight.ambient");
    _diffLoc = glGetUniformLocation(_vfProgLoc, "uLight.diffuse");
    _specLoc = glGetUniformLocation(_vfProgLoc, "uLight.specular");
    _shineLoc = glGetUniformLocation(_vfProgLoc, "uShininess");
    _lPosLoc = glGetUniformLocation(_vfProgLoc, "uLight.position");
}
/*%%%%%%*/

/*%%%LITSHADER: SEND TEXTURE%%%*/
// Tell the GPU to use a texture.
void LitShader::sendTex(GLuint texID) {
    glUniform1i(_useTexLoc, (texID != 0) ? 1 : 0);

    if (texID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
        glUniform1i(_texLoc, 0);
    }
}
/*%%%%%%*/

/*%%%LITSHADER: SEND COLOR%%%*/
// Send the desired color of an object to the GPU
void LitShader::sendCol(const glm::vec3& color) {
    glUniform3fv(_colLoc, 1, glm::value_ptr(color));
}
/*%%%%%%*/

/*%%%LITSHADER: SEND Normals%%%*/
// Send Normals to the GPU
void LitShader::sendN(const glm::mat3& n) {
    glUniformMatrix3fv(_nLoc, 1, GL_FALSE, glm::value_ptr(n));
};
/*%%%%%%*/

/*%%%LITSHADER: SEND LIGHT%%%*/
// Send a PointLight to the GPU
void LitShader::sendLight(const PointLight& l, const glm::mat4& v) {
    sendLight(l.pos(), l.amb(), l.diff(), l.spec(), v);
}
/*%%%%%%*/

/*%%%LITSHADER: SEND LIGHT%%%*/
void LitShader::sendLight(
    const glm::vec3& pos,
    const glm::vec3& amb,
    const glm::vec3& diff,
    const glm::vec3& spec,
    const glm::mat4& view) {
    glUniform3fv(_ambLoc, 1, glm::value_ptr(amb));
    glUniform3fv(_diffLoc, 1, glm::value_ptr(diff));
    glUniform3fv(_specLoc, 1, glm::value_ptr(spec));

    glm::vec3 lightPosView = glm::vec3(view * glm::vec4(pos, 1.0f));
    glUniform3fv(_lPosLoc, 1, glm::value_ptr(lightPosView));
}
/*%%%%%%*/

/*%%%LITSHADER: SEND SHININESS%%%*/
// Send the shininess value of an object to the GPU.
void LitShader::sendShine(const float s){
    glUniform1f(_shineLoc, s);
}
/*%%%%%%*/