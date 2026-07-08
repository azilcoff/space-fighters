#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "globals.hpp"
#include "block.h"

GLuint load_shader_program()
{
    std::ifstream vertex_shader_file("./src/shaders/vertex.vert"),
                  fragment_shader_file("./src/shaders/fragment.frag");
    
    std::stringstream vertex_shader_source_stream,
                      fragment_shader_source_stream;

    vertex_shader_source_stream << vertex_shader_file.rdbuf();
    fragment_shader_source_stream << fragment_shader_file.rdbuf();

    vertex_shader_file.close();
    fragment_shader_file.close();

    const std::string vertex_shader_source_str = vertex_shader_source_stream.str(),
                      fragment_shader_source_str = fragment_shader_source_stream.str();

    const GLchar *vertex_shader_source = vertex_shader_source_str.c_str(),
                *fragment_shader_source = fragment_shader_source_str.c_str();


    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER),
           fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_source, 0);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, 0);
    
    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto p_players = reinterpret_cast<std::array<Block*, 2>*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS){
        if (key == GLFW_KEY_W)
            p_players->at(0)->velocity.y = -PLAYER_SPEED;
        else if (key == GLFW_KEY_S)
            p_players->at(0)->velocity.y = PLAYER_SPEED;
        else if (key == GLFW_KEY_UP)
            p_players->at(1)->velocity.y = -PLAYER_SPEED;
        else if (key == GLFW_KEY_DOWN)
            p_players->at(1)->velocity.y = PLAYER_SPEED; 
    }
    else{
        if (key == GLFW_KEY_W && p_players->at(0)->velocity.y == -PLAYER_SPEED)
            p_players->at(0)->velocity.y = 0.0f;
        else if (key == GLFW_KEY_S && p_players->at(0)->velocity.y == PLAYER_SPEED)
            p_players->at(0)->velocity.y = 0.0f;
        else if (key == GLFW_KEY_UP && p_players->at(1)->velocity.y == -PLAYER_SPEED)
            p_players->at(1)->velocity.y = 0.0f;
        else if (key == GLFW_KEY_DOWN && p_players->at(1)->velocity.y == PLAYER_SPEED)
            p_players->at(1)->velocity.y = 0.0f;
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH<int>, WINDOW_HEIGHT<int>, "Space Fighters", NULL, NULL);

    if (window == NULL){
        std::cerr << "Error making window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    glViewport(0, 0, WINDOW_WIDTH<GLsizei>, WINDOW_HEIGHT<GLsizei>);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLuint shader_program = load_shader_program();

    //Init blocks here
    Block player_1(glm::vec2(0.0f, PLAYER_START_Y), glm::vec3(0.0f, 255.0f, 0.0f), PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, true);
    Block player_2(glm::vec2(WINDOW_WIDTH<float> - PLAYER_SHIP_WIDTH, PLAYER_START_Y), glm::vec3(0.0f, 0.0f, 255.0f), PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, true);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::array<Block*, 2> players = {
        &player_1,
        &player_2
    };

    glfwSetWindowUserPointer(window, &players);

    glfwSetKeyCallback(window, key_callback);

    glUseProgram(shader_program);
    const glm::mat4 projection = glm::ortho(0.0f, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>, 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    while (!glfwWindowShouldClose(window)){
        //Update blocks here
        player_1.update();
        player_2.update();

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);
        //Draw blocks here
        player_1.draw();
        player_2.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader_program);
    player_1.destroy();
    player_2.destroy();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}