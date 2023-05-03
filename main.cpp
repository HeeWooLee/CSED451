#include "Angel.h"
#include "Game.h"
#include <vector>

using namespace std;

Game* game;


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
	glEnable(GL_LINE_SMOOTH);
	glDepthFunc(GL_LEQUAL);	
	game = new Game();
	game->shaderInit();
	game->init();
	cout << "called once " << endl;
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	game->drawAll();
	gameOverChecker();

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


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case ' ':
		game->getCharacter()->setJumping();
		break;
	}
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
