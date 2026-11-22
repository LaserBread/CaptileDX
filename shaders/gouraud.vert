#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;
uniform mat3 uN;
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light uLight;
uniform vec3 uObjectColor;
uniform float uShininess;

out vec3 lightingColor; // ordinary smooth interpolation
out vec2 texCoord;

void main()
{
    vec3 fragPos = vec3(uV * uM * vec4(aPos, 1.0));
    vec3 N = normalize(uN * aNormal);
    vec3 L = normalize(uLight.position - fragPos);
    vec3 V = normalize(-fragPos);
    vec3 H = normalize(L + V);
    vec3 ambient = uLight.ambient;
    vec3 diffuse = max(dot(N, L), 0.0) * uLight.diffuse;
    vec3 specular = pow(max(dot(N, H), 0.0), uShininess) * uLight.specular;
    lightingColor = (ambient + diffuse + specular) * uObjectColor;
    texCoord = aTexCoord;
    gl_Position = uP * uV * uM * vec4(aPos, 1.0);
}