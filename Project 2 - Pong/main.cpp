/*
* Project 2 - Pong
* CONTROLS:
*  Player 1 up: W
*  Player 1 down: S
*  Player 2 up: UP ARROW
*  Player 2 down: DOWN ARROW
*  Start: SPACEBAR
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
#include "glm/gtx/rotate_vector.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <random>

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;

float padl_w = 0.4f, padl_h = 2.0f;
float paddle_speed = 4.0f;

float ball_w = 0.4f, ball_h = 0.4f;
float ball_speed = 6.0f, ball_rand;

glm::vec3 paddle1_position = glm::vec3(-4, 0, 0);
glm::vec3 paddle1_movement;

glm::vec3 paddle2_position = glm::vec3(4, 0, 0);
glm::vec3 paddle2_movement;

glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(0);

glm::mat4 viewMatrix, projectionMatrix, paddle1Matrix, paddle2Matrix, ballMatrix;

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> dist(-1.0, 1.0);

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
    displayWindow = SDL_CreateWindow("Pong X-Treme", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    //program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
}

void ProcessInput() {
    paddle1_movement = glm::vec3(0);
    paddle2_movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_SPACE] && ball_movement == glm::vec3(0))
        ball_movement = glm::vec3(1, 0, 0);
    if (keys[SDL_SCANCODE_W])
        paddle1_movement.y = 1.0f;
    if (keys[SDL_SCANCODE_S])
        paddle1_movement.y = -1.0f;
    if (keys[SDL_SCANCODE_UP])
        paddle2_movement.y = 1.0f;
    if (keys[SDL_SCANCODE_DOWN])
        paddle2_movement.y = -1.0f;
}

bool isCollideBoxtoBox(const float x1, const float x2, const float y1, const float y2,
    const float w1, const float w2, const float h1, const float h2) {
    float xdist = fabs(x2 - x1) - ((w1 + w2) / 2.0f);
    float ydist = fabs(y2 - y1) - ((h1 + h2) / 2.0f);

    return xdist < 0 && ydist < 0;
}

float lastTicks = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    //Paddle 1
    if (glm::length(paddle1_movement) > 1.0f)
        paddle1_movement = glm::normalize(paddle1_movement);

    if (paddle1_position.y + (paddle1_movement.y * paddle_speed * deltaTime) > 2.75f)
        paddle1_position.y = 2.75f;
    else if (paddle1_position.y + (paddle1_movement.y * paddle_speed * deltaTime) < -2.75f)
        paddle1_position.y = -2.75f;
    else
        paddle1_position += paddle1_movement * paddle_speed * deltaTime;

    paddle1Matrix = glm::mat4(1.0f);
    paddle1Matrix = glm::translate(paddle1Matrix, paddle1_position);

    //Paddle 2
    if (glm::length(paddle2_movement) > 1.0f)
        paddle2_movement = glm::normalize(paddle2_movement);

    if (paddle2_position.y + (paddle2_movement.y * paddle_speed * deltaTime) > 2.75f)
        paddle2_position.y = 2.75f;
    else if (paddle2_position.y + (paddle2_movement.y * paddle_speed * deltaTime) < -2.75f)
        paddle2_position.y = -2.75f;
    else
        paddle2_position += paddle2_movement * paddle_speed * deltaTime;

    paddle2Matrix = glm::mat4(1.0f);
    paddle2Matrix = glm::translate(paddle2Matrix, paddle2_position);

    //Ball
    if (glm::length(ball_movement) > 0.0f)
        ball_movement = glm::normalize(ball_movement);

    if (ball_position.x < -4.8f || ball_position.x > 4.8f) {
        gameIsRunning = false;
        return;
    }
    if (ball_position.y + (ball_movement.y * ball_speed * deltaTime) > 3.55f) {
        ball_position.y = 3.55f;
        ball_movement.y *= -1.0f;
    }
    if (ball_position.y + (ball_movement.y * ball_speed * deltaTime) < -3.55f) {
        ball_position.y = -3.55f;
        ball_movement.y *= -1.0f;
    }
    ball_rand = dist(mt);
    if (isCollideBoxtoBox(ball_position.x + (ball_movement.x * ball_speed * deltaTime), paddle1_position.x,
        ball_position.y + (ball_movement.y * ball_speed * deltaTime), paddle1_position.y,
        ball_w, padl_w, ball_h, padl_h)) {
        //Check if bumped from the top/bottom
        if (ball_position.y > paddle1_position.y + ((padl_h + ball_h) / 2) ||
            ball_position.y < paddle1_position.y - ((padl_h + ball_h) / 2))
            ball_movement.y *= -1.0f;
        else {
            ball_movement.x *= -1.0f;
            ball_movement.y = 0;
            ball_movement = glm::rotate(ball_movement, ball_rand, glm::vec3(0.0f, 0.0f, 1.0f));
        }
    }
    if (isCollideBoxtoBox(ball_position.x + (ball_movement.x * ball_speed * deltaTime), paddle2_position.x,
        ball_position.y + (ball_movement.y * ball_speed * deltaTime), paddle2_position.y,
        ball_w, padl_w, ball_h, padl_h)) {
        if (ball_position.y > paddle2_position.y + ((padl_h + ball_h) / 2) ||
            ball_position.y < paddle2_position.y - ((padl_h + ball_h) / 2))
            ball_movement.y *= -1.0f;
        else {
            ball_movement.x *= -1.0f;
            ball_movement.y = 0;
            ball_movement = glm::rotate(ball_movement, ball_rand, glm::vec3(0.0f, 0.0f, 1.0f));
        }
    }

    ball_position += ball_movement * ball_speed * deltaTime;

    ballMatrix = glm::mat4(1.0f);
    ballMatrix = glm::translate(ballMatrix, ball_position);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0,
                          0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    float paddleVertices[] = { -padl_w / 2, -padl_h / 2, padl_w / 2, -padl_h / 2, padl_w / 2, padl_h / 2,
                               -padl_w / 2, -padl_h / 2, padl_w / 2, padl_h / 2, -padl_w / 2, padl_h / 2 };
    float ballVertices[] = { -ball_w / 2, -ball_h / 2, ball_w / 2, -ball_h / 2, ball_w / 2, ball_h / 2,
                             -ball_w / 2, -ball_h / 2, ball_w / 2, ball_h / 2, -ball_w / 2, ball_h / 2 };

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddleVertices);
    glEnableVertexAttribArray(program.positionAttribute);

    program.SetModelMatrix(paddle1Matrix);
    //glBindTexture(GL_TEXTURE_2D, paddle1TextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    program.SetModelMatrix(paddle2Matrix);
    //glBindTexture(GL_TEXTURE_2D, paddle2TextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballVertices);

    program.SetModelMatrix(ballMatrix);
    //glBindTexture(GL_TEXTURE_2D, ballTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.texCoordAttribute);
    glDisableVertexAttribArray(program.positionAttribute);

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