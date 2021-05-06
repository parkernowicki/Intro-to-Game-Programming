#include "Level1.h"

#define LEVEL1_WIDTH 48
#define LEVEL1_HEIGHT 48

#define LEVEL1_BADDYCOUNT 34
#define LEVEL1_JEWELCOUNT 3

unsigned int level1_data[] =
{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 4, 1, 4, 3, 3, 4, 4, 4, 4, 4, 4, 4, 1, 3, 4, 4, 4, 4, 4, 4, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 4, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 4, 1, 1, 4, 1, 1, 1, 1, 4, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 1, 1, 1, 1, 4, 3, 1, 1, 1, 1, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 3, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 4, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 1, 1, 4, 1, 4, 4, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 4, 4, 4, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 4, 1, 1, 1, 4, 4, 1, 4, 4, 4, 1, 1, 4, 4, 1, 1, 1, 1, 1, 2, 1, 3, 3, 3,
    3, 3, 3, 3, 2, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 4, 4, 4, 3, 3,
    3, 3, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 4, 1, 1, 1, 4, 1, 1, 4, 3, 3,
    3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 3, 1, 1, 1, 1, 1, 1, 4, 3, 3,
    3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 4, 4, 4, 1, 1, 1, 1, 4, 1, 1, 4, 3, 3,
    3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 3, 3, 3, 4, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 4, 4, 4, 3, 3,
    3, 3, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3,
    3, 3, 1, 1, 1, 1, 3, 3, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1, 3, 3, 3, 3, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 4, 1, 1, 1, 3, 3, 3,
    3, 3, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 4, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3,
    3, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3,
    3, 3, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 2, 3,
    3, 3, 1, 1, 3, 3, 3, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 2, 1, 1, 2, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1, 1, 1, 1, 1, 2, 3,
    3, 3, 1, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 2, 2, 2, 2, 2, 1, 1, 2, 3,
    3, 3, 1, 1, 1, 1, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 2, 1, 1, 1, 1, 1, 1, 2, 3,
    3, 3, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2,
    3, 3, 3, 3, 1, 1, 3, 3, 3, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 2,
    3, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 2, 2, 1, 1, 1, 2, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2,
    3, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 2, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
    3, 3, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 3, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
    3, 3, 1, 1, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 3, 1, 2, 1, 3, 3, 3, 3, 3, 1, 1, 3, 3, 3, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
    3, 3, 1, 1, 1, 3, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 3, 3, 3, 1, 1, 1, 3, 3, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 2, 2,
    3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 1, 1, 1, 3, 3, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
    3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 1, 1, 1, 3, 3, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 2,
    3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 1, 1, 1, 3, 3, 2, 2, 2, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3,
    3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 1, 1, 1, 2, 3, 3, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,
    3, 3, 3, 3, 3, 3, 2, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 3, 3, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3,
    3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 1, 1, 3, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3,
    3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 1, 1, 1, 2, 3, 2, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 2,
    3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 2, 3, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
    3, 3, 1, 1, 1, 1, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 3, 1, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2,
    3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 3, 3, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
    3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 2, 3, 3, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3,
    3, 3, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 1, 1, 3, 3, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3,
    3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 2, 3, 3, 3, 3, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3,
    3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 1, 1, 1, 3, 3, 3, 3, 3, 3, 1, 1, 2, 1, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

void Level1::Initialize() {
    state.nextScene = -1;

    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	GLuint mapTextureID = Util::LoadTexture("Textures/tile1.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 5, 1);

    //Init player
    state.player = new Entity;
    state.player->type = PLAYER;
    state.player->position = glm::vec3(23.5f, -24.0f, 0.0f);
    state.player->acceleration = 16.0f;
    state.player->moveSpeed = 150.0f;
    state.player->width = 0.6f;
    state.player->draw_width = 2.0f;
    state.player->draw_height = 2.0f;
    state.player->iTimerMax = 1.5f;
    state.player->textureID = Util::LoadTexture("Textures/skelly.png");

    state.player->animRight = new int[2]{ 0, 6 };
    state.player->animLeft = new int[2]{ 1, 7 };
    state.player->animDown = new int[2]{ 2, 8 };
    state.player->animUp = new int[2]{ 3, 9 };
    state.player->animAttackRight = new int[1]{ 5 };
    state.player->animAttackLeft = new int[1]{ 11 };
    state.player->animAttackDown = new int[1]{17 };
    state.player->animAttackUp = new int[1]{ 23 };
    state.player->animIndices = state.player->animDown;
    state.player->animFrames = 2;
    state.player->animRows = 4;
    state.player->animCols = 6;

    //Init baddies
    baddiesleft = LEVEL1_BADDYCOUNT;
    state.baddies = new Entity[LEVEL1_BADDYCOUNT];
    GLuint roscoeTextureID = Util::LoadTexture("Textures/roscoe.png");
    GLuint rupertTextureID = Util::LoadTexture("Textures/rupert.png");
    GLuint ozzpreyTextureID = Util::LoadTexture("Textures/ozzprey.png");

    //The Roscoes
    for (int i = 0; i < 18; i++) {
        state.baddies[i].type = BADDY;
        state.baddies[i].ai = PACER;
        state.baddies[i].acceleration = 64.0f;
        state.baddies[i].moveSpeed = 80.0f;
        state.baddies[i].health = 2;
        state.baddies[i].textureID = roscoeTextureID;

        state.baddies[i].animDefault = new int[2]{ 0, 1 };
        state.baddies[i].animIndices = state.baddies[i].animDefault;
        state.baddies[i].animFrames = 2;
        state.baddies[i].animRows = 2;
        state.baddies[i].animCols = 1;
    }
    state.baddies[0].position = glm::vec3(8.0f, -21.0f, 0.0f);
    state.baddies[1].position = glm::vec3(13.0f, -21.0f, 0.0f);
    state.baddies[2].position = glm::vec3(12.0f, -13.0f, 0.0f);
    state.baddies[3].position = glm::vec3(8.0f, -12.0f, 0.0f);
    state.baddies[4].position = glm::vec3(4.0f, -20.0f, 0.0f);
    state.baddies[5].position = glm::vec3(4.0f, -27.0f, 0.0f);
    state.baddies[6].position = glm::vec3(15.0f, -31.0f, 0.0f);
    state.baddies[7].position = glm::vec3(8.0f, -42.0f, 0.0f);
    state.baddies[8].position = glm::vec3(13.0f, -39.0f, 0.0f);
    state.baddies[9].position = glm::vec3(24.0f, -36.0f, 0.0f);
    state.baddies[10].position = glm::vec3(36.0f, -42.0f, 0.0f);
    state.baddies[11].position = glm::vec3(40.0f, -42.0f, 0.0f);
    state.baddies[12].position = glm::vec3(31.0f, -33.0f, 0.0f);
    state.baddies[13].position = glm::vec3(41.0f, -32.0f, 0.0f);
    state.baddies[14].position = glm::vec3(41.0f, -26.0f, 0.0f);
    state.baddies[15].position = glm::vec3(34.0f, -6.0f, 0.0f);
    state.baddies[16].position = glm::vec3(27.0f, -6.0f, 0.0f);
    state.baddies[17].position = glm::vec3(19.0f, -7.0f, 0.0f);

    //The Ruperts
    for (int i = 18; i < 25; i++) {
        state.baddies[i].type = BADDY;
        state.baddies[i].ai = CHASER;
        state.baddies[i].state = WAIT;
        state.baddies[i].acceleration = 4.0f;
        state.baddies[i].moveSpeed = 100.0f;
        state.baddies[i].health = 2;
        state.baddies[i].height = 0.8f;
        state.baddies[i].textureID = rupertTextureID;

        state.baddies[i].animRows = 2;
        state.baddies[i].animCols = 1;
    }
    state.baddies[18].position = glm::vec3(39.0f, -15.0f, 0.0f);
    state.baddies[19].position = glm::vec3(43.0f, -6.0f, 0.0f);
    state.baddies[20].position = glm::vec3(32.0f, -16.0f, 0.0f);
    state.baddies[21].position = glm::vec3(28.0f, -12.0f, 0.0f);
    state.baddies[22].position = glm::vec3(22.0f, -12.0f, 0.0f);
    state.baddies[23].position = glm::vec3(25.0f, -4.0f, 0.0f);
    state.baddies[24].position = glm::vec3(16.0f, -3.0f, 0.0f);

    //The Ozzpreys
    for (int i = 25; i < 34; i++) {
        state.baddies[i].type = BADDY;
        state.baddies[i].ai = SINER;
        state.baddies[i].movement.x = -1;
        state.baddies[i].acceleration = 16.0f;
        state.baddies[i].moveSpeed = 120.0f;
        state.baddies[i].height = 0.6f;
        state.baddies[i].textureID = ozzpreyTextureID;

        state.baddies[i].animRows = 3;
        state.baddies[i].animCols = 1;
    }
    state.baddies[25].position = glm::vec3(12.0f, -33.0f, 0.0f);
    state.baddies[26].position = glm::vec3(13.0f, -42.0f, 0.0f);
    state.baddies[27].position = glm::vec3(15.0f, -43.0f, 0.0f);
    state.baddies[28].position = glm::vec3(36.0f, -42.5f, 0.0f);
    state.baddies[29].position = glm::vec3(44.0f, -42.5f, 0.0f);
    state.baddies[30].position = glm::vec3(36.0f, -37.5f, 0.0f);
    state.baddies[31].position = glm::vec3(44.0f, -37.5f, 0.0f);
    state.baddies[32].position = glm::vec3(38.0f, -33.0f, 0.0f);
    state.baddies[33].position = glm::vec3(46.0f, -33.0f, 0.0f);

    //Init jewels
    state.jewels = new Entity[LEVEL1_JEWELCOUNT];
    GLuint fJewelTextureID = Util::LoadTexture("Textures/forest_jewel.png");
    GLuint mJewelTextureID = Util::LoadTexture("Textures/mountain_jewel.png");
    GLuint rJewelTextureID = Util::LoadTexture("Textures/ruins_jewel.png");

    for (int i = 0; i < LEVEL1_JEWELCOUNT; i++) {
        state.jewels[i].type = JEWEL;

        state.jewels[i].animDefault = new int[2]{ 0, 1 };
        state.jewels[i].animIndices = state.jewels[i].animDefault;
        state.jewels[i].animFrames = 2;
        state.jewels[i].animRows = 2;
        state.jewels[i].animCols = 1;
    }
    state.jewels[0].position = glm::vec3(17.0f, -41.0f, 0.0f);
    state.jewels[0].textureID = fJewelTextureID;
    state.jewels[1].position = glm::vec3(42.0f, -20.5f, 0.0f);
    state.jewels[1].textureID = mJewelTextureID;
    state.jewels[2].position = glm::vec3(13.0f, -3.0f, 0.0f);
    state.jewels[2].textureID = rJewelTextureID;

    //Init gate
    state.gate = new Entity;
    GLuint gateTextureID = Util::LoadTexture("Textures/jewel_gate.png");

    state.gate->type = GATE;
    state.gate->draw_width = 2.0f;
    state.gate->textureID = gateTextureID;
    state.gate->animRows = 2;
    state.gate->animCols = 1;
    state.gate->position = glm::vec3(23.5f, -21.0f, 0.0f);

    glClearColor(0.941f, 0.886f, 0.588f, 1.0f);

    fontTextureID = Util::LoadTexture("Textures/pixel_font.png");
    livesTextureID = Util::LoadTexture("Textures/lives.png");
    fJewelIconTextureID = Util::LoadTexture("Textures/forest_jewel_icon.png");
    mJewelIconTextureID = Util::LoadTexture("Textures/mountain_jewel_icon.png");
    rJewelIconTextureID = Util::LoadTexture("Textures/ruins_jewel_icon.png");

    leveltheme = Mix_LoadMUS("Sounds/overworld.mp3");
    victory = Mix_LoadMUS("Sounds/victory.mp3");
    dead = Mix_LoadMUS("Sounds/dead.mp3");
    hurt = Mix_LoadWAV("Sounds/doh.wav");
    schwing = Mix_LoadWAV("Sounds/schwing.wav");
    warp = Mix_LoadWAV("Sounds/warp.wav");

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_VolumeChunk(hurt, MIX_MAX_VOLUME / 2);
    Mix_VolumeChunk(warp, MIX_MAX_VOLUME / 2);
    Mix_PlayMusic(leveltheme, -1);
}

void Level1::Update(float timestep, int* lives) {
    state.player->Update(timestep,
        NULL,
        state.map,
        state.baddies, LEVEL1_BADDYCOUNT,
        state.jewels, LEVEL1_JEWELCOUNT,
        state.gate);
    for (int i = 0; i < LEVEL1_BADDYCOUNT; i++) {
        state.baddies[i].Update(timestep,
            state.player,
            state.map,
            NULL, 0,
            NULL, 0,
            state.gate);
    }
    for (int i = 0; i < LEVEL1_JEWELCOUNT; i++) {
        state.jewels[i].Update(timestep,
            state.player,
            state.map,
            NULL, 0,
            NULL, 0,
            state.gate);
    }
    state.gate->Update(timestep,
        state.player,
        state.map,
        NULL, 0,
        NULL, 0,
        NULL);
    if (state.player->collidedBottomEnt != NULL) {
        if (state.player->collidedBottomEnt->type == BADDY
            && state.player->collidedBottomEnt->isActive
            && !state.player->isInvincible) {
            *lives -= 1;
            if (*lives == 0) {
                state.player->isDead = true;
                state.player->isInvincible = false;
                Mix_PlayMusic(dead, 1);
            }
            else {
                state.player->isInvincible = true;
                Mix_PlayChannel(-1, hurt, 0);
            }
        }
        else if (state.player->collidedBottomEnt->type == JEWEL
            && state.player->collidedBottomEnt->isActive) {
            state.player->collidedBottomEnt->isActive = false;
            state.player->position = glm::vec3(23.5f, -24.0f, 0.0f);
            *lives += 3;
            Mix_PlayChannel(-1, warp, 0);
            if (!state.jewels[0].isActive && !state.jewels[1].isActive
                && !state.jewels[2].isActive)
                state.gate->isOpen = true;
        }
        else if (state.player->collidedBottomEnt->type == GATE
            && state.gate->isOpen)
            state.nextScene = 2;
    }
    else if (state.player->collidedTopEnt != NULL) {
        if (state.player->collidedTopEnt->type == BADDY
            && state.player->collidedTopEnt->isActive
            && !state.player->isInvincible) {
            *lives -= 1;
            if (*lives == 0) {
                state.player->isDead = true;
                state.player->isInvincible = false;
                Mix_PlayMusic(dead, 1);
            }
            else {
                state.player->isInvincible = true;
                Mix_PlayChannel(-1, hurt, 0);
            }
        }
        else if (state.player->collidedTopEnt->type == JEWEL
            && state.player->collidedTopEnt->isActive) {
            state.player->collidedTopEnt->isActive = false;
            state.player->position = glm::vec3(23.5f, -24.0f, 0.0f);
            *lives += 3;
            Mix_PlayChannel(-1, warp, 0);
            if (!state.jewels[0].isActive && !state.jewels[1].isActive
                && !state.jewels[2].isActive)
                state.gate->isOpen = true;
        }
        else if (state.player->collidedTopEnt->type == GATE
            && state.gate->isOpen)
            state.nextScene = 2;
    }
    else if (state.player->collidedLeftEnt != NULL) {
        if (state.player->collidedLeftEnt->type == BADDY
            && state.player->collidedLeftEnt->isActive
            && !state.player->isInvincible) {
            *lives -= 1;
            if (*lives == 0) {
                state.player->isDead = true;
                state.player->isInvincible = false;
                Mix_PlayMusic(dead, 1);
            }
            else {
                state.player->isInvincible = true;
                Mix_PlayChannel(-1, hurt, 0);
            }
        }
        else if (state.player->collidedLeftEnt->type == JEWEL
            && state.player->collidedLeftEnt->isActive) {
            state.player->collidedLeftEnt->isActive = false;
            state.player->position = glm::vec3(23.5f, -24.0f, 0.0f);
            *lives += 3;
            Mix_PlayChannel(-1, warp, 0);
            if (!state.jewels[0].isActive && !state.jewels[1].isActive
                && !state.jewels[2].isActive)
                state.gate->isOpen = true;
        }
        else if (state.player->collidedLeftEnt->type == GATE
            && state.gate->isOpen)
            state.nextScene = 2;
    }
    else if (state.player->collidedRightEnt != NULL) {
        if (state.player->collidedRightEnt->type == BADDY
            && state.player->collidedRightEnt->isActive
            && !state.player->isInvincible) {
            *lives -= 1;
            if (*lives == 0) {
                state.player->isDead = true;
                state.player->isInvincible = false;
                Mix_PlayMusic(dead, 1);
            }
            else {
                state.player->isInvincible = true;
                Mix_PlayChannel(-1, hurt, 0);
            }
        }
        else if (state.player->collidedRightEnt->type == JEWEL
            && state.player->collidedRightEnt->isActive) {
            state.player->collidedRightEnt->isActive = false;
            state.player->position = glm::vec3(23.5f, -24.0f, 0.0f);
            *lives += 3;
            Mix_PlayChannel(-1, warp, 0);
            if (!state.jewels[0].isActive && !state.jewels[1].isActive
                && !state.jewels[2].isActive)
                state.gate->isOpen = true;
        }
        else if (state.player->collidedRightEnt->type == GATE
            && state.gate->isOpen)
            state.nextScene = 2;
    }
}

void Level1::Render(ShaderProgram* program, int lives) {
    glClear(GL_COLOR_BUFFER_BIT);

    state.map->Render(program);

    state.gate->Render(program);

    for (int i = 0; i < LEVEL1_JEWELCOUNT; i++)
        state.jewels[i].Render(program);

    for (int i = 0; i < LEVEL1_BADDYCOUNT; i++)
        state.baddies[i].Render(program);

    state.player->Render(program);

    //Now render UI
    program->SetViewMatrix(uiViewMatrix);
    program->SetProjectionMatrix(uiProjectionMatrix);

    for (int i = 0; i < (lives / 6) + 1; i++) {
        for (int j = 0; j < std::min(lives - (i * 6), 6); j++)
            Util::DrawIcon(program, livesTextureID, glm::vec3(-4.6 + (j * 0.6), 3.4 - (i * 0.5), 0), 0.5, 0.5);
    }

    if (!state.jewels[0].isActive)
        Util::DrawIcon(program, fJewelIconTextureID, glm::vec3(2.4, 3.2, 0), 1.0, 1.0);
    if (!state.jewels[1].isActive)
        Util::DrawIcon(program, mJewelIconTextureID, glm::vec3(3.4, 3.2, 0), 1.0, 1.0);
    if (!state.jewels[2].isActive)
        Util::DrawIcon(program, rJewelIconTextureID, glm::vec3(4.4, 3.2, 0), 1.0, 1.0);

    if (state.player->isDead)
        Util::DrawText(program, fontTextureID, "YOU DIED", 0.5f, 0.0f, glm::vec3(-2.0f, 1.0f, 0.0f));
    if (state.player->isWin)
        Util::DrawText(program, fontTextureID, "YOU WIN!", 0.5f, 0.0f, glm::vec3(-2.0f, 1.0f, 0.0f));
}

void Level1::RenderBackground(ShaderProgram* program) {
    program->SetViewMatrix(uiViewMatrix);
    program->SetProjectionMatrix(uiProjectionMatrix);

    Util::DrawIcon(program, backgroundTextureID, glm::vec3(0), 10.0, 7.5);
}