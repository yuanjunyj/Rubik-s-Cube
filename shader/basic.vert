#version 330 core

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 rotationMatrix;

out vec3 vColor;
out vec3 vNormal;
out vec3 vPos;
out vec2 vTexCoord;
flat out int facetIndex;

void main() {
    vColor = color;
    mat4 model = rotationMatrix * modelMatrix;
    vNormal = mat3(transpose(inverse(model))) * normal;
    vPos = vec3(model * vec4(position, 1.0));
    vTexCoord = texCoord;
    if (normal == vec3(0, 0, 1)) {
        facetIndex = 0;
    } else if (normal == vec3(0, 0, -1)) {
        facetIndex = 1;
    } else if (normal == vec3(0, 1, 0)) {
        facetIndex = 2;
    } else if (normal == vec3(0, -1, 0)) {
        facetIndex = 3;
    } else if (normal == vec3(1, 0, 0)) {
        facetIndex = 4;
    } else if (normal == vec3(-1, 0, 0)) {
        facetIndex = 5;
    }

    gl_Position = projectionMatrix * viewMatrix * model * vec4(position, 1.0);
}
