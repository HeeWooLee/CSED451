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

void Object::setName(int _name) {
	name = _name;
}

void Object::setY(float _y) {
	y = _y;
}

void Object::setX(float _x) {
	x = _x;
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
	state = 0;
	move(48, 270);
	frame = 20;
	count = 0; upDown = 0; direction = 0;
	transform = Translate(- game->getModel()->characterInfo[0][0]) * transform;
	setSize(game->getModel()->characterInfo[0][1]);
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
	draw_code(buf);
}


Cube::Cube() {
	setSize(game->getModel()->cubeInfo[1]);
	levels = { 1,1,1,2,1, 1, 0,  1, 2,3 };
	randomLevel();
	transform = Translate(- game->getModel()->cubeInfo[0]) * transform;
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

	matStack.push(transform);
	for (size_t i = 0; i < levels.size(); i++) {
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
	while (!matStack.empty()) {
		matStack.pop();
	}

}

Star::Star() {
	setSize(game->getModel()->starInfo[1]);
	transform = Translate(-game->getModel()->starInfo[0]) * transform;
}


void Star::draw() {
	GLuint* buf = game->getModel()->star;
	draw_code(buf);
}


Fireball::Fireball() {
	setSize(game->getModel()->fireballInfo[1]);
	transform = Translate(-game->getModel()->fireballInfo[0]) * transform;
}

void Fireball::draw() {
	GLuint* buf = game->getModel()->fireball;
	draw_code(buf);
}


Mushroom::Mushroom() {
	setSize(game->getModel()->mushroomInfo[1]);
	transform = Translate(-game->getModel()->mushroomInfo[0]) * transform;
}

void Mushroom::draw() {
	GLuint* buf = game->getModel()->mushroom;
	draw_code(buf);
}