#include "object2D.h"

#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::DrawTriangle(const std::string &name, glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-2, -3, 0), color),
        VertexFormat(glm::vec3(2, -3, 0), color),
        VertexFormat(glm::vec3(0, 2, 0), color)
    };
    std::vector<unsigned int> indices = {0, 1, 2};

    Mesh* object = new Mesh(name);
    object->InitFromData(vertices, indices);
    return object;
}
Mesh* object2D::DrawCircle(const std::string& name, glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), color)
    };
    std::vector<unsigned int> indices = {0};
    int index = 1;

    for (float teta = 0; teta < 6.3; teta += 0.1) {
        vertices.push_back(VertexFormat(glm::vec3(sin(teta), cos(teta), 0), color));
        indices.push_back(index);
        index++;
    }
    Mesh* object = new Mesh(name);
    object->SetDrawMode(GL_TRIANGLE_FAN);
    object->InitFromData(vertices, indices);
    return object;
}
Mesh* object2D::DrawRectangle(const std::string& name, glm::vec3 color, bool fill, float cover)
{   
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-1, 2, cover), color),
        VertexFormat(glm::vec3(1, 2, cover), color),
        VertexFormat(glm::vec3(1, -2, cover), color),
        VertexFormat(glm::vec3(-1, -2, cover), color)
    };
    std::vector<unsigned int> indices = { 0, 1, 2, 3};

    Mesh* object = new Mesh(name);
    if (!fill) {
        object->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }
    object->InitFromData(vertices, indices);
    return object;
}
Mesh* object2D::DrawWin(const std::string& name, glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), color),
        VertexFormat(glm::vec3(1, 0, 0), color),
        VertexFormat(glm::vec3(1.5, -2, 0), color),
        VertexFormat(glm::vec3(2, -1, 0), color),
        VertexFormat(glm::vec3(3, -1, 0), color),
        VertexFormat(glm::vec3(3.5, -2, 0), color),
        VertexFormat(glm::vec3(4, 0, 0), color),
        VertexFormat(glm::vec3(5, 0, 0), color),
        VertexFormat(glm::vec3(4, -4, 0), color),
        VertexFormat(glm::vec3(3, -4, 0), color),
        VertexFormat(glm::vec3(2.5, -3, 0), color),
        VertexFormat(glm::vec3(2, -4, 0), color),
        VertexFormat(glm::vec3(1, -4, 0), color)
    };
    std::vector<unsigned int> indices = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    Mesh* object = new Mesh(name);
    object->SetDrawMode(GL_LINE_LOOP);
    object->InitFromData(vertices, indices);
    return object;
}
Mesh* object2D::DrawLose(const std::string& name, glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, 0, 0), color),
        VertexFormat(glm::vec3(1, 0, 0), color),
        VertexFormat(glm::vec3(1, -3, 0), color),
        VertexFormat(glm::vec3(3, -3, 0), color),
        VertexFormat(glm::vec3(3, -4, 0), color),
        VertexFormat(glm::vec3(0, -4, 0), color),
    };
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5};

    Mesh* object = new Mesh(name);
    object->SetDrawMode(GL_LINE_LOOP);
    object->InitFromData(vertices, indices);
    return object;
}
float object2D::TriangleArea(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
    return abs(p1.x * (p2.y * p3.z - p2.z * p3.y) - p1.y * (p2.x * p3.z - p2.z * p3.x) + p1.z * (p2.x * p3.y - p2.y * p3.x));
}
bool object2D::CheckPoint(glm::vec3 p, glm::mat3 transformMatrix)
{
    glm::mat3 trans = glm::transpose(transformMatrix);
    glm::mat3x4 square{ -2.5, 2.5, 2.5, -2.5,
                        0, 0, -5, -5,
                        1, 1, 1, 1};
    glm::mat3x4 ans;
    for(int i=0; i<3; i++)
        for (int j = 0; j < 4; j++) {
            ans[i][j] = 0;
            for (int r = 0; r < 3; r++)
                ans[i][j] += trans[i][r] * square[r][j];
        }
    glm::vec3 p0{ ans[0][0], ans[1][0], ans[2][0] };
    glm::vec3 p1{ ans[0][1], ans[1][1], ans[2][1] };
    glm::vec3 p2{ ans[0][2], ans[1][2], ans[2][2] };
    glm::vec3 p3{ ans[0][3], ans[1][3], ans[2][3] };
    float sum1, sum2;
    sum1 = sum2 = 0.0;

    sum1 += TriangleArea(p, p0, p1);
    sum1 += TriangleArea(p, p1, p2);
    sum1 += TriangleArea(p, p2, p3);
    sum1 += TriangleArea(p, p3, p0);

    sum2 += TriangleArea(p0, p1, p2);
    sum2 += TriangleArea(p0, p2, p3);

    if (abs(sum1 - sum2) < 10)
        return true;
    else
        return false;
}
