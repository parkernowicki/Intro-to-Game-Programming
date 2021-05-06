#include "Level2.h"

#define LEVEL2_WIDTH 10
#define LEVEL2_HEIGHT 8

#define LEVEL2_BADDYCOUNT 3

unsigned int level2_data[] =
{
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 1, 1, 1, 1, 1, 1, 2, 2,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 2, 1, 1, 1, 1, 1, 1, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

void Level2::Initialize() {
    state.nextScene = -1;

    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    GLuint mapTextureID = Util::LoadTexture("Textures/tile2.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 3, 1);

    //Init player
    state.player = new Entity;
    state.player->type = PLAYER;
    state.player->position = glm::vec3(4.5f, -4.0f, 0.0f);
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
    state.player->animAttackDown = new int[1]{ 17 };
    state.player->animAttackUp = new int[1]{ 23 };
    state.player->animIndices = state.player->animDown;
    state.player->animFrames = 2;
    state.player->animRows = 4;
    state.player->animCols = 6;

    //Init baddies
    baddiesleft = LEVEL2_BADDYCOUNT;
    state.baddies = new Entity[LEVEL2_BADDYCOUNT];
    GLuint bosscoeTextureID = Util::LoadTexture("Textures/bosscoeV2.png");
    GLuint ozzpreyTextureID = Util::LoadTexture("Textures/ozzpreysoul.png");

    //The boss!
	state.baddies[0].type = BADDY;
	state.baddies[0].ai = BOSS_PACER;
	state.baddies[0].acceleration = 64.0f;
	state.baddies[0].moveSpeed = 9.0f;
    state.baddies[0].width = 1.6f;
    state.baddies[0].height = 1.8f;
    state.baddies[0].draw_width = 2.0f;
    state.baddies[0].draw_height = 2.0f;
	state.baddies[0].health = 32;
    state.baddies[0].isGlobal = true;
    state.baddies[0].bossEntrance = true;
	state.baddies[0].textureID = bosscoeTextureID;

	state.baddies[0].animDefault = new int[2]{ 0, 2 };
	state.baddies[0].animIndices = state.baddies[0].animDefault;
	state.baddies[0].animFrames = 2;
	state.baddies[0].animRows = 2;
	state.baddies[0].animCols = 2;
    state.baddies[0].position = glm::vec3(4.5f, 1.0f, 0.0f);

    //The Ozzpreys
    for (int i = 1; i < 3; i++) {
        state.baddies[i].type = BADDY;
        state.baddies[i].ai = SINER;
        state.baddies[i].movement.x = -1;
        state.baddies[i].acceleration = 16.0f;
        state.baddies[i].height = 0.6f;
        state.baddies[i].health = -1;
        state.baddies[i].isGlobal = true;
        state.baddies[i].textureID = ozzpreyTextureID;

        state.baddies[i].animRows = 3;
        state.baddies[i].animCols = 1;
    }
    state.baddies[1].position = glm::vec3(14.0f, -4.0f, 0.0f);
    state.baddies[1].moveSpeed = 200.0f;
    state.baddies[2].position = glm::vec3(16.0f, -6.0f, 0.0f);
    state.baddies[2].moveSpeed = 120.0f;

    glClearColor(0.089f, 0.163f, 0.176f, 1.0f);

    fontTextureID = Util::LoadTexture("Textures/pixel_font.png");
    livesTextureID = Util::LoadTexture("Textures/lives.png");
    fJewelIconTextureID = Util::LoadTexture("Textures/forest_jewel_icon.png");
    mJewelIconTextureID = Util::LoadTexture("Textures/mountain_jewel_icon.png");
    rJewelIconTextureID = Util::LoadTexture("Textures/ruins_jewel_icon.png");

    leveltheme = Mix_LoadMUS("Sounds/final.mp3");
    victory = Mix_LoadMUS("Sounds/victory.mp3");
    dead = Mix_LoadMUS("Sounds/dead.mp3");
    hurt = Mix_LoadWAV("Sounds/doh.wav");
    schwing = Mix_LoadWAV("Sounds/schwing.wav");

    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_VolumeChunk(hurt, MIX_MAX_VOLUME / 2);
    Mix_PlayMusic(leveltheme, -1);
}

void Level2::Update(float timestep, int* lives) {
    if (!state.baddies[0].isActive) {
        state.nextScene = 3;
        return;
    }
    state.player->Update(timestep,
        NULL,
        state.map,
        state.baddies, LEVEL2_BADDYCOUNT,
        state.jewels, 0,
        state.gate);
    for (int i = 0; i < LEVEL2_BADDYCOUNT; i++) {
        if (state.baddies[i].ai != SINER || state.player->timeActive >= 17.0f)
            state.baddies[i].Update(timestep,
                state.player,
                state.map,
                NULL, 0,
                NULL, 0,
                state.gate);
    }
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
}

void Level2::Render(ShaderProgram* program, int lives) {
    glClear(GL_COLOR_BUFFER_BIT);

    state.map->Render(program);

    for (int i = 0; i < LEVEL2_BADDYCOUNT; i++)
        state.baddies[i].Render(program);

    state.player->Render(program);

    //Now render UI
    program->SetViewMatrix(uiViewMatrix);
    program->SetProjectionMatrix(uiProjectionMatrix);

    for (int i = 0; i < (lives / 6) + 1; i++) {
        for (int j = 0; j < std::min(lives - (i * 6), 6); j++)
            Util::DrawIcon(program, livesTextureID, glm::vec3(-4.6 + (j * 0.6), 3.4 - (i * 0.5), 0), 0.5, 0.5);
    }

	Util::DrawIcon(program, fJewelIconTextureID, glm::vec3(2.4, 3.2, 0), 1.0, 1.0);
	Util::DrawIcon(program, mJewelIconTextureID, glm::vec3(3.4, 3.2, 0), 1.0, 1.0);
	Util::DrawIcon(program, rJewelIconTextureID, glm::vec3(4.4, 3.2, 0), 1.0, 1.0);

    if (state.player->isDead)
        Util::DrawText(program, fontTextureID, "YOU DIED", 0.5f, 0.0f, glm::vec3(-2.0f, 1.0f, 0.0f));
    if (state.player->isWin)
        Util::DrawText(program, fontTextureID, "YOU WIN!", 0.5f, 0.0f, glm::vec3(-2.0f, 1.0f, 0.0f));
}

void Level2::RenderBackground(ShaderProgram* program) {
    program->SetViewMatrix(uiViewMatrix);
    program->SetProjectionMatrix(uiProjectionMatrix);

    Util::DrawIcon(program, backgroundTextureID, glm::vec3(0), 10.0, 7.5);
}