/* SHADER (Source) - Ethan Jones - shader
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include "shader.hpp"

#include <GL/glew.h>

#include <fstream>
#include <glm/glm.hpp>  // Basic include for GLM
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

#include "inits.h"
#include "texture.hpp"
#include "light.hpp"
/*===========================================================================*/

/*======== SHADER: CONSTRUCTOR ==============================================*/
// Compiles shaders specified into a shader.
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // We need create our shader programs and save the ID number for each
    _vProgLoc = glCreateShader(GL_VERTEX_SHADER);
    _fProgLoc = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderStr = loadShaderSource(vertexPath);
    std::string fragShaderStr = loadShaderSource(fragmentPath);

    // We need to convert our strings into c-strings
    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    // We need to load the shader source code into the shader program itself
    glShaderSource(_vProgLoc, 1, &vertShaderSrc, NULL);
    glShaderSource(_fProgLoc, 1, &fragShaderSrc, NULL);

    // We need to compile the shader source code
    glCompileShader(_vProgLoc);
    glCompileShader(_fProgLoc);

    // We need to create our program and attach our shader
    _vfProgLoc = glCreateProgram();
    glAttachShader(_vfProgLoc, _vProgLoc);
    glAttachShader(_vfProgLoc, _fProgLoc);

    // We need to link the compiled shaders into a single program
    glLinkProgram(_vfProgLoc);

    // Clean up individual shaders after linking
    glDeleteShader(_vProgLoc);
    glDeleteShader(_fProgLoc);
    _vProgLoc = 0;
    _fProgLoc = 0;

    _mLoc = glGetUniformLocation(_vfProgLoc, "m");
    _vLoc = glGetUniformLocation(_vfProgLoc, "v");
    _pLoc = glGetUniformLocation(_vfProgLoc, "p");
}
/*===========================================================================*/

/*======== SHADER: DESTRUCTOR ===============================================*/
Shader::~Shader() {
    if (_vfProgLoc != 0) {
        glDeleteProgram(_vfProgLoc);
        _vfProgLoc = 0;  // Prevent accidental reuse or double deletion
    }
}
/*===========================================================================*/

/*======== SHADER: GET MODEL MATRIX LOCATION ================================*/
// Returns the model matrix
GLint Shader::mLoc() {
    return _mLoc;
}
/*===========================================================================*/

/*======== SHADER: GET VIEW MATRIX LOCATION =================================*/
// Returns the view matrix
GLint Shader::vLoc() {
    return _vLoc;
}
/*===========================================================================*/

/*======== SHADER: GET PROJECTION MATRIX LOCATION ===========================*/
// Returns the projection matrix
GLint Shader::pLoc() {
    return _pLoc;
}
/*===========================================================================*/

/*======== SHADER: GET PROGRAM LOCATION =====================================*/
// Returns the program location
GLuint Shader::progLoc() {
    return _vfProgLoc;
}
/*===========================================================================*/

/*======== SHADER: SEND MODEL MATRIX ========================================*/
// Send the model matrix to the GPU for this shader.
void Shader::sendM(const glm::mat4& m) {
    glUniformMatrix4fv(_mLoc, 1, GL_FALSE, glm::value_ptr(m));
}
/*===========================================================================*/

/*======== SHADER: SEND VIEW MATRIX =========================================*/
// Send the view matrix to the GPU for this shader.
void Shader::sendV(const glm::mat4& v) {
    glUniformMatrix4fv(_vLoc, 1, GL_FALSE, glm::value_ptr(v));
}
/*===========================================================================*/

/*======== SHADER: SEND PROJECTION MATRIX ===================================*/
// Send the projection matrix to the GPU for this shader.
void Shader::sendP(const glm::mat4& p) {
    glUniformMatrix4fv(_pLoc, 1, GL_FALSE, glm::value_ptr(p));
}
/*===========================================================================*/
