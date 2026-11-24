#include "Mesh.h"

Mesh::Mesh(
    const std::vector<GLfloat>& vertices,
    const std::vector<GLuint>& indices,
    GLuint texture,
    const std::array<float, 4>& color
)
{
    indexCount = (GLsizei)indices.size();

    textureID = texture;
    baseColor = color;

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

    // Posiciones
    VAO1->LinkVBO(
        *VBO1,
        0,
        3,
        8 * sizeof(GLfloat),
        0
    );

    // Normales
    VAO1->LinkVBO(
        *VBO1,
        1,
        3,
        8 * sizeof(GLfloat),
        3 * sizeof(GLfloat)
    );

    // Coordenadas UV
    VAO1->LinkVBO(
        *VBO1,
        2,
        2,
        8 * sizeof(GLfloat),
        6 * sizeof(GLfloat)
    );

    VBO1->Unbind();

    // El EBO debe permanecer asociado al VAO.
    VAO1->Unbind();
}

void Mesh::Draw(GLuint shaderProgram)
{
    // Activar textura
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Enviar color del material
    GLint colorLocation = glGetUniformLocation(
        shaderProgram,
        "baseColorFactor"
    );

    glUniform4fv(
        colorLocation,
        1,
        baseColor.data()
    );

    // Dibujar malla
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

    glDeleteTextures(1, &textureID);

    delete VAO1;
    delete VBO1;
    delete EBO1;
}