#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstddef>
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

struct GameData
{
    std::vector<Block> lazers;
    Block* player_1;
    Block* player_2;
    Block* plr_1_health_indicator;
    Block* plr_2_health_indicator;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto p_game_data = reinterpret_cast<GameData*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS){
        if (key == GLFW_KEY_W)
            p_game_data->player_1->velocity.y = -PLAYER_SPEED;
        else if (key == GLFW_KEY_S)
            p_game_data->player_1->velocity.y = PLAYER_SPEED;
        else if (key == GLFW_KEY_UP)
            p_game_data->player_2->velocity.y = -PLAYER_SPEED;
        else if (key == GLFW_KEY_DOWN)
            p_game_data->player_2->velocity.y = PLAYER_SPEED; 
        else if (key == GLFW_KEY_Q){
            const glm::vec2 player_pos = p_game_data->player_1->get_position();
            Block lazer(glm::vec2(player_pos.x + LAZER_WIDTH, player_pos.y + p_game_data->player_1->height / 2.0f - LAZER_HEIGHT / 2.0f), glm::vec3(255.0f, 0.0f, 0.0f), LAZER_WIDTH, LAZER_HEIGHT, false);
            lazer.velocity.x = LAZER_SPEED;
            p_game_data->lazers.push_back(lazer);
        }
        else if (key == GLFW_KEY_ENTER){
            const glm::vec2 player_pos = p_game_data->player_2->get_position();
            Block lazer(glm::vec2(player_pos.x - LAZER_WIDTH, player_pos.y + p_game_data->player_2->height / 2.0f - LAZER_HEIGHT / 2.0f), glm::vec3(255.0f, 0.0f, 0.0f), LAZER_WIDTH, LAZER_HEIGHT, false);
            lazer.velocity.x = -LAZER_SPEED;
            p_game_data->lazers.push_back(lazer);
        }
    }
    else{
        if (key == GLFW_KEY_W && p_game_data->player_1->velocity.y == -PLAYER_SPEED)
            p_game_data->player_1->velocity.y = 0.0f;
        else if (key == GLFW_KEY_S && p_game_data->player_1->velocity.y == PLAYER_SPEED)
            p_game_data->player_1->velocity.y = 0.0f;
        else if (key == GLFW_KEY_UP && p_game_data->player_2->velocity.y == -PLAYER_SPEED)
            p_game_data->player_2->velocity.y = 0.0f;
        else if (key == GLFW_KEY_DOWN && p_game_data->player_2->velocity.y == PLAYER_SPEED)
            p_game_data->player_2->velocity.y = 0.0f;
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
    Block plr_1_health_indicator(glm::vec2(PLAYER_SHIP_WIDTH + 10.0f, WINDOW_HEIGHT<float> - 10.0f - PLAYER_HEALTH_INDICATOR_SIZE), glm::vec3(0.0f, 255.0f, 0.0f), PLAYER_HEALTH_INDICATOR_SIZE, PLAYER_HEALTH_INDICATOR_SIZE, false);
    Block plr_2_health_indicator(glm::vec2(WINDOW_WIDTH<float> - 10.0f - PLAYER_SHIP_WIDTH, WINDOW_HEIGHT<float> - 10.0f - PLAYER_HEALTH_INDICATOR_SIZE), glm::vec3(0.0f, 255.0f, 0.0f), PLAYER_HEALTH_INDICATOR_SIZE, PLAYER_HEALTH_INDICATOR_SIZE, false);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GameData game_data = {
        .lazers = {},
        .player_1 = &player_1,
        .player_2 = &player_2,
    };

    glfwSetWindowUserPointer(window, &game_data);

    glfwSetKeyCallback(window, key_callback);

    glUseProgram(shader_program);
    const glm::mat4 projection = glm::ortho(0.0f, WINDOW_WIDTH<float>, WINDOW_HEIGHT<float>, 0.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    while (!glfwWindowShouldClose(window)){
        //Update blocks here
        player_1.update();
        player_2.update();
        for (std::size_t i = 0; i < game_data.lazers.size(); ++i){
            game_data.lazers[i].update();
            const bool is_moving_right = game_data.lazers[i].velocity.x > 0.0f;
            const glm::vec2 lazer_pos = game_data.lazers[i].get_position(),
                            player_pos = is_moving_right ? player_2.get_position() : player_1.get_position();
            const bool hit_player = (is_moving_right ? (lazer_pos.x + LAZER_WIDTH >= WINDOW_WIDTH<float> - PLAYER_SHIP_WIDTH) : (lazer_pos.x <= PLAYER_SHIP_WIDTH)) && lazer_pos.y >= player_pos.y && lazer_pos.y + LAZER_HEIGHT <= player_pos.y + PLAYER_SHIP_HEIGHT;
            if ((lazer_pos.x + game_data.lazers[i].width < 0.0f || lazer_pos.x > WINDOW_WIDTH<float>) || hit_player){
                game_data.lazers[i].destroy();
                game_data.lazers.erase(game_data.lazers.begin() + i);
                --i;
                if (hit_player){
                    Block* const plr_health_indicator = is_moving_right ? &plr_2_health_indicator : &plr_1_health_indicator;
                    const glm::vec3 color = plr_health_indicator->get_color();
                    if (color.g == 255.0f){
                        plr_health_indicator->set_color(glm::vec3(255.0f, 202.0f, 12.0f));
                    }
                    else if (color == glm::vec3(255.0f, 202.0f, 12.0f)){
                        plr_health_indicator->set_color(glm::vec3(255.0f, 0.0f, 0.0f));
                    }
                    else{
                        std::cout << "Player " << (is_moving_right ? '1' : '2') << std::endl;
                        glDeleteProgram(shader_program);
                        player_1.destroy();
                        player_2.destroy();
                        for (auto& lazer : game_data.lazers){
                            lazer.destroy();
                        }
                        plr_1_health_indicator.destroy();
                        plr_2_health_indicator.destroy();
                        glfwDestroyWindow(window);
                        glfwTerminate();
                        return 0;
                    }
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);
        //Draw blocks here
        player_1.draw();
        player_2.draw();
        for (const auto& lazer : game_data.lazers){
            lazer.draw();
        }
        plr_1_health_indicator.draw();
        plr_2_health_indicator.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader_program);
    player_1.destroy();
    player_2.destroy();
    for (auto& lazer : game_data.lazers){
        lazer.destroy();
    }
    plr_1_health_indicator.destroy();
    plr_2_health_indicator.destroy();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}