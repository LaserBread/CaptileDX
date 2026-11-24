/* OBJECT (Source) - Ethan Jones - object
 * DESCRIPTION:
 *
 */

/*======== HEADERS ==========================================================*/
#include "object.hpp"
/*===========================================================================*/

/*======== GLOBALS ==========================================================*/

/*===========================================================================*/

// object.cpp
#include <soil2/SOIL2.h>

#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "entity.hpp"
#include "light.hpp"
#include "object.hpp"
#include "texture.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_DISABLE_FAST_FLOAT
#include "tiny_obj_loader.h"

// ------------------------------------------------------------
// public
// ------------------------------------------------------------

Object::~Object() {
    glDeleteBuffers(1, &vboVertex);
    glDeleteBuffers(1, &vboNormal);
    glDeleteBuffers(1, &vboTexCoord);

    glDeleteVertexArrays(1, &vao);

    // The shader should not be deleted since it's shared. Other objects may be
    // using it.

    if (textureID) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

Object::Object(const std::string& objPath,
               std::shared_ptr<Shader> inShader) : Entity::Entity() {
    shader = std::move(inShader);

    loadOBJ(objPath);

    setupBuffers();
    tex = nullptr;
}

Object::Object(const std::string& objPath,
               std::shared_ptr<Shader> inShader, const std::shared_ptr<Texture> in_tex) : Entity::Entity(){
    shader = std::move(inShader);

    loadOBJ(objPath);

    setupBuffers();
    tex = std::move(in_tex);
}

Object::Object(const std::string& objPath,
               std::shared_ptr<Shader> inShader, const std::string& tex_path) : Entity::Entity() {
    shader = std::move(inShader);

    loadOBJ(objPath);

    setupBuffers();
    tex = std::make_shared<Texture>(tex_path);
}

void Object::setColor(const glm::vec3& c) {
    color = c;
}

void Object::setColor(const float r, const float g, const float b) {
    color = glm::vec3(r, g, b);
}

void Object::draw(const glm::mat4& view, const glm::mat4& projection, const PointLight& light, const glm::mat4&) const {
    // HACK: While a downcast may be necessary, it should be handled by the
    // object specifying a shader.
    std::shared_ptr<LitShader> lShader = std::dynamic_pointer_cast<LitShader>(shader);

    shader->use(projection, view);

    shader->sendM(modelMatrix);

    glm::mat3 n = glm::mat3(glm::transpose(glm::inverse(view * modelMatrix)));
    lShader->sendN(n);

    lShader->sendCol(color);

    // I guess I now know where Kotlin's elvis operator ( ?: ) comes from.
    lShader->sendTex((tex != nullptr)? tex->texID() : 0);

    lShader->sendLight(light, view);

    lShader->sendShine(shininess);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glBindVertexArray(0);
}

void Object::cleanup() {
}

void Object::setShininess(float s) {
    shininess = s;
}

// ------------------------------------------------------------
// private – geometry
// ------------------------------------------------------------

bool Object::loadOBJ(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());
    if (!warn.empty())
        std::cout << "WARN: " << warn << "\n";
    if (!err.empty())
        std::cerr << "ERR: " << err << "\n";
    if (!ok)
        return false;

    vertices.clear();
    normals.clear();
    texCoords.clear();

    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

                if (idx.normal_index >= 0) {
                    normals.push_back(attrib.normals[3 * idx.normal_index + 0]);
                    normals.push_back(attrib.normals[3 * idx.normal_index + 1]);
                    normals.push_back(attrib.normals[3 * idx.normal_index + 2]);
                } else {
                    normals.push_back(0.0f);
                    normals.push_back(0.0f);
                    normals.push_back(1.0f);
                }

                if (idx.texcoord_index >= 0) {
                    texCoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                    texCoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                } else {
                    texCoords.push_back(0.0f);
                    texCoords.push_back(0.0f);
                }
            }
            index_offset += fv;
        }
    }

    numVertices = static_cast<int>(vertices.size() / 3);
    return true;
}

void Object::setupBuffers() {
    // glGenVertexArrays(numVAOs, VAO);
    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vboVertex);
    glGenBuffers(1, &vboNormal);
    glGenBuffers(1, &vboTexCoord);

    glBindVertexArray(vao);

    // Position → location 0
    glBindBuffer(GL_ARRAY_BUFFER, vboVertex);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal → location 1
    glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
    glBufferData(GL_ARRAY_BUFFER,
                 normals.size() * sizeof(float),
                 normals.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // TexCoord → location 2
    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoord);
    glBufferData(GL_ARRAY_BUFFER,
                 texCoords.size() * sizeof(float),
                 texCoords.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

OrbitLayer::OrbitLayer(const std::string& objPath, const std::shared_ptr<Shader> inShader) : Entity(){
    body = std::make_shared<Object>(objPath,inShader);
    
}

void OrbitLayer::draw(const glm::mat4& view, const glm::mat4& projection, const PointLight& light, const glm::mat4& stk) const {
    body->draw(view, projection, light, modelMatrix * stk);
    if(below) below->draw(view, projection, light, modelMatrix * stk);
}
