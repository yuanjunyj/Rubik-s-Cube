#version 330 core

in vec3 vColor;
in vec3 vNorm;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vPos;
flat in int facetIndex;

uniform vec3 baseColor;
uniform bool isBlock;
uniform bool useColor;
uniform bool useImage;
uniform bool focus;

uniform vec3 cameraPos;
uniform sampler2D shadow;
uniform samplerCube skybox;
uniform sampler2D images[6];
uniform int material_type;

varying vec3 viewSpacePosition;
varying vec4 vShadowCoord;

uniform vec3 lightPosition;
uniform vec3 viewPosition;

uniform bool isFocused;

float unpack(vec4 colour) {
    const vec4 bitShifts = vec4(1.0 / (256.0 * 256.0 * 256.0),
                                1.0 / (256.0 * 256.0),
                                1.0 / 256.0,
                                1);
    return dot(colour, bitShifts);
}

float PCF() {
    float bias = 0.001;
    float pcfValue = 0;
    vec4 shadowMapPosition = vShadowCoord / vShadowCoord.w;
    shadowMapPosition = (shadowMapPosition + 1.0) / 2.0;
    float currentDepth = shadowMapPosition.z;
    vec2 texelSize = 1.0 / textureSize(shadow, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            vec4 packedZValue = texture(shadow, shadowMapPosition.st + vec2(x, y) * texelSize);
            float closestDepth = unpack(packedZValue);
            pcfValue += float(currentDepth - bias > closestDepth);
        }
    }
    pcfValue /= 9.0;
    return pcfValue;
}

void main()
{
    vec3 N = normalize(vNorm);
    vec3 I = normalize(vPos - cameraPos);
    vec3 R = reflect(I, N);
    vec3 T = refract(I, N, 0.66);

    vec4 reflectedColor = textureCube(skybox, R);
    vec4 refractedColor = textureCube(skybox, T);

    if (isBlock == true) {
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
    } else {
        if (useColor == true) {
            gl_FragColor = vec4(vColor, 1.0);
        } else if (useImage == true) {
            gl_FragColor = texture2D(images[facetIndex], vTexCoord);
        }

        if(isFocused) {
            gl_FragColor *= 2;
        }
    }

    // Shadow
//    float shadowColor = 1.0;
//    if (vShadowCoord.w > 0.0) {
//        shadowColor = (1.0 - PCF()) * 0.8 + 0.2;
//    }
//    gl_FragColor *= shadowColor;

    // Focus
//    if (focus == true) {
//        gl_FragColor *= 0.5;
//    }


    // Phong
    vec3 pL = normalize(vec3(lightPosition - vPos));
    vec3 pV = normalize(cameraPos - vPos);
    vec3 pR = reflect(-pL, normalize(vNormal));
    float ambient = 0.5;
    float diffuse = 0.2 * max(dot(pL, normalize(vNormal)), 0.0);
    float specular = 0.3 * pow(max(dot(pR, pV), 0.0), 100);
    float phongColor = diffuse + ambient + specular;

    gl_FragColor *= phongColor;

}
