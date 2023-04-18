#include "Character1.h"

enum Body
{
    pelvis,
    leftUpperLeg,
    leftLowerLeg,
    leftFoot,
    rightUpperLeg,
    rightLowerLeg,
    rightFoot,
    torso,
    neck,
    head,
    leftUpperArm,
    leftLowerArm,
    leftHand,
    rightUpperArm,
    rightLowerArm,
    rightHand,

};
Color pink = { 0.97f, 0.81f, 0.87f };
Color salmon = { 0.97f, 0.83f, 0.81f };
Color champagne = { 0.97f, 0.91f, 0.81f };
Color lemon = { 0.95f, 0.97f, 0.81f };
Color melon = { 0.87f, 0.97f, 0.81f };
Color hotPink = { 1.0f, 0.0f, 0.45f };

Color red = { 1.0f, 0.0f, 0.0f };
Color green = { 0.0f, 1.0f, 0.0f };
Color blue = { 0.0f, 0.0f, 1.0f };
GLfloat unit = 0.5f;
Position legScale = { 1 * unit, 4 * unit };
int interval = 40;

Character::Character() {
    frame = 0;
    model[0] = loadObjFile("meshes/character_pose1.obj");
    model[1] = loadObjFile("meshes/character_pose2.obj");
    model[2] = loadObjFile("meshes/character_pose3.obj");
    

    PosX =  -0.7f;
    PosY = 0.5f;
    jumpLimit = 1.0f;
    jumpSpeed = 0.01f;
    jumping = false;
    falling = false;
    actualLimit = jumpLimit;
    width = 0.5f;
    height = 0.9f;


    // initialize character hierarchy
    frames[0][pelvis] = { { 0.0f, 9 * unit }, 0.0f, {2 * unit , 1 * unit}, red, -1, -1 }; // pelvis
    frames[0][leftUpperLeg] = { {  1 * unit, -2 * unit }, 45.0f, {1 * unit, 4 * unit}, hotPink,-1, -1 }; // left upper leg
    frames[0][leftLowerLeg] = { { -1 * unit ,  -3 * unit }, -50.0f, {1 * unit, 4 * unit}, hotPink, -1, -1 }; // left lower leg
    frames[0][leftFoot] = { {  1 * unit , -2 * unit }, 0.0f, {2 * unit , 1 * unit}, hotPink, -1, -1 }; // left foot
    frames[0][rightUpperLeg] = { { 0.0f, -2 * unit }, 0.0f, {1 * unit, 4 * unit}, blue, -1, -1 }; // right upper leg
    frames[0][rightLowerLeg] = { { 0.0f, -4 * unit }, 0.0f, {1 * unit, 4 * unit}, blue, -1, -1 }; // right lower leg
    frames[0][rightFoot] = { { 1 * unit, -2 * unit }, 0.0f, {2 * unit , 1 * unit}, blue, -1, -1 }; // right foot
    frames[0][torso] = { { 0.0f, 3 * unit }, 0.0f, {1.0f, 2.5f}, pink, -1, -1 }; // torso
    frames[0][neck] = { { 0.0f, 3 * unit }, 0.0f, {0.4f, 0.5f}, salmon, -1, -1 }; // neck
    frames[0][head] = { { 0.0f, 1 * unit }, 0.0f, {1.0f, 1.0f}, champagne, -1, -1 }; // head
    frames[0][leftUpperArm] = { { -1 * unit, 1 * unit }, -30.0f, { 1 * unit, 1.5f}, pink, -1, -1 }; // left upper arm
    frames[0][leftLowerArm] = { { 0.0f, -3 * unit }, 0.0f, {1 * unit, 1.5f}, pink, -1, -1 }; // left lower arm
    frames[0][leftHand] = { { 0.0f, -1 * unit}, 0.0f, {1 * unit, 0.5f},lemon,  -1, -1 }; // left hand
    frames[0][rightUpperArm] = { { 1 * unit, 1 * unit}, 30.0f, {1 * unit, 1.5f}, blue, -1, -1 }; // right upper arm
    frames[0][rightLowerArm] = { { 0.0f, -3 * unit }, 0.0f, {1 * unit, 1.5f}, blue, -1, -1 }; // right lower arm
    frames[0][rightHand] = { { 0.0f, -1 * unit }, 0.0f, {1 * unit, 0.5f}, lemon, -1, -1 }; // right hand


    frames[1][pelvis] = { { 0.0f, 9 * unit }, 0.0f, {2 * unit , 1 * unit}, red, -1, -1 }; // pelvis
    frames[1][leftUpperLeg] = { {  1 * unit, -2 * unit }, 30.0f, {1 * unit, 4 * unit}, hotPink,-1, -1 }; // left upper leg
    frames[1][leftLowerLeg] = { { 0 * unit ,  -4 * unit }, 0, {1 * unit, 4 * unit}, hotPink, -1, -1 }; // left lower leg
    frames[1][leftFoot] = { {  1 * unit , -2 * unit }, 0.0f, {2 * unit , 1 * unit}, hotPink, -1, -1 }; // left foot
    frames[1][rightUpperLeg] = { { -1 * unit, -2 * unit }, -30.0f, {1 * unit, 4 * unit}, blue, -1, -1 }; // right upper leg
    frames[1][rightLowerLeg] = { { 0.0f, -4 * unit }, 0, {1 * unit, 4 * unit}, blue, -1, -1 }; // right lower leg
    frames[1][rightFoot] = { { 1 * unit, -2 * unit }, 0.0f, {2 * unit , 1 * unit}, blue, -1, -1 }; // right foot
    frames[1][torso] = { { 0.0f, 3 * unit }, 0.0f, {1.0f, 2.5f}, pink, -1, -1 }; // torso
    frames[1][neck] = { { 0.0f, 3 * unit }, 0.0f, {0.4f, 0.5f}, salmon, -1, -1 }; // neck
    frames[1][head] = { { 0.0f, 1 * unit }, 0.0f, {1.0f, 1.0f}, champagne, -1, -1 }; // head
    frames[1][leftUpperArm] = { { -1 * unit, 1 * unit }, -30.0f, { 1 * unit, 1.5f}, pink, -1, -1 }; // left upper arm
    frames[1][leftLowerArm] = { { 0.5f * unit, -3 * unit }, 20.0f, {1 * unit, 1.5f}, pink, -1, -1 }; // left lower arm
    frames[1][leftHand] = { { 0.0f, -1 * unit}, 0.0f, {1 * unit, 0.5f},lemon,  -1, -1 }; // left hand
    frames[1][rightUpperArm] = { { 1 * unit, 1 * unit}, 30.0f, {1 * unit, 1.5f}, blue, -1, -1 }; // right upper arm
    frames[1][rightLowerArm] = { { 0.5f * unit, -3 * unit }, 20.0f, {1 * unit, 1.5f}, blue, -1, -1 }; // right lower arm
    frames[1][rightHand] = { { 0.0f, -1 * unit }, 0.0f, {1 * unit, 0.5f}, lemon, -1, -1 }; // right hand


    frames[2][pelvis] = { { 0.0f, 9 * unit }, 0.0f, {2 * unit , 1 * unit}, red, -1, -1 }; // pelvis
    frames[2][rightUpperLeg] = { {  1 * unit, -2 * unit }, 45.0f, {1 * unit, 4 * unit}, blue,-1, -1 }; // left upper leg
    frames[2][rightLowerLeg] = { { -1 * unit ,  -3 * unit }, -50.0f, {1 * unit, 4 * unit}, blue, -1, -1 }; // left lower leg
    frames[2][rightFoot] = { {  1 * unit , -2 * unit }, 0.0f, {2 * unit , 1 * unit}, blue, -1, -1 }; // left foot
    frames[2][leftUpperLeg] = { { 0.0f, -2 * unit }, 0.0f, {1 * unit, 4 * unit}, hotPink, -1, -1 }; // right upper leg
    frames[2][leftLowerLeg] = { { 0.0f, -4 * unit }, 0.0f, {1 * unit, 4 * unit}, hotPink, -1, -1 }; // right lower leg
    frames[2][leftFoot] = { { 1 * unit, -2 * unit }, 0.0f, {2 * unit , 1 * unit}, hotPink, -1, -1 }; // right foot
    frames[2][torso] = { { 0.0f, 3 * unit }, 0.0f, {1.0f, 2.5f}, pink, -1, -1 }; // torso
    frames[2][neck] = { { 0.0f, 3 * unit }, 0.0f, {0.4f, 0.5f}, salmon, -1, -1 }; // neck
    frames[2][head] = { { 0.0f, 1 * unit }, 0.0f, {1.0f, 1.0f}, champagne, -1, -1 }; // head
    frames[2][rightUpperArm] = { { -1 * unit, 1 * unit }, -30.0f, { 1 * unit, 1.5f}, blue, -1, -1 }; // left upper arm
    frames[2][rightLowerArm] = { { 0.0f, -3 * unit }, 0.0f, {1 * unit, 1.5f}, blue, -1, -1 }; // left lower arm
    frames[2][rightHand] = { { 0.0f, -1 * unit}, 0.0f, {1 * unit, 0.5f},lemon,  -1, -1 }; // left hand
    frames[2][leftUpperArm] = { { 1 * unit, 1 * unit}, 30.0f, {1 * unit, 1.5f}, pink, -1, -1 }; // right upper arm
    frames[2][leftLowerArm] = { { 0.0f, -3 * unit }, 0.0f, {1 * unit, 1.5f}, pink, -1, -1 }; // right lower arm
    frames[2][leftHand] = { { 0.0f, -1 * unit }, 0.0f, {1 * unit, 0.5f}, lemon, -1, -1 }; // right hand
    
    frames[3][pelvis] = { { 0.0f, 9 * unit }, 0.0f, {2 * unit , 1 * unit}, red, -1, -1 }; // pelvis
    frames[3][rightUpperLeg] = { {  1 * unit, -2 * unit }, 30.0f, {1 * unit, 4 * unit}, blue,-1, -1 }; // left upper leg
    frames[3][rightLowerLeg] = { { 0 * unit ,  -4 * unit }, 0, {1 * unit, 4 * unit}, blue, -1, -1 }; // left lower leg
    frames[3][rightFoot] = { {  1 * unit , -2 * unit }, 0.0f, {2 * unit , 1 * unit}, blue, -1, -1 }; // left foot
    frames[3][leftUpperLeg] = { { -1 * unit, -2 * unit }, -30.0f, {1 * unit, 4 * unit}, hotPink, -1, -1 }; // right upper leg
    frames[3][leftLowerLeg] = { { 0.0f, -4 * unit }, 0, {1 * unit, 4 * unit}, hotPink, -1, -1 }; // right lower leg
    frames[3][leftFoot] = { { 1 * unit, -2 * unit }, 0.0f, {2 * unit , 1 * unit}, hotPink, -1, -1 }; // right foot
    frames[3][torso] = { { 0.0f, 3 * unit }, 0.0f, {1.0f, 2.5f}, pink, -1, -1 }; // torso
    frames[3][neck] = { { 0.0f, 3 * unit }, 0.0f, {0.4f, 0.5f}, salmon, -1, -1 }; // neck
    frames[3][head] = { { 0.0f, 1 * unit }, 0.0f, {1.0f, 1.0f}, champagne, -1, -1 }; // head
    frames[3][rightUpperArm] = { { -1 * unit, 1 * unit }, -30.0f, { 1 * unit, 1.5f}, blue, -1, -1 }; // left upper arm
    frames[3][rightLowerArm] = { { 0.5f * unit, -3 * unit }, 20.0f, {1 * unit, 1.5f}, blue, -1, -1 }; // left lower arm
    frames[3][rightHand] = { { 0.0f, -1 * unit}, 0.0f, {1 * unit, 0.5f},lemon,  -1, -1 }; // left hand
    frames[3][leftUpperArm] = { { 1 * unit, 1 * unit}, 30.0f, {1 * unit, 1.5f}, pink, -1, -1 }; // right upper arm
    frames[3][leftLowerArm] = { { 0.5f * unit, -3 * unit }, 20.0f, {1 * unit, 1.5f}, pink, -1, -1 }; // right lower arm
    frames[3][leftHand] = { { 0.0f, -1 * unit }, 0.0f, {1 * unit, 0.5f}, lemon, -1, -1 }; // right hand
    
    frames[4][pelvis] = { { 0.0f, 9 * unit }, 0.0f, {2 * unit , 1 * unit}, red, -1, -1 }; // pelvis
    frames[4][rightUpperLeg] = { {  1 * unit, -2 * unit }, 30.0f, {1 * unit, 4 * unit}, blue,-1, -1 }; // left upper leg
    frames[4][rightLowerLeg] = { { 0 * unit ,  -4 * unit }, 0, {1 * unit, 4 * unit}, blue, -1, -1 }; // left lower leg
    frames[4][rightFoot] = { {  1 * unit , -2 * unit }, 0.0f, {2 * unit , 1 * unit}, blue, -1, -1 }; // left foot
    frames[4][leftUpperLeg] = { { 0 * unit, -2 * unit }, 10.0f, {1 * unit, 4 * unit}, hotPink, -1, -1 }; // right upper leg
    frames[4][leftLowerLeg] = { { 0.0f, -4 * unit }, 0, {1 * unit, 4 * unit}, hotPink, -1, -1 }; // right lower leg
    frames[4][leftFoot] = { { 1 * unit, -2 * unit }, 0.0f, {2 * unit , 1 * unit}, hotPink, -1, -1 }; // right foot
    frames[4][torso] = { { 0.0f, 3 * unit }, 0.0f, {1.0f, 2.5f}, pink, -1, -1 }; // torso
    frames[4][neck] = { { 0.0f, 3 * unit }, 0.0f, {0.4f, 0.5f}, salmon, -1, -1 }; // neck
    frames[4][head] = { { 0.0f, 1 * unit }, 0.0f, {1.0f, 1.0f}, champagne, -1, -1 }; // head
    frames[4][rightUpperArm] = { { -1 * unit, 1 * unit }, 180.0f, { 1 * unit, 1.5f}, blue, -1, -1 }; // left upper arm
    frames[4][rightLowerArm] = { { 0.5f * unit, -3 * unit }, 0.0f, {1 * unit, 1.5f}, blue, -1, -1 }; // left lower arm
    frames[4][rightHand] = { { 0.0f, -1 * unit}, 0.0f, {1 * unit, 0.5f},lemon,  -1, -1 }; // left hand
    frames[4][leftUpperArm] = { { 1 * unit, 1 * unit}, 170.0f, {1 * unit, 1.5f}, pink, -1, -1 }; // right upper arm
    frames[4][leftLowerArm] = { { 0.5f * unit, -3 * unit }, 0.0f, {1 * unit, 1.5f}, pink, -1, -1 }; // right lower arm
    frames[4][leftHand] = { { 0.0f, -1 * unit }, 0.0f, {1 * unit, 0.5f}, lemon, -1, -1 }; // right hand


}

