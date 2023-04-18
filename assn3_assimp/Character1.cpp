#include "Character1.h"
int interval = 100;

Character::Character() {
    frame = 0;
    model[0] = loadObjFile("meshes/character_pose1.obj");
    model[1] = loadObjFile("meshes/character_pose2.obj");
    model[2] = loadObjFile("meshes/character_pose3.obj");
    

    PosX =  -0.7f;
    PosY = 0.0f;
    jumpLimit = 1.0f;
    jumpSpeed = 0.01f;
    jumping = false;
    falling = false;
    actualLimit = jumpLimit;
    width = 0.5f;
    height = 0.9f;

}

void Character::init() {
    for (size_t i = 0; i < 3; i++) {
        sendDataToOpenGL(&model[i], &VAO[i], & VBO[i], &EBO[i]);
    }
}

void Character::drawCircle() {
    float x, y;
    for (double i = 0; i <= 360;) {
        glBegin(GL_TRIANGLES);
        x = 5 * cos(i);
        y = 5 * sin(i);
        glVertex2d(x, y);
        i = i + .5;
        x = 5 * cos(i);
        y = 5 * sin(i);
        glVertex2d(x, y);
        glVertex2d(0, 0);
        glEnd();
        i = i + .5;
    }
}

int nextFrame(int frame) {
    if (frame != 3)
        return frame + 1;
    else
        return 0;
}

int animeCalc(int anime) {
    return anime% interval;
}

void Character::hierarchicalRender(int currentNodeInd, int anime) {
    if (currentNodeInd != -1) {
        Node* currentNode = &frames[frame][currentNodeInd];
        Node* nextFrameNode = &frames[nextFrame(frame)][currentNodeInd];
        int offset = animeCalc(anime);
        Position translationOffset = { (nextFrameNode->translation.x - currentNode->translation.x)/interval * offset,\
            (nextFrameNode->translation.y - currentNode->translation.y) / interval * offset };
        Position translationFinal = { (currentNode->translation.x + translationOffset.x),
            (currentNode->translation.y + translationOffset.y)
        };
        // translationFinal = { (currentNode->translation.x ),  (currentNode->translation.y )   };

        GLfloat rotationOffset = (nextFrameNode->angle - currentNode->angle) / interval * offset;
        GLfloat rotationFinal = currentNode->angle + rotationOffset;
        if (frame == 4) {
            translationFinal = { (currentNode->translation.x),  (currentNode->translation.y) };
            rotationFinal = currentNode->angle;
        }
        glPushMatrix();

        glTranslatef(translationFinal.x / 10, translationFinal.y / 10, 0.0f);
        glRotatef(rotationFinal, 0.0f, 0.0f, 1.0f);

        glPushMatrix();
        glScalef(currentNode->scale.x / 100, currentNode->scale.y / 100, 0.0f);
        glColor3f(currentNode->color.r, currentNode->color.g, currentNode->color.b);
        drawCircle();
        glPopMatrix();
        hierarchicalRender(currentNode->child,  anime);
        glPopMatrix();
        hierarchicalRender(currentNode->sibling, anime);
    }
}

void Character::frameSelection(int* anime) {
    if (0 <= *anime && *anime < interval) { 
        frame = 0;
    }
    else if (1 * interval <= *anime && *anime < 2 * interval) {
        frame = 1;
    }
    else if (2 * interval <= *anime && *anime < 3 * interval) {
        frame = 2;
    }
    else {
        *anime = 0;
    }
    if (falling) {
        frame = 4;
    }
    if (jumping || PosY > baseline) {
        frame = 0;
    }
}

void Character::drawCharacter(int* anime) {
    // hierarchicalRender(0, *anime);
    frameSelection(anime);
    draw(&VAO[frame], &model[frame]);
}

void Character::collisionBox() {
    glColor3f(0.62f, 0.12f, 0.94f);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f * width, height);
    glVertex2f(0.5f * width, height);
    glVertex2f(0.5f * width, 0.0f);
    glVertex2f(-0.5f * width, 0.0f);
    glEnd();
}

void Character::setJumping(GLint flag) {
    if (!jumping && PosY < baseline + 0.05) {
        jumping = true;
        switch (flag) {
        case 0: //higher mushroom
            actualLimit = baseline + jumpLimit * 0.8;
            break;
        case 1: //same mushroom
            actualLimit = baseline + jumpLimit * 0.4;
            break;

        case 2: //no mushroom
            actualLimit = baseline + jumpLimit ;
            break;
        }
    }
}

void Character::updatePosY() {
    if (jumping) {
        if (PosY < actualLimit) {
            PosY += jumpSpeed;
        }
        else {
            jumping = false;
        }
    }
    else {
        if (PosY > baseline) {
            PosY -= jumpSpeed;
        }
        else if (PosY < baseline + 0.05) {
            PosY = 0;
        }
    }

    //if (!falling && PosY < (baselineFront + 0.01f) && PosY > (baselineFront - 0.02f)) {
    //    PosY = baselineFront;
    //} 

    //if (!falling && PosY < (baselineFront - 0.1f) ) {
    //    std::cout << "terrain collision" << std::endl;
    //    *onGame = false;
    //}

    //if (baselineFront == 0.0f && PosY < baselinePrev) {
    //    falling = true;
    //}


    //if (falling) {
    //    PosY -= 2 * jumpSpeed;
    //    if (PosY < -1.5f) {
    //       *onGame = false;
    //    }
    //}
}

GLfloat Character::getPosY() {
    return PosY;
}

GLfloat Character::getPosX() {
    return PosX;
}

GLfloat Character::getWidth() {
    return width;
}

GLfloat Character::getHeight() {
    return height;
}