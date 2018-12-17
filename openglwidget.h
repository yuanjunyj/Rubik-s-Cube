#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "camera.h"
#include "rubik.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
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

private:
    void initialize();
    void render();

private:
    Camera* m_camera;
    QPoint m_mousePos;
    enum MouseStatus {
        Released = 0,
        LeftPressed = 1,
        RightPressed = 2
    };
    MouseStatus m_mouseStatus;
    QMatrix4x4 m_viewMatrix, m_projectionMatrix;
    Rubik* m_rubik;

};

#endif // OPENGLWIDGET_H
