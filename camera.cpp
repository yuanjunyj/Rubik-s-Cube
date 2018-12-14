#include "camera.h"

Camera::Camera() :
    front(QVector3D(0, 0, -1)),
    up(QVector3D(0, 1, 0)),
    pos(QVector3D(0, 0, 5))
{

}
