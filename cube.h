#ifndef CUBE_H
#define CUBE_H

#include "vertex.h"
#include "shader.h"
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>


class Cube : protected QOpenGLFunctions
{
public:
    Cube();
    ~Cube();

private:
    double m_length;
    Vertex *m_vertices;
    QMatrix4x4 m_modelMatrix;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLTexture m_texture;

public:
    void generateVertices(double length);
    void setPosition(QVector3D translation);
    void render(Shader* shader);

private:
    void setVertexAttribute(QOpenGLShaderProgram* program, int attribute_location, GLenum element_type, quint32 element_size, quint32 offset);

};

#endif // CUBE_H
