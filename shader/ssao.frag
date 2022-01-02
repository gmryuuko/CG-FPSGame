#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D gPosition; // 观察空间position
uniform sampler2D gNormal;   // 观察空间法向量
uniform sampler2D texNoise;  // 噪声图像

// 采样核参数
uniform vec3 samples[64];
uniform int kernelSize;
uniform float radius;
uniform float bias;
uniform mat4 projection;

// 把4x4噪声图像平铺到屏幕大小
const vec2 noiseScale = vec2(1920.0/4.0, 1080.0/4.0); 

void main() {
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).xyz);
    vec3 randVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // TBN
    vec3 tangent = normalize(randVec - normal * dot(randVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // 采样坐标
        vec3 samplePos = TBN * samples[i];
        samplePos = fragPos + samplePos * radius; 
        
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset; // 裁切空间
        offset.xyz = (offset.xyz / offset.w) * 0.5 + 0.5; // [-1, 1] -> [0, 1]
        float sampleDepth = texture(gPosition, offset.xy).z; // 采样深度
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    
    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = vec4(vec3(occlusion), 0);
}