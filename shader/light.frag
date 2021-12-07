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

out vec4 FragColor;

void main() {
    vec3 color = useTexDiff ? vec3(texture(texDiffuse, texCoords)) : colorDiffuse;
    FragColor = vec4(color, 1.0);
}