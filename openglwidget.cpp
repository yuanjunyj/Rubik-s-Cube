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
    double ratio = (double) width / height;
    glViewport(0, 0, width, height);
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(120, ratio, 0.1, 100);
    m_depthmap->updateLightMatrices(width, height);
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
        cancelFocusCubes();
        clearLayerRecord();
        uping = 0;
        righting = 0;
        fronting = 0;
        index = 0;
        update();
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
    if(m_mouseStatus == LeftPressed) {
        if(layerRecord[index][0] != -1 && m_keyLock == false) {
            QPoint delta = event->pos() - m_mousePos;
            getScrewDirAngle(delta);
        }
        update();
    }
    m_mouseStatus = Released;
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event) {
    if (m_keyLock) {
        return;
    }
    switch (event->key()) {
    case Qt::Key_Up:
        keyUpPressed();
        break;
    case Qt::Key_Right:
        keyRightPressed();
        break;
    case Qt::Key_Down:
        keyDownPressed();
        break;
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
        m_rubik->setMaterialType(event->text().toInt());
        break;
    case Qt::Key_Space:
        m_rubik->togglePasterType();
        break;
    default:
        break;
    }
    update();
}

void OpenGLWidget::wheelEvent(QWheelEvent *event) {
    if(event->delta() > 0) { // 向上
        m_camera->getCloser();
    }
    else {
        m_camera->getAway();
    }
    m_viewMatrix = m_camera->getViewMatrix();
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
    rubik_program->setUniformValue("lightPosition", m_depthmap->getLightPos());
    rubik_program->setUniformValue("skybox", 10);
    m_skybox->getSkyBoxTexture()->bind(10);
    rubik_program->setUniformValue(rubik_program->uniformLocation("shadow"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_depthmap->getShadowTexture());
//    m_depthmap->bindShadowTexture(0);
    rubik_program->release();
    m_rubik->render();
    m_skybox->getSkyBoxTexture()->release();
//    m_depthmap->releaseShadowTexture();

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
    program->bind();
    program->setUniformValue("lightViewMatrix", m_depthmap->getLightViewMatrix());
    program->setUniformValue("lightProjectionMatrix", m_depthmap->getLightProjectionMatrix());
    program->release();
//    glViewport(0, 0, m_depthmap->getShadowMapWidth(), m_depthmap->getShadowMapHeight());
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
    m_depthmap->updateLightMatrices(width(), height());
    uping = 0;
    righting = 0;
    fronting = 0;
    index = 0;
    clearLayerRecord();
}

void OpenGLWidget::unlockKey() {
    m_keyLock = false;
}

void OpenGLWidget::keyUpPressed() {
    m_keyLock = false;
    if(uping == 0) {
        cancelFocusCubes();
        index = 0;
        uping = 1;
        righting = 0;
        fronting = 0;
        clearLayerRecord();
        updateLayerRecord(1);
    }
    else {
        cancelFocusCubes();
        index = (index + 1) % 3;
    }
    setFocusCubes();
}

void OpenGLWidget::keyDownPressed() {
    if(fronting == 0) {
        cancelFocusCubes();
        index = 0;
        fronting = 1;
        uping = 0;
        righting = 0;
        clearLayerRecord();
        updateLayerRecord(0);
    }
    else {
        cancelFocusCubes();
        index = (index + 1) % 3;
    }
    setFocusCubes();
}

void OpenGLWidget::keyRightPressed() {
    if(righting == 0) {
        cancelFocusCubes();
        index = 0;
        righting = 1;
        uping = 0;
        fronting = 0;
        clearLayerRecord();
        updateLayerRecord(2);
    }
    else {
        cancelFocusCubes();
        index = (index + 1) % 3;
    }
    setFocusCubes();
}

void OpenGLWidget::clearLayerRecord() {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 10; j++) {
            layerRecord[i][j] = -1;
        }
    }
}

void OpenGLWidget::updateLayerRecord(int type) {
    m_rubik->getLayerRecord(layerRecord, type);
}

void OpenGLWidget::setFocusCubes() {
    if(layerRecord[index][0] == -1) {
        return;
    }
    for(int i = 0; i < 9; i++) {
        m_rubik->setFocusCube(layerRecord[index][i]);
    }
}

