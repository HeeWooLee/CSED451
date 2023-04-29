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
public:
	GLuint ver_loc; /* attribute variable */
	GLuint model_loc; /* uniform variable */
	Game();
	void shaderInit();
	void init();

	GLuint getProgram() { return program; }
	Model* getModel() { return model; }
	Character* getCharacter() { return character; }
};