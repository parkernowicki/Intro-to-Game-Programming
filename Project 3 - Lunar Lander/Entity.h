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

enum EntityType { PLAYER, WALL, PLATFORM };

class Entity {
public:
	EntityType type;

	glm::vec3 position;
	glm::vec3 movement;
	glm::vec3 acceleration;
	glm::vec3 velocity;

	float speed;

	float width = 1.0f, height = 1.0f;

	GLuint textureID = 0;

	glm::mat4 modelMatrix;

	int* animDefault = NULL;
	int* animIndices = NULL;
	int animFrames = 0;
	int animIndex = 0;
	float animTime = 0;
	int animCols = 0;
	int animRows = 0;

	bool isActive = true;
	bool isDead = false;
	bool isWin = false;

	Entity* lastCollided = NULL;

	Entity();

	bool isCollideBoxtoBox(Entity* other);
	void handleCollisionsX(Entity* objects, int objectCount);
	void handleCollisionsY(Entity* objects, int objectCount);
	void Update(float timestep, Entity* walls, int wallcount, Entity* platform, int platformcount);
	void DrawSpriteFromTextureAtlas(ShaderProgram* program, int index);
	void Render(ShaderProgram* program);
};