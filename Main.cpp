#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderClass.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BezierCurve.h"
#include <array>
#include "stb_image.h"
#include "VAO.h"
#include "VBO.h"

// VERTICES DEL FONDO
float backgroundVertices[] =
{
    // Posición       // Textura
    -1.0f,  1.0f,     0.0f, 1.0f,
    -1.0f, -1.0f,     0.0f, 0.0f,
     1.0f, -1.0f,     1.0f, 0.0f,

    -1.0f,  1.0f,     0.0f, 1.0f,
     1.0f, -1.0f,     1.0f, 0.0f,
     1.0f,  1.0f,     1.0f, 1.0f
};

GLuint backgroundVAO, backgroundVBO;

GLuint backgroundTexture = 0;

glm::vec3 CalculateBezier(
    const std::array<glm::vec3, 4>& points,
    float t
)
{
    float u = 1.0f - t;

    return
        u * u * u * points[0] +
        3.0f * u * u * t * points[1] +
        3.0f * u * t * t * points[2] +
        t * t * t * points[3];
}

glm::vec3 CalculateBezierTangent(
    const std::array<glm::vec3, 4>& points,
    float t
)
{
    float u = 1.0f - t;

    return glm::normalize(
        3.0f * u * u * (points[1] - points[0]) +
        6.0f * u * t * (points[2] - points[1]) +
        3.0f * t * t * (points[3] - points[2])
    );
}

int main()
{

    // ==========================================
    // INICIALIZAR GLFW
    // ==========================================

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        800,
        800,
        "Nave Espacial - OpenGL",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Error al inicializar GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 800);
    glEnable(GL_DEPTH_TEST);

    // ==========================================
    // SHADER
    // ==========================================

    Shader shaderProgram("default.vert", "default.frag");
    shaderProgram.Activate();

    // SHADER DEL FONDO
    Shader backgroundShader(
        "background.vert",
        "background.frag"
    );

    // ==========================================
// VÉRTICES DEL BACKGROUND
// ==========================================

    GLfloat backgroundVertices[] =
    {
        // POSICIONES       // COORDENADAS UV

        -1.0f,  1.0f,       0.0f, 1.0f,
        -1.0f, -1.0f,       0.0f, 0.0f,
         1.0f, -1.0f,       1.0f, 0.0f,

        -1.0f,  1.0f,       0.0f, 1.0f,
         1.0f, -1.0f,       1.0f, 0.0f,
         1.0f,  1.0f,       1.0f, 1.0f
    };

    // Crear VAO y VBO
    VAO backgroundVAO;
    backgroundVAO.Bind();

    VBO backgroundVBO(
        backgroundVertices,
        sizeof(backgroundVertices)
    );

    // Posición
    backgroundVAO.LinkVBO(
        backgroundVBO,
        0,
        2,
        4 * sizeof(GLfloat),
        0
    );

    // Coordenadas de textura
    backgroundVAO.LinkVBO(
        backgroundVBO,
        1,
        2,
        4 * sizeof(GLfloat),
        2 * sizeof(GLfloat)
    );

    backgroundVAO.Unbind();
    backgroundVBO.Unbind();
    // ==========================================
