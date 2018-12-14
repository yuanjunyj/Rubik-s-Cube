#ifndef RUBIK_H
#define RUBIK_H

#include "cube.h"


class Rubik
{
public:
    Rubik();

private:
    void generateCubes(double cube_length);

private:
    Cube* m_cubes;
};

#endif // RUBIK_H
