#include "cube.h"
#include <QOpenGLShaderProgram>

#define SHADOW_TEXTURE_UNIT GL_TEXTURE0

//const QVector3D Cube::s_base_color = QVector3D(47 / 255., 79 / 255., 79 / 255.); // DarkSlateGray
const QVector3D Cube::s_base_color = QVector3D(1, 1, 1); // White
const QString Cube::s_facets_order = "FBUDRL";

Cube::Cube() :
    m_length(1.0),
    m_blockBuffer(QOpenGLBuffer::VertexBuffer),
    m_pasterBuffer(QOpenGLBuffer::VertexBuffer)
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

    //is Focused
    m_isFocused = false;
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

    // Right
    m_facets[24].setNode(positions[0], normals[0], texCoords[0]);
    m_facets[25].setNode(positions[1], normals[0], texCoords[2]);
    m_facets[26].setNode(positions[4], normals[0], texCoords[1]);
    m_facets[27].setNode(positions[5], normals[0], texCoords[3]);
    m_facets[28].setNode(positions[4], normals[0], texCoords[1]);
    m_facets[29].setNode(positions[1], normals[0], texCoords[2]);

    // Left
    m_facets[30].setNode(positions[2], normals[3], texCoords[0]);
    m_facets[31].setNode(positions[3], normals[3], texCoords[2]);
    m_facets[32].setNode(positions[6], normals[3], texCoords[1]);
    m_facets[33].setNode(positions[7], normals[3], texCoords[3]);
    m_facets[34].setNode(positions[6], normals[3], texCoords[1]);
    m_facets[35].setNode(positions[3], normals[3], texCoords[2]);

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
                switch (facet_index) {
                case 0:
                case 1:
                    m_pasters[6 * i + j].position.setX(m_pasters[6 * i + j].position.x() * 0.8);
                    m_pasters[6 * i + j].position.setY(m_pasters[6 * i + j].position.y() * 0.8);
                    m_pasters[6 * i + j].position.setZ(m_pasters[6 * i + j].position.z() + 0.01 * (0.5 - facet_index));
                    break;
                case 2:
                case 3:
                    m_pasters[6 * i + j].position.setX(m_pasters[6 * i + j].position.x() * 0.8);
                    m_pasters[6 * i + j].position.setZ(m_pasters[6 * i + j].position.z() * 0.8);
                    m_pasters[6 * i + j].position.setY(m_pasters[6 * i + j].position.y() + 0.01 * (2.5 - facet_index));
                    break;
                case 4:
                case 5:
                    m_pasters[6 * i + j].position.setZ(m_pasters[6 * i + j].position.z() * 0.8);
                    m_pasters[6 * i + j].position.setY(m_pasters[6 * i + j].position.y() * 0.8);
                    m_pasters[6 * i + j].position.setX(m_pasters[6 * i + j].position.x() + 0.01 * (4.5 - facet_index));
                    break;
                default:
                    break;
                }
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

void Cube::setFacet(QChar facet, QVector3D color, int x, int y) {
    int index = s_facets_order.indexOf(facet);
    m_color[index] = color;
    for (int i = 0; i < 6; ++i) {
        m_facets[6 * index + i].texCoord.setX((x + m_facets[6 * index + i].texCoord.x()) / 3);
        m_facets[6 * index + i].texCoord.setY((y + m_facets[6 * index + i].texCoord.y()) / 3);
    }
}

void Cube::render(QOpenGLShaderProgram* program) {
    int positionLoc = program->attributeLocation("position"),
        normalLoc = program->attributeLocation("normal"),
        texCoordLoc = program->attributeLocation("texCoord"),
        colorLoc = program->attributeLocation("color");

    program->setUniformValue("modelMatrix", m_modelMatrix);
    program->setUniformValue("baseColor", s_base_color);

    // Block buffer
    m_blockBuffer.bind();
    int offset = 0;
    setVertexAttribute(program, positionLoc, GL_FLOAT, 3, offset);
    offset += 3 * sizeof(GLfloat);
    setVertexAttribute(program, normalLoc, GL_FLOAT, 3, offset);
    offset += 3 * sizeof(GLfloat);
    setVertexAttribute(program, texCoordLoc, GL_FLOAT, 2, offset);
    m_blockBuffer.release();
    program->setUniformValue("isBlock", true);
    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);

    // Paster buffer
    m_pasterBuffer.bind();
    offset = 0;
    setVertexAttribute(program, positionLoc, GL_FLOAT, 3, offset);
    offset += 3 * sizeof(GLfloat);
    setVertexAttribute(program, normalLoc, GL_FLOAT, 3, offset);
    offset += 3 * sizeof(GLfloat);
    setVertexAttribute(program, texCoordLoc, GL_FLOAT, 2, offset);
    offset += 2 * sizeof(GLfloat);
    setVertexAttribute(program, colorLoc, GL_FLOAT, 3, offset);
    m_pasterBuffer.release();
    program->setUniformValue("isBlock", false);
    program->setUniformValue("isFocused", m_isFocused);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * m_pasters_count);
    glDepthFunc(GL_LESS);
}

void Cube::renderShadow(QOpenGLShaderProgram *depthProgram) {
    m_blockBuffer.bind();
    depthProgram->enableAttributeArray("position");
    depthProgram->setAttributeBuffer("position", GL_FLOAT, 0, 3, sizeof(Vertex));
    depthProgram->setUniformValue("modelMatrix", m_modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);
    m_blockBuffer.release();

    m_pasterBuffer.bind();
    depthProgram->enableAttributeArray("position");
    depthProgram->setAttributeBuffer("position", GL_FLOAT, 0, 3, sizeof(Vertex));
    depthProgram->setUniformValue("modelMatrix", m_modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * m_pasters_count);
    m_pasterBuffer.release();
}

void Cube::setVertexAttribute(QOpenGLShaderProgram* program, int attribute_location, GLenum element_type, quint32 element_size, quint32 offset) {
    program->enableAttributeArray(attribute_location);
    program->setAttributeBuffer(attribute_location, element_type, offset, element_size, sizeof(Vertex));
}

void Cube::setFocus() {
    m_isFocused = true;
}

void Cube::cancelFocus() {
    m_isFocused = false;
}

QVector3D Cube::getPosition(QMatrix4x4 m_rotationMatrix) {
    QMatrix4x4 temp = m_rotationMatrix * m_modelMatrix;
    return QVector3D(temp.row(0).w(), temp.row(1).w(), temp.row(2).w());
}
