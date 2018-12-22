#ifndef DEPTHMAP_H
#define DEPTHMAP_H

#include <QOpenGLFramebufferObject>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include "shader.h"


class DepthMap : protected QOpenGLFunctions
{
public:
    DepthMap(int shadow_width = 1024, int shadow_height = 1024);
    ~DepthMap();
    Shader* getShader() const { return m_shader; }
    QOpenGLFramebufferObject* getFBO() const { return m_fbo; }
    GLuint getShadowTexture() const { return m_fbo->texture(); }
    void bindShadowTexture(int pos);
    void releaseShadowTexture();
    void setLight(int window_width, int window_height);
    QMatrix4x4 getLightViewMatrix() const { return m_lightViewMatrix; }
    QMatrix4x4 getLightProjectionMatrix() const { return m_lightProjectionMatrix; }

private:
    int m_shadowWidth, m_shadowHeight;
    QOpenGLFramebufferObject* m_fbo;
    QOpenGLTexture* m_texture;
    QMatrix4x4 m_lightViewMatrix, m_lightProjectionMatrix;
    Shader* m_shader;

};

#endif // DEPTHMAP_H
