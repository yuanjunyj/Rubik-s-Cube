#version 330 core
#extension GL_NV_shadow_samplers_cube : enable
in vec3 vColor;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vPos;
flat in int facetIndex;

uniform vec3 baseColor;
uniform bool useColor;
uniform bool useImage;

uniform vec3 cameraPos;
uniform sampler2D shadow;
uniform samplerCube skybox;
uniform sampler2D images[6];
uniform int material_type;

varying vec3 viewSpacePosition;
varying vec4 vShadowCoord;

float unpack(vec4 colour) {
    const vec4 bitShifts = vec4(1.0 / (256.0 * 256.0 * 256.0),
                                1.0 / (256.0 * 256.0),
                                1.0 / 256.0,
                                1);
    return dot(colour, bitShifts);
}

float shadowSimple() {
    vec4 shadowMapPosition = vShadowCoord / vShadowCoord.w;
    shadowMapPosition = (shadowMapPosition + 1.0) / 2.0;
    vec4 packedZValue = texture2D(shadow, shadowMapPosition.st);
    float distanceFromLight = unpack(packedZValue);
    float bias = 1e-6;
    return float(distanceFromLight > shadowMapPosition.z - bias);
}

void main()
{
    vec3 N = normalize(vNormal);
    vec3 I = normalize(vPos - cameraPos);
    vec3 R = reflect(I, N);
    vec3 T = refract(I, N, 0.66);

    vec4 reflectedColor = textureCube(skybox, R);
    vec4 refractedColor = textureCube(skybox, T);

    float shadowColor = 1.0;
    if (vShadowCoord.w > 0.0) {
        shadowColor = shadowSimple();
        shadowColor = shadowColor * 0.5 + 0.5;
    }

    if (useColor == true) {
        gl_FragColor = vec4(vColor, 1.0);
    }

    else if (useImage == true) {

        gl_FragColor = texture2D(images[facetIndex], vTexCoord);
    }

    else {
        if (material_type == 0) {
            gl_FragColor = vec4(baseColor, 1.0);
        } else if (material_type == 1) {
            gl_FragColor = reflectedColor;
        } else if (material_type == 2) {
            gl_FragColor = refractedColor;
        } else if (material_type == 3) {
            float fresnel = 0.4 + 0.6 * pow(min(0.0, 1.0 - dot(-I, N)), 4.0);
            gl_FragColor = mix(refractedColor, reflectedColor, fresnel);
        }
    }

    gl_FragColor = shadowColor * gl_FragColor;
}
