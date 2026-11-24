#include "BezierCurve.h"

BezierCurve::BezierCurve(
    const std::array<glm::vec3, 4>& controlPoints,
    int segments
)
{
    std::vector<GLfloat> vertices;

    // Calcular los puntos de la curva
    for (int i = 0; i <= segments; i++)
    {
        float t = (float)i / segments;
        float u = 1.0f - t;

        glm::vec3 point =
            u * u * u * controlPoints[0] +
            3.0f * u * u * t * controlPoints[1] +
            3.0f * u * t * t * controlPoints[2] +
            t * t * t * controlPoints[3];

        vertices.push_back(point.x);
        vertices.push_back(point.y);
        vertices.push_back(point.z);
    }

    pointCount = (GLsizei)(vertices.size() / 3);

    // Crear VAO y VBO
    VAO1 = new VAO();
    VAO1->Bind();

    VBO1 = new VBO(
        vertices.data(),
        vertices.size() * sizeof(GLfloat)
    );

    // Cada punto tiene 3 componentes: X, Y, Z
    VAO1->LinkVBO(
        *VBO1,
        0,
        3,
        3 * sizeof(GLfloat),
        0
    );

    VBO1->Unbind();
    VAO1->Unbind();
}

void BezierCurve::Draw()
{
    VAO1->Bind();

    glDrawArrays(GL_LINE_STRIP, 0, pointCount);

    VAO1->Unbind();
}

BezierCurve::~BezierCurve()
{
    VAO1->Delete();
    VBO1->Delete();

    delete VAO1;
    delete VBO1;
}