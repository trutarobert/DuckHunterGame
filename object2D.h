#pragma once

#include <string>
#include <iostream>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    Mesh* DrawTriangle(const std::string &name, glm::vec3 color);
    Mesh* DrawCircle(const std::string& name, glm::vec3 color);
    Mesh* DrawRectangle(const std::string& name, glm::vec3 color, bool fill, float cover);
    Mesh* DrawWin(const std::string& name, glm::vec3 color);
    Mesh* DrawLose(const std::string& name, glm::vec3 color);
    float TriangleArea(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
    bool CheckPoint(glm::vec3 p, glm::mat3 transformMatrix);
}
