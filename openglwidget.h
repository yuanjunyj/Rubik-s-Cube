#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "camera.h"
#include "rubik.h"
#include "skybox.h"
#include "depthmap.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QOpenGLWidget* widget = Q_NULLPTR);
    ~OpenGLWidget();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void initialize();
    void render();
    void renderShadow();
    void keyUpPressed();
    void keyRightPressed();
    void keyDownPressed();
    void clearLayerRecord();
    void updateLayerRecord(int type);
    void setFocusCubes();
    void cancelFocusCubes();
    void screwCube(int type, int direction, int angle);
    void getScrewDirAngle(QPoint delta);


private:
    Camera* m_camera;
    QPoint m_mousePos;
    enum MouseStatus {
        Released = 0,
        LeftPressed = 1,
        RightPressed = 2
    };
    MouseStatus m_mouseStatus;
    bool m_keyLock;
    QMatrix4x4 m_viewMatrix, m_projectionMatrix;
    Rubik* m_rubik;
    SkyBox* m_skybox;
    DepthMap* m_depthmap;

    int uping;
    int righting;
    int fronting;
    int index;
    int layerRecord[3][10];
    bool signal;

public slots:
    void unlockKey();

};

#endif // OPENGLWIDGET_H
