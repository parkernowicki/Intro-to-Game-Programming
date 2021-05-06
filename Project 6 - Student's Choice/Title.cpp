#include "Title.h"

void Title::Initialize() {
    state.nextScene = -1;

    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    backgroundTextureID = Util::LoadTexture("Textures/title_background.png");

    state.player = new Entity;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    fontTextureID = Util::LoadTexture("Textures/pixel_font.png");

    leveltheme = Mix_LoadMUS("Sounds/title.mp3");

    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    Mix_PlayMusic(leveltheme, -1);
}

void Title::Update(float timestep, int* lives) {

}

void Title::Render(ShaderProgram* program, int lives) {
    program->SetViewMatrix(uiViewMatrix);
    program->SetProjectionMatrix(uiProjectionMatrix);

    Util::DrawText(program, fontTextureID, "SKELLYQUEST 2", 0.5f, 0.0f, glm::vec3(-3.0f, 1.0f, 0.0f));
    Util::DrawText(program, fontTextureID, "PREPARE TO DIE EDITION", 0.25f, 0.0f, glm::vec3(-2.65f, 0.3f, 0.0f));
    Util::DrawText(program, fontTextureID, "Press ENTER to begin", 0.25f, 0.0f, glm::vec3(-2.4f, -1.5f, 0.0f));
}

void Title::RenderBackground(ShaderProgram* program) {
    program->SetViewMatrix(uiViewMatrix);
    program->SetProjectionMatrix(uiProjectionMatrix);

    Util::DrawIcon(program, backgroundTextureID, glm::vec3(0), 10.0, 7.5);
}