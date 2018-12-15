#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QOpenGLWidget *widget) :
    QOpenGLWidget(widget)
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

}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {

}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {

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
