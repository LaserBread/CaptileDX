/* TEXTURE (Header) - Ethan Jones - texture
 * DESCRIPTION: Extends the RAII model to textures

 *
 * DEPENDENCIES:
*/

/*======== GUARDS ===========================================================*/
// These are generated on file creation.  Don't change them.
#ifndef TEXTURE_H
#define TEXTURE_H
/*===========================================================================*/

/*======== HEADERS ==========================================================*/
#include <GL/glew.h>
#include <memory>

#include <glm/glm.hpp>
#include <string>
#include "color.h"
/*===========================================================================*/

class LitShader;

/*======= TEXTURE =====================================================*/
class Texture {
    /*-------- MEMBER VARIABLES ---------------------------------------------*/
   private:
    std::string _path;
    GLuint _texID;
    int _wrapModeS = GL_CLAMP_TO_EDGE;
    int _wrapModeT = GL_CLAMP_TO_EDGE;

    glm::vec4 _borderCol = glm::vec4(HEX2GL(0x000000),1.0f);
    /*-----------------------------------------------------------------------*/

    /*-------- MEMBER FUNCTION PROTOTYPES -----------------------------------*/
   private:
    
   public:
    void updateTex();
    Texture(const std::string& inpath);
    ~Texture();

    std::string path() const;
    glm::vec4 borderCol() const;
    GLuint texID() const;
    int wrapModeS() const;
    int wrapModeT() const;

    void setWrapMode(const int s, const int t);
    void setWrapMode(const int m);
    void setWrapModeS(const int s);
    void setWrapModeT(const int t);

    void setBorderCol(const glm::vec3&);
    void setBorderCol(const glm::vec4&);
    void setBorderCol(const float, const float, const float);
    void setBorderCol(const float, const float, const float, const float);
    /*-----------------------------------------------------------------------*/
};
/*===========================================================================*/

/*======== OTHER FUNCTION PROTOTYPES ========================================*/

/*===========================================================================*/

#endif
