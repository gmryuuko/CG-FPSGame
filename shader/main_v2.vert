/* version 2
 * 法线贴图
 * PCF
 */
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
uniform mat4 spotLightSpaceMatrix;

out Vsout {
    vec3 fragPos;
    vec3 viewFragPos;
    vec3 normal;
    vec2 texCoords;
    vec4 fragPosLightSpace;
    vec4 fragPosSpotLightSpace;
    mat3 TBN;
    mat4 view;
} vsout;


void main() {

    vec4 modelPos = model * vec4(aPos, 1.0);
    vec4 viewPos = view * modelPos;

    gl_Position = projection * viewPos;

    // output
    vsout.fragPos = vec3(modelPos);
    vsout.viewFragPos = vec3(viewPos);
    vsout.texCoords = aTexCoords;
    vsout.normal = normalize(mat3(model) * aNormal); // 禁止使用不等比scale
    vsout.fragPosLightSpace = lightSpaceMatrix * vec4(vsout.fragPos, 1.0);
    vsout.fragPosSpotLightSpace = spotLightSpaceMatrix * vec4(vsout.fragPos, 1.0);
    // TBN
    vec3 T = normalize(vec3(model * vec4(tangent, 0)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0)));
    vsout.TBN = mat3(T, B, N);
    vsout.view = view;
}
