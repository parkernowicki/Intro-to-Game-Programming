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

enum EntityType { PLAYER, BADDY, JEWEL, GATE };

enum AIType { AI_DEF, PACER, SINER, CHASER, BOSS_PACER };
enum AIState { ST_DEF, WAIT, CHASE };

class Entity {
public:
	EntityType type;

	AIType ai = AI_DEF;
	AIState state = ST_DEF;

	glm::vec3 position;
	glm::vec3 movement;
	glm::vec3 velocity;

	glm::mat4 modelMatrix;

	float acceleration = 0.0f, moveSpeed = 0.0f;

	float width = 1.0f, height = 1.0f, draw_width = 1.0f, draw_height = 1.0f;

	bool collidedTopMap = false;
	bool collidedBottomMap = false;
	bool collidedLeftMap = false;
	bool collidedRightMap = false;

	Entity* collidedTopEnt = NULL;
	Entity* collidedBottomEnt = NULL;
	Entity* collidedLeftEnt = NULL;
	Entity* collidedRightEnt = NULL;

	float timeActive = 0.0f, iTimer = 0.0f, iTimerMax = 0.2f, meleeTimer = 0.0f;

	bool isInvincible = false;
	bool badHitState = false;
	bool isMeleeAttack = false;
	bool isActive = true;
	bool isDead = false;
	bool isWin = false;
	bool isOpen = false;
	bool isGlobal = false;
	bool bossEntrance = false;

	int health = 1;

	GLuint textureID = -1;

	int *animDefault = NULL, *animLeft = NULL, *animRight = NULL, *animUp = NULL, *animDown = NULL;
	int *animAttackLeft = NULL, *animAttackRight = NULL, *animAttackUp = NULL, *animAttackDown = NULL;
	int *animIndices = NULL;
	int animFrames = 0;
	int animIndex = 0;
	float animTime = 0.0f;
	int animCols = 0;
	int animRows = 0;

	Entity();

	void AI(Entity* player);
	void AIPacer();
	void AISiner(Entity* player);
	void AIChaser(Entity* player);
	void AIBossPacer(Entity* player);

	bool isCollideBoxtoBox(Entity* other);
	void handleCollisionsY(Entity* objects, int objectCount, bool correct);
	void handleCollisionsX(Entity* objects, int objectCount, bool correct);
	void handleMapCollisionsY(Map* map);
	void handleMapCollisionsX(Map* map);
	void Update(float timestep,
		Entity* player,
		Map* map,
		Entity* baddies, int baddycount,
		Entity* jewels, int jewelcount,
		Entity* gate);
	void DrawSpriteFromTextureAtlas(ShaderProgram* program, int index);
	void Render(ShaderProgram* program);
};