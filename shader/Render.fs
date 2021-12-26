#version 330 

in float alpha;
in float age;
in float lifeSpan;
uniform sampler2D spark;
uniform sampler2D launcher;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 ViewPosition;
layout (location = 3) out vec4 NormalOut;
layout (location = 4) out vec4 AmbientOut;

void main()
{
    vec4 texColor;
    if((age/lifeSpan) > 0.4)
        texColor = texture(spark,gl_PointCoord);
    else 
        texColor = texture(launcher,gl_PointCoord);
	texColor.a = texColor.r;

    FragColor = vec4(1.0f,0.3,0.1,alpha)*texColor;
    BrightColor = vec4(1.0f,0.3,0.1,alpha)*texColor;
    ViewPosition = vec4(0);
    NormalOut = vec4(0);
    AmbientOut = vec4(0);
}
