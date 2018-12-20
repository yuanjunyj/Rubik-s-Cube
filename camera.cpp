#include "camera.h"
#include <QtMath>

Camera::Camera() :
    m_front(QVector3D(0, 0, -1)),
    m_up(QVector3D(0, 1, 0)),
    m_pos(QVector3D(0, 0, 5))
{

}

QMatrix4x4 Camera::getViewMatrix() {
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(m_pos, m_pos + m_front, m_up);
    return viewMatrix;
}
