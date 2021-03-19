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

enum EntityType { PLAYER, PLATFORM, BADDY };

enum AIType { WALKER, WAITANDGO };
enum AIState { PATROL };

class Entity {
public:
	EntityType type;

	AIType ai;
	AIState state;

	glm::vec3 position;
	glm::vec3 movement;
	glm::vec3 gravity;
	glm::vec3 velocity;

	glm::mat4 modelMatrix;

	float acceleration = 0.0f, topSpeed = 0.0f;
	bool isJumping = false;

	float width = 1.0f, height = 1.0f;

	Entity* entColliding = NULL;

	bool collidedTop = false;
	bool collidedBottom = false;
	bool collidedLeft = false;
	bool collidedRight = false;

	bool isActive = true;
	bool isDead = false;
	bool isWin = false;

	GLuint textureID = -1;

	int* animDefault = NULL;
	int* animIndices = NULL;
	int animFrames = 0;
	int animIndex = 0;
	float animTime = 0;
	int animCols = 0;
	int animRows = 0;

	Entity();

	void AI(Entity* player);
	void AIWalker();
	void AIWaitandGo(Entity* player);

	bool isCollideBoxtoBox(Entity* other);
	void handleCollisionsX(Entity* objects, int objectCount);
	void handleCollisionsY(Entity* objects, int objectCount);
	void Update(float timestep,
		Entity* player,
		Entity* platforms, int platformcount,
		Entity* baddies, int baddycount);
	void DrawSpriteFromTextureAtlas(ShaderProgram* program, int index);
	void Render(ShaderProgram* program);
};