/*
* Project 4 - Rise of the AI
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define PLATFORM_COUNT 20
#define BADDY_COUNT 3

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* baddies;
};

GameState state;

int baddiesleft;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;

glm::mat4 viewMatrix, projectionMatrix;

GLuint fontTextureID;

#define FIXED_TIMESTEP 1.0f / 60.0f
float lastTicks = 0.0f;
float accumulator = 0.0f;

Mix_Music* level1, *victory;
Mix_Chunk* die, *stomp;

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

    glClearColor(0.2f, 0.1f, 0.1f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    state.player = new Entity;
    state.player->type = PLAYER;
    state.player->position = glm::vec3(-4.0f, -2.25f, 0.0f);
    state.player->acceleration = 16.0f;
    state.player->walkSpeed = 150.0f;
    state.player->runSpeed = 300.0f;
    state.player->jumpSpeed = 8.2f;
    state.player->textureID = LoadTexture("Textures/ufo.png");

    state.player->animDefault = new int[4]{ 0, 2, 4, 6 };
    state.player->animIndices = state.player->animDefault;
    state.player->animFrames = 4;
    state.player->animRows = 4;
    state.player->animCols = 2;

    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("Textures/block.png");

    for (int i = 0; i < 11; i++) {
        state.platforms[i].type = PLATFORM;
        state.platforms[i].position = glm::vec3(-5.0f + i, -3.25f, 0.0f);
        state.platforms[i].textureID = platformTextureID;
    }

    state.platforms[11].type = PLATFORM;
    state.platforms[11].position = glm::vec3(-5.0f, -2.25f, 0.0f);
    state.platforms[11].textureID = platformTextureID;

    state.platforms[12].type = PLATFORM;
    state.platforms[12].position = glm::vec3(5.0f, -2.25f, 0.0f);
    state.platforms[12].textureID = platformTextureID;

    for (int i = 13; i < 18; i++) {
        state.platforms[i].type = PLATFORM;
        state.platforms[i].position = glm::vec3(-15.0f + i, 0.75f, 0.0f);
        state.platforms[i].textureID = platformTextureID;
    }

    state.platforms[18].type = PLATFORM;
    state.platforms[18].position = glm::vec3(4.0f, -2.25f, 0.0f);
    state.platforms[18].textureID = platformTextureID;

    state.platforms[19].type = PLATFORM;
    state.platforms[19].position = glm::vec3(5.0f, -0.25f, 0.0f);
    state.platforms[19].textureID = platformTextureID;

    for (int i = 0; i < PLATFORM_COUNT; i++)
        state.platforms[i].Update(0, state.player, NULL, 0, NULL, 0);

    baddiesleft = BADDY_COUNT;
    state.baddies = new Entity[BADDY_COUNT];
    GLuint roscoeTextureID = LoadTexture("Textures/roscoe.png");
    GLuint rupertTextureID = LoadTexture("Textures/rupert.png");
    GLuint ozzpreyTextureID = LoadTexture("Textures/ozzprey.png");

    state.baddies[0].type = BADDY;
    state.baddies[0].ai = PACER;
    state.baddies[0].position = glm::vec3(3.0f, -2.25f, 0.0f);
    state.baddies[0].movement.x = 1;
    state.baddies[0].acceleration = 16.0f;
    state.baddies[0].walkSpeed = 80.0f;
    state.baddies[0].textureID = roscoeTextureID;

    state.baddies[0].animDefault = new int[2]{ 0, 1 };
    state.baddies[0].animIndices = state.baddies[0].animDefault;
    state.baddies[0].animFrames = 2;
    state.baddies[0].animRows = 2;
    state.baddies[0].animCols = 1;

    state.baddies[1].type = BADDY;
    state.baddies[1].ai = HOPPER;
    state.baddies[1].state = WAIT;
    state.baddies[1].position = glm::vec3(-1.5f, 1.75f, 0.0f);
    state.baddies[1].acceleration = 16.0f;
    state.baddies[1].walkSpeed = 80.0f;
    state.baddies[1].jumpSpeed = 6.0f;
    state.baddies[1].height = 0.8f;
    state.baddies[1].textureID = rupertTextureID;

    state.baddies[1].animRows = 2;
    state.baddies[1].animCols = 1;

    state.baddies[2].type = BADDY;
    state.baddies[2].ai = SINER;
    state.baddies[2].position = glm::vec3(4.0f, 1.0f, 0.0f);
    state.baddies[2].movement.x = -1;
    state.baddies[2].acceleration = 16.0f;
    state.baddies[2].walkSpeed = 120.0f;
    state.baddies[2].height = 0.6f;
    state.baddies[2].gravity = glm::vec3(0);
    state.baddies[2].textureID = ozzpreyTextureID;

    state.baddies[2].animRows = 3;
    state.baddies[2].animCols = 1;

    fontTextureID = LoadTexture("Textures/pixel_font.png");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    level1 = Mix_LoadMUS("Sounds/stage1.mp3");
    victory = Mix_LoadMUS("Sounds/victory.mp3");
    die = Mix_LoadWAV("Sounds/doh.wav");
    stomp = Mix_LoadWAV("Sounds/stomp.wav");

    Mix_PlayMusic(level1, -1);
}

void ProcessInput() {
    state.player->movement = glm::vec3(0);

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
				if (state.player->collidedBottom != NULL) {
					if (state.player->collidedBottom->type == PLATFORM)
						state.player->isJumping = true;
				}
				break;
			}
		}
	}

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_D])
        state.player->movement.x = 1.0f;
    if (keys[SDL_SCANCODE_A])
        state.player->movement.x = -1.0f;
    if (keys[SDL_SCANCODE_LSHIFT])
        state.player->isRunning = true;

    if (glm::length(state.player->movement) > 1.0f)
        state.player->movement = glm::normalize(state.player->movement);
}

void Update() {
    if (state.player->isDead || state.player->isWin) return;

    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
		state.player->Update(FIXED_TIMESTEP,
            NULL,
			state.platforms, PLATFORM_COUNT,
			state.baddies, BADDY_COUNT);
        for (int i = 0; i < BADDY_COUNT; i++) {
            state.baddies[i].Update(FIXED_TIMESTEP,
                state.player,
                state.platforms, PLATFORM_COUNT,
                NULL, 0);
        }
        if (state.player->collidedBottom != NULL) {
            if (state.player->collidedBottom->type == BADDY) {
                state.player->collidedBottom->isActive = false;
                if (--baddiesleft == 0) {
                    state.player->isWin = true;
                    Mix_PlayMusic(victory, 0);
                }
                state.player->isJumping = true;
                Mix_PlayChannel(-1, stomp, 0);
            }
        }
        if (state.player->collidedTop != NULL) {
            if (state.player->collidedTop->type == BADDY) {
                state.player->isDead = true;
                Mix_PlayChannel(-1, die, 0);
                Mix_HaltMusic();
            }
        }
        if (state.player->collidedLeft != NULL) {
            if (state.player->collidedLeft->type == BADDY) {
                state.player->isDead = true;
                Mix_PlayChannel(-1, die, 0);
                Mix_HaltMusic();
            }
        }
        if (state.player->collidedRight != NULL) {
            if (state.player->collidedRight->type == BADDY) {
                state.player->isDead = true;
                Mix_PlayChannel(-1, die, 0);
                Mix_HaltMusic();
            }
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text,
    float size, float spacing, glm::vec3 position) {
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++) {
        int index = (int)text[i];
        float offset = (size + spacing) * i;

        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;

        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            });

        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height,
            });
    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++)
        state.platforms[i].Render(&program);

    for (int i = 0; i < BADDY_COUNT; i++)
        state.baddies[i].Render(&program);

    state.player->Render(&program);

    if (state.player->isDead)
        DrawText(&program, fontTextureID, "YOU DIED", 0.25f, 0.0f, glm::vec3(-4.0f, 3.0f, 0.0f));
    if (state.player->isWin)
        DrawText(&program, fontTextureID, "TARGETS DESTROYED", 0.25f, 0.0f, glm::vec3(-4.0f, 3.0f, 0.0f));

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    Mix_FreeMusic(level1);
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