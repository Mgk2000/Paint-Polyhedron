#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec2 v_texcoord;
varying vec3 v_normal;
void main()
{
    vec3 lightPos = vec3(0.0, 0.0, 1.0);
    float NL = max(dot(normalize(v_normal), lightPos), 0.0);
    vec3 color = texture2D(texture, v_texcoord).rgb;
    vec3 col = clamp(color * 0.5 + color * 0.5 * NL, 0.0, 1.0);
    gl_FragColor = vec4(col, 1.0);
}
/*
    "varying highp vec3 vert;\n"
    "varying highp vec3 vertNormal;\n"
    "uniform highp vec3 lightPos;\n"
    "void main() {\n"
    "   highp vec3 L = normalize(lightPos - vert);\n"
    "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
    "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
    "   highp vec3 col = clamp(color * 0.8 + color * 0.2 * NL, 0.0, 1.0);\n"
    "   gl_FragColor = vec4(col, 1.0);\n"
*/
