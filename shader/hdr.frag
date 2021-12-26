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
uniform float HP;

bool inRange(float x, float mn, float mx) {
    return mn <= x && x <= mx;
}

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

    // x = [-700, -500]
    // y = [-400, -300]
    float x1 = -700, x2 = -500, y1 = -400, y2 = -360, hp = HP;
    if (hp > 1) hp = 1;
    if (hp < 0) hp = 0;
    // 左边框
    if (inRange(pos.x, x1 - 2, x1) && inRange(pos.y, y1 - 2, y2 + 2)) {
        FragColor = vec4(0, 0.8, 0, 1);
    }
    // right 边框
    if (inRange(pos.x, x2, x2 + 2) && inRange(pos.y, y1 - 2, y2 + 2)) {
        FragColor = vec4(0, 0.8, 0, 1);
    }
    // up
    if (inRange(pos.x, x1 - 2, x2 + 2) && inRange(pos.y, y2, y2 + 2)) {
        FragColor = vec4(0, 0.8, 0, 1);
    }
    // down
    if (inRange(pos.x, x1 - 2, x2 + 2) && inRange(pos.y, y1 - 2, y1)) {
        FragColor = vec4(0, 0.8, 0, 1);
    }
    // hp
    if (inRange(pos.y, y1, y2) && inRange(pos.x, x1, x1 + (x2 - x1) * hp)) {
        FragColor = vec4(0, 0.8, 0, 1);
    }

}