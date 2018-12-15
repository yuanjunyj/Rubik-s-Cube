#include <QApplication>
#include "openglwidget.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    OpenGLWidget w;
    w.resize(400, 400);
    w.show();

    return app.exec();
}
