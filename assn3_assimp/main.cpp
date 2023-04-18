#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
// #include "OBJLoader.h"
#include "Game.h"
// global variable to store the current view mode
int currentViewMode = 1;


int anime = 1;
Game game;
void init() {
}

// reshape function to handle window resizing and set the correct projection
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (currentViewMode == 1) {
        gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
    }

    else if (currentViewMode == 2) {
        glOrtho(-5, 5, -5, 5, 0.1, 100.0);
    }

    else {

        gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 1.0, 100.0);
    }


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// a keyboard function to handle keyboard input and switch the view mode
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1':
        currentViewMode = 1;
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        break;
    case '2':
        currentViewMode = 2;
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        break;
    case '3':
        currentViewMode = 3;
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        break;
    case ' ':
        game.setJump();
        break;

    case 'q':
    case 'Q':
    case 27:
        game.endGame();
        break;
    default:
        break;

    }
}


void initializedGL(void) //run only once
{
    glewExperimental = GL_TRUE;
    glewInit();
    game.init();
}


void paintGL(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (currentViewMode == 1) {
        // View mode 1: Camera behind the star
        gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
    else if (currentViewMode == 2) {
        gluLookAt(0, 0, 0.2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
    else {
        // View mode 2: Camera at the top left direction
        //gluLookAt(5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        //gluLookAt(-5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        gluLookAt(-3.0, 3.0, 0.0, 5.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    game.draw();

    glutSwapBuffers();
    glutPostRedisplay();
}

void update(int value) {
    game.update();
            glutPostRedisplay();
            glutTimerFunc(5, update, 0);
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    init();
    glutInitWindowSize(640, 480);
    glutCreateWindow("OpenGL Window");
    initializedGL();
    glutDisplayFunc(paintGL);

    glutReshapeFunc(reshape); // Register the reshape function
    glutKeyboardFunc(keyboard); // Register the keyboard function

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glutTimerFunc(5, update, 0);

    glutMainLoop();

    return 0;
}