#include "Game.h"

Game::Game() {
	scale = vec3(0.001f, 0.001f, 0.001f);
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
	character = new Character();
	cube = new Cube();
	star = new Star();
	fireball = new Fireball();
	mushroom = new Mushroom();
}



void Game::drawAll() {
	character->draw();
	cube->draw();
	star->draw();
	fireball->draw();
	mushroom->draw();
}