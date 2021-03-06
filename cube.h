#ifndef CUBE_H
#define CUBE_H

#include "vertex.h"
#include "shader.h"
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>


class Cube : protected QOpenGLFunctions
{
public:
    Cube();
    ~Cube();

private:
    static const QVector3D s_base_color;
    static const QString s_facets_order;
    double m_length;
    Vertex *m_facets, *m_pasters;
    int m_pasters_count;
    QMatrix4x4 m_modelMatrix;
    QOpenGLBuffer m_blockBuffer, m_pasterBuffer;
    QVector3D m_color[6];
    bool m_isFocused;

public:
    void createBlock();
    QMatrix4x4 getModelMatrices() const { return m_modelMatrix; }
    void setModelMatrix(QMatrix4x4 model) { m_modelMatrix = model; }
    void scale(double ratio);
    void translate(QVector3D translation);
    void rotate(int angle, QVector3D axis); // angle should be multiples of 90
    void setFacet(QChar facet, QVector3D color, int x, int y);
    void createPasters();
    void render(QOpenGLShaderProgram* program);
    void renderShadow(QOpenGLShaderProgram *depthProgram);
    void setFocus();
    void cancelFocus();
    QVector3D getPosition(QMatrix4x4 m_rotationMatrix);

private:
    void initialize();
    void setVertexAttribute(QOpenGLShaderProgram* program, int attribute_location, GLenum element_type, quint32 element_size, quint32 offset);

};

#endif // CUBE_H
