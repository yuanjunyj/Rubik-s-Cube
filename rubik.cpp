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
        m_position[x][y][z] = i;
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

void Rubik::screw(QString step) {
    int x = -1, y = -1, z = -1, angle;
    QVector3D axis, layer_center;
    // Break down single step
    QChar layer = step[0], operation = ' ';
    if (step.length() > 1) {
        operation = step[1];
    }
    // Define steps (Dirty code)
    // Layer
    if (step == 'L') {
        x = 0;
        axis = QVector3D(-1, 0, 0);
    } else if (step == 'M') {
        x = 1;
        axis = QVector3D(-1, 0, 0);
    } else if (step == 'R') {
        x = 2;
        axis = QVector3D(1, 0, 0);
    } else if (step == 'D') {
        y = 0;
        axis = QVector3D(0, -1, 0);
    } else if (step == 'E') {
        y = 1;
        axis = QVector3D(0, -1, 0);
    } else if (step == 'U') {
        y = 2;
        axis = QVector3D(0, 1, 0);
    } else if (step == 'B') {
        z = 0;
        axis = QVector3D(0, 0 , -1);
    } else if (step == 'S') {
        z = 1;
        axis = QVector3D(0, 0, 1);
    } else if (step == 'F') {
        z = 2;
        axis = QVector3D(0, 0, 1);
    }
    layer_center = QVector3D(x >= 0 ? x : 1,
                             y >= 0 ? y : 1,
                             z >= 0 ? z : 1);
    // Operation
    if (operation == ' ') {
        angle = -90;
    } else if (operation == '\'') {
        angle = 90;
    } else if (operation == '2') {
        angle = 180;
    }

    // Compute new cube position
    int new_position[3][3][3];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k) {
                if ((x != -1 && x != i) || (y != -1 && y != j) || (z != -1 && z != k)) {
                    new_position[i][j][k] = m_position[i][j][k];
                    continue;
                }
                QVector3D representation(i, j, k), next;
                representation -= layer_center;
                if (operation == ' ') {
                    next = QVector3D::crossProduct(representation, axis);
                } else if (operation == '\'') {
                    next = QVector3D::crossProduct(axis, representation);
                } else if (operation == '2') {
                    next = QVector3D::crossProduct(axis, representation);
                    next = QVector3D::crossProduct(axis, next);
                }
                next += layer_center;
                int new_x = next.x(), new_y = next.y(), new_z = next.z();
                new_position[new_x][new_y][new_z] = m_position[i][j][k];
                m_cubes[m_position[i][j][k]].rotate(angle, axis);
            }
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k) {
                m_position[i][j][k] = new_position[i][j][k];
            }
}
