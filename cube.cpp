#include "cube.h"
#include <QOpenGLShaderProgram>

Cube::Cube() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_colorBuffer(QOpenGLBuffer::VertexBuffer),
    m_texture(QOpenGLTexture::Target2D)
{
    initializeOpenGLFunctions();
    initialize();
}

Cube::~Cube() {
    m_vertexBuffer.destroy();
    m_colorBuffer.destroy();
    delete [] m_vertices;
}

void Cube::initialize() {
    // Color
    for (int i =0; i < 6; ++i) {
        m_color[i] = QVector3D(47 / 255., 79 / 255., 79 / 255.); // DarkSlateGray
    }

    // Model Matrix
    m_modelMatrix.setToIdentity();
}

void Cube::generateVertices(double length) {
    m_length = length;
    double half_length = length / 2.0;
    const QVector3D positions[8] = {
        QVector3D( half_length, -half_length, half_length ),
        QVector3D( half_length, -half_length, -half_length ),
        QVector3D( -half_length, -half_length, -half_length ),
        QVector3D( -half_length, -half_length, half_length ),
        QVector3D( half_length, half_length, half_length ),
        QVector3D( half_length, half_length, -half_length ),
        QVector3D( -half_length, half_length, -half_length ),
        QVector3D( -half_length, half_length, half_length )
    };
    const QVector3D normals[6] = {
        QVector3D( 1.0, 0.0, 0.0 ),
        QVector3D( 0.0, 1.0, 0.0 ),
        QVector3D( 0.0, 0.0, 1.0 ),
        QVector3D( -1.0, 0.0, 0.0 ),
        QVector3D( 0.0, -1.0, 0.0 ),
        QVector3D( 0.0, 0.0, -1.0 )
    };
    const QVector2D texCoords[4] = {
        QVector2D( 0.0, 0.0 ),
        QVector2D( 0.0, 1.0 ),
        QVector2D( 1.0, 0.0 ),
        QVector2D( 1.0, 1.0 )
    };

    m_vertices = new Vertex[6 * 2 * 3];

    // Front
    m_vertices[0].set(positions[3], normals[2], texCoords[0]);
    m_vertices[1].set(positions[0], normals[2], texCoords[2]);
    m_vertices[2].set(positions[7], normals[2], texCoords[1]);
    m_vertices[3].set(positions[4], normals[2], texCoords[3]);
    m_vertices[4].set(positions[7], normals[2], texCoords[1]);
    m_vertices[5].set(positions[0], normals[2], texCoords[2]);

    // Back
    m_vertices[6].set(positions[6], normals[5], texCoords[0]);
    m_vertices[7].set(positions[5], normals[5], texCoords[2]);
    m_vertices[8].set(positions[2], normals[5], texCoords[1]);
    m_vertices[9].set(positions[1], normals[5], texCoords[3]);
    m_vertices[10].set(positions[2], normals[5], texCoords[1]);
    m_vertices[11].set(positions[5], normals[5], texCoords[2]);

    // Up
    m_vertices[12].set(positions[7], normals[1], texCoords[0]);
    m_vertices[13].set(positions[4], normals[1], texCoords[2]);
    m_vertices[14].set(positions[6], normals[1], texCoords[1]);
    m_vertices[15].set(positions[5], normals[1], texCoords[3]);
    m_vertices[16].set(positions[6], normals[1], texCoords[1]);
    m_vertices[17].set(positions[4], normals[1], texCoords[2]);

    // Down
    m_vertices[18].set(positions[2], normals[4], texCoords[0]);
    m_vertices[19].set(positions[1], normals[4], texCoords[2]);
    m_vertices[20].set(positions[3], normals[4], texCoords[1]);
    m_vertices[21].set(positions[0], normals[4], texCoords[3]);
    m_vertices[22].set(positions[3], normals[4], texCoords[1]);
    m_vertices[23].set(positions[1], normals[4], texCoords[2]);

    // Left
    m_vertices[24].set(positions[2], normals[3], texCoords[0]);
    m_vertices[25].set(positions[3], normals[3], texCoords[2]);
    m_vertices[26].set(positions[6], normals[3], texCoords[1]);
    m_vertices[27].set(positions[7], normals[3], texCoords[3]);
    m_vertices[28].set(positions[6], normals[3], texCoords[1]);
    m_vertices[29].set(positions[3], normals[3], texCoords[2]);

    // Right
    m_vertices[30].set(positions[0], normals[0], texCoords[0]);
    m_vertices[31].set(positions[1], normals[0], texCoords[2]);
    m_vertices[32].set(positions[4], normals[0], texCoords[1]);
    m_vertices[33].set(positions[5], normals[0], texCoords[3]);
    m_vertices[34].set(positions[4], normals[0], texCoords[1]);
    m_vertices[35].set(positions[1], normals[0], texCoords[2]);

    // Vertex Buffer
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(m_vertices, 6 * 2 * 3 * sizeof(Vertex));
    m_vertexBuffer.release();
}

void Cube::generateColorBuffer() {
    QVector3D *colors = new QVector3D[6 * 2 * 3];
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            colors[6 * i + j] = m_color[i];
        }
    }

    // Color Buffer
    m_colorBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_colorBuffer.create();
    m_colorBuffer.bind();
    m_colorBuffer.allocate(colors, 6 * 2 * 3 * sizeof(QVector3D));
    m_colorBuffer.release();

}

void Cube::translate(QVector3D translation) {
    m_modelMatrix.translate(translation);
}

void Cube::setFacetColor(QChar facet, QVector3D color) {
    const QString facets_order = "FBUDLR";
    m_color[facets_order.indexOf(facet)] = color;
}

void Cube::render(Shader* shader) {
    QOpenGLShaderProgram* program = shader->getProgram();

    int s_positionLoc = program->attributeLocation("position"),
        s_normalLoc = program->attributeLocation("normal"),
        s_texCoordLoc = program->attributeLocation("texCoord"),
        s_colorLoc = program->attributeLocation("color");

    program->bind();

    // Data buffer
    m_vertexBuffer.bind();
    int offset = 0;
    setVertexAttribute(program, s_positionLoc, GL_FLOAT, 3, offset);
    offset += 3 * sizeof(GLfloat);
    setVertexAttribute(program, s_normalLoc, GL_FLOAT, 3, offset);
    offset += 3 * sizeof(GLfloat);
    setVertexAttribute(program, s_texCoordLoc, GL_FLOAT, 2, offset);
    m_vertexBuffer.release();

    m_colorBuffer.bind();
    setColorAttribute(program, s_colorLoc, GL_FLOAT, 3, 0);
    m_colorBuffer.release();

    program->setUniformValue("modelMatrix", m_modelMatrix);

    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);

    program->release();
}

void Cube::setVertexAttribute(QOpenGLShaderProgram* program, int attribute_location, GLenum element_type, quint32 element_size, quint32 offset) {
    program->enableAttributeArray(attribute_location);
    program->setAttributeBuffer(attribute_location, element_type, offset, element_size, sizeof(Vertex));
}

void Cube::setColorAttribute(QOpenGLShaderProgram* program, int attribute_location, GLenum element_type, quint32 element_size, quint32 offset) {
    program->enableAttributeArray(attribute_location);
    program->setAttributeBuffer(attribute_location, element_type, offset, element_size, 0);
}
