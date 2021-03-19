#include "Entity.h"

Entity::Entity() {

    position = glm::vec3(0);
    movement = glm::vec3(0);
    gravity = glm::vec3(0.0f, -16.0f, 0.0f);
    velocity = glm::vec3(0);

    modelMatrix = glm::mat4(1.0f);
}

void Entity::AI(Entity* player) {
	switch (ai) {
	case WALKER:
		AIWalker();
		break;
	case WAITANDGO:
		AIWaitandGo(player);
		break;
	}
}

void Entity::AIWalker() {
    if (entColliding != NULL) {
        if (entColliding->type == PLATFORM
            && (collidedLeft || collidedRight))
            movement.x *= -1;
    }
}

void Entity::AIWaitandGo(Entity* player) {
    switch (state) {
    case PATROL:
        break;
    }
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
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }

            entColliding = &objects[i];
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
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }

            entColliding = &objects[i];
        }
    }
}

void Entity::Update(float timestep,
	Entity* player,
	Entity* platforms, int platformcount,
	Entity* baddies, int baddycount) {
    if (!isActive || isDead || isWin) return;

    if (animIndices != NULL) {
        animTime += timestep;
        if (animTime >= 0.25f) {
            animTime = 0;
            animIndex = (animIndex + 1) % animFrames;
        }
    }

    if (type == BADDY) {
        AI(player);
    }

    entColliding = NULL;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    if (isJumping) {
        isJumping = false;
        velocity.y = 8.0f;
    }

    if (movement == glm::vec3(0)) {
        if (velocity.x < 0)
            velocity.x = fmin(velocity.x + (acceleration * timestep), 0);
        else if (velocity.x > 0)
            velocity.x = fmax(velocity.x - (acceleration * timestep), 0);
    }
    velocity += (gravity + (movement * acceleration)) * timestep;
    if (velocity.x >= topSpeed * timestep) velocity.x = topSpeed * timestep;
    if (velocity.x <= -topSpeed * timestep) velocity.x = -topSpeed * timestep;

    position.x += velocity.x * timestep;
    handleCollisionsX(platforms, platformcount);
    handleCollisionsX(baddies, baddycount);

    position.y += velocity.y * timestep;
    handleCollisionsY(platforms, platformcount);
    handleCollisionsY(baddies, baddycount);

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