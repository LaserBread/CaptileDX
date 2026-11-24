/* OBJECT (Header) - Ethan Jones - object
 * DESCRIPTION:
 *
 * DEPENDENCIES:
 */

/*======== GUARDS ===========================================================*/
// These are generated on file creation.  Don't change them.
#ifndef OBJECT_H
#define OBJECT_H
/*===========================================================================*/

/*======== HEADERS ==========================================================*/
#include <GL/glew.h>

#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "color.h"
#include "entity.hpp"
#include "light.hpp"
#include "shader.hpp"
#include "texture.hpp"
/*===========================================================================*/

/*%%%EXCEPTION: BAD OBJECT%%%*/
// Throws an error when an object is wrong. I like the more visceral crashing,
// okay?
class bad_object : public std::runtime_error {
   public:
    explicit bad_object(const std::string& message) : std::runtime_error(message) {};
};
/*%%%%%%*/

#define numVAOs 1
#define numVBOs
/*%%%OBJECT%%%*/
// In proper implementation, Object will be a virtual class that other classes
// inherit from. But this requires a better sense of how Scenes and such will
// be handled here. Essentially, I am building this into an engine of sorts.
class Object : public Entity {
   private:
    void _draw_commit();
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;

   public:
    int numVertices = 0;
    float shininess;

    // The intent of splitting these is so subclasses can have their own
    // vao and vbo entries, which will be important when it comes to
    // shaders.
    GLuint vao = 0;  // Points to the VAO for this specific object.

    GLuint vboVertex = 0;    // Points to the Vertex VBO for this object
    GLuint vboNormal = 0;    // Points to the Normal VBO for this object
    GLuint vboTexCoord = 0;  // Points to the Texture Coordinate VBO for this object

    std::shared_ptr<Shader> shader;

    GLuint textureID = 0;
    std::shared_ptr<Texture> tex;
    glm::vec3 color{1.0f, 1.0f, 1.0f};

    Object(const std::string& objPath, const std::shared_ptr<Shader> inShader);
    Object(const std::string& objPath,
           std::shared_ptr<Shader> inShader, const std::string& tex_path);
    Object(const std::string& objPath,
           std::shared_ptr<Shader> inShader, const std::shared_ptr<Texture> in_tex);
    virtual ~Object();

    void draw(const glm::mat4& view, const glm::mat4& projection, const PointLight& light, const glm::mat4& = glm::mat4{1.0f}) const override;
    void cleanup();
    void setColor(const glm::vec3& c);
    void setColor(const float r, const float g, const float b);
    void setShininess(float);

    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;

   private:
    bool loadOBJ(const std::string& path);
    void setupBuffers();
};
/*===========================================================================*/

class OrbitLayer : public Entity {
   public:
    std::shared_ptr<OrbitLayer> below;
    std::shared_ptr<Object> body;

    OrbitLayer(const std::string& objPath, const std::shared_ptr<Shader> inShader);
    OrbitLayer(const std::string& objPath, const std::shared_ptr<Shader> inShader, const std::shared_ptr<Texture> in_tex);
    ~OrbitLayer() = default;

    void draw(const glm::mat4& view, const glm::mat4& projection, const PointLight& light, const glm::mat4& = glm::mat4{1.0f}) const override;

    OrbitLayer(const OrbitLayer&) = delete;
    OrbitLayer& operator=(const OrbitLayer&) = delete;
};
#endif
