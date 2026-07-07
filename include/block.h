#pragma once

#include <array>
#include "glad/glad.h"
#include <glm/glm.hpp>

class Block
{
private:
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    glm::vec2 position;
    glm::vec3 color;
    GLfloat width;
    GLfloat height;
    std::array<GLfloat, 30> vertices;

    void update_vertices();
public:
    Block(const glm::vec2& position, const glm::vec3& color, const GLfloat width, const GLfloat height);

    static constexpr std::array<GLushort, 6> INDICES = {
        2, 1, 0,
        3, 2, 0
    };

    void draw() const;
    void update();

    void destroy();
};