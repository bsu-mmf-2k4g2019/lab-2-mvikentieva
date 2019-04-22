#include "glwindow.h"

#include <QImage>
#include <QTime>
#include <iostream>

static const char *vertexShaderSource_pyramid =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aCol;\n"
    "layout (location = 2) in vec2 aTex;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "    ourColor = aCol;\n"
    "    TexCoord = aTex;\n"
    "}\n\0";

static const char *fragmentShaderSource_pyramid =
    "#version 330 core\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D ourBg;\n"
    "uniform sampler2D ourFace;\n"
    "void main()\n"
    "{\n"
    "    FragColor = mix(texture(ourBg, TexCoord), texture(ourFace, TexCoord), 0.5) * vec4(ourColor, 1.0);\n"
    "}\n\0";


static QVector3D PrismPositions[] = {
  QVector3D(-0.3f,   8.0f, -4.5f),
  QVector3D(-5.3f,   3.0f, -9.5f),
  QVector3D(-10.3f, -2.0f, -14.5f),
};


static QVector3D pyramidsPositions[] = {
  QVector3D( 0.0f,  0.0f,  0.0f),
  QVector3D( 2.0f,  5.0f, -15.0f),
  QVector3D(-1.5f, -2.2f, -2.5f),
  QVector3D(-3.8f, -2.0f, -12.3f),
  QVector3D( 2.4f, -0.4f, -3.5f),
  QVector3D(-1.7f,  3.0f, -7.5f),
};

static QVector3D cubePositions[] = {
  QVector3D( 6.5f,  1.0f,  2.5f),
  QVector3D( 6.5f,  5.0f,  2.5f),
  QVector3D( 6.5f,  -3.0f,  4.5f),

};




GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)

    ,camera_up(3.0f, 4.0f, 3.0f)
    ,camera_front(0.0f, 0.0f, -3.0f)

{
}

GLWidget::~GLWidget()
{
}

void GLWidget::timerEvent(QTimerEvent *event)
{
    update();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = 0.30f; // adjust accordingly
    if (event->key() == Qt::Key_W || event->text() == "ц" || event->text() == "Ц")
        camera_pos += cameraSpeed * camera_front;
    if (event->key() == Qt::Key_S || event->text() == "ы" || event->text() == "Ы")
        camera_pos -= cameraSpeed * camera_front;
    if (event->key() == Qt::Key_A || event->text() == "ф" || event->text() == "Ф")
        camera_pos -= QVector3D::crossProduct(camera_front, camera_up).normalized() * cameraSpeed;
    if (event->key() == Qt::Key_D || event->text() == "в" || event->text() == "В")
        camera_pos += QVector3D::crossProduct(camera_front, camera_up).normalized() * cameraSpeed;
    if (event->key() == Qt::Key_Space)
        camera_pos += cameraSpeed * camera_up.normalized();
    if (event->key() == Qt::Key_Backspace)
        camera_pos -= camera_up.normalized()* cameraSpeed ;

}



