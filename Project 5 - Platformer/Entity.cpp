#include "Entity.h"

Entity::Entity() {
    position = glm::vec3(0);
    movement = glm::vec3(0);
    gravity = glm::vec3(0.0f, -32.0f, 0.0f);
    velocity = glm::vec3(0);
}

void Entity::AI(Entity* player) {
	switch (ai) {
	case PACER:
		AIPacer();
		break;
    case SINER:
        AISiner();
        break;
	case HOPPER:
		AIHopper(player);
		break;
    case BOSS_PACER:
        AIBossPacer(player);
        break;
	}
}

void Entity::AIPacer() {
	if (collidedLeftMap) {
		movement.x = 1;
	}
    else if (collidedRightMap) {
        movement.x = -1;
    }
}

void Entity::AISiner() {
    position.y += sin(fmod(timeActive * 2.0, 2.0 * M_PI)) / 30.0;
}

void Entity::AIHopper(Entity* player) {
    if (collidedBottomMap)
        movement = glm::vec3(0);

    switch (state) {
    case WAIT:
        if (glm::distance(position, player->position) <= 3.0f)
            state = CHASE;
        break;
    case CHASE:
        if (glm::distance(position, player->position) > 3.0f)
            state = WAIT;
        else {
            if (player->position.x < position.x)
                movement.x = -1;
            else if(player->position.x > position.x)
                movement.x = 1;
            if (collidedBottomMap)
                isJumping = true;
        }
        break;
    }
}

void Entity::AIBossPacer(Entity* player) {
    if (isInvincible)
        walkSpeed = 240.0f;
    else {
        walkSpeed = 80.0f;
        if (fmod(timeActive, 1.0) > 0.75) {
            if (player->position.x < position.x)
                movement.x = -1;
            else if (player->position.x > position.x)
                movement.x = 1;
        }
    }
    if (collidedLeftMap)
        movement.x = 1;
    else if (collidedRightMap)
        movement.x = -1;
}

bool Entity::isCollideBoxtoBox(Entity* other) {
    if (!other->isActive) return false;

    float xdist = fabs(other->position.x - position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(other->position.y - position.y) - ((height + other->height) / 2.0f);

    return xdist < 0 && ydist < 0;
}

void Entity::handleCollisionsY(Entity* objects, int objectcount, bool correct) {
    for (int i = 0; i < objectcount; i++) {
        if (isCollideBoxtoBox(&objects[i])) {
            float ydist = fabs(position.y - objects[i].position.y);
            float penetrationY = fabs(ydist - ((height + objects[i].height) / 2.0f));
            if (velocity.y > objects[i].velocity.y) {
                if (correct) {
                    position.y -= penetrationY;
                    velocity.y = 0;
                }
                collidedTopEnt = &objects[i];
                objects[i].collidedBottomEnt = this;
            }
            else if (velocity.y < objects[i].velocity.y) {
                if (correct) {
                    position.y += penetrationY;
                    velocity.y = 0;
                }
                collidedBottomEnt = &objects[i];
                objects[i].collidedTopEnt = this;
            }
        }
    }
}

void Entity::handleCollisionsX(Entity* objects, int objectCount, bool correct) {
    for (int i = 0; i < objectCount; i++) {
        if (isCollideBoxtoBox(&objects[i])) {
            float xdist = fabs(position.x - objects[i].position.x);
            float penetrationX = fabs(xdist - ((width + objects[i].width) / 2.0f));
            if (velocity.x > objects[i].velocity.x) {
                if (correct) {
                    position.x -= penetrationX;
                    velocity.x = 0;
                }
                collidedRightEnt = &objects[i];
                objects[i].collidedLeftEnt = this;
            }
            else if (velocity.x < objects[i].velocity.x) {
                if (correct) {
                    position.x += penetrationX;
                    velocity.x = 0;
                }
                collidedLeftEnt = &objects[i];
                objects[i].collidedRightEnt = this;
            }
        }
    }
}

void Entity::handleMapCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTopMap = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTopMap = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTopMap = true;
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottomMap = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottomMap = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottomMap = true;
    }

    if (ai == PACER || ai == BOSS_PACER) {
        if (!map->IsSolid(bottom_left, &penetration_x, &penetration_y))
            movement.x = 1;
        else if (!map->IsSolid(bottom_right, &penetration_x, &penetration_y))
            movement.x = -1;
    }
}

void Entity::handleMapCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeftMap = true;
    }
    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRightMap = true;
    }
}

