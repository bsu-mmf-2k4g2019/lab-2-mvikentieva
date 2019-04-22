#ifndef WIDGET_H
#define WIDGET_H

#include <QtOpenGL>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget() override;

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

protected:
    void timerEvent(QTimerEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    // Shader programms
    QOpenGLShaderProgram m_prog_pyramid;
    QOpenGLShaderProgram m_prog_hexagonalPrism;
    QOpenGLShaderProgram m_prog_cube;

    // VAOs
    GLuint m_vao_pyramid_id;
    GLuint m_vao_hexagonalPrism_id;
    GLuint m_vao_cube_id;

    // Textures
    GLuint m_texture_bg_id;
    GLuint m_texture_smile_id;

    // Uniforms
    GLint  m_hexagonalPrism_color_id;

    QVector3D camera_pos;
    QVector3D camera_up;
    QVector3D camera_front;
    QVector3D pressPosition;

};

#endif // WIDGET_H
