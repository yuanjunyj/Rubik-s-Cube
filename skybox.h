#ifndef SKYBOX_H
#define SKYBOX_H

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include "shader.h"


class SkyBox : protected QOpenGLFunctions
{
public:
    SkyBox();
    ~SkyBox();

public:
    void render();
    QOpenGLTexture* getSkyBoxTexture() const { return m_cubemap_texture; }
    Shader* getShader() const { return m_shader; }

private:
    QOpenGLTexture* m_cubemap_texture;
    Shader* m_shader;
    QOpenGLBuffer m_vertexBuffer;

private:
    void createBox();
    void initTexture();
    void destroy();
};

#endif // SKYBOX_H
