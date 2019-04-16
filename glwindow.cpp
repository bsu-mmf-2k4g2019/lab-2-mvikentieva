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

static const char *vertexShaderSource_triangle =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n\0";

static const char *vertexShaderSource_sphere =
        "#version 330 core\n"

        "layout(points) in; \n"
        "layout(line_strip, max_vertices = 2) out;\n"

        "uniform mat4 MVP;\n"

        "in vec3 VNormal[];\n"

        "out vec3 fcolor;\n"

        "void main(void)\n"
        "{\n"
            "float size = 2.5f;\n"

            "fcolor = vec3(0.0f, 0.0f, 1.0f);\n"
            "gl_Position = MVP * gl_in[0].gl_Position;\n"
            "EmitVertex();\n"

            "fcolor = vec3(1.0f, 1.0f, 0.0f);\n"
            "gl_Position = MVP * vec4(gl_in[0].gl_Position.xyz + vec3( VNormal[0].x * size, VNormal[0].y * size, VNormal[0].z * size), 1.0f);\n"
           " EmitVertex();\n"

            "EndPrimitive();\n"
       " }\n";

static const char *fragmentShaderSource_sphere =
        "#version 330 core \n"

        "in vec3 Position;\n"
        "in vec3 Normal;\n"
        "in vec2 TexCoords;\n"

        "out vec4 FragColor;\n"

        "in vec3 fcolor;\n"

        "void main(void)\n"
        "{\n"
           " FragColor = vec4(fcolor, 1.0f);\n"
        "}\n";

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

static const char *fragmentShaderSource_triangle =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 color;\n"
    "void main()\n"
    "{\n"
    "    FragColor = color;\n"
    "}\n\0";

static QVector3D pyramidsPositions[] = {
  QVector3D( 0.0f,  0.0f,  0.0f),
  QVector3D( 2.0f,  5.0f, -15.0f),
  QVector3D(-1.5f, -2.2f, -2.5f),
  QVector3D(-3.8f, -2.0f, -12.3f),
  QVector3D( 2.4f, -0.4f, -3.5f),
  QVector3D(-1.7f,  3.0f, -7.5f),
  QVector3D( 1.3f, -2.0f, -2.5f),
  QVector3D( 1.5f,  2.0f, -2.5f),
  QVector3D( 1.5f,  0.2f, -1.5f),

};

