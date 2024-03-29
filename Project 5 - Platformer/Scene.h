#pragma once
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
#include "Util.h"
#include "Entity.h"
#include "Map.h"

struct GameState {
	Map* map;
	Entity* player;
	Entity* baddies;
	int nextScene;
};

class Scene {
public:
	GameState state;

	glm::mat4 uiViewMatrix, uiProjectionMatrix;

	int baddiesleft;

	Mix_Music* leveltheme, * victory, * dead;
	Mix_Chunk* hurt, * stomp, * bosshit;

	GLuint backgroundTextureID, fontTextureID, livesTextureID;

	virtual void Initialize() = 0;
	virtual void Update(float deltaTime, int* lives) = 0;
	virtual void Render(ShaderProgram* program, int lives) = 0;
	virtual void RenderBackground(ShaderProgram* program) = 0;
};