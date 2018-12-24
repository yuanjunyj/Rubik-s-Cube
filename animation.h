#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QVector3D>
#include <QTimer>

class Rubik;


class Animation : public QObject
{
    Q_OBJECT

public:
    Animation(Rubik *rubik);
    void setRotationAttributes(int angle, QVector3D axis);
    void addCube(int cube_index);
    void start();

public:
    enum Speed {
        Slow = 0,
        Fast = 1
    };
    static Speed s_speed;

private:
    Rubik* m_rubik;
    int m_angle, m_step, m_current;
    QVector3D m_axis;
    std::vector<int> m_animated_cubes;
    QTimer *m_timer;

private:
    void stop();

signals:
    void finished();

public slots:
    void animate();

};

#endif // ANIMATION_H
