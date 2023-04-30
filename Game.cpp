#include "Game.h"

Game::Game() {
	scale = vec3(0.001f, 0.001f, 0.001f);
	speed = -0.0001f;
	baseline = initBaseline = -0.5f;
}

void Game::shaderInit() {
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	ver_loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(ver_loc);

	model_loc = glGetUniformLocation(program, "model_view");
}


void Game::init() {
	model = new Model();
	cube = new Cube();
	character = new Character();
	// star = new Star();
	// fireball = new Fireball();
}



void Game::drawAll() {
	character->draw();
	cube->draw();
	// star->draw();
	// fireball->draw();
	// mushroom->draw();
}