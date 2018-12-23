#ifndef RUBIK_H
#define RUBIK_H

#include "cube.h"
#include "shader.h"
#include "animation.h"
#include <QObject>
#include <QOpenGLTexture>

class OpenGLWidget;


class Rubik : public QObject
{
    Q_OBJECT

    friend class Animation;

public:
    Rubik();
    ~Rubik();

public:
    void setParent(OpenGLWidget* parent);
    void render();
    void renderShadow(QOpenGLShaderProgram *depthProgram);
    Shader* getShader() const { return m_shader; }
    void rotate(float angle, QVector3D axis);
    void screw(QString step);

private:
    void createCubes();
    void createImageTexture(const QString *filepath);

private:
    OpenGLWidget* m_parent;
    Cube* m_cubes;
    QOpenGLTexture** m_texture;
    Shader* m_shader;
    QMatrix4x4 m_rotationMatrix;
    int m_position[3][3][3];
    Animation *m_animation;
    int m_materialType;

signals:
    void screwDone();

public slots:
    void animationFinished();
};

#endif // RUBIK_H
