#version 330 core

// material
uniform vec3 colorAmbient;
uniform vec3 colorDiffuse;
uniform vec3 colorSpecular;
uniform float shininess;

// texture
uniform bool useTexDiff;
uniform bool useTexSpec;
uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;

// input
in vec2 texCoords;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

void main() {
    vec3 result = useTexDiff ? vec3(texture(texDiffuse, texCoords)) : colorDiffuse;

    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1) {
        BrightColor = vec4(result, 1.0);
    } else {
        BrightColor = vec4(0, 0, 0, 1);
    }
    FragColor = vec4(result, 1.0);
}