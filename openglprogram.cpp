#include "openglprogram.h"

OpenGLProgram::OpenGLProgram()
{

}

bool OpenGLProgram::init(const QString & vShaderFile, const QString & fShaderFile )
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, vShaderFile))
        return false;

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, fShaderFile))
        return false;

    // Link shader pipeline
    if (!program.link())
        return false;

    // Bind shader pipeline for use
    if (!program.bind())
        return false;
    return true;

}

void OpenGLProgram::use()
{

}
