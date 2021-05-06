#include "Entity.h"

Entity::Entity() {
    position = glm::vec3(0);
    movement = glm::vec3(0);
    velocity = glm::vec3(0);
}

void Entity::AI(Entity* player) {
	switch (ai) {
	case PACER:
		AIPacer();
		break;
    case SINER:
        AISiner(player);
        break;
	case CHASER:
		AIChaser(player);
		break;
    case BOSS_PACER:
        AIBossPacer(player);
        break;
	}
}

void Entity::AIPacer() {
    if (fmod(timeActive, 1.0f) < 0.05f) {
        int dir = rand() % 5;
        switch (dir) {
        case 0:
            movement = glm::vec3(0.0f, 1.0f, 0.0f);
            break;
        case 1:
            movement = glm::vec3(0.0f, -1.0f, 0.0f);
            break;
        case 2:
            movement = glm::vec3(1.0f, 0.0f, 0.0f);
            break;
        case 3:
            movement = glm::vec3(-1.0f, 0.0f, 0.0f);
            break;
        default:
            movement = glm::vec3(0.0f);
        }
    }
}

void Entity::AISiner(Entity* player) {
    position.y += sin(fmod(timeActive * 2.0, 2.0 * M_PI)) / 25.0;
    if (isGlobal) {
        if (position.x <= -2.0f)
            position.x = 12.0f;
    }
    else {
        if (position.x <= player->position.x - 6.0f)
            position.x = player->position.x + 6.0f;
    }
}

void Entity::AIChaser(Entity* player) {
    movement = glm::vec3(0);

    switch (state) {
    case WAIT:
        if (glm::distance(position, player->position) <= 2.5f)
            state = CHASE;
        break;
    case CHASE:
        if (glm::distance(position, player->position) > 4.0f)
            state = WAIT;
        else
            movement = glm::normalize(player->position - position);
        break;
    }
}

void Entity::AIBossPacer(Entity* player) {
    if (bossEntrance) {
        movement = glm::vec3(0.0f, -1.0f, 0.0f);
        if (timeActive >= 17.0f) {
            bossEntrance = false;
            moveSpeed = 100.0f;
        }
    }
    else {
        if (fmod(timeActive, 1.0f) < 0.05f) {
            int dir = rand() % 6;
            switch (dir) {
            case 0:
                movement = glm::vec3(0.0f, 1.0f, 0.0f);
                break;
            case 1:
                movement = glm::vec3(0.0f, -1.0f, 0.0f);
                break;
            case 2:
                movement = glm::vec3(1.0f, 0.0f, 0.0f);
                break;
            case 3:
                movement = glm::vec3(-1.0f, 0.0f, 0.0f);
                break;
            default:
                movement = glm::normalize(player->position - position);
            }
        }
    }
}

bool Entity::isCollideBoxtoBox(Entity* other) {
    if (other == NULL || !other->isActive) return false;

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
    if (map == NULL) return;
    // Probes for tiles
    glm::vec3 top_left = glm::vec3(position.x - (width / 2) + 0.05f, position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2) - 0.05f, position.y + (height / 2), position.z);

    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2) + 0.05f, position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2) - 0.05f, position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTopMap = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTopMap = true;
    }
    if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottomMap = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottomMap = true;
    }
}

void Entity::handleMapCollisionsX(Map* map)
{
    if (map == NULL) return;
    // Probes for tiles
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2) - 0.05f, position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2) + 0.05f, position.z);

    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2) - 0.05f, position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2) + 0.05f, position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeftMap = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeftMap = true;
    }
    if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRightMap = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRightMap = true;
    }
}

