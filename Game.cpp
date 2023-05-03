#include "Game.h"

Game::Game() {
	onGame = true;
	scale = vec3(0.001f, 0.001f, 0.001f);
	speed = -0.0002f;
	baseline = initBaseline = -0.5f;
	score = 0;
}

void Game::shaderInit() {
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	ver_loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(ver_loc);

	color_loc = glGetAttribLocation(program, "aColor");
	glEnableVertexAttribArray(color_loc);

	model_loc = glGetUniformLocation(program, "model_view");
	alpha_loc = glGetUniformLocation(program, "alpha");
}


void Game::init() {
	model = new Model();
	cube = new Cube();
	character = new Character();
	star = new Star();
	fireball = new Fireball();
}



void Game::drawAll() {
	cube->draw();
	character->draw();
	star->draw();
	fireball->draw();
}

void Game::setOnGame(bool _onGame) {
	onGame = _onGame;
}
void Game::addScore(int _score) {
	score += _score;
}