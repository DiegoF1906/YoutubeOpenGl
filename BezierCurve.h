#ifndef BEZIER_CURVE_H
#define BEZIER_CURVE_H

#include <vector>
#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "VAO.h"
#include "VBO.h"

class BezierCurve
{
public:
    VAO* VAO1;
    VBO* VBO1;
    GLsizei pointCount;

    BezierCurve(const std::array<glm::vec3, 4>& controlPoints, int segments = 100);
    ~BezierCurve();

    BezierCurve(const BezierCurve&) = delete;
    BezierCurve& operator=(const BezierCurve&) = delete;

    void Draw();
};

#endif