void Entity::Update(float timestep,
	Entity* player,
	Map* map,
	Entity* baddies, int baddycount,
    Entity* jewels, int jewelcount,
    Entity* gate) {
    if (!isActive || (type != PLAYER && !isGlobal && glm::distance(position, player->position) > 7.0f)) return;

    timeActive += timestep;

    if (animIndices != NULL) {
        if (type == JEWEL || glm::length(movement)) {
            animTime += timestep;
            if (animTime >= 0.25f) {
                animTime = 0;
                animIndex = (animIndex + 1) % animFrames;
            }
        }
        else
            animIndex = 0;
    }

    // Player can perform melee attacks
    if (isMeleeAttack) {
        meleeTimer += timestep;
        if (meleeTimer >= 0.2f) {
            meleeTimer = 0.0f;
            isMeleeAttack = false;
            if (animIndices == animAttackUp)
                animIndices = animUp;
            if (animIndices == animAttackLeft)
                animIndices = animLeft;
            if (animIndices == animAttackDown)
                animIndices = animDown;
            if (animIndices == animAttackRight)
                animIndices = animRight;
        }
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

    if (isInvincible) {
        iTimer += timestep;
        if (iTimer >= iTimerMax) {
            isInvincible = false;
            iTimer = 0.0f;
        }
    }

    //Decelerate if stopped
    if (movement.x == 0) {
        if (velocity.x < 0)
            velocity.x = fmin(velocity.x + (acceleration * timestep), 0);
        else if (velocity.x > 0)
            velocity.x = fmax(velocity.x - (acceleration * timestep), 0);
    }
    if (movement.y == 0) {
        if (velocity.y < 0)
            velocity.y = fmin(velocity.y + (acceleration * timestep), 0);
        else if (velocity.y > 0)
            velocity.y = fmax(velocity.y - (acceleration * timestep), 0);
    }

    velocity += movement * acceleration * timestep;
    //Clamp movement speed
	if (velocity.x >= moveSpeed * timestep) velocity.x = moveSpeed * timestep;
	if (velocity.x <= -moveSpeed * timestep) velocity.x = -moveSpeed * timestep;
    if (velocity.y >= moveSpeed * timestep) velocity.y = moveSpeed * timestep;
    if (velocity.y <= -moveSpeed * timestep) velocity.y = -moveSpeed * timestep;

    position.y += velocity.y * timestep;
    if (ai != SINER && type != GATE && !bossEntrance) {
        handleMapCollisionsY(map);
        handleCollisionsY(gate, 1, true);
    }
    if (type == BADDY)
        handleCollisionsY(player, 1, false);
    handleCollisionsY(baddies, baddycount, false);
    handleCollisionsY(jewels, jewelcount, false);

    position.x += velocity.x * timestep;
    if (ai != SINER && type != GATE && !bossEntrance) {
        handleMapCollisionsX(map);
        handleCollisionsX(gate, 1, true);
    }
    if (type == BADDY)
        handleCollisionsX(player, 1, false);
    handleCollisionsX(baddies, baddycount, false);
    handleCollisionsX(jewels, jewelcount, false);

    // Did we hit an enemy with our melee attack?
    if (type == BADDY && player->isMeleeAttack && !isInvincible && !bossEntrance) {
        float xdist, ydist;
        if (player->animIndices == player->animAttackUp) {
            xdist = fabs(player->position.x - position.x - 0.2f) - ((width + 0.2f) / 2.0f);
            ydist = fabs(player->position.y - position.y + 0.6f) - ((height + 0.8f) / 2.0f);
        }
        if (player->animIndices == player->animAttackLeft) {
            xdist = fabs(player->position.x - position.x - 0.6f) - ((width + 0.8f) / 2.0f);
            ydist = fabs(player->position.y - position.y) - ((height + 0.2f) / 2.0f);
        }
        if (player->animIndices == player->animAttackDown) {
            xdist = fabs(player->position.x - position.x + 0.2f) - ((width + 0.2f) / 2.0f);
            ydist = fabs(player->position.y - position.y - 0.6f) - ((height + 0.8f) / 2.0f);
        }
        if (player->animIndices == player->animAttackRight) {
            xdist = fabs(player->position.x - position.x + 0.6f) - ((width + 0.8f) / 2.0f);
            ydist = fabs(player->position.y - position.y) - ((height + 0.2f) / 2.0f);
        }

        if (xdist < 0 && ydist < 0) {
            --health;
            if (health == 0)
                isActive = false;
            else if (ai != SINER) {
                isInvincible = true;
                movement = glm::vec3(0);
            }
        }
    }

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
        DrawSpriteFromTextureAtlas(program, 4);
        return;
    }

    if (type == GATE) {
        if (isOpen)
            DrawSpriteFromTextureAtlas(program, 1);
        else
            DrawSpriteFromTextureAtlas(program, 0);
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

    if (ai == CHASER) {
        if (state == WAIT)
            DrawSpriteFromTextureAtlas(program, 0);
        else
            DrawSpriteFromTextureAtlas(program, 1);
        return;
    }

    if (bossEntrance) {
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