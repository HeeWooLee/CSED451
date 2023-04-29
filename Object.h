#pragma once
#include <stack>
#include "Angel.h"
using namespace std;
class Object {
private:
	float width, height, name;

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
	void setSize(float _width, float _height);
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

