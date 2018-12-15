#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLShaderProgram>


class Shader
{
public:
    Shader();
    ~Shader();
    QOpenGLShaderProgram* getProgram() const { return m_program; }
    void setupShader(const QString& vertexShader, const QString& fragmentShader);

private:
    QOpenGLShaderProgram* m_program;

};

#endif // SHADER_H
