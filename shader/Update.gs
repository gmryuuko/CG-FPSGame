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
	if(type_gs[0] == LAUNCHER){//火焰发射粒子
        if(age > lifeSpan_gs[0] ){
            //发射第二级粒子
            type_fs = BULLET;
            position_fs = position_gs[0];
            //与初始发射器一样，在最大和最小速度之间随机
            velocity_fs = (MAX_VELOCITY-MIN_VELOCITY)*random(position_gs[0].xz)+MIN_VELOCITY;
			//寿命同上
            //age_fs = (MAX_AGE-MIN_AGE)*random(position_gs[0].yz) + MIN_AGE;
			//求当前粒子到圆心的距离，默认中心在原点
            float d = sqrt(position_fs.x*position_fs.x + position_fs.z*position_fs.z);
			//火焰的寿命在中心长一点，边缘短，这里简单以到中心的距离为标准
			//r为火焰中心半径
			//if(d <= r)age_fs *= 1.3;
			//age_fs *= (1 + r/d);
            lifeSpan_fs = ((MAX_AGE-MIN_AGE)*random(position_gs[0].yz) + MIN_AGE)*(1+r/d);
            age_fs=0.0;
            alpha_fs = alpha_gs[0];
            size_fs = size_gs[0];
            EmitVertex();
            EndPrimitive();
            //lifeSpan_fs=(MAX_AGE-MIN_AGE)*random(position_gs[0].xz)+MIN_AGE;
            //age_fs=0.0;
            //age=(MAX_AGE-MIN_AGE)*random(position_gs[0].xz)+MIN_AGE;
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
    else{//第二级粒子
        if(age <= lifeSpan_gs[0]){
			//将时间转为以秒为单位
            float DeltaTimeSecs = dt/1000.0f;
			//求位置的变化量，这里未考虑重力加速度
            vec3 DeltaP = velocity_gs[0]*DeltaTimeSecs;
			vec3 DeltaV = DeltaTimeSecs*vec3(0.0,1.0,0.0);
            type_fs = BULLET;
            position_fs = position_gs[0] + DeltaP;
            velocity_fs = velocity_gs[0] + DeltaV;
            age_fs = age;
            lifeSpan_fs = lifeSpan_gs[0];
            //在粒子生命周期中，一开始比较小，后来增大，然后又减小
            //以下用当前剩余寿命和全部寿命设置大小和alpha,实际上曲线是呈现正太分布，中间大，两边小
            float factor = 1.0f/(((lifeSpan_fs-age_fs)/1000.0f - lifeSpan_fs/2000.0f)*((lifeSpan_fs-age_fs)/1000.0f - lifeSpan_fs/2000.0f)+1);
            //float factor = 1.0f/(1+((age_fs)/1000.0)*((age_fs)/1000.0));
            //if(position_fs.y>=5)
            alpha_fs = factor;
            //if(alpha_fs>=0.7 && alpha_fs<=1.0) alpha_fs*=0.5;
            size_fs = 55.0*factor;
            // if(alpha_fs>0.8){
            //     EmitVertex();
            //     EndPrimitive();
            // }
            EmitVertex();
            EndPrimitive();
        }
    }
}
