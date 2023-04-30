#include "Angel.h"
#include "Game.h"
#include <vector>

using namespace std;

Game* game;
mat4 projection;
int currentViewMode = 2;

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
		//case ' ':
		//	game.setJump();
		//	break;
		//case 'q':
		//case 'Q':
		//case 27:
		//	game.endGame();
		//	break;

	default:
		break;

	}
}
void update(int value) {
	// Third-person perspective view 
	if (currentViewMode == 1) {
		vec4 eye = vec4(1.3, 1.7, 7.0, 0.0);
		vec4 at = vec4(1.3, 1.0, -1.0, 0.0);
		vec4 up = vec4(0.0, 1.0, 0.0, 0.0);
		projection = LookAt(eye, at, up);
		projection = Perspective(20, 1.5, 0.1, 2000) * projection;
		glUniformMatrix4fv(game->projection_loc, 1, GL_TRUE, projection);
	}
	// Third-person ortho view 
	else if (currentViewMode == 2) {
		vec4 eye = vec4(1.3, 1.0, 10.0, 0.0);
		vec4 at = vec4(1.3, 1.0, 0.0, 0.0);
		vec4 up = vec4(0.0, 1.0, 0.0, 0.0);
		projection = LookAt(eye, at, up);
		/*projection = Perspective(1, 1.5, 0.1, 2000) * projection;*/
		projection = Ortho(-1.5, 1.5, -1.5, 1.5, 0.1, 2000) * projection;
		glUniformMatrix4fv(game->projection_loc, 1, GL_TRUE, projection);
	}

	// First-person perspective
	else {
		vec4 eye = vec4(-60.0, 5.0, 1.0, 0.0);
		vec4 at = vec4(5.0, 0.2, 1.0, 0.0);
		vec4 up = vec4(0.0, 1.0, 0.0, 0.0);
		projection = LookAt(eye, at, up);
		projection = Perspective(1, 1.5, 0.1, 2000) * projection;
		glUniformMatrix4fv(game->projection_loc, 1, GL_TRUE, projection);
	}

	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}
void main(int argc, char** argv) {

	init(argc, argv);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
}
