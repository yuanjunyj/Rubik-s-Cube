#include "openglwidget.h"
#include <QMouseEvent>

OpenGLWidget::OpenGLWidget(QOpenGLWidget *widget) :
    QOpenGLWidget(widget),
    m_mouseStatus(Released)
{

}

OpenGLWidget::~OpenGLWidget() {

}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    initialize();
}

void OpenGLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    m_projectionMatrix.setToIdentity();
    double ratio = (double) width / height;
    m_projectionMatrix.perspective(90, ratio, 0.1, 10);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render();
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        m_mouseStatus = RightPressed;
    } else if (event->button() == Qt::LeftButton) {
        m_mouseStatus = LeftPressed;
    }
    m_mousePos = event->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
    if (m_mouseStatus == Released) {
        return;
    }
    if (m_mouseStatus == RightPressed) {
        QPoint delta = event->pos() - m_mousePos;
        m_rubik->rotate(delta.x(), m_camera->getViewY());
        m_rubik->rotate(delta.y(), m_camera->getViewX());
        m_mousePos = event->pos();
        update();
    } else if (m_mouseStatus == LeftPressed) {

    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {
    m_mouseStatus = Released;
}

void OpenGLWidget::render() {
    QOpenGLShaderProgram* program = m_rubik->getShader()->getProgram();
    program->bind();
    program->setUniformValue("projectionMatrix", m_projectionMatrix);
    program->setUniformValue("viewMatrix", m_viewMatrix);
    program->release();
    m_rubik->render();
}

void OpenGLWidget::initialize() {
    m_camera = new Camera;
    m_rubik = new Rubik;
    m_viewMatrix = m_camera->getViewMatrix();
}
