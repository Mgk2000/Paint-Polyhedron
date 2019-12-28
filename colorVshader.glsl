#ifdef GL_ES
precision mediump int;
precision mediump float;
#endif
attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec4 a_colormult;
varying vec2 v_texcoord;
varying vec4 v_colormult;
void main()
{
    gl_Position = a_position;
    v_texcoord = a_texcoord;
    v_colormult = a_colormult;
}
