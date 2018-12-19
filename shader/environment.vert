uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
attribute vec3 a_position;
varying vec3 v_texCoord;

void main() {
    v_texCoord = a_position;
    vec4 pos = projectionMatrix * viewMatrix * vec4(a_position, 1.0);
    gl_Position = pos.xyww;
}
