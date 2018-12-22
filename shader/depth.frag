#ifdef GL_ES
precision highp float;
#endif

varying vec4 projectedPosition;

vec4 pack( float depth )
{
    const vec4 bitSh = vec4( 256.0 * 256.0 * 256.0,
                             256.0 * 256.0,
                             256.0,
                             1.0 );
    const vec4 bitMsk = vec4( 0.0,
                              1.0 / 256.0,
                              1.0 / 256.0,
                              1.0 / 256.0 );
    vec4 comp = fract( depth * bitSh );
    comp -= comp.xxyz * bitMsk;
    return comp;
}

void main( void )
{
    float normalizedZ = projectedPosition.z / projectedPosition.w;
    normalizedZ = ( normalizedZ + 1.0 ) / 2.0;
    gl_FragColor = pack( normalizedZ );
}
