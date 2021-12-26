#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform sampler2D brightBuffer;
uniform sampler2D ambientBuffer;
uniform sampler2D ambientOcclusion;
uniform bool ssaoOn;
uniform bool hdrOn;
uniform bool bloomOn;
uniform float exposure;

void main() {
    // HUD
    vec2 pos = TexCoords;
    pos = pos - vec2(0.5, 0.5);
    pos.x *= 1600;
    pos.y *= 900;
    // 圆形准星
    // float r2 = pos.x * pos.x + pos.y * pos.y;
    // if (r2 <= 4 || (20 * 20 <= r2 && r2 <= 22 * 22)) {
    //     FragColor = vec4(0, 0.9, 0, 1);
    //     return;
    // }
    // 十字准星
    float siz = 15, dis = 10;
    if ((dis <= abs(pos.x) && abs(pos.x) <= dis + siz) && (abs(pos.y) <= 2)) {
        FragColor = vec4(0, 0.9, 0, 1);
        return;
    }
    if ((dis <= abs(pos.y) && abs(pos.y) <= dis + siz) && (abs(pos.x) <= 1.5)) {
        FragColor = vec4(0, 0.9, 0, 1);
        return;
    }

    // FragColor = texture(ambientBuffer, TexCoords);
    // return;
    const float gamma = 1.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 brightColor = texture(brightBuffer, TexCoords).rgb;

    if (bloomOn) {
        hdrColor += brightColor;
    }

    // ssao
    if (ssaoOn) {
        hdrColor -= texture(ambientBuffer, TexCoords).rgb * (1.0 - texture(ambientOcclusion, TexCoords).r);
    }

    if(hdrOn) {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // also gamma correct while we're at it       
        result = pow(result, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
    else {
        vec3 result = hdrColor;
        result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}