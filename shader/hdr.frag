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