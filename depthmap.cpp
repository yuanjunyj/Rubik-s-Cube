#include "depthmap.h"

DepthMap::DepthMap(int shadow_width, int shadow_height) :
    m_shadowWidth(shadow_width),
    m_shadowHeight(shadow_height)
{
    initializeOpenGLFunctions();
    m_fbo = new QOpenGLFramebufferObject(shadow_width, shadow_height, GL_TEXTURE_2D);
    m_shader = new Shader;
    m_shader->setupShader("://shader/depth.vert", "://shader/depth.frag");
}

DepthMap::~DepthMap() {
    delete m_fbo;
}

void DepthMap::bindShadowTexture(int pos) {
    m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    QImage image = m_fbo->toImage(true);
    m_texture->create();
    m_texture->setData(image, QOpenGLTexture::MipMapGeneration::GenerateMipMaps);
    m_texture->bind(pos);
}

void DepthMap::releaseShadowTexture() {
    m_texture->release();
    m_texture->destroy();
    delete m_texture;
}

void DepthMap::setLight(int window_width, int window_height) {
    m_lightViewMatrix.setToIdentity();
    m_lightViewMatrix.lookAt(QVector3D(5, 5, -5), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    m_lightProjectionMatrix.setToIdentity();
    m_lightProjectionMatrix.perspective(120, (double) window_width / window_height, 0.1, 20);

    glViewport(0, 0, m_shadowWidth, m_shadowHeight);

    QOpenGLShaderProgram* program = m_shader->getProgram();
    program->bind();
    program->setUniformValue("lightViewMatrix", m_lightViewMatrix);
    program->setUniformValue("lightProjectionMatrix", m_lightProjectionMatrix);
    program->release();
}
