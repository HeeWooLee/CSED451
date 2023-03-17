#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>	
//#include <GL/glut.h>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

struct Position
{
    GLfloat x;
    GLfloat y;
};

GLuint score = 0;
bool onGame = true;
bool quitGame = false;
bool quiting = false;

GLfloat mapSpeed = 0.012f;

GLfloat characterPosY = 0.0f;
GLfloat jumpSpeed = 2.5 * mapSpeed;
GLfloat jumpLimit = 1.0f;
GLfloat fallSpeed = 0.1f;
bool jumping = false;
bool falling = false;
GLfloat characterCenter = -0.6f;
GLfloat characterHalfWidth = 0.1f;
GLfloat characterHalfHeight = 0.05f;

int windowWidth = 500;
int windowHeight = 500;

Position fireballPos;
GLfloat fireballPosX;
GLfloat fireballSpeed = 2.0 * mapSpeed;
std::vector<Position> fireballs;

Position starPos;
GLfloat starPosX;
GLfloat starSpeed = mapSpeed;
GLfloat transparent = 0.0f;
std::vector<Position> starPattern = { {0.1f, 0.1f}, {0.2f, 0.4f}, {0.3f, 0.7f}, {0.4f, 1.0f}, {0.5f, 0.7f}, {0.6f, 0.4f}, {0.7f, 0.1f} };
std::vector<Position> starPattern2 = { {0.0f, 0.05f}, {0.2f, 0.05f}, {0.4f, 0.05f}, {0.6f, 0.05f} };
std::vector<std::vector<Position>> starPatterns = { starPattern, starPattern2 };
std::vector<GLfloat> stars;
std::vector<GLfloat> starsPatternChoice;
std::vector<std::vector<GLfloat>> starsConsumed;

GLfloat hole = 0.5f;
GLfloat holeWidth = 0.4f;
GLfloat holeSpeed = mapSpeed;
std::vector<GLfloat> holes;

int fireballWidth, fireballHeight, nrChannels;

void init() {
    GLfloat tmp = 1.5f;
    Position tmpPos;
    fireballPos.x = 1.5f;
    fireballPos.y = 0.2f;

    starPos.x = 1.5f;
    starPos.y = 0.4f;

    holes.push_back(1.0f);
    holes.push_back(2.4f);
    for (size_t i = 0; i < 100; ++i) {
        tmp += (rand() % 15 + 9.0) / 10.0;
        holes.push_back(tmp);
    }

    tmp = 1.5f;
    for (size_t i = 0; i < 100; ++i) {
        tmp += (rand() % 13 + 6.0) / 10.0;
        stars.push_back(tmp);
        starsPatternChoice.push_back(rand() % 2);
        std::vector<GLfloat> consumed(starPatterns[starsPatternChoice[i]].size(), 1.0f);
        starsConsumed.push_back(consumed);
    }

    tmp = 1.5f;
    for (size_t i = 0; i < 150; ++i) {
        tmp += (rand() % 20 + 20.0) / 10.0;
        tmpPos.x = tmp;
        tmpPos.y = rand() % 8 / 10.0;
        fireballs.push_back(tmpPos);
    }



}

void drawRectangle() {
    glColor3f(0.62f, 0.12f, 0.94f);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, 0.1f);
    glVertex2f(0.1f, 0.1f);
    glVertex2f(0.1f, 0.0f);
    glVertex2f(-0.1f, 0.0f);
    glEnd();
}

void drawFireBall() {
    GLfloat mulFactor1 = 1.4f;
    GLfloat addFactor1 = 0.015f;

    GLfloat mulFactor3 = 2.0f;
    GLfloat addFactor3 = 0.025f;

    GLfloat mulFactor2 = 2.8f;
    GLfloat addFactor2 = 0.033f;


    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.1f);
    glVertex2f(0.1f, 0.1f);
    glVertex2f(0.2f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glEnd();

    glColor3f(1.0f, 0.647f, 0.0f); // orange
    glBegin(GL_QUADS);
    glVertex2f(0.0f / mulFactor1 + addFactor1, 0.1f / mulFactor1 + addFactor1);
    glVertex2f(0.1f / mulFactor1 + addFactor1, 0.1f / mulFactor1 + addFactor1);
    glVertex2f(0.2f / mulFactor1 + addFactor1, 0.0f + addFactor1);
    glVertex2f(0.0f / mulFactor1 + addFactor1, 0.0f + addFactor1);
    glEnd();


    glColor3f(1.0f, 1.0f, 0.0f); // yellow
    glBegin(GL_QUADS);
    glVertex2f(0.0f / mulFactor3 + addFactor3, 0.1f / mulFactor3 + addFactor3);
    glVertex2f(0.1f / mulFactor3 + addFactor3, 0.1f / mulFactor3 + addFactor3);
    glVertex2f(0.2f / mulFactor3 + addFactor3, 0.0f + addFactor3);
    glVertex2f(0.0f / mulFactor3 + addFactor3, 0.0f + addFactor3);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(0.0f + addFactor2, 0.1f / mulFactor2 + addFactor2);
    glVertex2f(0.1f / mulFactor2 + addFactor2, 0.1f / mulFactor2 + addFactor2);
    glVertex2f(0.1f / mulFactor2 + addFactor2, 0.0f + addFactor2);
    glVertex2f(0.0f + addFactor2, 0.0f + addFactor2);
    glEnd();
}

