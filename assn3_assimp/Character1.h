#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Utils.h"
#include "Model.h"
struct Node {
    Position translation;
    GLfloat angle;
    Position scale;
    Color color;
    int child;
    int sibling;
};


class Character {
private:
    GLint frame;
    Node frames[5][16];
    bool jumping;
    bool falling;
    GLfloat width;
    GLfloat height;
    GLfloat jumpSpeed;
    GLfloat jumpLimit;
    GLfloat actualLimit;
    GLfloat PosY;
    GLfloat PosX;
    Model model[3];
    GLuint VAO[3];
    GLuint VBO[3];
    GLuint EBO[3];
    GLfloat baseline = 0;

public:
    ~Character() {};
    Character();
    void init();
    void hierarchicalRender(int currentNodeInd, int anime);
    void drawCircle();
    void frameSelection(int* anime);
    void drawCharacter(int* anime);
    void collisionBox();
    void setJumping(GLint flag);
    void updatePosY();
    GLfloat getPosY();    GLfloat getPosX();
    GLfloat getWidth();
    GLfloat getHeight();
};
