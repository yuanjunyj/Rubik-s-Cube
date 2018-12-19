in vec3 vColor;
uniform vec3 baseColor;
uniform bool useColor;

void main()
{
    if (useColor == true) {
        gl_FragColor = vec4(vColor, 1.0);
    } else {
        gl_FragColor = vec4(baseColor, 1.0);
    }
}
