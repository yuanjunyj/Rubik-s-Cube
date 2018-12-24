#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>


class Camera
{
public:
    Camera();
    QMatrix4x4 getViewMatrix();
    QVector3D getPosition() const { return m_pos; }
    QVector3D getViewX() const { return QVector3D::crossProduct(m_front, m_up); }
    QVector3D getViewY() const { return QVector3D::crossProduct(-m_front, getViewX()); }
    QVector3D getViewZ() const { return -m_front; }
    void getCloser();
    void getAway();

private:
    QVector3D m_front, m_up, m_pos;
};

#endif // CAMERA_H
