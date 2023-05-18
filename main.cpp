#include "Angel.h"
#include "Game.h"
#include <vector>

using namespace std;

Game* game;
mat4 projection;
int currentViewMode = 1;
deque<float> starVis;

bool isCollision(float x, float y, float sizeX, float sizeY) {
	/* character info */
	float cx = game->getCharacter()->getX();
	float cy = game->getCharacter()->getY();
	float height = game->getCharacter()->getHeight();
	float width = game->getCharacter()->getWidth();
	//cout << "cx " << cx << " cy " << cy << endl;
	//cout << "fx " << x << " fy " << y << endl;
	float right = cx + width / 2.0;
	float left = cx - width / 2.0;
	float top = cy + height / 2.0;
	float bot = cy - height / 2.0;

	float leftLine = x - sizeX / 2.0;
	float rightLine = x + sizeX / 2.0;
	float topLine = y + sizeY / 2.0;
	float botLine = y - sizeY / 2.0;

	if (leftLine < left && left < rightLine) {
		if (botLine < top && top < topLine)
			return true;
		else if (botLine < bot && bot < topLine)
			return true;
		else
			return false;
	}
	else if (leftLine < right && right < rightLine) {
		if (botLine < top && top < topLine)
			return true;
		else if (botLine < bot && bot < topLine)
			return true;
		else
			return false;
	}
	else
		return false;

}

void gameOverChecker() {
	/* fireball collision check */
	deque<vec3>* FirePos = game->getFireball()->getFirePos();
	float FireStartPos = game->getFireball()->getStartPos();
	float FireballWidth = game->getFireball()->getWidth();
	float FireballHeight = game->getFireball()->getHeight();
	for (size_t i = 0; i < FirePos->size(); i++) {
		float fx = FireStartPos + FirePos->at(i).x;
		float fy = FirePos->at(i).y;
		if (isCollision(fx, fy, FireballWidth, FireballHeight)) {
			game->setOnGame(false);
		}
	}

	/* star collision checker */
	deque<vec3>* StarPos = game->getStar()->getStarPos();
	float StarStartPos = game->getStar()->getStartPos();
	float StarWidth = game->getStar()->getWidth();
	float c = game->getStar()->getHeight();
	for (size_t i = 0; i < StarPos->size(); i++) {
		float sx = StarStartPos + StarPos->at(i).x;
		float sy = StarPos->at(i).y;
		if (starVis[i] == 1.0 && isCollision(sx, sy, StarWidth, StarWidth)) {
			game->addScore(5);
			starVis[i] = 0.0;
			cout << "score added" << endl;
		}
	}

	/* character action checker */
	int CharacterState = game->getCharacter()->getState();
	float CharacterPosY = game->getCharacter()->getY();
	float CharacterHeight = game->getCharacter()->getHeight();

	if (CharacterPosY < -1.5 || (CharacterState != 3 && CharacterPosY < (game->baseline + CharacterHeight/ 2.0))) {
		game->setOnGame(false);
	}

}

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
	// Enable writing to the depth buffer
	glDepthMask(GL_TRUE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthFunc(GL_LESS);
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_TEXTURE_2D);

	game = new Game();
	game->shaderInit();
	game->init();
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wire frame mode
	game->drawAll();
	// gameOverChecker();

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
	case ' ':
		game->getCharacter()->setJumping();
		break;
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
	}
}
void update(int value) {
	// Third-person perspective view 
	if (currentViewMode == 1) {
		vec4 eye = vec4(1.5, 1.7, 8.0, 0.0);
		vec4 at = vec4(1.5, 1.0, -1.0, 0.0);
		vec4 up = vec4(0.0, 1.0, 0.0, 0.0);
		projection = LookAt(eye, at, up);
		projection = Perspective(17, 1.5, 0.1, 2000) * projection;
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
		vec4 eye = vec4(-1.0, 1.7, 1.0, 0.0);
		vec4 at = vec4(1.0, 0.0, 1.0, 0.0);
		vec4 up = vec4(0.0, 1.0, 0.0, 0.0);
		projection = LookAt(eye, at, up);
		projection = Perspective(80, 1.5, 0.1, 2000) * projection;
		glUniformMatrix4fv(game->projection_loc, 1, GL_TRUE, projection);
	}
	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

void main(int argc, char** argv) {
	init(argc, argv);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
}
