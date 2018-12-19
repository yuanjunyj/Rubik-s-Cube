#include "skybox.h"
#include <QImage>


SkyBox::SkyBox() :
    m_cubemap_texture(QOpenGLTexture::TargetCubeMap),
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer)
{
    initializeOpenGLFunctions();
    createBox();
    initTexture();
    m_shader = new Shader;
    m_shader->setupShader("://shader/environment.vert", "://shader/environment.frag");
}

SkyBox::~SkyBox() {
    destroy();
}

void SkyBox::createBox() {
    GLfloat skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    m_vertexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(skyboxVertices, 6 * 3 * 2 * 3 * sizeof(GLfloat));
    m_vertexBuffer.release();
}

void SkyBox::initTexture() {
    const QImage posx = QImage("://skybox/right.jpg").convertToFormat(QImage::Format_RGBA8888);
    const QImage posy = QImage("://skybox/top.jpg").convertToFormat(QImage::Format_RGBA8888);
    const QImage posz = QImage("://skybox/front.jpg").convertToFormat(QImage::Format_RGBA8888);
    const QImage negx = QImage("://skybox/left.jpg").convertToFormat(QImage::Format_RGBA8888);
    const QImage negy = QImage("://skybox/bottom.jpg").convertToFormat(QImage::Format_RGBA8888);
    const QImage negz = QImage("://skybox/back.jpg").convertToFormat(QImage::Format_RGBA8888);

    m_cubemap_texture.create();
    m_cubemap_texture.setSize(posx.width(), posx.height(), posx.depth());
    m_cubemap_texture.setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_cubemap_texture.allocateStorage();

    m_cubemap_texture.setData(0, 0, QOpenGLTexture::CubeMapPositiveX,
                               QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                               (const void*)posx.constBits(), 0);
    m_cubemap_texture.setData(0, 0, QOpenGLTexture::CubeMapPositiveY,
                               QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                               (const void*)posy.constBits(), 0);
    m_cubemap_texture.setData(0, 0, QOpenGLTexture::CubeMapPositiveZ,
                               QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                               (const void*)posz.constBits(), 0);
    m_cubemap_texture.setData(0, 0, QOpenGLTexture::CubeMapNegativeX,
                               QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                               (const void*)negx.constBits(), 0);
    m_cubemap_texture.setData(0, 0, QOpenGLTexture::CubeMapNegativeY,
                               QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                               (const void*)negy.constBits(), 0);
    m_cubemap_texture.setData(0, 0, QOpenGLTexture::CubeMapNegativeZ,
                               QOpenGLTexture::RGBA, QOpenGLTexture::UInt8,
                               (const void*)negz.constBits(), 0);

    m_cubemap_texture.generateMipMaps();
    m_cubemap_texture.setWrapMode(QOpenGLTexture::ClampToEdge);
    m_cubemap_texture.setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_cubemap_texture.setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void SkyBox::destroy() {
    m_vertexBuffer.destroy();
    m_cubemap_texture.destroy();
}

void SkyBox::render() {
    QOpenGLShaderProgram *program = m_shader->getProgram();

    program->bind();

    m_vertexBuffer.bind();
    int s_aPosLoc = program->attributeLocation("a_position");
    program->enableAttributeArray(s_aPosLoc);
    program->setAttributeBuffer(s_aPosLoc, GL_FLOAT, 0, 3, 0);
    m_vertexBuffer.release();

    program->setUniformValue("skybox", 0);
    m_cubemap_texture.bind(0);

    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);

    glDepthFunc(GL_LESS);

    m_cubemap_texture.release();
    program->release();
}
