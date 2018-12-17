attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;
attribute vec3 color;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 rotationMatrix;

out vec3 vColor;

void main() {
    vColor = color;
    gl_Position = projectionMatrix * viewMatrix * rotationMatrix * modelMatrix * vec4(position, 1.0);
}
