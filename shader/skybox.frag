#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = vec4(texture(skybox, texCoords));
    BrightColor = vec4(0);
    // FragColor = vec4(0.2, 0, 0, 0);
}
