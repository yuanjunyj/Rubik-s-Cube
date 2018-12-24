#include "camera.h"
#include <QtMath>

Camera::Camera() :
    m_front(QVector3D(-1, -1, -1)),
    m_up(QVector3D(0, 1, 0)),
    m_pos(QVector3D(3, 3, 3))
{

}

QMatrix4x4 Camera::getViewMatrix() {
    QMatrix4x4 viewMatrix;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(m_pos, m_pos + m_front, m_up);
    return viewMatrix;
}

void Camera::getCloser() {
    if(m_pos.y() <= 2) {
        return;
    }
    else {
        m_pos = QVector3D(m_pos.x() - 0.2, m_pos.y() - 0.2, m_pos.z() - 0.2);
    }
}

void Camera::getAway() {
    if(m_pos.y() >= 5) {
        return;
    }
    else {
        m_pos = QVector3D(m_pos.x() + 0.5, m_pos.y() + 0.5, m_pos.z() + 0.5);
    }
}