void Entity::Update(float timestep,
	Entity* player,
	Map* map,
	Entity* baddies, int baddycount) {
    if (!isActive || (type == BADDY && glm::distance(position, player->position) > 12.0f)) return;

    timeActive += timestep;

    if (animIndices != NULL) {
        if (type == PLAYER && collidedBottomMap) {
            if (animIndices == animJumpRight)
                animIndices = animRight;
            if (animIndices == animJumpLeft)
                animIndices = animLeft;
        }
        else if (type == PLAYER && !collidedBottomMap) {
            if (animIndices == animRight)
                animIndices = animJumpRight;
            if (animIndices == animLeft)
                animIndices = animJumpLeft;
        }
        if (glm::length(movement)) {
            animTime += timestep + abs(velocity.x * 0.005);
            if (animTime >= 0.25f) {
                animTime = 0;
                animIndex = (animIndex + 1) % animFrames;
            }
        }
        else
            animIndex = 0;
    }

    if (type == BADDY)
        AI(player);

    collidedTopMap = false;
    collidedBottomMap = false;
    collidedLeftMap = false;
    collidedRightMap = false;

    collidedTopEnt = NULL;
    collidedBottomEnt = NULL;
    collidedLeftEnt = NULL;
    collidedRightEnt = NULL;

    if (isJumping) {
        isJumping = false;
        inJumpState = true;
    }
    //We can hold jump to gain more height!
    if (inJumpState) {
        velocity.y = jumpSpeed;
        jumpTimer += timestep;
        if (jumpTimer >= jumpTimeMax)
            inJumpState = false;    
    }
    else
        jumpTimer = 0.0f;

    if (isInvincible) {
        iTimer += timestep;
        if (iTimer >= 3.0f) {
            isInvincible = false;
            iTimer = 0.0f;
        }
    }

    //Decelerate if stopped
    if (movement == glm::vec3(0)) {
        if (velocity.x < 0)
            velocity.x = fmin(velocity.x + (acceleration * timestep), 0);
        else if (velocity.x > 0)
            velocity.x = fmax(velocity.x - (acceleration * timestep), 0);
    }
    velocity += (gravity + (movement * acceleration)) * timestep;
    //Clamp movement speed
    if (!isRunning) {
        if (velocity.x >= walkSpeed * timestep) velocity.x = walkSpeed * timestep;
        if (velocity.x <= -walkSpeed * timestep) velocity.x = -walkSpeed * timestep;
    }
    else {
        if (velocity.x >= runSpeed * timestep) velocity.x = runSpeed * timestep;
        if (velocity.x <= -runSpeed * timestep) velocity.x = -runSpeed * timestep;
    }

    isRunning = false;

    position.y += velocity.y * timestep;
    if (ai != SINER)
        handleMapCollisionsY(map);
    if (type != PLAYER)
        handleCollisionsY(player, 1, false);
    handleCollisionsY(baddies, baddycount, false);

    position.x += velocity.x * timestep;
    if (ai != SINER)
        handleMapCollisionsX(map);
    if (type != PLAYER)
        handleCollisionsX(player, 1, false);
    handleCollisionsX(baddies, baddycount, false);

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

    float vertices[] = { -0.5 * draw_width, -0.5 * draw_height, 0.5 * draw_width, -0.5 * draw_height, 0.5 * draw_width, 0.5 * draw_height,
                        -0.5 * draw_width, -0.5 * draw_height, 0.5 * draw_width, 0.5 * draw_height, -0.5 * draw_width, 0.5 * draw_height };

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
    if (!isActive || (isInvincible && fmod(timeActive, 0.1) > 0.05)) return;

	program->SetModelMatrix(modelMatrix);

    if (isDead) {
        DrawSpriteFromTextureAtlas(program, 6);
        return;
    }

    if (ai == SINER) {
        if (sin(fmod(timeActive * 2.0, 2.0 * M_PI)) > 0.33)
            DrawSpriteFromTextureAtlas(program, 2);
        else if (sin(fmod(timeActive * 2.0, 2.0 * M_PI)) < -0.33)
            DrawSpriteFromTextureAtlas(program, 1);
        else
            DrawSpriteFromTextureAtlas(program, 0);
        return;
    }

    if (ai == HOPPER) {
        if (state == WAIT)
            DrawSpriteFromTextureAtlas(program, 0);
        else
            DrawSpriteFromTextureAtlas(program, 1);
        return;
    }

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5 * draw_width, -0.5 * draw_height, 0.5 * draw_width, -0.5 * draw_height, 0.5 * draw_width, 0.5 * draw_height,
                        -0.5 * draw_width, -0.5 * draw_height, 0.5 * draw_width, 0.5 * draw_height, -0.5 * draw_width, 0.5 * draw_height };
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