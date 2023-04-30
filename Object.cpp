#include "Object.h"

#include <time.h>
#include "Game.h"
using namespace std;
extern Game* game;


Object::Object() {
	x = 0; y = 0; width = 0; height = 0;
	ver_loc = game->ver_loc;
	model_loc = game->model_loc;
	//projection_loc = game->projection_loc;

	program = game->getProgram();
	transform = Scale(game->scale) * mat4(1.0f);
}

void Object::set(float _x, float _y, float _width, float _height) {
	x = _x; y = _y; width = _width; height = _height;
}

void Object::setSize(vec3 size) {
	width = size.x; height = size.y; depth = size.z;
}

void Object::setY(float _y) {
	y = _y;
}

void Object::setX(float _x) {
	x = _x;
}

void Object::setSpeed(float _speed) {
	speed = _speed;
}

void Object::initPosition(float _x, float _y, vec3 modelCenter) {
	setX(_x);
	setY(_y);
	transform = Translate(getX(),getY(), 0.0) * Translate(-modelCenter) * transform;
}

void Object::draw_code(GLuint* buf) {
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf[1]);
	glVertexAttribPointer(ver_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, transform);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, transform);
	glDrawElements(GL_TRIANGLES, buf[2], GL_UNSIGNED_INT, 0);
}

void Object::move(float _x, float _y) {
	x += _x; y += _y;
}

/*
 * Character class
 */
Character::Character() {
	setSize(game->getModel()->characterInfo[0][1]);
	initPosition(-0.7f, game->baseline + getHeight()/2, game->getModel()->characterInfo[0][0]);

}

void Character::draw() {
	//for (size_t i = 0; i < 3; i++) {
	//	GLuint* buf = game->getModel()->character[i];
	//	draw_code(buf);
	//}
	/*
	* THINGS TO ADD: 
	* - add keyboard jump and frame 
	*/
	GLuint* buf = game->getModel()->character[0];
	matStack.push(transform);
	transform = Translate(0.0, game->prevBaseline, 0.0) * transform;
	
	draw_code(buf);
	transform = matStack.top();
	matStack.pop();
}

/*
 * Cube class
 */
Cube::Cube() {
	setSize(game->getModel()->cubeInfo[1]);
	setSpeed(game->speed);
	currStart = resetStart = getWidth() * -6.0;

	initPosition( resetStart, game->baseline, game->getModel()->cubeInfo[0]);
	game->baseline += getHeight() / 2;

	levels = { 1,1,1,2,1, 1, 0,  1, 2,3 };
	randomLevel();
}

void Cube::randomLevel() {
	srand(time(NULL));
	int num = rand() % 4;
	int prev = num;
	for (int i = 0; i < 200; i++) {
		num = rand() % 4;
		if (num == 3) {
			if (prev == 1) {
				num = rand() % 2 + 1;
			}
		}
		if (num == 0) {
			if (rand() % 4 == 0 || prev == 0) {
				num = rand() % 3 + 1;
			}
		}

		levels.push_back(num);
		if (num == 0) {
			levels.push_back(prev);
		}
		else {
			prev = num;
		}
	}
}

void Cube::draw() {
	GLuint* buf = game->getModel()->cube;
	float width = getWidth();

	/* renew start point of drawing */
	if (currStart == resetStart) {
		matStack.push(transform);
	}
	currStart += speed;
	if (currStart < (resetStart - getWidth())) {
		currStart = resetStart;

		levels.push_back(levels.front());
		levels.pop_front();

		transform = matStack.top();
		matStack.pop();
	}

	/* draw terrain */
	transform = Translate(speed, 0.0f, 0.0f) * transform;
	matStack.push(transform);

	for (size_t i = 0; i < 20; i++) {
		GLuint level = levels[i];
		transform = Translate(width * i, 0.0, 0.0) * matStack.top();
		matStack.push(transform);
		for (size_t j = 0; j < level; j++) {
			transform = Translate(0.0, width * j, 0.0) * matStack.top();
			draw_code(buf);
		}
		matStack.pop();
	}

	transform = matStack.top();
	matStack.pop();

	/* baseline update */
	GLuint xIndex = ceil((game->getCharacter()->getX() - currStart - getWidth()/2) / getWidth());
	game->baseline = game->initBaseline + (levels[xIndex] + 1) * getHeight() + getHeight() / 2;
	game->prevBaseline  = game->initBaseline + (levels[xIndex-1] + 1) * getHeight() + getHeight() / 2;
}

/*
 * Star class
 */
Star::Star() {
	setSize(game->getModel()->starInfo[1]);
	transform = Translate(-game->getModel()->starInfo[0]) * transform;
}

void Star::draw() {
	GLuint* buf = game->getModel()->star;
	draw_code(buf);
}

/*
 * Fireball class
 */
Fireball::Fireball() {
	setSize(game->getModel()->fireballInfo[1]);
	transform = Translate(-game->getModel()->fireballInfo[0]) * transform;
}

void Fireball::draw() {
	GLuint* buf = game->getModel()->fireball;
	draw_code(buf);
}

/*
 * Mushroom class
 */
Mushroom::Mushroom() {
	setSize(game->getModel()->mushroomInfo[1]);
	transform = Translate(-game->getModel()->mushroomInfo[0]) * transform;
}

void Mushroom::draw() {
	GLuint* buf = game->getModel()->mushroom;
	draw_code(buf);
}