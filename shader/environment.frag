uniform samplerCube skybox;
varying vec3 v_texCoord;

void main()
{
    gl_FragColor = textureCube(skybox, v_texCoord);
//    gl_FragColor = vec4(0.2, 0.2, 0.2, 1.0);
}
