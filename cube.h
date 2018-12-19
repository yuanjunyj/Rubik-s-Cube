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
    static const QVector3D s_base_color;
    double m_length;
    Vertex *m_facets, *m_pasters;
    int m_pasters_count;
    QMatrix4x4 m_modelMatrix;
    QOpenGLBuffer m_blockBuffer, m_pasterBuffer;
    QOpenGLTexture m_texture;
    QVector3D m_color[6];

public:
    void createBlock();
    void scale(double ratio);
    void translate(QVector3D translation);
    void rotate(int angle, QVector3D axis); // angle should be multiples of 90
    void setFacetColor(QChar facet, QVector3D color);
    void createPasters();
    void render(Shader* shader);

private:
    void initialize();
    void setVertexAttribute(QOpenGLShaderProgram* program, int attribute_location, GLenum element_type, quint32 element_size, quint32 offset);

};

#endif // CUBE_H
