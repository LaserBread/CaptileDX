#version 410 core

uniform sampler2D tex;
uniform vec3 objectColor;
uniform int useTexture;

in vec2 fragTexCoord;

out vec4 fragColor;

void main() {
    vec3 base = objectColor;
    if (useTexture == 1) {
      base *= texture(tex, fragTexCoord).rgb;
    }
    fragColor = vec4(base, 1.0);
}