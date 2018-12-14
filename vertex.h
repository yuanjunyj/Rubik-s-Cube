#ifndef VERTEX_H
#define VERTEX_H

#include <QVector2D>
#include <QVector3D>


class Vertex
{
public:
    Vertex();
    void set(const QVector3D& _position, const QVector3D& _normal, const QVector2D& _texCoord);

public:
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

#endif // VERTEX_H
