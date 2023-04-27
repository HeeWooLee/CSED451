#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Model.h"

class GameObject {
protected: 
	std::vector<glm::fvec3> positions;
	GLfloat width, height, depth;
	GLfloat halfWidth, halfHeight, halfDepth;
	GLfloat moveSpeed;

	Model model;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
public:
	GameObject(const std::string&, GLfloat, GLfloat, GLfloat, GLfloat);
	virtual GLfloat getWidth();
	virtual GLfloat getHeight();
	virtual GLfloat getDepth();
	virtual void init() = 0;
	virtual void draw() = 0;
	virtual void detectCollision(glm::fvec3) = 0;

};

class Star : public GameObject {
private:
	std::vector<GLfloat> transparency;
public:
	Star(const std::string&, GLfloat, GLfloat, GLfloat, GLfloat);
	void draw();
	void init();
	void detectCollision(glm::fvec3);
};