#version 330 core

struct DirLight {
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

// material
uniform vec3 colorAmbient;
uniform vec3 colorDiffuse;
uniform vec3 colorSpecular;
uniform float shininess;

// texture
uniform bool useTexDiff;
uniform bool useTexSpec;
uniform sampler2D texDiffuse;
uniform sampler2D texSpecular;

// light
uniform int nPointLights;
uniform PointLight pointLights[16];
uniform int nDirLights;
uniform DirLight dirLights[16];

// shadow
uniform sampler2D depthMap;

// view
uniform vec3 viewPos;

// input
in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;
in vec4 fragPosLightSpace;

out vec4 FragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float lighting);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float lighting);
float CalcShadow(vec4 pos, vec3 normal, vec3 lightDir);

void main() {
    vec3 viewDir = normalize(viewPos - fragPos); // frag -> view
    
    // 是否被照亮
    float lighting = 1 - CalcShadow(fragPosLightSpace, normal, -dirLights[0].direction);

    vec3 result = vec3(0, 0, 0);
    for (int i = 0; i < nPointLights; i++) {
        result += CalcPointLight(pointLights[i], normal, fragPos, viewDir, lighting);
    }
    for (int i = 0; i < nDirLights; i++) {
        result += CalcDirLight(dirLights[i], normal, viewDir, lighting);
    }

    FragColor = vec4(result, 1.0);
    // FragColor = vec4(lighting);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float lighting) {
    vec3 ambient, diffuse, specular;
    vec3 lightDir = normalize(light.position - fragPos); // frag -> light

    // ambient
    vec3 colorA = useTexDiff ? vec3(texture(texDiffuse, texCoords)) : colorAmbient; 
    ambient = light.ambient * colorA;

    // diffuse
    vec3 colorD = useTexDiff ? vec3(texture(texDiffuse, texCoords)) : colorDiffuse;
    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = light.diffuse * diff * colorD;

    // specular
    vec3 colorS = useTexSpec ? vec3(texture(texSpecular, texCoords)) : colorSpecular;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0), shininess);
    specular = light.specular * spec * colorS;

    // 衰减

    // mix
    return ambient + lighting * (diffuse + specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float lighting) {
    vec3 ambient, diffuse, specular;
    vec3 lightDir = normalize(-light.direction);

    // ambient
    vec3 colorA = useTexDiff ? vec3(texture(texDiffuse, texCoords)) : colorAmbient; 
    ambient = light.ambient * colorA;

    // diffuse
    vec3 colorD = useTexDiff ? vec3(texture(texDiffuse, texCoords)) : colorDiffuse;
    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = light.diffuse * diff * colorD;

    // specular
    vec3 colorS = useTexSpec ? vec3(texture(texSpecular, texCoords)) : colorSpecular;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0), shininess);
    specular = light.specular * spec * colorS;

    // mix
    return ambient + lighting * (diffuse + specular);
}

float CalcShadow(vec4 pos, vec3 normal, vec3 lightDir) {
    // 透视除法
    vec3 projCoords = pos.xyz / pos.w;
    // [-1, 1] -> [0, 1]
    projCoords = projCoords * 0.5 + 0.5;
    // 检查当前片段是否处于阴影中
    float closestDepth = texture(depthMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    float bias = 0.01 * max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}