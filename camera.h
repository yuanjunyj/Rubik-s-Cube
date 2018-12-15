#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>


class Camera
{
public:
    Camera();
    QMatrix4x4 getViewMatrix();

public:
    QVector3D front, up, pos;
};

#endif // CAMERA_H
