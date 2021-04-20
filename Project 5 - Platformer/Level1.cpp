#include "Level1.h"

#define LEVEL1_WIDTH 64
#define LEVEL1_HEIGHT 8

#define LEVEL1_BADDYCOUNT 10

unsigned int level1_data[] =
{
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 3, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 3, 2, 2, 2, 0, 0, 2, 2, 3, 2, 0, 0, 2, 2, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 3, 2, 2, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	3, 2, 2, 3, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 1, 2, 2, 2, 3, 0, 0, 2, 2, 2, 2, 3, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 3, 2, 3, 3, 3, 3
};

void Level1::Initialize() {
    state.nextScene = -1;

    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	GLuint mapTextureID = Util::LoadTexture("Textures/tile1.png");
    backgroundTextureID = Util::LoadTexture("Textures/level1_background.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

    //Init player
    state.player = new Entity;
    state.player->type = PLAYER;
    state.player->position = glm::vec3(3.0f, -5.0f, 0.0f);
    state.player->acceleration = 16.0f;
    state.player->walkSpeed = 150.0f;
    state.player->runSpeed = 300.0f;
    state.player->jumpSpeed = 8.8f;
    state.player->jumpTimeMax = 0.25f;
    state.player->width = 0.6f;
    state.player->textureID = Util::LoadTexture("Textures/skelly.png");

    state.player->animRight = new int[2]{ 0, 7 };
    state.player->animLeft = new int[2]{ 1, 8 };
    state.player->animJumpRight = new int[2]{ 4, 11 };
    state.player->animJumpLeft = new int[2]{ 5, 12 };
    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 2;
    state.player->animRows = 2;
    state.player->animCols = 7;

    //Init baddies
    baddiesleft = LEVEL1_BADDYCOUNT;
    state.baddies = new Entity[LEVEL1_BADDYCOUNT];
    GLuint roscoeTextureID = Util::LoadTexture("Textures/roscoe.png");
    GLuint rupertTextureID = Util::LoadTexture("Textures/rupert.png");
    GLuint ozzpreyTextureID = Util::LoadTexture("Textures/ozzprey.png");

    for (int i = 0; i < 7; i++) {
        state.baddies[i].type = BADDY;
        state.baddies[i].ai = PACER;
        state.baddies[i].movement.x = -1;
        state.baddies[i].acceleration = 16.0f;
        state.baddies[i].walkSpeed = 80.0f;
        state.baddies[i].textureID = roscoeTextureID;

        state.baddies[i].animDefault = new int[2]{ 0, 1 };
        state.baddies[i].animIndices = state.baddies[0].animDefault;
        state.baddies[i].animFrames = 2;
        state.baddies[i].animRows = 2;
        state.baddies[i].animCols = 1;
    }
    state.baddies[0].position = glm::vec3(10.0f, -4.0f, 0.0f);
    state.baddies[1].position = glm::vec3(23.0f, -4.0f, 0.0f);
    state.baddies[2].position = glm::vec3(32.0f, -4.0f, 0.0f);
    state.baddies[3].position = glm::vec3(33.0f, -4.0f, 0.0f);
    state.baddies[4].position = glm::vec3(34.0f, -4.0f, 0.0f);
    state.baddies[5].position = glm::vec3(54.0f, -1.0f, 0.0f);
    state.baddies[6].position = glm::vec3(56.0f, -1.0f, 0.0f);

    for (int i = 7; i < 10; i++) {
        state.baddies[i].type = BADDY;
        state.baddies[i].ai = SINER;
        state.baddies[i].movement.x = -1;
        state.baddies[i].acceleration = 16.0f;
        state.baddies[i].walkSpeed = 120.0f;
        state.baddies[i].height = 0.6f;
        state.baddies[i].gravity = glm::vec3(0);
        state.baddies[i].textureID = ozzpreyTextureID;

        state.baddies[i].animRows = 3;
        state.baddies[i].animCols = 1;
    }
    state.baddies[7].position = glm::vec3(20.0f, -2.0f, 0.0f);
    state.baddies[8].position = glm::vec3(47.0f, -3.0f, 0.0f);
    state.baddies[9].position = glm::vec3(59.0f, -5.0f, 0.0f);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    fontTextureID = Util::LoadTexture("Textures/pixel_font.png");
    livesTextureID = Util::LoadTexture("Textures/lives.png");

    leveltheme = Mix_LoadMUS("Sounds/level1.mp3");
    victory = Mix_LoadMUS("Sounds/victory.mp3");
    dead = Mix_LoadMUS("Sounds/dead.mp3");
    hurt = Mix_LoadWAV("Sounds/doh.wav");
    stomp = Mix_LoadWAV("Sounds/stomp.wav");
    bosshit = Mix_LoadWAV("Sounds/bosshit.wav");

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_PlayMusic(leveltheme, -1);
}

void Level1::Update(float timestep, int* lives) {
    state.player->Update(timestep,
        NULL,
        state.map,
        state.baddies, LEVEL1_BADDYCOUNT);
    if (state.player->position.x >= 60.0f) {
        state.nextScene = 2;
        return;
    }
    for (int i = 0; i < LEVEL1_BADDYCOUNT; i++) {
        state.baddies[i].Update(timestep,
            state.player,
            state.map,
            NULL, 0);
    }
    if (state.player->collidedBottomEnt != NULL) {
        if (state.player->collidedBottomEnt->type == BADDY
            && !state.player->collidedBottomEnt->isInvincible) {
            //We killed it
            if (--state.player->collidedBottomEnt->health == 0) {
                state.player->collidedBottomEnt->isActive = false;
                --baddiesleft;
            }
            state.player->isJumping = true;
            Mix_PlayChannel(-1, stomp, 0);
        }
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
    }
    if (state.player->position.y < -8.0f) {
        *lives = 0;
        state.player->isDead = true;
        Mix_PlayMusic(dead, 1);
    }
}

void Level1::Render(ShaderProgram* program, int lives) {
    state.map->Render(program);

    for (int i = 0; i < LEVEL1_BADDYCOUNT; i++)
        state.baddies[i].Render(program);

    state.player->Render(program);

    //Now render UI
    program->SetViewMatrix(uiViewMatrix);
    program->SetProjectionMatrix(uiProjectionMatrix);

    for (int i = 0; i < lives; i++)
        Util::DrawIcon(program, livesTextureID, glm::vec3(-4.5 + (i * 0.6), 3.2, 0), 0.5, 0.5);

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