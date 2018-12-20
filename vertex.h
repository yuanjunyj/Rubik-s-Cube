#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QVector3D>


class Vertex
{
public:
    Vertex();
    void setNode(const QVector3D& _position, const QVector3D& _normal, const QVector2D& _texCoord);
    void setColor(const QVector3D& _color);

public:
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
    QVector3D color;
};

#endif // VERTEX_H