void drawTerrain() {
    glBegin(GL_QUADS);
    glColor3f(0.678431f, 1.0f, 0.184314f);//green
    glVertex2f(-5, 0.5);
    glVertex2f(5, 0.5);
    glVertex2f(5, 0);
    glVertex2f(-5, 0);
    glEnd();
}

void drawHole() {
    glBegin(GL_QUADS);
    glColor3f(0.22f, 0.22f, 0.22f);
    glVertex2f(0, 0.5);
    glVertex2f(holeWidth, 0.5);
    glVertex2f(holeWidth, 0);
    glVertex2f(0, 0);
    glEnd();
}

void drawStar(GLfloat consumed) {
    glColor4f(0.98f, 0.98f, 0.0f, consumed);  //yellow
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, characterHalfHeight);
    glVertex2f(characterHalfWidth / 2, 0.0);
    glVertex2f(-characterHalfWidth / 2, 0.0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, -characterHalfHeight + 0.035f);
    glVertex2f(-characterHalfWidth / 2, 0.035f);
    glVertex2f(characterHalfWidth / 2, 0.035f);
    glEnd();
}

void drawText(std::string text, void* font) {
    glRasterPos2i(0, 0);
    for (int i = 0; i < text.length(); i++) {
        glutBitmapCharacter(font, text[i]);
    }
}

