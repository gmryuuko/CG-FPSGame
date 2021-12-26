#version 330 core
layout (points) in;
layout (points,max_vertices = 4) out;

in float type_gs[];
in vec3 position_gs[];
in vec3 velocity_gs[];
in float age_gs[];
in float alpha_gs[];
in float size_gs[];
in float lifeSpan_gs[];

out float type_fs;
out vec3 position_fs;
out vec3 velocity_fs;
out float age_fs;
out float alpha_fs;
out float size_fs;
out float lifeSpan_fs;

uniform float dt;//每帧时间变化量
uniform float timer;//总的时间变化量
uniform float MAX_AGE;
uniform float MIN_AGE;
uniform vec3 MAX_VELOCITY;
uniform vec3 MIN_VELOCITY;
uniform float r;

#define LAUNCHER 0.0
#define BULLET 1.0


float random2 (float n) {
    return fract(sin(n)*1000000.0);
}


float random (vec2 st) {
    return fract(sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123);
}

void main()
{
    float age = age_gs[0] + dt;
    float error=0.0;
	if(type_gs[0] == LAUNCHER){//发射粒子
        if(age > lifeSpan_gs[0] ){
            //发射火花粒子
            type_fs = BULLET;
            position_fs = position_gs[0];
            velocity_fs = (MAX_VELOCITY-MIN_VELOCITY)*random(position_gs[0].xz)+MIN_VELOCITY;
            float d = sqrt(position_fs.x*position_fs.x + position_fs.z*position_fs.z);
            lifeSpan_fs = ((MAX_AGE-MIN_AGE)*random(position_gs[0].yz) + MIN_AGE)*(1+r/d);
            age_fs=0.0;
            alpha_fs = alpha_gs[0];
            size_fs = size_gs[0];
            EmitVertex();
            EndPrimitive();
            age=0.0;
        }
        type_fs = LAUNCHER;
        velocity_fs = velocity_gs[0];
        position_fs = position_gs[0];
        age_fs = age;
        alpha_fs = alpha_gs[0];
        size_fs = size_gs[0];
        lifeSpan_fs = lifeSpan_gs[0];
        EmitVertex();
        EndPrimitive();
    }
    else{//火花粒子
        if(age <= lifeSpan_gs[0]){
            float DeltaTimeSecs = dt/1000.0f;
            vec3 DeltaP = velocity_gs[0]*DeltaTimeSecs;
			vec3 DeltaV = DeltaTimeSecs*vec3(0.0,1.0,0.0);
            type_fs = BULLET;
            position_fs = position_gs[0] + DeltaP;
            velocity_fs = velocity_gs[0] + DeltaV;
            age_fs = age;
            lifeSpan_fs = lifeSpan_gs[0];
            float factor = 1.0f/(((lifeSpan_fs-2*age_fs)/2000.0f)*((lifeSpan_fs-2*age_fs)/2000.0f)+1);
            alpha_fs = factor;
            size_fs = 40.0*factor;
            EmitVertex();
            EndPrimitive();
        }
    }
}
