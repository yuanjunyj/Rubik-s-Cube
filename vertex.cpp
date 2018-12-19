#include "vertex.h"

Vertex::Vertex()
{

}

void Vertex::setNode(const QVector3D& _position, const QVector3D& _normal, const QVector2D& _texCoord) {
    position = _position;
    normal = _normal;
    texCoord = _texCoord;
}

void Vertex::setColor(const QVector3D& _color) {
    color = _color;
}
