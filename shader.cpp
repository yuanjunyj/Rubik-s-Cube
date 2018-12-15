#include "shader.h"

Shader::Shader()
{

}

Shader::~Shader() {
    delete m_program;
}

void Shader::setupShader(const QString& vertexShader, const QString& fragmentShader) {
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShader);
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShader);
    m_program->link();
}
