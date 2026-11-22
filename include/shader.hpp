/* SHADER (Header) - Ethan Jones - shader
 * DESCRIPTION:
 *
 * DEPENDENCIES:
 */

/*======== GUARDS ===========================================================*/
// These are generated on file creation.  Don't change them.
#ifndef SHADER_H
#define SHADER_H
/*===========================================================================*/

/*======== HEADERS ==========================================================*/
#include <GL/glew.h>

#include <glm/glm.hpp>  // Basic include for GLM
/*===========================================================================*/

/*%%%LOCATIONS%%%*/
#define FLAT_V "shaders/flat.vert"
#define FLAT_F "shaders/flat.frag"

#define GOURAUD_V "shaders/gouraud.vert"
#define GOURAUD_F "shaders/gouraud.frag"

#define PHONG_V "shaders/phong.vert"
#define PHONG_F "shaders/phong.frag"

#define TEXTURED_V "shaders/textured.vert"
#define TEXTURED_F "shaders/textured.frag"
/*%%%%%%*/

class PointLight;

/*======= SHADER =====================================================*/
class Shader {
   protected:
    /*-------- MEMBER VARIABLES ---------------------------------------------*/
    GLint _mLoc;
    GLint _vLoc;
    GLint _pLoc;

    GLuint _vProgLoc;
    GLuint _fProgLoc;
    GLuint _vfProgLoc;
    /*-----------------------------------------------------------------------*/

    Shader(const char* vertPath, const char* fragPath);

   public:
    /*-------- MEMBER FUNCTION PROTOTYPES -----------------------------------*/
    ~Shader();

    GLint mLoc();
    GLint pLoc();
    GLint vLoc();
    GLuint progLoc();

    void sendM(const glm::mat4&);
    void sendV(const glm::mat4&);
    void sendP(const glm::mat4&);

    virtual void use(const glm::mat4& proj, const glm::mat4& v) = 0;

    // Disable copy constructor/assignment to prevent double-freeing GPU handles
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    /*-----------------------------------------------------------------------*/
};
/*===========================================================================*/

/*======== SOLID SHADER =====================================================*/
class SolidShader : public Shader {
   public:
    SolidShader();

    void use(const glm::mat4& proj, const glm::mat4& v) override;

    SolidShader(const SolidShader&) = delete;
    SolidShader& operator=(const SolidShader&) = delete;
};
/*===========================================================================*/

/*%%%LIT SHADER%%%*/
class LitShader : public Shader {
   protected:
    /*%%MEMBER VARIABLES%%*/
    GLint _nLoc;
    GLint _ambLoc;
    GLint _lPosLoc;
    GLint _diffLoc;
    GLint _specLoc;
    GLint _shineLoc;
    GLint _colLoc;
    GLint _useTexLoc;
    GLint _texLoc;
    /*%%%%%%*/


   public:
    /*%%MEMBER FUNCTIONS%%*/
    LitShader(const char* vertPath, const char* fragPath);

    virtual void use(const glm::mat4& proj, const glm::mat4& v) = 0;

    void sendTex(GLuint texID);
    void sendCol(const glm::vec3&);
    void sendN(const glm::mat3&);
    void sendLight(const PointLight& l, const glm::mat4& v);
    void sendLight(const glm::vec3& pos, const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, const glm::mat4& view);

    void sendShine(const float);
    /*%%%%*/

    /*%%DISABLE COPY AND ASSIGNMENT CONSTRUCTORS%%*/
    LitShader(const LitShader&) = delete;
    LitShader& operator=(const LitShader&) = delete;
    /*%%%%*/
};
/*%%%%%%*/

class FlatShader : public LitShader {
   public:
    /*%%MEMBER FUNCTIONS%%*/
    FlatShader();
    void use(const glm::mat4& proj, const glm::mat4& v) override;
    /*%%%%*/

    /*%%DISABLE COPY AND ASSIGNMENT CONSTRUCTORS%%*/
    FlatShader(const FlatShader&) = delete;
    FlatShader& operator=(const FlatShader&) = delete;
    /*%%%%*/
};

class GouraudShader : public LitShader {
   public:
    /*%%MEMBER FUNCTIONS%%*/
    GouraudShader();
    void use(const glm::mat4& proj, const glm::mat4& v) override;
    /*%%%%*/

    /*%%DISABLE COPY AND ASSIGNMENT CONSTRUCTORS%%*/
    GouraudShader(const GouraudShader&) = delete;
    GouraudShader& operator=(const GouraudShader&) = delete;
    /*%%%%*/
};

class PhongShader : public LitShader {
   public:
    /*%%MEMBER FUNCTIONS%%*/
    PhongShader();
    void use(const glm::mat4& proj, const glm::mat4& v) override;
    /*%%%%*/

    /*%%DISABLE COPY AND ASSIGNMENT CONSTRUCTORS%%*/
    PhongShader(const PhongShader&) = delete;
    PhongShader& operator=(const PhongShader&) = delete;
    /*%%%%*/
};

#endif
