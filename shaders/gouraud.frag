#version 410 core

in vec3 lightingColor;
in vec2 texCoord;

uniform sampler2D uTex;
uniform int useTexture;

out vec4 FragColor;

void main()
{
    vec3 color = lightingColor;
    if (useTexture == 1)
        color *= texture(uTex, texCoord).rgb;
    FragColor = vec4(color, 1.0);
}