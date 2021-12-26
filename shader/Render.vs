#version 330 core

layout (location = 0)in vec3 position_in;
layout (location = 1)in float age_in;
layout (location = 2)in float alpha_in;
layout (location = 3)in float size_in;
layout (location = 4)in float lifeSpan_in;

out float alpha;
out float age;
out float lifeSpan;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	gl_PointSize = size_in;
	gl_Position = projection * view * model * vec4(position_in,1.0f);
    alpha = alpha_in;
    age = age_in;
    lifeSpan = lifeSpan_in;
}
