#include "openglwidget.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMatrix3x3>

OpenGLWidget::OpenGLWidget(QOpenGLWidget *widget) :
    QOpenGLWidget(widget),
    m_mouseStatus(Released),
    m_keyLock(false)
{
//    setFixedSize(1024, 1024);
}

OpenGLWidget::~OpenGLWidget() {

}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(1, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
//    glEnable(GL_LINE_SMOOTH);
//    glEnable(GL_POLYGON_SMOOTH);
//    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_BLEND);
    initialize();
}

void OpenGLWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);
    m_projectionMatrix.setToIdentity();
    double ratio = (double) width / height;
    m_projectionMatrix.perspective(120, ratio, 0.1, 20);
}

void OpenGLWidget::paintGL() {
    // Shadow
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderShadow();
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

void OpenGLWidget::keyPressEvent(QKeyEvent *event) {
    if (m_keyLock) {
        return;
    }
    switch (event->key()) {
    case Qt::Key_U:
        m_rubik->screw("U");
        break;
    case Qt::Key_D:
        m_rubik->screw("D");
        break;
    case Qt::Key_L:
        m_rubik->screw("L");
        break;
    case Qt::Key_R:
        m_rubik->screw("R");
        break;
    case Qt::Key_F:
        m_rubik->screw("F");
        break;
    case Qt::Key_B:
        m_rubik->screw("B");
        break;
    case Qt::Key_M:
        m_rubik->screw("M");
        break;
    case Qt::Key_E:
        m_rubik->screw("E");
        break;
    case Qt::Key_S:
        m_rubik->screw("S");
        break;
    default:
        return;
    }
    m_keyLock = true;
    update();
}

void OpenGLWidget::render() {
    // Rubik
    QOpenGLShaderProgram* rubik_program = m_rubik->getShader()->getProgram();
    rubik_program->bind();
    rubik_program->setUniformValue("projectionMatrix", m_projectionMatrix);
    rubik_program->setUniformValue("viewMatrix", m_viewMatrix);
    rubik_program->setUniformValue("cameraPos", m_camera->getPosition());
    rubik_program->setUniformValue("lightViewMatrix", m_depthmap->getLightViewMatrix());
    rubik_program->setUniformValue("lightProjectionMatrix", m_depthmap->getLightProjectionMatrix());
    rubik_program->setUniformValue("skybox", 10);
    m_skybox->getSkyBoxTexture()->bind(10);
    rubik_program->setUniformValue(rubik_program->uniformLocation("shadow"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_depthmap->getShadowTexture());
    rubik_program->release();
    m_rubik->render();
    m_skybox->getSkyBoxTexture()->release();
    // SkyBox
    QOpenGLShaderProgram* skybox_program = m_skybox->getShader()->getProgram();
    skybox_program->bind();
    skybox_program->setUniformValue("projectionMatrix", m_projectionMatrix);
    QMatrix4x4 view = m_viewMatrix;
    view.setColumn(3, QVector4D(0, 0, 0, 1));
    skybox_program->setUniformValue("viewMatrix", view);
    skybox_program->release();
    m_skybox->render();
}

void OpenGLWidget::renderShadow() {
    QOpenGLShaderProgram* program = m_depthmap->getShader()->getProgram();
    QOpenGLFramebufferObject* FBO = m_depthmap->getFBO();
    FBO->bind();
    glCullFace(GL_FRONT);
    m_depthmap->setLight(width(), height());
    m_rubik->renderShadow(program);
    glCullFace(GL_BACK);
    FBO->release();
    glViewport(0, 0, width(), height());
}

void OpenGLWidget::initialize() {
    m_camera = new Camera;
    m_rubik = new Rubik;
    m_rubik->setParent(this);
    connect(m_rubik, SIGNAL(screwDone()), this, SLOT(unlockKey()));
    m_skybox = new SkyBox;
    m_viewMatrix = m_camera->getViewMatrix();
    m_depthmap = new DepthMap;
}

void OpenGLWidget::unlockKey() {
    m_keyLock = false;
}
