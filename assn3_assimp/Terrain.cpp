#include "Terrain.h"

Terrain::Terrain(GLfloat characterPosX, GLfloat gameSpeed) {

    terrainModel = loadObjFile("meshes/block.obj");
    starModel = loadObjFile("meshes/star.obj");
    mushroomModel = loadObjFile("meshes/mushroom.obj");


    for (int i = 0; i < 12; ++i) {
        blockHeight.push_back(FirstLevel);
    }
    for (int i = 0; i < 8; ++i) {
        updateBlockHeight();
    }
    startPoint = blockWidth * -9.0f;
    characterBottomLine = blockLevels[blockHeight[floor((characterPosX - startPoint) / blockWidth)]];
    mapSpeed = gameSpeed;
}

void Terrain::init() {
        sendDataToOpenGL(&terrainModel, &terrainVAO, &terrainVBO, &terrainEBO);
        sendDataToOpenGL(&starModel, &starVAO, &starVBO, &starEBO);
        sendDataToOpenGL(&mushroomModel, &mushroomVAO, &mushroomVBO, &mushroomEBO);

}

void Terrain::drawTerrainBlock(GLfloat height) {
    glBegin(GL_QUADS);
    glColor3f(0.678431f, 1.0f, 0.184314f);//green
    glVertex2f(0, height);
    glVertex2f(blockWidth, height);
    glVertex2f(blockWidth, 0);
    glVertex2f(0, 0);
    glEnd();
}

void Terrain::drawTerrainMushroom(GLfloat height) {

    glBegin(GL_QUADS);
    glColor3f(0.941176f, 1.0f, 0.941176f);
    glVertex2f(0.4, height + 0.05);
    glVertex2f(0.5, height + 0.05);
    glVertex2f(0.5, height);
    glVertex2f(0.4, height);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.388235f, 0.278431f);
    glVertex2f(0.38, height + 0.1);
    glVertex2f(0.41, height + 0.1);
    glVertex2f(0.41, height + 0.05);
    glVertex2f(0.38, height + 0.05);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.388235f, 0.278431f);
    glVertex2f(0.49, height + 0.1);
    glVertex2f(0.52, height + 0.1);
    glVertex2f(0.52, height + 0.05);
    glVertex2f(0.49, height + 0.05);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.388235f, 0.278431f);
    glVertex2f(0.405, height + 0.125);
    glVertex2f(0.43, height + 0.125);
    glVertex2f(0.43, height + 0.05);
    glVertex2f(0.405, height + 0.05);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.388235f, 0.278431f);
    glVertex2f(0.47, height + 0.125);
    glVertex2f(0.495, height + 0.125);
    glVertex2f(0.495, height + 0.05);
    glVertex2f(0.47, height + 0.05);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.388235f, 0.278431f);
    glVertex2f(0.43, height + 0.145);
    glVertex2f(0.47, height + 0.145);
    glVertex2f(0.47, height + 0.05);
    glVertex2f(0.43, height + 0.05);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.192157f, 0.309804f, 0.309804f);
    glVertex2f(0.42, height + 0.04);
    glVertex2f(0.43, height + 0.04);
    glVertex2f(0.43, height + 0.03);
    glVertex2f(0.42, height + 0.03);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.192157f, 0.309804f, 0.309804f);
    glVertex2f(0.47, height + 0.04);
    glVertex2f(0.48, height + 0.04);
    glVertex2f(0.48, height + 0.03);
    glVertex2f(0.47, height + 0.03);
    glEnd();
    //collision box
    /*
    glBegin(GL_QUADS);
    glColor4f(0.192157f, 0.309804f, 0.309804f, 0.5);
    glVertex2f(0.38, height);
    glVertex2f(0.52, height);
    glVertex2f(0.52, height + 0.15);
    glVertex2f(0.38, height + 0.15);
    glEnd();
    */
}

void Terrain::drawMushroom() {
    glTranslatef(startPoint, -1.0f, 0.0f);
    glTranslatef(-0.38, 0.0f, 0.0f);
    for (int i = 0; i < blockHeight.size(); ++i) {
        GLfloat height = blockLevels[blockHeight[i]];

        if (blockHeight[i] == 2) {
            /* if (blockHeight[i] != 0.3f) {*/
            draw(&mushroomVAO, &mushroomModel);
            //}
        }
        glTranslatef(blockWidth, 0.0f, 0.0f);
    }
}

void Terrain::updateBlockHeight() {
    GLint lastBlock = blockHeight.back();
    if (lastBlock == Hole) {
        blockHeight.push_back(blockHeight[blockHeight.size() - 2]);
    }
    else if (rand() % 7 > 5) {
        blockHeight.push_back(Hole);
    }
    else {
        int randNum = rand() % 3 - 1;

        if ((lastBlock == FirstLevel && randNum == -1) || (lastBlock == FourthLevel && randNum == 1)) {
            blockHeight.push_back(lastBlock - randNum);
        }
        else {
            blockHeight.push_back(lastBlock + randNum);
        }
    }
}

void Terrain::updateStartPoint() {
    startPoint -= mapSpeed;

    if (startPoint < blockWidth * -10.0f) {
        startPoint = blockWidth * -9.0f;
        blockHeight.pop_front();
        updateBlockHeight();
    }
}

GLfloat Terrain::updateCharacterStandingYPos(GLfloat PosX, GLint order) {
    int blockTmp = blockHeight[floor((PosX - startPoint) / blockWidth) + order];
    if (blockTmp == 2 && startPoint >= (blockWidth * -9.0f - 0.15f)) { //on mushroom
        return blockLevels[blockTmp] + 0.15f;
    }
    else {
        return blockLevels[blockTmp];
    }
}

void Terrain::drawBlockCollisionLine(GLfloat PosX) {
    GLfloat x = startPoint + (floor ((PosX - startPoint) / blockWidth)+ 1)* blockWidth;

    int blockTmp = blockHeight[floor ((PosX - startPoint) / blockWidth)+ 1];
    if (blockTmp == 2) {
        glBegin(GL_LINES);
        glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
        glVertex2f(x, 1.0f);
        glVertex2f(x, 0.0f);
        glEnd();
    }
}

GLint Terrain::checkNearMushroom(GLfloat PosX) {
    GLfloat x = startPoint + (floor((PosX - startPoint) / blockWidth) + 1) * blockWidth;
    int blockCurr = blockHeight[floor((PosX - startPoint) / blockWidth)];
    int blockNext= blockHeight[floor((PosX - startPoint) / blockWidth) + 1];
    std::cout << "here" << std::endl;
    if (blockNext == 2 && (x - PosX) < blockWidth- 0.001f) {
        if (blockCurr < blockNext) { // higher
            return 0;
        }
        else {
            return 1;   // same or less 
        }
    }
    else {
        return 2; // no mushroom
    }
 }

void Terrain::drawTerrain() {
    glTranslatef(startPoint, -1.0f, 0.0f);
    for (int i = 0; i < blockHeight.size(); ++i) {
        GLfloat height = blockLevels[blockHeight[i]];
        draw(&terrainVAO, &terrainModel);
        glTranslatef(blockWidth, 0.0f, 0.0f);
    }
}

GLfloat Terrain::getBlockWidth(){
    return blockWidth;
}