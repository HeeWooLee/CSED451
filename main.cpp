#include "Angel.h"
#include "Game.h"
#include <vector>

using namespace std;

Game* game;

void init(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);

	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Hello OpenGL");

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glDepthFunc(GL_LEQUAL);	
	game = new Game();
	game->shaderInit();
	game->init();
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(1.0f, 0.0f, 0.0f);

	game->drawAll();
	glutSwapBuffers();
	glutPostRedisplay();
}

void update(int value) {
	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

void reshape(int width, int height) {
	int windowWidth = width;
	int windowHeight = height;
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

void main(int argc, char** argv) {

	init(argc, argv);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
}
