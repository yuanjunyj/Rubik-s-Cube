#ifdef GL_ES
precision highp float;
#endif

attribute vec3 position;

uniform mat4 modelMatrix;
uniform mat4 rotationMatrix;
uniform mat4 lightViewMatrix;
uniform mat4 lightProjectionMatrix;

varying vec4 projectedPosition;

void main( void )
{
    mat4 model = rotationMatrix * modelMatrix;
    vec3 finalPosition = position;

    projectedPosition =
            lightProjectionMatrix *
            lightViewMatrix *
            model *
            vec4( finalPosition, 1.0 );

    gl_Position = projectedPosition;
}
