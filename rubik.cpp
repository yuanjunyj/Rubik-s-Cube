#include "rubik.h"
#include "openglwidget.h"
#include <QImage>

#define CUBE_LENGTH 1.0
#define SCALE 0.95
#define CUBE_SUM 27 // 3 * 3 * 3
#define CUBE_FACES 6

int new_position[3][3][3];
int new_color[6][3][3];
int facetMoveTo[6];
const QVector3D normals[6] = {
    QVector3D(0, 0, 1),
    QVector3D(0, 0, -1),
    QVector3D(0, 1, 0),
    QVector3D(0, -1, 0),
    QVector3D(1, 0, 0),
    QVector3D(-1, 0, 0)
}; // Same order as Cube::s_facets_order
int current_step = 0;

Rubik::Rubik()
{
    m_screwing = false;
    m_useColor = true;
    m_useImage = false;
    m_materialType = 0;
    m_screwStepsLeft = 0;
    m_rotationMatrix.setToIdentity();
    createCubes();
    for (int k = 0; k < CUBE_FACES; ++k)
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                m_color[k][i][j] = k;
            }
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

void Rubik::togglePasterType() {
    m_useColor = !m_useColor;
    m_useImage = !m_useImage;
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
    program->setUniformValue("useImage", m_useImage);
    program->setUniformValue("useColor", m_useColor);
    program->setUniformValue("focus", false);
    for (int i = 0; i < CUBE_FACES; ++i) {
        const std::string loc = "images[" + std::to_string(i) + "]";
        program->setUniformValue(loc.c_str(), i + 100);
        m_texture[i]->bind(i + 100);
    }

    for (int i = 0; i < CUBE_SUM; ++i) {
        m_cubes[i].render(program);
    }

    for (int i = 0; i < CUBE_FACES; ++i) {
        m_texture[i]->release();
    }
    program->release();
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
        angle = -180;
    } else if (operation == '3') {
        angle = 180;
    }

    // Create animation object
    m_animation = new Animation(this);
    m_animation->setRotationAttributes(angle, axis);

    // Compute new cube position and facet color;
    for (int i = 0; i < CUBE_FACES; ++i) {
        QVector3D original = normals[i], next;
        if (operation == ' ') {
            next = QVector3D::crossProduct(original, axis);
        } else if (operation == '\'') {
            next = QVector3D::crossProduct(axis, original);
        } else if (operation == '2') {
            next = QVector3D::crossProduct(axis, original);
            next = QVector3D::crossProduct(axis, next);
        } else if (operation == '3') {
            next = QVector3D::crossProduct(axis, original);
            next = QVector3D::crossProduct(axis, next);
        }
        if (next.x() == 0 && next.y() == 0 && next.z() == 0) {
            facetMoveTo[i] = i;
        } else {
            for (int j = 0; j < CUBE_FACES; ++j) {
                if (normals[j] == next) {
                    facetMoveTo[i] = j;
                    break;
                }
            }
        }
    }

    for (int k = 0; k < CUBE_FACES; ++k)
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                new_color[k][i][j] = m_color[k][i][j];
            }

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
                } else if (operation == '3') {
                    next = QVector3D::crossProduct(axis, representation);
                    next = QVector3D::crossProduct(axis, next);
                }
                next += layer_center;
                int new_i = next.x(), new_j = next.y(), new_k = next.z();
                new_position[new_i][new_j][new_k] = m_position[i][j][k];
                for (int f = 0; f < CUBE_FACES; f++) {
                    int color = -1, new_f = facetMoveTo[f];
                    switch (f) {
                    case 0:
                        if (k == 2) {
                            color = m_color[f][i][j];
                        }
                        break;
                    case 1:
                        if (k == 0) {
                            color = m_color[f][i][j];
                        }
                        break;
                    case 2:
                        if (j == 2) {
                            color = m_color[f][i][k];
                        }
                        break;
                    case 3:
                        if (j == 0) {
                            color = m_color[f][i][k];
                        }
                        break;
                    case 4:
                        if (i == 2) {
                            color = m_color[f][j][k];
                        }
                        break;
                    case 5:
                        if (i == 0) {
                            color = m_color[f][j][k];
                        }
                        break;
                    default:
                        break;
                    }
                    if (color == -1) {
                        continue;
                    }
                    switch (new_f) {
                    case 0:
                    case 1:
                        new_color[new_f][new_i][new_j] = color;
                        break;
                    case 2:
                    case 3:
                        new_color[new_f][new_i][new_k] = color;
                        break;
                    case 4:
                    case 5:
                        new_color[new_f][new_j][new_k] = color;
                        break;
                    default:
                        break;
                    }
                }

                m_animation->addCube(m_position[i][j][k]);
            }

    m_screwing = true;
    m_animation->start();
}

