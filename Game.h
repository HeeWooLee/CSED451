#pragma once
#include "Angel.h"
#include "Model.h"
#include "Object.h"
using namespace std;

class Game {
private:
	GLuint program;
	Model* model;
	Character* character;
	Cube* cube;
	Star* star;
	Fireball* fireball;
	Mushroom* mushroom;
	bool onGame;

	vec4 light_position;
	vec4 diffuseProduct = vec4(1.0, 1.0, 1.0, 0.0);
	vec4 ambientProduct = vec4(0.2, 0.2, 0.2, 0.0);
	vec4 specularProduct = vec4(0.1, 0.1, 0.1, 0.0);
	float shininess = 0.1;

	int score;
public:
	vec3 scale;
	float speed;
	float baseline, initBaseline, nextBaseline;
	bool isMushroomAhead;

	GLuint ver_loc, color_loc, texture_loc; /* attribute variable */
	GLuint model_loc, alpha_loc; /* uniform variable */
	GLuint projection_loc; /* uniform variable */

	Game();
	void shaderInit();
	void init();
	void drawAll();
	void setOnGame(bool);
	void addScore(int);

	bool getOnGame() { return onGame; }
	int getScore() { return score; }
	GLuint getProgram() { return program; }
	Model* getModel() { return model; }
	Character* getCharacter() { return character; }
	Fireball* getFireball() { return fireball; }
	Star* getStar() { return star; }
};