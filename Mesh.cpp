#include "Mesh.h"

Mesh::Mesh(
    const std::vector<GLfloat>& vertices,
    const std::vector<GLuint>& indices
)
{
    indexCount = (GLsizei)indices.size();

    // Crear VAO
    VAO1 = new VAO();
    VAO1->Bind();

    // Crear VBO
    VBO1 = new VBO(
        const_cast<GLfloat*>(vertices.data()),
        vertices.size() * sizeof(GLfloat)
    );

    // Crear EBO
    EBO1 = new EBO(
        const_cast<GLuint*>(indices.data()),
        indices.size() * sizeof(GLuint)
    );

    // Posición: 3 componentes
    VAO1->LinkVBO(
        *VBO1,
        0,
        3,
        8 * sizeof(GLfloat),
        0
    );

    // Normales: 3 componentes
    VAO1->LinkVBO(
        *VBO1,
        1,
        3,
        8 * sizeof(GLfloat),
        3 * sizeof(GLfloat)
    );

    // Coordenadas UV: 2 componentes
    VAO1->LinkVBO(
        *VBO1,
        2,
        2,
        8 * sizeof(GLfloat),
        6 * sizeof(GLfloat)
    );

    VAO1->Unbind();
    VBO1->Unbind();
    EBO1->Unbind();
}

void Mesh::Draw()
{
    VAO1->Bind();

    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        0
    );

    VAO1->Unbind();
}

Mesh::~Mesh()
{
    VAO1->Delete();
    VBO1->Delete();
    EBO1->Delete();

    delete VAO1;
    delete VBO1;
    delete EBO1;
}