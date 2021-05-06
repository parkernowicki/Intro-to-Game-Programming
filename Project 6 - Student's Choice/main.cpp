/*
* 
*/

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Title.h"
#include "Level1.h"
#include "Level2.h"
#include "End.h"
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

Scene *currentScene, *sceneList[4];

int lives;

void SwitchToScene(Scene* scene) {
    Mix_FreeMusic(currentScene->leveltheme);
    Mix_FreeMusic(currentScene->victory);
    Mix_FreeMusic(currentScene->dead);
    Mix_FreeChunk(currentScene->hurt);
    Mix_FreeChunk(currentScene->schwing);
    Mix_FreeChunk(currentScene->bossdead);
    Mix_FreeChunk(currentScene->warp);

    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("SKELLYQUEST 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    glUseProgram(program.programID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    lives = 3;

    sceneList[0] = new Title();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new End();

    currentScene = sceneList[0];
    currentScene->Initialize();
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
            if (currentScene->state.player->isDead) return;
			switch (event.key.keysym.sym) {
			case SDLK_RETURN:
				if (currentScene == sceneList[0])
					currentScene->state.nextScene = 1;
				break;
            case SDLK_UP:
                currentScene->state.player->isMeleeAttack = true;
                currentScene->state.player->animIndices = currentScene->state.player->animAttackUp;
                Mix_PlayChannel(-1, currentScene->schwing, 0);
                break;
            case SDLK_LEFT:
                currentScene->state.player->isMeleeAttack = true;
                currentScene->state.player->animIndices = currentScene->state.player->animAttackLeft;
                Mix_PlayChannel(-1, currentScene->schwing, 0);
                break;
            case SDLK_DOWN:
                currentScene->state.player->isMeleeAttack = true;
                currentScene->state.player->animIndices = currentScene->state.player->animAttackDown;
                Mix_PlayChannel(-1, currentScene->schwing, 0);
                break;
            case SDLK_RIGHT:
                currentScene->state.player->isMeleeAttack = true;
                currentScene->state.player->animIndices = currentScene->state.player->animAttackRight;
                Mix_PlayChannel(-1, currentScene->schwing, 0);
                break;
			}
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (!currentScene->state.player->isMeleeAttack) {
        if (keys[SDL_SCANCODE_W]) {
            currentScene->state.player->movement.y = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animUp;
        }
        if (keys[SDL_SCANCODE_A]) {
            currentScene->state.player->movement.x = -1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        }
        if (keys[SDL_SCANCODE_S]) {
            currentScene->state.player->movement.y = -1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animDown;
        }
        if (keys[SDL_SCANCODE_D]) {
            currentScene->state.player->movement.x = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
        }
    }

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
        currentScene->Update(FIXED_TIMESTEP, &lives);
        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    if (currentScene == sceneList[1])
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, -currentScene->state.player->position.y, 0));
    else
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-4.5f, 3.5f, 0));
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    currentScene->RenderBackground(&program);

    program.SetViewMatrix(viewMatrix);
    program.SetProjectionMatrix(projectionMatrix);

    currentScene->Render(&program, lives);

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
        if (currentScene->state.nextScene >= 0)
            SwitchToScene(sceneList[currentScene->state.nextScene]);
        Render();
    }

    Shutdown();
    return 0;
}