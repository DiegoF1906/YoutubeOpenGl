#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <vector>
#include <array>
#include <glad/glad.h>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Mesh
{
public:

    VAO* VAO1;
    VBO* VBO1;
    EBO* EBO1;

    GLsizei indexCount;

    GLuint textureID;

    std::array<float, 4> baseColor;

    Mesh(
        const std::vector<GLfloat>& vertices,
        const std::vector<GLuint>& indices,
        GLuint texture,
        const std::array<float, 4>& color
    );

    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void Draw(GLuint shaderProgram);
};

#endif