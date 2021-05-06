#pragma once
#include "Scene.h"

class Title : public Scene {
public:
	void Initialize() override;
	void Update(float deltaTime, int* lives) override;
	void Render(ShaderProgram* program, int lives) override;
	void RenderBackground(ShaderProgram* program) override;
};
