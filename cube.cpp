#include "cube.h"
#include <QOpenGLShaderProgram>

const QVector3D Cube::s_base_color = QVector3D(47 / 255., 79 / 255., 79 / 255.); // DarkSlateGray

Cube::Cube() :
    m_length(1.0),
    m_blockBuffer(QOpenGLBuffer::VertexBuffer),
    m_pasterBuffer(QOpenGLBuffer::VertexBuffer),
    m_texture(QOpenGLTexture::Target2D)
{
    initializeOpenGLFunctions();
    initialize();
}

Cube::~Cube() {
    m_blockBuffer.destroy();
    m_pasterBuffer.destroy();
    delete [] m_facets;
    delete [] m_pasters;
}

void Cube::initialize() {
    // Color
    for (int i =0; i < 6; ++i) {
        m_color[i] = s_base_color;
    }

    // Model Matrix
    m_modelMatrix.setToIdentity();
}

void Cube::createBlock() {
    const QVector3D positions[8] = {
        QVector3D( 0.5, -0.5, 0.5 ),
        QVector3D( 0.5, -0.5, -0.5 ),
        QVector3D( -0.5, -0.5, -0.5 ),
        QVector3D( -0.5, -0.5, 0.5 ),
        QVector3D( 0.5, 0.5, 0.5 ),
        QVector3D( 0.5, 0.5, -0.5 ),
        QVector3D( -0.5, 0.5, -0.5 ),
        QVector3D( -0.5, 0.5, 0.5 )
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

    m_facets = new Vertex[6 * 2 * 3];

    // Front
    m_facets[0].setNode(positions[3], normals[2], texCoords[0]);
    m_facets[1].setNode(positions[0], normals[2], texCoords[2]);
    m_facets[2].setNode(positions[7], normals[2], texCoords[1]);
    m_facets[3].setNode(positions[4], normals[2], texCoords[3]);
    m_facets[4].setNode(positions[7], normals[2], texCoords[1]);
    m_facets[5].setNode(positions[0], normals[2], texCoords[2]);

    // Back
    m_facets[6].setNode(positions[6], normals[5], texCoords[0]);
    m_facets[7].setNode(positions[5], normals[5], texCoords[2]);
    m_facets[8].setNode(positions[2], normals[5], texCoords[1]);
    m_facets[9].setNode(positions[1], normals[5], texCoords[3]);
    m_facets[10].setNode(positions[2], normals[5], texCoords[1]);
    m_facets[11].setNode(positions[5], normals[5], texCoords[2]);

    // Up
    m_facets[12].setNode(positions[7], normals[1], texCoords[0]);
    m_facets[13].setNode(positions[4], normals[1], texCoords[2]);
    m_facets[14].setNode(positions[6], normals[1], texCoords[1]);
    m_facets[15].setNode(positions[5], normals[1], texCoords[3]);
    m_facets[16].setNode(positions[6], normals[1], texCoords[1]);
    m_facets[17].setNode(positions[4], normals[1], texCoords[2]);

    // Down
    m_facets[18].setNode(positions[2], normals[4], texCoords[0]);
    m_facets[19].setNode(positions[1], normals[4], texCoords[2]);
    m_facets[20].setNode(positions[3], normals[4], texCoords[1]);
    m_facets[21].setNode(positions[0], normals[4], texCoords[3]);
    m_facets[22].setNode(positions[3], normals[4], texCoords[1]);
    m_facets[23].setNode(positions[1], normals[4], texCoords[2]);

    // Left
    m_facets[24].setNode(positions[2], normals[3], texCoords[0]);
    m_facets[25].setNode(positions[3], normals[3], texCoords[2]);
    m_facets[26].setNode(positions[6], normals[3], texCoords[1]);
    m_facets[27].setNode(positions[7], normals[3], texCoords[3]);
    m_facets[28].setNode(positions[6], normals[3], texCoords[1]);
    m_facets[29].setNode(positions[3], normals[3], texCoords[2]);

    // Right
    m_facets[30].setNode(positions[0], normals[0], texCoords[0]);
    m_facets[31].setNode(positions[1], normals[0], texCoords[2]);
    m_facets[32].setNode(positions[4], normals[0], texCoords[1]);
    m_facets[33].setNode(positions[5], normals[0], texCoords[3]);
    m_facets[34].setNode(positions[4], normals[0], texCoords[1]);
    m_facets[35].setNode(positions[1], normals[0], texCoords[2]);

    // Facets Buffer
    m_blockBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_blockBuffer.create();
    m_blockBuffer.bind();
    m_blockBuffer.allocate(m_facets, 6 * 2 * 3 * sizeof(Vertex));
    m_blockBuffer.release();
}

void Cube::createPasters() {
    m_pasters_count = 0;
    std::vector<int> outer_facet;
    for (int i = 0; i < 6; ++i) {
        if (m_color[i] != s_base_color) {
            m_pasters_count++;
            outer_facet.push_back(i);
        }
    }
    m_pasters = new Vertex[m_pasters_count * 2 * 3];
    for (unsigned int i = 0; i < outer_facet.size(); ++i) {
        int facet_index = outer_facet[i];
        for (int j = 0; j < 6; ++j) {
                m_pasters[6 * i + j] = m_facets[6 * facet_index + j];
                m_pasters[6 * i + j].setColor(m_color[facet_index]);
        }
    }
    m_pasterBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_pasterBuffer.create();
    m_pasterBuffer.bind();
    m_pasterBuffer.allocate(m_pasters, m_pasters_count * 2 * 3 * sizeof(Vertex));
    m_pasterBuffer.release();
}

void Cube::scale(double ratio) {
    m_modelMatrix.scale(ratio);
    m_length *= ratio;
}

void Cube::translate(QVector3D translation) {
    m_modelMatrix.translate(translation);
}

void Cube::rotate(int angle, QVector3D axis) {
    QMatrix4x4 r;
    r.setToIdentity();
    r.rotate(angle, axis);
    m_modelMatrix = r * m_modelMatrix;
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

    program->setUniformValue("modelMatrix", m_modelMatrix);
    program->setUniformValue("baseColor", s_base_color);

    // Block buffer
    m_blockBuffer.bind();
    int offset = 0;
    setVertexAttribute(program, s_positionLoc, GL_FLOAT, 3, offset);
    offset += 3 * sizeof(GLfloat);
    setVertexAttribute(program, s_normalLoc, GL_FLOAT, 3, offset);
    offset += 3 * sizeof(GLfloat);
    setVertexAttribute(program, s_texCoordLoc, GL_FLOAT, 2, offset);
    m_blockBuffer.release();
    program->setUniformValue("useColor", false);
    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);

    // Paster buffer
    m_pasterBuffer.bind();
    offset = 0;
    setVertexAttribute(program, s_positionLoc, GL_FLOAT, 3, offset);
    offset += 3 * sizeof(GLfloat);
    setVertexAttribute(program, s_normalLoc, GL_FLOAT, 3, offset);
    offset += 3 * sizeof(GLfloat);
    setVertexAttribute(program, s_texCoordLoc, GL_FLOAT, 2, offset);
    offset += 2 * sizeof(GLfloat);
    setVertexAttribute(program, s_colorLoc, GL_FLOAT, 3, offset);
    m_pasterBuffer.release();
    program->setUniformValue("useColor", true);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * m_pasters_count);
    glDepthFunc(GL_LESS);
    program->release();


}

void Cube::setVertexAttribute(QOpenGLShaderProgram* program, int attribute_location, GLenum element_type, quint32 element_size, quint32 offset) {
    program->enableAttributeArray(attribute_location);
    program->setAttributeBuffer(attribute_location, element_type, offset, element_size, sizeof(Vertex));
}