void OpenGLWidget::cancelFocusCubes() {
    if(layerRecord[index][0] == -1) {
        return;
    }
    for(int i = 0; i < 9; i++) {
        m_rubik->cancelFocusCube(layerRecord[index][i]);
    }
}

void OpenGLWidget::screwCube(int type, int direction, int angle) {
    QString temp = "";
    if(angle == 2) {
        if(direction == 0)
            temp = "2";
        else
            temp = "3";
    }
    else {
        if(direction == 1)
            temp = "\'";
    }
    m_keyLock = true;
    if(type == 0) {
        m_rubik->screw("L" + temp);
    }
    else if(type == 1) {
        m_rubik->screw("M" + temp);
    }
    else if(type == 2) {
        m_rubik->screw("R" + temp);
    }
    else if(type == 3) {
        m_rubik->screw("D" + temp);
    }
    else if(type == 4) {
        m_rubik->screw("E" + temp);
    }
    else if(type == 5) {
        m_rubik->screw("U" + temp);
    }
    else if(type == 6) {
        m_rubik->screw("B" + temp);
    }
    else if(type == 7) {
        m_rubik->screw("S" + temp);
    }
    else {
        m_rubik->screw("F" + temp);
    }
}

void OpenGLWidget::getScrewDirAngle(QPoint delta) {
    int key_move;
    int direction, angle;
    if(uping == 1) {
        key_move = delta.x();
        if(abs(key_move) > 250) {
            if(key_move > 0) {
                direction = 0;
                angle = 2;
                //screwCube(layerRecord[index][9], 0, 2);
            }
            else {
                direction = 1;
                angle = 2;
                //screwCube(layerRecord[index][9], 1, 2);
            }
        }
        else {
            if(key_move > 0) {
                direction = 0;
                angle = 1;
                //screwCube(layerRecord[index][9], 0, 1);
            }
            else {
                direction = 1;
                angle = 1;
                //screwCube(layerRecord[index][9], 1, 1);
            }
        }
    }
    else if(righting == 1) {
        key_move = delta.y();
        if(abs(key_move) > 250) {
            if(key_move > 0) {
                direction = 1;
                angle = 2;
                //screwCube(layerRecord[index][9], 1, 2);
            }
            else {
                direction = 0;
                angle = 2;
                //screwCube(layerRecord[index][9], 0, 2);
            }
        }
        else {
            if(key_move > 0) {
                direction = 1;
                angle = 1;
                //screwCube(layerRecord[index][9], 1, 1);
            }
            else {
                direction = 0;
                angle = 1;
                //screwCube(layerRecord[index][9], 0, 1);
            }
        }
    }
    else {
        if(abs(delta.x()) > abs(delta.y())) {
            key_move = delta.x();
            if(abs(key_move) > 250) {
                if(key_move > 0) {
                    direction = 0;
                    angle = 2;
                    //screwCube(layerRecord[index][9], 0, 2);
                }
                else {
                    direction = 1;
                    angle = 2;
                    //screwCube(layerRecord[index][9], 1, 2);
                }
            }
            else {
                if(key_move > 0) {
                    direction = 0;
                    angle = 1;
                    //screwCube(layerRecord[index][9], 0, 1);
                }
                else {
                    direction = 1;
                    angle = 1;
                    //screwCube(layerRecord[index][9], 1, 1);
                }
            }
        }
        else {
            key_move = delta.y();
            if(abs(key_move) > 250) {
                if(key_move > 0) {
                    direction = 1;
                    angle = 2;
                    //screwCube(layerRecord[index][9], 1, 2);
                }
                else {
                    direction = 0;
                    angle = 2;
                    //screwCube(layerRecord[index][9], 0, 2);
                }
            }
            else {
                if(key_move > 0) {
                    direction = 1;
                    angle = 1;
                    //screwCube(layerRecord[index][9], 1, 1);
                }
                else {
                    direction = 0;
                    angle = 1;
                    //screwCube(layerRecord[index][9], 0, 1);
                }
            }
        }
    }
    if(layerRecord[index][9] == 5 || layerRecord[index][9] == 2 || layerRecord[index][9] == 8 || layerRecord[index][9] == 7) {
        direction = 1 - direction;
    }
    screwCube(layerRecord[index][9], direction, angle);
}

