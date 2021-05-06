#include "End.h"

void End::Initialize() {
    state.nextScene = -1;

    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    backgroundTextureID = Util::LoadTexture("Textures/ending_background.png");

    state.player = new Entity;
    state.player->position = glm::vec3(0);

    state.jewels = new Entity;
    state.jewels[0].type = JEWEL;
    state.jewels[0].position = glm::vec3(0.0f, -1.0f, 0.0f);
    state.jewels[0].draw_width = 2.0f;
    state.jewels[0].draw_height = 2.0f;
    state.jewels[0].textureID = Util::LoadTexture("Textures/skelly.png");

    state.jewels[0].animDown = new int[2]{ 2, 8 };
    state.jewels[0].animIndices = state.jewels[0].animDown;
    state.jewels[0].animFrames = 2;
    state.jewels[0].animRows = 4;
    state.jewels[0].animCols = 6;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    fontTextureID = Util::LoadTexture("Textures/pixel_font.png");

    leveltheme = Mix_LoadMUS("Sounds/victory.mp3");
    bossdead = Mix_LoadWAV("Sounds/bossdead.wav");

    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    Mix_VolumeChunk(bossdead, MIX_MAX_VOLUME / 2);
    Mix_PlayMusic(leveltheme, 1);
    Mix_PlayChannel(-1, bossdead, 0);
}

void End::Update(float timestep, int* lives) {
    state.jewels->Update(timestep,
        state.player,
        NULL,
        NULL, 0,
        NULL, 0,
        NULL);
}

void End::Render(ShaderProgram* program, int lives) {
    program->SetViewMatrix(uiViewMatrix);
    program->SetProjectionMatrix(uiProjectionMatrix);

    state.jewels->Render(program);

    Util::DrawText(program, fontTextureID, "QUEST COMPLETE", 0.5f, 0.0f, glm::vec3(-3.2f, 1.0f, 0.0f));
    Util::DrawText(program, fontTextureID, "Thanks for playing!", 0.25f, 0.0f, glm::vec3(-2.3f, 0.3f, 0.0f));
}

void End::RenderBackground(ShaderProgram* program) {
    program->SetViewMatrix(uiViewMatrix);
    program->SetProjectionMatrix(uiProjectionMatrix);

    Util::DrawIcon(program, backgroundTextureID, glm::vec3(0), 10.0, 7.5);
}