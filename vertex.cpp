#include "vertex.h"

Vertex::Vertex()
{

}

void Vertex::set(const QVector3D& _position, const QVector3D& _normal, const QVector2D& _texCoord) {
    position = _position;
    normal = _normal;
    texCoord = _texCoord;
}