static QVector3D PrismPositions[] = {
 QVector3D(-1.3f,  1.0f, -1.5f)
};

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)

    ,camera_up(0.0f, 1.0f, 0.0f)
    ,camera_front(0.0f, 0.0f, -1.0f)
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

    glClearColor(0, 0, 0, 1);
    glEnable(GL_DEPTH_TEST);

    // Create VAO & VBO, bind it to current GL_ARRAY_BUFFER and load vertices into it
    GLfloat vertices_pyramid[] = {
        // positions          // colors           // texture coords



        1.0f,  -1.0f,1.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,   // top right
        1.0f, -1.0f,-1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
       -1.0f, -1.0f,-1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
        -1.0f, -1.0f,1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top left



        -1.0f,-1.0f,-1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        -1.0f, -1.0f,1.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.0f,1.0f,0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


        -1.0f,-1.0f,-1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f, -1.0f,-1.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.0f,1.0f,0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,

        1.0f,-1.0f,1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        -1.0f, -1.0f,1.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.0f,1.0f,0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


        1.0f,-1.0f,-1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f, // эксп
        1.0f, -1.0f,1.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.0f, 1.0f,0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,



    };
    GLfloat vertices_hexagonalPrism[] = {
        // positions          // colors           // texture coords



        0.0f,  0.0f,1.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,   // top right
        1.0f, 0.0f,1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
       1.0f, 0.0f,0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
        0.0f, 0.0f,-1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top left
       -1.0f, 0.0f,-1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top left
        -1.0f, 0.0f,0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top left
        0.0f,  0.0f,0.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,   // top right


        0.0f,  1.0f,1.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,   // top right
        1.0f, 1.0f,1.0f,   0.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
       1.0f, 1.0f,0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
        0.0f, 1.0f,-1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top left
       -1.0f, 1.0f,-1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top left
        -1.0f, 1.0f,0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top left
        0.0f,  1.0f,0.0f,   1.0f, 0.0f, 0.5f,   1.0f, 1.0f,   // top right



        1.0f,1.0f,1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f, 0.0f,1.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.0f,1.0f,0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
         0.0f,0.0f,1.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


        1.0f,0.0f,1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f, 0.0f,0.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        1.0f,1.0f,1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        1.0f, 1.0f,0.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,

        1.0f,0.0f,0.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f,
        0.0f, 0.0f,-1.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.0f,1.0f,-1.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        1.0f,1.0f,0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


        -1.0f,0.0f,-1.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f, // эксп
        0.0f, 0.0f,-1.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 1.0f,-1.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         -1.0f, 1.0f,-1.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,


        -1.0f,0.0f,0.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f, // эксп
        -1.0f, 0.0f,-1.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f,0.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         -1.0f, 1.0f,-1.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,



        -1.0f,0.0f,0.0f,    1.0f, 0.0f, 0.5f,   1.0f, 1.0f, // эксп
        0.0f, 0.0f,1.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f,0.0f,  0.0f, 1.0f, 1.0f,   1.0f, 0.0f,
         0.0f, 1.0f,1.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,



    };
    GLfloat vertices_triangle[] = {
        0.9f,  0.9f, 0.0f,
        0.7f,  0.9f, 0.0f,
        0.8f,  0.7f, 0.0f
    };
    GLuint indices_pyramid[] = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        7, 8, 9,

        10, 11, 12,
        13, 14, 15




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




        14,15,17,

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


    GLuint indices_triangle[] = {
        0, 1, 2
    };



    GLuint VBO;
    GLuint EBO;

    // Create VAO ids

    glGenVertexArrays(1, &m_vao_pyramid_id);
    glGenVertexArrays(1, &m_vao_triangle_id);
    glGenVertexArrays(1, &m_vao_hexagonalPrism_id);

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



    // Fill data for the triangle
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(m_vao_triangle_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle), vertices_triangle, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_triangle), indices_triangle, GL_STATIC_DRAW);
    // Configure how OpenGL will interpret the VBO data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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

    QImage face(":/img/fire.jpg");
    face = face.convertToFormat(QImage::Format_RGB888).mirrored(false, true);

    glGenTextures(1, &m_texture_smile_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_smile_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, face.width(), face.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, face.bits());
    glGenerateMipmap(GL_TEXTURE_2D);




    // Prepare shader programms
    m_prog_pyramid.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource_pyramid);
    m_prog_pyramid.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource_pyramid);
    m_prog_pyramid.link();


    m_prog_hexagonalPrism.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource_pyramid);
     m_prog_hexagonalPrism.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource_pyramid);
     m_prog_hexagonalPrism.link();


    m_prog_triangle.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource_triangle);
    m_prog_triangle.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource_triangle);
    m_prog_triangle.link();

    // Obtain uniform ids
    m_triangle_color_id = m_prog_triangle.uniformLocation("color");

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

    glBindVertexArray(m_vao_pyramid_id);
    m_prog_pyramid.bind();
    m_prog_pyramid.setUniformValue("ourBg", 0);
    m_prog_pyramid.setUniformValue("ourFace", 1);
     m_prog_pyramid.setUniformValue("view", view);
    m_prog_pyramid.setUniformValue("projection", projection);


    for (size_t i = 0; i < 9; i++) {
        model.setToIdentity();
        model.translate(pyramidsPositions[i]);
        model.rotate((t / 10) % 360, QVector3D(1.0f, 1.0f, 0.0f));
         m_prog_pyramid.setUniformValue("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(m_vao_hexagonalPrism_id);
    m_prog_hexagonalPrism.bind();
    m_prog_hexagonalPrism.setUniformValue("ourBg", 0);
    m_prog_hexagonalPrism.setUniformValue("ourFace", 1);
     m_prog_hexagonalPrism.setUniformValue("view", view);
    m_prog_hexagonalPrism.setUniformValue("projection", projection);
    model.setToIdentity();
    model.translate(PrismPositions[1]);
    model.rotate((t / 10) % 360, QVector3D(-1.3f,  1.0f, -1.5f));
     m_prog_hexagonalPrism.setUniformValue("model", model);
    glDrawElements(GL_TRIANGLES, 144, GL_UNSIGNED_INT, 0);





    // Calculate color for the second triangle
    GLfloat r = sin(t     * 3.14f / 11000.0f) / 2.0f + 0.5f;
    GLfloat g = sin(t * 3 * 3.14f / 17000.0f) / 2.0f + 0.5f;
    GLfloat b = sin(t * 7 * 3.14f / 13000.0f) / 2.0f + 0.5f;

    glBindVertexArray(m_vao_triangle_id);
    m_prog_triangle.bind();
    m_prog_triangle.setUniformValue(m_triangle_color_id, r, g, b, 1.0f);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}
