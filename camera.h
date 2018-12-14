#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>


class Camera
{
public:
    Camera();

public:
    QVector3D front, up, pos;
};

#endif // CAMERA_H
