#version 330 core

in vec3 vColor;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vPos;
flat in int facetIndex;

uniform vec3 baseColor;
uniform bool useColor;
uniform bool useImage;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D images[6];
uniform int material_type;

void main()
{
    vec3 N = normalize(vNormal);
    vec3 I = normalize(vPos - cameraPos);
    vec3 R = reflect(I, N);
    vec3 T = refract(I, N, 0.66);

    vec4 reflectedColor = textureCube(skybox, R);
    vec4 refractedColor = textureCube(skybox, T);

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
}
