#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <string>
#include <vector>

#include "Mesh.h"

class Model
{
public:

    std::vector<Mesh*> meshes;

    Model(const std::string& path);

    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void Draw(GLuint shaderProgram);
};

#endif