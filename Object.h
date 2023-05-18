#pragma once
#include <stack>
#include <vector>
#include <deque>
#include "Angel.h"

using namespace std;

struct Material
{
	vec3 colour;
	int useColour;
	float reflectance;
	Material() {}
	Material(const vec3& colourVal, int useColourVal, float reflectanceVal)
		: colour(colourVal), useColour(useColourVal), reflectance(reflectanceVal) {
	}
};

class Object {
private:
	float width, height, depth;

protected:
	float x, y;
	GLuint ver_loc, tex_loc, normal_loc, tangent_loc, bitangent_loc,textureMode_loc, normalMap_loc, model_loc, color_loc, alpha_loc,program;
	
	Material material;

	/* tex */
	GLint textureSamplerLoc;
	GLint ambientLightLoc;
	GLint specularPowerLoc;
	GLint materialLoc;
	GLint pointLightLoc;
	GLint directionalLightLoc;

	
	GLuint sizeVer;
	GLuint* buf;
	GLuint sizeLineVer, sizeLineCol;
	mat4 transform;
	float alpha;
	stack<mat4> matStack;
	float speed;
	int sizeDraw, sizeLineDraw;
	int baseSize; 
	int normalMapMode = 0;

public:
	Object();
	void set(float _x, float _y, float _width, float _height);
	void setSize(vec3);
	void setY(float _y);
	void setX(float _x);
	void setSpeed(float _speed);
	void initPosition(float, float, vec3);
	void draw_code(GLuint*);
	virtual void draw() = 0;
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
	int state; // 0: walking, 1: jumping, 2: falling, 3: hole
	stack<mat4> matStack;
	int count, frame;
	float jumpLimit, actualLimit;
	bool init;

	int upDown;
	int direction; // 0, -1 -> left, 1 ->right

public:
	Character();
	void draw();
	void frameSelection();
	void setJumping();

	int getState() { return state; }
};

class Mushroom : public Object {
private:
	deque<bool> isDrawn;
public:
	Mushroom(float);
	void draw();
	void randomDrawn();

	void setTransform(mat4);
	stack<mat4>* getMatStack() { return &matStack; }
	deque<bool>* getIsDrawn() { return &isDrawn; }
	mat4 getTransform() { return transform; }
};

class Cube : public Object {
private: 
	float currStart, resetStart;
	deque<GLuint> levels;
	Mushroom* mushroom;
public: 
	Cube();
	void randomLevel();
	void draw();
};

class Star : public Object {
private:
	deque<vec3> starPos;
	float startPos;
public:
	Star();
	void randomGen();
	void draw();

	float getStartPos() { return startPos; }
	deque<vec3>* getStarPos() { return &starPos; }
};

class Fireball : public Object {
private:
	deque<vec3> firePos;
	float startPos;
public:
	Fireball();
	void draw();
	void randomGen();

	float getStartPos() { return startPos; }
	deque<vec3>* getFirePos() { return &firePos; }
};


