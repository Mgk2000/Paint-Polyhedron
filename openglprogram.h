#ifndef OPENGLPROGRAM_H
#define OPENGLPROGRAM_H
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class OpenGLProgram : protected QOpenGLFunctions
{
public:
    OpenGLProgram();
    bool init(const QString & vShaderFile, const QString & fShaderFile);
    void use();
    QOpenGLShaderProgram * shaderProgram() {return &program;}
private:
    QOpenGLShaderProgram program;
};

#endif // OPENGLPROGRAM_H
