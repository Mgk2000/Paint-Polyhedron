#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform mat3 normal_matrix;
attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
varying vec2 v_texcoord;
varying vec3 v_normal;
void main()
{
    vec4 mposition = a_position;
    mposition = mvp_matrix * mposition;
//    mposition.x = mposition.x*2;
//    mposition.y = mposition.y*2;
//    mposition.z = mposition.z+0.8;
    gl_Position = mposition;
    v_normal = normal_matrix * a_normal;
    v_texcoord = a_texcoord;
}
/*
"attribute vec4 vertex;\n"
"attribute vec3 normal;\n"
"varying vec3 vert;\n"
"varying vec3 vertNormal;\n"
"uniform mat4 projMatrix;\n"
"uniform mat4 mvMatrix;\n"
"uniform mat3 normalMatrix;\n"
"void main() {\n"
"   vert = vertex.xyz;\n"
"   vertNormal = normalMatrix * normal;\n"
"   gl_Position = projMatrix * mvMatrix * vertex;\n"
"}\n";*/
