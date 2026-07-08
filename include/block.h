#pragma once

#include <array>
#include "glad/glad.h"
#include <glm/glm.hpp>

class Block
{
private:
    bool solid;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    glm::vec2 position;
    glm::vec3 color;
    std::array<GLfloat, 30> vertices;

    void update_vertices();
public:
    GLfloat width;
    GLfloat height;
    glm::vec2 velocity;
    Block(const glm::vec2& position, const glm::vec3& color, const GLfloat width, const GLfloat height, const bool solid);

    static constexpr std::array<GLushort, 6> INDICES = {
        2, 1, 0,
        3, 2, 0
    };

    const glm::vec2 get_position() const;

    void draw() const;
    void update();

    void destroy();
};