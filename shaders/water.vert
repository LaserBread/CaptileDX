#version 410 core

// Vertex Attribute (from VBO)
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

// Uniform
uniform mat4 m;
uniform mat4 v;
uniform mat4 p;
uniform float t;
uniform float a;

// To Fragment Shader
out vec4 fragColor;

// Function to simulate water waves
vec3 applyWaterWave(vec3 vertexPos, float time, float amplitude) {
    vec3 wavePos = vertexPos;
    
    // Wave parameters (adjust these to change wave look)
    float frequency = 0.25f; // How tight/frequent the waves are
    float speed = 2.5f;     // How fast the waves move over time
    
    // Displace the Y coordinate using a sine function along the X axis
    wavePos.y += sin(wavePos.x * frequency + time * speed) * amplitude;
    
    return wavePos;
}

void main() {
    gl_Position = p * v * m * vec4(applyWaterWave(pos, t, a), 1.0);
    fragColor = vec4(color, 0.8f);
}
