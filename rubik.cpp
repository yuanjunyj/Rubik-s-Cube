#include "rubik.h"

#define CUBE_LENGTH 1.0
#define CUBE_SUM 27 // 3 * 3 * 3


Rubik::Rubik()
{
    m_rotationMatrix.setToIdentity();
    generateCubes(CUBE_LENGTH);
    generateShader("://shader/basic.vert", "://shader/basic.frag");
}

void Rubik::generateCubes(double cube_length) {
    m_cubes = new Cube[3 * 3 * 3];
    for (int i = 0; i < CUBE_SUM; ++i) {
        int x = i / 9,
            y = i % 9 / 3,
            z = i % 3;
        if (x == 0) {
            m_cubes[i].setFacetColor('L', QVector3D(0, 255 / 255., 0)); // Lime
        } else if (x == 2) {
            m_cubes[i].setFacetColor('R', QVector3D(255 / 255., 255 / 255., 0)); // Yellow
        }
        if (y == 0) {
            m_cubes[i].setFacetColor('D', QVector3D(255 / 255., 165 / 255., 0)); // Orange
        } else if (y == 2) {
            m_cubes[i].setFacetColor('U', QVector3D(255 / 255., 0, 0)); // Red
        }
        if (z == 0) {
            m_cubes[i].setFacetColor('B', QVector3D(0, 0, 0)); // Black
        } else if (z == 2) {
            m_cubes[i].setFacetColor('F', QVector3D(0, 255 / 255., 255 / 255.)); // Cyan
        }
        m_cubes[i].generateVertices(cube_length * 0.95);
        m_cubes[i].generateColorBuffer();
        m_cubes[i].translate(QVector3D((x - 1) * cube_length, (y - 1) * cube_length, (z - 1) * cube_length));
    }
}

void Rubik::generateShader(const QString& vertexShader, const QString& fragmentShader) {
    m_shader = new Shader;
    m_shader->setupShader(vertexShader, fragmentShader);
}

void Rubik::render() {
    QOpenGLShaderProgram* program = m_shader->getProgram();
    program->bind();
    program->setUniformValue("rotationMatrix", m_rotationMatrix);
    program->release();
    for (int i = 0; i < CUBE_SUM; ++i) {
        m_cubes[i].render(m_shader);
    }
}

void Rubik::rotate(float angle, QVector3D axis) {
    QMatrix4x4 r;
    r.setToIdentity();
    r.rotate(angle, axis);
    m_rotationMatrix = r * m_rotationMatrix;
}
