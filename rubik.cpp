#include "rubik.h"

#define CUBE_LENGTH 1.0
#define CUBE_SUM 27 // 3 * 3 * 3


Rubik::Rubik()
{
    generateCubes(CUBE_LENGTH);
}

void Rubik::generateCubes(double cube_length) {
    m_cubes = new Cube[3 * 3 * 3];
    for (int i = 0; i < CUBE_SUM; ++i) {
        m_cubes[i].generateVertices(CUBE_LENGTH);
        int x = i / 3,
            y = i % 9 / 3,
            z = i % 3;
        m_cubes[i].setPosition(QVector3D((x - 1) * CUBE_LENGTH, (y - 1) * CUBE_LENGTH, (z - 1) * CUBE_LENGTH));
    }
}
