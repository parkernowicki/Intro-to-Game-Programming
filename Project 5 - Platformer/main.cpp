/*
* Project 5 - Platformer
*  CONTROLS:
*  Left: A
*  Right: D
*  Jump: SPACE
*  Sprint: LShift
*/

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Scene.h"
#include "Level1.h"
#include "Util.h"
#include "Entity.h"
#include "Map.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;

glm::mat4 viewMatrix, projectionMatrix;

#define FIXED_TIMESTEP 1.0f / 60.0f
float lastTicks = 0.0f;
float accumulator = 0.0f;

Scene* currentScene;
Level1* level1;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Get them!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetViewMatrix(viewMatrix);
    program.SetProjectionMatrix(projectionMatrix);

    glUseProgram(program.programID);

    glClearColor(0.53f, 0.8f, 0.92f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontTextureID = Util::LoadTexture("Textures/pixel_font.png");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    level1_theme = Mix_LoadMUS("Sounds/stage1.mp3");
    victory = Mix_LoadMUS("Sounds/victory.mp3");
    die = Mix_LoadWAV("Sounds/doh.wav");
    stomp = Mix_LoadWAV("Sounds/stomp.wav");

    Mix_PlayMusic(level1_theme, -1);

    level1 = new Level1();
    SwitchToScene(level1);
}

void ProcessInput() {
    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (currentScene->state.player->collidedBottom != NULL) {
                    if (currentScene->state.player->collidedBottom->type == PLATFORM)
                        currentScene->state.player->isJumping = true;
                }
                break;
            }
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_D])
        currentScene->state.player->movement.x = 1.0f;
    if (keys[SDL_SCANCODE_A])
        currentScene->state.player->movement.x = -1.0f;
    if (keys[SDL_SCANCODE_LSHIFT])
        currentScene->state.player->isRunning = true;

    if (glm::length(currentScene->state.player->movement) > 1.0f)
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
}

void Update() {
    if (currentScene->state.player->isDead || currentScene->state.player->isWin) return;

    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->position.x > 5.5) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5.5, 3.75, 0));
    }
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);

    currentScene->Render(&program);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    Mix_FreeMusic(level1_theme);
    Mix_FreeMusic(victory);
    Mix_FreeChunk(die);
    Mix_FreeChunk(stomp);

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