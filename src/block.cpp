#include "block.h"

Block::Block(const glm::vec2& position, const glm::vec3& color, const GLfloat width, const GLfloat height):
    position(position),
    color(color / 255.0f),
    width(width),
    height(height)
{
    vertices = {
        position.x, position.y, color.r, color.g, color.b,
        position.x + width, position.y, color.r, color.g, color.b,
        position.x + width, position.y + height, color.r, color.g, color.b,
        position.x, position.y + height, color.r, color.g, color.b
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Block::INDICES.size() * sizeof(GLushort), Block::INDICES.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
}

void Block::update_vertices()
{
    vertices = {
        position.x, position.y, color.r, color.g, color.b,
        position.x + width, position.y, color.r, color.g, color.b,
        position.x + width, position.y + height, color.r, color.g, color.b,
        position.x, position.y + height, color.r, color.g, color.b
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());
}

void Block::update()
{
    position.x += 2.0f;
    update_vertices();
}
 
void Block::draw() const
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void Block::destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}