#version 330 core

struct DirLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant; // 1
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

// material
uniform vec3 colorAmbient;
uniform vec3 colorDiffuse;
uniform vec3 colorSpecular;
uniform float shininess;

// texture
uniform bool useTexDiff;
uniform bool useTexSpec;
uniform bool useTexNormal;
uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;
uniform sampler2D texNormal;

// light
uniform int nPointLights;
uniform PointLight pointLights[16];
uniform int nDirLights;
uniform DirLight dirLights[16];
uniform int nSpotLights;
uniform SpotLight spotLights[16];

// shadow
uniform sampler2D depthMap;
uniform sampler2D secDepthMap;
uniform sampler2D depthMap2;
uniform sampler2D secDepthMap2;

// view
uniform vec3 viewPos;

uniform bool normalOn;

// input
in Vsout {
    vec3 fragPos;
    vec3 viewFragPos;
    vec3 normal;
    vec2 texCoords;
    vec4 fragPosLightSpace;
    vec4 fragPosSpotLightSpace;
    mat3 TBN;
    mat4 view;
} vsout;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 ViewPosition;
layout (location = 3) out vec4 NormalOut;
layout (location = 4) out vec4 AmbientOut;

struct Colors {
    vec3 ambient;
    vec3 color;
};

// 计算点光源光照
Colors CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float lighting);
// 计算方向光照
Colors CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float lighting);
// 
Colors CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float lighting);
// 计算阴影
float CalcShadow(vec4 pos, vec3 normal, vec3 lightDir);
float CalcSpotShadow(vec4 pos, vec3 normal, vec3 lightDir);

void main() {
    // 观察方向
    vec3 viewDir = normalize(viewPos - vsout.fragPos); // frag -> view
    // 法线
    vec3 normal = vsout.normal;
    // 使用法线贴图
    if (useTexNormal) {
        normal = texture(texNormal, vsout.texCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0); // [0,1] -> [-1,1]
        normal = normalize(vsout.TBN * normal);
    }

    if (!normalOn) {
        normal = vsout.normal;
    }

    // 是否被照亮，这里只计算dirLights[0]的阴影
    float lighting = 1 - CalcShadow(vsout.fragPosLightSpace, vsout.normal, -dirLights[0].direction);
    float spotLighting = nSpotLights > 0 ? 1 - CalcSpotShadow(vsout.fragPosSpotLightSpace, vsout.normal, spotLights[0].position - vsout.fragPos) : 0;

    // 计算所有光照
    vec3 result = vec3(0, 0, 0);
    vec3 ambientResult = vec3(0, 0, 0);
    Colors colors;
    // 只有dirLights[0]和spotLights[0]有阴影
    colors = CalcDirLight(dirLights[0], normal, viewDir, lighting);
    result += colors.color;
    ambientResult += colors.ambient;
    for (int i = 0; i < nPointLights; i++) {
        colors = CalcPointLight(pointLights[i], normal, vsout.fragPos, viewDir, 1);
        result += colors.color;
        ambientResult += colors.ambient;
    }
    for (int i = 1; i < nDirLights; i++) {
        colors = CalcDirLight(dirLights[i], normal, viewDir, 1);
        result += colors.color;
        ambientResult += colors.ambient;
    }
    if (nSpotLights > 0) {
        colors = CalcSpotLight(spotLights[0], normal, vsout.fragPos, viewDir, spotLighting);
        result += colors.color;
        ambientResult += colors.ambient;
    }
    for (int i = 1; i < nSpotLights; i++) {
        colors = CalcSpotLight(spotLights[i], normal, vsout.fragPos, viewDir, 1);
        result += colors.color;
        ambientResult += colors.ambient;
    }

    
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.2) {
        BrightColor = vec4(result, 1.0);
    } else {
        BrightColor = vec4(0, 0, 0, 1);
    }

    FragColor = vec4(result, 1.0);
    ViewPosition = vec4(vsout.viewFragPos, 0);
    NormalOut = vec4(normalize(mat3(vsout.view) * normal), 1.0);
    AmbientOut = vec4(ambientResult, 1);
}

Colors CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float lighting) {
    vec3 ambient, diffuse, specular;
    vec3 lightDir = normalize(light.position - fragPos); // frag -> light

    // ambient
    vec3 colorA = useTexDiff ? vec3(texture(texDiffuse, vsout.texCoords)) : colorAmbient; 
    ambient = light.ambient * colorA;

    // diffuse
    vec3 colorD = useTexDiff ? vec3(texture(texDiffuse, vsout.texCoords)) : colorDiffuse;
    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = light.diffuse * diff * colorD;

    // specular
    if (shininess == 0) {
        specular = vec3(0);
    } else {
        vec3 colorS = useTexSpec ? vec3(texture(texSpecular, vsout.texCoords)) : colorSpecular;
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0), shininess);
        specular = light.specular * spec * colorS;
    }

    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   

    // mix
    return Colors(ambient, ambient + lighting * (diffuse + specular));
}

