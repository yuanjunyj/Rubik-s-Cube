#ifndef RUBIK_H
#define RUBIK_H

#include "cube.h"
#include "shader.h"
#include "animation.h"
#include "solver.h"
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
    void setMaterialType(int type);
    void togglePasterType();
    void render();
    void renderShadow(QOpenGLShaderProgram *depthProgram);
    Shader* getShader() const { return m_shader; }
    void rotate(float angle, QVector3D axis);
    void screw(QString step);
    void setFocusCube(int ord);
    void cancelFocusCube(int ord);
    bool getLayerRecord(int (&layerRecord)[3][10], int type);
    void solve();

private:
    void createCubes();
    void createImageTexture(const QString *filepath);
    void toScrew();

private:
    OpenGLWidget* m_parent;
    Cube* m_cubes;
    QOpenGLTexture** m_texture;
    Shader* m_shader;
    QMatrix4x4 m_rotationMatrix;
    int m_position[3][3][3];
    int m_color[6][3][3];
    Animation *m_animation;
    bool m_screwing, m_useColor, m_useImage;
    int m_materialType;
    int m_screwStepsLeft;
    std::vector<QString> m_solution;
    Solver m_solver;

signals:
    void screwDone();

public slots:
    void animationFinished();
};

#endif // RUBIK_H
