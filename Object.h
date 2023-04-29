#pragma once
#include <stack>
#include <vector>
#include "Angel.h"
using namespace std;
class Object {
private:
	float width, height, depth, name;

protected:
	float x, y;
	GLuint ver_loc, model_loc, program;
	GLuint buf, sizeVer;
	GLuint sizeLineVer, sizeLineCol;
	mat4 transform;
	int sizeDraw, sizeLineDraw;
	int baseSize;

public:
	Object();
	void set(float _x, float _y, float _width, float _height);
	void setSize(vec3);
	void setName(int _name);
	void setY(float _y);
	void setX(float _x);
	void draw_code(GLuint*);
	//virtual void draw() = 0;
	//virtual void setSpeed(float _speed) = 0;
	//virtual float getSpeed() = 0;
	void move(float _x, float _y);
	float getX() { return x; }
	float getY() { return y; }
	float getWidth() { return width; }
	float getHeight() { return height; }
};

class Character : public Object {
private:
	int state;
	stack<mat4> matStack;
	int count, frame;
	int upDown;
	int direction; // 0, -1 -> left, 1 ->right

public:
	Character();
	void draw();
};

class Cube : public Object {
private: 
	vector<GLuint> levels;
	stack<mat4> matStack;
public: 
	Cube();
	void randomLevel();
	void draw();
};

class Star : public Object {
private:
public:
	Star();
	void draw();
};

class Fireball : public Object {
private:
public:
	Fireball();
	void draw();
};


class Mushroom : public Object {
private:
public:
	Mushroom();
	void draw();
};
