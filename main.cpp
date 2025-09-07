#define STB_IMAGE_IMPLEMENTATION
#define GLEW_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdint>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <Shader.h>
#include <stb_image.h>
#include "camera.h"
#include "world.h"

#define PI 3.1415926535897932384626
extern block BlockID;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void loadtexture(const std::string filename, unsigned int *texture, bool nearest);
void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
void mouse_hit_even(GLFWwindow *window, int button, int action, int mods);

int width = 1200, height = 800;

float lastX = width / 2.0;
float lastY = height / 2.0;
bool firstMouse = true;
bool firstC = true;
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;
int width_line = 0;
bool chit = false;
pos3 hitblock;
GLuint tVAO, tVBO;
World *tempworld;

struct Ray
{
    void step(float scale)
    {
        ray_end.x += direction.x * scale;
        ray_end.z += direction.z * scale;
        ray_end.y += direction.y * scale;
    }

    void init(glm::vec3 position, glm::vec3 direc)
    {
        ray_start = position + glm::vec3(0, 0, 0);
        ray_end = position + glm::vec3(0, 0, 0);
        direction = direc;
    }

    float getLength()
    {
        return glm::distance(ray_start, ray_end);
    }

    glm::vec3 ray_start;
    glm::vec3 ray_end;
    glm::vec3 direction;
};

struct Character
{
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};

Camera PlayerCamera;

