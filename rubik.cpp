#include "rubik.h"
#include <QDebug>
#define CUBE_LENGTH 1.0
#define CUBE_SUM 27 // 3 * 3 * 3


Rubik::Rubik()
{
    generateCubes(CUBE_LENGTH);
    generateShader("://shader/basic.vert", "://shader/basic.frag");
}

void Rubik::generateCubes(double cube_length) {
    m_cubes = new Cube[3 * 3 * 3];
    for (int i = 0; i < CUBE_SUM; ++i) {
        m_cubes[i].generateVertices(cube_length * 0.95);
        int x = i / 9,
            y = i % 9 / 3,
            z = i % 3;
        qDebug() << x << y << z;
        m_cubes[i].setPosition(QVector3D((x - 1) * cube_length, (y - 1) * cube_length, (z - 1) * cube_length));
    }
}

void Rubik::generateShader(const QString& vertexShader, const QString& fragmentShader) {
    m_shader = new Shader;
    m_shader->setupShader(vertexShader, fragmentShader);
}

void Rubik::render() {
    for (int i = 0; i < CUBE_SUM; ++i) {
        m_cubes[i].render(m_shader);
    }
}
