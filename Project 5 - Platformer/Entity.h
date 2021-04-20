#pragma once
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
#include "Map.h"
#include <cmath>

enum EntityType { PLAYER, BADDY };

enum AIType { AI_DEF, PACER, SINER, HOPPER, BOSS_PACER };
enum AIState { ST_DEF, WAIT, CHASE };

class Entity {
public:
	EntityType type;

	AIType ai = AI_DEF;
	AIState state = ST_DEF;

	glm::vec3 position;
	glm::vec3 movement;
	glm::vec3 gravity;
	glm::vec3 velocity;

	glm::mat4 modelMatrix;

	float acceleration = 0.0f, walkSpeed = 0.0f, runSpeed = 0.0f, jumpSpeed = 0.0f;
	bool isJumping = false, inJumpState = false, isRunning = false, isInvincible = false;

	float width = 1.0f, height = 1.0f, draw_width = 1.0f, draw_height = 1.0f;

	bool collidedTopMap = false;
	bool collidedBottomMap = false;
	bool collidedLeftMap = false;
	bool collidedRightMap = false;

	Entity* collidedTopEnt = NULL;
	Entity* collidedBottomEnt = NULL;
	Entity* collidedLeftEnt = NULL;
	Entity* collidedRightEnt = NULL;

	float timeActive = 0.0f, jumpTimer = 0.0f, jumpTimeMax = 0.0f, iTimer = 0.0f;

	bool isActive = true;
	bool isDead = false;
	bool isWin = false;

	int health = 1;

	GLuint textureID = -1;

	int* animDefault = NULL, * animLeft = NULL, * animRight = NULL, * animJumpLeft = NULL, * animJumpRight = NULL;
	int* animIndices = NULL;
	int animFrames = 0;
	int animIndex = 0;
	float animTime = 0.0f;
	int animCols = 0;
	int animRows = 0;

	Entity();

	void AI(Entity* player);
	void AIPacer();
	void AISiner();
	void AIHopper(Entity* player);
	void AIBossPacer(Entity* player);

	bool isCollideBoxtoBox(Entity* other);
	void handleCollisionsY(Entity* objects, int objectCount, bool correct);
	void handleCollisionsX(Entity* objects, int objectCount, bool correct);
	void handleMapCollisionsY(Map* map);
	void handleMapCollisionsX(Map* map);
	void Update(float timestep,
		Entity* player,
		Map* map,
		Entity* baddies, int baddycount);
	void DrawSpriteFromTextureAtlas(ShaderProgram* program, int index);
	void Render(ShaderProgram* program);
};