std::unordered_map<GLchar, Character> Characters;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    int width = 1200, height = 800;
    GLFWwindow *window = glfwCreateWindow(width, height, "Minecraft", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    const GLubyte *version = glGetString(GL_VERSION);
    if (version == nullptr)
    {
        std::cerr << "Error getting OpenGL version" << std::endl;
        return -1;
    }

    std::cout << "OpenGL Version: " << version << std::endl;

    glfwSetWindowSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_hit_even);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    /* float vir[] = {
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,


        0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,



        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,



        1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,


    }; */

    Shader shader("shaders/shader.vs", "shaders/shader.fs");
    Shader textshader("shaders/text.vs", "shaders/text.fs");
    Shader lineshader("shaders/line.vs", "shaders/line.fs");
    Shader sightshader("shaders/sight.vs", "shaders/sight.fs");
    glm::mat4 textprojection = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
    textshader.use();
    textshader.setMat4("projection", textprojection);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "fonts/Minecraft.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        GLuint ttexture;
        glGenTextures(1, &ttexture);
        glBindTexture(GL_TEXTURE_2D, ttexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            ttexture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)

        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glGenVertexArrays(1, &tVAO);
    glGenBuffers(1, &tVBO);
    glBindVertexArray(tVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float vir[] = {
        //	  顶点坐标
        // front
        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,

        // back
        1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,

        // left
        0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,

        // right
        1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f};

    float sight16 = 16 / 36.0, sight20 = 20 / 36.0;
    float vir_front_sight[] = {
        sight16, 0.0f, 0.0f,
        sight20, 0.0f, 0.0f,
        sight20, 1.0f, 0.0f,
        sight20, 1.0f, 0.0f,
        sight16, 1.0f, 0.0f,
        sight16, 0.0f, 0.0f,

        0.0f, sight16, 0.0f,
        1.0f, sight16, 0.0f,
        1.0f, sight20, 0.0f,
        1.0f, sight20, 0.0f,
        0.0f, sight20, 0.0f,
        0.0f, sight16, 0.0f
    };

    GLuint line_VAO, line_VBO, front_sight_VAO, front_sight_VBO;

    glGenVertexArrays(1, &line_VAO);
    glBindVertexArray(line_VAO);

    glGenBuffers(1, &line_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, line_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vir), vir, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glGenVertexArrays(1, &front_sight_VAO);
    glBindVertexArray(front_sight_VAO);

    glGenBuffers(1, &front_sight_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, front_sight_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vir_front_sight), vir_front_sight, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    World overworld;
    tempworld = &overworld;
    overworld.loadthread();


    unsigned int texture;
    loadtexture("2.png", &texture, true);

    shader.use();
    shader.setInt("texture1", 0);
    glm::mat4 proj = glm::perspective(glm::radians(PlayerCamera.fov), (float)width / (float)height, 0.1f, 1500.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    Ray ray;
    pos3 last_end;

    glClearColor(0.533f, 0.706f, 0.957f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float temptime = static_cast<float>(glfwGetTime());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        overworld.toffset = {static_cast<int>(floor(PlayerCamera.cameraPos.x / 16.0)) - overworld.chunkdr, static_cast<int>(floor(PlayerCamera.cameraPos.z / 16.0)) - overworld.chunkdr};

        model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        view = glm::lookAt(PlayerCamera.cameraPos, PlayerCamera.cameraPos + PlayerCamera.cameraFront, PlayerCamera.cameraUp);
        // glDisable(GL_CULL_FACE);
        glEnable(GL_CULL_FACE);

        shader.use();
        shader.setMat4("projViewMatrix", proj * view * model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        for (auto it = overworld.check_list_for_main_thread.begin(); it != overworld.check_list_for_main_thread.end();)
        {
            Chunk &temchunk = overworld.get_chunk(it->x, it->y, it->z);
            temchunk.update_chunk();

            it = overworld.check_list_for_main_thread.erase(it);
        }
        for (auto itr = overworld.world.begin(); itr != overworld.world.end();)
        {
            if ((itr->first.x < overworld.light_flood_offset.x - 1 || itr->first.x > overworld.light_flood_offset.x + overworld.chunkl || itr->first.y < overworld.light_flood_offset.y - 1 || itr->first.y > overworld.light_flood_offset.y + overworld.chunkl) && (itr->first.x < overworld.load_offset.x - 1 || itr->first.x > overworld.load_offset.x + overworld.chunkl || itr->first.y < overworld.load_offset.y - 1 || itr->first.y > overworld.load_offset.y + overworld.chunkl))
            {
                if ((itr->first.x < overworld.light_flood_offset.x - 2 || itr->first.x > overworld.light_flood_offset.x + overworld.chunkl + 1 || itr->first.y < overworld.light_flood_offset.y - 2 || itr->first.y > overworld.light_flood_offset.y + overworld.chunkl + 1) && (itr->first.x < overworld.load_offset.x - 2 || itr->first.x > overworld.load_offset.x + overworld.chunkl + 1 || itr->first.y < overworld.load_offset.y - 2 || itr->first.y > overworld.load_offset.y + overworld.chunkl + 1))
                {
                    for (int i = 0; i < 24; i++)
                    {
                        itr->second.chunkxz[i].delete_chunk();
                    }
                    itr = overworld.world.erase(itr);
                }
                else
                {
                    if (itr->second.isflood)
                    {
                        itr->second.isflood = false;
                    }
                    itr++;
                }
            }
            else
            {
                if ((itr->first.x > overworld.light_flood_offset.x + 1 || itr->first.x < overworld.light_flood_offset.x + overworld.chunkdl - 1 || itr->first.y > overworld.light_flood_offset.y + 1 || itr->first.y < overworld.light_flood_offset.y + overworld.chunkdl - 1) && (itr->first.x > overworld.load_offset.x + 1 || itr->first.x < overworld.load_offset.x + overworld.chunkdl - 1 || itr->first.y > overworld.load_offset.y + 1 || itr->first.y < overworld.load_offset.y + overworld.chunkdl - 1))
                {
                    if (itr->second.isload)
                    {
                        bool t = false;

                        for (int i = 0; i < 24; i++)
                        {

                            itr->second.chunkxz[i].veboinit();
                        }
                    }
                }
                itr++;
            }
        }
        for (ray.init(PlayerCamera.cameraPos, PlayerCamera.cameraFront); ray.getLength() < 6; ray.step(0.05f))
        {
            int x = static_cast<int>(floor(ray.ray_end.x));
            int y = static_cast<int>(floor(ray.ray_end.y));
            int z = static_cast<int>(floor(ray.ray_end.z));

            Block block = overworld.get_block(x, y, z);
            blockdata *id = block.BlockNID;
            if (*id != BlockID.Air)
            {
                // glEnable(GL_BLEND);
                // glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
                glBindVertexArray(line_VAO);
                lineshader.use();
                
                lineshader.setMat4("projViewMatrix", proj * view * model);
                lineshader.setVec2("screenSize", glm::vec2(width, height));
                lineshader.setVec3("pos", glm::vec3(x,y,z));//x, y, z));

                // glDrawArrays(GL_LINES, 0, 24);
                // lineshader.setFloat("linewidth", -10.0);
                lineshader.setFloat("linewidth", 0.0);
                glDrawArrays(GL_LINES, 0, 24);
                lineshader.setFloat("linewidth", 1.0);
                glDrawArrays(GL_LINES, 0, 24);
                lineshader.setFloat("linewidth", -1.0);
                glDrawArrays(GL_LINES, 0, 24);
                lineshader.setFloat("linewidth", 2.0);
                glDrawArrays(GL_LINES, 0, 24);
                lineshader.setFloat("linewidth", -2.0);
                glDrawArrays(GL_LINES, 0, 24);
                chit = true;
                // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                RenderText(textshader, "block light: " + std::to_string(overworld.get_skylight(last_end.x,last_end.y,last_end.z)), 0.0f, 420.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
                RenderText(textshader, "block: " + overworld.get_block(last_end.x,last_end.y,last_end.z).BlockNID->name + "[" + std::to_string(last_end.x) + " " + std::to_string(last_end.y) + " " + std::to_string(last_end.z) + "]", 0.0f, 480.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
                hitblock = {x, y, z};

                break;
            }
            else
            {
                chit = false;
            }
            last_end = {x, y, z};
        }

        glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_COLOR);
        sightshader.use();
        glBindVertexArray(front_sight_VAO);
        sightshader.setVec2("screenSize", glm::vec2(width, height));

        glDrawArrays(GL_TRIANGLES, 0, 16);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




        std::string chunk_pos = std::to_string(static_cast<int>(floor(PlayerCamera.cameraPos.x / 16.0))) + " " + std::to_string(static_cast<int>(floor(PlayerCamera.cameraPos.y / 16.0))) + " " + std::to_string(static_cast<int>(floor(PlayerCamera.cameraPos.z / 16.0)));
        RenderText(textshader, "Chunk: " + chunk_pos, 0.0f, 0.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        std::string chunk_floodpos = std::to_string(overworld.light_flood_offset.x) + " " + std::to_string(static_cast<int>(overworld.light_flood_offset.y));
        RenderText(textshader, "Chunk Flood: " + chunk_floodpos, 0.0f, 240.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        std::string chunk_loadpos = std::to_string(static_cast<int>(overworld.load_offset.x)) + " " + std::to_string(static_cast<int>(overworld.load_offset.y));
        RenderText(textshader, "Chunk Load: " + chunk_loadpos, 0.0f, 300.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        if (glfwGetTime() - temptime < 1 / 60.0)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(1 / 60.0 - glfwGetTime() + temptime));
        }
        RenderText(textshader, "Fps: " + std::to_string(static_cast<int>(1 / (static_cast<float>(glfwGetTime()) - temptime))) + " " + std::to_string(overworld.fps) + " " + std::to_string(overworld.fps2), 0.0f, 60.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        RenderText(textshader, "Pos: " + std::to_string(static_cast<int>(PlayerCamera.cameraPos.x)) + " " + std::to_string(static_cast<int>(PlayerCamera.cameraPos.y)) + " " + std::to_string(static_cast<int>(PlayerCamera.cameraPos.z)), 0.0f, 120.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        RenderText(textshader, "Pos: " + std::to_string(overworld.world.size()), 0.0f, 180.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        RenderText(textshader, "block light: " + std::to_string(overworld.get_skylight(PlayerCamera.cameraPos.x,PlayerCamera.cameraPos.y,PlayerCamera.cameraPos.z)), 0.0f, 360.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    overworld.endingThread();
    overworld.endSignal();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    float cameraSpeed = static_cast<float>(50 * deltaTime);
    float cameraDisland = sqrt(PlayerCamera.cameraFront[0] * PlayerCamera.cameraFront[0] + PlayerCamera.cameraFront[2] * PlayerCamera.cameraFront[2]);
    glm::vec3 cameradis = glm::vec3(PlayerCamera.cameraFront[0] / cameraDisland, 0, PlayerCamera.cameraFront[2] / cameraDisland);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        PlayerCamera.cameraPos += cameraSpeed * cameradis;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        PlayerCamera.cameraPos -= cameraSpeed * cameradis;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        PlayerCamera.cameraPos -= cameraSpeed * PlayerCamera.cameraUp;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        PlayerCamera.cameraPos += cameraSpeed * PlayerCamera.cameraUp;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        PlayerCamera.cameraPos -= glm::normalize(glm::cross(cameradis, PlayerCamera.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        PlayerCamera.cameraPos += glm::normalize(glm::cross(cameradis, PlayerCamera.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && firstC)
    {
        // tempworld->checklist.push_back(pos3(13,66,13));
        // std::thread Flood_light_thread(&World::light_flooding, tempworld);
        // Flood_light_thread.detach();
        firstC = false;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        // tempworld->checklist.push_back(pos3(13,66,13));
        // std::thread Flood_light_thread(&World::light_flooding, tempworld);
        // Flood_light_thread.detach();
        width_line += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        // tempworld->checklist.push_back(pos3(13,66,13));
        // std::thread Flood_light_thread(&World::light_flooding, tempworld);
        // Flood_light_thread.detach();
        width_line -= 1;
    }
}

void loadtexture(const std::string filename, unsigned int *texture, bool nearest)
{
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    if (nearest)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.5;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    PlayerCamera.moveview(xoffset, yoffset);
}

void mouse_hit_even(GLFWwindow *window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (chit)
            {
                tempworld->checklist.push_back(hitblock);
            }
            break;
        }
}

void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // 激活对应的渲染状态
    s.use();
    glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(tVAO);

    // 遍历文本中所有的字符
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0, 0.0},
            {xpos, ypos, 0.0, 1.0},
            {xpos + w, ypos, 1.0, 1.0},

            {xpos, ypos + h, 0.0, 0.0},
            {xpos + w, ypos, 1.0, 1.0},
            {xpos + w, ypos + h, 1.0, 0.0}};
        // 在四边形上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // 更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, tVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // 绘制四边形
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // 更新位置到下一个字形的原点，注意单位是1/64像素
        x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}