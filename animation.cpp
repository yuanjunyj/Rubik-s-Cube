#include "animation.h"
#include "rubik.h"
#include "openglwidget.h"

Animation::Speed Animation::s_speed = Animation::Slow;

Animation::Animation(Rubik* rubik) :
    m_rubik(rubik)

{
    connect(this, SIGNAL(finished()), m_rubik, SLOT(animationFinished()));
}

void Animation::setRotationAttributes(int angle, QVector3D axis) {
    m_angle = angle;
    m_axis = axis;
}

void Animation::addCube(int cube_index) {
    m_animated_cubes.push_back(cube_index);
}

void Animation::start() {
    m_current = 0;
    m_timer = new QTimer;
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(animate()));
    if (s_speed == Slow) {
        m_step = m_angle / 45;
        m_timer->start(10);
    } else if (s_speed == Fast) {
        m_step = m_angle / 9;
        m_timer->start(10);
    }
}

void Animation::animate() {
    m_current += m_step;
    for (int& index:m_animated_cubes) {
        m_rubik->m_cubes[index].rotate(m_step, m_axis);
    }
    m_rubik->m_parent->update();
    if (m_current == m_angle) {
        stop();
        emit finished();
    }
}

void Animation::stop() {
    m_timer->stop();
    delete m_timer;
}
