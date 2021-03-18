/*
* Project 3 - Lunar Lander
*  CONTROLS:
*  Up: W
*  Down: S
*  Left: A
*  Right: D
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

#define WALL_COUNT 32

struct GameState {
	Entity* player;
    Entity* walls;
    Entity* platform;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;

glm::mat4 viewMatrix, projectionMatrix;

GLuint fontTextureID;

#define FIXED_TIMESTEP 1.0f / 60.0f
float lastTicks = 0.0f;
float accumulator = 0.0f;

Mix_Music* music, *music_victory;
Mix_Chunk* die;

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
    displayWindow = SDL_CreateWindow("UFO Adventure!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    state.player = new Entity;
    state.player->type = PLAYER;
    state.player->position = glm::vec3(0.0f, 3.0f, 0.0f);
    state.player->acceleration = glm::vec3(0.0f, -0.1f, 0.0f);
    state.player->speed = 0.5f;
    state.player->textureID = LoadTexture("Textures/ufo.png");

	state.player->animDefault = new int[4]{ 0, 2, 4, 6 };
    state.player->animIndices = state.player->animDefault;
    state.player->animFrames = 4;
    state.player->animRows = 4;
    state.player->animCols = 2;

    state.walls = new Entity[WALL_COUNT];
    GLuint wallTextureID = LoadTexture("Textures/block.png");

    state.walls[0].type = WALL;
    state.walls[0].textureID = wallTextureID;
    state.walls[0].position = glm::vec3(-4.5f, 3.25f, 0.0f);

    state.walls[1].type = WALL;
    state.walls[1].textureID = wallTextureID;
    state.walls[1].position = glm::vec3(-4.5f, 2.25f, 0.0f);

    state.walls[2].type = WALL;
    state.walls[2].textureID = wallTextureID;
    state.walls[2].position = glm::vec3(-4.5f, 1.25f, 0.0f);

    state.walls[3].type = WALL;
    state.walls[3].textureID = wallTextureID;
    state.walls[3].position = glm::vec3(-4.5f, 0.25f, 0.0f);

    state.walls[4].type = WALL;
    state.walls[4].textureID = wallTextureID;
    state.walls[4].position = glm::vec3(-4.5f, -0.75f, 0.0f);

    state.walls[5].type = WALL;
    state.walls[5].textureID = wallTextureID;
    state.walls[5].position = glm::vec3(-4.5f, -1.75f, 0.0f);

    state.walls[6].type = WALL;
    state.walls[6].textureID = wallTextureID;
    state.walls[6].position = glm::vec3(-4.5f, -2.75f, 0.0f);

    state.walls[7].type = WALL;
    state.walls[7].textureID = wallTextureID;
    state.walls[7].position = glm::vec3(-4.5f, -3.75f, 0.0f);

    state.walls[8].type = WALL;
    state.walls[8].textureID = wallTextureID;
    state.walls[8].position = glm::vec3(-3.5f, -3.75f, 0.0f);

    state.walls[9].type = WALL;
    state.walls[9].textureID = wallTextureID;
    state.walls[9].position = glm::vec3(-2.5f, -3.75f, 0.0f);

    state.walls[10].type = WALL;
    state.walls[10].textureID = wallTextureID;
    state.walls[10].position = glm::vec3(-1.5f, -3.75f, 0.0f);

    state.walls[11].type = WALL;
    state.walls[11].textureID = wallTextureID;
    state.walls[11].position = glm::vec3(-0.5f, -3.75f, 0.0f);

    state.walls[12].type = WALL;
    state.walls[12].textureID = wallTextureID;
    state.walls[12].position = glm::vec3(0.5f, -3.75f, 0.0f);

    state.walls[13].type = WALL;
    state.walls[13].textureID = wallTextureID;
    state.walls[13].position = glm::vec3(1.5f, -3.75f, 0.0f);

    state.walls[14].type = WALL;
    state.walls[14].textureID = wallTextureID;
    state.walls[14].position = glm::vec3(2.5f, -3.75f, 0.0f);

    state.walls[15].type = WALL;
    state.walls[15].textureID = wallTextureID;
    state.walls[15].position = glm::vec3(3.5f, -3.75f, 0.0f);

    state.walls[16].type = WALL;
    state.walls[16].textureID = wallTextureID;
    state.walls[16].position = glm::vec3(4.5f, -3.75f, 0.0f);

    state.walls[17].type = WALL;
    state.walls[17].textureID = wallTextureID;
    state.walls[17].position = glm::vec3(4.5f, -2.75f, 0.0f);

    state.walls[18].type = WALL;
    state.walls[18].textureID = wallTextureID;
    state.walls[18].position = glm::vec3(4.5f, -1.75f, 0.0f);

    state.walls[19].type = WALL;
    state.walls[19].textureID = wallTextureID;
    state.walls[19].position = glm::vec3(4.5f, -0.75f, 0.0f);

    state.walls[20].type = WALL;
    state.walls[20].textureID = wallTextureID;
    state.walls[20].position = glm::vec3(4.5f, 0.25f, 0.0f);

    state.walls[21].type = WALL;
    state.walls[21].textureID = wallTextureID;
    state.walls[21].position = glm::vec3(4.5f, 1.25f, 0.0f);

    state.walls[22].type = WALL;
    state.walls[22].textureID = wallTextureID;
    state.walls[22].position = glm::vec3(4.5f, 2.25f, 0.0f);

    state.walls[23].type = WALL;
    state.walls[23].textureID = wallTextureID;
    state.walls[23].position = glm::vec3(4.5f, 3.25f, 0.0f);

    state.walls[24].type = WALL;
    state.walls[24].textureID = wallTextureID;
    state.walls[24].position = glm::vec3(-3.5f, 0.25f, 0.0f);

    state.walls[25].type = WALL;
    state.walls[25].textureID = wallTextureID;
    state.walls[25].position = glm::vec3(-2.5f, 0.25f, 0.0f);

    state.walls[26].type = WALL;
    state.walls[26].textureID = wallTextureID;
    state.walls[26].position = glm::vec3(-1.5f, 0.25f, 0.0f);

    state.walls[27].type = WALL;
    state.walls[27].textureID = wallTextureID;
    state.walls[27].position = glm::vec3(-0.5f, 0.25f, 0.0f);

    state.walls[28].type = WALL;
    state.walls[28].textureID = wallTextureID;
    state.walls[28].position = glm::vec3(-1.5f, -2.75f, 0.0f);

    state.walls[29].type = WALL;
    state.walls[29].textureID = wallTextureID;
    state.walls[29].position = glm::vec3(0.5f, 0.25f, 0.0f);

    state.walls[30].type = WALL;
    state.walls[30].textureID = wallTextureID;
    state.walls[30].position = glm::vec3(1.5f, 0.25f, 0.0f);

    state.walls[31].type = WALL;
    state.walls[31].textureID = wallTextureID;
    state.walls[31].position = glm::vec3(1.5f, -0.75f, 0.0f);

    for (int i = 0; i < WALL_COUNT; i++)
        state.walls[i].Update(0, NULL, 0, NULL, 0);

    state.platform = new Entity;
    state.platform->type = PLATFORM;
    state.platform->height = 0.25;
    state.platform->position = glm::vec3(-3.0f, -3.125f, 0.0f);
    state.platform->textureID = LoadTexture("Textures/platform.png");

    state.platform->Update(0, NULL, 0, NULL, 0);

    fontTextureID = LoadTexture("Textures/pixel_font.png");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    music = Mix_LoadMUS("Sounds/ufo_1.mp3");
    music_victory = Mix_LoadMUS("Sounds/ufo_2.mp3");
    die = Mix_LoadWAV("Sounds/doh.wav");

    Mix_PlayMusic(music, -1);
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
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_D])
        state.player->movement.x = 1.0f;
    if (keys[SDL_SCANCODE_A])
        state.player->movement.x = -1.0f;
    if (keys[SDL_SCANCODE_W])
        state.player->movement.y = 1.0f;
    if (keys[SDL_SCANCODE_S])
        state.player->movement.y = -1.0f;

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
        state.player->Update(FIXED_TIMESTEP, state.walls, WALL_COUNT, state.platform, 1);
        if (state.player->lastCollided != NULL) {
            if (state.player->lastCollided->type == WALL) {
                state.player->isDead = true;
                Mix_PlayChannel(-1, die, 0);
                Mix_HaltMusic();
            }
            if (state.player->lastCollided->type == PLATFORM) {
                state.player->isWin = true;
                Mix_PlayMusic(music_victory, -1);
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

    for (int i = 0; i < WALL_COUNT; i++)
        state.walls[i].Render(&program);

    state.platform->Render(&program);

    state.player->Render(&program);

    if (state.player->isDead)
        DrawText(&program, fontTextureID, "Mission Failed", 0.25f, 0.0f, glm::vec3(-3.0f, 3.0f, 0.0f));
    if (state.player->isWin)
        DrawText(&program, fontTextureID, "Mission Successful", 0.25f, 0.0f, glm::vec3(-3.0f, 3.0f, 0.0f));

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    Mix_FreeMusic(music);
    Mix_FreeMusic(music_victory);
    Mix_FreeChunk(die);

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