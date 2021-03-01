/*
* Project 1 - Simple 2D Scene
*/

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, ryuMatrix, blankaMatrix, stageMatrix, projectionMatrix;

float ryu_x = 0.0f, ryu_speed = 2.0f;
float blanka_spin = 0.0f;

GLuint ryuTextureID, blankaTextureID, stageTextureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Street Fighter 6", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ryuTextureID = LoadTexture("Textures/ryu.png");
    blankaTextureID = LoadTexture("Textures/blanka.png");
    stageTextureID = LoadTexture("Textures/ryustage.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    if (ryu_x > 0.8f || ryu_x < -0.8f)
        ryu_speed *= -1;
    ryu_x += ryu_speed * deltaTime;
    blanka_spin += 1080.0f * deltaTime;

    stageMatrix = glm::mat4(1.0f);
    stageMatrix = glm::scale(stageMatrix, glm::vec3(3.0f, 3.0f, 0.0f));
    ryuMatrix = glm::mat4(1.0f);
    ryuMatrix = glm::translate(ryuMatrix, glm::vec3(ryu_x - 2.5, -1.0f, 0.0f));
    blankaMatrix = glm::mat4(1.0f);
    blankaMatrix = glm::translate(blankaMatrix, glm::vec3(2.5, -1.0f, 0.0f));
    blankaMatrix = glm::rotate(blankaMatrix, glm::radians(blanka_spin), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = { -2.0, -2.0, 2.0, -2.0, 2.0, 2.0, -2.0, -2.0, 2.0, 2.0, -2.0, 2.0 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    program.SetModelMatrix(stageMatrix);
    glBindTexture(GL_TEXTURE_2D, stageTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(ryuMatrix);
    glBindTexture(GL_TEXTURE_2D, ryuTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(blankaMatrix);
    glBindTexture(GL_TEXTURE_2D, blankaTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}