void display() {
    glClearColor(0.529412f, 0.807843f, 0.980392f, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glTranslatef(0.0f, -1, 0.0f);
    drawTerrain();
    glPopMatrix();

    glTranslatef(0.0f, -1, 0.0f);
    for (int i = 0; i < holes.size(); ++i) {
        GLfloat holePos = holes[i] + hole;
        if (holePos >= 4) {
            break;
        }
        if (holePos > -4 && holePos < 4) {
            glPushMatrix();
            glTranslatef(holePos, 0.0f, 0.0f);
            drawHole();
            glPopMatrix();
        }
    }
    glTranslatef(0.0f, 1, 0.0f);


    for (int i = 0; i < stars.size(); ++i) {
        GLuint choice = starsPatternChoice[i];
        GLfloat PosX = stars[i] + starPosX;
        if (PosX >= 4) {
            break;
        }
        if (PosX > -4 && PosX < 4) {
            glPushMatrix();
            glTranslatef(0.0f, -0.5, 0.0f);
            glTranslatef(PosX, 0.0f, 0.0f);
            std::vector<Position> chosen = starPatterns[choice];
            for (int j = 0; j < chosen.size(); ++j) {
                glPushMatrix();
                glTranslatef(0.0f, chosen[j].y, 0.0f);
                glTranslatef(chosen[j].x, 0.0f, 0.0f);
                drawStar(starsConsumed[i][j]);
                glPopMatrix();
            }
            glPopMatrix();
        }
    }

    glPushMatrix();
    glTranslatef(characterCenter, -0.5, 0.0f);
    glTranslatef(0.0f, characterPosY, 0.0f);
    drawRectangle();
    glPopMatrix();

    for (int i = 0; i < fireballs.size(); ++i) {
        GLfloat PosX = fireballs[i].x + fireballPosX;
        if (PosX >= 4) {
            break;
        }
        if (PosX > -4 && PosX < 4) {
            glPushMatrix();
            glTranslatef(0.0f, -0.5 + fireballs[i].y, 0.0f);
            glTranslatef(PosX, 0.0f, 0.0f);
            drawFireBall();
            glPopMatrix();
        }
    }

    glPushMatrix();
    glColor3f(0.62f, 0.12f, 0.94f);
    glTranslatef(-1.6f, 0.8f, 0.0f);
    drawText("Score: " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24);
    glPopMatrix();

    if (onGame == false) {
        glPushMatrix();
        glColor3f(0.529412f, 0.807843f, 0.980392f);
        glTranslatef(-1.6f, 0.8f, 0.0f);
        drawText("Score: " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.62f, 0.12f, 0.94f);
        glTranslatef(-0.15f, 0.0f, 0.0f);
        drawText("Game Over", GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.62f, 0.12f, 0.94f);
        glTranslatef(-0.16f, -0.1f, 0.0f);
        drawText("Final Score: " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();
    }
    if (quiting == true) {
        glPushMatrix();
        glColor3f(0.529412f, 0.807843f, 0.980392f);
        glTranslatef(-1.6f, 0.8f, 0.0f);
        drawText("Score: " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.62f, 0.12f, 0.94f);
        glTranslatef(-0.15f, 0.0f, 0.0f);
        drawText("Game Over", GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.62f, 0.12f, 0.94f);
        glTranslatef(-0.16f, -0.1f, 0.0f);
        drawText("Final Score: " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();
    }
    if (falling == true) {
        glPushMatrix();
        glColor3f(0.529412f, 0.807843f, 0.980392f);
        glTranslatef(-1.6f, 0.8f, 0.0f);
        drawText("Score: " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.62f, 0.12f, 0.94f);
        glTranslatef(-0.15f, 0.0f, 0.0f);
        drawText("Game Over", GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();

        glPushMatrix();
        glColor3f(0.62f, 0.12f, 0.94f);
        glTranslatef(-0.16f, -0.1f, 0.0f);
        drawText("Final Score: " + std::to_string(score), GLUT_BITMAP_TIMES_ROMAN_24);
        glPopMatrix();
    }

    glutSwapBuffers();
}

void holeContactDetected() {
    if (characterPosY < 0.01f) {
        for (int i = 0; i < holes.size(); ++i) {
            GLfloat holePos = holes[i] + hole;
            if (holePos >= 4) {
                break;
            }
            if (holePos > -4 && holePos < 4) {
                if ((holePos - characterHalfWidth / 2) < (characterCenter - characterHalfWidth) && (holePos + holeWidth) > (characterCenter + characterHalfWidth)) {
                    falling = true;
                    std::cout << "falling cond1" << std::endl;
                }
                else if ((holePos - characterHalfWidth / 2) < (characterCenter - characterHalfWidth) && (characterCenter + characterHalfWidth) < (holePos + holeWidth)) {
                    falling = true;
                    std::cout << "falling cond2" << std::endl;
                }
            }
        }
    }
}

void fireballContactDetected() {
    for (int i = 0; i < fireballs.size(); ++i) {
        GLfloat PosX = fireballs[i].x + fireballPosX;
        if (PosX >= 4) {
            break;
        }
        if (PosX > -4 && PosX < 4) {
            if (characterPosY < (fireballs[i].y + characterHalfHeight) && characterPosY >(fireballs[i].y - characterHalfHeight)
                && (characterCenter + characterHalfWidth) > PosX && (characterCenter - characterHalfWidth) < PosX) {
                std::cout << "fireball contact" << std::endl;
                onGame = false;
            }
        }
    }

}

void starContactDetected() {
    for (int i = 0; i < stars.size(); ++i) {
        GLuint choice = starsPatternChoice[i];
        GLfloat PosX = stars[i] + starPosX;
        if (PosX >= 4) {
            break;
        }
        if (PosX > -4.0 && PosX < 4.0) {
            std::vector<Position> chosen = starPatterns[choice];
            std::vector<GLfloat>* consumed = &(starsConsumed[i]);
            for (int j = 0; j < chosen.size(); ++j) {
                if ((*consumed)[j] != transparent && (characterPosY + 2.0 * characterHalfHeight) > chosen[j].y && characterPosY < chosen[j].y
                    && (characterCenter + 2.5 * characterHalfWidth) >(PosX + chosen[j].x) && (characterCenter - characterHalfWidth) < (PosX + chosen[j].x)) {
                    std::cout << "star contact" << std::endl;
                    score += 5;
                    (*consumed)[j] = transparent;

                    std::cout << "score " << score << std::endl;
                }
            }
        }
    }
}

void update(int value) {
    if (onGame) {
        if (!quitGame) {
            holeContactDetected();
            fireballContactDetected();
            starContactDetected();
            if (jumping) {
                if (characterPosY < jumpLimit) {
                    characterPosY += jumpSpeed;
                }
                else {
                    jumping = false;
                }
            }
            else {
                if (characterPosY > 0.0f) {
                    characterPosY -= jumpSpeed;
                }
            }

            if (!falling && characterPosY < 0.01f) {
                characterPosY = 0.0f;
            }

            if (falling) {
                characterPosY -= fallSpeed;
                if (characterPosY < -1.5f) {
                    onGame = false;
                }
            }

            if (quiting) {
                onGame = false;
            }

            fireballPosX -= fireballSpeed;

            starPosX -= starSpeed;

            hole -= holeSpeed;

            glutPostRedisplay();
            glutTimerFunc(10, update, 0);
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case ' ':
        if (key == ' ') {
            if (!jumping && characterPosY < 0.03) {
                jumping = true; // to avoid twice jump 
            }
        }
        break;

    case 'q':
    case 'Q':
    case 27:
        quiting = true;
        break;

    case 13: //press 'Enter' to restart 
    {
        onGame = true;
        break;
    }
    }
}

void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (windowWidth <= windowHeight) {
        GLfloat aspect = (GLfloat)windowHeight / (GLfloat)windowWidth;
        glOrtho(-1.0, 1.0, -1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    }
    else {
        GLfloat aspect = (GLfloat)windowWidth / (GLfloat)windowHeight;
        glOrtho(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0, -1.0, 1.0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);


    int no_menu_bar_width = GetSystemMetrics(SM_CXFULLSCREEN);
    int no_menu_bar__height = GetSystemMetrics(SM_CYFULLSCREEN);
    glutInitWindowPosition((no_menu_bar_width - 960) / 2, (no_menu_bar__height - 540) / 2);

    glutInitWindowSize(960, 540);
    init();
    glutCreateWindow("Jumping Rectangle");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutKeyboardFunc(keyboard);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(1, update, 0);

    glutMainLoop();
}