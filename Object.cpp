#include "Object.h"

#include <time.h>
#include "Game.h"
using namespace std;
extern Game* game;

Object::Object() {
	x = 0; y = 0; width = 0; height = 0;
	ver_loc = game->ver_loc;
	model_loc = game->model_loc;
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
	transform = Translate(0.0, game->baseline, 0.0) * transform;
	
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

	mushroom = new Mushroom(resetStart);
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
	float height = getHeight();

	stack<mat4>* mushMatStack = mushroom->getMatStack();
	deque<bool>* mushIsDrawn = mushroom->getIsDrawn();

	/* renew start point of drawing */
	if (currStart == resetStart) {
		matStack.push(transform);

		mushMatStack->push(mushroom->getTransform());
	}
	currStart += speed;
	if (currStart < (resetStart - getWidth())) {
		currStart = resetStart;

		levels.push_back(levels.front());
		levels.pop_front();

		mushIsDrawn->push_back(mushIsDrawn->front());
		mushIsDrawn->pop_front();

		transform = matStack.top();
		matStack.pop();

		mushroom->setTransform(mushMatStack->top());
		mushMatStack->pop();
	}

	/* draw terrain */
	transform = Translate(speed, 0.0f, 0.0f) * transform;
	matStack.push(transform);

	mushroom->setTransform(Translate(speed, 0.0f, 0.0f) * mushroom->getTransform());
	mushMatStack->push(mushroom->getTransform());

	for (size_t i = 0; i < 20; i++) {
		GLuint level = levels[i];
		transform = Translate(width * i, 0.0, 0.0) * matStack.top();
		matStack.push(transform);

		mushroom->setTransform(Translate(width * i, 0.0, 0.0)* mushMatStack->top());
		mushMatStack->push(mushroom->getTransform());

		for (size_t j = 0; j < level; j++) {
			transform = Translate(0.0, height * j, 0.0) * matStack.top();
			draw_code(buf);
		}
		if (level != 0 && mushIsDrawn->at(i)) {
			mushroom->setTransform(Translate(0.0, height * (level - 0.5) + mushroom->getHeight() / 2, 0.0) * mushMatStack->top());
			mushroom->draw();
		}
		matStack.pop();
		mushMatStack->pop();
	}

	transform = matStack.top();
	matStack.pop();

	mushroom->setTransform(mushMatStack->top());
	mushMatStack->pop();
	/* baseline update */
	GLuint xIndex = ceil((game->getCharacter()->getX() - currStart - getWidth()/2) / getWidth()) ;
	game->baseline = game->initBaseline + (levels[xIndex]+1)  * getHeight() + getHeight() / 2;
	if (levels[xIndex] != 0 && mushIsDrawn->at(xIndex)
		&&
		(resetStart - width / 2.0 - mushroom->getWidth() / 2.0) < currStart &&
		(resetStart - width / 2.0 + mushroom->getWidth() / 2.0) > currStart
		)  {
		game->baseline += mushroom->getHeight();
	}
	game->nextBaseline  = game->initBaseline + (levels[xIndex+1]+1)  * getHeight() + getHeight() / 2;
}

/*
 * Star class
 */
Star::Star() {
	buf = game->getModel()->star;
	setSize(game->getModel()->starInfo[1]);
	transform = Translate(-game->getModel()->starInfo[0]) * transform;
}

void Star::draw() {
	draw_code(buf);
}

/*
 * Fireball class
 */
Fireball::Fireball() {
	buf = game->getModel()->fireball;
	setSize(game->getModel()->fireballInfo[1]);
	transform = Translate(-game->getModel()->fireballInfo[0]) * transform;
}

void Fireball::draw() {
	draw_code(buf);
}

/*
 * Mushroom class
 */
Mushroom::Mushroom(float resetStart) {
	buf = game->getModel()->mushroom;
	setSize(game->getModel()->mushroomInfo[1]);
	initPosition(resetStart, game->baseline, game->getModel()->mushroomInfo[0]);

	randomDrawn();
}

void Mushroom::draw() {
	draw_code(buf);
}

void Mushroom::randomDrawn() {
	for (int i = 0; i < 20; i++) {
		bool value = (rand() % 8 == 0);
		isDrawn.push_back(value);
	}
}

void Mushroom::setTransform(mat4 _transform) {
	transform = _transform;
}