#include "rubik.h"
#include "openglwidget.h"
#include <QImage>

#define CUBE_LENGTH 1.0
#define SCALE 0.95
#define CUBE_SUM 27 // 3 * 3 * 3
#define CUBE_FACES 6


Rubik::Rubik()
{
    m_materialType = 0;
    m_rotationMatrix.setToIdentity();
    createCubes();
    m_shader = new Shader;
    m_shader->setupShader("://shader/basic.vert", "://shader/basic.frag");
    QString texture_filenames[6] = {"://texture/bamboo.jpg", "://texture/spiral.jpg", "://texture/wall.jpg", "://texture/flower.jpg" ,"://texture/shining.jpg", "://texture/stone.jpg"};
    createImageTexture(texture_filenames);
}

Rubik::~Rubik() {
    for (int i = 0; i < CUBE_FACES; ++i) {
        m_texture[i]->destroy();
        delete m_texture[i];
    }
    delete [] m_texture;
    delete m_shader;
}

void Rubik::setParent(OpenGLWidget* parent) {
    m_parent = parent;
}

void Rubik::setMaterialType(int type) {
    m_materialType = type;
    m_parent->unlockKey();
}

void Rubik::createCubes() {
    m_cubes = new Cube[3 * 3 * 3];
    for (int i = 0; i < CUBE_SUM; ++i) {
        int x = i / 9,
            y = i % 9 / 3,
            z = i % 3;
        m_position[x][y][z] = i;
        m_cubes[i].createBlock();
        if (x == 0) {
            m_cubes[i].setFacet('L', QVector3D(0, 255 / 255., 0), y, z); // Lime
        } else if (x == 2) {
            m_cubes[i].setFacet('R', QVector3D(65 / 255., 105 / 255., 225 / 255.), z, y); // Cyan
        }
        if (y == 0) {
            m_cubes[i].setFacet('D', QVector3D(255 / 255., 165 / 255., 0), z, x); // Orange
        } else if (y == 2) {
            m_cubes[i].setFacet('U', QVector3D(255 / 255., 0, 0), x, z); // Red
        }
        if (z == 0) {
            m_cubes[i].setFacet('B', QVector3D(238 / 255., 130 / 255., 238 / 255.), x, y); // Violet
        } else if (z == 2) {
            m_cubes[i].setFacet('F', QVector3D(255 / 255., 255 / 255., 0), y, x); // Yellow
        }
        m_cubes[i].createPasters();
        m_cubes[i].translate(QVector3D((x - 1) * CUBE_LENGTH, (y - 1) * CUBE_LENGTH, (z - 1) * CUBE_LENGTH));
        m_cubes[i].scale(SCALE);
    }
}

void Rubik::createImageTexture(const QString *filepath) {
    m_texture = new QOpenGLTexture*[6];
    for (int i = 0; i < CUBE_FACES; ++i) {
        m_texture[i] = new QOpenGLTexture(QOpenGLTexture::Target2D);
        QImage image(filepath[i]);
        Q_ASSERT(!filepath[i].isNull());
        m_texture[i]->setData(image);
        m_texture[i]->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_texture[i]->setMagnificationFilter(QOpenGLTexture::Linear);
    }
}

void Rubik::render() {
    QOpenGLShaderProgram* program = m_shader->getProgram();
    program->bind();
    program->setUniformValue("rotationMatrix", m_rotationMatrix);
    program->setUniformValue("material_type", m_materialType);
    program->setUniformValue("useImage", false);
    program->setUniformValue("useColor", false);
    for (int i = 0; i < CUBE_FACES; ++i) {
        const std::string loc = "images[" + std::to_string(i) + "]";
        program->setUniformValue(loc.c_str(), i + 100);
        m_texture[i]->bind(i + 100);
    }
    program->release();
    for (int i = 0; i < CUBE_SUM; ++i) {
        m_cubes[i].render(program);
    }
    for (int i = 0; i < CUBE_FACES; ++i) {
        m_texture[i]->release();
    }
}

void Rubik::renderShadow(QOpenGLShaderProgram *depthProgram) {
    depthProgram->bind();
    depthProgram->setUniformValue("rotationMatrix", m_rotationMatrix);
    for (int i = 0; i < CUBE_SUM; ++i) {
        m_cubes[i].renderShadow(depthProgram);
    }
    depthProgram->release();
}

void Rubik::rotate(float angle, QVector3D axis) {
    QMatrix4x4 r;
    r.setToIdentity();
    r.rotate(angle, axis);
    m_rotationMatrix = r * m_rotationMatrix;
}

void Rubik::screw(QString step) {
    int x = -1, y = -1, z = -1, angle = 0;
    QVector3D axis, layer_center;
    // Break down single step
    QChar layer = step[0], operation = ' ';
    if (step.length() > 1) {
        operation = step[1];
    }
    // Define steps (Dirty code)
    // Layer
    if (layer == 'L') {
        x = 0;
        axis = QVector3D(-1, 0, 0);
    } else if (layer == 'M') {
        x = 1;
        axis = QVector3D(-1, 0, 0);
    } else if (layer == 'R') {
        x = 2;
        axis = QVector3D(1, 0, 0);
    } else if (layer == 'D') {
        y = 0;
        axis = QVector3D(0, -1, 0);
    } else if (layer == 'E') {
        y = 1;
        axis = QVector3D(0, -1, 0);
    } else if (layer == 'U') {
        y = 2;
        axis = QVector3D(0, 1, 0);
    } else if (layer == 'B') {
        z = 0;
        axis = QVector3D(0, 0 , -1);
    } else if (layer == 'S') {
        z = 1;
        axis = QVector3D(0, 0, 1);
    } else if (layer == 'F') {
        z = 2;
        axis = QVector3D(0, 0, 1);
    }
    layer_center = QVector3D(x >= 0 ? x : 1,
                             y >= 0 ? y : 1,
                             z >= 0 ? z : 1);
    // Operation
    if (operation == ' ') {
        angle = -90;
    } else if (operation == '\'') {
        angle = 90;
    } else if (operation == '2') {
        angle = 180;
    }

    // Create animation object
    m_animation = new Animation(this);
    m_animation->setRotationAttributes(angle, axis);
    // Compute new cube position
    int new_position[3][3][3];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k) {
                if ((x != -1 && x != i) || (y != -1 && y != j) || (z != -1 && z != k)) {
                    new_position[i][j][k] = m_position[i][j][k];
                    continue;
                }
                QVector3D representation(i, j, k), next;
                representation -= layer_center;
                if (operation == ' ') {
                    next = QVector3D::crossProduct(representation, axis);
                } else if (operation == '\'') {
                    next = QVector3D::crossProduct(axis, representation);
                } else if (operation == '2') {
                    next = QVector3D::crossProduct(axis, representation);
                    next = QVector3D::crossProduct(axis, next);
                }
                next += layer_center;
                int new_x = next.x(), new_y = next.y(), new_z = next.z();
                new_position[new_x][new_y][new_z] = m_position[i][j][k];
                m_animation->addCube(m_position[i][j][k]);
            }
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k) {
                m_position[i][j][k] = new_position[i][j][k];
            }
    m_animation->start();
}

void Rubik::animationFinished() {
    delete m_animation;
    emit screwDone();
}
