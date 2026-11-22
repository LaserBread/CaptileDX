#version 410 core

// Vertex Attribute (from VBO)
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

// Uniform
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;

// To Fragment Shader
out vec4 fragColor;

void main() {
    gl_Position = p * v * m * vec4(pos, 1.0f);
    fragColor = vec4(color, 1.0f);
}