void Character::init() {
    for (size_t i = 0; i < 3; i++) {
        sendDataToOpenGL(&model[i], &VAO[i], & VBO[i], &EBO[i]);
    }
}

void Character::linkBody() {
    for (unsigned int i = 0; i < 5; i++) {
        frames[i][pelvis].child =leftUpperLeg; // pelvis 
        frames[i][leftUpperLeg].child =leftLowerLeg; // 
        frames[i][leftLowerLeg].child =leftFoot;
        frames[i][leftUpperLeg].sibling =rightUpperLeg;
        frames[i][rightUpperLeg].child =rightLowerLeg;
        frames[i][rightLowerLeg].child =rightFoot;
        frames[i][rightUpperLeg].sibling =torso;
        frames[i][torso].child =neck;
        frames[i][neck].child =head;
        frames[i][neck].sibling =leftUpperArm;
        frames[i][leftUpperArm].child =leftLowerArm;
        frames[i][leftLowerArm].child =leftHand;
        frames[i][leftUpperArm].sibling =rightUpperArm;
        frames[i][rightUpperArm].child =rightLowerArm;
        frames[i][rightLowerArm].child =rightHand;
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

void Character::updatePosY(GLfloat baselineFront, GLfloat baselineCenter, GLfloat baselinePrev, bool* onGame) {
    if (jumping) {
        if (PosY < actualLimit) {
            PosY += jumpSpeed;
        }
        else {
            jumping = false;
        }
    }
    else {
        if (PosY > baselineCenter) {
            PosY -= jumpSpeed;
        }
    }

    if (!falling && PosY < (baselineFront + 0.01f) && PosY > (baselineFront - 0.02f)) {
        PosY = baselineFront;
    } 

    if (!falling && PosY < (baselineFront - 0.1f) ) {
        std::cout << "terrain collision" << std::endl;
        *onGame = false;
    }

    if (baselineFront == 0.0f && PosY < baselinePrev) {
        falling = true;
    }


    if (falling) {
        PosY -= 2 * jumpSpeed;
        if (PosY < -1.5f) {
           *onGame = false;
        }
    }
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