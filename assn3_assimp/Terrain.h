#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <deque>
#include "Model.h"

enum BlockLevel {
    Hole,
    FirstLevel,
    SecondLevel,
    ThirdLevel,
    FourthLevel,
};

class Terrain {
    private:
        std::deque<GLint> blockHeight;
        GLfloat startPoint;
        GLfloat characterBottomLine;
        GLfloat mapSpeed;
        GLfloat blockLevels[5] = { 0.0f, 0.5f, 0.7f, 0.9f, 1.1f, };
        GLfloat blockWidth = 0.5f;

        Model terrainModel;
        GLuint terrainVAO;
        GLuint terrainVBO;
        GLuint terrainEBO;

        Model starModel;
        GLuint starVAO;
        GLuint starVBO;
        GLuint starEBO;

        Model mushroomModel;
        GLuint mushroomVAO;
        GLuint mushroomVBO;
        GLuint mushroomEBO;

        Model ballModel;
        GLuint ballVAO;
        GLuint ballVBO;
        GLuint ballEBO;
    public:
        ~Terrain() {};
        Terrain();
        void init();
        void updateBlockHeight();
        void updateStartPoint();
        GLfloat updateCharacterStandingYPos(GLfloat characterPosX, GLint order);
        void drawTerrainBlock(GLfloat height);
        void drawTerrain();

        void drawStar();
        void drawT();
        void drawMushroom();
        void drawBall();
        GLfloat getBlockWidth();
        void drawBlockCollisionLine(GLfloat PosX); 
        GLint checkNearMushroom(GLfloat PosX);
};