#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

class NormalMap {
private:
	unsigned w, h;
	GLuint buf;

public:
	bool load(const char* fn);
	void bind();
	GLuint* getBufAddress() { return &buf; }
};