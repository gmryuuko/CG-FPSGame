#version 330 

in float alpha;
in float age;
in float lifeSpan;
uniform sampler2D flameSpark;
uniform sampler2D flameStart;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 ViewPosition;
layout (location = 3) out vec4 NormalOut;
layout (location = 4) out vec4 AmbientOut;

void main()
{
    vec4 texColor;
    if((age/lifeSpan) > 0.4)
        texColor = texture(flameSpark,gl_PointCoord);
    else 
        texColor = texture(flameStart,gl_PointCoord);
    //if(texColor.r < 0.1f)discard;
	texColor.a = texColor.r;

    FragColor = vec4(0.5f,0.3,0.1,alpha)*texColor;
    BrightColor = vec4(0.5f,0.3,0.1,alpha)*texColor*4;
    ViewPosition = vec4(0);
    NormalOut = vec4(0);
    AmbientOut = vec4(0);
}