Colors CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float lighting) {
    vec3 ambient, diffuse, specular;
    vec3 lightDir = normalize(-light.direction);

    // ambient
    vec3 colorA = useTexDiff ? vec3(texture(texDiffuse, vsout.texCoords)) : colorAmbient; 
    ambient = light.ambient * colorA;

    // diffuse
    vec3 colorD = useTexDiff ? vec3(texture(texDiffuse, vsout.texCoords)) : colorDiffuse;
    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = light.diffuse * diff * colorD;

    // specular
    if (shininess == 0) {
        specular = vec3(0);
    } else {
        vec3 colorS = useTexSpec ? vec3(texture(texSpecular, vsout.texCoords)) : colorSpecular;
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0), shininess);
        specular = light.specular * spec * colorS;
    }

    // mix
    return Colors(ambient, ambient + lighting * (diffuse + specular));
}

float CalcShadow(vec4 pos, vec3 normal, vec3 lightDir) {

    // 使用了正面剔除之后，背面就会产生条纹状的光照
    // 如果使用了bias + 正面剔除，整个背面都会被光照
    // 如果不使用法线贴图，那么dot(normal, lightDir) < 0，没有影响
    // 使用了法线贴图之后就可能产生背面光照，所以这里使用顶点法线而非法线贴图中采样的法线进行背面判断
    if (dot(normal, lightDir) < 0) return 1;

    // 透视除法
    vec3 projCoords = pos.xyz / pos.w;
    // [-1, 1] -> [0, 1]
    projCoords = projCoords * 0.5 + 0.5;
    // 检查当前片段是否处于阴影中
    float closestDepth = texture(depthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    // native shadow map
    // float shadow = currentDepth - 0.005 > closestDepth  ? 1.0 : 0.0;


    // shadow bias
    // 使用了正面剔除之后一般不需要
    // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // PCF
    float shadow = 0;
    int level = 2;
    vec2 texSize = 1.0 / textureSize(depthMap, 0);
    for (int x = -level; x <= level; x++) {
        for (int y = -level; y <= level; y++) {
            float pcfDepth = 0.5 * (texture(depthMap, projCoords.xy + vec2(x, y) * texSize).r 
                                + texture(secDepthMap, projCoords.xy + vec2(x, y) * texSize).r);
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= pow((level * 2 + 1), 2);

    return shadow;
}

float CalcSpotShadow(vec4 pos, vec3 normal, vec3 lightDir) {

    // 使用了正面剔除之后，背面就会产生条纹状的光照
    // 如果使用了bias + 正面剔除，整个背面都会被光照
    // 如果不使用法线贴图，那么dot(normal, lightDir) < 0，没有影响
    // 使用了法线贴图之后就可能产生背面光照，所以这里使用顶点法线而非法线贴图中采样的法线进行背面判断

    // 透视除法
    vec3 projCoords = pos.xyz / pos.w;
    // [-1, 1] -> [0, 1]
    projCoords = projCoords * 0.5 + 0.5;
    // 检查当前片段是否处于阴影中
    float closestDepth = texture(depthMap2, projCoords.xy).r;
    float currentDepth = projCoords.z;

    // native shadow map
    // float shadow = currentDepth - 0.005 > closestDepth  ? 1.0 : 0.0;


    // shadow bias
    // 使用了正面剔除之后一般不需要
    // float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // PCF
    float shadow = 0;
    int level = 2;
    vec2 texSize = 1.0 / textureSize(depthMap2, 0);
    for (int x = -level; x <= level; x++) {
        for (int y = -level; y <= level; y++) {
            float pcfDepth = 0.5 * (texture(depthMap2, projCoords.xy + vec2(x, y) * texSize).r 
                                + texture(secDepthMap2, projCoords.xy + vec2(x, y) * texSize).r);
            shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= pow((level * 2 + 1), 2);

    return shadow;
}

Colors CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float lighting) {
    vec3 ambient, diffuse, specular;
    vec3 lightDir = normalize(light.position - fragPos); // frag -> light

    // ambient
    vec3 colorA = useTexDiff ? vec3(texture(texDiffuse, vsout.texCoords)) : colorAmbient; 
    ambient = light.ambient * colorA;

    // diffuse
    vec3 colorD = useTexDiff ? vec3(texture(texDiffuse, vsout.texCoords)) : colorDiffuse;
    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = light.diffuse * diff * colorD;

    // specular
    if (shininess == 0) {
        specular = vec3(0);
    } else {
        vec3 colorS = useTexSpec ? vec3(texture(texSpecular, vsout.texCoords)) : colorSpecular;
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0), shininess);
        specular = light.specular * spec * colorS;
    }

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   

    // mix
    return Colors(ambient, ambient + lighting * (diffuse + specular));
}