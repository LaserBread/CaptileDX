/* TEXTURE (Source) - Ethan Jones - texture
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include "texture.hpp"

#include <GL/glew.h>
#include <soil2/SOIL2.h>

#include <glm/glm.hpp>
#include <stdexcept>
/*===========================================================================*/

/*======== GLOBALS ==========================================================*/

/*===========================================================================*/

Texture::Texture(const std::string& path) {
    _path = path;
    _texID = SOIL_load_OGL_texture(
        path.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
    if (_texID == 0) throw std::runtime_error("Couldn't load the texture at " + path);
}

Texture::~Texture() {
    glDeleteTextures(1, &_texID);
    _texID = 0;
}

void Texture::updateTex() {
    glBindTexture(GL_TEXTURE_2D, _texID);  // required to set params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapModeS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapModeT);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(_borderCol));
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
}

// Validate a wrap mode
void validate(int x, char t) {
    if (!(
            x == GL_REPEAT ||
            x == GL_MIRRORED_REPEAT ||
            x == GL_CLAMP_TO_EDGE ||
            x == GL_CLAMP_TO_BORDER))
        throw std::invalid_argument(
            "Argument " + std::to_string(t) + " (" +
            std::to_string(x) +
            ") does not represent a valid OpenGL wrap mode!");
}

/*%%%TEXTURE: SET BOTH WRAP MODES%%%*/
// Sets the wrap mode for the S and T individually.
// @throws invalid_argument if not a valid wrap mode.
void Texture::setWrapMode(const int s, const int t) {
    validate(s, 's');
    validate(t, 't');

    _wrapModeS = s;
    _wrapModeT = t;
}

/*%%%TEXTURE: SET WRAP MODES UNIFORMALLY%%%*/
// Sets the wrap mode for the S and T uniformally.
// @throws invalid_argument if not a valid wrap mode.
void Texture::setWrapMode(const int m) {
    validate(m, 'm');

    _wrapModeS = m;
    _wrapModeT = m;
}

/*%%%TEXTURE: SET WRAP MODE FOR S DIRECTION%%%*/
// Sets the wrap mode for the S direction.
// @throws invalid_argument if not a valid wrap mode.
void Texture::setWrapModeS(const int s) {
    validate(s, 's');

    _wrapModeS = s;
}
/*%%%%%%*/

/*%%%TEXTURE: SET WRAP MODE FOR T DIRECTION%%%*/
// Sets the wrap mode for the T direction.
// @throws invalid_argument if not a valid wrap mode.
void Texture::setWrapModeT(const int t) {
    validate(t, 't');

    _wrapModeT = t;
}
/*%%%%%%*/

/*%%%TEXTURE: SET BORDER COLOR OPAQUE%%%*/
void Texture::setBorderCol(const glm::vec3& color) {
    _borderCol = glm::vec4(color, 1.0f);
}

void Texture::setBorderCol(const glm::vec4& color) {
    _borderCol = color;
}

void Texture::setBorderCol(const float r, const float g, const float b) {
    _borderCol = glm::vec4(r, g, b, 1.0f);
}

void Texture::setBorderCol(const float r, const float g, const float b, const float a) {
    _borderCol = glm::vec4(r, g, b, a);
}
/*%%%%%%*/

/*%%%TEXTURE: GET PATH%%%*/
// Gets the path for this texture.
std::string Texture::path() const {
    return _path;
}
/*%%%%%%*/

/*%%%TEXTURE: GET COLOR%%%*/
// Gets the border color for border clamp mode
glm::vec4 Texture::borderCol() const {
    return _borderCol;
}
/*%%%%%%*/

/*%%%TEXTURE: GET TEXTURE ID%%%*/
// Gets the texture ID
GLuint Texture::texID() const {
    return _texID;
}
/*%%%%%%*/

/*%%%TEXTURE: GET WRAP MODE S%%%*/
// Gets the wrap mode for the S direction
int Texture::wrapModeS() const {
    return _wrapModeS;
}
/*%%%%%%*/

/*%%%TEXTURE: GET WRAP MODE T%%%*/
// Gets the wrap mode for the T direction
int Texture::wrapModeT() const {
    return _wrapModeT;
}
/*%%%%%%*/