void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 1, 1, 1);
    glEnable(GL_DEPTH_TEST);


    GLfloat vertices_pyramid[] = {
        // positions          // colors           // texture coords

        1.0f,  -1.0f,1.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f, -1.0f,-1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
       -1.0f, -1.0f,-1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        -1.0f, -1.0f,1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,



       -1.0f,-1.0f,-1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
       -1.0f,-1.0f, 1.0f,    0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


       -1.0f,-1.0f,-1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,    0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,

        1.0f,-1.0f,1.0f,     1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
       -1.0f,-1.0f,1.0f,     0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


        1.0f,-1.0f,-1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,    0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    };


    GLfloat vertices_hexagonalPrism[] = {
        // positions          // colors           // texture coords



        0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        0.0f, 0.0f,-1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
       -1.0f, 0.0f,-1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
       -1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,


        0.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        0.0f, 1.0f,-1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
       -1.0f, 1.0f,-1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
       -1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,


        1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


        1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,

        1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        0.0f, 0.0f,-1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 1.0f,-1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


       -1.0f, 0.0f,-1.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        0.0f, 0.0f,-1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 1.0f,-1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
       -1.0f, 1.0f,-1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


       -1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
       -1.0f, 0.0f,-1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
       -1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
       -1.0f, 1.0f,-1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


       -1.0f,0.0f,0.0f,    1.0f, 0.0f, 0.5f,    1.0f, 1.0f,
        0.0f, 0.0f,1.0f,   0.0f, 1.0f, 1.0f,    1.0f, 0.0f,
       -1.0f, 1.0f,0.0f,   0.0f, 1.0f, 1.0f,    1.0f, 0.0f,
        0.0f, 1.0f,1.0f,   0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
    };


    GLfloat vertices_cube[] = {
           // positions          // colors           // texture coords
           0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
           0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
          -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
          -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left

           0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top right
           0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
          -0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
          -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // top left

          -0.5f,  0.5f,-0.5f,   1.0f, 0.5f, 0.0f,   1.0f, 1.0f,   // top right
          -0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
           0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
           0.5f,  0.5f,-0.5f,   1.0f, 0.0f, 0.5f,   0.0f, 1.0f,   // top left

          -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top right
          -0.5f,  0.5f,-0.5f,   1.0f, 0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
           0.5f,  0.5f,-0.5f,   1.0f, 0.0f, 0.5f,   0.0f, 0.0f,   // bottom left
           0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top left

           0.5f,  0.5f,-0.5f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,   // top right
           0.5f, -0.5f,-0.5f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
           0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
           0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top left

          -0.5f,  0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top right
          -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
          -0.5f, -0.5f,-0.5f,   1.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
          -0.5f,  0.5f,-0.5f,   1.0f, 0.5f, 0.0f,   0.0f, 1.0f,   // top left
       };

    GLuint indices_pyramid[] = {
    0, 1, 2,
    0, 2, 3,

    4, 5, 6,
    7, 8, 9,

    10, 11, 12,
    13, 14, 15,
    };

    GLuint indices_hexagonalPrism[] = {
    0, 1, 2,
    2, 3,0,
    0, 4,3,
    4,0,5,
    5,6,0,
    0,6,1,

    7, 8, 9,
    9, 10,7,
    7,11,10,
    11,7,12,
    12,13,7,
    7,13,8,

    14,15,16,
    16,15,17,

    18,19,20,
    20,19,21,

    22,23,24,
    22,24,25,

    26,27,28,
    26,28,29,


    30,31,32,
    32,31,33,

    34,35,36,
    36,35,37,


 };

    GLuint indices_cube[] = {
    0, 1, 2,
    0, 2, 3,

    4, 5, 6,
    4, 6, 7,

    8, 9, 10,
    8, 10, 11,

    12, 13, 14,
    12, 14, 15,

    16, 17, 18,
    16, 18, 19,

    20, 21, 22,
    20, 22, 23,
};



    GLuint VBO;
    GLuint EBO;

    // Create VAO ids

    glGenVertexArrays(1, &m_vao_pyramid_id);
    glGenVertexArrays(1, &m_vao_hexagonalPrism_id);
    glGenVertexArrays(1, &m_vao_cube_id);





    // Fill data for the container
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(m_vao_hexagonalPrism_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_hexagonalPrism), vertices_hexagonalPrism, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_hexagonalPrism), indices_hexagonalPrism, GL_STATIC_DRAW);
    // Configure how OpenGL will interpret the VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 36 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind current VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind current EBO

    // Fill data for the container
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(m_vao_pyramid_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_pyramid), vertices_pyramid, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_pyramid), indices_pyramid, GL_STATIC_DRAW);
    // Configure how OpenGL will interpret the VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind current VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind current EBO




    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);



    // Bind container VAO to store all buffer settings related to container object
    glBindVertexArray(m_vao_cube_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_cube), vertices_cube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_cube), indices_cube, GL_STATIC_DRAW);
    // Configure how OpenGL will interpret the VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0); // Unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind current VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind current EBO



    // Prepare textures
    QImage bg(":/img/container.jpg");
    bg = bg.convertToFormat(QImage::Format_RGB888);

    glGenTextures(1, &m_texture_bg_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_bg_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bg.width(), bg.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bg.bits());
    glGenerateMipmap(GL_TEXTURE_2D);

    QImage fire(":/img/fire.jpg");
    fire = fire.convertToFormat(QImage::Format_RGB888).mirrored(false, true);

    glGenTextures(1, &m_texture_smile_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_smile_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fire.width(), fire.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, fire.bits());
    glGenerateMipmap(GL_TEXTURE_2D);




    // Prepare shader programms
    m_prog_pyramid.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource_pyramid);
    m_prog_pyramid.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource_pyramid);
    m_prog_pyramid.link();

    m_prog_cube.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource_pyramid);
    m_prog_cube.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource_pyramid);
    m_prog_cube.link();



    m_prog_hexagonalPrism.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource_pyramid);
    m_prog_hexagonalPrism.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource_pyramid);
    m_prog_hexagonalPrism.link();

}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    QTime cur_t = QTime::currentTime();
    int t = cur_t.second() * 1000 + cur_t.msec();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture_bg_id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture_smile_id);

    QMatrix4x4 model;
    QMatrix4x4 view;
    QMatrix4x4 projection;

    view.lookAt(camera_pos, camera_pos + camera_front, camera_up);
    projection.perspective(45.0f, width() / height(), 0.1f, 100.0f);





    glBindVertexArray(m_vao_hexagonalPrism_id);
    m_prog_hexagonalPrism.bind();
    m_prog_hexagonalPrism.setUniformValue("ourBg", 0);
    m_prog_hexagonalPrism.setUniformValue("ourFace", 1);
    m_prog_hexagonalPrism.setUniformValue("view", view);
    m_prog_hexagonalPrism.setUniformValue("projection", projection);

     for (size_t i = 0; i < 3; i++) {
        model.setToIdentity();
        model.translate(PrismPositions[i]);
        model.rotate((t / 10) % 360, QVector3D(-0.3f,  8.0f, -4.5f));
        m_prog_hexagonalPrism.setUniformValue("model", model);
        glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_INT, 0);
     }


    glBindVertexArray(m_vao_pyramid_id);
    m_prog_pyramid.bind();
    m_prog_pyramid.setUniformValue("ourBg", 0);
    m_prog_pyramid.setUniformValue("ourFace", 1);
    m_prog_pyramid.setUniformValue("view", view);
    m_prog_pyramid.setUniformValue("projection", projection);

    for (size_t i = 0; i < 6; i++) {
       model.setToIdentity();
       model.translate(pyramidsPositions[i]);
       model.rotate((t / 20) % 360, QVector3D(1.0f, -2.0f, -0.5f));
       m_prog_pyramid.setUniformValue("model", model);
       glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }




    glBindVertexArray(m_vao_cube_id);
    m_prog_cube.bind();
    m_prog_cube.setUniformValue("ourBg", 0);
    m_prog_cube.setUniformValue("ourFace", 1);
    m_prog_cube.setUniformValue("view", view);
    m_prog_cube.setUniformValue("projection", projection);

    for (size_t i = 0; i < 3; i++) {
        model.setToIdentity();
        model.translate(cubePositions[i]);
        model.rotate((t / 10) % 360,  QVector3D( 6.3f,  3.0f,  2.5f));
        m_prog_cube.setUniformValue("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

}
