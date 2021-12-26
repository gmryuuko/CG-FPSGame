#version 330 core 
layout (location = 0) in float type;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 velocity;
layout (location = 3) in float age;
layout (location = 4) in float alpha;
layout (location = 5) in float size;
layout (location = 6) in float lifeSpan;
out float type_gs;
out vec3 position_gs;
out vec3 velocity_gs;
out float age_gs;
out float alpha_gs;
out float size_gs;
out float lifeSpan_gs;

void main()
{
	type_gs = type;
	position_gs = position;
	velocity_gs = velocity;
	age_gs = age;
    alpha_gs = alpha;
    size_gs = size;
    lifeSpan_gs = lifeSpan;
}
