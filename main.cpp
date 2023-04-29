#include "Angel.h"
#include "Game.h"
#include <vector>

using namespace std;

Game* game;


//GLuint VBO;
//GLuint EBO;
//GLuint VAO;
//GLuint program;
//GLuint ver_loc;
//void loadData() {
//	vector<vec3> vertex_positions = {
//		vec3(-1.0f, -1.0f, -1.0f),
//		vec3(-1.0f, -1.0f,  1.0f),
//		vec3(-1.0f,  1.0f, -1.0f),
//		vec3(-1.0f,  1.0f,  1.0f),
//		vec3(1.0f, -1.0f, -1.0f),
//		vec3(1.0f, -1.0f,  1.0f),
//		vec3(1.0f,  1.0f, -1.0f),
//		vec3(1.0f,  1.0f,  1.0f),
//	};
//
//	vector<GLuint> indices = {
//		0, 1, 2, 2, 1, 3, // left face
//		4, 5, 6, 6, 5, 7, // right face
//		0, 4, 2, 2, 4, 6, // bottom face
//		1, 5, 3, 3, 5, 7, // top face
//		0, 1, 4, 4, 1, 5, // front face
//		2, 3, 6, 6, 3, 7  // back face
//	};
//
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, vertex_positions.size() * sizeof(vec3), &vertex_positions[0], GL_STATIC_DRAW);
//
//	glGenBuffers(1, &EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
//}

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
	// loadData();
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColor3f(1.0f, 0.0f, 0.0f);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glVertexAttribPointer(game->ver_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	game->getCharacter()->draw();
	glutSwapBuffers();
	glutPostRedisplay();
}

void update(int value) {
	glutPostRedisplay();
	glutTimerFunc(5, update, 0);
}

void main(int argc, char** argv) {

	init(argc, argv);

	glutDisplayFunc(renderScene);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
}
