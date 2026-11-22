#version 410 core

// Vertex Attribute (from VBO)
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

// Uniform
uniform mat4 mv;
uniform mat4 p;
uniform mat3 n;

out vec2 fragTexCoord;

void main() {
    gl_Position = p * mv * vec4(pos, 1.0f);
    fragTexCoord = texCoord;
}