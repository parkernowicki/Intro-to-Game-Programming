#include "Entity.h"

Entity::Entity() {

    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0.0f;

    modelMatrix = glm::mat4(1.0f);
}

bool Entity::isCollideBoxtoBox(Entity* other) {
    if (!isActive || !other->isActive) return false;

    float xdist = fabs(other->position.x - position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(other->position.y - position.y) - ((height + other->height) / 2.0f);

    return xdist < 0 && ydist < 0;
}

void Entity::handleCollisionsX(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        if (isCollideBoxtoBox(&objects[i])) {
            float xdist = fabs(position.x - objects[i].position.x);
            float penetrationX = fabs(xdist - ((width + objects[i].width) / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
            }

            lastCollided = &objects[i];
        }
    }
}

void Entity::handleCollisionsY(Entity* objects, int objectcount) {
    for (int i = 0; i < objectcount; i++) {
        if (isCollideBoxtoBox(&objects[i])) {
            float ydist = fabs(position.y - objects[i].position.y);
            float penetrationY = fabs(ydist - ((height + objects[i].height) / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
            }

            lastCollided = &objects[i];
        }
    }
}

void Entity::Update(float timestep, Entity* walls, int wallcount, Entity* platform, int platformcount) {
    if (!isActive || isDead || isWin) return;

    if (animIndices != NULL) {
        animTime += timestep;
        if (animTime >= 0.25f) {
            animTime = 0;
            animIndex = (animIndex + 1) % animFrames;
        }
    }

    velocity += (acceleration + (movement * speed)) * timestep;

    position.x += velocity.x * timestep;
    handleCollisionsX(walls, wallcount);
    handleCollisionsX(platform, platformcount);

    position.y += velocity.y * timestep;
    handleCollisionsY(walls, wallcount);
    handleCollisionsY(platform, platformcount);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, int index) {
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
    u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {
    if (!isActive) return;

	program->SetModelMatrix(modelMatrix);

    if (isDead) {
        DrawSpriteFromTextureAtlas(program, 1);
        return;
    }
    if (isWin) {
        DrawSpriteFromTextureAtlas(program, 3);
        return;
    }

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, animIndices[animIndex]);
        return;
    }

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glBindTexture(GL_TEXTURE_2D, textureID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}