// CARGAR TEXTURA DEL BACKGROUND
// ==========================================

    int backgroundWidth;
    int backgroundHeight;
    int backgroundChannels;

    // Invertir imagen verticalmente
    stbi_set_flip_vertically_on_load(true);

    unsigned char* backgroundBytes = stbi_load(
        "fondoespacial.png",
        &backgroundWidth,
        &backgroundHeight,
        &backgroundChannels,
        STBI_rgb_alpha
    );

    GLuint backgroundTexture;

    glGenTextures(1, &backgroundTexture);

    glBindTexture(
        GL_TEXTURE_2D,
        backgroundTexture
    );

    if (backgroundBytes)
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            backgroundWidth,
            backgroundHeight,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            backgroundBytes
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE
        );

        glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE
        );

        std::cout << "Background cargado correctamente" << std::endl;
    }
    else
    {
        std::cout << "Error cargando fondoespacial.png" << std::endl;
    }

    stbi_image_free(backgroundBytes);

    glBindTexture(GL_TEXTURE_2D, 0);

    // ==========================================
    // CÁMARA
    // ==========================================
    
    float cameraAngle = 0.0f;
    float cameraHeight = 0.0f;
    float cameraDistance = 5.0f;

    Shader curveShader("bezier.vert", "bezier.frag");

    // Cuatro puntos de control
    // PRIMER SEGMENTO
    std::array<glm::vec3, 4> points1 = {
        glm::vec3(-2.5f,  0.0f,  0.0f),
        glm::vec3(-2.0f,  1.5f,  0.0f),
        glm::vec3(-1.3f,  1.5f,  0.0f),
        glm::vec3(-0.8f,  0.0f,  0.0f)
    };

    // SEGUNDO SEGMENTO
    std::array<glm::vec3, 4> points2 = {
        glm::vec3(-0.8f,  0.0f,  0.0f),
        glm::vec3(-0.3f, -1.5f,  0.0f),
        glm::vec3(0.3f, -1.5f,  0.0f),
        glm::vec3(0.8f,  0.0f,  0.0f)
    };

    // TERCER SEGMENTO
    std::array<glm::vec3, 4> points3 = {
        glm::vec3(0.8f,  0.0f,  0.0f),
        glm::vec3(1.3f,  1.5f,  0.0f),
        glm::vec3(2.0f,  1.5f,  0.0f),
        glm::vec3(2.5f,  0.0f,  0.0f)
    };

    // Crear las tres curvas
    BezierCurve curve1(points1, 100);
    BezierCurve curve2(points2, 100);
    BezierCurve curve3(points3, 100);

    // ==========================================
    // CARGAR Y DIBUJAR LA NAVE
    // ==========================================


    {
        Model spaceship("Models/Spaceship.glb");

        while (!glfwWindowShouldClose(window))
        {
            // ==================================
            // CONTROLES DE CÁMARA
            // ==================================

            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                cameraAngle += 0.02f;

            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                cameraAngle -= 0.02f;

            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                cameraHeight += 0.02f;

            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                cameraHeight -= 0.02f;

            // ==================================
            // LIMPIAR PANTALLA
            // ==================================

            glClearColor(0.02f, 0.03f, 0.08f, 1.0f);

            glClear(
                GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT
            );
            // ==========================================
            // DIBUJAR BACKGROUND
            // ==========================================

            // Desactivar profundidad temporalmente
            glDisable(GL_DEPTH_TEST);

            // Activar shader
            backgroundShader.Activate();

            // Activar textura
            glActiveTexture(GL_TEXTURE0);

            glBindTexture(
                GL_TEXTURE_2D,
                backgroundTexture
            );

            // Configurar textura
            glUniform1i(
                glGetUniformLocation(
                    backgroundShader.ID,
                    "backgroundTexture"
                ),
                0
            );

            // Dibujar rectángulo
            backgroundVAO.Bind();

            glDrawArrays(
                GL_TRIANGLES,
                0,
                6
            );

            backgroundVAO.Unbind();

            // Volver a activar profundidad
            glEnable(GL_DEPTH_TEST);

            shaderProgram.Activate();

            // ==================================
            // POSICIÓN DE LA CÁMARA
            // ==================================

            glm::vec3 cameraPosition;

            cameraPosition.x = cameraDistance * sin(cameraAngle);
            cameraPosition.y = cameraHeight;
            cameraPosition.z = cameraDistance * cos(cameraAngle);

            // ==================================
            // ILUMINACIÓN
            // ==================================

            glUniform3f(
                glGetUniformLocation(shaderProgram.ID, "lightPos"),
                3.0f, 3.0f, 3.0f
            );

            glUniform3f(
                glGetUniformLocation(shaderProgram.ID, "lightColor"),
                1.0f, 1.0f, 1.0f
            );

            glUniform3fv(
                glGetUniformLocation(shaderProgram.ID, "viewPos"),
                1,
                glm::value_ptr(cameraPosition)
            );

            // ==================================
            // MATRIZ MODEL
            // ==================================


            // Tiempo de animación
            float elapsedTime = (float)glfwGetTime();

            // Velocidad del recorrido
            float pathTime = fmod(elapsedTime * 0.15f, 3.0f);

            // Identificar el segmento actual
            int segment = (int)pathTime;

            // Tiempo dentro del segmento
            float t = pathTime - segment;

            // Calcular posición de la nave
            glm::vec3 shipPosition;

            if (segment == 0)
            {
                shipPosition = CalculateBezier(points1, t);
            }
            else if (segment == 1)
            {
                shipPosition = CalculateBezier(points2, t);
            }
            else
            {
                shipPosition = CalculateBezier(points3, t);
            }
            glm::vec3 shipDirection;

            if (segment == 0)
            {
                shipDirection = CalculateBezierTangent(points1, t);
            }
            else if (segment == 1)
            {
                shipDirection = CalculateBezierTangent(points2, t);
            }
            else
            {
                shipDirection = CalculateBezierTangent(points3, t);
            }

            // Crear matriz del modelo

            glm::mat4 model = glm::mat4(1.0f);

            // Posición de la nave
            model = glm::translate(model, shipPosition);

            // Orientación hacia la dirección del movimiento
            glm::quat rotation = glm::quatLookAt(
                -shipDirection,
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            model *= glm::mat4_cast(rotation);

            // Escala de la nave
            model = glm::scale(model, glm::vec3(0.2f));

            // ==================================
            // MATRIZ VIEW
            // ==================================

            glm::mat4 view = glm::lookAt(
                cameraPosition,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            // ==================================
            // MATRIZ PROJECTION
            // ==================================

            glm::mat4 projection = glm::perspective(
                glm::radians(45.0f),
                1.0f,
                0.1f,
                100.0f
            );

            // ==================================
            // ENVIAR MATRICES AL SHADER
            // ==================================

            glUniformMatrix4fv(
                glGetUniformLocation(shaderProgram.ID, "model"),
                1,
                GL_FALSE,
                glm::value_ptr(model)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(shaderProgram.ID, "view"),
                1,
                GL_FALSE,
                glm::value_ptr(view)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(shaderProgram.ID, "projection"),
                1,
                GL_FALSE,
                glm::value_ptr(projection)
            );

            // ==================================
            // DIBUJAR NAVE
            // ==================================

            spaceship.Draw(shaderProgram.ID);

            // Dibujar curva de Bezier
            curveShader.Activate();

            glm::mat4 curveModel = glm::mat4(1.0f);

            glUniformMatrix4fv(
                glGetUniformLocation(curveShader.ID, "model"),
                1, GL_FALSE, glm::value_ptr(curveModel)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(curveShader.ID, "view"),
                1, GL_FALSE, glm::value_ptr(view)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(curveShader.ID, "projection"),
                1, GL_FALSE, glm::value_ptr(projection)
            );

            glUniform3f(
                glGetUniformLocation(curveShader.ID, "lineColor"),
                0.0f, 0.8f, 1.0f
            );

            glLineWidth(3.0f);
            curve1.Draw();
            curve2.Draw();
            curve3.Draw();

            // ==================================
            // ACTUALIZAR
            // ==================================

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } // La nave se destruye antes de cerrar OpenGL

    // ==========================================
    // LIBERAR RECURSOS
    // ==========================================

    shaderProgram.Delete();

    backgroundVAO.Delete();
    backgroundVBO.Delete();

    backgroundShader.Delete();

    glDeleteTextures(
        1,
        &backgroundTexture
    );
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}