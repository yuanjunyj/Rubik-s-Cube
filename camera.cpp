#include "camera.h"

Camera::Camera() :
    front(QVector3D(-1, -1, -1)),
    up(QVector3D(0, 1, 0)),
    pos(QVector3D(3, 3, 3))
{

}

QMatrix4x4 Camera::getViewMatrix() {
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(pos, pos + front, up);
    return viewMatrix;
}
