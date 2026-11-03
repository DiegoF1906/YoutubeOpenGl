#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <vector>
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

    Mesh(
        const std::vector<GLfloat>& vertices,
        const std::vector<GLuint>& indices
    );

    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void Draw();
};

#endif