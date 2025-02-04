#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec4 v_colormult;
varying vec2 v_texcoord;

void main()
{
    // Set fragment color from texture
    gl_FragColor = texture2D(texture, v_texcoord) * v_colormult;
}
