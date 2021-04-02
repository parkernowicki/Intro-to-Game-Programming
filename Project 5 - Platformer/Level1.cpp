#include "Level1.h"

#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8

#define LEVEL1_BADDYCOUNT 0

unsigned int level1_data[] =
{
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
	3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
	3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level1::Initialize() {
	GLuint mapTextureID = Util::LoadTexture("tile1.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

    //Init player
    state.player = new Entity;
    state.player->type = PLAYER;
    state.player->position = glm::vec3(5.0f, -4.0f, 0.0f);
    state.player->acceleration = 16.0f;
    state.player->walkSpeed = 150.0f;
    state.player->runSpeed = 300.0f;
    state.player->jumpSpeed = 8.2f;
    state.player->textureID = Util::LoadTexture("Textures/ufo.png");

    state.player->animDefault = new int[4]{ 0, 2, 4, 6 };
    state.player->animIndices = state.player->animDefault;
    state.player->animFrames = 4;
    state.player->animRows = 4;
    state.player->animCols = 2;

    //Init baddies
    baddiesleft = LEVEL1_BADDYCOUNT;
    state.baddies = new Entity[LEVEL1_BADDYCOUNT];
    GLuint roscoeTextureID = Util::LoadTexture("Textures/roscoe.png");
    GLuint rupertTextureID = Util::LoadTexture("Textures/rupert.png");
    GLuint ozzpreyTextureID = Util::LoadTexture("Textures/ozzprey.png");

    /*state.baddies[0].type = BADDY;
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
    state.baddies[1].position = glm::vec3(-3.5f, 2.75f, 0.0f);
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
    state.baddies[2].animCols = 1;*/
}

void Level1::Update(float timestep) {
    state.player->Update(timestep,
        NULL,
        state.map,
        state.baddies, LEVEL1_BADDYCOUNT);
    for (int i = 0; i < LEVEL1_BADDYCOUNT; i++) {
        state.baddies[i].Update(timestep,
            state.player,
            state.map,
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
        if (state.player->collidedTop->type == BADDY
            && state.player->collidedTop->isActive) {
            state.player->isDead = true;
            Mix_PlayChannel(-1, die, 0);
            Mix_HaltMusic();
        }
    }
    else if (state.player->collidedLeft != NULL) {
        if (state.player->collidedLeft->type == BADDY
            && state.player->collidedLeft->isActive) {
            state.player->isDead = true;
            Mix_PlayChannel(-1, die, 0);
            Mix_HaltMusic();
        }
    }
    else if (state.player->collidedRight != NULL) {
        if (state.player->collidedRight->type == BADDY
            && state.player->collidedRight->isActive) {
            state.player->isDead = true;
            Mix_PlayChannel(-1, die, 0);
            Mix_HaltMusic();
        }
    }
}

void Level1::Render(ShaderProgram* program) {

    state.map->Render(program);

    for (int i = 0; i < LEVEL1_BADDYCOUNT; i++)
        state.baddies[i].Render(program);

    state.player->Render(program);

    if (state.player->isDead)
        Util::DrawText(program, fontTextureID, "YOU DIED", 0.25f, 0.0f, glm::vec3(-4.0f, 3.0f, 0.0f));
    if (state.player->isWin)
        Util::DrawText(program, fontTextureID, "TARGETS DESTROYED", 0.25f, 0.0f, glm::vec3(-4.0f, 3.0f, 0.0f));
}