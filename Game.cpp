#include "Game.h"

Game::Game() {
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
}