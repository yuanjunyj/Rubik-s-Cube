#ifndef CUBE_H
#define CUBE_H

#include "vertex.h"
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>


class Cube
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


};

#endif // CUBE_H
