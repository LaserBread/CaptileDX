#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;
uniform mat3 uN;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main()
{
    fragPos = vec3(uV * uM * vec4(aPos, 1.0));
    normal = uN * aNormal;
    texCoord = aTexCoord;
    gl_Position = uP * uV * uM * vec4(aPos, 1.0);
}
