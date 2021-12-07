#version 330 core
out vec4 FragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = vec4(texture(skybox, texCoords));
    // FragColor = vec4(0.2, 0, 0, 0);
}
