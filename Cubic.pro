QT += core gui widgets opengl

SOURCES += \
    main.cpp \
    openglwidget.cpp \
    cube.cpp \
    vertex.cpp \
    rubik.cpp \
    camera.cpp \
    shader.cpp \
    animation.cpp \
    skybox.cpp \
    depthmap.cpp \
    solver.cpp

HEADERS += \
    openglwidget.h \
    cube.h \
    vertex.h \
    rubik.h \
    camera.h \
    shader.h \
    animation.h \
    skybox.h \
    depthmap.h \
    solver.h

RESOURCES += \
    shaders.qrc \
    images.qrc
