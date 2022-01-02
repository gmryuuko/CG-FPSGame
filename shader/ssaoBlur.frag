#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D ssaoInput;

void main()  {
    vec2 texSize = 1.0 / vec2(textureSize(ssaoInput, 0));
    float res = 0;

    // 5*5 
    for (int x = -2; x <= 2; ++x)  {
        for (int y = -2; y <= 2; ++y)  {
            vec2 offset = vec2(x, y) * texSize;
            res += texture(ssaoInput, TexCoords + offset).r;
        }
    }

    FragColor = vec4(vec3(res / 25.0), 1);
}  