#ifndef RUBIK_H
#define RUBIK_H

#include "cube.h"
#include "shader.h"


class Rubik
{
public:
    Rubik();

public:
    void render();
    Shader* getShader() const { return m_shader; }
    void rotate(float angle, QVector3D axis);

private:
    void generateCubes(double cube_length);
    void generateShader(const QString& vertexShader, const QString& fragmentShader);

private:
    Cube* m_cubes;
    Shader* m_shader;
    QMatrix4x4 m_rotationMatrix;
};

#endif // RUBIK_H
