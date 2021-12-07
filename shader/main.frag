#version 330 core

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

// view
uniform vec3 viewPos;

// input
in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 FragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 viewDir = normalize(viewPos - fragPos); // frag -> view

    vec3 result = vec3(0, 0, 0);
    for (int i = 0; i < nPointLights; i++) {
        result += CalcPointLight(pointLights[i], normal, fragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
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
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0), shininess);
    specular = light.specular * spec * colorSpecular;

    // 衰减

    // mix
    // return ambient + diffuse + specular;
    return ambient + diffuse + specular;
}