void Rubik::toScrew() {
    --m_screwStepsLeft;
    screw(m_solution[current_step]);
    ++current_step;
}

void Rubik::animationFinished() {
    delete m_animation;
    m_screwing = false;

    // Apply new position and facet color
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k) {
                m_position[i][j][k] = new_position[i][j][k];
            }
    for (int k = 0; k < CUBE_FACES; ++k)
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                m_color[k][i][j] = new_color[k][i][j];
            }
    if (m_screwStepsLeft == 0) {
        emit screwDone();
    } else {
        toScrew();
    }
}

void Rubik::setFocusCube(int ord) {
    m_cubes[ord].setFocus();
}

void Rubik::cancelFocusCube(int ord) {
    m_cubes[ord].cancelFocus();
}

void Rubik::getLayerRecord(int (&layerRecord)[3][10], int type) {
    int sideCenterCubes[6];
    sideCenterCubes[0] = m_position[0][1][1];
    sideCenterCubes[1] = m_position[1][0][1];
    sideCenterCubes[2] = m_position[1][1][0];
    sideCenterCubes[3] = m_position[2][1][1];
    sideCenterCubes[4] = m_position[1][2][1];
    sideCenterCubes[5] = m_position[1][1][2];

    QVector3D sideCenterCubesPos[6];
    for(int i = 0; i < 6; i++) {
        sideCenterCubesPos[i] = m_cubes[sideCenterCubes[i]].getPosition(m_rotationMatrix);
    }

    int Ord = -1;
    double m_min = 100;
    double m_max = -100;
    for(int i = 0; i < 6; i++) {
        if(type == 2) {
            if(sideCenterCubesPos[i][type] > m_max) {
                Ord = i;
                m_max = sideCenterCubesPos[i][type];
            }
        }
        else {
            if(sideCenterCubesPos[i][type] < m_min) {
                Ord = i;
                m_min = sideCenterCubesPos[i][type];
            }
        }
    }

    if(Ord == 0) {
        for(int k = 0; k < 3; k++) {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    layerRecord[k][i * 3 + j] = m_position[k][i][j];
                }
            }
        }
        layerRecord[0][9] = 0;
        layerRecord[1][9] = 1;
        layerRecord[2][9] = 2;
    }
    else if(Ord == 1) {
        for(int k = 0; k < 3; k++) {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    layerRecord[k][i * 3 + j] = m_position[i][k][j];
                }
            }
        }
        layerRecord[0][9] = 3;
        layerRecord[1][9] = 4;
        layerRecord[2][9] = 5;
    }
    else if(Ord == 2) {
        for(int k = 0; k < 3; k++) {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    layerRecord[k][i * 3 + j] = m_position[i][j][k];
                }
            }
        }
        layerRecord[0][9] = 6;
        layerRecord[1][9] = 7;
        layerRecord[2][9] = 8;
    }
    else if(Ord == 3) {
        for(int k = 2; k >= 0; k--) {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    layerRecord[2 - k][i * 3 + j] = m_position[k][i][j];
                }
            }
        }
        layerRecord[0][9] = 2;
        layerRecord[1][9] = 1;
        layerRecord[2][9] = 0;
    }
    else if(Ord == 4) {
        for(int k = 2; k >= 0; k--) {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    layerRecord[2 - k][i * 3 + j] = m_position[i][k][j];
                }
            }
        }
        layerRecord[0][9] = 5;
        layerRecord[1][9] = 4;
        layerRecord[2][9] = 3;
    }
    else {
        for(int k = 2; k >= 0; k--) {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    layerRecord[2 - k][i * 3 + j] = m_position[i][j][k];
                }
            }
        }
        layerRecord[0][9] = 8;
        layerRecord[1][9] = 7;
        layerRecord[2][9] = 6;
    }
}

void Rubik::solve() {
    m_solution.clear();
    m_solution.push_back("U");
    m_solution.push_back("L'");
    m_screwStepsLeft = m_solution.size();
    current_step = 0;
    toScrew();
}
